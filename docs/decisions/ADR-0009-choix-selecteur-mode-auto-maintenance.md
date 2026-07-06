# ADR-0009 - Choix du sélecteur de mode AUTO / MAINTENANCE

## Statut

Acceptée

## Contexte

La V1 doit disposer d'une commande locale simple pour choisir l'intention d'exploitation : mode automatique nominal ou mode maintenance. Ce choix doit être lisible en façade, stable mécaniquement et compatible avec les entrées digitales du KC868-A32.

Le capot ouvert reste prioritaire sur le sélecteur : même si le sélecteur est sur AUTO, un capot ouvert force l'état maintenance ou sécurité selon la situation. Le sélecteur n'est donc pas une sécurité de capot ni une coupure de puissance.

Le coffret est installé dans un local technique humide avec condensation et projections possibles. Les organes de façade doivent rester cohérents avec un coffret IP55 minimum, préférence IP65.

## Décision

Le sélecteur physique `AUTO / MAINTENANCE` retenu pour le montage MVP est un sélecteur de façade type XB2-BD25 ou équivalent, avec les caractéristiques suivantes :

- diamètre de perçage 22 mm ;
- 2 positions maintenues, non momentanées ;
- contact `1NO + 1NC` ;
- raccordement en contacts secs basse tension vers les entrées digitales du KC868-A32 ;
- commun raccordé au `GND` des entrées, avec une voie pour `MODE_AUTO` et une voie pour `MODE_MAINTENANCE`.

La validation banc doit confirmer au multimètre le sens réel des contacts et le sens logique lu par le KC868-A32. La position AUTO doit activer `MODE_AUTO`, la position MAINTENANCE doit activer `MODE_MAINTENANCE`, et les deux états doivent rester mutuellement exclusifs.

## Conséquences

- La façade du coffret doit prévoir un perçage 22 mm pour le sélecteur.
- Deux entrées digitales sont réservées pour lire explicitement `MODE_AUTO` et `MODE_MAINTENANCE`.
- Le firmware peut détecter une incohérence si les deux entrées sont actives ou inactives alors que le câblage final devrait imposer un état unique.
- Le sélecteur ne coupe pas directement les organes de puissance ; les sorties restent commandées par la logique automate et les sécurités.
- La référence finale doit confirmer la présence d'un joint de façade, une tenue mécanique correcte, des bornes accessibles et une profondeur compatible avec la porte du coffret.

## Alternatives considérées

- Sélecteur 1NO seul : plus simple, mais moins explicite pour distinguer un vrai mode maintenance d'une rupture de fil ou d'un état non lu.
- Sélecteur 3 positions : non retenu en V1, car les commandes manuel et test sont portées par des boutons dédiés.
- Boutons séparés AUTO et MAINTENANCE : moins lisibles en façade et inutilement plus complexes pour un choix de mode stable.
- Sélecteur avec coupure de puissance : non retenu, car la coupure locale et la consignation sont traitées par l'architecture électrique du coffret, pas par l'entrée de mode automate.
