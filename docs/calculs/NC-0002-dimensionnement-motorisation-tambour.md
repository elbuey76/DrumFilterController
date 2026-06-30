# NC-0002 - Dimensionnement de la motorisation du tambour

## Objet

Documenter les hypotheses de dimensionnement retenues pour le moteur de rotation du tambour du FAT et les validations necessaires avant integration definitive.

## Donnees d'entree

| Donnee | Valeur | Source / commentaire |
| --- | --- | --- |
| Type de moteur | Moteur d'essuie-glace avant avec reducteur integre et tringlerie | Piece candidate pour entrainer le tambour |
| Vehicule d'origine | Peugeot 106 phase 2 | Donnee annonce piece |
| Fabricant | SWF | Donnee annonce piece |
| Reference SWF | 403.835 / 403835 | Donnee annonce piece |
| Reference longue observee | 7905562055 | Donnee annonce piece |
| Reference PSA probablement associee | 640583 / 6405.83 | Equivalence probable, non consideree comme preuve directe |
| Connecteur | 5 broches | Donnee annonce piece |
| Tension nominale | 12 V DC | Hypothese coherente avec un moteur automobile d'essuie-glace |
| Vitesse a utiliser | Petite vitesse uniquement | Choix de conception pour limiter vitesse, bruit, a-coups et consommation |
| Puissance nominale estimee | environ 40 W | Hypothese de dimensionnement, non confirmee par fiche constructeur |
| Vitesse sortie moteur estimee | probablement 40 a 60 tr/min a vide | Valeur typique a mesurer sur la piece reelle |
| Couple moteur | inconnu, probablement quelques N.m | Donnee critique a valider experimentalement |
| Usage prevu | Intermittent pendant les cycles de lavage | Compatible avec le type de moteur envisage |
| Usage continu 24/24 | non recommande | Risque d'echauffement, moteur non retenu pour fonctionnement permanent |
| Etancheite | non compatible immersion | Moteur a garder hors eau et protege des projections |

## Hypotheses electriques de dimensionnement

| Hypothese | Valeur retenue | Commentaire |
| --- | --- | --- |
| Tension moteur | 12 V DC | Alimentation moteur distincte ou rail 12 V dimensionne en consequence |
| Courant nominal theorique | environ 3,3 A | Calcul indicatif 40 W / 12 V |
| Courant de fonctionnement a prevoir | 4 a 8 A | Hypothese prudente en charge legere a moderee |
| Courant de demarrage ou effort important | 10 a 20 A possible | A prendre en compte pour alimentation, fusible et protection |
| Alimentation minimale | 12 V / 10 A | Minimum a valider sur essais |
| Alimentation recommandee | 12 V / 15 A | Marge utile pour demarrage, frottements et encrassement |
| Fusible initial | 10 a 15 A | A ajuster apres mesure du courant reel |
| Protection blocage | detection surintensite ou limitation de courant | Fortement recommandee, le blocage peut faire monter le courant rapidement |

Le courant nominal theorique ne doit pas etre utilise seul pour dimensionner l'alimentation ou les protections. Le courant de demarrage, le couple resistant du tambour, l'encrassement et un eventuel blocage mecanique sont les cas dimensionnants.

## Transmission mecanique retenue

| Element | Valeur | Commentaire |
| --- | --- | --- |
| Pignon moteur | 10 cm | Donnee projet |
| Engrenage tambour | 30 cm | Donnee projet |
| Rapport de reduction | 3:1 | 30 cm / 10 cm |
| Effet theorique sur vitesse | vitesse divisee par 3 | Hors glissement et pertes |
| Effet theorique sur couple | couple multiplie par environ 3 | Hors pertes mecaniques |

Avec une vitesse moteur estimee de 40 a 60 tr/min, la vitesse tambour attendue serait d'environ 13 a 20 tr/min. Cette valeur reste indicative tant que la vitesse reelle du moteur n'a pas ete mesuree.

## Contraintes de cablage

Le brochage exact du moteur n'est pas encore confirme. L'architecture probable d'un moteur d'essuie-glace 5 broches comprend :

- masse ;
- petite vitesse ;
- grande vitesse ;
- alimentation ou retour parking ;
- contact parking.

Le cablage final doit etre identifie au multimetre et par essai 12 V protege. Le projet ne doit pas supposer le brochage exact tant que la piece n'a pas ete caracterisee.

## Conclusion de dimensionnement

Le moteur SWF 403.835 de Peugeot 106 phase 2 est un candidat acceptable pour entrainer le tambour du FAT en usage intermittent, avec reduction mecanique 3:1 et petite vitesse uniquement.

La conception ne doit pas dependre d'une valeur de couple constructeur non disponible. Le dimensionnement final doit etre valide par essais reels sur le tambour.

## Points a verifier sur installation reelle

- identifier le brochage exact du moteur ;
- mesurer le courant a vide ;
- mesurer la vitesse a vide ;
- mesurer le courant avec tambour a sec ;
- mesurer le courant avec tambour en eau ;
- mesurer le courant au demarrage ;
- realiser un test de blocage controle pour definir le seuil de securite ;
- valider un cycle de lavage complet sans blocage ni echauffement excessif.
