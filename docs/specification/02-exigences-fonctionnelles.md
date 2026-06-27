# Exigences fonctionnelles

## Tableau des exigences

| ID | Exigence | Priorité | Commentaire |
| --- | --- | --- | --- |
| F-001 | Le système doit détecter un besoin de lavage à partir du niveau d'eau dans le filtre à tambour. | Must | Le ou les capteurs exacts restent à choisir. |
| F-002 | Le système doit démarrer une pompe de rinçage pendant le cycle de lavage. | Must | La sortie devra probablement piloter un relais ou contacteur. |
| F-003 | Le système doit commander la rotation du tambour pendant le cycle de lavage. | Must | La commande dépendra du moteur retenu. |
| F-004 | Le système doit arrêter automatiquement le cycle après une durée configurable. | Must | Valeur à définir lors des essais. |
| F-005 | Le système doit imposer un délai minimal entre deux cycles automatiques. | Must | Protection contre un capteur instable ou un filtre saturé. |
| F-006 | Le système doit proposer un mode manuel de lavage. | Should | Le mode manuel doit conserver les protections essentielles. |
| F-007 | Le système doit signaler les états marche, cycle en cours et défaut. | Should | Voyants, écran ou interface réseau selon architecture. |
| F-008 | Le système devrait journaliser les cycles et défauts. | Could | Utile pour diagnostic mais non bloquant au prototype. |
| F-009 | Le système doit commander un seuil de niveau bas de sécurité distinct du seuil de lavage. | Must | Ce seuil protège l'installation en cas de manque d'eau. |
| F-010 | Le système doit couper une ou plusieurs prises auxiliaires lorsque le seuil bas est atteint. | Must | Typiquement pompes de bassin, UV ou autres équipements configurés. |
| F-011 | Le système doit maintenir les prises auxiliaires coupées tant que la condition de niveau bas persiste. | Must | Le redémarrage doit être maîtrisé pour éviter les oscillations. |
| F-012 | Le système doit permettre de définir quelles sorties sont coupées par la sécurité niveau bas. | Should | Permet d'adapter le produit à différentes installations. |

## Séquence nominale de lavage

```mermaid
sequenceDiagram
    participant C as Capteur
    participant Ctrl as Contrôleur
    participant P as Pompe de rinçage
    participant M as Moteur tambour

    C->>Ctrl: Demande de lavage
    Ctrl->>Ctrl: Vérifie délais et défauts
    Ctrl->>P: Active pompe
    Ctrl->>M: Active rotation
    Ctrl->>Ctrl: Attend durée configurée
    Ctrl->>M: Arrête rotation
    Ctrl->>P: Arrête pompe
    Ctrl->>Ctrl: Lance pause sécurité
```

## Séquence de sécurité niveau bas

```mermaid
sequenceDiagram
    participant N as Capteur niveau bas
    participant Ctrl as Contrôleur
    participant AUX as Prises auxiliaires

    N->>Ctrl: Niveau bas détecté
    Ctrl->>AUX: Coupe alimentation
    Ctrl->>Ctrl: Interdit ou limite les actions automatiques
    N->>Ctrl: Niveau redevenu normal
    Ctrl->>Ctrl: Vérifie temporisation ou acquittement
    Ctrl->>AUX: Réautorise alimentation
```
