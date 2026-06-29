# Table des entrees / sorties

## Objet

Ce document liste les entrees et sorties a prevoir pour le prototype du controleur de filtre a tambour. Il sert de base au choix de la carte de controle, au schema de cablage et aux essais sur table.

La table reste une reference de conception : les references exactes de carte, relais, contacteurs, borniers et protections seront figees plus tard.

## Conventions

| Champ | Signification |
| --- | --- |
| ID | Repere provisoire de signal. |
| Type | Entree automate, sortie automate, circuit de puissance ou information IHM. |
| Priorite | `Must`, `Should` ou `Could`, selon les conventions du backlog. |
| Etat | Niveau de maturite du signal. |
| Interface cible | Technologie pressentie ou contrainte connue. |
| Comportement attendu | Role fonctionnel et regles de securite associees. |

## Entrees automate

| ID | Nom | Priorite | Etat | Interface cible | Comportement attendu |
| --- | --- | --- | --- | --- | --- |
| DI-001 | `EP_LAVAGE` | Must | Retenu, position a regler | CR18-8DN, NPN, 12-24 VDC, 3 fils | Demande de lavage lorsque le niveau eau propre est abaisse. Le signal doit etre filtre par retard configurable avant lancement d'un cycle. |
| DI-002 | `EP_CRITIQUE` | Must | Retenu, position a regler | CR18-8DN, NPN, 12-24 VDC, 3 fils | Niveau eau propre critique. Force la mise en securite : arret filtration, arret UV, inhibition tambour et rincage. |
| DI-003 | `CAPOT_OUVERT` | Must | A integrer | Contact sec, interrupteur de securite ou capteur adapte au capot | Interdit le lavage automatique, coupe la rotation tambour et coupe l'UV. Peut forcer ou proposer le mode maintenance selon la logique retenue. |
| DI-004 | `MODE_AUTO` | Must | A definir | Selecteur, bouton ou IHM locale | Demande d'exploitation nominale. Ne doit etre acceptee que si les securites critiques sont saines. |
| DI-005 | `MODE_MANUEL` | Must | A definir | Selecteur, bouton ou IHM locale | Autorise les commandes individuelles tout en conservant les verrouillages critiques. |
| DI-006 | `MODE_MAINTENANCE` | Must | A definir | Selecteur, bouton ou IHM locale | Inhibe le demarrage automatique du tambour et prepare les interventions. |
| DI-007 | `TEST_LAVAGE` | Must | A definir | Bouton local ou commande IHM | Lance un cycle de test automatise si les preconditions sont remplies. |
| DI-008 | `RESET_ALARME` | Must | A definir | Bouton local ou commande IHM | Acquitte les alarmes lorsque les conditions de retour au service sont satisfaites. |
| DI-009 | `MANU_TAMBOUR` | Must | A definir | Bouton local ou commande IHM | Demande manuelle de rotation tambour, refusee si capot ouvert, niveau critique ou defaut critique. |
| DI-010 | `MANU_RINCAGE` | Must | A definir | Bouton local ou commande IHM | Demande manuelle de pompe de rincage, refusee si niveau critique ou defaut critique. |
| DI-011 | `MANU_POMPE_FILTRATION` | Must | A definir | Bouton local ou commande IHM | Demande manuelle de pompe principale, refusee si niveau critique ou defaut critique. |
| DI-012 | `MANU_POMPE_DECO` | Must | A definir | Bouton local ou commande IHM | Demande manuelle de pompe decoration, soumise aux memes securites hydrauliques que la pompe principale. |
| DI-013 | `MANU_UV` | Must | A definir | Bouton local ou commande IHM | Demande manuelle UV, refusee si filtration non autorisee ou niveau critique present. |
| AI-001 | `TEMP_BASSIN` | Must | A choisir | Sonde temperature a definir | Mesure temperature eau bassin, avec detection de perte de mesure et seuils d'alerte. |
| AI-002 | `TEMP_LOCAL` | Must | A choisir | Sonde temperature a definir | Mesure temperature local technique ou coffret, avec detection de perte de mesure et seuils d'alerte. |
| DI-014 | `POSITION_TAMBOUR` | Should | Option | Capteur de position a etudier | Option utile pour l'indexation et certains diagnostics futurs. Non requis pour rester en diagnostic indirect V1. |

## Sorties automate et puissance

| ID | Nom | Priorite | Etat | Interface cible | Comportement attendu |
| --- | --- | --- | --- | --- | --- |
| DO-001 | `CMD_TAMBOUR` | Must | A dimensionner | Relais, contacteur ou variateur selon moteur | Commande rotation tambour pendant lavage, test, manuel autorise ou indexation. Inhibee en niveau critique, capot ouvert, maintenance ou defaut critique. |
| DO-002 | `CMD_RINCAGE` | Must | A dimensionner | Relais ou contacteur selon pompe de rincage | Commande pompe de rincage pendant lavage ou test. Inhibee en niveau critique, maintenance incompatible ou defaut critique. |
| DO-003 | `CMD_POMPE_FILTRATION` | Must | A dimensionner | Relais, contacteur ou commande pompe variable | Autorise la pompe principale. Coupe en niveau critique, defaut critique ou strategie de securite retenue. |
| DO-004 | `CMD_POMPE_DECO` | Must | A dimensionner | Relais ou contacteur | Autorise la pompe decoration. Meme securite hydraulique que la pompe principale, car elle aspire au meme endroit. |
| DO-005 | `CMD_UV` | Must | A dimensionner | Relais ou contacteur adapte a l'UV | Autorise l'UV seulement si la filtration est autorisee et qu'aucun niveau critique n'est actif. Coupe a l'ouverture capot selon la strategie retenue. |
| DO-006 | `CMD_MISE_A_NIVEAU` | Must | A dimensionner | Relais ou contacteur selon dispositif d'appoint | Coupe en niveau critique pour eviter un remplissage infini en cas de fuite ou incident hydraulique. |
| DO-007 | `VOYANT_MARCHE` | Should | A definir | LED, voyant 12/24 V ou IHM | Indique alimentation controleur ou systeme operationnel. |
| DO-008 | `VOYANT_AUTO` | Should | A definir | LED, voyant ou IHM | Indique mode auto actif. |
| DO-009 | `VOYANT_MANUEL` | Should | A definir | LED, voyant ou IHM | Indique mode manuel actif. |
| DO-010 | `VOYANT_MAINTENANCE` | Should | A definir | LED, voyant ou IHM | Indique mode maintenance actif. |
| DO-011 | `VOYANT_DEGRADE` | Should | A definir | LED, voyant ou IHM | Indique fonctionnement degrade. |
| DO-012 | `VOYANT_LAVAGE` | Should | A definir | LED, voyant ou IHM | Indique cycle de lavage, test ou rotation commandee selon code d'affichage retenu. |
| DO-013 | `VOYANT_ALARME` | Must | A definir | LED, voyant ou IHM | Signale une alarme active ou un defaut critique. Doit rester lisible localement. |
| COM-001 | `IHM_ECRAN` | Should | Option a definir | Ecran local, bus serie, I2C, SPI ou autre | Affiche mode, defaut actif, etat niveaux, temperatures et compteurs utiles si un ecran est retenu. |

Les bulleurs de la cuve bio et du bassin ne sont pas pilotes par le controleur. Ils seront branches directement sur le 220 V et ne doivent pas passer par un circuit coupe par la securite niveau critique.

## Groupes de securite de puissance

| Groupe | Equipements | Comportement en niveau critique | Remarque |
| --- | --- | --- | --- |
| Groupe coupe securite hydraulique | Pompe filtration, pompe decoration, UV, mise a niveau automatique | Coupe ou inhibition | Ces equipements peuvent aggraver une marche a sec, une vidange ou un fonctionnement hors eau. |
| Groupe lavage FAT | Tambour, pompe de rincage | Inhibe | Aucun lavage ne doit etre lance en situation d'eau insuffisante. |
| Groupe hors controleur | Bulleur cuve bio, bulleur bassin | Non pilote | Alimentation directe 220 V, hors sorties controlees et hors circuit coupe par niveau critique. |
| Groupe signalisation | Voyants, ecran | Maintenu si possible | La signalisation doit rester disponible pour comprendre l'etat de securite. |

## Points a arbitrer avant schema electrique

- Choisir la tension de commande 12 V ou 24 V.
- Choisir la carte de controle ou l'automate compact.
- Decider si les commandes de mode sont des boutons physiques, un selecteur, une IHM locale ou une combinaison.
- Dimensionner les relais/contacteurs selon les puissances reelles du moteur tambour, de la pompe de rincage, de la pompe principale, de la pompe decoration, de l'UV et de la mise a niveau.
- Definir les protections electriques et borniers separes entre basse tension et puissance.
- Confirmer la technologie des sondes de temperature bassin et local.
- Verifier que les bulleurs alimentes directement en 220 V restent electriquement separes des circuits coupes par le controleur.
- Evaluer empiriquement la consommation d'eau de rincage a partir des essais.
- Conserver le capteur de position tambour comme option tant que son utilite n'est pas validee.
