# Backlog projet

## A cadrer

| ID | Element | Priorite | Statut |
| --- | --- | --- | --- |
| B-001 | Consolider les caracteristiques du moteur de tambour candidat SWF 403.835 : 12 V DC, puissance estimee 40 W, courant, vitesse, couple et brochage. | Must | En cours |
| B-002 | Consolider les caracteristiques de la pompe de rincage VEVOR / Leo EKJ-802S et mesurer son point de fonctionnement reel aux buses. | Must | En cours |
| B-003 | Definir l'implantation des deux capteurs CR18-8DN V1, EP_LAVAGE et EP_CRITIQUE, en prevoyant une reserve mecanique pour un eventuel troisieme capteur futur si les essais le justifient. | Must | En cours |
| B-004 | Definir l'environnement du coffret et l'indice de protection attendu. | Must | En cours |
| B-005 | Definir les cotes exactes du niveau normal cote sale, du niveau normal cote propre, du seuil de lavage et du niveau bas sur le tube de report en 32 mm. | Must | A faire |
| B-006 | Mesurer sur site la hauteur finale du support du FAT par rapport au niveau hydraulique du bassin avant fabrication. | Must | A faire |
| B-007 | Determiner si la logique de lavage repose sur une cote simple cote propre, sur une comparaison sale/propre, ou sur une combinaison des deux. | Must | Tranchee |
| B-008 | Ajouter des reperes visuels de niveau pour identifier d'un coup d'oeil l'etat hydraulique du FAT, par exemple avec de petites cales plastiques collees aux niveaux de reference. | Should | A faire |
| B-009 | Prevoir un bypass passif ou mecanique entre eau sale et eau propre permettant de maintenir le passage vers la biofiltration en mode survie quand le lavage FAT est indisponible, tant que le niveau eau propre n'est pas critique. | Must | A faire |
| B-010 | Valider l'hypothese de niveau normal cote sale autour de 20 cm au-dessus du bas du tambour soit environ 27 cm depuis le fond, avec ses reperes associes environ 64,5 % du diametre en hauteur, environ 59 % de circonference immergee et environ 58 cm de longueur developpee immergee. | Must | A faire |
| B-011 | Calculer et valider avant decoupe la forme, le nombre et la surface totale des ouvertures du tambour afin d'obtenir une surface de filtration utile d'environ 0,20 a 0,23 m2. | Must | A faire |
| B-012 | Definir precisement les modes auto, manuel, maintenance, degrade, test et arret total avec leurs transitions autorisees. | Must | A faire |
| B-013 | Definir la politique de reprise apres coupure de courant pour chaque mode et chaque sortie, avec reprise normale, reprise EP_LAVAGE seule et blocage EP_CRITIQUE. | Must | Tranchee |
| B-014 | Retenir la liste minimale d'auto-diagnostics indirects V1 : EP_CRITIQUE, incoherence niveau, lavage inefficace apres 3 tentatives, capot dangereux, A15, commande UV incoherente, perte sondes temperature ; reporter absence anormale de lavage, lavage trop frequent, moteur bloque, pompe rincage HS et pression absente. | Must | Tranchee |
| B-015 | Garder le mode hiver automatique hors V1 ; conserver seulement les alertes temperature eau/local en V1 et reetudier les adaptations hiver en V1.1/V2 apres observation reelle. | Should | Tranchee |
| B-016 | Definir la fonction de mesure de temperature bassin : sonde numerique etanche candidate, implantation representative de l'eau du bassin, plage utile et seuils d'alerte informative V1 < 4 deg C et > 28 deg C. | Must | Tranchee |
| B-017 | Definir la fonction de mesure de temperature ambiante local : sonde numerique candidate, implantation representative de l'air du local, plage utile et seuils d'alerte informative V1 < 2 deg C et > 40 deg C. | Must | Tranchee |
| B-022 | Figer les parametres de lavage auto mini 10 s, maxi 45 s, rotation residuelle 2 a 5 s, anti-redemarrage 30 a 120 s, 3 tentatives, pause 30 a 120 s, seuils horaire et journalier, avec retard EP_LAVAGE initial 5 a 15 s et confirmation EP_CRITIQUE initiale 0,5 a 2 s. | Must | En cours |
| B-018 | Retenir une IHM locale V1 avec ecran texte ou petit afficheur et commandes physiques essentielles. | Must | Tranchee |
| B-019 | Retenir les voyants complementaires V1 : MARCHE vert et ALARME rouge obligatoires, LAVAGE jaune ou ambre optionnel, en gardant l'ecran comme support principal de lecture des etats. | Should | Tranchee |
| B-020 | Retenir pour une V2 une remontee distante Wi-Fi avec notifications actionnables : EP_CRITIQUE, capteurs incoherents, capot dangereux, A15, lavage inefficace, retour courant apres coupure et perte sonde temperature persistante. | Should | Tranchee |
| B-021 | Choisir pour le MVP une plateforme compatible Wi-Fi V2 sans remplacement materiel principal ; Ethernet non requis, BLE seul insuffisant et SMS non retenu par defaut. | Must | Tranchee |
| B-023 | Definir le test journalier automatique V1.1 avec fenetre configurable par defaut 11h-15h, report automatique si securite active, capot ouvert, EP_CRITIQUE, lavage en cours ou alarme bloquante, puis criteres de succes et diagnostic attendu. | Should | Tranchee |
| B-024 | Retenir pour V1.1 une indexation tambour au temps, par courte rotation configurable apres certains lavages reussis ou periodiquement, sans angle precis tant qu'aucun capteur position n'est retenu. | Should | Tranchee |
| B-025 | Definir les statistiques de lavage V1.1 en separant lavages automatiques reussis, tests, echecs et interruptions ; restitution locale simple : compteurs jour, dernier lavage et nombre d'echecs. | Should | Tranchee |
| B-026 | Retenir l'indice experimental stable `lavages par heure x duree moyenne lavage`, sans action automatique ; alerte future seulement sur derive relative apres observation. | Should | Tranchee |
| B-027 | Estimer la consommation d'eau par `debit mesure aux buses x duree de rincage cumulee`, avec pertes evacuation et besoin de remplissage indicatifs sans compteur d'eau dedie. | Should | Tranchee |
| B-028 | Retenir en V1.1 des compteurs horaires cumules simples par organe principal ; remises a zero maintenance et seuils de rappel reportes V2. | Should | Tranchee |
| B-029 | Definir la vue synthese de l'IHM locale avec mode actuel, etat niveau, etat lavage, alarme active et temperature eau visibles sans navigation complexe, plus priorite d'affichage des alarmes V1. | Must | Tranchee |
| B-030 | Implementer pour une V2 la politique anti-spam des notifications immediates : apparition, rappel rare si toujours actif et retour a la normale pour les alarmes importantes. | Should | Tranchee |
| B-031 | Implementer pour une V2 Wi-Fi une synthese quotidienne desactivee par defaut, configurable, envoyee a 18h00 par defaut si activee, sur le meme canal que les notifications immediates. | Should | Tranchee |
| B-032 | Implementer le pilotage horaire pompe decoration V1.1/V2 avec deux plages maximum par jour, memes horaires tous les jours, interrupteur actif/inactif et priorite securites, manuel local, distant, horaire. | Should | Tranchee |
| B-033 | Figer le nommage et l'ordre physique des capteurs EP_LAVAGE et EP_CRITIQUE ainsi que les regles d'incoherence associees, notamment EP_CRITIQUE actif avec EP_LAVAGE inactif comme defaut bloquant. | Must | En cours |
| B-034 | Definir la taxonomie finale des alarmes indirectes, leurs codes `Axx - MESSAGE COURT` et leurs formulations utilisateur non ambigues, dont A01 a A15 V1. | Must | Tranchee |
| B-035 | Documenter que rotation tambour, courant mesure, fuite local et niveau eau sale restent hors diagnostic direct V1 faute de capteurs dedies ; les diagnostics logiciels restent bases sur effets observes. | Must | Tranchee |
| B-036 | Documenter que la pompe decoration suit les memes securites que la filtration car elle aspire au meme endroit. | Must | Tranchee |
| B-037 | Prevoir une protection materielle obligatoire contre la surintensite ou le blocage du moteur tambour ; remonter l'information a l'automate seulement si le module choisi le permet simplement. | Must | Tranchee |
| B-038 | Isoler ou ignorer proprement les fonctions de parking du moteur d'essuie-glace en V1, sauf contrainte de brochage simple ; reetudier leur usage en V1.1 pour indexation tambour. | Should | Tranchee |
| B-039 | Mesurer le debit de rincage de reference aux buses apres montage reel de la pompe, de la rampe et des pertes de charge ; utiliser la courbe pompe seulement comme estimation provisoire. | Must | Tranchee |
| B-040 | Ne pas ajouter de pressostat, debitmetre ou retour courant pour diagnostiquer le rincage en V1 ; reporter ces capteurs V1.1/V2 si les essais montrent trop d'ambiguite. | Should | Tranchee |
| B-041 | Integrer au choix de plateforme V1 la capacite d'heure fiable V2 sans remplacement materiel principal, par RTC, temps local conserve, module temps, synchronisation reseau ou equivalent, sans dependance exclusive a Internet. | Must | Tranchee |
| B-042 | Retenir l'UV hors tambour, apres la pompe principale, asservi a filtration autorisee plus absence EP_CRITIQUE, sans coupure sur defaut FAT non critique. | Must | Tranchee |

## FAT existant a finaliser

| ID | Element | Priorite | Statut |
| --- | --- | --- | --- |
| B-050 | Realiser les ouvertures dans le tuyau servant de tambour apres validation du calcul de surface utile. | Must | A faire |
| B-051 | Poser la maille inox autour du tambour. | Must | A faire |
| B-052 | Raccorder la rampe d'aspersion de 32 mm a la pompe de rincage. | Must | A faire |
| B-053 | Percer la prise d'eau de la pompe de rincage sur une paroi du filtre. | Must | A faire |
| B-054 | Percer et equiper la vidange du filtre. | Must | A faire |
| B-055 | Poser le joint a levre de separation eau propre / eau sale autour du tambour. | Must | A faire |
| B-056 | Fabriquer le support du FAT pour respecter la cote de trop-plein mesuree sur site. | Must | A faire |
| B-057 | Fixer les capteurs sur le report de niveau en 32 mm. | Must | A faire |
| B-058 | Realiser le capot transparent de fermeture avec detection d'ouverture, permettant de voir le tambour tourner sans ouvrir le FAT. | Must | A faire |

## Prototype materiel

| ID | Element | Priorite | Statut |
| --- | --- | --- | --- |
| B-101 | Comparer les architectures de controle V1, notamment automate compact / module industriel DIN et carte microcontroleur type ESP32 ou Arduino industriel, selon cout, effort de developpement, robustesse et maintenabilite. | Must | A faire |
| B-102 | Definir les entrees/sorties necessaires au prototype, compatibles avec les capteurs CR18-8DN NPN 12-24 VDC. | Must | Tranchee |
| B-100 | Geler le nombre minimal d'entrees/sorties V1 avant de rechercher et comparer des references materielles candidates. | Must | Tranchee |
| B-103 | Realiser un schema de cablage de principe. | Must | A faire |
| B-104 | Monter un prototype basse tension avec boutons, voyant MARCHE vert, voyant ALARME rouge et voyant LAVAGE optionnel jaune ou ambre. | Should | A faire |
| B-105 | Reserver des sorties distinctes pour les equipements a couper sur niveau bas, sans integrer les bulleurs branches directement sur le 220 V. | Must | A faire |
| B-106 | Definir l'architecture de contacteurs ou relais permettant la coupure de securite des equipements controles. | Must | A faire |
| B-107 | Definir l'interface electrique des entrees pour lecture fiable des capteurs CR18-8DN 3 fils. | Must | A faire |
| B-108 | Prevoir un selecteur physique simple AUTO / MAINTENANCE et des boutons physiques momentanes pour TEST_LAVAGE, RESET_ALARME, MANU_TAMBOUR et MANU_RINCAGE. | Must | Tranchee |
| B-109 | Prevoir l'entree materielle necessaire pour la sonde de temperature bassin et son cablage. | Must | A faire |
| B-110 | Prevoir l'entree materielle necessaire pour la sonde de temperature ambiante local et son cablage. | Must | A faire |
| B-111 | Prevoir les composants necessaires pour l'IHM locale V1 : ecran texte ou petit afficheur, bouton reset alarme dedie, commandes physiques essentielles, voyant MARCHE vert, voyant ALARME rouge et voyant LAVAGE optionnel. | Must | A faire |
| B-112 | Prevoir pour une V2 les composants et interfaces necessaires a la remontee distante retenue module radio, reseau ou passerelle. | Should | V2 |
| B-113 | Garder le capteur de position tambour comme option V2 ou V1.1 tardive, seulement si l'indexation au temps pose probleme ou si une position reproductible devient necessaire. | Should | Tranchee |
| B-114 | Dimensionner la commande 12 V DC du moteur tambour avec fusible, relais/contacteur DC et alimentation adaptes aux appels de courant. | Must | A faire |
| B-115 | Identifier au multimetre le brochage exact du moteur tambour 5 broches avant tout cablage definitif. | Must | A faire |
| B-116 | Dimensionner la commande secteur de la pompe de rincage EKJ-802S avec organe de coupure adapte aux charges moteur, terre et protection. | Must | A faire |

## Firmware

| ID | Element | Priorite | Statut |
| --- | --- | --- | --- |
| B-201 | Creer le squelette du firmware. | Must | A faire |
| B-202 | Implementer la machine a etats initiale. | Must | A faire |
| B-203 | Ajouter les temporisations de securite : retard EP_LAVAGE configurable, anti-rebond tres court EP_CRITIQUE, anti-rebond capot ouverture 100 a 500 ms, fermeture capot stable 1 a 2 s, retour niveau normal stable et acquittement apres critique. | Must | A faire |
| B-204 | Ajouter un mecanisme de configuration simple. | Should | A faire |
| B-205 | Ajouter journalisation des cycles et defauts. | Could | A faire |
| B-206 | Integrer la logique des capteurs de niveau, du capteur d'ouverture du capot transparent et des securites associees, avec contact capot normalement ferme fail-safe et strategie prudente selon perte de confiance EP_CRITIQUE ou EP_LAVAGE. | Must | A faire |
| B-207 | Implementer le comportement des pompes et sorties auxiliaires sur niveau bas et capot ouvert : EP_CRITIQUE coupe filtration, decoration, UV et mise a niveau ; capot FAT coupe tambour/rincage/lavage mais ne coupe pas l'UV si filtration autorisee. | Must | A faire |
| B-208 | Implementer l'inhibition de la rotation tambour et de la pompe de rincage sur niveau bas. | Must | A faire |
| B-209 | Verifier que les bulleurs cuve bio et bassin sont hors controleur et non impactes par l'etat niveau bas. | Must | A faire |
| B-210 | Implementer la politique de reprise apres coupure d'alimentation sans attente indefinie : boot sorties sures, auto-test, reprise degradee EP_LAVAGE seule avec filtration/UV si autorises, blocage EP_CRITIQUE avec acquittement. | Must | A faire |
| B-211 | Implementer le mode manuel V1 limite aux commandes FAT : rotation tambour, rincage, cycle test et reset alarme, avec interverrouillages critiques et refus capot ouvert pour MANU_TAMBOUR et MANU_RINCAGE. | Must | A faire |
| B-212 | Implementer le mode maintenance avec inhibition partielle des alarmes et temporisation de reprise. | Must | A faire |
| B-213 | Implementer le mode degrade et les auto-diagnostics minimaux. | Must | A faire |
| B-214 | Implementer le mode test lavage avec un seul cycle borne, autorise en AUTO et MAINTENANCE sous preconditions, et verdict automatique : cycle execute si EP_LAVAGE inactif au depart, niveau OK ou echec EP_LAVAGE actif si EP_LAVAGE etait actif. | Must | A faire |
| B-214A | Implementer les priorites operateur V1 : capot ouvert prioritaire sur AUTO, commandes manuelles a action maintenue refusees capot ouvert sans alarme bloquante preventive, test lavage autonome refuse avec message si capot ouvert ou securite critique, et interrompu par securite pendant le cycle. | Must | Tranchee |
| B-215 | Implementer l'acquisition de temperature bassin, la detection de perte de mesure `A11 - SONDE EAU ABSENTE` et les alertes associees, seuils initiaux < 4 deg C et > 28 deg C. | Must | A faire |
| B-216 | Implementer l'acquisition de temperature ambiante local, la detection de perte de mesure `A12 - SONDE LOCAL ABSENTE` et les alertes associees, seuils initiaux < 2 deg C et > 40 deg C. | Must | A faire |
| B-217 | Implementer la logique de remontee de statut sur l'IHM locale avec priorite EP_CRITIQUE/incoherence capteurs, capot dangereux, defaut lavage, capot ouvert trop longtemps, temperature puis infos et messages `Axx - MESSAGE COURT`. | Must | A faire |
| B-217A | Implementer l'alerte V1 capot ouvert trop longtemps avec temporisation configurable, valeur initiale 10 minutes, message `A15 - CAPOT OUVERT LONG`, allumage fixe du voyant rouge, sans clignotement, et effacement automatique apres fermeture stable sans maintien du voyant. | Must | A faire |
| B-217B | Implementer l'etat IHM `MAINTENANCE - CAPOT OUVERT` et le retour automatique au mode demande apres fermeture stable si aucune alarme bloquante capot dangereux n'existe. | Must | A faire |
| B-217C | Conserver une trace minimale persistante et non bloquante de `A15 - CAPOT OUVERT LONG`, ecrite au moment de l'evenement et conservee apres coupure, sous forme de compteur persistant plus dernier evenement si simple ; horodater le dernier evenement seulement si une horloge fiable existe facilement dans le MVP. Une memoire circulaire courte est acceptable, sans historique long. | Must | A faire |
| B-217D | Au redemarrage, redetecter `A15 - CAPOT OUVERT LONG` si l'alerte etait active avant coupure et que le capot est encore ouvert ; sinon relancer la temporisation A15 si le capot est ouvert. | Must | A faire |
| B-217E | Implementer le mini-journal persistant V1 : compteurs par code d'evenement plus dernier evenement global, ou memoire circulaire 8/16 evenements si simple ; persister A15, EP_CRITIQUE, capot ouvert dangereux, lavage inefficace, redemarrage apres coupure et acquittements reussis des alarmes bloquantes. | Must | A faire |
| B-218 | Implementer pour une V2 la logique de remontee d'etat et de notifications a distance selon l'architecture retenue. | Should | V2 |
| B-219 | Implementer la logique de lavage avec duree mini 10 s, duree maxi 45 s, rotation residuelle 2 a 5 s, anti-redemarrage 30 a 120 s, pause entre tentatives 30 a 120 s et defaut apres 3 tentatives max. | Must | A faire |
| B-220 | Implementer le comptage des lavages par heure et par jour ainsi que les alertes associees. | Should | V1.1 |
| B-221 | Implementer le test journalier automatique V1.1 avec verdict explicite OK/ECHEC, fenetre 11h-15h par defaut, report sur condition d'inhibition et journalisation du verdict. | Should | V1.1 |
| B-222 | Implementer l'indexation periodique au temps du tambour par courte rotation configurable, en respectant les verrouillages de securite. | Should | V1.1 |
| B-223 | Implementer les statistiques V1.1 en categories separees : lavages automatiques reussis, tests, echecs et interruptions, avec affichage local simple. | Should | V1.1 |
| B-224 | Implementer l'indice d'encrassement experimental stable `lavages par heure x duree moyenne lavage`, sans action automatique. | Should | V1.1/V2 |
| B-225 | Implementer le suivi empirique de consommation d'eau par debit mesure aux buses x duree de rincage cumulee, avec pertes et appoint marques comme estimatifs. | Should | V1.1/V2 |
| B-226 | Implementer les compteurs horaires cumules simples des organes principaux, sans remise a zero maintenance ni seuil de rappel en V1.1. | Should | V1.1 |
| B-227 | Implementer l'affichage local des etats, temperatures, dernier lavage et compteurs utiles retenus. | Should | V1.1 |
| B-228 | Implementer pour une V2 l'emission des notifications immediates retenues, dont `A15 - CAPOT OUVERT LONG`, avec anti-repetition et gestion du retour a la normale. | Should | V2 |
| B-229 | Implementer pour une V2 la synthese quotidienne de fonctionnement desactivee par defaut, configurable, avec horaire par defaut 18h00 et independance vis-a-vis des notifications immediates. | Should | V2 |
| B-230 | Implementer la programmation horaire de la pompe decoration avec deux plages maximum par jour, memes horaires tous les jours, interrupteur actif/inactif et sans automatisme hiver au depart. | Should | V1.1/V2 |
| B-231 | Implementer la logique d'alarmes indirectes basees sur EP_LAVAGE, EP_CRITIQUE et le resultat observable des cycles de lavage, avec reset refuse tant que EP_LAVAGE reste actif apres defaut lavage maintenu. | Must | A faire |
| B-232 | Implementer la detection capteurs incoherents, le comportement au demarrage avec niveau bas et la mise en securite associee, dont EP_CRITIQUE actif avec EP_LAVAGE inactif comme defaut bloquant. | Must | A faire |
| B-233 | Implementer les alertes de commande incoherente UV, absence anormale de lavage, redemarrages frequents et sorties commandees trop longtemps. | Should | V1.1 |
| B-234 | Etendre les statistiques avec temps de retour EP_LAVAGE, tentatives par lavage, activations EP_CRITIQUE, temperatures min/max/moyenne et historiques capot. | Should | V1.1 |

## Documentation et validation

| ID | Element | Priorite | Statut |
| --- | --- | --- | --- |
| B-301 | Rediger un plan de tests V1 trace exigence par exigence, avec matrice unique de couverture et fiches detaillees pour les essais complexes. | Must | En cours |
| B-302 | Decrire les scenarios de defaut a tester dans le plan de tests, en les reliant aux exigences et aux criteres d'acceptation. | Must | A faire |
| B-303 | Documenter le cablage final. | Should | A faire |
| B-304 | Valider par test que EP_CRITIQUE coupe bien filtration, pompe decoration, UV et mise a niveau auto, puis que l'UV ne repart qu'apres filtration et stabilisation, sans impact sur les deux bulleurs branches hors controleur. | Must | A faire |
| B-305 | Valider par test le comportement de reprise apres coupure de courant : demarrage nominal, EP_LAVAGE seule, lavage de reprise inefficace, EP_CRITIQUE actif, capot ouvert, log A15 conserve et reaffichage A15 si la condition reste presente. | Must | A faire |
| B-306 | Valider par test l'entree en maintenance a l'ouverture du capot transparent, avec contact normalement ferme, rupture vue ouverte, anti-rebond ouverture/fermeture, affichage `MAINTENANCE - CAPOT OUVERT`, retour automatique apres fermeture stable sans alarme bloquante, arret tambour/rincage, interdiction lavage auto, refus des commandes manuelles tambour/rincage capot ouvert et maintien UV si filtration autorisee. | Must | A faire |
| B-307 | Valider par test les scenarios de mode degrade retenus. | Must | A faire |
| B-308 | Valider par test le mode TEST_LAVAGE : autorisation AUTO/MAINTENANCE sous preconditions, refus capot ouvert ou securite critique, un seul cycle borne, verdict `TEST OK - CYCLE EXECUTE` si EP_LAVAGE inactif au depart, verdict niveau OK ou echec si EP_LAVAGE actif, et aucune relance multiple. | Must | A faire |
| B-309 | Valider par test la mesure de temperature bassin, les alertes sur seuil < 4 deg C et > 28 deg C, le caractere informatif et la detection de perte de sonde A11. | Must | A faire |
| B-310 | Valider par test la mesure de temperature ambiante local, les alertes sur seuil < 2 deg C et > 40 deg C, le caractere informatif et la detection de perte de sonde A12. | Must | A faire |
| B-311 | Valider par test que l'IHM locale remonte correctement les modes, alarmes A01 a A15, defauts, cycles en cours, resets refuses explicites, tests refuses explicites, voyant rouge sur A15, effacement automatique A15 et bypass suppose. | Must | A faire |
| B-312 | Valider pour une V2 par test que les notifications a distance sont pertinentes, non repetitives et sans impact sur le fonctionnement local. | Should | V2 |
| B-313 | Valider par test les cas lavage reussi, lavage minimum 10 s, rotation residuelle 2 a 5 s, anti-redemarrage 30 a 120 s, retour EP_LAVAGE pendant anti-redemarrage, lavage a duree maxi 45 s, relance apres pause 30 a 120 s, puis defaut critique apres 3 tentatives max. | Must | A faire |
| B-314 | Valider par test le test journalier automatique, son verdict et son inhibition dans les cas non autorises. | Should | V1.1 |
| B-315 | Valider par test que l'indexation du tambour change bien la zone immergee sans perturber l'exploitation ni la securite. | Should | V1.1 |
| B-316 | Valider par test la coherence des statistiques de lavage et leur consolidation sur 7 jours et 30 jours. | Should | V1.1 |
| B-317 | Valider par test la coherence et la stabilite de l'indice d'encrassement dans le temps. | Should | V1.1/V2 |
| B-318 | Valider par test la coherence des indicateurs de consommation d'eau et leur etiquetage comme estimations empiriques. | Should | V1.1/V2 |
| B-319 | Valider par test la coherence des compteurs de temps de fonctionnement et leur persistance. | Should | V1.1 |
| B-320 | Valider par test la lisibilite et la coherence des informations affichees sur l'IHM locale. | Should | A faire |
| B-321 | Valider pour une V2 par test que les notifications immediates partent sur les bons evenements, sans spam et avec retour a la normale coherent. | Should | V2 |
| B-322 | Valider pour une V2 par test le contenu, l'horaire par defaut 18h00, la desactivation par defaut et la desactivation de la synthese quotidienne sans impact sur les alertes critiques. | Should | V2 |
| B-323 | Valider par test la programmation horaire de la pompe decoration, ses deux plages maximum par jour, son interrupteur actif/inactif et sa priorite derriere securites, manuel local et commande distante. | Should | V1.1/V2 |
| B-324 | Valider par test la detection des incoherences EP_LAVAGE et EP_CRITIQUE, le filtrage EP_LAVAGE 5 a 15 s, l'anti-rebond EP_CRITIQUE 0,5 a 2 s, le reset refuse sur EP_LAVAGE persistant et la mise en securite associee. | Must | A faire |
| B-325 | Valider par test qu'un demarrage avec EP_LAVAGE ou EP_CRITIQUE actifs applique la strategie retenue : reprise degradee avec lavage si EP_LAVAGE seule, etat sur bloque si EP_CRITIQUE. | Must | A faire |
| B-326 | Valider par test que les messages d'alarme restent indirects et n'affirment pas une panne d'organe non instrumentee, notamment lavage inefficace au lieu de pompe rincage HS ou tambour bloque. | Must | A faire |
| B-327 | Valider par test les alertes d'absence anormale de lavage, de redemarrages frequents et de sortie commandee trop longtemps. | Should | V1.1 |
| B-328 | Mesurer le courant moteur tambour a vide, avec tambour a sec, avec tambour en eau et au demarrage. | Must | A faire |
| B-329 | Mesurer la vitesse reelle du moteur tambour et la vitesse tambour obtenue avec la reduction 3:1. | Must | A faire |
| B-330 | Realiser un test de blocage controle du tambour pour definir ou confirmer le seuil de protection surintensite. | Must | A faire |
| B-331 | Mesurer le debit reel de la pompe de rincage aux buses pendant un cycle type. | Must | A faire |
| B-332 | Verifier la qualite du jet de rincage, l'amorcage, le reamorcage et l'absence d'echauffement anormal sur cycles repetes. | Must | A faire |
| B-333 | Ajuster l'estimation de consommation d'eau de rincage a partir du debit reel et des durees de cycle. | Should | A faire |
