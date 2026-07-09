(function attachFatSimulatorLogic(root) {
  const SystemState = Object.freeze({
    BOOT: "BOOT",
    AUTO_WAIT: "AUTO_WAIT",
    WASH_AUTO: "WASH_AUTO",
    POST_WASH: "POST_WASH",
    SAFETY_PAUSE: "SAFETY_PAUSE",
    RETRY_PAUSE: "RETRY_PAUSE",
    MANUAL: "MANUAL",
    MAINTENANCE: "MAINTENANCE",
    TEST_WASH: "TEST_WASH",
    DEGRADED: "DEGRADED",
    FAULT: "FAULT"
  });

  const AlarmCode = Object.freeze({
    NONE: "NONE",
    A01: "A01",
    A02: "A02",
    A03: "A03",
    A04: "A04",
    A05: "A05",
    A11: "A11",
    A12: "A12",
    A13: "A13",
    A14: "A14",
    A15: "A15"
  });

  const PersistentEventCode = Object.freeze({
    NONE: "NONE",
    BOOT: "BOOT",
    A01: "A01",
    A02: "A02",
    A03: "A03",
    A04: "A04",
    A15: "A15",
    RESET_OK: "RESET_OK"
  });

  const alarmDefinitions = Object.freeze({
    NONE: { code: null, message: "", severity: "INFO" },
    A01: { code: "A01", message: "A01 - NIVEAU CRITIQUE", severity: "BLOCKING" },
    A02: { code: "A02", message: "A02 - CAPTEURS NIVEAU INCOHERENTS", severity: "BLOCKING" },
    A03: { code: "A03", message: "A03 - CAPOT OUVERT DANGER", severity: "BLOCKING" },
    A04: { code: "A04", message: "A04 - LAVAGE INEFFICACE", severity: "DEGRADED" },
    A05: { code: "A05", message: "A05 - RESET REFUSE", severity: "WARNING" },
    A11: { code: "A11", message: "A11 - SONDE EAU ABSENTE", severity: "WARNING" },
    A12: { code: "A12", message: "A12 - SONDE LOCAL ABSENTE", severity: "WARNING" },
    A13: { code: "A13", message: "A13 - TEST REFUSE CAPOT", severity: "WARNING" },
    A14: { code: "A14", message: "A14 - TEST REFUSE SECURITE", severity: "WARNING" },
    A15: { code: "A15", message: "A15 - CAPOT OUVERT LONG", severity: "WARNING" }
  });

  function defaultConfig() {
    return {
      epLavageDelayMs: 10000,
      epCritiqueDebounceMs: 1000,
      capotOpenDebounceMs: 300,
      capotCloseStableMs: 1500,
      capotLongOpenMs: 10 * 60 * 1000,
      washMinMs: 10000,
      washMaxMs: 45000,
      residualRotationMs: 3000,
      antiRestartMs: 60000,
      retryPauseMs: 60000,
      maxWashAttempts: 3,
      tempBassinLowC: 4,
      tempBassinHighC: 28,
      tempLocalLowC: 2,
      tempLocalHighC: 40
    };
  }

  function defaultInputs() {
    return {
      epLavage: false,
      epCritique: false,
      capotOuvert: false,
      modeAuto: true,
      modeMaintenance: false,
      btnReset: false,
      btnTestLavage: false,
      btnManuTambour: false,
      btnManuRincage: false,
      tempBassinValid: true,
      tempLocalValid: true,
      tempBassinC: 20,
      tempLocalC: 20
    };
  }

  function defaultOutputs() {
    return {
      cmdTambour: false,
      cmdRincage: false,
      cmdPompeFiltration: false,
      cmdPompeDeco: false,
      cmdUv: false,
      cmdMiseANiveau: false,
      voyantMarche: false,
      voyantLavage: false,
      voyantAlarme: false
    };
  }

  function defaultStatus() {
    return {
      state: SystemState.BOOT,
      message: "BOOT",
      alarmCode: null
    };
  }

  function defaultJournal() {
    return {
      totalEvents: 0,
      lastEvent: PersistentEventCode.NONE,
      a15Active: false,
      eventCounts: {
        BOOT: 0,
        A01: 0,
        A02: 0,
        A03: 0,
        A04: 0,
        A15: 0,
        RESET_OK: 0
      }
    };
  }

  function clone(value) {
    return JSON.parse(JSON.stringify(value));
  }

  class Safety {
    constructor(config) {
      this.config = config;
      this.capotInitialized = false;
      this.capotStableOpen = false;
      this.capotCandidateOpen = false;
      this.capotCandidateSinceMs = 0;
      this.capotStableOpenSinceMs = 0;
      this.criticalInitialized = false;
      this.criticalStable = false;
      this.criticalCandidate = false;
      this.criticalCandidateSinceMs = 0;
    }

    update(inputs, nowMs) {
      if (!this.capotInitialized) {
        this.capotInitialized = true;
        this.capotStableOpen = inputs.capotOuvert;
        this.capotCandidateOpen = inputs.capotOuvert;
        this.capotCandidateSinceMs = nowMs;
        this.capotStableOpenSinceMs = inputs.capotOuvert ? nowMs : 0;
      }

      if (!this.criticalInitialized) {
        this.criticalInitialized = true;
        this.criticalStable = inputs.epCritique;
        this.criticalCandidate = inputs.epCritique;
        this.criticalCandidateSinceMs = nowMs;
      }

      const capotDelayMs = inputs.capotOuvert ? this.config.capotOpenDebounceMs : this.config.capotCloseStableMs;
      const capotChanged = this.updateDebounced(
        inputs.capotOuvert,
        "capotStableOpen",
        "capotCandidateOpen",
        "capotCandidateSinceMs",
        capotDelayMs,
        nowMs
      );

      if (capotChanged && this.capotStableOpen) {
        this.capotStableOpenSinceMs = nowMs;
      }

      this.updateDebounced(
        inputs.epCritique,
        "criticalStable",
        "criticalCandidate",
        "criticalCandidateSinceMs",
        this.config.epCritiqueDebounceMs,
        nowMs
      );

      return {
        capotOpen: this.capotStableOpen,
        capotOpenLong: this.capotStableOpen && nowMs - this.capotStableOpenSinceMs >= this.config.capotLongOpenMs,
        levelCritical: this.criticalStable,
        levelIncoherent: this.criticalStable && !inputs.epLavage
      };
    }

    updateDebounced(rawValue, stableKey, candidateKey, candidateSinceKey, delayMs, nowMs) {
      if (rawValue !== this[candidateKey]) {
        this[candidateKey] = rawValue;
        this[candidateSinceKey] = nowMs;
      }

      if (this[stableKey] !== this[candidateKey] && nowMs - this[candidateSinceKey] >= delayMs) {
        this[stableKey] = this[candidateKey];
        return true;
      }

      return false;
    }
  }

  class WashCycle {
    constructor(config) {
      this.config = config;
      this.phase = "Idle";
      this.phaseStartedAtMs = 0;
      this.epLavageActiveSinceMs = 0;
      this.testResultMessage = null;
      this.epLavageWasActive = false;
      this.testStartedWithEpLavage = false;
      this.attemptCount = 0;
    }

    update(inputs, nowMs) {
      const result = {
        state: SystemState.AUTO_WAIT,
        message: "AUTO - ATTENTE",
        alarmCode: AlarmCode.NONE,
        cmdTambour: false,
        cmdRincage: false,
        voyantLavage: false,
        voyantAlarme: false
      };

      if (this.phase === "Idle") {
        if (!this.lavageConfirmed(inputs, nowMs)) {
          result.message = inputs.epLavage ? "AUTO - DEMANDE LAVAGE EN TEMPORISATION" : "AUTO - ATTENTE";
          return result;
        }

        this.startWash(nowMs);
      }

      for (;;) {
        switch (this.phase) {
          case "Washing":
            if (nowMs - this.phaseStartedAtMs >= this.config.washMaxMs) {
              if (this.attemptCount >= this.config.maxWashAttempts) {
                this.phase = "Degraded";
              } else {
                this.phase = "RetryPause";
              }
              this.phaseStartedAtMs = nowMs;
              continue;
            }

            if (nowMs - this.phaseStartedAtMs >= this.config.washMinMs && !inputs.epLavage) {
              this.phase = "PostWash";
              this.phaseStartedAtMs = nowMs;
              continue;
            }

            result.state = SystemState.WASH_AUTO;
            result.message = "LAVAGE AUTO";
            result.cmdTambour = true;
            result.cmdRincage = true;
            result.voyantLavage = true;
            return result;

          case "PostWash":
            if (nowMs - this.phaseStartedAtMs >= this.config.residualRotationMs) {
              this.phase = "SafetyPause";
              this.phaseStartedAtMs = nowMs;
              continue;
            }

            result.state = SystemState.POST_WASH;
            result.message = "POST-LAVAGE - ROTATION RESIDUELLE";
            result.cmdTambour = true;
            result.voyantLavage = true;
            return result;

          case "SafetyPause":
            if (nowMs - this.phaseStartedAtMs >= this.config.antiRestartMs) {
              this.phase = "Idle";
              this.attemptCount = 0;
              this.epLavageWasActive = false;
              continue;
            }

            result.state = SystemState.SAFETY_PAUSE;
            result.message = "PAUSE ANTI-REDEMARRAGE";
            return result;

          case "RetryPause":
            if (nowMs - this.phaseStartedAtMs >= this.config.retryPauseMs) {
              if (inputs.epLavage) {
                this.startWash(nowMs);
              } else {
                this.phase = "Idle";
                this.attemptCount = 0;
                this.epLavageWasActive = false;
              }
              continue;
            }

            result.state = SystemState.RETRY_PAUSE;
            result.message = "LAVAGE INEFFICACE - PAUSE AVANT RETENTATIVE";
            return result;

          case "TestWashing":
            if (nowMs - this.phaseStartedAtMs >= this.config.washMinMs) {
              this.phase = "TestDone";
              this.phaseStartedAtMs = nowMs;
              if (!this.testStartedWithEpLavage) {
                this.testResultMessage = "TEST OK - CYCLE EXECUTE";
              } else if (!inputs.epLavage) {
                this.testResultMessage = "TEST OK - NIVEAU OK";
              } else {
                this.testResultMessage = "TEST ECHEC - LAVAGE INEFFICACE";
              }
              continue;
            }

            result.state = SystemState.TEST_WASH;
            result.message = "TEST LAVAGE";
            result.cmdTambour = true;
            result.cmdRincage = true;
            result.voyantLavage = true;
            return result;

          case "TestDone":
            if (nowMs - this.phaseStartedAtMs >= this.config.residualRotationMs) {
              this.phase = "Idle";
              this.testStartedWithEpLavage = false;
              this.testResultMessage = null;
              continue;
            }

            result.state = SystemState.AUTO_WAIT;
            result.message = this.testResultMessage || "TEST TERMINE";
            return result;

          case "Degraded":
            result.state = SystemState.DEGRADED;
            result.message = "A04 - LAVAGE INEFFICACE";
            result.alarmCode = AlarmCode.A04;
            result.voyantAlarme = true;
            return result;

          case "Idle":
          default:
            result.message = inputs.epLavage ? "AUTO - DEMANDE LAVAGE EN TEMPORISATION" : "AUTO - ATTENTE";
            return result;
        }
      }
    }

    startTest(epLavageActiveAtStart, nowMs) {
      if (this.phase !== "Idle") {
        return false;
      }

      this.phase = "TestWashing";
      this.phaseStartedAtMs = nowMs;
      this.testStartedWithEpLavage = epLavageActiveAtStart;
      this.epLavageWasActive = false;
      return true;
    }

    abort() {
      if (this.phase !== "Degraded") {
        this.phase = "Idle";
        this.attemptCount = 0;
        this.epLavageWasActive = false;
        this.testStartedWithEpLavage = false;
        this.testResultMessage = null;
      }
    }

    resetAlarm() {
      this.phase = "Idle";
      this.attemptCount = 0;
      this.epLavageWasActive = false;
      this.testStartedWithEpLavage = false;
      this.testResultMessage = null;
    }

    hasBlockingAlarm() {
      return this.phase === "Degraded";
    }

    isTestRunning() {
      return this.phase === "TestWashing";
    }

    isTestActive() {
      return this.phase === "TestWashing" || this.phase === "TestDone";
    }

    startWash(nowMs) {
      this.phase = "Washing";
      this.phaseStartedAtMs = nowMs;
      this.attemptCount += 1;
      this.epLavageWasActive = false;
    }

    lavageConfirmed(inputs, nowMs) {
      if (!inputs.epLavage) {
        this.epLavageWasActive = false;
        return false;
      }

      if (!this.epLavageWasActive) {
        this.epLavageWasActive = true;
        this.epLavageActiveSinceMs = nowMs;
      }

      return nowMs - this.epLavageActiveSinceMs >= this.config.epLavageDelayMs;
    }
  }

  class Controller {
    constructor(config) {
      this.config = { ...defaultConfig(), ...(config || {}) };
      this.safety = new Safety(this.config);
      this.washCycle = new WashCycle(this.config);
      this.journal = defaultJournal();
      this.blockingAlarmLatched = AlarmCode.NONE;
      this.washAlarmPersisted = false;
      this.dangerousOutputWasActive = false;
      this.state = SystemState.BOOT;
      this.currentStatus = defaultStatus();
    }

    begin() {
      this.recordPersistentEvent(PersistentEventCode.BOOT);
      this.setStatus(SystemState.BOOT, "BOOT", null);
    }

    update(inputs, nowMs) {
      const safety = this.safety.update(inputs, nowMs);

      if (!this.capotOpen(inputs, safety) && this.journal.a15Active) {
        this.setPersistentA15Active(false);
      }

      if (this.capotOpen(inputs, safety) && this.dangerousOutputWasActive) {
        this.latchBlockingAlarm(AlarmCode.A03);
      }

      if (inputs.btnReset) {
        if (!this.resetAllowed(inputs, safety)) {
          this.setAlarmStatus(SystemState.FAULT, AlarmCode.A05, this.resetRefusalMessage(inputs, safety));
          return this.finishUpdate(
            this.blockingAlarmLatched === AlarmCode.A03 && !safety.levelCritical && !safety.levelIncoherent ? this.capotDangerOutputs() : this.safeOutputs()
          );
        }

        const hadBlockingAlarm = this.blockingAlarmLatched !== AlarmCode.NONE || this.washCycle.hasBlockingAlarm();
        this.blockingAlarmLatched = AlarmCode.NONE;
        this.washAlarmPersisted = false;
        this.washCycle.resetAlarm();
        if (hadBlockingAlarm) {
          this.recordPersistentEvent(PersistentEventCode.RESET_OK);
        }
      }

      if (inputs.btnTestLavage) {
        if (this.capotOpen(inputs, safety)) {
          this.setAlarmStatus(SystemState.MAINTENANCE, AlarmCode.A13);
          return this.finishUpdate(this.testRefusedOutputs(false));
        }

        if (safety.levelCritical || safety.levelIncoherent || this.blockingAlarmLatched !== AlarmCode.NONE || (!inputs.modeAuto && !inputs.modeMaintenance)) {
          this.setAlarmStatus(SystemState.FAULT, AlarmCode.A14);
          return this.finishUpdate(
            this.blockingAlarmLatched === AlarmCode.A03 && !safety.levelCritical && !safety.levelIncoherent ? this.capotDangerOutputs() : this.testRefusedOutputs(true)
          );
        }

        if (!this.washCycle.isTestActive() && !this.washCycle.startTest(inputs.epLavage, nowMs)) {
          this.setAlarmStatus(SystemState.FAULT, AlarmCode.A14, "A14 - TEST REFUSE CYCLE ACTIF");
          return this.finishUpdate(this.testRefusedOutputs(true));
        }
      }

      if (safety.levelIncoherent) {
        this.latchBlockingAlarm(AlarmCode.A02);
      }

      if (!safety.levelIncoherent && safety.levelCritical) {
        this.latchBlockingAlarm(AlarmCode.A01);
      }

      if (this.blockingAlarmLatched !== AlarmCode.NONE) {
        this.washCycle.abort();
        this.setAlarmStatus(SystemState.FAULT, this.blockingAlarmLatched);
        return this.finishUpdate(this.blockingAlarmLatched === AlarmCode.A03 ? this.capotDangerOutputs() : this.safeOutputs());
      }

      if (this.washCycle.hasBlockingAlarm()) {
        if (!this.washAlarmPersisted) {
          this.recordPersistentEvent(PersistentEventCode.A04);
          this.washAlarmPersisted = true;
        }

        const degraded = {
          state: SystemState.DEGRADED,
          message: alarmDefinitions.A04.message,
          alarmCode: AlarmCode.A04,
          voyantAlarme: true,
          cmdTambour: false,
          cmdRincage: false,
          voyantLavage: false
        };
        this.setAlarmStatus(SystemState.DEGRADED, AlarmCode.A04);
        return this.finishUpdate(this.washOutputs(degraded));
      }

      if (this.capotOpen(inputs, safety)) {
        this.washCycle.abort();
        const outputs = this.maintenanceOutputs();
        if (safety.capotOpenLong || this.journal.a15Active) {
          if (!this.journal.a15Active) {
            this.recordPersistentEvent(PersistentEventCode.A15);
            this.setPersistentA15Active(true);
          }
          outputs.voyantAlarme = true;
          this.setAlarmStatus(SystemState.MAINTENANCE, AlarmCode.A15);
        } else if (inputs.btnManuTambour || inputs.btnManuRincage) {
          this.setStatus(SystemState.MAINTENANCE, "MANU REFUSE - CAPOT", null);
        } else {
          this.setStatus(SystemState.MAINTENANCE, "MAINTENANCE - CAPOT OUVERT", null);
        }
        return this.finishUpdate(outputs);
      }

      if ((inputs.modeMaintenance || !inputs.modeAuto) && !this.washCycle.isTestActive()) {
        this.washCycle.abort();

        if (inputs.btnManuTambour || inputs.btnManuRincage) {
          this.setStatus(SystemState.MANUAL, "COMMANDE MANUELLE", null);
          return this.finishUpdate(this.manualOutputs(inputs));
        }

        this.setStatus(
          inputs.modeMaintenance ? SystemState.MAINTENANCE : SystemState.MANUAL,
          inputs.modeMaintenance ? "MAINTENANCE" : "MANUEL",
          null
        );
        return this.finishUpdate(this.maintenanceOutputs());
      }

      const wash = this.washCycle.update(inputs, nowMs);
      const outputs = this.washOutputs(wash);
      if (wash.alarmCode !== AlarmCode.NONE) {
        this.setAlarmStatus(wash.state, wash.alarmCode, wash.message);
        return this.finishUpdate(outputs);
      }

      const tempAlarm = this.temperatureAlarm(inputs);
      if (tempAlarm !== AlarmCode.NONE) {
        outputs.voyantAlarme = true;
        this.setAlarmStatus(wash.state, tempAlarm);
        return this.finishUpdate(outputs);
      }

      this.setStatus(wash.state, wash.message, null);
      return this.finishUpdate(outputs);
    }

    status() {
      return { ...this.currentStatus };
    }

    journalSnapshot() {
      return clone(this.journal);
    }

    safeOutputs() {
      const outputs = defaultOutputs();
      outputs.voyantAlarme = true;
      return outputs;
    }

    capotDangerOutputs() {
      const outputs = this.nominalOutputs();
      outputs.cmdTambour = false;
      outputs.cmdRincage = false;
      outputs.voyantLavage = false;
      outputs.voyantAlarme = true;
      return outputs;
    }

    nominalOutputs() {
      const outputs = defaultOutputs();
      outputs.cmdPompeFiltration = true;
      outputs.cmdPompeDeco = true;
      outputs.cmdUv = true;
      outputs.cmdMiseANiveau = true;
      outputs.voyantMarche = true;
      return outputs;
    }

    maintenanceOutputs() {
      const outputs = this.nominalOutputs();
      outputs.cmdTambour = false;
      outputs.cmdRincage = false;
      outputs.voyantLavage = false;
      return outputs;
    }

    manualOutputs(inputs) {
      const outputs = this.nominalOutputs();
      outputs.cmdTambour = inputs.btnManuTambour;
      outputs.cmdRincage = inputs.btnManuRincage;
      outputs.voyantLavage = outputs.cmdTambour || outputs.cmdRincage;
      return outputs;
    }

    washOutputs(wash) {
      const outputs = this.nominalOutputs();
      outputs.cmdTambour = Boolean(wash.cmdTambour);
      outputs.cmdRincage = Boolean(wash.cmdRincage);
      outputs.voyantLavage = Boolean(wash.voyantLavage);
      outputs.voyantAlarme = Boolean(wash.voyantAlarme);
      return outputs;
    }

    resetAllowed(inputs, safety) {
      if (safety.levelCritical || safety.levelIncoherent || (this.blockingAlarmLatched === AlarmCode.A03 && this.capotOpen(inputs, safety))) {
        return false;
      }

      if (this.washCycle.hasBlockingAlarm() && inputs.epLavage) {
        return false;
      }

      return true;
    }

    resetRefusalMessage(inputs, safety) {
      if (safety.levelIncoherent) {
        return "A05 - RESET REFUSE - CAPTEURS NIVEAU";
      }

      if (safety.levelCritical) {
        return "A05 - RESET REFUSE - EP_CRITIQUE ACTIF";
      }

      if (this.blockingAlarmLatched === AlarmCode.A03 && this.capotOpen(inputs, safety)) {
        return "A05 - RESET REFUSE - CAPOT OUVERT";
      }

      if (this.washCycle.hasBlockingAlarm() && inputs.epLavage) {
        return "A05 - RESET REFUSE - EP_LAVAGE ACTIF";
      }

      return "A05 - RESET REFUSE";
    }

    temperatureAlarm(inputs) {
      if (!inputs.tempBassinValid) {
        return AlarmCode.A11;
      }

      if (!inputs.tempLocalValid) {
        return AlarmCode.A12;
      }

      return AlarmCode.NONE;
    }

    testRefusedOutputs(blocking) {
      const outputs = blocking ? this.safeOutputs() : this.maintenanceOutputs();
      outputs.voyantAlarme = blocking;
      return outputs;
    }

    capotOpen(inputs, safety) {
      return inputs.capotOuvert || safety.capotOpen;
    }

    recordPersistentEvent(eventCode) {
      if (!eventCode || eventCode === PersistentEventCode.NONE || !(eventCode in this.journal.eventCounts)) {
        return;
      }

      this.journal.eventCounts[eventCode] += 1;
      this.journal.totalEvents += 1;
      this.journal.lastEvent = eventCode;
    }

    setPersistentA15Active(active) {
      this.journal.a15Active = active;
    }

    latchBlockingAlarm(alarmCode) {
      if (this.blockingAlarmLatched === alarmCode) {
        return;
      }

      this.blockingAlarmLatched = alarmCode;
      this.recordPersistentEvent(this.persistentEventForAlarm(alarmCode));
    }

    persistentEventForAlarm(alarmCode) {
      switch (alarmCode) {
        case AlarmCode.A01:
          return PersistentEventCode.A01;
        case AlarmCode.A02:
          return PersistentEventCode.A02;
        case AlarmCode.A03:
          return PersistentEventCode.A03;
        case AlarmCode.A04:
          return PersistentEventCode.A04;
        case AlarmCode.A15:
          return PersistentEventCode.A15;
        default:
          return PersistentEventCode.NONE;
      }
    }

    setStatus(state, message, alarmCode) {
      this.state = state;
      this.currentStatus = {
        state,
        message,
        alarmCode
      };
    }

    setAlarmStatus(state, alarmCode, messageOverride) {
      const definition = alarmDefinitions[alarmCode] || alarmDefinitions.NONE;
      this.setStatus(state, messageOverride || definition.message, definition.code);
    }

    finishUpdate(outputs) {
      this.dangerousOutputWasActive = outputs.cmdTambour || outputs.cmdRincage;
      return { ...outputs };
    }
  }

  function modeName(inputs, state) {
    if (state === SystemState.FAULT) return "DEFAUT";
    if (state === SystemState.DEGRADED) return "DEGRADE";
    if (inputs.modeMaintenance) return "MAINTENANCE";
    return inputs.modeAuto ? "AUTO" : "MANUEL";
  }

  function levelName(inputs) {
    if (inputs.epCritique && !inputs.epLavage) return "INCOHERENT";
    if (inputs.epCritique) return "CRITIQUE";
    return inputs.epLavage ? "LAVAGE" : "OK";
  }

  function boolText(value) {
    return value ? "ON" : "OFF";
  }

  const api = {
    SystemState,
    AlarmCode,
    PersistentEventCode,
    alarmDefinitions,
    defaultConfig,
    defaultInputs,
    defaultOutputs,
    defaultStatus,
    defaultJournal,
    Controller,
    Safety,
    WashCycle,
    modeName,
    levelName,
    boolText
  };

  root.FatSimulatorLogic = api;

  if (typeof module !== "undefined" && module.exports) {
    module.exports = api;
  }
})(typeof globalThis !== "undefined" ? globalThis : this);
