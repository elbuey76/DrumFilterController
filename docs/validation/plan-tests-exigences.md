# Plan de tests par exigences

## Objectif

Valider la V1 du controleur de filtre a tambour en reliant chaque exigence a une preuve de validation.

Ce plan remplace une simple liste de scenarios critiques. Les scenarios restent utiles, mais ils doivent etre rattaches aux exigences qu'ils couvrent.

La structure retenue est double :

- une matrice unique de tracabilite pour prouver que chaque exigence est couverte ;
- des fiches de test separees pour les procedures complexes, les essais de securite, les mesures ou les campagnes sur installation reelle.

## Regles de couverture

- Chaque exigence `Must` doit etre couverte avant installation reelle.
- Chaque exigence `Should` doit etre couverte, reportee explicitement ou marquee hors iteration.
- Chaque exigence `Could` doit etre reportee ou couverte si elle est implementee.
- Un test peut couvrir plusieurs exigences.
- Une exigence peut necessiter plusieurs tests si elle combine comportement nominal, securite et reprise apres defaut.
- Les comportements de securite doivent etre valides par test physique ou sur banc.
- Les exigences documentaires, mecaniques non automatisees ou d'architecture peuvent etre validees par inspection, revue ou preuve documentaire.

## Modes de validation

| Mode | Usage attendu |
| --- | --- |
| Test sur banc | Comportement firmware, entrees/sorties, temporisations, interverrouillages et IHM. |
| Test installation | Hydraulique reelle, niveaux, bypass, rincage, reprise et comportement avec les equipements definitifs. |
| Mesure | Debit, courant, vitesse, temperature ou toute valeur dimensionnante. |
| Inspection | Cablage, separation basse tension/puissance, capot, implantation capteurs, protections et conformite documentaire. |
| Revue documentaire | Exigences de formulation, decisions d'architecture, exclusions V1/V2 et tracabilite. |

## Statuts de validation

| Statut | Signification |
| --- | --- |
| A definir | Le test ou la preuve reste a rediger. |
| Pret | Le test est decrit et peut etre execute. |
| Passe | Le test a ete execute avec succes. |
| Echoue | Le test a ete execute avec un resultat non conforme. |
| Reporte | L'exigence est explicitement reportee hors iteration. |
| Non applicable | L'exigence ne s'applique pas a l'iteration ou a l'architecture retenue. |

## Fiches de test detaillees

Les fiches detaillees doivent etre creees lorsque la procedure ne tient pas clairement dans la matrice.

Convention de nommage proposee :

`FT-xxxx-titre-court.md`

Chaque fiche doit contenir au minimum :

- objectif ;
- exigences couvertes ;
- preconditions ;
- materiel necessaire ;
- procedure ;
- resultats attendus ;
- resultat observe ;
- verdict ;
- date et version testee.

## Matrice de tracabilite

| Exigence | Priorite | Mode de validation | Cas de test / preuve | Critere d'acceptation | Statut |
| --- | --- | --- | --- | --- | --- |
| F-001 | Must | Test | A definir | A definir | A definir |
| F-002 | Must | Test | A definir | A definir | A definir |
| F-003 | Must | Test | A definir | A definir | A definir |
| F-004 | Must | Test | Duree lavage maxi | Un lavage ne depasse pas la duree maximale configuree, cible initiale 45 s. | A definir |
| F-005 | Must | Test | Anti-redemarrage lavage | Deux cycles automatiques ne peuvent pas s'enchainer sans respecter le delai configure, cible initiale 30 a 120 s. | A definir |
| F-040 | Must | Test | Duree lavage mini | Le rincage et la rotation restent actifs au moins 10 s au reglage initial, meme si EP_LAVAGE revient vite normal. | A definir |
| F-031 | Must | Test | Alertes temperature eau | Les alertes basse < 4 deg C, haute > 28 deg C et perte de mesure A11 sont signalees sans bloquer les fonctions. | A definir |
| F-033 | Must | Test | Alertes temperature local | Les alertes basse < 2 deg C, haute > 40 deg C et perte de mesure A12 sont signalees sans bloquer les fonctions. | A definir |
| F-041 | Must | Test | Rotation residuelle | Apres arret rincage, le tambour continue pendant la duree residuelle configuree, cible initiale 2 a 5 s, puis l'anti-redemarrage demarre. | A definir |
| F-043 | Must | Test | Relance apres echec lavage | Apres duree maxi atteinte avec EP_LAVAGE actif, la tentative suivante attend la pause configuree 30 a 120 s. | A definir |
| F-044 | Must | Test | Defaut apres tentatives max | Apres 3 tentatives au reglage initial sans retour normal, le defaut lavage maintenu est declare et les lavages auto sont inhibes. | A definir |
| F-006 | Must | Test | A definir | A definir | A definir |
| F-066 | Must | Revue documentaire | Formulation indirecte alarmes | Aucun message V1 n'affirme une panne d'organe non instrumentee ; les libelles portent sur effets observes. | A definir |
| F-075 | Must | Test | Alarmes bloquantes V1 | Retour automatique impossible tant que la cause n'a pas disparu et que l'alarme n'est pas acquittee. | A definir |
| F-076 | Must | Test | Reset refuse cause active | Le reset est refuse et l'IHM indique la cause encore active. | A definir |
| F-080 | Must | Test | Filtrage EP_LAVAGE | Une activation plus courte que la temporisation configuree ne lance pas de lavage ; une activation maintenue lance le lavage. | A definir |
| F-081 | Must | Test | Confirmation EP_CRITIQUE | Une impulsion plus courte que l'anti-rebond configure ne declenche pas la securite ; une activation maintenue declenche la mise en securite. | A definir |
| F-082 | Must | Test | Reprise apres EP_CRITIQUE | Le systeme refuse le redemarrage avant retour niveau normal stable et acquittement local valide. | A definir |
| F-083 | Must | Test | Reprise UV apres EP_CRITIQUE | Apres acquittement, la filtration redemarre avant l'UV et l'UV attend la temporisation de stabilisation. | A definir |
| F-084 | Must | Test | Incoherence capteurs niveau | EP_CRITIQUE actif avec EP_LAVAGE inactif declenche un defaut bloquant hydraulique et coupe les sorties protegees. | A definir |
| F-085 | Must | Test | Reset refuse EP_LAVAGE persistant | Le reset est refuse tant que EP_LAVAGE reste actif apres defaut lavage maintenu. | A definir |
| F-086 | Must | Test | Capteur niveau non fiable | Le systeme bloque hydrauliquement si EP_CRITIQUE est non fiable et inhibe seulement les lavages auto si le doute porte uniquement sur EP_LAVAGE. | A definir |
| F-087 | Must | Test | EP_LAVAGE pendant anti-redemarrage | Si EP_LAVAGE revient pendant l'anti-redemarrage, aucun lavage ne repart avant la fin de la temporisation puis confirmation EP_LAVAGE. | A definir |
| F-088 | Must | Test | Priorite affichage alarmes | Si plusieurs alarmes sont actives, l'IHM affiche la plus prioritaire selon l'ordre V1 defini ; une alarme plus prioritaire masque A15 a l'ecran sans l'effacer tant que le capot reste ouvert trop longtemps. | A definir |
| F-089 | Must | Test | Message reset refuse | Chaque reset refuse affiche une cause courte et explicite. | A definir |
| F-090 | Must | Test | Bypass suppose | En degrade sans instrumentation bypass, l'IHM affiche bypass suppose et ne pretend pas mesurer bypass actif. | A definir |
| F-091 | Must | Test | Codes messages V1 | Les alarmes A01 a A15 sont affichees au format code court plus message texte. | A definir |
| F-092 | Must | Test | Voyants V1 | Le voyant MARCHE vert et le voyant ALARME rouge signalent correctement marche et alarme ; le voyant LAVAGE est teste s'il est cable. | A definir |
| F-093 | Must | Test | TEST_LAVAGE borne | TEST_LAVAGE execute un seul cycle complet borne et ne lance pas de relances multiples. | A definir |
| F-094 | Must | Test | TEST_LAVAGE niveau normal | Si EP_LAVAGE est inactif au depart, le verdict est TEST OK - CYCLE EXECUTE sans pretendre lavage efficace. | A definir |
| F-095 | Must | Test | TEST_LAVAGE EP_LAVAGE actif | Si EP_LAVAGE est actif au depart, le verdict depend du retour niveau apres le cycle borne et ne declare pas de defaut lavage maintenu par le test seul. | A definir |
| F-096 | Must | Test | TEST_LAVAGE preconditions | TEST_LAVAGE est autorise en AUTO et MAINTENANCE seulement si capot ferme, EP_CRITIQUE absent, capteurs coherents et aucun defaut critique bloquant. | A definir |
| F-097 | Must | Test | TEST_LAVAGE capot ouvert refuse | Une demande TEST_LAVAGE capot ouvert est refusee sans mouvement avec message explicite A13. | A definir |
| F-098 | Must | Test | TEST_LAVAGE securite refuse | Une demande TEST_LAVAGE avec EP_CRITIQUE, capteurs incoherents ou defaut critique est refusee avec message explicite A14. | A definir |
| F-099 | Must | Test | Commandes manuelles capot ouvert refusees | MANU_TAMBOUR et MANU_RINCAGE sont refusees capot ouvert sans activation de sortie. | A definir |
| F-100 | Must | Test | Refus manuel non bloquant | Une commande manuelle refusee avant activation affiche un message local simple sans creer d'alarme bloquante a acquitter. | A definir |
| F-101 | Must | Inspection | Capot transparent unique | Le capot installe est transparent, permet de voir le tambour tourner et correspond au couvercle transparent unique du FAT. | A definir |
| F-102 | Must | Test | Contact capot normalement ferme | Capot ferme = contact ferme ; capot ouvert, fil coupe ou connecteur debranche sont interpretes comme CAPOT_OUVERT. | A definir |
| F-103 | Must | Test | Temporisation capot | L'ouverture est confirmee apres 100 a 500 ms et la fermeture doit rester stable 1 a 2 s avant reautorisation. | A definir |
| F-104 | Must | Test | Capot ouvert trop longtemps | Apres 10 minutes au reglage initial, ou apres la temporisation configuree, l'IHM affiche A15 - CAPOT OUVERT LONG sans ajouter de blocage supplementaire. | A definir |
| F-105 | Must | Test | Etat capot ouvert informatif | Capot ouvert hors action dangereuse, l'IHM affiche MAINTENANCE - CAPOT OUVERT sans demander d'acquittement. | A definir |
| F-106 | Must | Test | Retour apres fermeture capot | Apres fermeture stable 1 a 2 s, le systeme revient au mode demande si aucune alarme bloquante capot dangereux n'existe ; sinon il attend acquittement. | A definir |
| F-107 | Must | Test | Voyant rouge A15 | Lorsque A15 est actif, VOYANT_ALARME rouge est allume fixe. | A definir |
| F-108 | Must | Test | A15 sans acquittement | Apres fermeture stable du capot, A15 disparait automatiquement sans reset alarme et sans maintien artificiel du voyant rouge. | A definir |
| F-109 | Must | Test | Trace persistante A15 | Un compteur persistant, plus le dernier evenement si simple a implementer, est ecrit quand A15 survient, persiste apres coupure et permet de constater qu'un capot ouvert long s'est produit sans bloquer le fonctionnement. Si une horloge fiable existe facilement, le dernier evenement est horodate ; aucun historique long n'est exige en V1. | A definir |
| F-110 | Must | Test | Pas de clignotement A15 V1 | VOYANT_ALARME ne clignote pas pour A15 en V1. | A definir |
| F-111 | Must | Test | Pas de maintien voyant apres A15 | Une fois A15 disparu, VOYANT_ALARME suit uniquement les autres alarmes actives. | A definir |
| F-112 | Must | Revue documentaire | Plateforme compatible heure fiable V2 | Le choix de plateforme V1 documente une voie realiste vers une heure fiable V2 sans remplacement materiel principal et sans dependance exclusive a Internet. | A definir |
| F-113 | Must | Test | Reprise A15 apres coupure | Si A15 etait actif avant coupure et que le capot reste ouvert au redemarrage, A15 est reaffiche apres lecture stable ; si A15 n'etait pas actif, la temporisation A15 repart. | A definir |
| F-114 | Must | Test | Evenements persistants V1 | Le mini-journal persistant conserve au minimum A15, EP_CRITIQUE, capot ouvert dangereux, lavage inefficace et redemarrage apres coupure ; les alertes temperature ne sont pas persistantes en MVP. | A definir |
| F-115 | Must | Test | Format mini-journal V1 | Le mini-journal V1 expose des compteurs persistants par code d'evenement plus le dernier evenement global, ou une memoire circulaire courte 8/16 evenements si retenue par la plateforme. | A definir |
| F-116 | Must | Test | Journalisation acquittements | Les acquittements reussis des alarmes bloquantes sont persistants ; les refus repetitifs ne creent pas chacun une entree persistante et le dernier refus reste affiche localement. | A definir |
| F-117 | Must | Inspection | Implantation UV | L'UV est implante hors tambour, apres la pompe principale, et son autorisation depend de la filtration autorisee et de l'absence EP_CRITIQUE. | A definir |
| F-118 | Must | Revue documentaire | Cote support FAT | La fabrication du support FAT s'appuie sur une mesure terrain documentee du niveau hydraulique cible et du trop-plein physique. | A definir |
| F-119 | Must | Revue calcul | Ouvertures tambour | Le plan de decoupe ou percage du tambour est justifie par un calcul montrant environ 0,20 a 0,23 m2 de surface filtrante utile sous toile inox 74 microns. | A definir |
| F-120 | Must | Revue + test | Auto-diagnostics V1 | Les diagnostics obligatoires V1 sont couverts et les diagnostics reportes ne sont pas presentes comme disponibles en V1. | A definir |
| F-121 | Must | Mesure | Debit rincage reference | Le debit de rincage de reference est mesure aux buses sur le montage reel ; la courbe pompe n'est utilisee que comme estimation provisoire. | A definir |
| F-122 | Must | Revue documentaire | Pas capteur rincage V1 | Le schema V1 ne contient pas de pressostat, debitmetre ou retour courant rincage dedie ; le diagnostic reste A04 - LAVAGE INEFFICACE. | A definir |
| F-123 | Must | Revue documentaire | Pas diagnostic direct V1 | Le schema V1 ne contient pas de capteur dedie rotation, courant mesure, fuite local ou niveau eau sale ; les protections materielles restent presentes. | A definir |
| F-124 | Must | Revue cablage | Parking moteur non utilise V1 | Les fonctions parking du moteur d'essuie-glace sont isolees ou ignorees proprement, sauf contrainte de brochage simple documentee. | A definir |
| F-125 | Must | Test | Pas mode hiver V1 | Les alertes temperature eau/local s'affichent sans declencher de mode hiver automatique ni adaptation de fonctionnement. | A definir |
| F-126 | Must | Revue documentaire | Plateforme Wi-Fi V2 ready | Le choix materiel MVP documente une voie Wi-Fi V2 sans remplacement de plateforme principale. | A definir |
| F-127 | Should | Revue documentaire | Anti-spam notifications V2 | La specification V2 prevoit apparition, rappel rare et retour a la normale pour les alarmes importantes. | A definir |
| F-128 | Should | Revue documentaire | Etat distant simple V2 | La specification V2 couvre etat courant, derniere alarme, dernier lavage, temperature eau/local et dernier redemarrage. | A definir |
| F-129 | Should | Test | Fenetre test journalier V1.1 | Le test journalier V1.1 se lance dans la fenetre configuree, defaut 11h-15h, et se reporte si une condition d'inhibition est presente. | A definir |
| F-130 | Should | Test | Verdict test journalier V1.1 | Le test journalier retourne OK si le cycle borne s'execute sans securite et, si EP_LAVAGE etait actif, si EP_LAVAGE revient normal ; sinon il retourne ECHEC. | A definir |
| F-131 | Should | Test | Categories statistiques lavage | Les lavages automatiques reussis, tests, echecs et interruptions alimentent des categories separees. | A definir |
| F-132 | Should | Test | Restitution stats V1.1 | L'ecran local affiche au minimum compteurs jour, dernier lavage et nombre d'echecs ; historique detaille et export sont absents en V1.1. | A definir |
| F-133 | Should | Test | Indice encrassement observation | L'indice d'encrassement est calcule selon la formule stable et ne declenche aucune action automatique. | A definir |
| F-134 | Should | Test | Compteurs horaires simples | Les compteurs horaires cumulent par organe principal sans remise a zero maintenance ni seuil de rappel en V1.1. | A definir |
| F-135 | Should | Test | Synthese quotidienne V2 | La synthese quotidienne est desactivee par defaut ; si elle est activee, l'horaire par defaut est 18h00 et les notifications immediates restent independantes. | A definir |
| F-136 | Must | Revue IHM | Libelle niveau distant | L'IHM distante affiche `NIVEAU FAT CRITIQUE` pour EP_CRITIQUE et ne contient pas de libelle `BASSIN NIVEAU BAS` sans capteur bassin distinct. | A definir |
| F-137 | Should | Test | Plages pompe decoration | La programmation pompe decoration accepte au maximum deux plages horaires par jour et applique les memes plages tous les jours. | A definir |
| F-138 | Should | Test | Interrupteur pompe decoration | L'interrupteur logiciel actif/inactif inhibe la programmation pompe decoration, notamment pour une longue periode d'arret, sans automatisme hiver. | A definir |
| F-139 | Must | Test | Priorite pompe decoration | Les securites et defauts bloquants priment sur manuel local, distant et horaire ; le manuel local prime sur distant, qui prime sur programmation horaire. | A definir |
| F-055 | Should | Test | Consommation estimee | La consommation d'eau est estimee par debit mesure aux buses x duree de rincage cumulee. | A definir |
| F-056 | Should | Revue IHM | Pertes et appoint indicatifs | Les pertes vers evacuation et le besoin de remplissage sont etiquetes comme estimations indicatives sans compteur d'eau dedie. | A definir |
| S-039 | Must | Test | Alarmes bloquantes V1 | Les alarmes concernees restent actives jusqu'a disparition de la cause et acquittement valide. | A definir |
| S-040 | Must | Test | Reset refuse cause active | L'acquittement ne masque pas une condition dangereuse ou non resolue. | A definir |
| S-043 | Must | Test | Filtrage EP_LAVAGE | La plage configuree est comprise dans la cible V1 et filtre les fluctuations courtes. | A definir |
| S-044 | Must | Test | Confirmation EP_CRITIQUE | La confirmation reste tres courte et declenche la securite sans temporisation longue. | A definir |
| S-045 | Must | Test | Reprise apres EP_CRITIQUE | Le redemarrage automatique reste bloque jusqu'au retour normal stable puis acquittement. | A definir |
| S-046 | Must | Test | Reprise UV apres EP_CRITIQUE | L'UV reste coupe pendant la relance filtration et n'est reautorise qu'apres stabilisation. | A definir |
| S-047 | Must | Test | Incoherence capteurs niveau | Toute incoherence critique bascule en defaut bloquant hydraulique. | A definir |
| S-048 | Must | Test | Reset refuse EP_LAVAGE persistant | L'acquittement ne masque pas un niveau encore en demande lavage apres echec. | A definir |
| S-049 | Must | Test | Capteur niveau non fiable | La reaction distingue perte de confiance EP_CRITIQUE et doute limite a EP_LAVAGE. | A definir |
| S-050 | Must | Test | Refus manuel capot ouvert | Les commandes manuelles tambour et rincage capot ouvert sont refusees avant activation de sortie. | A definir |
| S-051 | Must | Test | Capot ouvert pendant action dangereuse | L'ouverture capot pendant action active coupe tambour/rincage et cree une alarme bloquante jusqu'a fermeture et acquittement. | A definir |
| S-052 | Must | Test | Capot fail-safe | Une rupture de liaison ou deconnexion du contact capot est traitee comme capot ouvert. | A definir |
| S-053 | Must | Test | Anti-rebond ouverture capot | Les rebonds courts sont filtres mais l'ouverture stable est prise en compte en 100 a 500 ms. | A definir |
| S-054 | Must | Test | Validation fermeture capot | Les commandes dangereuses ne sont reautorisees qu'apres fermeture stable 1 a 2 s. | A definir |
| S-055 | Must | Test | Capot ouvert informatif | Capot ouvert hors action dangereuse reste un etat de maintenance sans acquittement. | A definir |
| S-056 | Must | Test | Reprise capot conditionnelle | La reprise automatique apres fermeture stable est bloquee si une alarme capot dangereux reste a acquitter. | A definir |

## Scenarios transverses a developper

| Scenario | Exigences candidates | Objectif |
| --- | --- | --- |
| Lavage nominal reussi | F-001 a F-005, F-039 a F-041, F-087 | Verifier le cycle normal de lavage, y compris duree minimale 10 s, rotation residuelle, anti-redemarrage et arret a retour niveau normal. |
| Filtrage EP_LAVAGE | F-080, S-043 | Verifier que les fluctuations courtes ne lancent pas de lavage et qu'une demande stable le lance. |
| Confirmation EP_CRITIQUE | F-081, S-044 | Verifier l'anti-rebond court avant mise en securite critique. |
| Lavage indisponible sans EP_CRITIQUE | F-027, F-043, F-044, F-085, F-120, F-122, F-123, F-124, S-020, S-021, S-048 | Verifier le passage en degrade apres 3 tentatives, la pause entre tentatives, l'inhibition lavage, le maintien bio via bypass, le reset refuse tant que EP_LAVAGE persiste, le diagnostic indirect A04 sans capteur rincage dedie, l'absence de capteurs diagnostic direct V1 et le parking moteur non utilise. |
| Incoherence capteurs niveau | F-068, F-084, S-047 | Verifier que EP_CRITIQUE actif avec EP_LAVAGE inactif declenche la mise en securite bloquante. |
| Capteur niveau non fiable | F-086, S-049 | Verifier la difference de reaction entre perte de confiance EP_CRITIQUE et doute limite a EP_LAVAGE. |
| Alertes temperature V1 | F-031, F-033, F-125, A06, A07, A08, A09, A11, A12 | Verifier seuils eau/local, perte de mesure, affichage IHM, absence de blocage automatique et absence de mode hiver automatique en V1. |
| Niveau eau propre critique | F-009 a F-014, F-082, F-083, F-114, F-116, F-117, C01, S-004 a S-006, S-045, S-046 | Verifier la mise en securite hydraulique, le retour niveau normal stable, l'acquittement avant redemarrage, la reprise filtration puis UV, l'implantation UV hors tambour apres pompe, et la persistance EP_CRITIQUE plus acquittement reussi. |
| Test lavage V1 | F-029, F-079, F-093, F-094, F-095, F-096, F-097, F-098 | Verifier autorisation AUTO/MAINTENANCE sous preconditions, refus capot ouvert ou securite critique, test a niveau normal, test avec EP_LAVAGE actif, verdicts associes, interruption securite et absence de relances multiples. |
| Capot ouvert et commandes dangereuses | F-025, F-099, F-100, F-101, F-102, F-103, F-104, F-105, F-106, F-107, F-108, F-109, F-110, F-111, F-113, F-114, F-115, F-116, S-012, S-013, S-050, S-051, S-052, S-053, S-054, S-055, S-056 | Verifier le capot transparent unique, le contact normalement ferme fail-safe, les temporisations ouverture/fermeture, l'etat MAINTENANCE - CAPOT OUVERT, le retour automatique apres fermeture stable si aucune alarme bloquante n'existe, le refus preventif des commandes manuelles capot ouvert, l'alerte A15 si capot oublie ouvert, le voyant rouge fixe A15 sans clignotement, l'effacement automatique de A15 sans maintien du voyant, la trace persistante A15, le mini-journal V1, la reprise A15 apres coupure, l'inhibition du mouvement, la logique maintenance et l'alarme bloquante seulement si le capot s'ouvre pendant une action deja active. |
| Reset refuse cause active | F-076, S-040 | Verifier que l'acquittement ne masque pas une cause encore active. |
| IHM alarmes V1 | F-035, F-036, F-066, F-088, F-089, F-090, F-091, F-092, F-104, F-105, F-107, F-108, F-109, F-110, F-111, F-112, F-113, F-114, F-115, F-116, F-126 | Verifier priorite d'affichage, messages A01 a A15, messages de reset refuse, tests refuses, formulation prudente du bypass, etat capot ouvert, capot ouvert trop longtemps, voyant rouge fixe A15 sans clignotement, masquage de A15 par alarme plus prioritaire sans effacement, effacement automatique A15 sans maintien du voyant, trace persistante A15, mini-journal V1, reprise A15 apres coupure, compatibilite plateforme avec heure fiable V2 et Wi-Fi V2, voyants V1 et absence de diagnostic abusif. |
| Reprise apres coupure alimentation | F-021, F-069, F-071, F-113, F-114, F-115, S-010 | Verifier le boot sorties sures, le retour AUTO nominal, la reprise degradee avec EP_LAVAGE seule, le maintien filtration/UV si lavage inefficace sans EP_CRITIQUE, le blocage sur EP_CRITIQUE, la conservation du mini-journal, la conservation du log A15 et le reaffichage A15 si le capot reste ouvert. |
