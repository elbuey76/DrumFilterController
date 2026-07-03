# NC-0001 - Dimensionnement de la pompe de filtration principale

## Objet

Documenter le raisonnement de dimensionnement de la pompe principale de filtration pour le bassin et fixer un débit nominal compatible avec la bonde de fond, le FAT et la biofiltration.

## Données d'entrée

| Donnée | Valeur | Source / commentaire |
| --- | --- | --- |
| Dimensions bassin | 186 x 411 x 108 cm | Volume géométrique théorique documenté dans la spécification |
| Volume géométrique théorique bassin | 8,26 m3 | Valeur de référence pour le renouvellement |
| Temps de renouvellement vise | 1 h à 1 h 30 | Hypothèse de dimensionnement |
| Débit minimal dans la bonde de fond | 6 m3/h | Condition imposee pour conserver une vitesse d'entrainement suffisante |
| Vitesse d'entrainement visee dans la bonde de fond | environ 0,18 m/s | Valeur cible fournie |
| Repartition cible aspiration | 75 % bonde / 25 % skimmer | Hypothèse de réglage |
| Seuil de vigilance FAT | au-delà de 10 m3/h | Surface filtrante potentiellement limitante |
| Volume biofiltration | 1000 L nominal | Cuve IBC avec tapis japonais |

## Calcul du débit de renouvellement

Formule utilisée :

`Debit = Volume du bassin / Temps de renouvellement`

| Hypothèse | Calcul | Résultat |
| --- | --- | --- |
| Renouvellement en 1 h 30 | 8,26 / 1,5 | 5,51 m3/h |
| Renouvellement en 1 h | 8,26 / 1 | 8,26 m3/h |

## Interprétation

Le simple objectif de renouvellement conduirait a une plage minimale d'environ 5,5 à 8,3 m3/h. Cette plage n'est cependant pas suffisante pour satisfaire la contrainte de débit minimal dans la bonde de fond si l'on souhaite conserver une aspiration partagee avec le skimmer.

## Calcul de la repartition bonde / skimmer

Hypothèse retenue :

- 75 % du débit total sur la bonde de fond ;
- 25 % du débit total sur le skimmer.

| Débit total | Bonde de fond à 75 % | Skimmer à 25 % | Commentaire |
| --- | --- | --- | --- |
| 8 m3/h | 6,0 m3/h | 2,0 m3/h | Atteint le minimum bonde vise |
| 9 m3/h | 6,75 m3/h | 2,25 m3/h | Point intermediaire confortable |
| 10 m3/h | 7,5 m3/h | 2,5 m3/h | Haut de plage envisagé |

## Contrôle par la contrainte bonde de fond

Pour garantir au moins 6 m3/h dans la bonde de fond avec une repartition de 75 % :

`Debit total minimal = 6 / 0,75 = 8 m3/h`

Cette contrainte fixe donc le bas de la plage nominale à 8 m3/h, ce qui est plus contraignant que le simple renouvellement en 1 h 30.

## Impact sur la biofiltration

Temps de passage théoriques fournis :

| Débit | Temps de passage théorique |
| --- | --- |
| 8 m3/h | 7,5 min |
| 10 m3/h | 6 min |

Interprétation :

- 8 m3/h preserve un temps de contact plus favorable pour la bio ;
- 10 m3/h reste envisagéable mais reduit la marge biologique ;
- il est donc pertinent de viser un fonctionnement normal règle dans la plage 8 à 10 m3/h plutot qu'un fonctionnement systematique au-dessus.

## Impact sur le FAT

Le FAT actuel est considere comme potentiellement limité en surface filtrante au-delà de 10 m3/h. En l'absence de validation complementaire sur la surface utile réelle du tambour, cette valeur est retenue comme plafond de débit nominal prudent.

## Conclusion de dimensionnement

### Point de fonctionnement nominal vise

Le débit réel de filtration principale a viser est de 8 à 10 m3/h.

### Conséquence sur le choix de pompe

Une pompe variable de classe 20 m3/h est pertinente si elle est exploitee comme pompe avec réservé de capacité et non comme pompe a faire tourner à 20 m3/h en permanence.

Justifications :

- absorber les pertes de charge réelles de l'installation ;
- règler finement le débit utile ;
- conserver une marge de réglage lorsque le circuit s'encrasse ;
- éviter un fonctionnement à pleine vitesse ;
- tenir compte de l'influence de la deuxième pompe sur les débits effectifs de la chaine de filtration.

### Exemple de matériel cible

Pompe envisagée a ce stade : DM Vario WiFi 20.000 RD803 ou equivalent, a condition de pouvoir règler de façon stable le point de fonctionnement réel autour de 8 à 10 m3/h.

## Points à vérifier sur installation réelle

- pertes de charge réelles entre bassin, FAT, bio, pompe et retour ;
- débit réel obtenu sur la bonde de fond et sur le skimmer après réglage ;
- comportement du FAT près de 10 m3/h ;
- impact de la deuxième pompe sur les niveaux et les débits ;
- moyen de mesure ou d'estimation fiable du débit pour le réglage final.
