# ADR-0002 - Dimensionner la pompe principale sur une pompe variable avec marge

## Statut

Acceptée

## Contexte

La filtration principale du bassin doit concilier plusieurs contraintes hydrauliques :

- renouveler l'eau du bassin dans un ordre de grandeur de 1 h a 1 h 30 ;
- garantir au moins 6 m3/h dans la bonde de fond afin de conserver une vitesse d'entrainement d'environ 0,18 m/s ;
- conserver une part d'aspiration sur le skimmer ;
- ne pas surcharger le FAT existant dont la surface filtrante pourrait devenir limitante au-dela de 10 m3/h ;
- ne pas traverser la biofiltration a un debit excessif afin de conserver un temps de contact acceptable.

Le calcul documente dans [../calculs/NC-0001-dimensionnement-pompe-filtration.md](../calculs/NC-0001-dimensionnement-pompe-filtration.md) conduit a viser un debit nominal reel de 8 a 10 m3/h avec une repartition cible de 75 % sur la bonde de fond et 25 % sur le skimmer.

Une deuxieme pompe de decoration pourra modifier les equilibres hydrauliques de l'installation. Le choix de la pompe principale doit donc conserver une marge de reglage.

## Decision

Retenir comme direction de conception une pompe principale de filtration variable, de classe 20 m3/h environ, reglee en exploitation normale pour atteindre un debit reel de 8 a 10 m3/h et garantir au moins 6 m3/h sur la bonde de fond.

Une reference de travail compatible avec cette orientation est la DM Vario WiFi 20.000 RD803, ou tout equivalent offrant :

- une reserve de debit pour absorber les pertes de charge ;
- un reglage fin du point de fonctionnement ;
- un fonctionnement non permanent a pleine puissance ;
- une stabilite suffisante a debit partiel.

## Consequences

- La pompe choisie ne fixe pas le debit nominal a 20 m3/h ; elle fournit une marge de capacite au-dessus du besoin reel.
- Le reglage final devra etre fait sur installation reelle, avec vannes et si possible une mesure de debit.
- La verification hydraulique devra inclure l'effet de la deuxieme pompe sur la bonde de fond, le skimmer, le FAT et la biofiltration.
- Si le FAT confirme une limitation stricte avant 10 m3/h, la consigne de pompe devra rester sous cette valeur.
- Si les pertes de charge reelles sont faibles et que la pompe se regle mal a bas regime, un modele plus petit pourra etre reconsidere.

## Alternatives considerees

- Pompe dimensionnee juste a 8 ou 10 m3/h : plus simple sur le papier, mais moins de marge face aux pertes de charge et au vieillissement de l'installation.
- Pompe fixe non reglable : moins flexible pour equilibrer bonde de fond, skimmer et biofiltration.
- Pompe plus puissante encore : marge supplementaire, mais risque accru de surdebit, de reglage plus delicat et de consommation inutile.
