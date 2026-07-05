# NC-0004 - Dimensionnement du depart eclairage exterieur

## Objet

Cette note trace le choix de protection du depart eclairage exterieur ajoute dans le coffret FAT. Ce circuit est distinct des fonctions bassin : il n'intervient pas dans la securite hydraulique, le lavage du FAT, la filtration, l'UV ni la mise a niveau.

## Hypotheses d'entree

| Donnee | Valeur retenue | Commentaire |
| --- | --- | --- |
| Nombre de spots | 6 | Spots LED exterieurs |
| Puissance unitaire | 3 W | Donnee luminaire |
| Puissance totale eclairage | 18 W | 6 x 3 W |
| Detection | Detecteurs presents | Consommation faible devant la marge du depart |
| Tension | 230 VAC | Alimentation secteur monophase |
| Longueur de cable | environ 10 a 15 m | Longueur indiquee pour le depart |

## Calcul

Puissance totale :

```text
P = 6 x 3 W = 18 W
```

Courant nominal sous 230 VAC :

```text
I = P / U = 18 / 230 = 0,078 A
```

Le courant nominal d'eclairage est donc d'environ 0,08 A. Meme avec la consommation des detecteurs et une marge d'appel des alimentations LED, le courant reste tres faible devant un depart 6 A.

## Decision de dimensionnement

Le depart eclairage exterieur est retenu avec un disjoncteur 6 A courbe C.

Ce calibre est tres superieur au courant nominal des spots, mais il reste adapte a un petit depart dedie d'eclairage exterieur, laisse une marge pour les appels de courant des alimentations LED et conserve une protection nettement plus fine qu'un depart standard 10 A ou 16 A.

## Limites et validations restantes

- Confirmer la section du cable retenue pour le depart.
- Confirmer le mode de pose et la protection mecanique du cable exterieur.
- Utiliser du materiel adapte a l'environnement exterieur : IP, presse-etoupes, boites de derivation et raccordements.
- Conserver la protection differentielle 30 mA en amont du tableau local.
- Verifier la classe des luminaires et raccorder la terre si les spots ou detecteurs sont de classe I.
