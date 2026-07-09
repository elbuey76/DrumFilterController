const test = require("node:test");
const assert = require("node:assert/strict");

const logic = require("../simulator/logic.js");

function fastConfig() {
  return {
    ...logic.defaultConfig(),
    epLavageDelayMs: 10,
    epCritiqueDebounceMs: 10,
    capotOpenDebounceMs: 10,
    capotCloseStableMs: 10,
    washMinMs: 10,
    washMaxMs: 30,
    residualRotationMs: 10,
    antiRestartMs: 10,
    retryPauseMs: 10,
    capotLongOpenMs: 50
  };
}

class Harness {
  constructor(config = fastConfig()) {
    this.config = config;
    this.controller = new logic.Controller(config);
    this.inputs = logic.defaultInputs();
    this.outputs = logic.defaultOutputs();
    this.nowMs = 0;
    this.controller.begin(this.nowMs);
  }

  tick() {
    this.outputs = this.controller.update(this.inputs, this.nowMs);
    return this.outputs;
  }

  advance(deltaMs) {
    this.nowMs += deltaMs;
    return this.tick();
  }

  status() {
    return this.controller.status();
  }

  pressReset() {
    this.inputs.btnReset = true;
    this.tick();
    this.inputs.btnReset = false;
    return this.outputs;
  }

  pressTest() {
    this.inputs.btnTestLavage = true;
    this.tick();
    this.inputs.btnTestLavage = false;
    return this.outputs;
  }

  startAutoWash() {
    this.inputs.epLavage = true;
    this.tick();
    return this.advance(this.config.epLavageDelayMs);
  }
}

function expectDangerousOutputsOff(outputs) {
  assert.equal(outputs.cmdTambour, false);
  assert.equal(outputs.cmdRincage, false);
}

function expectNominalFiltrationOn(outputs) {
  assert.equal(outputs.cmdPompeFiltration, true);
  assert.equal(outputs.cmdUv, true);
}

function expectLifeSupportOutputsOn(outputs) {
  assert.equal(outputs.cmdPompeFiltration, true);
  assert.equal(outputs.cmdPompeDeco, true);
  assert.equal(outputs.cmdUv, true);
  assert.equal(outputs.cmdMiseANiveau, true);
}

function expectProtectedOutputsOff(outputs) {
  assert.equal(outputs.cmdPompeFiltration, false);
  assert.equal(outputs.cmdPompeDeco, false);
  assert.equal(outputs.cmdUv, false);
  assert.equal(outputs.cmdMiseANiveau, false);
  expectDangerousOutputsOff(outputs);
}

test("default config mirrors firmware timing defaults", () => {
  assert.deepEqual(logic.defaultConfig(), {
    epLavageDelayMs: 10000,
    epCritiqueDebounceMs: 1000,
    capotOpenDebounceMs: 300,
    capotCloseStableMs: 1500,
    capotLongOpenMs: 600000,
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
  });
});

test("boot nominal reaches AUTO_WAIT with nominal outputs", () => {
  const h = new Harness();
  h.tick();

  assert.equal(h.status().state, logic.SystemState.AUTO_WAIT);
  assert.equal(h.status().message, "AUTO - ATTENTE");
  assert.equal(h.status().alarmCode, null);
  expectNominalFiltrationOn(h.outputs);
  expectDangerousOutputsOff(h.outputs);
});

test("auto wash succeeds then returns to AUTO_WAIT", () => {
  const h = new Harness();
  h.tick();
  h.startAutoWash();

  assert.equal(h.status().state, logic.SystemState.WASH_AUTO);
  assert.equal(h.outputs.cmdTambour, true);
  assert.equal(h.outputs.cmdRincage, true);

  h.advance(h.config.washMinMs);
  h.inputs.epLavage = false;
  h.tick();

  assert.equal(h.status().state, logic.SystemState.POST_WASH);
  assert.equal(h.outputs.cmdTambour, true);
  assert.equal(h.outputs.cmdRincage, false);

  h.advance(h.config.residualRotationMs);
  assert.equal(h.status().state, logic.SystemState.SAFETY_PAUSE);
  expectDangerousOutputsOff(h.outputs);

  h.advance(h.config.antiRestartMs);
  assert.equal(h.status().state, logic.SystemState.AUTO_WAIT);
  assert.equal(h.status().message, "AUTO - ATTENTE");
  expectDangerousOutputsOff(h.outputs);
});

test("persistent wash demand retries then degrades with A04", () => {
  const h = new Harness();
  h.tick();
  h.startAutoWash();

  h.advance(h.config.washMaxMs);
  assert.equal(h.status().state, logic.SystemState.RETRY_PAUSE);
  expectDangerousOutputsOff(h.outputs);

  h.advance(h.config.retryPauseMs);
  assert.equal(h.status().state, logic.SystemState.WASH_AUTO);

  h.advance(h.config.washMaxMs);
  assert.equal(h.status().state, logic.SystemState.RETRY_PAUSE);

  h.advance(h.config.retryPauseMs);
  assert.equal(h.status().state, logic.SystemState.WASH_AUTO);

  h.advance(h.config.washMaxMs);
  assert.equal(h.status().state, logic.SystemState.DEGRADED);
  assert.equal(h.status().alarmCode, "A04");
  expectDangerousOutputsOff(h.outputs);
  assert.equal(h.outputs.voyantAlarme, true);
});

test("capot opening during wash latches A03 and blocks reset while open", () => {
  const h = new Harness();
  h.tick();
  h.startAutoWash();

  h.inputs.capotOuvert = true;
  h.tick();

  assert.equal(h.status().state, logic.SystemState.FAULT);
  assert.equal(h.status().alarmCode, "A03");
  expectDangerousOutputsOff(h.outputs);
  expectLifeSupportOutputsOn(h.outputs);

  h.pressReset();
  assert.equal(h.status().state, logic.SystemState.FAULT);
  assert.equal(h.status().alarmCode, "A05");
  expectDangerousOutputsOff(h.outputs);
  expectLifeSupportOutputsOn(h.outputs);

  h.inputs.capotOuvert = false;
  h.tick();
  h.advance(h.config.capotCloseStableMs);
  h.pressTest();
  assert.equal(h.status().state, logic.SystemState.FAULT);
  assert.equal(h.status().alarmCode, "A14");
  expectDangerousOutputsOff(h.outputs);
  expectLifeSupportOutputsOn(h.outputs);
});

test("critical level and incoherent level latch A01/A02", () => {
  const critical = new Harness();
  critical.tick();
  critical.inputs.epLavage = true;
  critical.inputs.epCritique = true;
  critical.tick();
  critical.advance(critical.config.epCritiqueDebounceMs);

  assert.equal(critical.status().state, logic.SystemState.FAULT);
  assert.equal(critical.status().alarmCode, "A01");
  expectProtectedOutputsOff(critical.outputs);

  const incoherent = new Harness();
  incoherent.tick();
  incoherent.inputs.epCritique = true;
  incoherent.tick();
  incoherent.advance(incoherent.config.epCritiqueDebounceMs);

  assert.equal(incoherent.status().state, logic.SystemState.FAULT);
  assert.equal(incoherent.status().alarmCode, "A02");
  expectProtectedOutputsOff(incoherent.outputs);
});

test("test wash accepted returns success message", () => {
  const h = new Harness();
  h.tick();
  h.pressTest();

  assert.equal(h.status().state, logic.SystemState.TEST_WASH);
  assert.equal(h.outputs.cmdTambour, true);
  assert.equal(h.outputs.cmdRincage, true);

  h.advance(h.config.washMinMs);

  assert.equal(h.status().state, logic.SystemState.AUTO_WAIT);
  assert.equal(h.status().message, "TEST OK - CYCLE EXECUTE");
  assert.equal(h.status().alarmCode, null);
  expectDangerousOutputsOff(h.outputs);
});

test("repeated test press during active test is ignored without transient alarm", () => {
  const h = new Harness();
  h.tick();
  h.pressTest();

  h.pressTest();

  assert.equal(h.status().state, logic.SystemState.TEST_WASH);
  assert.equal(h.status().message, "TEST LAVAGE");
  assert.equal(h.status().alarmCode, null);
  assert.equal(h.outputs.cmdTambour, true);
  assert.equal(h.outputs.cmdRincage, true);
  assert.equal(h.outputs.voyantAlarme, false);
});

test("test refused with capot open or active safety", () => {
  const capot = new Harness();
  capot.tick();
  capot.inputs.capotOuvert = true;
  capot.pressTest();

  assert.equal(capot.status().state, logic.SystemState.MAINTENANCE);
  assert.equal(capot.status().alarmCode, "A13");
  expectDangerousOutputsOff(capot.outputs);
  expectLifeSupportOutputsOn(capot.outputs);
  assert.equal(capot.outputs.voyantAlarme, false);

  const safety = new Harness();
  safety.tick();
  safety.inputs.epCritique = true;
  safety.tick();
  safety.advance(safety.config.epCritiqueDebounceMs);
  safety.pressTest();

  assert.equal(safety.status().state, logic.SystemState.FAULT);
  assert.equal(safety.status().alarmCode, "A14");
  expectProtectedOutputsOff(safety.outputs);
});

test("manual commands run only while maintained", () => {
  const h = new Harness();
  h.tick();
  h.inputs.modeAuto = false;
  h.inputs.modeMaintenance = true;
  h.inputs.btnManuTambour = true;
  h.tick();

  assert.equal(h.status().state, logic.SystemState.MANUAL);
  assert.equal(h.status().message, "COMMANDE MANUELLE");
  assert.equal(h.outputs.cmdTambour, true);
  assert.equal(h.outputs.cmdRincage, false);

  h.inputs.btnManuTambour = false;
  h.tick();
  assert.equal(h.status().state, logic.SystemState.MAINTENANCE);
  expectDangerousOutputsOff(h.outputs);
});

test("missing temperature probes warn without stopping nominal outputs", () => {
  const water = new Harness();
  water.tick();
  water.inputs.tempBassinValid = false;
  water.tick();

  assert.equal(water.status().state, logic.SystemState.AUTO_WAIT);
  assert.equal(water.status().alarmCode, "A11");
  expectNominalFiltrationOn(water.outputs);
  expectDangerousOutputsOff(water.outputs);
  assert.equal(water.outputs.voyantAlarme, true);

  const local = new Harness();
  local.tick();
  local.inputs.tempLocalValid = false;
  local.tick();

  assert.equal(local.status().state, logic.SystemState.AUTO_WAIT);
  assert.equal(local.status().alarmCode, "A12");
  expectNominalFiltrationOn(local.outputs);
  expectDangerousOutputsOff(local.outputs);
  assert.equal(local.outputs.voyantAlarme, true);
});
