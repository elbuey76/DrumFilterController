# Table des entrées / sorties

## Objet

Ce document liste les entrées et sorties à prévoir pour le prototype du contrôleur de filtre à tambour. Il sert de base au choix de la carte de contrôle, au schéma de câblage et aux essais sur table.

La table reste une référence de conception : les références exactes de carte, relais, contacteurs, borniers et protections seront figees plus tard.

Le noyau E/S V1 est gele fonctionnellement avant le choix de plateforme matérielle. Les références exactes pourront ajouter des marges ou des E/S optionnelles, mais ne doivent pas retirer les signaux Must ci-dessous.

## Conventions

| Champ | Signification |
| --- | --- |
| ID | Repere provisoire de signal. |
| Type | Entrée automate, sortie automate, circuit de puissance ou information IHM. |
| Priorité | `Must`, `Should` ou `Could`, selon les conventions du backlog. |
| État | Niveau de maturite du signal. |
| Interface cible | Technologie pressentie ou contrainte connue. |
| Comportement attendu | Rôle fonctionnel et règles de sécurité associées. |

## Entrées automate

| ID | Nom | Priorité | État | Interface cible | Comportement attendu |
| --- | --- | --- | --- | --- | --- |
| DI-001 | `EP_LAVAGE` | Must | Retenu, position à régler | CR18-8DN, NPN, 12-24 VDC, 3 fils | Premier niveau côté eau propre sur le report 32 mm. Demande de lavage lorsque le niveau eau propre est abaissé. Le signal doit être filtré par retard configurable avant lancement d'un cycle, cible initiale 5 à 15 s. Si seul ce capteur est douteux, inhiber les lavages automatiques mais maintenir filtration si EP_CRITIQUE est sain. |
| DI-002 | `EP_CRITIQUE` | Must | Retenu, position à régler | CR18-8DN, NPN, 12-24 VDC, 3 fils | Deuxième niveau côté eau propre sur le report 32 mm. Force la mise en sécurité après anti-rebond très court, cible initiale 0,5 à 2 s : arrêt filtration, arrêt UV, inhibition tambour et rinçage. Toute perte de confiance dans ce signal est bloquante hydraulique. Redémarrage seulement après retour normal stable et acquittement local valide. |
| DI-003 | `CAPOT_OUVERT` | Must | À intégrer | Contact sec normalement fermé, interrupteur de sécurité ou capteur adapté au capot transparent | Prioritaire sur le sélecteur AUTO / MAINTENANCE. Le capot et le couvercle transparent désignent la même pièce physique. Capot fermé = contact fermé ; capot ouvert, fil coupé ou connecteur débranché = capot ouvert. Anti-rebond ouverture 100 à 500 ms ; fermeture stable 1 à 2 s avant réautorisation. Capot ouvert hors action dangereuse : afficher `MAINTENANCE - CAPOT OUVERT`, sans acquittement. Après fermeture stable, retour au mode demande si aucune alarme bloquante capot dangereux n'existe. Interdit le lavage automatique, coupe la rotation tambour et le rinçage. L'UV reste asservi à la filtration autorisée et à l'absence de EP_CRITIQUE. Force maintenance ou sécurité selon la situation. |
| DI-004 | `MODE_AUTO` | Must | Retenu V1 | Sélecteur physique simple `AUTO / MAINTENANCE` | Demande d'exploitation nominale. Ne doit être acceptée que si les sécurités critiques sont saines. |
| DI-005 | `MODE_MANUEL` | Must | Retenu V1 | Boutons momentanés locaux `MANU_TAMBOUR` et `MANU_RINCAGE` | Le mode manuel V1 est limité aux commandes FAT ponctuelles, tout en conservant les verrouillages critiques. |
| DI-006 | `MODE_MAINTENANCE` | Must | Retenu V1 | Sélecteur physique simple `AUTO / MAINTENANCE` | Inhibe le démarrage automatique du tambour et prepare les interventions. |
| DI-007 | `TEST_LAVAGE` | Must | Retenu V1 | Bouton physique local momentané | Lance un seul cycle de test autonome borne après appui bref en AUTO ou MAINTENANCE si le capot est fermé, EP_CRITIQUE absent, les capteurs de niveau cohérents et aucun défaut critique bloquant actif. Pas de relances multiples. Verdict `TEST OK - CYCLE EXÉCUTÉ` si EP_LAVAGE était inactif au départ ; verdict selon retour EP_LAVAGE s'il était actif. Refus immédiat avec `A13 - TEST REFUSÉ CAPOT` si capot ouvert, ou `A14 - TEST REFUSÉ SÉCURITÉ` si EP_CRITIQUE, capteurs incohérents ou défaut critique. Arrêt immédiat si capot ouvert, EP_CRITIQUE ou défaut critique apparaît pendant le test. |
| DI-008 | `RESET_ALARME` | Must | Retenu V1 | Bouton physique local dédié | Acquitte les alarmes lorsque les conditions de retour au service sont satisfaites. Requis après EP_CRITIQUE confirmé, après retour niveau normal stable. L'acquittement distant est réservé à une V2. |
| DI-009 | `MANU_TAMBOUR` | Must | Retenu V1 | Bouton physique local à action maintenue | Demande manuelle de rotation tambour tant que le bouton est maintenu, refusée si capot ouvert, niveau critique ou défaut critique. Refus capot ouvert avec message local simple, sans alarme bloquante si aucune sortie dangereuse n'a démarré. |
| DI-010 | `MANU_RINCAGE` | Must | Retenu V1 | Bouton physique local à action maintenue | Demande manuelle de pompe de rinçage tant que le bouton est maintenu, refusée si capot ouvert, niveau critique ou défaut critique. Refus capot ouvert avec message local simple, sans alarme bloquante si aucune sortie dangereuse n'a démarré. |
| DI-011 | `MANU_POMPE_FILTRATION` | Won't for now | Exclu V1 | Non retenu pour le mode manuel V1 | La pompe principale reste pilotée par les autorisations et sécurités globales, sans commande manuelle independante sur l'IHM V1. |
| DI-012 | `MANU_POMPE_DECO` | Won't for now | Exclu V1 | Non retenu pour le mode manuel V1 | La pompe décoration reste pilotée par les autorisations et sécurités globales, sans commande manuelle independante sur l'IHM V1. |
| DI-013 | `MANU_UV` | Won't for now | Exclu V1 | Non retenu pour le mode manuel V1 | L'UV reste asservi à l'autorisation filtration et à l'absence de niveau critique, sans commande manuelle independante sur l'IHM V1. |
| AI-001 | `TEMP_BASSIN` | Must | Retenu V1, technologie candidate | Sonde numérique étanche type DS18B20 ou équivalent adapté à la plateforme | Mesure une température représentative de l'eau du bassin, avec détection de perte de mesure et seuils d'alerte. Seuils initiaux configurables : basse < 4 deg C, haute > 28 deg C. Perte de mesure : `A11 - SONDE EAU ABSENTE`. Alerte informative uniquement en V1. |
| AI-002 | `TEMP_LOCAL` | Must | Retenu V1, technologie candidate | Sonde numérique simple ou équivalent adapté à la plateforme | Mesure la température de l'air du local de filtration, avec détection de perte de mesure et seuils d'alerte. Seuils initiaux configurables : basse < 2 deg C, haute > 40 deg C. Perte de mesure : `A12 - SONDE LOCAL ABSENTE`. Alerte informative uniquement en V1. |
| DI-014 | `POSITION_TAMBOUR` | Should | Option V2 ou V1.1 tardive | Capteur de position à étudier | Non retenu par défaut en V1.1. À ajouter seulement si l'indexation au temps pose problème ou si une position reproductible devient nécessaire. Non requis et non ajoute en V1 pour rester en diagnostic indirect. |
| AI-003 | `COURANT_TAMBOUR` | Should | Option V1.1 | Capteur de courant DC à étudier plus tard | La protection matérielle surintensité/blocage reste obligatoire en V1. Aucun capteur de courant dédié n'est ajouté en V1 ; seul un contact défaut simple fourni naturellement par le module de protection peut être exploité si cela ne complexifie pas le MVP. |

## Sorties automate et puissance

| ID | Nom | Priorité | État | Interface cible | Comportement attendu |
| --- | --- | --- | --- | --- | --- |
| DO-001 | `CMD_TAMBOUR` | Must | À dimensionner | Commande 12 V DC petite vitesse du moteur SWF 403.835 via relais, contacteur DC ou module adapté | Commande rotation tambour pendant lavage, test ou manuel autorisé. L'indexation est une extension V1.1. Inhibée en niveau critique, capot ouvert, maintenance ou défaut critique. |
| DO-002 | `CMD_RINCAGE` | Must | À dimensionner | Commande 220-240 VAC de la pompe VEVOR / Leo EKJ-802S via relais ou contacteur adapté aux charges moteur | Commande pompe de rinçage pendant lavage ou test. Inhibée en niveau critique, maintenance incompatible ou défaut critique. |
| DO-003 | `CMD_POMPE_FILTRATION` | Must | À dimensionner | Relais, contacteur ou commande pompe variable | Autorise la pompe principale. Coupé en niveau critique, défaut critique ou stratégie de sécurité retenue. |
| DO-004 | `CMD_POMPE_DECO` | Must | À dimensionner | Relais ou contacteur | Autorise la pompe décoration. Suit exactement la même sécurité hydraulique que la pompe principale, car elle aspire au même endroit : OFF sur EP_CRITIQUE. |
| DO-005 | `CMD_UV` | Must | À dimensionner | Relais ou contacteur adapté à l'UV | Autorise l'UV seulement si la filtration est autorisée et qu'aucun niveau critique n'est actif. L'ouverture du capot FAT ne coupe pas l'UV par elle-meme, car l'UV est hors tambour. Après EP_CRITIQUE acquitté, réautorisation après redémarrage filtration et courte stabilisation. |
| DO-006 | `CMD_MISE_A_NIVEAU` | Must | À dimensionner | Relais ou contacteur selon dispositif d'appoint | Coupé en niveau critique pour éviter un remplissage infini, masquer une fuite ou aggraver une situation hydraulique anormale. |
| DO-007 | `VOYANT_MARCHE` | Must | Retenu V1 | Voyant physique vert, LED ou voyant 12/24 V selon coffret | Indique marche contrôleur, système opérationnel ou auto OK selon le câblage retenu. Complément visuel minimal de l'écran. |
| DO-008 | `VOYANT_AUTO` | Should | À définir | LED, voyant ou IHM | Indique mode auto actif. |
| DO-009 | `VOYANT_MANUEL` | Should | À définir | LED, voyant ou IHM | Indique mode manuel actif. |
| DO-010 | `VOYANT_MAINTENANCE` | Should | À définir | LED, voyant ou IHM | Indique mode maintenance actif. |
| DO-011 | `VOYANT_DEGRADE` | Should | À définir | LED, voyant ou IHM | Indique fonctionnement dégradé. |
| DO-012 | `VOYANT_LAVAGE` | Should | Option V1 | Voyant physique jaune ou ambre, LED, voyant ou IHM | Indique cycle de lavage, test, dégradé ou maintenance si le câblage reste simple. Non bloquant car l'écran affiche le cycle. |
| DO-013 | `VOYANT_ALARME` | Must | Retenu V1 | Voyant physique rouge, LED ou voyant 12/24 V selon coffret | Signale une alarme active, un défaut critique ou `A15 - CAPOT OUVERT LONG`. Pour A15, le voyant rouge est fixe, sans clignotement et sans maintien après fermeture stable du capot. Complément visuel minimal de l'écran, lisible localement. |
| COM-001 | `IHM_ECRAN` | Must | Retenu V1 | Écran texte ou petit afficheur local, bus série, I2C, SPI ou autre | Affiche au minimum mode actuel, état niveau, état lavage, alarme active et température eau. Affiche `MAINTENANCE - CAPOT OUVERT` tant que le capot est ouvert hors action dangereuse. Priorise EP_CRITIQUE/incohérence capteurs, capot dangereux, défaut lavage, capot ouvert trop longtemps, température puis infos. Messages au format `Axx - MESSAGE COURT`, liste minimale A01 à A15. `A15` disparaît automatiquement après fermeture stable et ne demande pas d'acquittement. Affiche la cause des resets refusés et tests refusés. Ne présente pas le bypass passif comme mesure ; utiliser MODE DÉGRADÉ - BYPASS SUPPOSÉ. Les détails peuvent être accessibles par page ou défilement. |

## Synthèse du noyau E/S V1

### Entrées minimales V1

- `EP_LAVAGE`
- `EP_CRITIQUE`
- `CAPOT_OUVERT`
- sélecteur physique `AUTO / MAINTENANCE`
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
- voyant marche vert et voyant alarme rouge comme complément visuel minimal
- voyant lavage jaune ou ambre optionnel si câblage simple

Les bulleurs de la cuve bio et du bassin ne sont pas pilotes par le contrôleur. Ils seront branchés directement sur le 220 V et ne doivent pas passer par un circuit coupé par la sécurité niveau critique.

## Groupes de sécurité de puissance

| Groupe | Équipements | Comportement en niveau critique | Remarque |
| --- | --- | --- | --- |
| Groupe coupé sécurité hydraulique | Pompe filtration, pompe décoration, UV, mise à niveau automatique | Coupé ou inhibition | Ces équipements peuvent aggraver une marche à sec, une vidange ou un fonctionnement hors eau. |
| Groupe lavage FAT | Tambour, pompe de rinçage | Inhibe | Aucun lavage ne doit être lance en situation d'eau insuffisante. |
| Protection moteur tambour | Fusible 10 à 15 A initial, protection matérielle surintensité/blocage | Coupure matérielle obligatoire ; retour défaut automate optionnel | Calibre à ajuster après mesures du courant réel, notamment démarrage et blocage contrôlé. |
| Protection pompe rinçage | Protection secteur et organe de coupure adaptés à une pompe 230 VAC de 0,6 à 0,8 kW | Coupé ou défaut selon architecture retenue | Pompe classe I à raccorder à la terre ; tenir compte du courant d'appel moteur. |
| Groupe hors contrôleur | Bulleur cuve bio, bulleur bassin | Non piloté | Alimentation directe 220 V, hors sorties contrôlées et hors circuit coupé par niveau critique. |
| Groupe signalisation | Voyants, écran | Maintenu si possible | La signalisation doit rester disponible pour comprendre l'état de sécurité. |

## Points à arbitrer avant schéma électrique

- Choisir la tension de commande 12 V ou 24 V.
- Comparer carte microcontrôleur et automate compact / module industriel DIN selon coût, effort de développement, robustesse et maintenabilité.
- Dimensionner les E/S à partir du noyau V1 gelé ci-dessus.
- Dimensionner les relais/contacteurs selon les puissances réelles du moteur tambour, de la pompe de rinçage, de la pompe principale, de la pompe décoration, de l'UV et de la mise à niveau.
- Identifier le brochage exact du moteur tambour 5 broches avant de figer la commande petite vitesse et l'isolation propre du contact parking en V1.
- Prévoir une protection matérielle surintensité/blocage du moteur tambour ; retour automate seulement si simple avec le module retenu.
- Dimensionner la commande de la pompe de rinçage comme une charge moteur secteur 230 VAC, avec terre et protection adaptée.
- Définir les protections électriques et borniers séparés entre basse tension et puissance.
- Confirmer la technologie des sondes de température bassin et local.
- Vérifier que les bulleurs alimentés directement en 220 V restent électriquement séparés des circuits coupés par le contrôleur.
- Évaluer empiriquement la consommation d'eau de rinçage à partir des essais.
- Conserver le capteur de position tambour comme option V2 ou V1.1 tardive tant que l'indexation au temps n'a pas montre de limité.
