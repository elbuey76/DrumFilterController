# ADR-0002 - Dimensionner la pompe principale sur une pompe variable avec marge

## Statut

Acceptée

## Contexte

La filtration principale du bassin doit concilier plusieurs contraintes hydrauliques :

- renouveler l'eau du bassin dans un ordre de grandeur de 1 h à 1 h 30 ;
- garantir au moins 6 m3/h dans la bonde de fond afin de conserver une vitesse d'entrainement d'environ 0,18 m/s ;
- conserver une part d'aspiration sur le skimmer ;
- ne pas surcharger le FAT existant dont la surface filtrante pourrait devenir limitante au-delà de 10 m3/h ;
- ne pas traverser la biofiltration a un débit excessif afin de conserver un temps de contact acceptable.

Le calcul documenté dans [../calculs/NC-0001-dimensionnement-pompe-filtration.md](../calculs/NC-0001-dimensionnement-pompe-filtration.md) conduit a viser un débit nominal réel de 8 à 10 m3/h avec une repartition cible de 75 % sur la bonde de fond et 25 % sur le skimmer.

Une deuxième pompe de décoration pourra modifier les equilibres hydrauliques de l'installation. Le choix de la pompe principale doit donc conserver une marge de réglage.

## Decision

Retenir comme direction de conception une pompe principale de filtration variable, de classe 20 m3/h environ, réglée en exploitation normale pour atteindre un débit réel de 8 à 10 m3/h et garantir au moins 6 m3/h sur la bonde de fond.

Une référence de travail compatible avec cette orientation est la DM Vario WiFi 20.000 RD803, ou tout equivalent offrant :

- une réserve de débit pour absorber les pertes de charge ;
- un réglage fin du point de fonctionnement ;
- un fonctionnement non permanent à pleine puissance ;
- une stabilite suffisante a débit partiel.

## Conséquences

- La pompe choisie ne fixe pas le débit nominal à 20 m3/h ; elle fournit une marge de capacité au-dessus du besoin réel.
- Le réglage final devra être fait sur installation réelle, avec vannes et si possible une mesure de débit.
- La vérification hydraulique devra inclure l'effet de la deuxième pompe sur la bonde de fond, le skimmer, le FAT et la biofiltration.
- Si le FAT confirmé une limitation stricte avant 10 m3/h, la consigne de pompe devra rester sous cette valeur.
- Si les pertes de charge réelles sont faibles et que la pompe se règle mal à bas régime, un modèle plus petit pourra être reconsidéré.

## Alternatives considerees

- Pompe dimensionnée juste à 8 ou 10 m3/h : plus simple sur le papier, mais moins de marge face aux pertes de charge et au vieillissement de l'installation.
- Pompe fixe non réglable : moins flexible pour equilibrer bonde de fond, skimmer et biofiltration.
- Pompe plus puissante encore : marge supplémentaire, mais risque accru de surdebit, de réglage plus delicat et de consommation inutile.
