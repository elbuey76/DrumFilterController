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
| S-005 | Le système doit couper sur niveau bas la pompe principale, la pompe décoration, l'UV et la mise à niveau automatique. | Must | Réduit le risque de vidange du bassin, de marche à sec et de remplissage infini sur fuite. |
| S-006 | Le système doit inhiber sur niveau bas toute rotation du tambour et toute commande de pompe de rinçage. | Must | Le FAT ne doit plus lancer de lavage en situation d'eau insuffisante. |
| S-007 | Le système doit préserver l'alimentation du bulleur de la cuve bio et du bulleur du bassin même en niveau bas. | Must | Ces fonctions restent utiles pour la survie biologique et l'oxygénation. |
| S-008 | Le système devrait détecter l'absence de rotation du tambour. | Could | Nécessite un capteur supplémentaire. |
| S-009 | Le système devrait intégrer une temporisation ou hystérésis de réarmement après niveau bas. | Should | Évite les remises sous tension répétées. |

## Politique de repli en niveau bas

En cas de détection de niveau d'eau trop bas, le système doit se placer dans un état sûr qui :

- coupe les équipements susceptibles de vider le bassin ou de fonctionner à sec ;
- interdit les fonctions de lavage du FAT ;
- maintient les fonctions d'aération considérées vitales pour la biologie et les poissons.

## Contraintes environnementales

- humidité importante ;
- projections d'eau possibles ;
- parasites électriques liés aux moteurs ;
- accès maintenance potentiellement limité ;
- besoin de composants remplaçables facilement.
