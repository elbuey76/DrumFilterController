# Architecture logicielle

## Modules pressentis

| Module | Responsabilité |
| --- | --- |
| Entrées | Lire les capteurs et boutons, appliquer anti-rebond/filtrage. |
| Temporisations | Centraliser les délais, durées et timeouts. |
| Machine à états | Décider des transitions et de l'état courant. |
| Sorties | Piloter relais, voyants, buzzer et autres actionneurs. |
| Configuration | Stocker les paramètres modifiables. |
| Journalisation | Enregistrer cycles, alarmes et événements importants. |

## Machine à états initiale

```mermaid
stateDiagram-v2
    [*] --> ARRET
    ARRET --> AUTO_ATTENTE: mode auto
    AUTO_ATTENTE --> LAVAGE: demande lavage valide
    LAVAGE --> PAUSE_SECURITE: durée lavage atteinte
    PAUSE_SECURITE --> AUTO_ATTENTE: délai écoulé
    AUTO_ATTENTE --> DEFAUT: anomalie
    LAVAGE --> DEFAUT: timeout ou défaut critique
    DEFAUT --> ARRET: acquittement
    ARRET --> MANUEL: commande manuelle
    MANUEL --> ARRET: fin commande
```

## Paramètres configurables

- durée de lavage ;
- délai minimal entre cycles ;
- durée maximale de marche continue ;
- nombre maximal de cycles dans une fenêtre de temps ;
- logique de déclenchement ;
- comportement après coupure d'alimentation ;
- activation ou non des extensions de journalisation.
