# Table des entrees / sorties

## Objet

Ce document liste les entrees et sorties a prevoir pour le prototype du controleur de filtre a tambour. Il sert de base au choix de la carte de controle, au schema de cablage et aux essais sur table.

La table reste une reference de conception : les references exactes de carte, relais, contacteurs, borniers et protections seront figees plus tard.

Le noyau E/S V1 est gele fonctionnellement avant le choix de plateforme materielle. Les references exactes pourront ajouter des marges ou des E/S optionnelles, mais ne doivent pas retirer les signaux Must ci-dessous.

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
| DI-001 | `EP_LAVAGE` | Must | Retenu, position a regler | CR18-8DN, NPN, 12-24 VDC, 3 fils | Premier niveau cote eau propre sur le report 32 mm. Demande de lavage lorsque le niveau eau propre est abaisse. Le signal doit etre filtre par retard configurable avant lancement d'un cycle, cible initiale 5 a 15 s. Si seul ce capteur est douteux, inhiber les lavages automatiques mais maintenir filtration si EP_CRITIQUE est sain. |
| DI-002 | `EP_CRITIQUE` | Must | Retenu, position a regler | CR18-8DN, NPN, 12-24 VDC, 3 fils | Deuxieme niveau cote eau propre sur le report 32 mm. Force la mise en securite apres anti-rebond tres court, cible initiale 0,5 a 2 s : arret filtration, arret UV, inhibition tambour et rincage. Toute perte de confiance dans ce signal est bloquante hydraulique. Redemarrage seulement apres retour normal stable et acquittement local valide. |
| DI-003 | `CAPOT_OUVERT` | Must | A integrer | Contact sec normalement ferme, interrupteur de securite ou capteur adapte au capot transparent | Prioritaire sur le selecteur AUTO / MAINTENANCE. Le capot et le couvercle transparent designent la meme piece physique. Capot ferme = contact ferme ; capot ouvert, fil coupe ou connecteur debranche = capot ouvert. Anti-rebond ouverture 100 a 500 ms ; fermeture stable 1 a 2 s avant reautorisation. Capot ouvert hors action dangereuse : afficher `MAINTENANCE - CAPOT OUVERT`, sans acquittement. Apres fermeture stable, retour au mode demande si aucune alarme bloquante capot dangereux n'existe. Interdit le lavage automatique, coupe la rotation tambour et le rincage. L'UV reste asservi a la filtration autorisee et a l'absence de EP_CRITIQUE. Force maintenance ou securite selon la situation. |
| DI-004 | `MODE_AUTO` | Must | Retenu V1 | Selecteur physique simple `AUTO / MAINTENANCE` | Demande d'exploitation nominale. Ne doit etre acceptee que si les securites critiques sont saines. |
| DI-005 | `MODE_MANUEL` | Must | Retenu V1 | Boutons momentanes locaux `MANU_TAMBOUR` et `MANU_RINCAGE` | Le mode manuel V1 est limite aux commandes FAT ponctuelles, tout en conservant les verrouillages critiques. |
| DI-006 | `MODE_MAINTENANCE` | Must | Retenu V1 | Selecteur physique simple `AUTO / MAINTENANCE` | Inhibe le demarrage automatique du tambour et prepare les interventions. |
| DI-007 | `TEST_LAVAGE` | Must | Retenu V1 | Bouton physique local momentane | Lance un seul cycle de test autonome borne apres appui bref en AUTO ou MAINTENANCE si le capot est ferme, EP_CRITIQUE absent, les capteurs de niveau coherents et aucun defaut critique bloquant actif. Pas de relances multiples. Verdict `TEST OK - CYCLE EXECUTE` si EP_LAVAGE etait inactif au depart ; verdict selon retour EP_LAVAGE s'il etait actif. Refus immediat avec `A13 - TEST REFUSE CAPOT` si capot ouvert, ou `A14 - TEST REFUSE SECURITE` si EP_CRITIQUE, capteurs incoherents ou defaut critique. Arret immediat si capot ouvert, EP_CRITIQUE ou defaut critique apparait pendant le test. |
| DI-008 | `RESET_ALARME` | Must | Retenu V1 | Bouton physique local dedie | Acquitte les alarmes lorsque les conditions de retour au service sont satisfaites. Requis apres EP_CRITIQUE confirme, apres retour niveau normal stable. L'acquittement distant est reserve a une V2. |
| DI-009 | `MANU_TAMBOUR` | Must | Retenu V1 | Bouton physique local a action maintenue | Demande manuelle de rotation tambour tant que le bouton est maintenu, refusee si capot ouvert, niveau critique ou defaut critique. Refus capot ouvert avec message local simple, sans alarme bloquante si aucune sortie dangereuse n'a demarre. |
| DI-010 | `MANU_RINCAGE` | Must | Retenu V1 | Bouton physique local a action maintenue | Demande manuelle de pompe de rincage tant que le bouton est maintenu, refusee si capot ouvert, niveau critique ou defaut critique. Refus capot ouvert avec message local simple, sans alarme bloquante si aucune sortie dangereuse n'a demarre. |
| DI-011 | `MANU_POMPE_FILTRATION` | Won't for now | Exclu V1 | Non retenu pour le mode manuel V1 | La pompe principale reste pilotee par les autorisations et securites globales, sans commande manuelle independante sur l'IHM V1. |
| DI-012 | `MANU_POMPE_DECO` | Won't for now | Exclu V1 | Non retenu pour le mode manuel V1 | La pompe decoration reste pilotee par les autorisations et securites globales, sans commande manuelle independante sur l'IHM V1. |
| DI-013 | `MANU_UV` | Won't for now | Exclu V1 | Non retenu pour le mode manuel V1 | L'UV reste asservi a l'autorisation filtration et a l'absence de niveau critique, sans commande manuelle independante sur l'IHM V1. |
| AI-001 | `TEMP_BASSIN` | Must | Retenu V1, technologie candidate | Sonde numerique etanche type DS18B20 ou equivalent adapte a la plateforme | Mesure une temperature representative de l'eau du bassin, avec detection de perte de mesure et seuils d'alerte. Seuils initiaux configurables : basse < 4 deg C, haute > 28 deg C. Perte de mesure : `A11 - SONDE EAU ABSENTE`. Alerte informative uniquement en V1. |
| AI-002 | `TEMP_LOCAL` | Must | Retenu V1, technologie candidate | Sonde numerique simple ou equivalent adapte a la plateforme | Mesure la temperature de l'air du local de filtration, avec detection de perte de mesure et seuils d'alerte. Seuils initiaux configurables : basse < 2 deg C, haute > 40 deg C. Perte de mesure : `A12 - SONDE LOCAL ABSENTE`. Alerte informative uniquement en V1. |
| DI-014 | `POSITION_TAMBOUR` | Should | Option V2 ou V1.1 tardive | Capteur de position a etudier | Non retenu par defaut en V1.1. A ajouter seulement si l'indexation au temps pose probleme ou si une position reproductible devient necessaire. Non requis et non ajoute en V1 pour rester en diagnostic indirect. |
| AI-003 | `COURANT_TAMBOUR` | Should | Option V1.1 | Capteur de courant DC a etudier plus tard | La protection materielle surintensite/blocage reste obligatoire en V1. Aucun capteur de courant dedie n'est ajoute en V1 ; seul un contact defaut simple fourni naturellement par le module de protection peut etre exploite si cela ne complexifie pas le MVP. |

## Sorties automate et puissance

| ID | Nom | Priorite | Etat | Interface cible | Comportement attendu |
| --- | --- | --- | --- | --- | --- |
| DO-001 | `CMD_TAMBOUR` | Must | A dimensionner | Commande 12 V DC petite vitesse du moteur SWF 403.835 via relais, contacteur DC ou module adapte | Commande rotation tambour pendant lavage, test ou manuel autorise. L'indexation est une extension V1.1. Inhibee en niveau critique, capot ouvert, maintenance ou defaut critique. |
| DO-002 | `CMD_RINCAGE` | Must | A dimensionner | Commande 220-240 VAC de la pompe VEVOR / Leo EKJ-802S via relais ou contacteur adapte aux charges moteur | Commande pompe de rincage pendant lavage ou test. Inhibee en niveau critique, maintenance incompatible ou defaut critique. |
| DO-003 | `CMD_POMPE_FILTRATION` | Must | A dimensionner | Relais, contacteur ou commande pompe variable | Autorise la pompe principale. Coupe en niveau critique, defaut critique ou strategie de securite retenue. |
| DO-004 | `CMD_POMPE_DECO` | Must | A dimensionner | Relais ou contacteur | Autorise la pompe decoration. Suit exactement la meme securite hydraulique que la pompe principale, car elle aspire au meme endroit : OFF sur EP_CRITIQUE. |
| DO-005 | `CMD_UV` | Must | A dimensionner | Relais ou contacteur adapte a l'UV | Autorise l'UV seulement si la filtration est autorisee et qu'aucun niveau critique n'est actif. L'ouverture du capot FAT ne coupe pas l'UV par elle-meme, car l'UV est hors tambour. Apres EP_CRITIQUE acquitte, reautorisation apres redemarrage filtration et courte stabilisation. |
| DO-006 | `CMD_MISE_A_NIVEAU` | Must | A dimensionner | Relais ou contacteur selon dispositif d'appoint | Coupe en niveau critique pour eviter un remplissage infini, masquer une fuite ou aggraver une situation hydraulique anormale. |
| DO-007 | `VOYANT_MARCHE` | Must | Retenu V1 | Voyant physique vert, LED ou voyant 12/24 V selon coffret | Indique marche controleur, systeme operationnel ou auto OK selon le cablage retenu. Complement visuel minimal de l'ecran. |
| DO-008 | `VOYANT_AUTO` | Should | A definir | LED, voyant ou IHM | Indique mode auto actif. |
| DO-009 | `VOYANT_MANUEL` | Should | A definir | LED, voyant ou IHM | Indique mode manuel actif. |
| DO-010 | `VOYANT_MAINTENANCE` | Should | A definir | LED, voyant ou IHM | Indique mode maintenance actif. |
| DO-011 | `VOYANT_DEGRADE` | Should | A definir | LED, voyant ou IHM | Indique fonctionnement degrade. |
| DO-012 | `VOYANT_LAVAGE` | Should | Option V1 | Voyant physique jaune ou ambre, LED, voyant ou IHM | Indique cycle de lavage, test, degrade ou maintenance si le cablage reste simple. Non bloquant car l'ecran affiche le cycle. |
| DO-013 | `VOYANT_ALARME` | Must | Retenu V1 | Voyant physique rouge, LED ou voyant 12/24 V selon coffret | Signale une alarme active, un defaut critique ou `A15 - CAPOT OUVERT LONG`. Pour A15, le voyant rouge est fixe, sans clignotement et sans maintien apres fermeture stable du capot. Complement visuel minimal de l'ecran, lisible localement. |
| COM-001 | `IHM_ECRAN` | Must | Retenu V1 | Ecran texte ou petit afficheur local, bus serie, I2C, SPI ou autre | Affiche au minimum mode actuel, etat niveau, etat lavage, alarme active et temperature eau. Affiche `MAINTENANCE - CAPOT OUVERT` tant que le capot est ouvert hors action dangereuse. Priorise EP_CRITIQUE/incoherence capteurs, capot dangereux, defaut lavage, capot ouvert trop longtemps, temperature puis infos. Messages au format `Axx - MESSAGE COURT`, liste minimale A01 a A15. `A15` disparait automatiquement apres fermeture stable et ne demande pas d'acquittement. Affiche la cause des resets refuses et tests refuses. Ne presente pas le bypass passif comme mesure ; utiliser MODE DEGRADE - BYPASS SUPPOSE. Les details peuvent etre accessibles par page ou defilement. |

## Synthese du noyau E/S V1

### Entrees minimales V1

- `EP_LAVAGE`
- `EP_CRITIQUE`
- `CAPOT_OUVERT`
- selecteur physique `AUTO / MAINTENANCE`
- `RESET_ALARME`
- `TEST_LAVAGE`
- `MANU_TAMBOUR`
- `MANU_RINCAGE`
- `TEMP_BASSIN`
- `TEMP_LOCAL`

### Sorties minimales V1

- `CMD_TAMBOUR`
- `CMD_RINCAGE`
- `CMD_POMPE_FILTRATION`
- `CMD_POMPE_DECO`
- `CMD_UV`
- `CMD_MISE_A_NIVEAU`
- `IHM_ECRAN`
- voyant marche vert et voyant alarme rouge comme complement visuel minimal
- voyant lavage jaune ou ambre optionnel si cablage simple

Les bulleurs de la cuve bio et du bassin ne sont pas pilotes par le controleur. Ils seront branches directement sur le 220 V et ne doivent pas passer par un circuit coupe par la securite niveau critique.

## Groupes de securite de puissance

| Groupe | Equipements | Comportement en niveau critique | Remarque |
| --- | --- | --- | --- |
| Groupe coupe securite hydraulique | Pompe filtration, pompe decoration, UV, mise a niveau automatique | Coupe ou inhibition | Ces equipements peuvent aggraver une marche a sec, une vidange ou un fonctionnement hors eau. |
| Groupe lavage FAT | Tambour, pompe de rincage | Inhibe | Aucun lavage ne doit etre lance en situation d'eau insuffisante. |
| Protection moteur tambour | Fusible 10 a 15 A initial, protection materielle surintensite/blocage | Coupe materielle obligatoire ; retour defaut automate optionnel | Calibre a ajuster apres mesures du courant reel, notamment demarrage et blocage controle. |
| Protection pompe rincage | Protection secteur et organe de coupure adaptes a une pompe 230 VAC de 0,6 a 0,8 kW | Coupe ou defaut selon architecture retenue | Pompe classe I a raccorder a la terre ; tenir compte du courant d'appel moteur. |
| Groupe hors controleur | Bulleur cuve bio, bulleur bassin | Non pilote | Alimentation directe 220 V, hors sorties controlees et hors circuit coupe par niveau critique. |
| Groupe signalisation | Voyants, ecran | Maintenu si possible | La signalisation doit rester disponible pour comprendre l'etat de securite. |

## Points a arbitrer avant schema electrique

- Choisir la tension de commande 12 V ou 24 V.
- Comparer carte microcontroleur et automate compact / module industriel DIN selon cout, effort de developpement, robustesse et maintenabilite.
- Dimensionner les E/S a partir du noyau V1 gele ci-dessus.
- Dimensionner les relais/contacteurs selon les puissances reelles du moteur tambour, de la pompe de rincage, de la pompe principale, de la pompe decoration, de l'UV et de la mise a niveau.
- Identifier le brochage exact du moteur tambour 5 broches avant de figer la commande petite vitesse et l'isolation propre du contact parking en V1.
- Prevoir une protection materielle surintensite/blocage du moteur tambour ; retour automate seulement si simple avec le module retenu.
- Dimensionner la commande de la pompe de rincage comme une charge moteur secteur 230 VAC, avec terre et protection adaptee.
- Definir les protections electriques et borniers separes entre basse tension et puissance.
- Confirmer la technologie des sondes de temperature bassin et local.
- Verifier que les bulleurs alimentes directement en 220 V restent electriquement separes des circuits coupes par le controleur.
- Evaluer empiriquement la consommation d'eau de rincage a partir des essais.
- Conserver le capteur de position tambour comme option V2 ou V1.1 tardive tant que l'indexation au temps n'a pas montre de limite.
