# Objectifs et perimetre

## Objectif principal

Construire un controleur de filtre a tambour pour bassin capable d'automatiser les cycles de lavage, de proteger les equipements, de surveiller la temperature de l'eau du bassin et la temperature ambiante du local, et de rester simple a diagnostiquer et a maintenir.

## Resultat attendu

Le projet doit aboutir a :

- une definition claire des fonctions attendues ;
- une architecture materielle et logicielle documentee ;
- un prototype validable sur table puis sur installation reelle ;
- une base de firmware maintenable ;
- une documentation suffisante pour cablage, configuration, tests et depannage ;
- une strategie de mise en securite des equipements auxiliaires en cas de niveau bas ;
- une remontee d'alertes utile pour l'exploitation, y compris sur temperature bassin et temperature ambiante du local ;
- une IHM locale capable de remonter clairement le statut du systeme ;
- une preparation explicite d'une capacite de notification a distance de l'etat et des alarmes pour une V2.

## Donnees d'entree connues

Les donnees d'entree suivantes decrivent l'installation cible a prendre en compte pour le dimensionnement et les choix de conception :

- le FAT a une surface interne totale de 78 cm x 47 cm et un trop-plein physique fixe a 30,5 cm de hauteur d'eau ;
- le compartiment eau propre mesure 62 cm x 47 cm et contient un tambour de 31 cm de diametre sur 57 cm de longueur utile ;
- l'arrivee en eau sale se fait par deux entrees de 110 mm et la sortie en eau propre par deux sorties de 110 mm ;
- la goutiere d'evacuation collecte le debordement et evacue les dechets rinces vers un tuyau de 100 mm ;
- un tube de report de niveau en 32 mm existe deja cote eau propre pour recevoir les capteurs ;
- la ligne de filtration comprend ensuite une cuve IBC de 1000 L pour la biofiltration, une pompe de filtration avec retour en 63 mm, une pompe decoration et un UV encore a positionner ;
- une mesure de temperature bassin doit etre ajoutee pour informer l'automate et remonter des alertes ;
- une mesure de temperature ambiante du local doit etre ajoutee pour informer l'automate et remonter des alertes.

Le detail des cotes, volumes theoriques, interfaces et travaux restants est documente dans [05-donnees-installation-et-fat-existant.md](05-donnees-installation-et-fat-existant.md).

## Perimetre de la premiere version

La premiere version couvre :

- detection du besoin de nettoyage par niveau d'eau dans le FAT ;
- sequence de lavage automatique ;
- coupure de prises auxiliaires sur niveau bas de securite ;
- mode manuel ;
- temporisations de securite ;
- signalisation des etats principaux ;
- mise en defaut controlee en cas d'anomalie ;
- mesure de temperature de l'eau du bassin avec remontee d'alertes ;
- mesure de temperature ambiante du local avec remontee d'alertes ;
- IHM locale de statut ;
- preparation explicite d'une extension de connectivite et de notifications a distance pour une V2.

## Hors perimetre initial

Les points suivants sont envisageables, mais ne doivent pas bloquer le premier prototype :

- application mobile ;
- connectivite distante active et notifications associees ;
- supervision cloud ;
- carte electronique entierement dediee ;
- historisation longue duree ;
- interface utilisateur avancee.
