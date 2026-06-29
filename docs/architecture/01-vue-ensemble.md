# Vue d'ensemble de l'architecture

## Diagramme de contexte

```mermaid
flowchart LR
    Bassin[Bassin / filtre à tambour]
    Capteurs[Capteurs niveau, capot, températures]
    Controleur[Contrôleur basse tension]
    Puissance[Relais, contacteurs ou variateur]
    Pompe[Pompe de rinçage]
    Moteur[Moteur tambour]
    UI[Boutons, voyants, écran optionnel]

    Bassin --> Capteurs
    Capteurs --> Controleur
    UI <--> Controleur
    Controleur --> Puissance
    Puissance --> Pompe
    Puissance --> Moteur
```

## Principes d'architecture

- Garder la logique de commande en basse tension.
- Isoler clairement la partie puissance.
- Préférer des modules du commerce pour le prototype.
- Documenter toute décision structurante dans le journal de décisions.
- Organiser le firmware autour d'une machine à états testable.
