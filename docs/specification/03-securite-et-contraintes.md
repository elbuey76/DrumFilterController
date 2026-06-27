# Exigences de sécurité et contraintes

## Sécurité électrique

Le contrôleur doit séparer la basse tension de commande de la puissance moteur/pompe. Les éléments alimentés par le secteur doivent être installés dans un coffret adapté, avec protections électriques dimensionnées et câblage conforme aux règles applicables.

## Exigences de protection

| ID | Exigence | Priorité | Commentaire |
| --- | --- | --- | --- |
| S-001 | Le système doit couper les sorties en état de défaut. | Must | État sûr par défaut. |
| S-002 | Le système doit limiter la durée maximale d'un cycle. | Must | Protection pompe et moteur. |
| S-003 | Le système doit limiter les cycles répétés trop rapprochés. | Must | Détection d'encrassement anormal ou capteur bloqué. |
| S-004 | Le système doit détecter un niveau bas d'eau indépendant de la logique normale de lavage. | Must | Protection de pompe et des équipements du bassin. |
| S-005 | Le système doit couper les prises auxiliaires configurées sur détection de niveau bas. | Must | Sorties normalement au repos en cas de défaut ou perte de commande. |
| S-006 | Le système devrait détecter l'absence de rotation du tambour. | Could | Nécessite un capteur supplémentaire. |
| S-007 | Le système devrait intégrer une temporisation ou hystérésis de réarmement après niveau bas. | Should | Évite les remises sous tension répétées. |

## Contraintes environnementales

- humidité importante ;
- projections d'eau possibles ;
- parasites électriques liés aux moteurs ;
- accès maintenance potentiellement limité ;
- besoin de composants remplaçables facilement.
