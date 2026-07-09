# FT-0001 - Scenarios simulateur firmware V0.1

## Objectif

Valider les scenarios obligatoires du firmware MVP V0.1 avec le moniteur serie PlatformIO, sans materiel KC868-A32 raccorde.

## Version testee

- Firmware : V0.1
- Environnement : `firmware/platformio.ini`, `env:kc868_a32`
- Commande build : `python -m platformio run`
- Banc automatise host-side : `python -m platformio test -e native`

## Automatisation host-side

Un banc de tests automatise couvre les scenarios FT-0001 au niveau logique metier, sans carte ESP32 et sans moniteur serie.

- Fichier : `firmware/test/test_ft0001_controller/test_ft0001_controller.cpp`
- Point d'entree teste : `Controller::update()`
- Entrees pilotees : `InputsSnapshot`
- Temps pilote : `nowMs` simule

Limites de couverture : ces tests valident les transitions d'etat, alarmes, messages metier et sorties commandees par le `Controller`. Le parsing des commandes du simulateur est couvert separement par `test_simulator_commands`. L'affichage serie, le transport `Stream` Arduino, les entrees/sorties physiques de la carte KC868-A32, le banc electrique et l'installation reelle restent hors couverture host-side.

Verdict automatise au 2026-07-07 : `python -m platformio test -e native` passe avec 35 tests reussis sur 35, dont SIM-001 a SIM-016 et les scenarios de persistance minimale.

## Preconditions

- Le firmware compile.
- Le moniteur serie est ouvert a `115200` bauds.
- Les entrees par defaut sont nominales : `AUTO`, capot ferme, `EP_LAVAGE = OFF`, `EP_CRITIQUE = OFF`, sondes presentes.

## Commandes utiles

```text
help
status
auto
maintenance
manual
lavage on
lavage off
critique on
critique off
capot open
capot close
test
reset
tambour on
tambour off
rincage on
rincage off
temp eau lost
temp local lost
temp eau 20
temp local 18
```

## Scenarios

| ID | Procedure | Resultat attendu | Automatisation |
| --- | --- | --- | --- |
| SIM-001 | Demarrer, puis `status`. | `AUTO_WAIT`, filtration `ON`, UV `ON`, tambour `OFF`, rincage `OFF`, alarme `none`. | Host-side : `test_sim_001_boot_nominal` |
| SIM-002 | `capot open`, attendre l'affichage. | `MAINTENANCE`, message `MAINTENANCE - CAPOT OUVERT`, tambour/rincage `OFF`. | Host-side : `test_sim_002_capot_open_switches_to_maintenance` |
| SIM-003 | `capot close`, attendre retour stable, puis `critique on`. | Apres anti-rebond court : `FAULT`, alarme `A02` si `EP_LAVAGE = OFF`, ou `A01` si `EP_LAVAGE = ON`; sorties dangereuses `OFF`; le defaut reste actif apres `critique off` tant que `reset` n'est pas execute. | Host-side : `test_sim_003_critical_level_incoherence_latches_until_reset` |
| SIM-004 | `critique off`, `reset`, `lavage on`, attendre la temporisation lavage. | `WASH_AUTO`, tambour `ON`, rincage `ON`, voyant lavage `ON`. | Host-side : `test_sim_004_005_auto_wash_nominal_then_returns_to_auto_wait` |
| SIM-005 | Pendant `WASH_AUTO`, attendre au moins 10 s puis `lavage off`. | Transition `POST_WASH`, puis `SAFETY_PAUSE`, puis retour `AUTO_WAIT` apres anti-redemarrage. | Host-side : `test_sim_004_005_auto_wash_nominal_then_returns_to_auto_wait` |
| SIM-006 | `lavage on` permanent pendant 3 tentatives. | `RETRY_PAUSE` entre tentatives, puis `DEGRADED`, alarme `A04`, tambour/rincage `OFF`. | Host-side : `test_sim_006_persistent_wash_demand_retries_then_degrades_with_a04` |
| SIM-007 | Pendant `WASH_AUTO`, executer `capot open`. | `FAULT`, alarme `A03`, tambour/rincage `OFF`; filtration, decoration, UV et mise a niveau restent `ON` si EP_CRITIQUE est absent ; reset refuse tant que le capot reste ouvert. | Host-side : `test_sim_007_capot_open_during_wash_triggers_a03_and_blocks_reset` |
| SIM-008 | `capot close`, attendre retour stable, puis `reset`. | Si la cause a disparu, l'alarme bloquante est acquittee et le systeme revient au mode demande. | Host-side : `test_sim_008_capot_fault_can_reset_after_capot_close_stable` |
| SIM-009 | `capot open`, puis `test`. | `A13 - TEST REFUSE CAPOT`, aucune sortie dangereuse active, voyant alarme non allume pour ce refus preventif. | Host-side : `test_sim_009_test_refused_when_capot_open` |
| SIM-010 | `capot close`, attendre retour stable, `critique on`, puis `test`. | `A14 - TEST REFUSE SECURITE`, aucune sortie dangereuse active. | Host-side : `test_sim_010_test_refused_when_level_safety_fault_active` |
| SIM-011 | `critique off`, `reset`, `auto`, `test`. | `TEST_WASH`, tambour/rincage `ON`, puis `AUTO_WAIT` avec message `TEST OK - CYCLE EXECUTE`. | Host-side : `test_sim_011_test_wash_ok_returns_success_message` |
| SIM-012 | `maintenance`, `tambour on`. | `MANUAL`, tambour `ON` tant que la commande est maintenue. | Host-side : `test_sim_012_manual_tambour_command_runs_in_manual_mode` |
| SIM-013 | Pendant commande manuelle active, `capot open`. | `FAULT`, alarme `A03`, tambour/rincage `OFF`; filtration, decoration, UV et mise a niveau restent `ON` si EP_CRITIQUE est absent. | Host-side : `test_sim_013_capot_open_during_manual_command_triggers_a03` |
| SIM-014 | `temp eau lost`. | Alarme warning `A11`, sans arret des fonctions nominales. | Host-side : `test_sim_014_missing_water_temperature_warns_without_stopping_nominal_outputs` |
| SIM-015 | `temp eau 20`, puis `temp local lost`. | Alarme warning `A12`, sans arret des fonctions nominales. | Host-side : `test_sim_015_missing_local_temperature_warns_without_stopping_nominal_outputs` |
| SIM-016 | `temp local 18`, `lavage on`, `test`, laisser `EP_LAVAGE` actif pendant tout le test. | `TEST_WASH`, puis `AUTO_WAIT` avec message `TEST ECHEC - LAVAGE INEFFICACE`; pas de `A04` maintenu par le test seul. | Host-side : `test_sim_016_failed_test_wash_does_not_latch_a04` |

## Verdict

| Champ | Valeur |
| --- | --- |
| Resultat observe | Automatisation host-side : SIM-001 a SIM-016 et scenarios de persistance minimale passes via `python -m platformio test -e native` ; build ESP32 `python -m platformio run` reussi. |
| Verdict | Passe pour la couverture simulateur host-side ; validation materiel KC868-A32, transport serie reel et banc physique a faire separement. |
| Date | 2026-07-07 |
| Operateur | Codex |
