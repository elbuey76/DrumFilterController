# Table des entrées / sorties

## Objet

Ce document liste les entrées et sorties à prévoir pour le MVP du contrôleur de filtre à tambour. Il sert de base au choix de la carte de contrôle, au schéma de câblage et aux essais sur table.

La table reste une référence de conception. La plateforme KC868-A16 ESP32 classique et les principaux organes de puissance sont retenus pour la commande définitive du MVP selon l'ADR-0012 ; les borniers, sections de cable, repérages et validations de schema restent a figer. La même plateforme matérielle doit rester la base de la V2.

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
| DI-001 | `EP_LAVAGE` | Must | Retenu, position à régler | CR18-8DN, NPN, 12-24 VDC, 3 fils, support réglable | Premier niveau côté eau propre sur le report 32 mm, placé au-dessus de EP_CRITIQUE. Demande de lavage lorsque le niveau eau propre est abaissé. Le signal doit être filtré par retard configurable avant lancement d'un cycle, cible initiale 5 à 15 s. Si seul ce capteur est douteux, inhiber les lavages automatiques mais maintenir filtration si EP_CRITIQUE est sain. |
| DI-002 | `EP_CRITIQUE` | Must | Retenu, position à régler | CR18-8DN, NPN, 12-24 VDC, 3 fils, support réglable | Deuxième niveau côté eau propre sur le report 32 mm, placé sous EP_LAVAGE. Force la mise en sécurité après anti-rebond très court, cible initiale 0,5 à 2 s : arrêt filtration, arrêt UV, inhibition tambour et rinçage. Toute perte de confiance dans ce signal est bloquante hydraulique. Redémarrage seulement après retour normal stable et acquittement local valide. |
| DI-003 | `CAPOT_OUVERT` | Must | Retenu V1, validation montage à faire | Fin de course OMRCH `ME-8104`, contact `1NO1NC`, raccorde en contact sec vers `GND` sur entree digitale KC868-A16 | Prioritaire sur le sélecteur AUTO / MAINTENANCE. Le capot et le couvercle transparent désignent la même pièce physique. Le montage doit donner capot fermé = boucle fermée ; capot ouvert, fil coupé ou connecteur débranché = boucle ouverte et donc `CAPOT_OUVERT`. Le choix entre borne `NO` ou `NC` doit être validé au multimètre selon la mécanique réelle. Anti-rebond ouverture 100 à 500 ms ; fermeture stable 1 à 2 s avant réautorisation. Capot ouvert hors action dangereuse : afficher `MAINTENANCE - CAPOT OUVERT`, sans acquittement. Après fermeture stable, retour au mode demande si aucune alarme bloquante capot dangereux n'existe. Interdit le lavage automatique, coupe la rotation tambour et le rinçage. L'UV reste asservi à la filtration autorisée et à l'absence de EP_CRITIQUE. Force maintenance ou sécurité selon la situation. |
| DI-004 | `MODE_AUTO` | Must | Retenu V1 | Sélecteur physique `AUTO / MAINTENANCE` type XB2-BD25 ou équivalent, 22 mm, 2 positions maintenues, contact `1NO + 1NC`, voie AUTO raccordée en contact sec vers `GND` sur entrée digitale KC868-A16 | Demande d'exploitation nominale. Ne doit être acceptée que si les sécurités critiques sont saines. Le sens logique exact doit être validé sur banc. |
| DI-005 | `MODE_MANUEL` | Must | Retenu V1 | Boutons poussoirs locaux 22 mm momentanés `1NO1NC`, noirs, `MANU_TAMBOUR` et `MANU_RINCAGE` | Le mode manuel V1 est limité aux commandes FAT ponctuelles, tout en conservant les verrouillages critiques. Les deux boutons noirs doivent être étiquetés clairement en façade. |
| DI-006 | `MODE_MAINTENANCE` | Must | Retenu V1 | Même sélecteur physique que `MODE_AUTO`, voie MAINTENANCE raccordée en contact sec vers `GND` sur entrée digitale KC868-A16 | Inhibe le démarrage automatique du tambour et prepare les interventions. Les deux voies `MODE_AUTO` et `MODE_MAINTENANCE` doivent être mutuellement exclusives après validation câblage. |
| DI-007 | `TEST_LAVAGE` | Must | Retenu V1 | Bouton poussoir local 22 mm momentané `1NO1NC`, jaune, contact `NO` en contact sec vers `GND` sur entrée digitale KC868-A16 | Lance un seul cycle de test autonome borne après appui bref en AUTO ou MAINTENANCE si le capot est fermé, EP_CRITIQUE absent, les capteurs de niveau cohérents et aucun défaut critique bloquant actif. Pas de relances multiples. Verdict `TEST OK - CYCLE EXÉCUTÉ` si EP_LAVAGE était inactif au départ ; verdict selon retour EP_LAVAGE s'il était actif. Refus immédiat avec `A13 - TEST REFUSÉ CAPOT` si capot ouvert, ou `A14 - TEST REFUSÉ SÉCURITÉ` si EP_CRITIQUE, capteurs incohérents ou défaut critique. Arrêt immédiat si capot ouvert, EP_CRITIQUE ou défaut critique apparaît pendant le test. |
| DI-008 | `RESET_ALARME` | Must | Retenu V1 | Bouton poussoir local dédié 22 mm momentané `1NO1NC`, bleu, contact `NO` en contact sec vers `GND` sur entrée digitale KC868-A16 | Acquitte les alarmes lorsque les conditions de retour au service sont satisfaites. Requis après EP_CRITIQUE confirmé, après retour niveau normal stable. L'acquittement distant est réservé à une V2. |
| DI-009 | `MANU_TAMBOUR` | Must | Retenu V1 | Bouton poussoir local 22 mm momentané `1NO1NC`, noir, contact `NO` en contact sec vers `GND` sur entrée digitale KC868-A16 | Demande manuelle de rotation tambour tant que le bouton est maintenu, refusée si capot ouvert, niveau critique ou défaut critique. Refus capot ouvert avec message local simple, sans alarme bloquante si aucune sortie dangereuse n'a démarré. |
| DI-010 | `MANU_RINCAGE` | Must | Retenu V1 | Bouton poussoir local 22 mm momentané `1NO1NC`, noir, contact `NO` en contact sec vers `GND` sur entrée digitale KC868-A16 | Demande manuelle de pompe de rinçage tant que le bouton est maintenu, refusée si capot ouvert, niveau critique ou défaut critique. Refus capot ouvert avec message local simple, sans alarme bloquante si aucune sortie dangereuse n'a démarré. |
| DI-011 | `MANU_POMPE_FILTRATION` | Won't for now | Exclu V1 | Non retenu pour le mode manuel V1 | La pompe principale reste pilotée par les autorisations et sécurités globales, sans commande manuelle independante sur l'IHM V1. |
| DI-012 | `MANU_POMPE_DECO` | Won't for now | Exclu V1 | Non retenu pour le mode manuel V1 | La pompe décoration reste pilotée par les autorisations et sécurités globales, sans commande manuelle independante sur l'IHM V1. |
| DI-013 | `MANU_UV` | Won't for now | Exclu V1 | Non retenu pour le mode manuel V1 | L'UV reste asservi à l'autorisation filtration et à l'absence de niveau critique, sans commande manuelle independante sur l'IHM V1. |
| AI-001 | `TEMP_BASSIN` | Must | Retenu V1, validation banc à faire | DS18B20 étanche inox 3 fils, longueur cible 3 m, alimentation 3,3 V, bus 1-Wire, pull-up 4,7 kΩ vers 3,3 V | Mesure une température représentative de l'eau du bassin, en arrivée gravitaire avant pompe/UV ou directement dans le bassin en zone brassée, ombragée et accessible. Détection de perte de mesure et seuils d'alerte : basse < 4 deg C, haute > 28 deg C. Perte de mesure : `A11 - SONDE EAU ABSENTE`. Alerte informative uniquement en V1. Brochage des couleurs à vérifier sur banc avant cablage. |
| AI-002 | `TEMP_LOCAL` | Must | Retenu V1, validation banc à faire | Même DS18B20 étanche inox 3 fils que `TEMP_BASSIN`, longueur cible 3 m, alimentation 3,3 V, bus 1-Wire, pull-up 4,7 kΩ vers 3,3 V | Mesure la température de l'air du local de filtration, hors coffret, à distance des alimentations, contacteurs, pompes, soleil et volume chaud sous capot. La sonde inox doit être montée sur support ventilé pour représenter l'air du local. Détection de perte de mesure et seuils d'alerte : basse < 2 deg C, haute > 40 deg C. Perte de mesure : `A12 - SONDE LOCAL ABSENTE`. Alerte informative uniquement en V1. Brochage des couleurs à vérifier sur banc avant cablage. |
| DI-014 | `POSITION_TAMBOUR` | Should | Option V2 ou V1.1 tardive | Capteur de position à étudier | Non retenu par défaut en V1.1. À ajouter seulement si l'indexation au temps pose problème ou si une position reproductible devient nécessaire. Non requis et non ajoute en V1 pour rester en diagnostic indirect. |
| AI-003 | `COURANT_TAMBOUR` | Should | Option V1.1 | Capteur de courant DC à étudier plus tard | La protection matérielle surintensité/blocage reste obligatoire en V1. Aucun capteur de courant dédié n'est ajouté en V1 ; seul un contact défaut simple fourni naturellement par le module de protection peut être exploité si cela ne complexifie pas le MVP. |

### Interface électrique KC868-A16 / CR18-8DN

Les capteurs de niveau `EP_LAVAGE` et `EP_CRITIQUE` sont cables directement sur les entrees digitales du KC868-A16, sans relais d'interface par defaut, sous reserve de validation banc.

La KC868-A16 dispose d'entrees digitales optocouplees prevues pour des contacts secs ramenes vers `GND`. La compatibilite electrique avec la sortie collecteur ouvert NPN du CR18-8DN est retenue en premiere intention, mais le courant d'entree et la tension residuelle doivent etre mesures sur la revision A16 recue avant cablage definitif.

La documentation CR18-8DN indique une sortie `NPN` 3 fils, normalement ouverte pour le suffixe `N`, avec alimentation `12-24 VDC`, plage admissible `10-30 VDC`, consommation capteur `10 mA` max, courant de sortie `200 mA` max et tension résiduelle `1,5 V` max. Ces valeurs donnent une marge probable, a confirmer avec le courant reel de l'entree A16.

| Fil CR18-8DN | Fonction | Raccordement V1 |
| --- | --- | --- |
| Marron | Alimentation capteur | `+12 VDC` capteurs, départ fusible 1 A |
| Bleu | 0 V capteur | `GND / 0 V` commun avec les entrées KC868-A16 |
| Noir | Sortie NPN | Entrée digitale KC868-A16, par exemple `I1` pour `EP_LAVAGE` et `I2` pour `EP_CRITIQUE` |

La logique firmware doit considérer ces entrées comme actives quand la sortie NPN tire l'entrée vers 0 V. Le sens logique exact lu via les expanseurs d'E/S de la KC868-A16 doit être vérifié sur banc et inversé en logiciel si nécessaire.

Cette interface directe ne rend pas les capteurs de niveau intrinsèquement fail-safe : une rupture du fil noir ou une absence d'alimentation capteur peut ressembler à un capteur inactif. La V1 compense par cohérence logicielle entre `EP_LAVAGE` et `EP_CRITIQUE`, temporisations prudentes et essais périodiques de validation, mais ne prétend pas détecter toutes les ruptures de ligne sans capteur ou diagnostic supplémentaire.

## Sorties automate et puissance

| ID | Nom | Priorité | État | Interface cible | Comportement attendu |
| --- | --- | --- | --- | --- | --- |
| DO-001 | `CMD_TAMBOUR` | Must | Retenu, validation comportement à faire | Sortie MOSFET KC868-A16 12 VDC vers bobine du relais HELLA 4RD 933 332-551 ; moteur Fyearfly 12 VDC 10 rpm ; fusible 5 A ; courant de blocage annoncé 6,5 A | Commande rotation tambour pendant lavage, test ou manuel autorisé. Mesurer le courant de bobine et ajouter une suppression de surtension si elle n'est pas integree. L'indexation est une extension V1.1. Inhibée en niveau critique, capot ouvert, maintenance ou défaut critique. |
| DO-002 | `CMD_RINCAGE` | Must | Retenu, relais d'interface à choisir | Sortie MOSFET KC868-A16 vers bobine 12 VDC d'un relais d'interface rail DIN ; contact du relais vers bobine 230 VAC du Schneider TeSys LC1D18P7, 3P, AC-3 18 A ; pompe VEVOR / Leo EKJ-802S | Commande pompe de rinçage pendant lavage ou test. Le relais d'interface doit avoir un contact qualifie pour charge electromagnetique, cible `AC-15 >= 1 A sous 230 VAC`. Le schema final doit assurer separation BT/secteur et reperage du 230 VAC de commande. Inhibée en niveau critique, maintenance incompatible ou défaut critique. |
| DO-003 | `CMD_POMPE_FILTRATION` | Must | Retenu, validation courant à faire | Sortie MOSFET KC868-A16 12 VDC vers contacteur TOMZN TOCT1-25Z 25 A, bobine 12 VDC | Autorise la pompe principale. Valider moins de 500 mA par bobine et la suppression de surtension. Coupé en niveau critique, défaut critique ou stratégie de sécurité retenue. Depart 230 VAC separe de l'UV, de la pompe décoration et de la mise à niveau. |
| DO-004 | `CMD_POMPE_DECO` | Must | Retenu, validation courant à faire | Sortie MOSFET KC868-A16 12 VDC vers contacteur TOMZN TOCT1-25Z 25 A, bobine 12 VDC | Autorise la pompe décoration. Valider moins de 500 mA par bobine et la suppression de surtension. Suit exactement la même sécurité hydraulique que la pompe principale, car elle aspire au même endroit : OFF sur EP_CRITIQUE. |
| DO-005 | `CMD_UV` | Must | Retenu, validation courant à faire | Sortie MOSFET KC868-A16 12 VDC vers contacteur TOMZN TOCT1-25Z 25 A, bobine 12 VDC | Autorise l'UV seulement si la filtration est autorisée et qu'aucun niveau critique n'est actif. Valider moins de 500 mA par bobine et la suppression de surtension. L'ouverture du capot FAT ne coupe pas l'UV par elle-meme, car l'UV est hors tambour. Après EP_CRITIQUE acquitté, réautorisation après redémarrage filtration et courte stabilisation. |
| DO-006 | `CMD_MISE_A_NIVEAU` | Must | Retenu, validation courant à faire | Sortie MOSFET KC868-A16 12 VDC vers contacteur TOMZN TOCT1-25Z 25 A, bobine 12 VDC ou sortie equivalente selon dispositif final | Valider moins de 500 mA par bobine et la suppression de surtension. Coupé en niveau critique pour éviter un remplissage infini, masquer une fuite ou aggraver une situation hydraulique anormale. |
| DO-007 | `VOYANT_MARCHE` | Must | Retenu V1 | Voyant LED étanche 16 mm, vert, 12 VDC | Indique marche contrôleur, système opérationnel ou auto OK selon le câblage retenu. Complément visuel minimal de l'écran. |
| DO-008 | `VOYANT_AUTO` | Should | À définir | LED, voyant ou IHM | Indique mode auto actif. |
| DO-009 | `VOYANT_MANUEL` | Should | À définir | LED, voyant ou IHM | Indique mode manuel actif. |
| DO-010 | `VOYANT_MAINTENANCE` | Should | À définir | LED, voyant ou IHM | Indique mode maintenance actif. |
| DO-011 | `VOYANT_DEGRADE` | Should | À définir | LED, voyant ou IHM | Indique fonctionnement dégradé. |
| DO-012 | `VOYANT_LAVAGE` | Should | Retenu montage MVP | Voyant LED étanche 16 mm, jaune, 12 VDC | Indique cycle de lavage, test, dégradé ou maintenance si le câblage reste simple. Non bloquant car l'écran affiche le cycle. |
| DO-013 | `VOYANT_ALARME` | Must | Retenu V1 | Voyant LED étanche 16 mm, rouge, 12 VDC | Signale une alarme active, un défaut critique ou `A15 - CAPOT OUVERT LONG`. Pour A15, le voyant rouge est fixe, sans clignotement et sans maintien après fermeture stable du capot. Complément visuel minimal de l'écran, lisible localement. |
| COM-001 | `IHM_ECRAN` | Must | Retenu V1, validation banc à faire | LCD 2004 / 20x4 I2C 3,3 V, fond bleu, module I2C type PCF8574 ou equivalent, raccordement cible sur `GPIO32` / `GPIO33` de la KC868-A16 via un bus I2C auxiliaire separe | Affiche au minimum mode actuel, état niveau, état lavage, alarme active et température eau. Affiche `MAINTENANCE - CAPOT OUVERT` tant que le capot est ouvert hors action dangereuse. Priorise EP_CRITIQUE/incohérence capteurs, capot dangereux, défaut lavage, capot ouvert trop longtemps, température puis infos. Messages au format `Axx - MESSAGE COURT`, liste minimale A01 à A15. `A15` disparaît automatiquement après fermeture stable et ne demande pas d'acquittement. Affiche la cause des resets refusés et tests refusés. Ne présente pas le bypass passif comme mesure ; utiliser MODE DÉGRADÉ - BYPASS SUPPOSÉ. Les détails peuvent être accessibles par page ou défilement. Ne pas connecter l'écran au bus I2C interne des PCF8574 ; vérifier adresse I2C, contraste, rétroéclairage et absence de pull-up 5 V avant câblage final. |
| COM-002 | `RTC_DS3231` | Must | Retenu comme voie heure fiable V2, validation banc à faire | Module RTC DS3231 I2C 3,3 V avec batterie rechargeable, adresse attendue `0x68`, raccordement cible sur `GPIO32` / `GPIO33` de la KC868-A16 via le bus I2C auxiliaire si partage stable avec le LCD | Fournit une heure civile locale pour horodatage, statistiques, dernier lavage, test journalier, programmation horaire, notifications et synthese quotidienne. Le Wi-Fi/NTP peut recaler la RTC en V2 mais ne doit pas etre l'unique source. Si l'heure est invalide ou absente, le controleur reste fonctionnel avec compteurs persistants et horodatage inconnu. Les temporisations de securite et de cycle ne doivent pas dependre de la RTC. |

## Synthèse du noyau E/S V1

### Entrées minimales V1

- `EP_LAVAGE`
- `EP_CRITIQUE`
- `CAPOT_OUVERT`
- sélecteur physique `AUTO / MAINTENANCE` 22 mm, 2 positions maintenues, `1NO + 1NC`
- `RESET_ALARME` bouton poussoir bleu 22 mm momentané `1NO1NC`
- `TEST_LAVAGE` bouton poussoir jaune 22 mm momentané `1NO1NC`
- `MANU_TAMBOUR` bouton poussoir noir 22 mm momentané `1NO1NC`
- `MANU_RINCAGE` bouton poussoir noir 22 mm momentané `1NO1NC`
- `TEMP_BASSIN`
- `TEMP_LOCAL`

### Sorties minimales V1

- `CMD_TAMBOUR`
- `CMD_RINCAGE`
- `CMD_POMPE_FILTRATION`
- `CMD_POMPE_DECO`
- `CMD_UV`
- `CMD_MISE_A_NIVEAU`
- `IHM_ECRAN` LCD 2004 / 20x4 I2C 3,3 V
- `RTC_DS3231` module RTC I2C 3,3 V avec batterie rechargeable pour l'heure fiable V2
- voyant marche vert 16 mm 12 VDC et voyant alarme rouge 16 mm 12 VDC comme complément visuel minimal
- voyant lavage jaune 16 mm 12 VDC retenu pour le montage MVP

Les bulleurs de la cuve bio et du bassin ne sont pas pilotes par le contrôleur. Ils seront branchés directement sur le 220 V et ne doivent pas passer par un circuit coupé par la sécurité niveau critique.

## Groupes de sécurité de puissance

| Groupe | Équipements | Comportement en niveau critique | Remarque |
| --- | --- | --- | --- |
| Groupe coupé sécurité hydraulique | Pompe filtration, pompe décoration, UV, mise à niveau automatique | Coupé ou inhibition | Ces équipements peuvent aggraver une marche à sec, une vidange ou un fonctionnement hors eau. |
| Groupe lavage FAT | Tambour, pompe de rinçage | Inhibe | Aucun lavage ne doit être lance en situation d'eau insuffisante. |
| Protection moteur tambour | Fusible ATO 5 A, relais HELLA 12 V 15 A inductif | Coupure matérielle par fusible ; retour défaut automate non prévu en V1 | Courant de blocage annoncé 6,5 A d'après screenshot fournisseur ; calibre plus restrictif retenu pour essai, comportement à vérifier par démarrage, charge nominale et blocage contrôlé. |
| Protection pompe rinçage | Disjoncteur 10 A courbe C et contacteur Schneider LC1D18P7 | Coupé ou défaut selon architecture retenue | Pompe classe I à raccorder à la terre ; tenir compte du courant d'appel moteur. Le contacteur plus calibre ne change pas le calibre de protection. |
| Groupe hors contrôleur | Bulleur cuve bio, bulleur bassin | Non piloté | Alimentation directe 220 V, hors sorties contrôlées et hors circuit coupé par niveau critique. |
| Groupe signalisation | Voyants, écran | Maintenu si possible | La signalisation doit rester disponible pour comprendre l'état de sécurité. |

## Protections électriques retenues

| Circuit | Protection / organe | Remarque |
| --- | --- | --- |
| Protection amont cote maison | Disjoncteur dedie 16 A | Calibre retenu pour la liaison existante d'environ 20 m entre tableau maison et coffret filtration, tant que sa section n'est pas confirmee en 2,5 mm2. |
| Tete de tableau | Interrupteur differentiel 2P 30 mA, 40 A, type A | La notice AquaForte DM-Vario demande une protection 30 mA sans imposer type F ou B ; calibre 40 A coherent avec les charges reelles et les prises maintenance ponctuelles. |
| Alimentation 12 VDC | Disjoncteur 4 A courbe C | Alimente la Mean Well NDR-120-12. |
| Pompe de rincage | Disjoncteur 10 A courbe C | Alimente le depart pompe commande par contacteur Schneider. |
| Prises local | Disjoncteur 16 A courbe C | 1 prise bulleur bassin, 1 prise bulleur filtre bio, 2 prises maintenance ponctuelle. |
| Pompe filtration | Disjoncteur 6 A courbe C | Depart dedie organe essentiel. |
| UV, pompe decoration, mise a niveau | Disjoncteur 6 A courbe C | Depart separe de la filtration. |
| Eclairage exterieur | Disjoncteur 6 A courbe C | Depart dedie hors automatisme FAT, pour 6 spots LED exterieurs de 3 W avec detecteurs ; charge nominale 18 W, environ 0,08 A sous 230 VAC, cable environ 10 a 15 m. |
| Distribution 12 VDC moteur tambour | Fusible ATO 5 A | Moteur Fyearfly via relais HELLA. |
| Distribution 12 VDC automate | Fusible ATO 3 A | KC868-A16, alimentation principale et banques de sorties selon schema final. |
| Distribution 12 VDC capteurs et boutons | Fusible ATO 1 A | Entrees terrain et commandes locales. |
| Distribution 12 VDC IHM et accessoires | Fusible ATO 1 A | Ecran, voyants, accessoires. |
| Fixation porte-fusibles ATO | Adaptateur rail DIN imprime en 3D si necessaire | A prevoir selon le modele exact du porte-fusibles 4 emplacements. |

## Points à arbitrer avant schéma électrique

- Choisir la référence finale de l'interrupteur differentiel 2P 30 mA, 40 A, type A.
- Mesurer les courants des bobines HELLA et TOMZN, verifier la limite de 500 mA par sortie A16 et definir les suppressions de surtension DC.
- Choisir et valider le relais d'interface rail DIN 12 VDC / 230 VAC pour la bobine du LC1D18P7, avec contact cible `AC-15 >= 1 A sous 230 VAC`.
- Valider sur banc le LCD 2004 I2C 3,3 V : adresse, lisibilite, contraste, retroeclairage, affectation `GPIO32` / `GPIO33` et compatibilite des niveaux logiques.
- Valider sur banc la RTC DS3231 I2C 3,3 V : adresse `0x68`, batterie rechargeable livree, conservation de l'heure apres coupure, absence de pull-up vers 5 V et cohabitation avec le LCD 2004 sur le bus I2C auxiliaire separe.
- Valider sur banc les deux DS18B20 etanches inox : brochage reel des couleurs, lecture 3,3 V, pull-up 4,7 kΩ, detection perte de sonde, identification stable eau/local et choix bus 1-Wire separes ou commun.
- Valider le contact capot OMRCH ME-8104 : bornes `NO`/`NC` a utiliser pour obtenir capot ferme = boucle fermee, capot ouvert/fil coupe/connecteur debranche = boucle ouverte, et fonctionnement repetable avec la came du capot.
- Mesurer le courant réel du moteur Fyearfly a vide, en charge et au blocage pour confirmer le fusible 5 A.
- Finaliser les protections, borniers, reperages et cheminements separes entre basse tension et puissance, avec sections retenues en premiere intention : 2 x 2,5 mm2 moteur tambour 12 VDC, 0,5 mm2 voyants et boutons, 1,5 mm2 circuits 230 VAC cote filtration y compris cablage interne d'armoire.
- Valider sur banc la lecture KC868-A16 des deux CR18-8DN : courant d'entree, repos, détection, fil noir débranché, bleu débranché, marron débranché et inversion logique firmware éventuelle.
- Finaliser le support rail DIN imprime en 3D du relais HELLA.
- Finaliser l'adaptateur rail DIN imprime en 3D du porte-fusibles ATO si le modele retenu n'est pas DIN natif.
- Confirmer les références exactes des sondes de température bassin et local.
- Vérifier que les bulleurs alimentés directement en 220 V restent électriquement séparés des circuits coupés par le contrôleur.
- Valider la section, le cheminement et la reference materielle du depart eclairage exterieur protege en 6 A courbe C.
- Évaluer empiriquement la consommation d'eau de rinçage à partir des essais.
- Conserver le capteur de position tambour comme option V2 ou V1.1 tardive tant que l'indexation au temps n'a pas montre de limité.
