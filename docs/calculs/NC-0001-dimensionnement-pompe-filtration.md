# NC-0001 - Dimensionnement de la pompe de filtration principale

## Objet

Documenter le raisonnement de dimensionnement de la pompe principale de filtration pour le bassin et fixer un debit nominal compatible avec la bonde de fond, le FAT et la biofiltration.

## Donnees d'entree

| Donnee | Valeur | Source / commentaire |
| --- | --- | --- |
| Dimensions bassin | 186 x 411 x 108 cm | Volume geometrique theorique documente dans la specification |
| Volume geometrique theorique bassin | 8,26 m3 | Valeur de reference pour le renouvellement |
| Temps de renouvellement vise | 1 h a 1 h 30 | Hypothese de dimensionnement |
| Debit minimal dans la bonde de fond | 6 m3/h | Condition imposee pour conserver une vitesse d'entrainement suffisante |
| Vitesse d'entrainement visee dans la bonde de fond | environ 0,18 m/s | Valeur cible fournie |
| Repartition cible aspiration | 75 % bonde / 25 % skimmer | Hypothese de reglage |
| Seuil de vigilance FAT | au-dela de 10 m3/h | Surface filtrante potentiellement limitante |
| Volume biofiltration | 1000 L nominal | Cuve IBC avec tapis japonais |

## Calcul du debit de renouvellement

Formule utilisee :

`Debit = Volume du bassin / Temps de renouvellement`

| Hypothese | Calcul | Resultat |
| --- | --- | --- |
| Renouvellement en 1 h 30 | 8,26 / 1,5 | 5,51 m3/h |
| Renouvellement en 1 h | 8,26 / 1 | 8,26 m3/h |

## Interpretation

Le simple objectif de renouvellement conduirait a une plage minimale d'environ 5,5 a 8,3 m3/h. Cette plage n'est cependant pas suffisante pour satisfaire la contrainte de debit minimal dans la bonde de fond si l'on souhaite conserver une aspiration partagee avec le skimmer.

## Calcul de la repartition bonde / skimmer

Hypothese retenue :

- 75 % du debit total sur la bonde de fond ;
- 25 % du debit total sur le skimmer.

| Debit total | Bonde de fond a 75 % | Skimmer a 25 % | Commentaire |
| --- | --- | --- | --- |
| 8 m3/h | 6,0 m3/h | 2,0 m3/h | Atteint le minimum bonde vise |
| 9 m3/h | 6,75 m3/h | 2,25 m3/h | Point intermediaire confortable |
| 10 m3/h | 7,5 m3/h | 2,5 m3/h | Haut de plage envisage |

## Controle par la contrainte bonde de fond

Pour garantir au moins 6 m3/h dans la bonde de fond avec une repartition de 75 % :

`Debit total minimal = 6 / 0,75 = 8 m3/h`

Cette contrainte fixe donc le bas de la plage nominale a 8 m3/h, ce qui est plus contraignant que le simple renouvellement en 1 h 30.

## Impact sur la biofiltration

Temps de passage theoriques fournis :

| Debit | Temps de passage theorique |
| --- | --- |
| 8 m3/h | 7,5 min |
| 10 m3/h | 6 min |

Interpretation :

- 8 m3/h preserve un temps de contact plus favorable pour la bio ;
- 10 m3/h reste envisageable mais reduit la marge biologique ;
- il est donc pertinent de viser un fonctionnement normal regle dans la plage 8 a 10 m3/h plutot qu'un fonctionnement systematique au-dessus.

## Impact sur le FAT

Le FAT actuel est considere comme potentiellement limite en surface filtrante au-dela de 10 m3/h. En l'absence de validation complementaire sur la surface utile reelle du tambour, cette valeur est retenue comme plafond de debit nominal prudent.

## Conclusion de dimensionnement

### Point de fonctionnement nominal vise

Le debit reel de filtration principale a viser est de 8 a 10 m3/h.

### Consequence sur le choix de pompe

Une pompe variable de classe 20 m3/h est pertinente si elle est exploitee comme pompe avec reserve de capacite et non comme pompe a faire tourner a 20 m3/h en permanence.

Justifications :

- absorber les pertes de charge reelles de l'installation ;
- regler finement le debit utile ;
- conserver une marge de reglage lorsque le circuit s'encrasse ;
- eviter un fonctionnement a pleine vitesse ;
- tenir compte de l'influence de la deuxieme pompe sur les debits effectifs de la chaine de filtration.

### Exemple de materiel cible

Pompe envisagee a ce stade : DM Vario WiFi 20.000 RD803 ou equivalent, a condition de pouvoir regler de facon stable le point de fonctionnement reel autour de 8 a 10 m3/h.

## Points a verifier sur installation reelle

- pertes de charge reelles entre bassin, FAT, bio, pompe et retour ;
- debit reel obtenu sur la bonde de fond et sur le skimmer apres reglage ;
- comportement du FAT pres de 10 m3/h ;
- impact de la deuxieme pompe sur les niveaux et les debits ;
- moyen de mesure ou d'estimation fiable du debit pour le reglage final.
