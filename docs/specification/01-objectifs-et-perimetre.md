# Objectifs et périmètre

## Objectif principal

Construire un contrôleur de filtre à tambour pour bassin capable d'automatiser les cycles de lavage, de protéger les équipements et de rester simple à diagnostiquer et à maintenir.

## Résultat attendu

Le projet doit aboutir à :

- une définition claire des fonctions attendues ;
- une architecture matérielle et logicielle documentée ;
- un prototype validable sur table puis sur installation réelle ;
- une base de firmware maintenable ;
- une documentation suffisante pour câblage, configuration, tests et dépannage ;
- une stratégie de mise en sécurité des équipements auxiliaires en cas de niveau bas.

## Données d'entrée connues

Les données d'entrée suivantes décrivent l'installation cible à prendre en compte pour le dimensionnement et les choix de conception :

- le FAT a une surface interne totale de 78 cm x 47 cm et un trop-plein physique fixé à 30,5 cm de hauteur d'eau ;
- le compartiment eau propre mesure 62 cm x 47 cm et contient un tambour de 31 cm de diamètre sur 57 cm de longueur utile ;
- l'arrivée en eau sale se fait par deux entrées de 110 mm et la sortie en eau propre par deux sorties de 110 mm ;
- la goutiere d'evacuation collecte le debordement et evacue les dechets rinces vers un tuyau de 100 mm ;
- un tube de report de niveau en 32 mm existe deja cote eau propre pour recevoir les capteurs ;
- la ligne de filtration comprend ensuite une cuve IBC de 1000 L pour la biofiltration, une pompe de filtration avec retour en 63 mm, une pompe decoration et un UV encore a positionner.

Le detail des cotes, volumes theoriques, interfaces et travaux restants est documente dans [05-donnees-installation-et-fat-existant.md](05-donnees-installation-et-fat-existant.md).

## Périmètre de la première version

La première version couvre :

- détection du besoin de nettoyage par niveau d'eau dans le FAT ;
- séquence de lavage automatique ;
- coupure de prises auxiliaires sur niveau bas de sécurité ;
- mode manuel ;
- temporisations de sécurité ;
- signalisation des états principaux ;
- mise en défaut contrôlée en cas d'anomalie.

## Hors périmètre initial

Les points suivants sont envisageables, mais ne doivent pas bloquer le premier prototype :

- application mobile ;
- supervision cloud ;
- carte électronique entièrement dédiée ;
- historisation longue durée ;
- interface utilisateur avancée.
