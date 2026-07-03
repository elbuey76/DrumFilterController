# NC-0002 - Dimensionnement de la motorisation du tambour

## Objet

Documenter les hypothèses de dimensionnement retenues pour le moteur de rotation du tambour du FAT et les validations nécessaires avant intégration definitive.

## Données d'entrée

| Donnée | Valeur | Source / commentaire |
| --- | --- | --- |
| Type de moteur | Moteur d'essuie-glace avant avec reducteur intégré et tringlerie | Pièce candidate pour entrainer le tambour |
| Vehicule d'origine | Peugeot 106 phase 2 | Donnée annoncé pièce |
| Fabricant | SWF | Donnée annoncé pièce |
| Référence SWF | 403.835 / 403835 | Donnée annoncé pièce |
| Référence longue observée | 7905562055 | Donnée annoncé pièce |
| Référence PSA probablement associée | 640583 / 6405.83 | Équivalence probable, non considérée comme preuve directe |
| Connecteur | 5 broches | Donnée annoncé pièce |
| Tension nominale | 12 V DC | Hypothèse cohérente avec un moteur automobile d'essuie-glace |
| Vitesse a utiliser | Petite vitesse uniquement | Choix de conception pour limiter vitesse, bruit, a-coups et consommation |
| Puissance nominale estimée | environ 40 W | Hypothèse de dimensionnement, non confirmée par fiche constructeur |
| Vitesse sortie moteur estimée | probablement 40 à 60 tr/min a vide | Valeur typique à mesurer sur la pièce réelle |
| Couple moteur | inconnu, probablement quelques N.m | Donnée critique à valider expérimentalement |
| Usage prévu | Intermittent pendant les cycles de lavage | Compatible avec le type de moteur envisagé |
| Usage continu 24/24 | non recommandé | Risque d'échauffement, moteur non retenu pour fonctionnement permanent |
| Étanchéité | non compatible immersion | Moteur a garder hors eau et protège des projections |

## Hypothèses électriques de dimensionnement

| Hypothèse | Valeur retenue | Commentaire |
| --- | --- | --- |
| Tension moteur | 12 V DC | Alimentation moteur distincte ou rail 12 V dimensionne en conséquence |
| Courant nominal théorique | environ 3,3 A | Calcul indicatif 40 W / 12 V |
| Courant de fonctionnement à prévoir | 4 à 8 A | Hypothèse prudente en charge legere a modérée |
| Courant de démarrage ou effort important | 10 à 20 A possible | A prendre en compte pour alimentation, fusible et protection |
| Alimentation minimale | 12 V / 10 A | Minimum à valider sur essais |
| Alimentation recommandée | 12 V / 15 A | Marge utile pour démarrage, frottements et encrassement |
| Fusible initial | 10 à 15 A | A ajuster après mesure du courant réel |
| Protection blocage | détection surintensité ou limitation de courant | Fortement recommandée, le blocage peut faire monter le courant rapidement |

Le courant nominal théorique ne doit pas être utilisé seul pour dimensionner l'alimentation ou les protections. Le courant de démarrage, le couple résistant du tambour, l'encrassement et un éventuel blocage mécanique sont les cas dimensionnants.

## Transmission mécanique retenue

| Element | Valeur | Commentaire |
| --- | --- | --- |
| Pignon moteur | 10 cm | Donnée projet |
| Engrenage tambour | 30 cm | Donnée projet |
| Rapport de réduction | 3:1 | 30 cm / 10 cm |
| Effet théorique sur vitesse | vitesse divisee par 3 | Hors glissement et pertes |
| Effet théorique sur couple | couple multiplie par environ 3 | Hors pertes mécaniques |

Avec une vitesse moteur estimée de 40 à 60 tr/min, la vitesse tambour attendue serait d'environ 13 à 20 tr/min. Cette valeur reste indicative tant que la vitesse réelle du moteur n'a pas été mesurée.

## Contraintes de câblage

Le brochage exact du moteur n'est pas encore confirmé. L'architecture probable d'un moteur d'essuie-glace 5 broches comprend :

- masse ;
- petite vitesse ;
- grande vitesse ;
- alimentation ou retour parking ;
- contact parking.

Le câblage final doit être identifié au multimetre et par essai 12 V protège. Le projet ne doit pas supposer le brochage exact tant que la pièce n'a pas été caractérisée.

## Conclusion de dimensionnement

Le moteur SWF 403.835 de Peugeot 106 phase 2 est un candidat acceptable pour entrainer le tambour du FAT en usage intermittent, avec réduction mécanique 3:1 et petite vitesse uniquement.

La conception ne doit pas dependre d'une valeur de couple constructeur non disponible. Le dimensionnement final doit être valide par essais reels sur le tambour.

## Points à vérifier sur installation réelle

- identifier le brochage exact du moteur ;
- mesurer le courant a vide ;
- mesurer la vitesse a vide ;
- mesurer le courant avec tambour à sec ;
- mesurer le courant avec tambour en eau ;
- mesurer le courant au démarrage ;
- realiser un test de blocage contrôlé pour définir le seuil de sécurité ;
- valider un cycle de lavage complet sans blocage ni échauffement excessif.
