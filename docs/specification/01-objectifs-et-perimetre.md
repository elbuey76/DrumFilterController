# Objectifs et périmètre

## Objectif principal

Construire un contrôleur de filtre à tambour pour bassin capable d'automatiser les cycles de lavage, de protéger les équipements, de surveiller la température de l'eau du bassin et la température ambiante du local, et de rester simple a diagnostiquer et a maintenir.

## Résultat attendu

Le projet doit aboutir a :

- une definition claire des fonctions attendues ;
- une architecture matérielle et logicielle documentée ;
- un MVP validable sur table puis sur installation réelle ;
- une base de firmware maintenable ;
- une documentation suffisante pour câblage, configuration, tests et dépannage ;
- une stratégie de mise en sécurité des équipements auxiliaires en cas de niveau bas ;
- une remontée d'alertes utile pour l'exploitation, y compris sur température bassin et température ambiante du local ;
- une IHM locale capable de remonter clairement le statut du système ;
- une préparation explicite d'une capacité de notification à distance de l'état et des alarmes pour une V2.

## Données d'entrée connues

Les données d'entrée suivantes decrivent l'installation cible a prendre en compte pour le dimensionnement et les choix de conception :

- le FAT a une surface interne totale de 78 cm x 47 cm et un trop-plein physique fixe à 30,5 cm de hauteur d'eau ;
- le compartiment eau propre mesure 62 cm x 47 cm et contient un tambour de 31 cm de diamètre sur 57 cm de longueur utile ;
- l'arrivee en eau sale se fait par deux entrées de 110 mm et la sortie en eau propre par deux sorties de 110 mm ;
- la goutiere d'évacuation collecte le debordement et evacue les dechets rinces vers un tuyau de 100 mm ;
- un tube de report de niveau en 32 mm existe deja côté eau propre pour recevoir les capteurs ;
- la ligne de filtration comprend ensuite une cuve IBC de 1000 L pour la biofiltration, une pompe de filtration avec retour en 63 mm, une pompe décoration et un UV hors tambour, dans la filtration, après la pompe principale ;
- une mesure de température bassin doit être ajoutée pour informer l'automate et remonter des alertes ;
- une mesure de température ambiante du local doit être ajoutée pour informer l'automate et remonter des alertes.

Le détail des cotes, volumes théoriques, interfaces et travaux restants est documenté dans [05-donnees-installation-et-fat-existant.md](05-donnees-installation-et-fat-existant.md).

## Périmètre de la première version

La première version couvre :

- détection du besoin de nettoyage par niveau d'eau dans le FAT ;
- séquence de lavage automatique ;
- coupure de prises auxiliaires sur niveau bas de sécurité ;
- mode manuel ;
- temporisations de sécurité ;
- signalisation des états principaux ;
- mise en défaut contrôlée en cas d'anomalie ;
- mesure de température de l'eau du bassin avec remontée d'alertes ;
- mesure de température ambiante du local avec remontée d'alertes ;
- IHM locale de statut ;
- préparation explicite d'une extension de connectivité et de notifications à distance pour une V2.

La frontiere V1 est volontairement stricte : lavage automatique, sécurités niveau et capot, bypass dégradé, IHM locale, températures, reset local et plan de tests trace par exigences.

## Hors périmètre initial

Les points suivants sont envisagéables, mais ne doivent pas bloquer le MVP :

- application mobile ;
- connectivité distante active et notifications associées ;
- supervision cloud ;
- carte électronique entièrement dédiée ;
- historisation longue durée ;
- interface utilisateur avancée ;
- test journalier automatique ;
- statistiques avancées 7 jours / 30 jours ;
- indice d'encrassement ;
- estimation de consommation d'eau ;
- compteurs horaires détaillés, sauf compteur simple ajouté sans risque si le firmware le rend trivial.
