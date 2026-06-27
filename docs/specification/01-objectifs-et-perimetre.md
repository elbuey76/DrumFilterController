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
