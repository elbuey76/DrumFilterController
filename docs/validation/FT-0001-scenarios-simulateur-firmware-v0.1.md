# FT-0001 - Scenarios simulateur firmware V0.1

## Objectif

Valider les scenarios obligatoires du firmware MVP V0.1 avec le moniteur serie PlatformIO, sans materiel KC868-A32 raccorde.

## Version testee

- Firmware : V0.1
- Environnement : `firmware/platformio.ini`, `env:kc868_a32`
- Commande build : `python -m platformio run`

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

| ID | Procedure | Resultat attendu |
| --- | --- | --- |
| SIM-001 | Demarrer, puis `status`. | `AUTO_WAIT`, filtration `ON`, UV `ON`, tambour `OFF`, rincage `OFF`, alarme `none`. |
| SIM-002 | `capot open`, attendre l'affichage. | `MAINTENANCE`, message `MAINTENANCE - CAPOT OUVERT`, tambour/rincage `OFF`. |
| SIM-003 | `capot close`, attendre retour stable, puis `critique on`. | Apres anti-rebond court : `FAULT`, alarme `A02` si `EP_LAVAGE = OFF`, ou `A01` si `EP_LAVAGE = ON`; sorties dangereuses `OFF`; le defaut reste actif apres `critique off` tant que `reset` n'est pas execute. |
| SIM-004 | `critique off`, `reset`, `lavage on`, attendre la temporisation lavage. | `WASH_AUTO`, tambour `ON`, rincage `ON`, voyant lavage `ON`. |
| SIM-005 | Pendant `WASH_AUTO`, attendre au moins 10 s puis `lavage off`. | Transition `POST_WASH`, puis `SAFETY_PAUSE`, puis retour `AUTO_WAIT` apres anti-redemarrage. |
| SIM-006 | `lavage on` permanent pendant 3 tentatives. | `RETRY_PAUSE` entre tentatives, puis `DEGRADED`, alarme `A04`, tambour/rincage `OFF`. |
| SIM-007 | Pendant `WASH_AUTO`, executer `capot open`. | `FAULT`, alarme `A03`, tambour/rincage `OFF`; reset refuse tant que le capot reste ouvert. |
| SIM-008 | `capot close`, attendre retour stable, puis `reset`. | Si la cause a disparu, l'alarme bloquante est acquittee et le systeme revient au mode demande. |
| SIM-009 | `capot open`, puis `test`. | `A13 - TEST REFUSE CAPOT`, aucune sortie dangereuse active. |
| SIM-010 | `capot close`, attendre retour stable, `critique on`, puis `test`. | `A14 - TEST REFUSE SECURITE`, aucune sortie dangereuse active. |
| SIM-011 | `critique off`, `reset`, `auto`, `test`. | `TEST_WASH`, tambour/rincage `ON`, puis `AUTO_WAIT` avec message `TEST OK - CYCLE EXECUTE`. |
| SIM-012 | `maintenance`, `tambour on`. | `MANUAL`, tambour `ON` tant que la commande est maintenue. |
| SIM-013 | Pendant commande manuelle active, `capot open`. | `FAULT`, alarme `A03`, tambour/rincage `OFF`. |
| SIM-014 | `temp eau lost`. | Alarme warning `A11`, sans arret des fonctions nominales. |
| SIM-015 | `temp eau 20`, puis `temp local lost`. | Alarme warning `A12`, sans arret des fonctions nominales. |
| SIM-016 | `temp local 18`, `lavage on`, `test`, laisser `EP_LAVAGE` actif pendant tout le test. | `TEST_WASH`, puis `AUTO_WAIT` avec message `TEST ECHEC - LAVAGE INEFFICACE`; pas de `A04` maintenu par le test seul. |

## Verdict

| Champ | Valeur |
| --- | --- |
| Resultat observe | A renseigner pendant l'essai |
| Verdict | A renseigner : Passe / Echoue |
| Date | A renseigner |
| Operateur | A renseigner |
