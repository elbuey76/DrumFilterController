# Plan de tests par exigences

## Objectif

Valider la V1 du contrôleur de filtre à tambour en reliant chaque exigence a une preuve de validation.

Ce plan remplace une simple liste de scénarios critiques. Les scénarios restent utiles, mais ils doivent être rattachés aux exigences qu'ils couvrent.

La structure retenue est double :

- une matrice unique de traçabilité pour prouver que chaque exigence est couverte ;
- des fiches de test séparées pour les procédures complexes, les essais de sécurité, les mesures ou les campagnes sur installation réelle.

## Règles de couverture

- Chaque exigence `Must` doit être couverte avant installation réelle.
- Chaque exigence `Should` doit être couverte, reportée explicitement ou marquée hors itération.
- Chaque exigence `Could` doit être reportée ou couverte si elle est implémentée.
- Un test peut couvrir plusieurs exigences.
- Une exigence peut nécessiter plusieurs tests si elle combine comportement nominal, sécurité et reprise après défaut.
- Les comportements de sécurité doivent être validés par test physique ou sur banc.
- Les exigences documentaires, mécaniques non automatisées ou d'architecture peuvent être validées par inspection, revue ou preuve documentaire.

## Modes de validation

| Mode | Usage attendu |
| --- | --- |
| Test sur banc | Comportement firmware, entrées/sorties, temporisations, interverrouillages et IHM. |
| Test installation | Hydraulique réelle, niveaux, bypass, rinçage, reprise et comportement avec les équipements définitifs. |
| Mesure | Débit, courant, vitesse, température ou toute valeur dimensionnante. |
| Inspection | Câblage, séparation basse tension/puissance, capot, implantation capteurs, protections et conformité documentaire. |
| Revue documentaire | Exigences de formulation, décisions d'architecture, exclusions V1/V2 et traçabilité. |

## Statuts de validation

| Statut | Signification |
| --- | --- |
| À définir | Le test ou la preuve reste à rédiger. |
| Prêt | Le test est décrit et peut être exécute. |
| Passé | Le test a été exécuté avec succès. |
| Échoué | Le test a été exécuté avec un résultat non conforme. |
| Reporté | L'exigence est explicitement reportée hors itération. |
| Non applicable | L'exigence ne s'applique pas à l'itération ou à l'architecture retenue. |

## Fiches de test détaillées

Les fiches détaillées doivent être créées lorsque la procédure ne tient pas clairement dans la matrice.

Convention de nommage proposée :

`FT-xxxx-titre-court.md`

Chaque fiche doit contenir au minimum :

- objectif ;
- exigences couvertes ;
- préconditions ;
- matériel nécessaire ;
- procédure ;
- résultats attendus ;
- résultat observé ;
- verdict ;
- date et version testée.

## Matrice de traçabilité

| Exigence | Priorité | Mode de validation | Cas de test / preuve | Critère d'acceptation | Statut |
| --- | --- | --- | --- | --- | --- |
| F-001 | Must | Test | À définir | À définir | À définir |
| F-002 | Must | Test | À définir | À définir | À définir |
| F-003 | Must | Test | À définir | À définir | À définir |
| F-004 | Must | Test | Durée lavage maxi | Un lavage ne dépasse pas la durée maximale configurée, cible initiale 45 s. | À définir |
| F-005 | Must | Test | Anti-redémarrage lavage | Deux cycles automatiques ne peuvent pas s'enchaîner sans respecter le délai configuré, cible initiale 30 à 120 s. | À définir |
| F-040 | Must | Test | Durée lavage mini | Le rinçage et la rotation restent actifs au moins 10 s au réglage initial, même si EP_LAVAGE revient vite normal. | À définir |
| F-031 | Must | Test | Alertes température eau | Les alertes basse < 4 deg C, haute > 28 deg C et perte de mesure A11 sont signalées sans bloquer les fonctions. | À définir |
| F-033 | Must | Test | Alertes température local | Les alertes basse < 2 deg C, haute > 40 deg C et perte de mesure A12 sont signalées sans bloquer les fonctions. | À définir |
| F-041 | Must | Test | Rotation résiduelle | Après arrêt rinçage, le tambour continue pendant la durée résiduelle configurée, cible initiale 2 à 5 s, puis l'anti-redémarrage démarré. | À définir |
| F-043 | Must | Test | Relance après échec lavage | Après durée maxi atteinte avec EP_LAVAGE actif, la tentative suivante attend la pause configurée 30 à 120 s. | À définir |
| F-044 | Must | Test | Défaut après tentatives max | Après 3 tentatives au réglage initial sans retour normal, le défaut lavage maintenu est déclaré et les lavages auto sont inhibés. | À définir |
| F-006 | Must | Test | À définir | À définir | À définir |
| F-066 | Must | Revue documentaire | Formulation indirecte alarmes | Aucun message V1 n'affirme une panne d'organe non instrumentée ; les libellés portent sur effets observés. | À définir |
| F-075 | Must | Test | Alarmes bloquantes V1 | Retour automatique impossible tant que la cause n'a pas disparu et que l'alarme n'est pas acquittée. | À définir |
| F-076 | Must | Test | Reset refusé cause active | Le reset est refusé et l'IHM indique la cause encore active. | À définir |
| F-080 | Must | Test | Filtrage EP_LAVAGE | Une activation plus courte que la temporisation configurée ne lance pas de lavage ; une activation maintenue lance le lavage. | À définir |
| F-081 | Must | Test | Confirmation EP_CRITIQUE | Une impulsion plus courte que l'anti-rebond configuré ne déclenche pas la sécurité ; une activation maintenue déclenche la mise en sécurité. | À définir |
| F-082 | Must | Test | Reprise après EP_CRITIQUE | Le système refuse le redémarrage avant retour niveau normal stable et acquittement local valide. | À définir |
| F-083 | Must | Test | Reprise UV après EP_CRITIQUE | Après acquittement, la filtration redémarre avant l'UV et l'UV attend la temporisation de stabilisation. | À définir |
| F-084 | Must | Test | Incohérence capteurs niveau | EP_CRITIQUE actif avec EP_LAVAGE inactif déclenche un défaut bloquant hydraulique et coupe les sorties protégées. | À définir |
| F-085 | Must | Test | Reset refusé EP_LAVAGE persistant | Le reset est refusé tant que EP_LAVAGE reste actif après défaut lavage maintenu. | À définir |
| F-086 | Must | Test | Capteur niveau non fiable | Le système bloque hydrauliquement si EP_CRITIQUE est non fiable et inhibe seulement les lavages auto si le doute porte uniquement sur EP_LAVAGE. | À définir |
| F-087 | Must | Test | EP_LAVAGE pendant anti-redémarrage | Si EP_LAVAGE revient pendant l'anti-redémarrage, aucun lavage ne repart avant la fin de la temporisation puis confirmation EP_LAVAGE. | À définir |
| F-088 | Must | Test | Priorité affichage alarmes | Si plusieurs alarmes sont actives, l'IHM affiche la plus prioritaire selon l'ordre V1 défini ; une alarme plus prioritaire masque A15 à l'écran sans l'effacer tant que le capot reste ouvert trop longtemps. | À définir |
| F-089 | Must | Test | Message reset refusé | Chaque reset refusé affiche une cause courte et explicite. | À définir |
| F-090 | Must | Test | Bypass supposé | En dégradé sans instrumentation bypass, l'IHM affiche bypass supposé et ne prétend pas mesurer bypass actif. | À définir |
| F-091 | Must | Test | Codes messages V1 | Les alarmes A01 à A15 sont affichées au format code court plus message texte. | À définir |
| F-092 | Must | Test | Voyants V1 | Le voyant MARCHE vert et le voyant ALARME rouge signalent correctement marche et alarme ; le voyant LAVAGE est teste s'il est câblé. | À définir |
| F-093 | Must | Test | TEST_LAVAGE borné | TEST_LAVAGE exécute un seul cycle complet borné et ne lance pas de relances multiples. | À définir |
| F-094 | Must | Test | TEST_LAVAGE niveau normal | Si EP_LAVAGE est inactif au départ, le verdict est TEST OK - CYCLE EXÉCUTÉ sans prétendre lavage efficace. | À définir |
| F-095 | Must | Test | TEST_LAVAGE EP_LAVAGE actif | Si EP_LAVAGE est actif au départ, le verdict dépend du retour niveau après le cycle borné et ne déclare pas de défaut lavage maintenu par le test seul. | À définir |
| F-096 | Must | Test | TEST_LAVAGE préconditions | TEST_LAVAGE est autorisé en AUTO et MAINTENANCE seulement si capot fermé, EP_CRITIQUE absent, capteurs cohérents et aucun défaut critique bloquant. | À définir |
| F-097 | Must | Test | TEST_LAVAGE capot ouvert refusé | Une demande TEST_LAVAGE capot ouvert est refusée sans mouvement avec message explicite A13. | À définir |
| F-098 | Must | Test | TEST_LAVAGE sécurité refusé | Une demande TEST_LAVAGE avec EP_CRITIQUE, capteurs incohérents ou défaut critique est refusée avec message explicite A14. | À définir |
| F-099 | Must | Test | Commandes manuelles capot ouvert refusées | MANU_TAMBOUR et MANU_RINCAGE sont refusées capot ouvert sans activation de sortie. | À définir |
| F-100 | Must | Test | Refus manuel non bloquant | Une commande manuelle refusée avant activation affiche un message local simple sans créer d'alarme bloquante à acquitter. | À définir |
| F-101 | Must | Inspection | Capot transparent unique | Le capot installé est transparent, permet de voir le tambour tourner et correspond au couvercle transparent unique du FAT. | À définir |
| F-102 | Must | Test | Contact capot normalement fermé | Capot fermé = contact fermé ; capot ouvert, fil coupé ou connecteur débranché sont interprétés comme CAPOT_OUVERT. | À définir |
| F-103 | Must | Test | Temporisation capot | L'ouverture est confirmée après 100 à 500 ms et la fermeture doit rester stable 1 à 2 s avant réautorisation. | À définir |
| F-104 | Must | Test | Capot ouvert trop longtemps | Après 10 minutes au réglage initial, ou après la temporisation configurée, l'IHM affiche A15 - CAPOT OUVERT LONG sans ajouter de blocage supplémentaire. | À définir |
| F-105 | Must | Test | État capot ouvert informatif | Capot ouvert hors action dangereuse, l'IHM affiche MAINTENANCE - CAPOT OUVERT sans demander d'acquittement. | À définir |
| F-106 | Must | Test | Retour après fermeture capot | Après fermeture stable 1 à 2 s, le système revient au mode demande si aucune alarme bloquante capot dangereux n'existe ; sinon il attend acquittement. | À définir |
| F-107 | Must | Test | Voyant rouge A15 | Lorsque A15 est actif, VOYANT_ALARME rouge est allumé fixe. | À définir |
| F-108 | Must | Test | A15 sans acquittement | Après fermeture stable du capot, A15 disparaît automatiquement sans reset alarme et sans maintien artificiel du voyant rouge. | À définir |
| F-109 | Must | Test | Trace persistante A15 | Un compteur persistant, plus le dernier événement si simple à implémenter, est écrit quand A15 survient, persiste après coupure et permet de constater qu'un capot ouvert long s'est produit sans bloquer le fonctionnement. Si une horloge fiable existe facilement, le dernier événement est horodaté ; aucun historique long n'est exigé en V1. | À définir |
| F-110 | Must | Test | Pas de clignotement A15 V1 | VOYANT_ALARME ne clignote pas pour A15 en V1. | À définir |
| F-111 | Must | Test | Pas de maintien voyant après A15 | Une fois A15 disparu, VOYANT_ALARME suit uniquement les autres alarmes actives. | À définir |
| F-112 | Must | Revue documentaire | Plateforme compatible heure fiable V2 | Le choix de plateforme V1 documente une voie réaliste vers une heure fiable V2 sans remplacement matériel principal et sans dépendance exclusive à Internet. | À définir |
| F-113 | Must | Test | Reprise A15 après coupure | Si A15 était actif avant coupure et que le capot reste ouvert au redémarrage, A15 est réaffiché après lecture stable ; si A15 n'était pas actif, la temporisation A15 repart. | À définir |
| F-114 | Must | Test | Événements persistants V1 | Le mini-journal persistant conserve au minimum A15, EP_CRITIQUE, capot ouvert dangereux, lavage inefficace et redémarrage après coupure ; les alertes température ne sont pas persistantes en MVP. | À définir |
| F-115 | Must | Test | Format mini-journal V1 | Le mini-journal V1 expose des compteurs persistants par code d'événement plus le dernier événement global, ou une mémoire circulaire courte 8/16 événements si retenue par la plateforme. | À définir |
| F-116 | Must | Test | Journalisation acquittements | Les acquittements réussis des alarmes bloquantes sont persistants ; les refus répétitifs ne créent pas chacun une entrée persistante et le dernier refus reste affiché localement. | À définir |
| F-117 | Must | Inspection | Implantation UV | L'UV est implanté hors tambour, après la pompe principale, et son autorisation dépend de la filtration autorisée et de l'absence EP_CRITIQUE. | À définir |
| F-118 | Must | Revue documentaire | Cote support FAT | La fabrication du support FAT s'appuie sur une mesure terrain documentée du niveau hydraulique cible et du trop-plein physique. | À définir |
| F-119 | Must | Revue calcul | Ouvertures tambour | Le plan de découpe ou perçage du tambour est justifié par un calcul montrant environ 0,20 à 0,23 m2 de surface filtrante utile sous toile inox 74 microns. | À définir |
| F-120 | Must | Revue + test | Auto-diagnostics V1 | Les diagnostics obligatoires V1 sont couverts et les diagnostics reportés ne sont pas présentes comme disponibles en V1. | À définir |
| F-121 | Must | Mesure | Débit rinçage référence | Le débit de rinçage de référence est mesuré aux buses sur le montage réel ; la courbe pompe n'est utilisée que comme estimation provisoire. | À définir |
| F-122 | Must | Revue documentaire | Pas capteur rinçage V1 | Le schéma V1 ne contient pas de pressostat, débitmètre ou retour courant rinçage dédié ; le diagnostic reste A04 - LAVAGE INEFFICACE. | À définir |
| F-123 | Must | Revue documentaire | Pas diagnostic direct V1 | Le schéma V1 ne contient pas de capteur dédié rotation, courant mesuré, fuite local ou niveau eau sale ; les protections matérielles restent présentes. | À définir |
| F-124 | Must | Revue câblage | Pas de parking moteur logiciel V1 | Le schema et le firmware ne dependent pas d'une fonction de parking ou de position moteur ; l'indexation reste au temps sauf ajout futur d'un capteur dédié. | À définir |
| F-125 | Must | Test | Pas mode hiver V1 | Les alertes température eau/local s'affichent sans déclencher de mode hiver automatique ni adaptation de fonctionnement. | À définir |
| F-126 | Must | Revue documentaire | Plateforme Wi-Fi V2 ready | Le choix matériel MVP documente une voie Wi-Fi V2 sans remplacement de plateforme principale. | À définir |
| F-127 | Should | Revue documentaire | Anti-spam notifications V2 | La spécification V2 prevoit apparition, rappel rare et retour à la normale pour les alarmes importantes. | À définir |
| F-128 | Should | Revue documentaire | État distant simple V2 | La spécification V2 couvre état courant, dernière alarme, dernier lavage, température eau/local et dernier redémarrage. | À définir |
| F-129 | Should | Test | Fenêtre test journalier V1.1 | Le test journalier V1.1 se lance dans la fenêtre configurée, défaut 11h-15h, et se reporte si une condition d'inhibition est présente. | À définir |
| F-130 | Should | Test | Verdict test journalier V1.1 | Le test journalier retourne OK si le cycle borné s'exécute sans sécurité et, si EP_LAVAGE était actif, si EP_LAVAGE revient normal ; sinon il retourne ÉCHEC. | À définir |
| F-131 | Should | Test | Catégories statistiques lavage | Les lavages automatiques réussis, tests, échecs et interruptions alimentent des catégories séparées. | À définir |
| F-132 | Should | Test | Restitution stats V1.1 | L'écran local affiche au minimum compteurs jour, dernier lavage et nombre d'échecs ; historique détaillé et export sont absents en V1.1. | À définir |
| F-133 | Should | Test | Indice encrassement observation | L'indice d'encrassement est calcule selon la formule stable et ne déclenche aucune action automatique. | À définir |
| F-134 | Should | Test | Compteurs horaires simples | Les compteurs horaires cumulent par organe principal sans remise à zéro maintenance ni seuil de rappel en V1.1. | À définir |
| F-135 | Should | Test | Synthèse quotidienne V2 | La synthèse quotidienne est désactivée par défaut ; si elle est activée, l'horaire par défaut est 18h00 et les notifications immédiates restent indépendantes. | À définir |
| F-136 | Must | Revue IHM | Libellé niveau distant | L'IHM distante affiche `NIVEAU FAT CRITIQUE` pour EP_CRITIQUE et ne contient pas de libellé `BASSIN NIVEAU BAS` sans capteur bassin distinct. | À définir |
| F-137 | Should | Test | Plages pompe décoration | La programmation pompe décoration accepte au maximum deux plages horaires par jour et applique les mêmes plages tous les jours. | À définir |
| F-138 | Should | Test | Interrupteur pompe décoration | L'interrupteur logiciel actif/inactif inhibe la programmation pompe décoration, notamment pour une longue période d'arrêt, sans automatisme hiver. | À définir |
| F-139 | Must | Test | Priorité pompe décoration | Les sécurités et défauts bloquants priment sur manuel local, distant et horaire ; le manuel local prime sur distant, qui prime sur programmation horaire. | À définir |
| F-055 | Should | Test | Consommation estimée | La consommation d'eau est estimée par débit mesuré aux buses x durée de rinçage cumulée. | À définir |
| F-056 | Should | Revue IHM | Pertes et appoint indicatifs | Les pertes vers évacuation et le besoin de remplissage sont étiquetées comme estimations indicatives sans compteur d'eau dédié. | À définir |
| S-039 | Must | Test | Alarmes bloquantes V1 | Les alarmes concernees restent actives jusqu'à disparition de la cause et acquittement valide. | À définir |
| S-040 | Must | Test | Reset refusé cause active | L'acquittement ne masque pas une condition dangereuse ou non resolue. | À définir |
| S-043 | Must | Test | Filtrage EP_LAVAGE | La plage configurée est comprise dans la cible V1 et filtre les fluctuations courtes. | À définir |
| S-044 | Must | Test | Confirmation EP_CRITIQUE | La confirmation reste très courte et déclenche la sécurité sans temporisation longue. | À définir |
| S-045 | Must | Test | Reprise après EP_CRITIQUE | Le redémarrage automatique reste bloque jusqu'au retour normal stable puis acquittement. | À définir |
| S-046 | Must | Test | Reprise UV après EP_CRITIQUE | L'UV reste coupé pendant la relance filtration et n'est réautorise qu'après stabilisation. | À définir |
| S-047 | Must | Test | Incohérence capteurs niveau | Toute incohérence critique bascule en défaut bloquant hydraulique. | À définir |
| S-048 | Must | Test | Reset refusé EP_LAVAGE persistant | L'acquittement ne masque pas un niveau encore en demande lavage après échec. | À définir |
| S-049 | Must | Test | Capteur niveau non fiable | La reaction distingue perte de confiance EP_CRITIQUE et doute limité a EP_LAVAGE. | À définir |
| S-050 | Must | Test | Refus manuel capot ouvert | Les commandes manuelles tambour et rinçage capot ouvert sont refusées avant activation de sortie. | À définir |
| S-051 | Must | Test | Capot ouvert pendant action dangereuse | L'ouverture capot pendant action active coupe tambour/rinçage et crée une alarme bloquante jusqu'à fermeture et acquittement. | À définir |
| S-052 | Must | Test | Capot fail-safe | Une rupture de liaison ou déconnexion du contact capot est traitée comme capot ouvert. | À définir |
| S-053 | Must | Test | Anti-rebond ouverture capot | Les rebonds courts sont filtres mais l'ouverture stable est prise en compte en 100 à 500 ms. | À définir |
| S-054 | Must | Test | Validation fermeture capot | Les commandes dangereuses ne sont réautorisees qu'après fermeture stable 1 à 2 s. | À définir |
| S-055 | Must | Test | Capot ouvert informatif | Capot ouvert hors action dangereuse reste un état de maintenance sans acquittement. | À définir |
| S-056 | Must | Test | Reprise capot conditionnelle | La reprise automatique après fermeture stable est bloquee si une alarme capot dangereux reste à acquitter. | À définir |

## Scenarios transverses a développer

| Scenario | Exigences candidates | Objectif |
| --- | --- | --- |
| Lavage nominal reussi | F-001 a F-005, F-039 a F-041, F-087 | Vérifier le cycle normal de lavage, y compris durée minimale 10 s, rotation résiduelle, anti-redémarrage et arrêt a retour niveau normal. |
| Filtrage EP_LAVAGE | F-080, S-043 | Vérifier que les fluctuations courtes ne lancent pas de lavage et qu'une demande stable le lance. |
| Confirmation EP_CRITIQUE | F-081, S-044 | Vérifier l'anti-rebond court avant mise en sécurité critique. |
| Lavage indisponible sans EP_CRITIQUE | F-027, F-043, F-044, F-085, F-120, F-122, F-123, F-124, S-020, S-021, S-048 | Vérifier le passage en dégradé après 3 tentatives, la pause entre tentatives, l'inhibition lavage, le maintien bio via bypass, le reset refusé tant que EP_LAVAGE persiste, le diagnostic indirect A04 sans capteur rinçage dédié, l'absence de capteurs diagnostic direct V1 et l'absence de dépendance a une position moteur. |
| Incohérence capteurs niveau | F-068, F-084, S-047 | Vérifier que EP_CRITIQUE actif avec EP_LAVAGE inactif déclenche la mise en sécurité bloquante. |
| Capteur niveau non fiable | F-086, S-049 | Vérifier la difference de reaction entre perte de confiance EP_CRITIQUE et doute limité a EP_LAVAGE. |
| Alertes température V1 | F-031, F-033, F-125, A06, A07, A08, A09, A11, A12 | Vérifier seuils eau/local, perte de mesure, affichage IHM, absence de blocage automatique et absence de mode hiver automatique en V1. |
| Niveau eau propre critique | F-009 a F-014, F-082, F-083, F-114, F-116, F-117, C01, S-004 a S-006, S-045, S-046 | Vérifier la mise en sécurité hydraulique, le retour niveau normal stable, l'acquittement avant redémarrage, la reprise filtration puis UV, l'implantation UV hors tambour après pompe, et la persistance EP_CRITIQUE plus acquittement reussi. |
| Test lavage V1 | F-029, F-079, F-093, F-094, F-095, F-096, F-097, F-098 | Vérifier autorisation AUTO/MAINTENANCE sous préconditions, refus capot ouvert ou sécurité critique, test a niveau normal, test avec EP_LAVAGE actif, verdicts associés, interruption sécurité et absence de relances multiples. |
| Capot ouvert et commandes dangereuses | F-025, F-099, F-100, F-101, F-102, F-103, F-104, F-105, F-106, F-107, F-108, F-109, F-110, F-111, F-113, F-114, F-115, F-116, S-012, S-013, S-050, S-051, S-052, S-053, S-054, S-055, S-056 | Vérifier le capot transparent unique, le contact normalement fermé fail-safe, les temporisations ouverture/fermeture, l'état MAINTENANCE - CAPOT OUVERT, le retour automatique après fermeture stable si aucune alarme bloquante n'existe, le refus preventif des commandes manuelles capot ouvert, l'alerte A15 si capot oublie ouvert, le voyant rouge fixe A15 sans clignotement, l'effacement automatique de A15 sans maintien du voyant, la trace persistante A15, le mini-journal V1, la reprise A15 après coupure, l'inhibition du mouvement, la logique maintenance et l'alarme bloquante seulement si le capot s'ouvre pendant une action deja active. |
| Reset refusé cause active | F-076, S-040 | Vérifier que l'acquittement ne masque pas une cause encore active. |
| IHM alarmes V1 | F-035, F-036, F-066, F-088, F-089, F-090, F-091, F-092, F-104, F-105, F-107, F-108, F-109, F-110, F-111, F-112, F-113, F-114, F-115, F-116, F-126 | Vérifier priorité d'affichage, messages A01 à A15, messages de reset refusé, tests refusés, formulation prudente du bypass, état capot ouvert, capot ouvert trop longtemps, voyant rouge fixe A15 sans clignotement, masquage de A15 par alarme plus prioritaire sans effacement, effacement automatique A15 sans maintien du voyant, trace persistante A15, mini-journal V1, reprise A15 après coupure, compatibilité plateforme avec heure fiable V2 et Wi-Fi V2, voyants V1 et absence de diagnostic abusif. |
| Reprise après coupure alimentation | F-021, F-069, F-071, F-113, F-114, F-115, S-010 | Vérifier le boot sorties sûres, le retour AUTO nominal, la reprise dégradée avec EP_LAVAGE seule, le maintien filtration/UV si lavage inefficace sans EP_CRITIQUE, le blocage sur EP_CRITIQUE, la conservation du mini-journal, la conservation du log A15 et le réaffichage A15 si le capot reste ouvert. |
