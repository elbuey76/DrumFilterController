# Exigences de sécurité et contraintes

## Sécurité électrique

Le contrôleur doit separer la basse tension de commande de la puissance moteur/pompe. Les elements alimentes par le secteur doivent être installes dans un coffret adapté, avec protections électriques dimensionnées et câblage conforme aux règles applicables.

## Exigences de protection

| ID | Exigence | Priorité | Commentaire |
| --- | --- | --- | --- |
| S-001 | Le système doit couper les sorties en état de défaut critique. | Must | État sur par défaut. |
| S-002 | Le système doit limiter la durée maximale d'un cycle. | Must | Protection pompe et moteur. |
| S-003 | Le système doit limiter les cycles répétés trop rapproches. | Must | Détection d'encrassement anormal ou capteur bloqué. |
| S-004 | Le système doit détecter un niveau bas d'eau independant de la logique normale de lavage. | Must | Protection de pompe et des équipements du bassin. |
| S-005 | Le système doit couper sur niveau bas la pompe principale, la pompe décoration, l'UV et la mise à niveau automatique. | Must | Reduit le risque de vidange du bassin, de marche à sec et de remplissage infini sur fuite. En V1, ce niveau bas correspond à EP_CRITIQUE confirmé. |
| S-006 | Le système doit inhiber sur niveau bas toute rotation du tambour et toute commande de pompe de rinçage. | Must | Le FAT ne doit plus lancer de lavage en situation d'eau insuffisante. |
| S-007 | L'architecture doit laisser le bulleur de la cuve bio et le bulleur du bassin hors des sorties contrôlées et hors du circuit coupé par niveau bas. | Must | Ces bulleurs seront branchés directement sur le 220 V afin de rester indépendants de la logique de coupure. |
| S-008 | Le système ne devrait déclarer une absence de rotation du tambour que si un retour de rotation dédié est ajoute ; sinon il doit formuler un diagnostic indirect de lavage inefficace. | Should | Évite un diagnostic trop affirmatif avec l'instrumentation V1. |
| S-008A | La V1 ne doit pas ajouter de capteurs dedies pour diagnostiquer directement rotation tambour, courant mesuré, fuite local ou niveau eau sale. | Must | Les protections matérielles restent obligatoires. Un retour défaut simple fourni par un module de protection peut être exploité, mais sans prétendre mesurer directement la cause. |
| S-009 | Le système devrait integrer une temporisation ou hysteresis de réarmement après niveau bas. | Should | Évite les remises sous tension repetees. |
| S-010 | Le système doit reappliquer les verrouillages critiques au redémarrage après coupure de courant avant d'autoriser des sorties. | Must | Évite un redémarrage brutal ou incohérent. |
| S-011 | Le système ne doit pas autoriser le mode manuel a contourner les interverrouillages critiques, notamment niveau bas, marche à sec, capot ouvert et défaut critique. | Must | Le mode manuel reste un mode surveille. |
| S-012 | L'ouverture du capot ou du compartiment d'intervention doit interdire le lavage automatique, couper la rotation tambour et couper le rinçage. | Must | Protection opérateur dans la zone FAT. L'UV est hors tambour et reste autorisé si la filtration est autorisée et si EP_CRITIQUE est absent. |
| S-013 | La sortie du mode maintenance doit imposer une temporisation avant toute reprise automatique des organes en mouvement. | Must | Évite un redémarrage brutal quand l'opérateur referme le capot. |
| S-014 | Le système doit differencier les défauts degradables des défauts critiques afin de maintenir le bassin vivant quand cela est acceptable. | Must | Le dégradé ne doit pas masquer un danger immédiat. |
| S-015 | Le système doit limiter les auto-diagnostics indirects V1 aux conditions observables et sécurités retenues. | Must | Minimum V1 : EP_CRITIQUE, incohérence EP_CRITIQUE actif avec EP_LAVAGE inactif, lavage inefficace après 3 tentatives, capot ouvert dangereux, capot ouvert trop longtemps A15, commande UV incohérente, perte sonde température eau/local. Absence anormale de lavage et lavage trop fréquent sont reportés V1.1. |
| S-016 | Le système devrait détecter une perte de mesure ou une valeur incohérente du capteur de température bassin et remonter une alerte dédiée. | Should | Alerte informative V1 `A11 - SONDE EAU ABSENTE`, sans blocage automatique des fonctions. |
| S-017 | Le système doit détecter une perte de mesure ou une valeur incohérente du capteur de température ambiante du local et remonter une alerte dédiée. | Must | Alerte informative V1 `A12 - SONDE LOCAL ABSENTE`, sans blocage automatique des fonctions. |
| S-018 | Si une remontée d'état à distance est retenue, le système devrait continuer a assurer ses fonctions locales essentielles même en cas de perte de connectivité. | Should | La supervision distante ne doit pas devenir un point de defaillance bloquant. |
| S-019 | Si une remontée d'état à distance est retenue, le système devrait limiter les notifications répétitives et distinguer un nouvel événement d'un état deja signale. | Should | Évite l'effet spam et rend les alertes exploitables. |
| S-020 | Le système doit limiter le nombre de tentatives de lavage consecutives et déclarer un défaut lavage maintenu si le niveau ne revient pas à la normale après le nombre maximum configuré. | Must | Évite un rinçage sans fin en cas de colmatage ou panne. Ce défaut ne devient critique pour la filtration que si EP_CRITIQUE, incohérence capteurs ou risque pompe à sec est present. |
| S-021 | En cas de défaut lavage sans EP_CRITIQUE, le système doit inhiber le lavage automatique et maintenir l'alarme jusqu'à acquittement, tout en gardant la pompe principale active si un bypass hydraulique maintient la biofiltration. | Must | Le maintien de la vie biologique prime tant que le niveau eau propre n'est pas critique. EP_CRITIQUE, incohérence capteurs ou risque pompe à sec restent des cas de coupure filtration. |
| S-022 | Le système devrait surveiller le nombre de lavages par heure et par jour pour détecter une dérive de fonctionnement. | Should | Peut alimenter un mode dégradé ou une alerte preventive. |
| S-023 | Le test journalier automatique ne doit pas pouvoir se lancer si une condition de sécurité incompatible est présente, notamment capot ouvert, maintenance, niveau bas ou défaut critique. | Must | Exigence applicable à partir de la V1.1, le test journalier automatique etant hors V1. |
| S-024 | Toute fonction d'indexation périodique du tambour doit respecter les mêmes verrouillages de sécurité que la rotation de lavage. | Must | Évite un mouvement inattendu pendant une intervention ou un incident hydraulique. |
| S-025 | Les statistiques de lavage devraient distinguer autant que possible un cycle de lavage abouti d'une tentative interrompue ou d'un test journalier. | Should | Reporté V1.1 avec les statistiques avancées et le test journalier. |
| S-026 | L'indice d'encrassement devrait être calcule selon une definition stable dans le temps afin de rendre les comparaisons saisonnieres et historiques pertinentes. | Should | Reporté V1.1 ou V2. |
| S-027 | Les indicateurs de consommation d'eau devraient indiquer clairement qu'ils sont estimés empiriquement. | Should | Reporté V1.1 ou V2. |
| S-028 | Les compteurs de temps de fonctionnement devraient être cumulés de maniere robuste face aux coupures et distinguer les organes suivis sans ambiguite. | Should | Reporté V1.1, sauf compteur simple sans risque en V1. |
| S-029 | Si une synthèse quotidienne est retenue, sa desactivation ne doit pas supprimer les notifications immédiates de sécurité ou de défaut critique. | Must | La commodite du résumé journalier doit rester séparée de l'alerte utile. |
| S-030 | Les notifications immédiates devraient distinguer une apparition d'événement, un retour à la normale et un état deja signale afin de rester exploitables. | Should | Évite les ambiguities et les messages répétitifs peu utiles. |
| S-031 | Toute programmation horaire de la pompe décoration doit être inhibée par le niveau bas, un défaut critique ou toute autre sécurité supérieure applicable. | Must | La pompe décoration aspirant au même endroit que la filtration, une plage horaire ne doit jamais forcer une marche non sûre. |
| S-032 | Le système doit éviter les libellés de défaut affirmant une panne d'organe non prouvee par capteur dédié, mesure électrique ou retour de marche réel. | Must | En V1, on privilegie les effets observés comme lavage inefficace, niveau eau propre anormal ou commande incohérente. |
| S-033 | Le moteur tambour doit disposer d'une protection adaptée contre surintensité ou blocage mécanique. | Must | Moteur Fyearfly 12 VDC 10 rpm, fusible ATO 7,5 A retenu en première approche ; confirmer par mesures a vide, en charge, au démarrage et au blocage. |
| S-034 | Le système ne doit pas supposer une protection thermique interne du moteur tambour sans preuve. | Must | La protection externe doit rester dimensionnante tant que la référence moteur n'est pas documentée complètement. |
| S-035 | Le moteur tambour doit rester hors immersion et protège des projections directes. | Must | Le moteur automobile est acceptable en ambiance humide protégée, mais pas en immersion. |
| S-036 | La pompe de rinçage 230 VAC doit être commandée par un organe adapté aux charges moteur et protégée cote secteur. | Must | Pompe VEVOR / Leo EKJ-802S, classe I, 0,6 à 0,8 kW ; depart disjoncteur 10 A courbe C et contacteur Schneider TeSys LC1D12P7 bobine 230 VAC retenus, a valider au schema final. |
| S-037 | La pompe de rinçage doit être raccordee à la terre et rester hors immersion. | Must | Protection classe I et IPX4 : compatible projections, pas immersion ni exposition forte non protégée. |
| S-038 | Le système ne doit pas assimiler commande de pompe de rinçage a débit ou pression réel sans mesure dédiée. | Must | Sans pressostat, débitmètre ou retour électrique exploité, le diagnostic doit rester indirect. |
| S-039 | Les alarmes bloquantes V1 doivent rester actives jusqu'à disparition de la cause et acquittement valide. | Must | Concerne au minimum EP_CRITIQUE, capteurs niveau incohérents, capot ouvert pendant action dangereuse et défaut lavage maintenu après tentatives max. |
| S-040 | Un acquittement demande alors que la cause d'alarme est toujours active doit être refusé et signale localement. | Must | Le reset ne doit pas permettre de masquer un état dangereux ou non résolu. |
| S-041 | Le capot ouvert doit être prioritaire sur toute demande opérateur de mode ou de test. | Must | Il force maintenance ou sécurité même si le sélecteur physique est sur AUTO. |
| S-042 | Les commandes manuelles de tambour et rinçage doivent être a action maintenue et retomber à l'arrêt au relâchement. | Must | Limité les risques pendant les interventions et essais locaux. |
| S-043 | EP_LAVAGE doit être filtré avant action par une temporisation configurable de l'ordre de 5 à 15 s. | Must | Évite les lavages intempestifs sur fluctuations hydrauliques courtes. |
| S-044 | EP_CRITIQUE doit être confirmé seulement par un anti-rebond très court de l'ordre de 0,5 à 2 s avant mise en sécurité. | Must | Le niveau critique représente un danger hydraulique et ne doit pas être temporise longtemps. |
| S-045 | Une activation confirmée de EP_CRITIQUE doit imposer retour niveau normal stable puis acquittement local avant redémarrage. | Must | Évite un redémarrage automatique après événement hydraulique critique. |
| S-046 | Après un EP_CRITIQUE acquitté, l'UV ne doit être réautorise qu'après remise en route de la filtration et courte stabilisation hydraulique. | Must | Évite un fonctionnement UV pendant une transition de débit incertaine. |
| S-047 | Toute incohérence critique entre EP_LAVAGE et EP_CRITIQUE doit être traitée comme défaut bloquant hydraulique. | Must | Exemple : EP_CRITIQUE actif alors que EP_LAVAGE est inactif si l'ordre physique des capteurs interdit cette combinaison. |
| S-048 | Un défaut lavage maintenu avec EP_LAVAGE toujours actif doit refuser l'acquittement tant que la condition reste active. | Must | Le maintien filtration et UV via bypass ne doit pas transformer l'alarme en resettable alors que la cause persiste. |
| S-049 | Une perte de confiance dans EP_CRITIQUE doit être traitée plus severement qu'une perte de confiance limitee a EP_LAVAGE. | Must | EP_CRITIQUE protège contre le risque hydraulique ; EP_LAVAGE piloté surtout la demande de lavage. |
| S-050 | Une demande manuelle de tambour ou de rinçage avec capot ouvert doit être refusée avant activation de sortie. | Must | Ce refus affiche un message local simple et ne devient pas une alarme bloquante si aucune sortie dangereuse n'a démarré. |
| S-051 | L'ouverture du capot pendant une action dangereuse deja active doit couper immédiatement tambour et rinçage et maintenir une alarme bloquante jusqu'à fermeture et acquittement valide. | Must | Distingue le refus preventif d'une commande interdite et l'incident en cours d'action. |
| S-052 | Le contact de capot doit être fail-safe par câblage normalement fermé. | Must | Capot fermé = contact fermé ; capot ouvert, fil coupé ou connecteur débranché = capot ouvert. |
| S-053 | L'ouverture capot doit être prise en compte rapidement après un anti-rebond court. | Must | Cible V1 : confirmation ouverture en 100 à 500 ms. |
| S-054 | La fermeture capot doit être stable avant reprise automatique ou réautorisation des commandes dangereuses. | Must | Cible V1 : fermeture stable 1 à 2 s avant autorisation. |
| S-055 | Un capot ouvert hors action dangereuse doit rester un état informatif de maintenance sans acquittement. | Must | L'IHM affiche `MAINTENANCE - CAPOT OUVERT`; les sorties dangereuses restent inhibées. |
| S-056 | Après fermeture stable du capot, la reprise automatique vers le mode demande doit être autorisée uniquement s'il n'existe pas d'alarme capot dangereux bloquante. | Must | Si le capot s'est ouvert pendant une action dangereuse, la fermeture ne suffit pas : un acquittement valide reste requis. |
| S-057 | Le coffret FAT devrait disposer d'une coupure locale visible et cadenassable pour maintenance. | Should | Candidat disponible : Schneider Electric TeSys VCDN20, interrupteur-sectionneur 3P 690 V 20 A a poignee rouge cadenassable. A utiliser uniquement comme organe de coupure/consignation avant l'interrupteur differentiel du coffret, pas comme protection differentielle ou surintensite. Validation obligatoire du schema phase/neutre, du calibre amont 20 A, de l'etat du composant et de son montage en coffret. |
| S-058 | Le circuit d'eclairage exterieur ajoute dans le coffret doit rester distinct des fonctions bassin. | Should | Les lumieres exterieures n'ont pas de role dans la securite ni dans l'automatisme FAT. Elles doivent avoir un depart protege dedie, avec disjoncteur a choisir selon le schema final et les luminaires retenus. |

## Politique de repli en niveau bas

En cas de détection de niveau d'eau trop bas, le système doit se placer dans un état sûr qui :

- coupé les équipements susceptibles de vider le bassin ou de fonctionner à sec ;
- interdit les fonctions de lavage du FAT ;
- ne coupe pas les fonctions d'aeration considerees vitales pour la biologie et les poissons, celles-ci etant alimentees directement hors contrôleur.

## Règles de maintenance

Le mode maintenance doit au minimum :

- empecher tout démarrage automatique du tambour ;
- autoriser un arrêt propre des pompes ;
- inhiber partiellement les alarmes non critiques tout en conservant les alarmes de sécurité ;
- maintenir l'UV asservi à la filtration autorisée et à l'absence de EP_CRITIQUE, sans coupure systematique sur ouverture du capot FAT ;
- exiger une temporisation avant reprise automatique après fermeture du capot.

## Règles de fonctionnement dégradé

Le mode dégradé doit être conçu pour maintenir le bassin en vie tout en signalant clairement que le FAT n'est plus nominal. Le bypass passif est considere comme un mode de survie temporaire, pas comme un fonctionnement acceptable long terme. Les réactions cibles sont les suivantes :

- lavage inefficace répété sans niveau critique : arrêt du lavage auto, maintien de la pompe principale via bypass hydraulique si disponible, alarme maintenue ;
- capteur de niveau principal HS : bascule sur capteur de secours si disponible ;
- lavage trop fréquent : alarme et maintien du service si aucun risque immédiat n'est détecte ;
- commande UV incohérente : coupure UV et alarme ;
- niveau eau propre critique répété avec retour temporaire : maintien de la vie du bassin seulement si la stratégie retenue le permet encore sans risque immédiat.

## Contraintes environnementales

- local de filtration maconne, isole en XPS 5 cm ;
- FAT non expose à la pluie directe ;
- capot transparent relevable au-dessus du petit batiment, correspondant au couvercle transparent demande ; matiere et isolation encore à définir ;
- humidite importante et condensation possibles ;
- projections d'eau possibles dans le local ;
- moteur tambour a maintenir hors eau et a protéger des projections directes ;
- pompe de rinçage IPX4 a maintenir hors immersion, raccordee à la terre et protégée du gel ;
- parasites électriques liés aux moteurs ;
- accès maintenance potentiellement limité ;
- besoin de composants remplacables facilement.

## Contraintes de tableau electrique V1

- un interrupteur differentiel 2P 30 mA, 40 A, type A est retenu en tete de tableau ; la notice AquaForte DM-Vario demande une protection differentielle 30 mA sans imposer type F ou B ;
- le depart cote maison vers le coffret FAT n'est pas encore achete ni cable ; il devra etre protege par un disjoncteur dedie 20 A ;
- un interrupteur-sectionneur cadenassable TeSys VCDN20, 3P, 690 V, 20 A, est candidat pour la coupure locale du coffret FAT, place avant l'interrupteur differentiel du coffret, sous reserve de validation du schema final et du calibre amont 20 A ;
- l'alimentation 12 VDC est protegee par un disjoncteur 4 A courbe C ;
- la pompe de rincage est protegee par un disjoncteur 10 A courbe C ;
- les prises classiques du local sont protegees par un disjoncteur 16 A courbe C ;
- la pompe de filtration est protegee par un depart dedie 6 A courbe C ;
- l'UV, la pompe decoration et la mise a niveau sont sur un depart 6 A courbe C separe de la filtration ;
- un depart supplementaire pour lumieres exterieures est a prevoir dans le coffret, avec disjoncteur a choisir et sans interaction avec la logique bassin ;
- les departs 12 VDC sont proteges par fusibles ATO : moteur tambour 7,5 A, automate 3 A, capteurs/boutons 1 A, ecran/voyants/accessoires 1 A.

Le capteur de température ambiante doit mesurer l'air du local de filtration. Il ne doit donc pas être considere comme une simple mesure interne du coffret, sauf si l'implantation retenue permet de représenter correctement l'ambiance du local.

En V1, les alertes de température eau et température ambiante, y compris perte de sonde, sont informatives et ne bloquent aucune fonction. Les automatismes liés au gel, au mode hiver, à la surchauffe ou à la condensation sont reportés V1.1/V2.
