(function attachFatSimulatorScenarios(root) {
  const scenarios = [
    {
      id: "boot",
      title: "Demarrage nominal",
      durationMs: 2000,
      steps: [
        { atMs: 0, note: "Etat initial nominal en AUTO." }
      ]
    },
    {
      id: "wash-ok",
      title: "Lavage automatique reussi",
      durationMs: 190000,
      steps: [
        { atMs: 0, patch: { epLavage: true }, note: "EP_LAVAGE passe actif." },
        { atMs: 82000, patch: { epLavage: false }, note: "Le niveau revient normal apres la duree mini." }
      ]
    },
    {
      id: "wash-a04",
      title: "Lavage inefficace A04",
      durationMs: 650000,
      steps: [
        { atMs: 0, patch: { epLavage: true }, note: "EP_LAVAGE reste actif pendant toutes les tentatives." }
      ]
    },
    {
      id: "capot-a03",
      title: "Capot ouvert pendant lavage A03",
      durationMs: 70000,
      steps: [
        { atMs: 0, patch: { epLavage: true }, note: "Demande de lavage." },
        { atMs: 42000, patch: { capotOuvert: true }, note: "Ouverture capot pendant sortie dangereuse." }
      ]
    },
    {
      id: "critical-a01",
      title: "Niveau critique A01",
      durationMs: 5000,
      steps: [
        { atMs: 0, patch: { epLavage: true, epCritique: true }, note: "EP_CRITIQUE actif avec EP_LAVAGE actif." }
      ]
    },
    {
      id: "incoherent-a02",
      title: "Capteurs incoherents A02",
      durationMs: 5000,
      steps: [
        { atMs: 0, patch: { epLavage: false, epCritique: true }, note: "EP_CRITIQUE actif sans EP_LAVAGE." }
      ]
    },
    {
      id: "test-ok",
      title: "Test lavage accepte",
      durationMs: 65000,
      steps: [
        { atMs: 0, pulse: "btnTestLavage", note: "Appui sur TEST_LAVAGE." }
      ]
    },
    {
      id: "test-capot-a13",
      title: "Test refuse capot A13",
      durationMs: 4000,
      steps: [
        { atMs: 0, patch: { capotOuvert: true }, note: "Capot ouvert." },
        { atMs: 500, pulse: "btnTestLavage", note: "Test demande capot ouvert." }
      ]
    },
    {
      id: "test-security-a14",
      title: "Test refuse securite A14",
      durationMs: 5000,
      steps: [
        { atMs: 0, patch: { epCritique: true }, note: "Defaut securite niveau." },
        { atMs: 1500, pulse: "btnTestLavage", note: "Test demande sous securite active." }
      ]
    },
    {
      id: "temp-eau-a11",
      title: "Sonde eau absente A11",
      durationMs: 3000,
      steps: [
        { atMs: 0, patch: { tempBassinValid: false }, note: "Perte sonde temperature eau." }
      ]
    },
    {
      id: "temp-local-a12",
      title: "Sonde local absente A12",
      durationMs: 3000,
      steps: [
        { atMs: 0, patch: { tempLocalValid: false }, note: "Perte sonde temperature local." }
      ]
    }
  ];

  root.FatSimulatorScenarios = scenarios;

  if (typeof module !== "undefined" && module.exports) {
    module.exports = scenarios;
  }
})(typeof globalThis !== "undefined" ? globalThis : this);
