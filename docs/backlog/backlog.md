# Backlog projet

## À cadrer

| ID | Élément | Priorité | Statut |
| --- | --- | --- | --- |
| B-001 | Relever tension et puissance du moteur de tambour. | Must | À faire |
| B-002 | Fournir les caractéristiques de la pompe de rinçage déjà achetée puissance, débit, hauteur manométrique, tension, intensité, modèle. | Must | En attente utilisateur |
| B-003 | Définir le nombre et l'implantation des capteurs CR18-8DN pour le seuil de lavage, le niveau bas et la redondance éventuelle. | Must | À faire |
| B-004 | Définir l'environnement du coffret et l'indice de protection attendu. | Must | À faire |
| B-005 | Définir les cotes exactes du niveau normal cote sale, du niveau normal cote propre, du seuil de lavage et du niveau bas sur le tube de report en 32 mm. | Must | À faire |
| B-006 | Définir la hauteur finale du support du FAT par rapport au niveau hydraulique du bassin. | Must | À faire |
| B-007 | Déterminer si la logique de lavage repose sur une cote simple cote propre, sur une comparaison sale/propre, ou sur une combinaison des deux. | Must | À faire |
| B-008 | Ajouter des repères visuels de niveau pour identifier d'un coup d'oeil l'état hydraulique du FAT, par exemple avec de petites cales plastiques collées aux niveaux de référence. | Should | À faire |
| B-009 | Étudier un bypass passif ou mécanique entre eau sale et eau propre comme seconde sécurité si le capteur de niveau bas ne fonctionne pas, afin d'éviter la marche à sec de la pompe tout en acceptant un fonctionnement sans filtration mécanique du FAT. | Must | À faire |
| B-010 | Valider l'hypothèse de niveau normal cote sale autour de 20 cm au-dessus du bas du tambour soit environ 27 cm depuis le fond, avec ses reperes associes environ 64,5 % du diametre en hauteur, environ 59 % de circonference immergee et environ 58 cm de longueur developpee immergee. | Must | À faire |
| B-011 | Définir la forme, le nombre et la surface totale des ouvertures du tambour afin d'obtenir une surface de filtration utile d'environ 0,20 à 0,23 m2. | Must | À faire |

## FAT existant à finaliser

| ID | Élément | Priorité | Statut |
| --- | --- | --- | --- |
| B-050 | Réaliser les ouvertures dans le tuyau servant de tambour. | Must | À faire |
| B-051 | Poser la maille inox autour du tambour. | Must | À faire |
| B-052 | Raccorder la rampe d'aspersion de 32 mm à la pompe de rinçage. | Must | À faire |
| B-053 | Percer la prise d'eau de la pompe de rinçage sur une paroi du filtre. | Must | À faire |
| B-054 | Percer et équiper la vidange du filtre. | Must | À faire |
| B-055 | Poser le joint à lèvre de séparation eau propre / eau sale autour du tambour. | Must | À faire |
| B-056 | Fabriquer le support du FAT pour respecter la cote de trop-plein. | Must | À faire |
| B-057 | Fixer les capteurs sur le report de niveau en 32 mm. | Must | À faire |
| B-058 | Réaliser un capot de fermeture avec détection d'ouverture. | Should | À faire |

## Prototype matériel

| ID | Élément | Priorité | Statut |
| --- | --- | --- | --- |
| B-101 | Choisir une carte de contrôle candidate. | Must | À faire |
| B-102 | Définir les entrées/sorties nécessaires au prototype, compatibles avec les capteurs CR18-8DN NPN 12-24 VDC. | Must | À faire |
| B-103 | Réaliser un schéma de câblage de principe. | Must | À faire |
| B-104 | Monter un prototype basse tension avec boutons et voyants. | Should | À faire |
| B-105 | Réserver des sorties distinctes pour les équipements à couper sur niveau bas et pour les bulleurs à maintenir alimentés. | Must | À faire |
| B-106 | Définir l'architecture de contacteurs ou relais permettant la coupure de sécurité sans couper les bulleurs. | Must | À faire |
| B-107 | Définir l'interface électrique des entrées pour lecture fiable des capteurs CR18-8DN 3 fils. | Must | À faire |

## Firmware

| ID | Élément | Priorité | Statut |
| --- | --- | --- | --- |
| B-201 | Créer le squelette du firmware. | Must | À faire |
| B-202 | Implémenter la machine à états initiale. | Must | À faire |
| B-203 | Ajouter les temporisations de sécurité. | Must | À faire |
| B-204 | Ajouter un mécanisme de configuration simple. | Should | À faire |
| B-205 | Ajouter journalisation des cycles et défauts. | Could | À faire |
| B-206 | Intégrer la logique des capteurs de niveau, du capteur d'ouverture de capot et des sécurités associées. | Must | À faire |
| B-207 | Implémenter le comportement des pompes et sorties auxiliaires sur niveau bas et capot ouvert. | Must | À faire |
| B-208 | Implémenter l'inhibition de la rotation tambour et de la pompe de rinçage sur niveau bas. | Must | À faire |
| B-209 | Implémenter le maintien des bulleurs cuve bio et bassin pendant l'état niveau bas. | Must | À faire |

## Documentation et validation

| ID | Élément | Priorité | Statut |
| --- | --- | --- | --- |
| B-301 | Décrire une procédure de test sur table. | Must | À faire |
| B-302 | Décrire les scénarios de défaut à tester. | Must | À faire |
| B-303 | Documenter le câblage final. | Should | À faire |
| B-304 | Valider par test que le niveau bas coupe bien les pompes, l'UV, la mise à niveau auto et n'interrompt pas les deux bulleurs. | Must | À faire |
