# Validation

Ce dossier contient le plan de tests et les preuves de validation du contrôleur de filtre à tambour.

Le principe retenu est une validation tracée exigence par exigence : chaque exigence fonctionnelle ou de sécurité doit être couverte par un test, une inspection ou une justification explicite de report.

Fichier principal :

- [Plan de tests par exigences](plan-tests-exigences.md)
- [Checklist go/no-go materiel V1](checklist-go-no-go-materiel-v1.md)
- [FT-0001 - Scenarios simulateur firmware V0.1](FT-0001-scenarios-simulateur-firmware-v0.1.md)

## Tests automatises host-side

Les scenarios FT-0001 automatisables sont couverts par un banc de tests PlatformIO `native`, sans carte ESP32 et sans moniteur serie :

```powershell
cd firmware
python -m platformio test -e native
```

Le banc pilote directement `Controller::update()` avec des `InputsSnapshot` et un `nowMs` simule. Il se trouve dans `firmware/test/test_ft0001_controller/`.

## Preuve automatisee courante

La preuve automatisee de reference est le workflow GitHub Actions `Firmware`, defini dans `.github/workflows/firmware.yml`.

Ce workflow est execute a chaque push ou pull request touchant le firmware. Il lance :

```powershell
cd firmware
python -m platformio test -e native
python -m platformio run
```

La CI couvre donc :

- les tests host-side FT-0001 du `Controller` ;
- les tests host-side du parsing des commandes simulateur ;
- les tests host-side du buffer de ligne serie ;
- la compilation du firmware ESP32 `kc868_a32`.

Limite : cette preuve valide le code et la compilation dans l'environnement CI, mais ne remplace pas les essais materiels sur carte KC868-A32, les mesures hydrauliques ni la validation des entrees/sorties physiques.

Les procédures longues ou risquées peuvent être détaillées dans des fiches de test séparées, tout en restant référencées par la matrice de traçabilité.
