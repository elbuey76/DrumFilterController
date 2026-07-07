# Backlog projet

## A cadrer

| ID | Element | Priorité | Statut |
| --- | --- | --- | --- |
| B-001 | Consolider sur le montage MVP les caractéristiques du moteur de tambour retenu Fyearfly 12 VDC 10 rpm : courant a vide, courant en charge, courant au démarrage, comportement en blocage avec courant annoncé 6,5 A, couple utile et vitesse finale tambour. | Must | En cours |
| B-002 | Consolider les caractéristiques de la pompe de rinçage VEVOR / Leo EKJ-802S et mesurer son point de fonctionnement réel sur la rampe et les buses déjà achetées et fabriquées. | Must | En cours |
| B-003 | Définir l'implantation des deux capteurs CR18-8DN V1, EP_LAVAGE et EP_CRITIQUE, en prévoyant une réserve mécanique pour un éventuel troisieme capteur futur si les essais le justifiént. | Must | Tranchée |
| B-004 | Définir l'environnement du coffret et l'indice de protection attendu. | Must | Tranchée |
| B-005 | Définir les cotes exactes du niveau normal côté sale, du niveau normal côté propre, du seuil de lavage et du niveau bas sur le tube de report en 32 mm. | Must | À faire |
| B-006 | Mesurer sur site la hauteur finale du support du FAT par rapport au niveau hydraulique du bassin avant fabrication. | Must | À faire |
| B-007 | Determiner si la logique de lavage repose sur une cote simple côté propre, sur une comparaison sale/propre, ou sur une combinaison des deux. | Must | Tranchée |
| B-008 | Ajouter des reperes visuels de niveau pour identifier d'un coup d'oeil l'état hydraulique du FAT, par exemple avec de petites cales plastiques collees aux niveaux de référence. | Should | À faire |
| B-009 | Prévoir un bypass passif ou mécanique entre eau sale et eau propre permettant de maintenir le passage vers la biofiltration en mode survie quand le lavage FAT est indisponible, tant que le niveau eau propre n'est pas critique. | Must | À faire |
| B-010 | Valider l'hypothèse de niveau normal côté sale autour de 20 cm au-dessus du bas du tambour soit environ 27 cm depuis le fond, avec ses reperes associés environ 64,5 % du diamètre en hauteur, environ 59 % de circonference immergée et environ 58 cm de longueur developpee immergée. | Must | À faire |
| B-011 | Calculer et valider avant découpe la forme, le nombre et la surface totale des ouvertures du tambour afin d'obtenir une surface de filtration utile d'environ 0,20 à 0,23 m2. | Must | À faire |
| B-012 | Définir precisement les modes auto, manuel, maintenance, dégradé, test et arrêt total avec leurs transitions autorisées. | Must | À faire |
| B-013 | Définir la politique de reprise après coupure de courant pour chaque mode et chaque sortie, avec reprise normale, reprise EP_LAVAGE seule et blocage EP_CRITIQUE. | Must | Tranchée |
| B-014 | Retenir la liste minimale d'auto-diagnostics indirects V1 : EP_CRITIQUE, incohérence niveau, lavage inefficace après 3 tentatives, capot dangereux, A15, commande UV incohérente, perte sondes température ; reporter absence anormale de lavage, lavage trop fréquent, moteur bloque, pompe rinçage HS et pression absente. | Must | Tranchée |
| B-015 | Garder le mode hiver automatique hors V1 ; conserver seulement les alertes température eau/local en V1 et reetudier les adaptations hiver en V1.1/V2 après observation réelle. | Should | Tranchée |
| B-016 | Définir la fonction de mesure de température bassin : sonde DS18B20 étanche inox 3 fils, longueur cible 3 m, implantation représentative de l'eau du bassin, plage utile et seuils d'alerte informative V1 < 4 deg C et > 28 deg C. | Must | Tranchée |
| B-017 | Définir la fonction de mesure de température ambiante local : même sonde DS18B20 étanche inox 3 fils que pour le bassin, montée en support ventilé dans l'air du local, plage utile et seuils d'alerte informative V1 < 2 deg C et > 40 deg C. | Must | Tranchée |
| B-022 | Figer les paramètres de lavage auto mini 10 s, maxi 45 s, rotation résiduelle 2 à 5 s, anti-redémarrage 30 à 120 s, 3 tentatives, pause 30 à 120 s, seuils horaire et journalier, avec retard EP_LAVAGE initial 5 à 15 s et confirmation EP_CRITIQUE initiale 0,5 à 2 s. | Must | En cours |
| B-018 | Retenir une IHM locale V1 avec LCD 2004 / 20x4 I2C 3,3 V, fond bleu, et commandes physiques essentielles. | Must | Tranchée |
| B-019 | Retenir les voyants complémentaires V1 du montage MVP : MARCHE vert, ALARME rouge et LAVAGE jaune, en voyants LED étanches 16 mm 12 VDC, en gardant l'écran comme support principal de lecture des états. | Should | Tranchée |
| B-020 | Retenir pour une V2 une remontée distante Wi-Fi avec notifications actionnables : EP_CRITIQUE, capteurs incohérents, capot dangereux, A15, lavage inefficace, retour courant après coupure et perte sonde température persistante. | Should | Tranchée |
| B-021 | Choisir pour le MVP une plateforme définitive compatible Wi-Fi V2 sans remplacement matériel principal ; Ethernet non requis, BLE seul insuffisant et SMS non retenu par défaut. | Must | Tranchée |
| B-023 | Définir le test journalier automatique V1.1 avec fenêtre configurable par défaut 11h-15h, report automatique si sécurité active, capot ouvert, EP_CRITIQUE, lavage en cours ou alarme bloquante, puis critères de succès et diagnostic attendu. | Should | Tranchée |
| B-024 | Retenir pour V1.1 une indexation tambour au temps, par courte rotation configurable après certains lavages réussis ou périodiquement, sans angle précis tant qu'aucun capteur position n'est retenu. | Should | Tranchée |
| B-025 | Définir les statistiques de lavage V1.1 en separant lavages automatiques réussis, tests, échecs et interruptions ; restitution locale simple : compteurs jour, dernier lavage et nombre d'échecs. | Should | Tranchée |
| B-026 | Retenir l'indice expérimental stable `lavages par heure x duree moyenne lavage`, sans action automatique ; alerte future seulement sur dérive relative après observation. | Should | Tranchée |
| B-027 | Estimer la consommation d'eau par `debit mesure aux buses x durée de rinçage cumulee`, avec pertes évacuation et besoin de remplissage indicatifs sans compteur d'eau dédié. | Should | Tranchée |
| B-028 | Retenir en V1.1 des compteurs horaires cumulés simples par organe principal ; remises à zéro maintenance et seuils de rappel reportés V2. | Should | Tranchée |
| B-029 | Définir la vue synthèse de l'IHM locale avec mode actuel, état niveau, état lavage, alarme active et température eau visibles sans navigation complexe, plus priorité d'affichage des alarmes V1. | Must | Tranchée |
| B-030 | Implémenter pour une V2 la politique anti-spam des notifications immédiates : apparition, rappel rare si toujours actif et retour à la normale pour les alarmes importantes. | Should | Tranchée |
| B-031 | Implémenter pour une V2 Wi-Fi une synthèse quotidienne désactivée par défaut, configurable, envoyee à 18h00 par défaut si activée, sur le même canal que les notifications immédiates. | Should | Tranchée |
| B-032 | Implémenter le pilotage horaire pompe décoration V1.1/V2 avec deux plages maximum par jour, mêmes horaires tous les jours, interrupteur actif/inactif et priorité sécurités, manuel local, distant, horaire. | Should | Tranchée |
| B-033 | Figer le nommage et l'ordre physique des capteurs EP_LAVAGE et EP_CRITIQUE ainsi que les règles d'incohérence associées, notamment EP_CRITIQUE actif avec EP_LAVAGE inactif comme défaut bloquant. | Must | En cours |
| B-034 | Définir la taxonomie finale des alarmes indirectes, leurs codes `Axx - MESSAGE COURT` et leurs formulations utilisateur non ambigues, dont A01 à A15 V1. | Must | Tranchée |
| B-035 | Documenter que rotation tambour, courant mesuré, fuite local et niveau eau sale restent hors diagnostic direct V1 faute de capteurs dedies ; les diagnostics logiciels restent basés sur effets observés. | Must | Tranchée |
| B-036 | Documenter que la pompe décoration suit les mêmes sécurités que la filtration car elle aspire au même endroit. | Must | Tranchée |
| B-037 | Prévoir une protection matérielle obligatoire contre la surintensité ou le blocage du moteur tambour ; remonter l'information à l'automate seulement si le module choisi le permet simplement. | Must | Tranchée |
| B-038 | Ne pas dépendre d'une fonction de parking ou de position moteur en V1 ; garder l'indexation au temps en V1.1 sauf ajout futur d'un capteur de position. | Should | Tranchée |
| B-039 | Mesurer le débit de rinçage de référence sur les buses et la rampe déjà fabriquées après montage réel de la pompe et prise en compte des pertes de charge ; utiliser la courbe pompe seulement comme estimation provisoire. | Must | Tranchée |
| B-040 | Ne pas ajouter de pressostat, débitmètre ou retour courant pour diagnostiquer le rinçage en V1 ; reporter ces capteurs V1.1/V2 si les essais montrent trop d'ambiguite. | Should | Tranchée |
| B-041 | Integrer au choix de plateforme V1 la capacité d'heure fiable V2 sans remplacement matériel principal, par RTC DS3231 I2C 3,3 V avec batterie rechargeable, synchronisation réseau seulement en complement, sans dépendance exclusive à Internet. | Must | Tranchée |
| B-042 | Retenir l'UV hors tambour, après la pompe principale, asservi a filtration autorisée plus absence EP_CRITIQUE, sans coupure sur défaut FAT non critique. | Must | Tranchée |
| B-043 | Retenir en tete de tableau un interrupteur differentiel 2P 30 mA, 40 A, type A, coherent avec les charges reelles du MVP et la notice AquaForte DM-Vario qui demande 30 mA sans imposer type F ou B. | Must | Tranchée |
| B-044 | Retenir une distribution 230 VAC avec departs : 4 A courbe C alimentation 12 V, 10 A courbe C pompe rincage, 16 A courbe C prises local, 6 A courbe C pompe filtration, 6 A courbe C UV/pompe decoration/mise a niveau, 6 A courbe C eclairage exterieur. | Must | Tranchée |
| B-045 | Retenir une distribution 12 VDC Mean Well NDR-120-12 + porte-fusibles ATO 4 departs : moteur 5 A, automate 3 A, capteurs/boutons 1 A, ecran/voyants/accessoires 1 A. | Must | Tranchée |

## FAT existant a finaliser

| ID | Element | Priorité | Statut |
| --- | --- | --- | --- |
| B-050 | Réaliser les ouvertures dans le tuyau servant de tambour après validation du calcul de surface utile. | Must | À faire |
| B-051 | Poser la maille inox autour du tambour. | Must | À faire |
| B-052 | Raccorder la rampe d'aspersion de 32 mm déjà fabriquée à la pompe de rinçage. | Must | À faire |
| B-053 | Percer la prise d'eau de la pompe de rinçage sur une paroi du filtre. | Must | À faire |
| B-054 | Percer et equiper la vidange du filtre. | Must | À faire |
| B-055 | Poser le joint a levre de séparation eau propre / eau sale autour du tambour. | Must | À faire |
| B-056 | Fabriquer le support du FAT pour respecter la cote de trop-plein mesurée sur site. | Must | À faire |
| B-057 | Fixer les capteurs sur le report de niveau en 32 mm. | Must | À faire |
| B-058 | Réaliser le capot transparent de fermeture avec détection d'ouverture par fin de course OMRCH ME-8104, permettant de voir le tambour tourner sans ouvrir le FAT. | Must | À faire |

## Prototype matériel

| ID | Element | Priorité | Statut |
| --- | --- | --- | --- |
| B-101 | Documenter le choix de plateforme V1 KC868-A32 et vérifier ses interfaces exactes avec capteurs, boutons, relais, contacteurs, écran et voyants. | Must | Tranchée |
| B-102 | Définir les entrées/sorties nécessaires au MVP, compatibles avec les capteurs CR18-8DN NPN 12-24 VDC. | Must | Tranchée |
| B-100 | Geler le nombre minimal d'entrées/sorties V1 avant de rechercher et comparer des références matérielles candidates. | Must | Tranchée |
| B-103 | Réaliser un schéma de câblage de principe intégrant differentiel 30 mA, disjoncteurs, Mean Well NDR-120-12, KC868-A32, fusibles ATO, relais HELLA, contacteur Schneider et contacteurs TOMZN. | Must | À faire |
| B-104 | Monter le coffret basse tension MVP avec boutons, voyant MARCHE vert 16 mm 12 VDC, voyant ALARME rouge 16 mm 12 VDC et voyant LAVAGE jaune 16 mm 12 VDC. | Should | À faire |
| B-105 | Réserver des sorties distinctes pour les équipements à couper sur niveau bas, sans integrer les bulleurs branchés directement sur le 220 V. | Must | À faire |
| B-106 | Définir l'architecture de contacteurs ou relais permettant la coupure de sécurité des équipements controles : HELLA moteur tambour, Schneider pompe rincage, TOMZN filtration/UV/deco/mise a niveau. | Must | Tranchée |
| B-107 | Définir l'interface électrique des entrées pour lecture fiable des capteurs CR18-8DN 3 fils : sortie NPN directe vers entrée optocouplée KC868-A32, marron +12 VDC, bleu 0 V commun, noir vers `INPUT_Dx`, avec validation banc avant câblage final. | Must | Tranchée |
| B-108 | Prévoir un sélecteur physique AUTO / MAINTENANCE type XB2-BD25 ou équivalent, 22 mm, 2 positions maintenues, contact `1NO + 1NC`, et des boutons poussoirs 22 mm momentanés `1NO1NC` : RESET_ALARME bleu, TEST_LAVAGE jaune, MANU_TAMBOUR noir et MANU_RINCAGE noir. | Must | Tranchée |
| B-109 | Prévoir l'entrée matérielle nécessaire pour la sonde `TEMP_BASSIN` DS18B20 3 fils : GPIO 1-Wire, alimentation 3,3 V, GND commun, pull-up 4,7 kΩ, cheminement câble et support démontable côté eau représentative. | Must | À faire |
| B-110 | Prévoir l'entrée matérielle nécessaire pour la sonde `TEMP_LOCAL` DS18B20 3 fils : GPIO 1-Wire, alimentation 3,3 V, GND commun, pull-up 4,7 kΩ, support ventilé hors coffret ; bus séparé préféré si GPIO disponible. | Must | À faire |
| B-110A | Valider sur banc les deux sondes DS18B20 étanches inox identiques : brochage réel des couleurs, lecture en 3,3 V, perte de sonde, identification eau/local, choix bus 1-Wire séparés ou commun et stabilité avec câble 3 m. | Must | À faire |
| B-110B | Prévoir et valider le contact capot OMRCH ME-8104 : entrée digitale KC868-A32, contact sec vers GND, bornes NO/NC choisies au multimètre, capot fermé boucle fermée, capot ouvert ou rupture boucle ouverte, came réglable et répétabilité mécanique. | Must | À faire |
| B-111 | Prévoir les composants nécessaires pour l'IHM locale V1 : LCD 2004 / 20x4 I2C 3,3 V fond bleu, sélecteur AUTO / MAINTENANCE 22 mm 2 positions maintenues `1NO + 1NC`, boutons poussoirs 22 mm momentanés `1NO1NC` bleu/jaune/noir/noir pour reset, test, tambour manuel et rinçage manuel, voyant MARCHE vert 16 mm 12 VDC, voyant ALARME rouge 16 mm 12 VDC et voyant LAVAGE jaune 16 mm 12 VDC. | Must | À faire |
| B-111A | Valider sur banc le LCD 2004 I2C retenu : adresse I2C, contraste, retroeclairage, lisibilite, affectation `GPIO32` / `GPIO33`, compatibilite 3,3 V des lignes `SDA` / `SCL` et absence de perturbation des bus I2C internes du KC868-A32. | Must | À faire |
| B-111B | Prévoir et valider sur banc la RTC DS3231 I2C 3,3 V avec batterie rechargeable : adresse `0x68`, conservation de l'heure après coupure, absence de pull-up 5 V, cohabitation avec le LCD 2004 sur `GPIO32` / `GPIO33` ou bus I2C separé si nécessaire. | Must | À faire |
| B-112 | Prévoir pour une V2 les composants et interfaces nécessaires à la remontée distante retenue module radio, réseau ou passerelle. | Should | V2 |
| B-113 | Garder le capteur de position tambour comme option V2 ou V1.1 tardive, seulement si l'indexation au temps pose problème ou si une position reproductible devient nécessaire. | Should | Tranchée |
| B-114 | Valider la commande 12 V DC du moteur tambour avec fusible ATO 5 A, relais HELLA 12 V 15 A inductif, alimentation Mean Well NDR-120-12 et courant de blocage annoncé 6,5 A. | Must | En cours |
| B-115 | Valider le sens de rotation, la connectique et la fixation mécanique du motorreducteur Fyearfly 12 VDC 10 rpm avant câblage définitif. | Must | À faire |
| B-116 | Valider la commande secteur de la pompe de rinçage EKJ-802S avec disjoncteur 10 A courbe C, contacteur Schneider LC1D18P7 bobine 230 VAC, terre et protection. | Must | En cours |
| B-117 | Concevoir et imprimer un support rail DIN pour le relais HELLA automobile, avec maintien mécanique, accès aux cosses et repérage clair. | Should | À faire |
| B-118 | Verifier la compatibilite des relais KC868-A32 avec les bobines des contacteurs TOMZN 12 VDC, la bobine Schneider 230 VAC et la commande du relais HELLA. | Must | À faire |
| B-119 | Concevoir et imprimer si necessaire un adaptateur rail DIN pour le porte-fusibles ATO 4 emplacements, avec maintien mécanique, accès aux fusibles et repérage des departs. | Should | À faire |

## Firmware

Statuts specifiques a la V0.1 firmware :

- `Implémenté simulateur` : code et tests host-side disponibles, sans validation materiel.
- `Partiel simulateur` : comportement commence ou simule, mais il manque une partie V0.1, une persistance, une interface reelle ou une validation banc.

| ID | Element | Priorité | Statut |
| --- | --- | --- | --- |
| B-201 | Créer le squelette du firmware. | Must | Implémenté simulateur |
| B-202 | Implémenter la machine à états initiale. | Must | Implémenté simulateur |
| B-203 | Ajouter les temporisations de sécurité : retard EP_LAVAGE configurable, anti-rebond très court EP_CRITIQUE, anti-rebond capot ouverture 100 à 500 ms, fermeture capot stable 1 à 2 s, retour niveau normal stable et acquittement après critique. | Must | Partiel simulateur |
| B-204 | Ajouter un mécanisme de configuration simple. | Should | Implémenté simulateur |
| B-205 | Ajouter journalisation des cycles et défauts. | Could | À faire |
| B-206 | Integrer la logique des capteurs de niveau, du capteur d'ouverture du capot transparent et des sécurités associées, avec contact capot normalement fermé fail-safe et stratégie prudente selon perte de confiance EP_CRITIQUE ou EP_LAVAGE. | Must | Partiel simulateur |
| B-207 | Implémenter le comportement des pompes et sorties auxiliaires sur niveau bas et capot ouvert : EP_CRITIQUE coupé filtration, décoration, UV et mise à niveau ; capot FAT coupé tambour/rinçage/lavage mais ne coupe pas l'UV si filtration autorisée. | Must | Partiel simulateur |
| B-208 | Implémenter l'inhibition de la rotation tambour et de la pompe de rinçage sur niveau bas. | Must | Implémenté simulateur |
| B-209 | Vérifier que les bulleurs cuve bio et bassin sont hors contrôleur et non impactés par l'état niveau bas. | Must | À faire |
| B-210 | Implémenter la politique de reprise après coupure d'alimentation sans attente indéfinie : boot sorties sûres, auto-test, reprise dégradée EP_LAVAGE seule avec filtration/UV si autorisés, blocage EP_CRITIQUE avec acquittement. | Must | Partiel simulateur |
| B-211 | Implémenter le mode manuel V1 limité aux commandes FAT : rotation tambour, rinçage, cycle test et reset alarme, avec interverrouillages critiques et refus capot ouvert pour MANU_TAMBOUR et MANU_RINCAGE. | Must | Implémenté simulateur |
| B-212 | Implémenter le mode maintenance avec inhibition partielle des alarmes et temporisation de reprise. | Must | Partiel simulateur |
| B-213 | Implémenter le mode dégradé et les auto-diagnostics minimaux. | Must | Partiel simulateur |
| B-214 | Implémenter le mode test lavage avec un seul cycle borné, autorisé en AUTO et MAINTENANCE sous préconditions, et verdict automatique : cycle exécute si EP_LAVAGE inactif au départ, niveau OK ou échec EP_LAVAGE actif si EP_LAVAGE était actif. | Must | Implémenté simulateur |
| B-214A | Implémenter les priorités opérateur V1 : capot ouvert prioritaire sur AUTO, commandes manuelles a action maintenue refusées capot ouvert sans alarme bloquante preventive, test lavage autonome refusé avec message si capot ouvert ou sécurité critique, et interrompu par sécurité pendant le cycle. | Must | Partiel simulateur |
| B-215 | Implémenter l'acquisition de température bassin, la détection de perte de mesure `A11 - SONDE EAU ABSENTE` et les alertes associées, seuils initiaux < 4 deg C et > 28 deg C. | Must | Partiel simulateur |
| B-216 | Implémenter l'acquisition de température ambiante local, la détection de perte de mesure `A12 - SONDE LOCAL ABSENTE` et les alertes associées, seuils initiaux < 2 deg C et > 40 deg C. | Must | Partiel simulateur |
| B-217 | Implémenter la logique de remontée de statut sur le LCD 2004 I2C local avec priorité EP_CRITIQUE/incohérence capteurs, capot dangereux, défaut lavage, capot ouvert trop longtemps, température puis infos et messages `Axx - MESSAGE COURT`. | Must | Partiel simulateur |
| B-217A | Implémenter l'alerte V1 capot ouvert trop longtemps avec temporisation configurable, valeur initiale 10 minutes, message `A15 - CAPOT OUVERT LONG`, allumage fixe du voyant rouge, sans clignotement, et effacement automatique après fermeture stable sans maintien du voyant. | Must | Partiel simulateur |
| B-217B | Implémenter l'état IHM `MAINTENANCE - CAPOT OUVERT` et le retour automatique au mode demande après fermeture stable si aucune alarme bloquante capot dangereux n'existe. | Must | Implémenté simulateur |
| B-217C | Conserver une trace minimale persistante et non bloquante de `A15 - CAPOT OUVERT LONG`, écrite au moment de l'événement et conservée après coupure, sous forme de compteur persistant plus dernier événement si simple ; horodater le dernier événement seulement si la RTC DS3231 est disponible et initialisee facilement dans le MVP. Une mémoire circulaire courte est acceptable, sans historique long. | Must | Implémenté simulateur |
| B-217D | Au redémarrage, redétecter `A15 - CAPOT OUVERT LONG` si l'alerte était active avant coupure et que le capot est encore ouvert ; sinon relancer la temporisation A15 si le capot est ouvert. | Must | Implémenté simulateur |
| B-217E | Implémenter le mini-journal persistant V1 : compteurs par code d'événement plus dernier événement global, ou mémoire circulaire 8/16 événements si simple ; persister A15, EP_CRITIQUE, capot ouvert dangereux, lavage inefficace, redémarrage après coupure et acquittements réussis des alarmes bloquantes. | Must | Implémenté simulateur |
| B-218 | Implémenter pour une V2 la logique de remontée d'état et de notifications à distance selon l'architecture retenue. | Should | V2 |
| B-219 | Implémenter la logique de lavage avec durée mini 10 s, durée maxi 45 s, rotation résiduelle 2 à 5 s, anti-redémarrage 30 à 120 s, pause entre tentatives 30 à 120 s et défaut après 3 tentatives max. | Must | Implémenté simulateur |
| B-220 | Implémenter le comptage des lavages par heure et par jour ainsi que les alertes associées. | Should | V1.1 |
| B-221 | Implémenter le test journalier automatique V1.1 avec verdict explicite OK/ÉCHEC, fenêtre 11h-15h par défaut, report sur condition d'inhibition et journalisation du verdict. | Should | V1.1 |
| B-222 | Implémenter l'indexation périodique au temps du tambour par courte rotation configurable, en respectant les verrouillages de sécurité. | Should | V1.1 |
| B-223 | Implémenter les statistiques V1.1 en catégories séparées : lavages automatiques réussis, tests, échecs et interruptions, avec affichage local simple. | Should | V1.1 |
| B-224 | Implémenter l'indice d'encrassement expérimental stable `lavages par heure x duree moyenne lavage`, sans action automatique. | Should | V1.1/V2 |
| B-225 | Implémenter le suivi empirique de consommation d'eau par débit mesuré aux buses x durée de rinçage cumulée, avec pertes et appoint marques comme estimatifs. | Should | V1.1/V2 |
| B-226 | Implémenter les compteurs horaires cumulés simples des organes principaux, sans remise à zéro maintenance ni seuil de rappel en V1.1. | Should | V1.1 |
| B-227 | Implémenter l'affichage local des états, températures, dernier lavage et compteurs utiles retenus. | Should | V1.1 |
| B-228 | Implémenter pour une V2 l'emission des notifications immédiates retenues, dont `A15 - CAPOT OUVERT LONG`, avec anti-repetition et gestion du retour à la normale. | Should | V2 |
| B-229 | Implémenter pour une V2 la synthèse quotidienne de fonctionnement désactivée par défaut, configurable, avec horaire par défaut 18h00 et independance vis-a-vis des notifications immédiates. | Should | V2 |
| B-230 | Implémenter la programmation horaire de la pompe décoration avec deux plages maximum par jour, mêmes horaires tous les jours, interrupteur actif/inactif et sans automatisme hiver au départ. | Should | V1.1/V2 |
| B-231 | Implémenter la logique d'alarmes indirectes basées sur EP_LAVAGE, EP_CRITIQUE et le résultat observable des cycles de lavage, avec reset refusé tant que EP_LAVAGE reste actif après défaut lavage maintenu. | Must | Implémenté simulateur |
| B-232 | Implémenter la détection capteurs incohérents, le comportement au démarrage avec niveau bas et la mise en sécurité associée, dont EP_CRITIQUE actif avec EP_LAVAGE inactif comme défaut bloquant. | Must | Implémenté simulateur |
| B-233 | Implémenter les alertes de commande incohérente UV, absence anormale de lavage, redémarrages fréquents et sorties commandées trop longtemps. | Should | V1.1 |
| B-234 | Etendre les statistiques avec temps de retour EP_LAVAGE, tentatives par lavage, activations EP_CRITIQUE, températures min/max/moyenne et historiques capot. | Should | V1.1 |
| B-235 | Integrer le temps civil RTC DS3231 pour les logs, statistiques, dernier lavage, test journalier et programmation horaire, avec heure inconnue si RTC absente/non initialisee et sans utiliser la RTC pour les temporisations de securite. | Should | V1.1/V2 |

## Documentation et validation

| ID | Element | Priorité | Statut |
| --- | --- | --- | --- |
| B-301 | Rediger un plan de tests V1 trace exigence par exigence, avec matrice unique de couverture et fiches détaillées pour les essais complexes. | Must | En cours |
| B-302 | Décrire les scénarios de défaut a tester dans le plan de tests, en les reliant aux exigences et aux critères d'acceptation. | Must | À faire |
| B-303 | Documenter le câblage final. | Should | À faire |
| B-304 | Valider par test que EP_CRITIQUE coupé bien filtration, pompe décoration, UV et mise à niveau auto, puis que l'UV ne repart qu'après filtration et stabilisation, sans impact sur les deux bulleurs branchés hors contrôleur. | Must | À faire |
| B-305 | Valider par test le comportement de reprise après coupure de courant : démarrage nominal, EP_LAVAGE seule, lavage de reprise inefficace, EP_CRITIQUE actif, capot ouvert, log A15 conserve et réaffichage A15 si la condition reste présente. | Must | À faire |
| B-306 | Valider par test l'entrée en maintenance à l'ouverture du capot transparent, avec contact ME-8104 câblé fail-safe, rupture vue ouverte, anti-rebond ouverture/fermeture, affichage `MAINTENANCE - CAPOT OUVERT`, retour automatique après fermeture stable sans alarme bloquante, arrêt tambour/rinçage, interdiction lavage auto, refus des commandes manuelles tambour/rinçage capot ouvert et maintien UV si filtration autorisée. | Must | À faire |
| B-307 | Valider par test les scénarios de mode dégradé retenus. | Must | À faire |
| B-308 | Valider par test le mode TEST_LAVAGE : autorisation AUTO/MAINTENANCE sous préconditions, refus capot ouvert ou sécurité critique, un seul cycle borné, verdict `TEST OK - CYCLE EXÉCUTÉ` si EP_LAVAGE inactif au départ, verdict niveau OK ou échec si EP_LAVAGE actif, et aucune relance multiple. | Must | À faire |
| B-309 | Valider par test la mesure de température bassin, les alertes sur seuil < 4 deg C et > 28 deg C, le caractère informatif et la détection de perte de sonde A11. | Must | À faire |
| B-310 | Valider par test la mesure de température ambiante local, les alertes sur seuil < 2 deg C et > 40 deg C, le caractère informatif et la détection de perte de sonde A12. | Must | À faire |
| B-311 | Valider par test que l'IHM locale remonte correctement les modes, alarmes A01 à A15, défauts, cycles en cours, resets refusés explicites, tests refusés explicites, voyant rouge sur A15, effacement automatique A15 et bypass supposé. | Must | À faire |
| B-312 | Valider pour une V2 par test que les notifications à distance sont pertinentes, non répétitives et sans impact sur le fonctionnement local. | Should | V2 |
| B-313 | Valider par test les cas lavage reussi, lavage minimum 10 s, rotation résiduelle 2 à 5 s, anti-redémarrage 30 à 120 s, retour EP_LAVAGE pendant anti-redémarrage, lavage a durée maxi 45 s, relance après pause 30 à 120 s, puis défaut critique après 3 tentatives max. | Must | À faire |
| B-314 | Valider par test le test journalier automatique, son verdict et son inhibition dans les cas non autorisés. | Should | V1.1 |
| B-315 | Valider par test que l'indexation du tambour change bien la zone immergée sans perturber l'exploitation ni la sécurité. | Should | V1.1 |
| B-316 | Valider par test la cohérence des statistiques de lavage et leur consolidation sur 7 jours et 30 jours. | Should | V1.1 |
| B-317 | Valider par test la cohérence et la stabilite de l'indice d'encrassement dans le temps. | Should | V1.1/V2 |
| B-318 | Valider par test la cohérence des indicateurs de consommation d'eau et leur etiquetage comme estimations empiriques. | Should | V1.1/V2 |
| B-319 | Valider par test la cohérence des compteurs de temps de fonctionnement et leur persistance. | Should | V1.1 |
| B-320 | Valider par test la lisibilité et la cohérence des informations affichées sur l'IHM locale. | Should | À faire |
| B-321 | Valider pour une V2 par test que les notifications immédiates partent sur les bons événements, sans spam et avec retour à la normale cohérent. | Should | V2 |
| B-322 | Valider pour une V2 par test le contenu, l'horaire par défaut 18h00, la desactivation par défaut et la desactivation de la synthèse quotidienne sans impact sur les alertes critiques. | Should | V2 |
| B-323 | Valider par test la programmation horaire de la pompe décoration, ses deux plages maximum par jour, son interrupteur actif/inactif et sa priorité derriere sécurités, manuel local et commande distante. | Should | V1.1/V2 |
| B-324 | Valider par test la détection des incohérences EP_LAVAGE et EP_CRITIQUE, le filtrage EP_LAVAGE 5 à 15 s, l'anti-rebond EP_CRITIQUE 0,5 à 2 s, le reset refusé sur EP_LAVAGE persistant et la mise en sécurité associée. | Must | À faire |
| B-325 | Valider par test qu'un démarrage avec EP_LAVAGE ou EP_CRITIQUE actifs applique la stratégie retenue : reprise dégradée avec lavage si EP_LAVAGE seule, état sûr bloqué si EP_CRITIQUE. | Must | À faire |
| B-326 | Valider par test que les messages d'alarme restent indirects et n'affirment pas une panne d'organe non instrumentée, notamment lavage inefficace au lieu de pompe rinçage HS ou tambour bloque. | Must | À faire |
| B-327 | Valider par test les alertes d'absence anormale de lavage, de redémarrages fréquents et de sortie commandée trop longtemps. | Should | V1.1 |
| B-328 | Mesurer le courant moteur tambour a vide, avec tambour à sec, avec tambour en eau et au démarrage. | Must | À faire |
| B-329 | Mesurer la vitesse réelle du moteur Fyearfly et la vitesse finale du tambour avec la transmission retenue. | Must | À faire |
| B-330 | Réaliser un test de blocage contrôlé du tambour pour définir ou confirmer le seuil de protection surintensité. | Must | À faire |
| B-331 | Mesurer le débit réel de la pompe de rinçage aux buses pendant un cycle type. | Must | À faire |
| B-332 | Vérifier la qualité du jet de rinçage, l'amorçage, le reamorçage et l'absence d'échauffement anormal sur cycles répétés. | Must | À faire |
| B-333 | Ajuster l'estimation de consommation d'eau de rinçage à partir du débit réel et des durées de cycle. | Should | À faire |
