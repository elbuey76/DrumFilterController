(function bootSimulatorApp() {
  const logic = window.FatSimulatorLogic;
  const scenarios = window.FatSimulatorScenarios || [];

  const slowDefaults = {
    epLavageDelayMs: 30000,
    washMinMs: 45000,
    washMaxMs: 120000,
    residualRotationMs: 10000,
    antiRestartMs: 120000,
    retryPauseMs: 120000
  };
  const levelThresholds = {
    lavage: 55,
    critique: 25
  };
  let config = { ...logic.defaultConfig(), ...slowDefaults };
  let controller = null;
  let inputs = null;
  let outputs = null;
  let status = null;
  let nowMs = 0;
  let playing = true;
  let speed = 1;
  let intervalId = null;
  let previousSnapshot = null;
  let activeScenario = null;
  let outputActiveSince = {};
  let epLavageUiSinceMs = null;
  let transientNotice = null;

  const dom = {
    resetSim: document.getElementById("reset-sim"),
    modeSelector: document.getElementById("mode-selector"),
    modePositions: Array.from(document.querySelectorAll(".selector-position")),
    modeInputs: Array.from(document.querySelectorAll("input[name='mode']")),
    levelSlider: document.getElementById("level-slider"),
    levelFill: document.getElementById("level-fill"),
    levelValue: document.getElementById("level-value"),
    levelEpLavage: document.getElementById("level-ep-lavage"),
    levelEpCritique: document.getElementById("level-ep-critique"),
    levelIncoherent: document.getElementById("level-incoherent"),
    capotOuvert: document.getElementById("capot-ouvert"),
    btnReset: document.getElementById("btn-reset"),
    btnTest: document.getElementById("btn-test"),
    btnTambour: document.getElementById("btn-tambour"),
    btnRincage: document.getElementById("btn-rincage"),
    tempEau: document.getElementById("temp-eau"),
    tempEauValid: document.getElementById("temp-eau-valid"),
    tempLocal: document.getElementById("temp-local"),
    tempLocalValid: document.getElementById("temp-local-valid"),
    cfgEpDelay: document.getElementById("cfg-ep-delay"),
    cfgWashMin: document.getElementById("cfg-wash-min"),
    cfgWashMax: document.getElementById("cfg-wash-max"),
    cfgResidual: document.getElementById("cfg-residual"),
    cfgAntiRestart: document.getElementById("cfg-anti-restart"),
    cfgRetryPause: document.getElementById("cfg-retry-pause"),
    lcdLines: [
      document.getElementById("lcd-line-1"),
      document.getElementById("lcd-line-2"),
      document.getElementById("lcd-line-3"),
      document.getElementById("lcd-line-4")
    ],
    simTime: document.getElementById("sim-time"),
    stateName: document.getElementById("state-name"),
    alarmCode: document.getElementById("alarm-code"),
    playPause: document.getElementById("play-pause"),
    speedSelect: document.getElementById("speed-select"),
    scenarioList: document.getElementById("scenario-list"),
    eventLog: document.getElementById("event-log"),
    clearLog: document.getElementById("clear-log"),
    stepButtons: Array.from(document.querySelectorAll("[data-step-ms]")),
    indicators: Array.from(document.querySelectorAll("[data-indicator]")),
    outputCards: Array.from(document.querySelectorAll("[data-output]"))
  };

  const tempoControls = [
    ["cfgEpDelay", "epLavageDelayMs", "Tempo EP lavage"],
    ["cfgWashMin", "washMinMs", "Lavage mini"],
    ["cfgWashMax", "washMaxMs", "Lavage maxi"],
    ["cfgResidual", "residualRotationMs", "Post-lavage"],
    ["cfgAntiRestart", "antiRestartMs", "Anti-redemarrage"],
    ["cfgRetryPause", "retryPauseMs", "Pause tentative"]
  ];

  function resetSimulation(options = {}) {
    config = readConfigFromControls();
    controller = new logic.Controller(config);
    inputs = logic.defaultInputs();
    outputs = logic.defaultOutputs();
    status = logic.defaultStatus();
    nowMs = 0;
    previousSnapshot = null;
    activeScenario = null;
    outputActiveSince = {};
    epLavageUiSinceMs = null;
    transientNotice = null;
    controller.begin(nowMs);
    tick("Simulation remise a zero");
    syncControlsFromInputs();
    if (!options.keepLog) {
      dom.eventLog.innerHTML = "";
      logEvent("Simulation remise a zero");
    }
    setActiveScenarioButton(null);
  }

  function readConfigFromControls() {
    const nextConfig = { ...logic.defaultConfig() };
    tempoControls.forEach(([controlKey, configKey]) => {
      const seconds = Number(dom[controlKey].value);
      nextConfig[configKey] = Math.max(0, Number.isFinite(seconds) ? seconds : 0) * 1000;
    });

    if (nextConfig.washMaxMs < nextConfig.washMinMs) {
      nextConfig.washMaxMs = nextConfig.washMinMs;
      dom.cfgWashMax.value = String(nextConfig.washMaxMs / 1000);
    }

    return nextConfig;
  }

  function tick(reason) {
    outputs = controller.update(inputs, nowMs);
    status = controller.status();
    recordChanges(reason);
    render();
  }

  function advanceBy(deltaMs) {
    if (deltaMs <= 0) return;

    if (activeScenario) {
      advanceScenario(deltaMs);
      return;
    }

    nowMs += deltaMs;
    tick();
  }

  function advanceScenario(deltaMs) {
    let targetMs = nowMs + deltaMs;

    while (activeScenario && activeScenario.nextStepIndex < activeScenario.steps.length) {
      const nextStep = activeScenario.steps[activeScenario.nextStepIndex];
      const stepAt = activeScenario.startedAtMs + nextStep.atMs;
      if (stepAt > targetMs) break;

      if (stepAt > nowMs) {
        nowMs = stepAt;
        tick();
      }

      applyScenarioStep(nextStep);
      activeScenario.nextStepIndex += 1;
    }

    nowMs = targetMs;
    tick();

    if (activeScenario && nowMs - activeScenario.startedAtMs >= activeScenario.durationMs) {
      logEvent(`Scenario termine: ${activeScenario.title}`);
      activeScenario = null;
      setActiveScenarioButton(null);
    }
  }

  function applyScenarioStep(step) {
    if (step.patch) {
      if (Object.prototype.hasOwnProperty.call(step.patch, "epLavage")) {
        epLavageUiSinceMs = step.patch.epLavage ? nowMs : null;
      }
      Object.assign(inputs, step.patch);
      syncControlsFromInputs();
      logEvent(step.note || "Modification scenario");
      tick();
    }

    if (step.pulse) {
      logEvent(step.note || `Impulsion ${step.pulse}`);
      inputs[step.pulse] = true;
      tick();
      inputs[step.pulse] = false;
    }
  }

  function recordChanges(reason) {
    const nextSnapshot = {
      state: status.state,
      message: status.message,
      alarmCode: status.alarmCode,
      outputs: { ...outputs },
      inputs: { ...inputs }
    };

    if (reason) {
      logEvent(reason);
    }

    if (!previousSnapshot) {
      Object.keys(outputs).forEach((key) => {
        outputActiveSince[key] = outputs[key] ? nowMs : null;
      });
      previousSnapshot = nextSnapshot;
      return;
    }

    if (previousSnapshot.state !== status.state || previousSnapshot.message !== status.message || previousSnapshot.alarmCode !== status.alarmCode) {
      logEvent(`Etat ${status.state} - ${status.message}${status.alarmCode ? ` (${status.alarmCode})` : ""}`);
      captureTransientNotice();
    }

    const outputChanges = Object.keys(outputs)
      .filter((key) => previousSnapshot.outputs[key] !== outputs[key])
      .map((key) => {
        if (outputs[key]) {
          outputActiveSince[key] = nowMs;
          return `${labelForOutput(key)}=ON`;
        }

        const activeSince = outputActiveSince[key];
        const duration = typeof activeSince === "number" ? ` (${formatDuration(nowMs - activeSince)})` : "";
        outputActiveSince[key] = null;
        return `${labelForOutput(key)}=OFF${duration}`;
      });

    if (outputChanges.length > 0) {
      logEvent(`Sorties: ${outputChanges.join(", ")}`);
    }

    previousSnapshot = nextSnapshot;
  }

  function render() {
    const lcdLines = buildLcdLines();
    lcdLines.forEach((line, index) => {
      dom.lcdLines[index].textContent = line;
    });

    dom.simTime.textContent = formatTime(nowMs);
    dom.stateName.textContent = status.state;
    dom.alarmCode.textContent = status.alarmCode || "-";

    dom.playPause.textContent = playing ? "Pause" : "Lecture";
    dom.speedSelect.value = String(speed);

    dom.indicators.forEach((node) => {
      const key = node.getAttribute("data-indicator");
      node.classList.toggle("is-on", Boolean(outputs[key]));
    });

    dom.outputCards.forEach((node) => {
      const key = node.getAttribute("data-output");
      const value = Boolean(outputs[key]);
      node.classList.toggle("is-on", value);
      const strong = node.querySelector("strong");
      if (strong) strong.textContent = logic.boolText(value);
      const small = node.querySelector("small");
      if (small) {
        const activeSince = outputActiveSince[key];
        small.textContent = value && typeof activeSince === "number" ? formatDuration(nowMs - activeSince) : "0 s";
      }
    });

    renderLevelControl(Number(dom.levelSlider.value));
  }

  function buildLcdLines() {
    const mode = logic.modeName(inputs, status.state);
    const level = logic.levelName(inputs);
    const wash = outputs.cmdTambour || outputs.cmdRincage ? "ACTIF" : "REPOS";
    const notice = activeTransientNotice();
    const line3 = notice
      ? `${notice.alarmCode || "INFO"} ${shortAlarmMessage(notice.message)}`
      : status.alarmCode
      ? `${status.alarmCode} ${shortAlarmMessage(status.message)}`
      : `EAU ${tempText(inputs.tempBassinValid, inputs.tempBassinC)} LOC ${tempText(inputs.tempLocalValid, inputs.tempLocalC)}`;

    const line4 = notice ? operatorMessage(notice.message) : pendingWashCountdownText() || operatorMessage();

    return [
      fitLcd(`MODE ${mode}`),
      fitLcd(`NIV ${level} LAV ${wash}`),
      fitLcd(line3),
      fitLcd(line4)
    ];
  }

  function operatorMessage(sourceMessage) {
    const message = sourceMessage || status.message || "";

    if (message === "AUTO - ATTENTE") return "ATTENTE";
    if (message === "AUTO - DEMANDE LAVAGE EN TEMPORISATION") return "DEMANDE LAVAGE";
    if (message === "MAINTENANCE") return "PRET INTERVENTION";
    if (message === "MANUEL") return "AUTO NON ACTIF";
    if (message === "MAINTENANCE - CAPOT OUVERT") return "CAPOT OUVERT";
    if (message === "COMMANDE MANUELLE") return "COMMANDE MANUELLE";
    if (message === "LAVAGE AUTO") return "LAVAGE EN COURS";
    if (message === "POST-LAVAGE - ROTATION RESIDUELLE") return "ROTATION RESIDUELLE";
    if (message === "PAUSE ANTI-REDEMARRAGE") return "PAUSE REDEMARRAGE";
    if (message === "A13 - TEST REFUSE CAPOT") return "TEST REFUSE CAPOT";
    if (message === "A14 - TEST REFUSE SECURITE") return "TEST REFUSE SECURITE";
    if (message === "A05 - RESET REFUSE") return "RESET REFUSE";
    if (message.startsWith("A05 - RESET REFUSE - ")) return message.replace("A05 - ", "");

    return message
      .replace(/^AUTO\s*-\s*/, "")
      .replace(/^MAINTENANCE\s*-\s*/, "")
      .replace(/^A\d{2}\s*-\s*/, "");
  }

  function shortAlarmMessage(message) {
    return String(message || "")
      .replace(/^A\d{2}\s*-\s*/, "")
      .replace(/^RESET REFUSE\s*-\s*/, "RESET ");
  }

  function captureTransientNotice() {
    if (!status.alarmCode) {
      return;
    }

    const transientCodes = ["A05", "A13", "A14"];
    if (!transientCodes.includes(status.alarmCode)) {
      return;
    }

    transientNotice = {
      alarmCode: status.alarmCode,
      message: status.message,
      expiresAtMs: nowMs + 6000
    };
  }

  function activeTransientNotice() {
    if (!transientNotice) {
      return null;
    }

    if (nowMs > transientNotice.expiresAtMs) {
      transientNotice = null;
      return null;
    }

    return transientNotice;
  }

  function pendingWashCountdownText() {
    if (!inputs.epLavage || status.state !== logic.SystemState.AUTO_WAIT || status.message !== "AUTO - DEMANDE LAVAGE EN TEMPORISATION") {
      return "";
    }

    if (typeof epLavageUiSinceMs !== "number") {
      epLavageUiSinceMs = nowMs;
    }

    const remainingMs = Math.max(0, config.epLavageDelayMs - (nowMs - epLavageUiSinceMs));
    return `LAVAGE DANS ${Math.ceil(remainingMs / 1000)}s`;
  }

  function fitLcd(text) {
    const value = String(text || "");
    return value.length > 20 ? value.slice(0, 20) : value.padEnd(20, " ");
  }

  function tempText(valid, value) {
    return valid ? `${Number(value).toFixed(1)}C` : "ABS";
  }

  function formatTime(ms) {
    const minutes = Math.floor(ms / 60000);
    const seconds = Math.floor((ms % 60000) / 1000);
    const millis = Math.floor(ms % 1000);
    return `${String(minutes).padStart(2, "0")}:${String(seconds).padStart(2, "0")}.${String(millis).padStart(3, "0")}`;
  }

  function formatDuration(ms) {
    const totalSeconds = Math.floor(ms / 1000);
    const minutes = Math.floor(totalSeconds / 60);
    const seconds = totalSeconds % 60;
    return minutes > 0 ? `${minutes} min ${seconds} s` : `${seconds} s`;
  }

  function logEvent(message) {
    const item = document.createElement("li");
    const time = document.createElement("time");
    const text = document.createElement("span");
    time.textContent = formatTime(nowMs);
    text.textContent = message;
    item.append(time, text);
    dom.eventLog.prepend(item);

    while (dom.eventLog.children.length > 200) {
      dom.eventLog.lastElementChild.remove();
    }
  }

  function labelForOutput(key) {
    const labels = {
      cmdTambour: "Tambour",
      cmdRincage: "Rincage",
      cmdPompeFiltration: "Filtration",
      cmdPompeDeco: "Deco",
      cmdUv: "UV",
      cmdMiseANiveau: "Mise a niveau",
      voyantMarche: "Voyant marche",
      voyantLavage: "Voyant lavage",
      voyantAlarme: "Voyant alarme"
    };
    return labels[key] || key;
  }

  function syncControlsFromInputs() {
    dom.levelSlider.value = String(levelFromInputs());
    dom.levelIncoherent.checked = inputs.epCritique && !inputs.epLavage;
    dom.capotOuvert.checked = inputs.capotOuvert;
    dom.tempEau.value = inputs.tempBassinC;
    dom.tempEauValid.checked = inputs.tempBassinValid;
    dom.tempLocal.value = inputs.tempLocalC;
    dom.tempLocalValid.checked = inputs.tempLocalValid;

    const mode = inputs.modeMaintenance ? "maintenance" : "auto";
    syncModeControlsFromInputs();
    renderLevelControl(levelFromInputs());
  }

  function syncModeControlsFromInputs() {
    const mode = inputs.modeMaintenance ? "maintenance" : "auto";
    dom.modeSelector.dataset.position = mode;
    dom.modeInputs.forEach((input) => {
      input.checked = input.value === mode;
    });
  }

  function levelFromInputs() {
    if (inputs.epCritique) {
      return 15;
    }

    if (inputs.epLavage) {
      return 45;
    }

    return 100;
  }

  function renderLevelControl(level) {
    const clampedLevel = Math.max(0, Math.min(100, Number(level)));
    dom.levelFill.style.height = `${clampedLevel}%`;
    dom.levelValue.textContent = `${Math.round(clampedLevel)}%`;
    dom.levelEpLavage.textContent = inputs.epLavage ? "EP_LAVAGE ON" : "EP_LAVAGE OFF";
    dom.levelEpCritique.textContent = inputs.epCritique ? "EP_CRITIQUE ON" : "EP_CRITIQUE OFF";
    dom.levelEpLavage.classList.toggle("is-active", inputs.epLavage);
    dom.levelEpCritique.classList.toggle("is-critical", inputs.epCritique);
    dom.levelSlider.closest(".level-control").classList.toggle("is-wash", inputs.epLavage && !inputs.epCritique);
    dom.levelSlider.closest(".level-control").classList.toggle("is-critical", inputs.epCritique);
  }

  function setLevelFromSlider(logChange) {
    const level = Number(dom.levelSlider.value);
    const wasEpLavage = inputs.epLavage;
    dom.levelIncoherent.checked = false;
    inputs.epLavage = level <= levelThresholds.lavage;
    inputs.epCritique = level <= levelThresholds.critique;

    if (inputs.epLavage && !wasEpLavage) {
      epLavageUiSinceMs = nowMs;
    } else if (!inputs.epLavage) {
      epLavageUiSinceMs = null;
    }

    activeScenario = null;
    setActiveScenarioButton(null);
    if (logChange) {
      const label = inputs.epCritique ? "EP_CRITIQUE" : inputs.epLavage ? "EP_LAVAGE" : "OK";
      logEvent(`Niveau eau propre: ${Math.round(level)}% (${label})`);
    }
    tick();
  }

  function setLevelIncoherence(active) {
    if (active) {
      inputs.epLavage = false;
      inputs.epCritique = true;
      epLavageUiSinceMs = null;
      activeScenario = null;
      setActiveScenarioButton(null);
      logEvent("Incoherence niveau: ON");
      tick();
      return;
    }

    logEvent("Incoherence niveau: OFF");
    setLevelFromSlider(false);
  }

  function syncTempoControlsFromConfig() {
    tempoControls.forEach(([controlKey, configKey]) => {
      dom[controlKey].value = String(config[configKey] / 1000);
    });
  }

  function updateInput(key, value, label) {
    inputs[key] = value;
    if (key === "epLavage") {
      epLavageUiSinceMs = value ? nowMs : null;
    }
    activeScenario = null;
    setActiveScenarioButton(null);
    logEvent(`${label}: ${typeof value === "boolean" ? logic.boolText(value) : value}`);
    tick();
  }

  function setMode(mode) {
    if (mode === "auto") {
      inputs.modeAuto = true;
      inputs.modeMaintenance = false;
    } else if (mode === "maintenance") {
      inputs.modeAuto = false;
      inputs.modeMaintenance = true;
    }

    syncModeControlsFromInputs();
    activeScenario = null;
    setActiveScenarioButton(null);
    logEvent(`Mode demande: ${mode.toUpperCase()}`);
    tick();
  }

  function requestMode(mode) {
    const input = dom.modeInputs.find((modeInput) => modeInput.value === mode);
    if (input && !input.checked) {
      input.checked = true;
      setMode(input.value);
    }
  }

  function pulseButton(key, label) {
    inputs[key] = true;
    logEvent(`Impulsion ${label}`);
    tick();
    inputs[key] = false;
  }

  function bindHoldButton(button, key, label) {
    const hold = () => {
      if (inputs[key]) return;
      inputs[key] = true;
      button.classList.add("is-held");
      logEvent(`${label}: ON`);
      tick();
    };

    const release = () => {
      if (!inputs[key]) return;
      inputs[key] = false;
      button.classList.remove("is-held");
      logEvent(`${label}: OFF`);
      tick();
    };

    button.addEventListener("pointerdown", hold);
    button.addEventListener("pointerup", release);
    button.addEventListener("pointerleave", release);
    button.addEventListener("pointercancel", release);
    button.addEventListener("keydown", (event) => {
      if (event.key === " " || event.key === "Enter") hold();
    });
    button.addEventListener("keyup", release);
  }

  function renderScenarios() {
    dom.scenarioList.innerHTML = "";
    scenarios.forEach((scenario) => {
      const button = document.createElement("button");
      button.type = "button";
      button.dataset.scenarioId = scenario.id;
      button.textContent = scenario.title;
      button.addEventListener("click", () => startScenario(scenario));
      dom.scenarioList.append(button);
    });
  }

  function startScenario(scenario) {
    resetSimulation({ keepLog: true });
    activeScenario = {
      ...scenario,
      startedAtMs: nowMs,
      nextStepIndex: 0,
      steps: [...scenario.steps].sort((left, right) => left.atMs - right.atMs)
    };
    playing = true;
    speed = 1;
    logEvent(`Scenario lance: ${scenario.title}`);
    setActiveScenarioButton(scenario.id);
    render();
    advanceBy(1);
  }

  function setActiveScenarioButton(id) {
    dom.scenarioList.querySelectorAll("button").forEach((button) => {
      button.classList.toggle("active", button.dataset.scenarioId === id);
    });
  }

  function bindControls() {
    dom.resetSim.addEventListener("click", () => resetSimulation());

    dom.modeInputs.forEach((input) => {
      input.addEventListener("change", () => {
        if (input.checked) setMode(input.value);
      });
    });
    dom.modePositions.forEach((label) => {
      label.addEventListener("click", (event) => {
        event.preventDefault();
        requestMode(label.dataset.mode);
      });
    });
    dom.modeSelector.querySelector(".selector-body").addEventListener("click", (event) => {
      const rect = event.currentTarget.getBoundingClientRect();
      requestMode(event.clientX < rect.left + rect.width / 2 ? "auto" : "maintenance");
    });

    dom.levelSlider.addEventListener("input", () => setLevelFromSlider(false));
    dom.levelSlider.addEventListener("change", () => setLevelFromSlider(true));
    dom.levelIncoherent.addEventListener("change", () => setLevelIncoherence(dom.levelIncoherent.checked));
    dom.capotOuvert.addEventListener("change", () => updateInput("capotOuvert", dom.capotOuvert.checked, "Capot ouvert"));
    dom.tempEau.addEventListener("change", () => updateInput("tempBassinC", Number(dom.tempEau.value), "Temperature eau"));
    dom.tempEauValid.addEventListener("change", () => updateInput("tempBassinValid", dom.tempEauValid.checked, "Sonde eau presente"));
    dom.tempLocal.addEventListener("change", () => updateInput("tempLocalC", Number(dom.tempLocal.value), "Temperature local"));
    dom.tempLocalValid.addEventListener("change", () => updateInput("tempLocalValid", dom.tempLocalValid.checked, "Sonde local presente"));
    tempoControls.forEach(([controlKey, , label]) => {
      dom[controlKey].addEventListener("change", () => {
        const seconds = Number(dom[controlKey].value);
        logEvent(`${label}: ${Number.isFinite(seconds) ? seconds : 0} s, reset simulation`);
        resetSimulation({ keepLog: true });
      });
    });

    dom.btnReset.addEventListener("click", () => pulseButton("btnReset", "RESET"));
    dom.btnTest.addEventListener("click", () => pulseButton("btnTestLavage", "TEST_LAVAGE"));
    bindHoldButton(dom.btnTambour, "btnManuTambour", "MANU_TAMBOUR");
    bindHoldButton(dom.btnRincage, "btnManuRincage", "MANU_RINCAGE");

    dom.playPause.addEventListener("click", () => {
      playing = !playing;
      render();
    });

    dom.speedSelect.addEventListener("change", () => {
      speed = Number(dom.speedSelect.value);
      logEvent(`Vitesse: x${speed}`);
      render();
    });

    dom.stepButtons.forEach((button) => {
      button.addEventListener("click", () => advanceBy(Number(button.dataset.stepMs)));
    });

    dom.clearLog.addEventListener("click", () => {
      dom.eventLog.innerHTML = "";
    });
  }

  function startClock() {
    if (intervalId) window.clearInterval(intervalId);
    intervalId = window.setInterval(() => {
      if (playing) {
        advanceBy(100 * speed);
      }
    }, 100);
  }

  renderScenarios();
  bindControls();
  syncTempoControlsFromConfig();
  resetSimulation();
  startClock();
})();
