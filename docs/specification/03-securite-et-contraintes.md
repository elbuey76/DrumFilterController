# Exigences de securite et contraintes

## Securite electrique

Le controleur doit separer la basse tension de commande de la puissance moteur/pompe. Les elements alimentes par le secteur doivent etre installes dans un coffret adapte, avec protections electriques dimensionnees et cablage conforme aux regles applicables.

## Exigences de protection

| ID | Exigence | Priorite | Commentaire |
| --- | --- | --- | --- |
| S-001 | Le systeme doit couper les sorties en etat de defaut critique. | Must | Etat sur par defaut. |
| S-002 | Le systeme doit limiter la duree maximale d'un cycle. | Must | Protection pompe et moteur. |
| S-003 | Le systeme doit limiter les cycles repetes trop rapproches. | Must | Detection d'encrassement anormal ou capteur bloque. |
| S-004 | Le systeme doit detecter un niveau bas d'eau independant de la logique normale de lavage. | Must | Protection de pompe et des equipements du bassin. |
| S-005 | Le systeme doit couper sur niveau bas la pompe principale, la pompe decoration, l'UV et la mise a niveau automatique. | Must | Reduit le risque de vidange du bassin, de marche a sec et de remplissage infini sur fuite. En V1, ce niveau bas correspond a EP_CRITIQUE confirme. |
| S-006 | Le systeme doit inhiber sur niveau bas toute rotation du tambour et toute commande de pompe de rincage. | Must | Le FAT ne doit plus lancer de lavage en situation d'eau insuffisante. |
| S-007 | L'architecture doit laisser le bulleur de la cuve bio et le bulleur du bassin hors des sorties controlees et hors du circuit coupe par niveau bas. | Must | Ces bulleurs seront branches directement sur le 220 V afin de rester independants de la logique de coupure. |
| S-008 | Le systeme ne devrait declarer une absence de rotation du tambour que si un retour de rotation dedie est ajoute ; sinon il doit formuler un diagnostic indirect de lavage inefficace. | Should | Evite un diagnostic trop affirmatif avec l'instrumentation V1. |
| S-008A | La V1 ne doit pas ajouter de capteurs dedies pour diagnostiquer directement rotation tambour, courant mesure, fuite local ou niveau eau sale. | Must | Les protections materielles restent obligatoires. Un retour defaut simple fourni par un module de protection peut etre exploite, mais sans pretendre mesurer directement la cause. |
| S-009 | Le systeme devrait integrer une temporisation ou hysteresis de rearmement apres niveau bas. | Should | Evite les remises sous tension repetees. |
| S-010 | Le systeme doit reappliquer les verrouillages critiques au redemarrage apres coupure de courant avant d'autoriser des sorties. | Must | Evite un redemarrage brutal ou incoherent. |
| S-011 | Le systeme ne doit pas autoriser le mode manuel a contourner les interverrouillages critiques, notamment niveau bas, marche a sec, capot ouvert et defaut critique. | Must | Le mode manuel reste un mode surveille. |
| S-012 | L'ouverture du capot ou du compartiment d'intervention doit interdire le lavage automatique, couper la rotation tambour et couper le rincage. | Must | Protection operateur dans la zone FAT. L'UV est hors tambour et reste autorise si la filtration est autorisee et si EP_CRITIQUE est absent. |
| S-013 | La sortie du mode maintenance doit imposer une temporisation avant toute reprise automatique des organes en mouvement. | Must | Evite un redemarrage brutal quand l'operateur referme le capot. |
| S-014 | Le systeme doit differencier les defauts degradables des defauts critiques afin de maintenir le bassin vivant quand cela est acceptable. | Must | Le degrade ne doit pas masquer un danger immediat. |
| S-015 | Le systeme doit limiter les auto-diagnostics indirects V1 aux conditions observables et securites retenues. | Must | Minimum V1 : EP_CRITIQUE, incoherence EP_CRITIQUE actif avec EP_LAVAGE inactif, lavage inefficace apres 3 tentatives, capot ouvert dangereux, capot ouvert trop longtemps A15, commande UV incoherente, perte sonde temperature eau/local. Absence anormale de lavage et lavage trop frequent sont reportes V1.1. |
| S-016 | Le systeme devrait detecter une perte de mesure ou une valeur incoherente du capteur de temperature bassin et remonter une alerte dediee. | Should | Alerte informative V1 `A11 - SONDE EAU ABSENTE`, sans blocage automatique des fonctions. |
| S-017 | Le systeme doit detecter une perte de mesure ou une valeur incoherente du capteur de temperature ambiante du local et remonter une alerte dediee. | Must | Alerte informative V1 `A12 - SONDE LOCAL ABSENTE`, sans blocage automatique des fonctions. |
| S-018 | Si une remontee d'etat a distance est retenue, le systeme devrait continuer a assurer ses fonctions locales essentielles meme en cas de perte de connectivite. | Should | La supervision distante ne doit pas devenir un point de defaillance bloquant. |
| S-019 | Si une remontee d'etat a distance est retenue, le systeme devrait limiter les notifications repetitives et distinguer un nouvel evenement d'un etat deja signale. | Should | Evite l'effet spam et rend les alertes exploitables. |
| S-020 | Le systeme doit limiter le nombre de tentatives de lavage consecutives et declarer un defaut lavage maintenu si le niveau ne revient pas a la normale apres le nombre maximum configure. | Must | Evite un rincage sans fin en cas de colmatage ou panne. Ce defaut ne devient critique pour la filtration que si EP_CRITIQUE, incoherence capteurs ou risque pompe a sec est present. |
| S-021 | En cas de defaut lavage sans EP_CRITIQUE, le systeme doit inhiber le lavage automatique et maintenir l'alarme jusqu'a acquittement, tout en gardant la pompe principale active si un bypass hydraulique maintient la biofiltration. | Must | Le maintien de la vie biologique prime tant que le niveau eau propre n'est pas critique. EP_CRITIQUE, incoherence capteurs ou risque pompe a sec restent des cas de coupure filtration. |
| S-022 | Le systeme devrait surveiller le nombre de lavages par heure et par jour pour detecter une derive de fonctionnement. | Should | Peut alimenter un mode degrade ou une alerte preventive. |
| S-023 | Le test journalier automatique ne doit pas pouvoir se lancer si une condition de securite incompatible est presente, notamment capot ouvert, maintenance, niveau bas ou defaut critique. | Must | Exigence applicable a partir de la V1.1, le test journalier automatique etant hors V1. |
| S-024 | Toute fonction d'indexation periodique du tambour doit respecter les memes verrouillages de securite que la rotation de lavage. | Must | Evite un mouvement inattendu pendant une intervention ou un incident hydraulique. |
| S-025 | Les statistiques de lavage devraient distinguer autant que possible un cycle de lavage abouti d'une tentative interrompue ou d'un test journalier. | Should | Reporte V1.1 avec les statistiques avancees et le test journalier. |
| S-026 | L'indice d'encrassement devrait etre calcule selon une definition stable dans le temps afin de rendre les comparaisons saisonnieres et historiques pertinentes. | Should | Reporte V1.1 ou V2. |
| S-027 | Les indicateurs de consommation d'eau devraient indiquer clairement qu'ils sont estimes empiriquement. | Should | Reporte V1.1 ou V2. |
| S-028 | Les compteurs de temps de fonctionnement devraient etre cumules de maniere robuste face aux coupures et distinguer les organes suivis sans ambiguite. | Should | Reporte V1.1, sauf compteur simple sans risque en V1. |
| S-029 | Si une synthese quotidienne est retenue, sa desactivation ne doit pas supprimer les notifications immediates de securite ou de defaut critique. | Must | La commodite du resume journalier doit rester separee de l'alerte utile. |
| S-030 | Les notifications immediates devraient distinguer une apparition d'evenement, un retour a la normale et un etat deja signale afin de rester exploitables. | Should | Evite les ambiguities et les messages repetitifs peu utiles. |
| S-031 | Toute programmation horaire de la pompe decoration doit etre inhibee par le niveau bas, un defaut critique ou toute autre securite superieure applicable. | Must | La pompe decoration aspirant au meme endroit que la filtration, une plage horaire ne doit jamais forcer une marche non sure. |
| S-032 | Le systeme doit eviter les libelles de defaut affirmant une panne d'organe non prouvee par capteur dedie, mesure electrique ou retour de marche reel. | Must | En V1, on privilegie les effets observes comme lavage inefficace, niveau eau propre anormal ou commande incoherente. |
| S-033 | Le moteur tambour doit disposer d'une protection adaptee contre surintensite ou blocage mecanique. | Must | Le moteur d'essuie-glace 12 V peut tirer 10 a 20 A au demarrage ou en blocage ; la protection peut etre materielle autonome ou remontee a l'automate. |
| S-034 | Le systeme ne doit pas supposer une protection thermique interne du moteur tambour sans preuve. | Must | La protection externe doit rester dimensionnante tant que la reference moteur n'est pas documentee completement. |
| S-035 | Le moteur tambour doit rester hors immersion et protege des projections directes. | Must | Le moteur automobile est acceptable en ambiance humide protegee, mais pas en immersion. |
| S-036 | La pompe de rincage 230 VAC doit etre commandee par un organe adapte aux charges moteur et protegee cote secteur. | Must | Pompe VEVOR / Leo EKJ-802S, classe I, 0,6 a 0,8 kW ; tenir compte du courant d'appel au demarrage. |
| S-037 | La pompe de rincage doit etre raccordee a la terre et rester hors immersion. | Must | Protection classe I et IPX4 : compatible projections, pas immersion ni exposition forte non protegee. |
| S-038 | Le systeme ne doit pas assimiler commande de pompe de rincage a debit ou pression reel sans mesure dediee. | Must | Sans pressostat, debitmetre ou retour electrique exploite, le diagnostic doit rester indirect. |
| S-039 | Les alarmes bloquantes V1 doivent rester actives jusqu'a disparition de la cause et acquittement valide. | Must | Concerne au minimum EP_CRITIQUE, capteurs niveau incoherents, capot ouvert pendant action dangereuse et defaut lavage maintenu apres tentatives max. |
| S-040 | Un acquittement demande alors que la cause d'alarme est toujours active doit etre refuse et signale localement. | Must | Le reset ne doit pas permettre de masquer un etat dangereux ou non resolu. |
| S-041 | Le capot ouvert doit etre prioritaire sur toute demande operateur de mode ou de test. | Must | Il force maintenance ou securite meme si le selecteur physique est sur AUTO. |
| S-042 | Les commandes manuelles de tambour et rincage doivent etre a action maintenue et retomber a l'arret au relachement. | Must | Limite les risques pendant les interventions et essais locaux. |
| S-043 | EP_LAVAGE doit etre filtre avant action par une temporisation configurable de l'ordre de 5 a 15 s. | Must | Evite les lavages intempestifs sur fluctuations hydrauliques courtes. |
| S-044 | EP_CRITIQUE doit etre confirme seulement par un anti-rebond tres court de l'ordre de 0,5 a 2 s avant mise en securite. | Must | Le niveau critique represente un danger hydraulique et ne doit pas etre temporise longtemps. |
| S-045 | Une activation confirmee de EP_CRITIQUE doit imposer retour niveau normal stable puis acquittement local avant redemarrage. | Must | Evite un redemarrage automatique apres evenement hydraulique critique. |
| S-046 | Apres un EP_CRITIQUE acquitte, l'UV ne doit etre reautorise qu'apres remise en route de la filtration et courte stabilisation hydraulique. | Must | Evite un fonctionnement UV pendant une transition de debit incertaine. |
| S-047 | Toute incoherence critique entre EP_LAVAGE et EP_CRITIQUE doit etre traitee comme defaut bloquant hydraulique. | Must | Exemple : EP_CRITIQUE actif alors que EP_LAVAGE est inactif si l'ordre physique des capteurs interdit cette combinaison. |
| S-048 | Un defaut lavage maintenu avec EP_LAVAGE toujours actif doit refuser l'acquittement tant que la condition reste active. | Must | Le maintien filtration et UV via bypass ne doit pas transformer l'alarme en resettable alors que la cause persiste. |
| S-049 | Une perte de confiance dans EP_CRITIQUE doit etre traitee plus severement qu'une perte de confiance limitee a EP_LAVAGE. | Must | EP_CRITIQUE protege contre le risque hydraulique ; EP_LAVAGE pilote surtout la demande de lavage. |
| S-050 | Une demande manuelle de tambour ou de rincage avec capot ouvert doit etre refusee avant activation de sortie. | Must | Ce refus affiche un message local simple et ne devient pas une alarme bloquante si aucune sortie dangereuse n'a demarre. |
| S-051 | L'ouverture du capot pendant une action dangereuse deja active doit couper immediatement tambour et rincage et maintenir une alarme bloquante jusqu'a fermeture et acquittement valide. | Must | Distingue le refus preventif d'une commande interdite et l'incident en cours d'action. |
| S-052 | Le contact de capot doit etre fail-safe par cablage normalement ferme. | Must | Capot ferme = contact ferme ; capot ouvert, fil coupe ou connecteur debranche = capot ouvert. |
| S-053 | L'ouverture capot doit etre prise en compte rapidement apres un anti-rebond court. | Must | Cible V1 : confirmation ouverture en 100 a 500 ms. |
| S-054 | La fermeture capot doit etre stable avant reprise automatique ou reautorisation des commandes dangereuses. | Must | Cible V1 : fermeture stable 1 a 2 s avant autorisation. |
| S-055 | Un capot ouvert hors action dangereuse doit rester un etat informatif de maintenance sans acquittement. | Must | L'IHM affiche `MAINTENANCE - CAPOT OUVERT`; les sorties dangereuses restent inhibees. |
| S-056 | Apres fermeture stable du capot, la reprise automatique vers le mode demande doit etre autorisee uniquement s'il n'existe pas d'alarme capot dangereux bloquante. | Must | Si le capot s'est ouvert pendant une action dangereuse, la fermeture ne suffit pas : un acquittement valide reste requis. |

## Politique de repli en niveau bas

En cas de detection de niveau d'eau trop bas, le systeme doit se placer dans un etat sur qui :

- coupe les equipements susceptibles de vider le bassin ou de fonctionner a sec ;
- interdit les fonctions de lavage du FAT ;
- ne coupe pas les fonctions d'aeration considerees vitales pour la biologie et les poissons, celles-ci etant alimentees directement hors controleur.

## Regles de maintenance

Le mode maintenance doit au minimum :

- empecher tout demarrage automatique du tambour ;
- autoriser un arret propre des pompes ;
- inhiber partiellement les alarmes non critiques tout en conservant les alarmes de securite ;
- maintenir l'UV asservi a la filtration autorisee et a l'absence de EP_CRITIQUE, sans coupure systematique sur ouverture du capot FAT ;
- exiger une temporisation avant reprise automatique apres fermeture du capot.

## Regles de fonctionnement degrade

Le mode degrade doit etre concu pour maintenir le bassin en vie tout en signalant clairement que le FAT n'est plus nominal. Le bypass passif est considere comme un mode de survie temporaire, pas comme un fonctionnement acceptable long terme. Les reactions cibles sont les suivantes :

- lavage inefficace repete sans niveau critique : arret du lavage auto, maintien de la pompe principale via bypass hydraulique si disponible, alarme maintenue ;
- capteur de niveau principal HS : bascule sur capteur de secours si disponible ;
- lavage trop frequent : alarme et maintien du service si aucun risque immediat n'est detecte ;
- commande UV incoherente : coupure UV et alarme ;
- niveau eau propre critique repete avec retour temporaire : maintien de la vie du bassin seulement si la strategie retenue le permet encore sans risque immediat.

## Contraintes environnementales

- local de filtration maconne, isole en XPS 5 cm ;
- FAT non expose a la pluie directe ;
- capot transparent relevable au-dessus du petit batiment, correspondant au couvercle transparent demande ; matiere et isolation encore a definir ;
- humidite importante et condensation possibles ;
- projections d'eau possibles dans le local ;
- moteur tambour a maintenir hors eau et a proteger des projections directes ;
- pompe de rincage IPX4 a maintenir hors immersion, raccordee a la terre et protegee du gel ;
- parasites electriques lies aux moteurs ;
- acces maintenance potentiellement limite ;
- besoin de composants remplacables facilement.

Le capteur de temperature ambiante doit mesurer l'air du local de filtration. Il ne doit donc pas etre considere comme une simple mesure interne du coffret, sauf si l'implantation retenue permet de representer correctement l'ambiance du local.

En V1, les alertes de temperature eau et temperature ambiante, y compris perte de sonde, sont informatives et ne bloquent aucune fonction. Les automatismes lies au gel, au mode hiver, a la surchauffe ou a la condensation sont reportes V1.1/V2.
