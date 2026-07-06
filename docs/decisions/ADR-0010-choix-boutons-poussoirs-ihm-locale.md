# ADR-0010 - Choix des boutons poussoirs de l'IHM locale

## Statut

Acceptée

## Contexte

La V1 doit conserver des commandes locales physiques simples pour les actions d'exploitation et de maintenance : lancement d'un test de lavage, acquittement des alarmes et commandes manuelles maintenues du tambour et du rinçage.

Ces boutons ne commandent pas directement la puissance. Ils sont lus par le KC868-A32 en contacts secs basse tension, puis la logique automate applique les interverrouillages : capot, niveau critique, cohérence capteurs et défauts critiques.

Le coffret est installé dans un local technique humide avec condensation et projections possibles. Les organes de façade doivent donc rester cohérents avec un coffret IP55 minimum, préférence IP65, et rester lisibles avec des étiquettes claires.

## Décision

Les boutons poussoirs physiques retenus pour le montage MVP sont des boutons de façade type EB2 ou équivalent, avec les caractéristiques suivantes :

- diamètre de perçage 22 mm ;
- action momentanée, sans verrouillage ;
- contact `1NO1NC` ;
- raccordement en contact sec vers les entrées digitales du KC868-A32 ;
- usage principal du contact `NO` : appui = entrée active ;
- validation banc du sens réel des contacts et du sens logique lu par le KC868-A32.

Les couleurs retenues sont :

| Fonction | Couleur | Repère |
| --- | --- | --- |
| Reset alarme | Bleu | `RESET_ALARME` |
| Test lavage | Jaune | `TEST_LAVAGE` |
| Manuel tambour | Noir | `MANU_TAMBOUR` |
| Manuel rinçage | Noir | `MANU_RINCAGE` |

Les deux boutons noirs doivent impérativement recevoir une étiquette façade explicite afin d'éviter toute confusion entre tambour et rinçage.

## Conséquences

- La façade du coffret doit prévoir quatre perçages 22 mm pour les boutons poussoirs.
- Quatre entrées digitales sont réservées aux commandes `RESET_ALARME`, `TEST_LAVAGE`, `MANU_TAMBOUR` et `MANU_RINCAGE`.
- Les boutons `MANU_TAMBOUR` et `MANU_RINCAGE` restent à action maintenue : la sortie correspondante ne peut être active que tant que le bouton est appuyé et que les sécurités restent valides.
- Le bouton rouge n'est pas utilisé pour ces commandes afin de le réserver à une éventuelle fonction d'arrêt ou à une signalisation d'urgence future.
- Le bouton vert n'est pas utilisé pour ces commandes afin de ne pas le confondre avec marche / auto OK.
- La référence finale doit confirmer la présence d'un joint de façade, une tenue mécanique correcte, des bornes accessibles et une profondeur compatible avec la porte du coffret.

## Alternatives considérées

- Boutons verrouillables : non retenus, car les commandes manuelles dangereuses doivent retomber au relâchement.
- Quatre couleurs toutes distinctes : possible, mais le choix retenu privilégie bleu pour reset, jaune pour test et noir pour les commandes manuelles, avec étiquetage explicite.
- Boutons lumineux : non nécessaires en V1, car l'état est porté par le LCD et les voyants dédiés.
- Boutons rouges ou verts : non retenus pour éviter la confusion avec alarme, arrêt, marche ou auto OK.
