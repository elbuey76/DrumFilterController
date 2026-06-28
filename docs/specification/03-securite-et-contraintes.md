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
| S-005 | Le systeme doit couper sur niveau bas la pompe principale, la pompe decoration, l'UV et la mise a niveau automatique. | Must | Reduit le risque de vidange du bassin, de marche a sec et de remplissage infini sur fuite. |
| S-006 | Le systeme doit inhiber sur niveau bas toute rotation du tambour et toute commande de pompe de rincage. | Must | Le FAT ne doit plus lancer de lavage en situation d'eau insuffisante. |
| S-007 | Le systeme doit preserver l'alimentation du bulleur de la cuve bio et du bulleur du bassin meme en niveau bas. | Must | Ces fonctions restent utiles pour la survie biologique et l'oxygenation. |
| S-008 | Le systeme ne devrait declarer une absence de rotation du tambour que si un retour de rotation dedie est ajoute ; sinon il doit formuler un diagnostic indirect de lavage inefficace. | Should | Evite un diagnostic trop affirmatif avec l'instrumentation V1. |
| S-009 | Le systeme devrait integrer une temporisation ou hysteresis de rearmement apres niveau bas. | Should | Evite les remises sous tension repetees. |
| S-010 | Le systeme doit reappliquer les verrouillages critiques au redemarrage apres coupure de courant avant d'autoriser des sorties. | Must | Evite un redemarrage brutal ou incoherent. |
| S-011 | Le systeme ne doit pas autoriser le mode manuel a contourner les interverrouillages critiques, notamment niveau bas, marche a sec, capot ouvert et defaut critique. | Must | Le mode manuel reste un mode surveille. |
| S-012 | L'ouverture du capot ou du compartiment d'intervention doit couper l'UV, interdire le lavage automatique et couper la rotation tambour. | Must | Protection operateur en maintenance. |
| S-013 | La sortie du mode maintenance doit imposer une temporisation avant toute reprise automatique des organes en mouvement. | Must | Evite un redemarrage brutal quand l'operateur referme le capot. |
| S-014 | Le systeme doit differencier les defauts degradables des defauts critiques afin de maintenir le bassin vivant quand cela est acceptable. | Must | Le degrade ne doit pas masquer un danger immediat. |
| S-015 | Le systeme doit surveiller au minimum la coherence des capteurs EP_BAS et EP_CRITIQUE, la frequence des lavages, le succes du cycle test et le comportement au demarrage avec niveau eau propre bas. | Should | Base minimale des auto-diagnostics indirects. |
| S-016 | Le systeme devrait detecter une perte de mesure ou une valeur incoherente du capteur de temperature bassin et remonter une alerte dediee. | Should | Evite de se croire protege alors que la mesure est absente. |
| S-017 | Le systeme devrait detecter une perte de mesure ou une valeur incoherente du capteur de temperature ambiante du local et remonter une alerte dediee. | Should | Permet de ne pas rater un risque environnemental faute de mesure valide. |
| S-018 | Si une remontee d'etat a distance est retenue, le systeme devrait continuer a assurer ses fonctions locales essentielles meme en cas de perte de connectivite. | Should | La supervision distante ne doit pas devenir un point de defaillance bloquant. |
| S-019 | Si une remontee d'etat a distance est retenue, le systeme devrait limiter les notifications repetitives et distinguer un nouvel evenement d'un etat deja signale. | Should | Evite l'effet spam et rend les alertes exploitables. |
| S-020 | Le systeme doit limiter le nombre de tentatives de lavage consecutives et declarer un defaut critique si le niveau ne revient pas a la normale apres le nombre maximum configure. | Must | Evite un rincage sans fin en cas de colmatage ou panne. |
| S-021 | En cas de defaut lavage critique, le systeme doit arreter la pompe principale selon la temporisation configuree et maintenir l'alarme jusqu'a acquittement. | Must | Protege l'hydraulique et force une intervention. |
| S-022 | Le systeme devrait surveiller le nombre de lavages par heure et par jour pour detecter une derive de fonctionnement. | Should | Peut alimenter un mode degrade ou une alerte preventive. |
| S-023 | Le test journalier automatique ne doit pas pouvoir se lancer si une condition de securite incompatible est presente, notamment capot ouvert, maintenance, niveau bas ou defaut critique. | Must | Le diagnostic automatique ne doit pas creer de risque supplementaire. |
| S-024 | Toute fonction d'indexation periodique du tambour doit respecter les memes verrouillages de securite que la rotation de lavage. | Must | Evite un mouvement inattendu pendant une intervention ou un incident hydraulique. |
| S-025 | Les statistiques de lavage devraient distinguer autant que possible un cycle de lavage abouti d'une tentative interrompue ou d'un test journalier. | Should | Evite de polluer l'analyse de tendance avec des cas atypiques. |
| S-026 | L'indice d'encrassement devrait etre calcule selon une definition stable dans le temps afin de rendre les comparaisons saisonnieres et historiques pertinentes. | Should | Un changement de formule doit etre trace pour conserver la valeur du suivi. |
| S-027 | Les indicateurs de consommation d'eau devraient distinguer clairement les valeurs mesurees des valeurs estimees. | Should | Evite des interpretations erronees sur la consommation reelle du systeme. |
| S-028 | Les compteurs de temps de fonctionnement devraient etre cumules de maniere robuste face aux coupures et distinguer les organes suivis sans ambiguite. | Should | Evite des derivees de maintenance dues a des compteurs peu fiables. |
| S-029 | Si une synthese quotidienne est retenue, sa desactivation ne doit pas supprimer les notifications immediates de securite ou de defaut critique. | Must | La commodite du resume journalier doit rester separee de l'alerte utile. |
| S-030 | Les notifications immediates devraient distinguer une apparition d'evenement, un retour a la normale et un etat deja signale afin de rester exploitables. | Should | Evite les ambiguities et les messages repetitifs peu utiles. |
| S-031 | Toute programmation horaire de la pompe decoration doit etre inhibee par le niveau bas, un defaut critique ou toute autre securite superieure applicable. | Must | Une plage horaire ne doit jamais forcer une marche non sure. |
| S-032 | Le systeme doit eviter les libelles de defaut affirmant une panne d'organe non prouvee par capteur dedie, mesure electrique ou retour de marche reel. | Must | En V1, on privilegie les effets observes comme lavage inefficace, niveau eau propre anormal ou commande incoherente. |

## Politique de repli en niveau bas

En cas de detection de niveau d'eau trop bas, le systeme doit se placer dans un etat sur qui :

- coupe les equipements susceptibles de vider le bassin ou de fonctionner a sec ;
- interdit les fonctions de lavage du FAT ;
- maintient les fonctions d'aeration considerees vitales pour la biologie et les poissons.

## Regles de maintenance

Le mode maintenance doit au minimum :

- empecher tout demarrage automatique du tambour ;
- autoriser un arret propre des pompes ;
- inhiber partiellement les alarmes non critiques tout en conservant les alarmes de securite ;
- couper l'UV des l'ouverture du capot ;
- exiger une temporisation avant reprise automatique apres fermeture du capot.

## Regles de fonctionnement degrade

Le mode degrade doit etre concu pour maintenir le bassin en vie tout en signalant clairement que le FAT n'est plus nominal. Les reactions cibles sont les suivantes :

- lavage inefficace repete sans niveau critique : arret lavage auto ou maintien restreint selon la strategie retenue, alarme maintenue ;
- capteur de niveau principal HS : bascule sur capteur de secours si disponible ;
- lavage trop frequent : alarme et maintien du service si aucun risque immediat n'est detecte ;
- commande UV incoherente : coupure UV et alarme ;
- niveau eau propre critique repete avec retour temporaire : maintien de la vie du bassin seulement si la strategie retenue le permet encore sans risque immediat.

## Contraintes environnementales

- humidite importante ;
- projections d'eau possibles ;
- parasites electriques lies aux moteurs ;
- acces maintenance potentiellement limite ;
- besoin de composants remplacables facilement.
