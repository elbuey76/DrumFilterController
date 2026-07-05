# Table des entrées / sorties

## Objet

Ce document liste les entrées et sorties à prévoir pour le MVP du contrôleur de filtre à tambour. Il sert de base au choix de la carte de contrôle, au schéma de câblage et aux essais sur table.

La table reste une référence de conception. La plateforme KC868-A32 et les principaux organes de puissance sont retenus pour la commande définitive du MVP ; les borniers, sections de cable, repérages et validations de schema restent a figer. La même plateforme matérielle doit rester la base de la V2.

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
| AI-001 | `TEMP_BASSIN` | Must | Retenu V1 | Sonde numérique étanche type DS18B20 ou équivalent adapté à la plateforme | Mesure une température représentative de l'eau du bassin, en arrivée gravitaire avant pompe/UV ou directement dans le bassin en zone brassée, ombragée et accessible. Détection de perte de mesure et seuils d'alerte : basse < 4 deg C, haute > 28 deg C. Perte de mesure : `A11 - SONDE EAU ABSENTE`. Alerte informative uniquement en V1. |
| AI-002 | `TEMP_LOCAL` | Must | Retenu V1 | Sonde numérique simple ou équivalent adapté à la plateforme | Mesure la température de l'air du local de filtration, hors coffret, à distance des alimentations, contacteurs, pompes, soleil et volume chaud sous capot. Détection de perte de mesure et seuils d'alerte : basse < 2 deg C, haute > 40 deg C. Perte de mesure : `A12 - SONDE LOCAL ABSENTE`. Alerte informative uniquement en V1. |
| DI-014 | `POSITION_TAMBOUR` | Should | Option V2 ou V1.1 tardive | Capteur de position à étudier | Non retenu par défaut en V1.1. À ajouter seulement si l'indexation au temps pose problème ou si une position reproductible devient nécessaire. Non requis et non ajoute en V1 pour rester en diagnostic indirect. |
| AI-003 | `COURANT_TAMBOUR` | Should | Option V1.1 | Capteur de courant DC à étudier plus tard | La protection matérielle surintensité/blocage reste obligatoire en V1. Aucun capteur de courant dédié n'est ajouté en V1 ; seul un contact défaut simple fourni naturellement par le module de protection peut être exploité si cela ne complexifie pas le MVP. |

### Interface électrique KC868-A32 / CR18-8DN

Les capteurs de niveau `EP_LAVAGE` et `EP_CRITIQUE` sont câblés directement sur les entrées digitales du KC868-A32, sans relais d'interface par défaut.

Le schéma KC868-A32 montre que chaque entrée digitale `INPUT_Dx` pilote un optocoupleur `EL357`. La LED d'entrée est alimentée depuis `12VIN` au travers d'une résistance série `2 kΩ`, et l'entrée terrain est activée lorsque le signal est ramené vers `GND`. L'entrée est donc adaptée à un contact sec vers 0 V ou à une sortie collecteur ouvert NPN capable d'absorber ce courant.

La documentation CR18-8DN indique une sortie `NPN` 3 fils, normalement ouverte pour le suffixe `N`, avec alimentation `12-24 VDC`, plage admissible `10-30 VDC`, consommation capteur `10 mA` max, courant de sortie `200 mA` max et tension résiduelle `1,5 V` max. Le courant d'entrée attendu côté KC868 est de l'ordre de quelques mA avec la résistance `2 kΩ`, donc largement inférieur à la capacité de sortie du capteur.

| Fil CR18-8DN | Fonction | Raccordement V1 |
| --- | --- | --- |
| Marron | Alimentation capteur | `+12 VDC` capteurs, départ fusible 1 A |
| Bleu | 0 V capteur | `GND / 0 V` commun avec les entrées KC868-A32 |
| Noir | Sortie NPN | Entrée digitale KC868-A32, par exemple `INPUT_D1` pour `EP_LAVAGE` et `INPUT_D2` pour `EP_CRITIQUE` |

La logique firmware doit considérer ces entrées comme actives quand la sortie NPN tire l'entrée vers 0 V. Le sens logique exact lu via les expanseurs d'E/S du KC868-A32 doit être vérifié sur banc et inversé en logiciel si nécessaire.

Cette interface directe ne rend pas les capteurs de niveau intrinsèquement fail-safe : une rupture du fil noir ou une absence d'alimentation capteur peut ressembler à un capteur inactif. La V1 compense par cohérence logicielle entre `EP_LAVAGE` et `EP_CRITIQUE`, temporisations prudentes et essais périodiques de validation, mais ne prétend pas détecter toutes les ruptures de ligne sans capteur ou diagnostic supplémentaire.

## Sorties automate et puissance

| ID | Nom | Priorité | État | Interface cible | Comportement attendu |
| --- | --- | --- | --- | --- | --- |
| DO-001 | `CMD_TAMBOUR` | Must | Retenu, validation comportement à faire | Relais KC868-A32 vers relais HELLA 4RD 933 332-551, 12 V, charge inductive 15 A ; moteur Fyearfly 12 VDC 10 rpm ; fusible 7,5 A ; courant de blocage annoncé 6,5 A | Commande rotation tambour pendant lavage, test ou manuel autorisé. L'indexation est une extension V1.1. Inhibée en niveau critique, capot ouvert, maintenance ou défaut critique. |
| DO-002 | `CMD_RINCAGE` | Must | Retenu, schema à finaliser | Relais KC868-A32 vers bobine 230 VAC d'un contacteur Schneider TeSys LC1D12P7, 3P, AC-3 12 A ; pompe VEVOR / Leo EKJ-802S | Commande pompe de rinçage pendant lavage ou test. Inhibée en niveau critique, maintenance incompatible ou défaut critique. |
| DO-003 | `CMD_POMPE_FILTRATION` | Must | Retenu, schema à finaliser | Relais KC868-A32 vers contacteur TOMZN TOCT1-25Z 25 A, bobine 12 VDC | Autorise la pompe principale. Coupé en niveau critique, défaut critique ou stratégie de sécurité retenue. Depart 230 VAC separe de l'UV, de la pompe décoration et de la mise à niveau. |
| DO-004 | `CMD_POMPE_DECO` | Must | Retenu, schema à finaliser | Relais KC868-A32 vers contacteur TOMZN TOCT1-25Z 25 A, bobine 12 VDC | Autorise la pompe décoration. Suit exactement la même sécurité hydraulique que la pompe principale, car elle aspire au même endroit : OFF sur EP_CRITIQUE. |
| DO-005 | `CMD_UV` | Must | Retenu, schema à finaliser | Relais KC868-A32 vers contacteur TOMZN TOCT1-25Z 25 A, bobine 12 VDC | Autorise l'UV seulement si la filtration est autorisée et qu'aucun niveau critique n'est actif. L'ouverture du capot FAT ne coupe pas l'UV par elle-meme, car l'UV est hors tambour. Après EP_CRITIQUE acquitté, réautorisation après redémarrage filtration et courte stabilisation. |
| DO-006 | `CMD_MISE_A_NIVEAU` | Must | Retenu, schema à finaliser | Relais KC868-A32 vers contacteur TOMZN TOCT1-25Z 25 A, bobine 12 VDC ou sortie equivalente selon dispositif final | Coupé en niveau critique pour éviter un remplissage infini, masquer une fuite ou aggraver une situation hydraulique anormale. |
| DO-007 | `VOYANT_MARCHE` | Must | Retenu V1 | Voyant physique vert, LED ou voyant 12/24 V selon coffret | Indique marche contrôleur, système opérationnel ou auto OK selon le câblage retenu. Complément visuel minimal de l'écran. |
| DO-008 | `VOYANT_AUTO` | Should | À définir | LED, voyant ou IHM | Indique mode auto actif. |
| DO-009 | `VOYANT_MANUEL` | Should | À définir | LED, voyant ou IHM | Indique mode manuel actif. |
| DO-010 | `VOYANT_MAINTENANCE` | Should | À définir | LED, voyant ou IHM | Indique mode maintenance actif. |
| DO-011 | `VOYANT_DEGRADE` | Should | À définir | LED, voyant ou IHM | Indique fonctionnement dégradé. |
| DO-012 | `VOYANT_LAVAGE` | Should | Option V1 | Voyant physique jaune ou ambre, LED, voyant ou IHM | Indique cycle de lavage, test, dégradé ou maintenance si le câblage reste simple. Non bloquant car l'écran affiche le cycle. |
| DO-013 | `VOYANT_ALARME` | Must | Retenu V1 | Voyant physique rouge, LED ou voyant 12/24 V selon coffret | Signale une alarme active, un défaut critique ou `A15 - CAPOT OUVERT LONG`. Pour A15, le voyant rouge est fixe, sans clignotement et sans maintien après fermeture stable du capot. Complément visuel minimal de l'écran, lisible localement. |
| COM-001 | `IHM_ECRAN` | Must | Retenu V1, validation banc à faire | LCD 2004 / 20x4 I2C 3,3 V, fond bleu, module I2C type PCF8574 ou equivalent, raccordement cible sur port d'extension KC868-A32 `GPIO32` / `GPIO33` en I2C logiciel | Affiche au minimum mode actuel, état niveau, état lavage, alarme active et température eau. Affiche `MAINTENANCE - CAPOT OUVERT` tant que le capot est ouvert hors action dangereuse. Priorise EP_CRITIQUE/incohérence capteurs, capot dangereux, défaut lavage, capot ouvert trop longtemps, température puis infos. Messages au format `Axx - MESSAGE COURT`, liste minimale A01 à A15. `A15` disparaît automatiquement après fermeture stable et ne demande pas d'acquittement. Affiche la cause des resets refusés et tests refusés. Ne présente pas le bypass passif comme mesure ; utiliser MODE DÉGRADÉ - BYPASS SUPPOSÉ. Les détails peuvent être accessibles par page ou défilement. Ne pas connecter l'écran aux bus I2C internes des PCF8574 relais/entrées ; vérifier adresse I2C, contraste, rétroéclairage et absence de pull-up 5 V avant câblage final. |
| COM-002 | `RTC_DS3231` | Must | Retenu comme voie heure fiable V2, validation banc à faire | Module RTC DS3231 I2C 3,3 V avec batterie rechargeable, adresse attendue `0x68`, raccordement cible sur port d'extension KC868-A32 `GPIO32` / `GPIO33` en I2C logiciel si partage stable avec le LCD | Fournit une heure civile locale pour horodatage, statistiques, dernier lavage, test journalier, programmation horaire, notifications et synthese quotidienne. Le Wi-Fi/NTP peut recaler la RTC en V2 mais ne doit pas etre l'unique source. Si l'heure est invalide ou absente, le controleur reste fonctionnel avec compteurs persistants et horodatage inconnu. Les temporisations de securite et de cycle ne doivent pas dependre de la RTC. |

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
- `IHM_ECRAN` LCD 2004 / 20x4 I2C 3,3 V
- `RTC_DS3231` module RTC I2C 3,3 V avec batterie rechargeable pour l'heure fiable V2
- voyant marche vert et voyant alarme rouge comme complément visuel minimal
- voyant lavage jaune ou ambre optionnel si câblage simple

Les bulleurs de la cuve bio et du bassin ne sont pas pilotes par le contrôleur. Ils seront branchés directement sur le 220 V et ne doivent pas passer par un circuit coupé par la sécurité niveau critique.

## Groupes de sécurité de puissance

| Groupe | Équipements | Comportement en niveau critique | Remarque |
| --- | --- | --- | --- |
| Groupe coupé sécurité hydraulique | Pompe filtration, pompe décoration, UV, mise à niveau automatique | Coupé ou inhibition | Ces équipements peuvent aggraver une marche à sec, une vidange ou un fonctionnement hors eau. |
| Groupe lavage FAT | Tambour, pompe de rinçage | Inhibe | Aucun lavage ne doit être lance en situation d'eau insuffisante. |
| Protection moteur tambour | Fusible ATO 7,5 A, relais HELLA 12 V 15 A inductif | Coupure matérielle par fusible ; retour défaut automate non prévu en V1 | Courant de blocage annoncé 6,5 A d'après screenshot fournisseur ; calibre cohérent en première intention, comportement à vérifier par démarrage et blocage contrôlé. |
| Protection pompe rinçage | Disjoncteur 10 A courbe C et contacteur Schneider LC1D12P7 | Coupé ou défaut selon architecture retenue | Pompe classe I à raccorder à la terre ; tenir compte du courant d'appel moteur. |
| Groupe hors contrôleur | Bulleur cuve bio, bulleur bassin | Non piloté | Alimentation directe 220 V, hors sorties contrôlées et hors circuit coupé par niveau critique. |
| Groupe signalisation | Voyants, écran | Maintenu si possible | La signalisation doit rester disponible pour comprendre l'état de sécurité. |

## Protections électriques retenues

| Circuit | Protection / organe | Remarque |
| --- | --- | --- |
| Tete de tableau | Interrupteur differentiel 2P 30 mA, 40 A, type A | La notice AquaForte DM-Vario demande une protection 30 mA sans imposer type F ou B ; calibre 40 A coherent avec les charges reelles et les prises maintenance ponctuelles. |
| Alimentation 12 VDC | Disjoncteur 4 A courbe C | Alimente la Mean Well NDR-120-12. |
| Pompe de rincage | Disjoncteur 10 A courbe C | Alimente le depart pompe commande par contacteur Schneider. |
| Prises local | Disjoncteur 16 A courbe C | 1 prise bulleur bassin, 1 prise bulleur filtre bio, 2 prises maintenance ponctuelle. |
| Pompe filtration | Disjoncteur 6 A courbe C | Depart dedie organe essentiel. |
| UV, pompe decoration, mise a niveau | Disjoncteur 6 A courbe C | Depart separe de la filtration. |
| Eclairage exterieur | Disjoncteur 6 A courbe C | Depart dedie hors automatisme FAT, pour 6 spots LED exterieurs de 3 W avec detecteurs ; charge nominale 18 W, environ 0,08 A sous 230 VAC, cable environ 10 a 15 m. |
| Distribution 12 VDC moteur tambour | Fusible ATO 7,5 A | Moteur Fyearfly via relais HELLA. |
| Distribution 12 VDC automate | Fusible ATO 3 A | KC868-A32. |
| Distribution 12 VDC capteurs et boutons | Fusible ATO 1 A | Entrees terrain et commandes locales. |
| Distribution 12 VDC IHM et accessoires | Fusible ATO 1 A | Ecran, voyants, accessoires. |
| Fixation porte-fusibles ATO | Adaptateur rail DIN imprime en 3D si necessaire | A prevoir selon le modele exact du porte-fusibles 4 emplacements. |

## Points à arbitrer avant schéma électrique

- Choisir la référence finale de l'interrupteur differentiel 2P 30 mA, 40 A, type A.
- Verifier la compatibilite exacte entre sorties du KC868-A32, bobines de contacteurs TOMZN 12 VDC, bobine Schneider 230 VAC et relais HELLA.
- Valider sur banc le LCD 2004 I2C 3,3 V : adresse, lisibilite, contraste, retroeclairage, affectation `GPIO32` / `GPIO33` et compatibilite des niveaux logiques.
- Valider sur banc la RTC DS3231 I2C 3,3 V : adresse `0x68`, batterie rechargeable livree, conservation de l'heure apres coupure, absence de pull-up vers 5 V et cohabitation avec le LCD 2004 sur le bus I2C logiciel.
- Mesurer le courant réel du moteur Fyearfly a vide, en charge et au blocage pour confirmer le fusible 7,5 A.
- Definir les protections, borniers, sections de cable, reperages et cheminements separes entre basse tension et puissance.
- Valider sur banc la lecture KC868-A32 des deux CR18-8DN : repos, détection, fil noir débranché, bleu débranché, marron débranché et inversion logique firmware éventuelle.
- Finaliser le support rail DIN imprime en 3D du relais HELLA.
- Finaliser l'adaptateur rail DIN imprime en 3D du porte-fusibles ATO si le modele retenu n'est pas DIN natif.
- Confirmer les références exactes des sondes de température bassin et local.
- Vérifier que les bulleurs alimentés directement en 220 V restent électriquement séparés des circuits coupés par le contrôleur.
- Valider la section, le cheminement et la reference materielle du depart eclairage exterieur protege en 6 A courbe C.
- Évaluer empiriquement la consommation d'eau de rinçage à partir des essais.
- Conserver le capteur de position tambour comme option V2 ou V1.1 tardive tant que l'indexation au temps n'a pas montre de limité.
