# NC-0002 - Dimensionnement de la motorisation du tambour

## Objet

Documenter les hypotheses de dimensionnement retenues pour le moteur de rotation du tambour du FAT et les validations necessaires pour le MVP definitif.

## Decision de reference

La motorisation tambour V1 retenue est un motorreducteur Fyearfly 12 VDC 10 rpm. Ce choix remplace le candidat initial de moteur d'essuie-glace SWF 403.835.

Le moteur est alimente par le rail 12 VDC issu d'une alimentation Mean Well NDR-120-12, 120 W, 10 A. Son depart est protege par un fusible ATO 7,5 A et commande par un relais HELLA 4RD 933 332-551, 12 V, donne pour 15 A en charge inductive. Un support sera imprime en 3D pour permettre le montage propre du relais sur rail DIN.

## Donnees d'entree

| Donnee | Valeur | Source / commentaire |
| --- | --- | --- |
| Type de moteur | Motorreducteur a vis sans fin | Piece retenue pour entrainer le tambour |
| Marque | Fyearfly | Donnee capture produit |
| Tension nominale | 12 V DC | Rail basse tension V1 |
| Vitesse selectionnee | 10 rpm | Variante retenue sur la capture produit |
| Courant nominal annonce | < 1,6 A | Donnee produit, a confirmer par mesure |
| Courant de blocage / d'arret annonce | 6,5 A | Donnee produit issue du screenshot fournisseur transmis, utile pour pre-dimensionner le fusible |
| Sortie mecanique | Arbre en D, diametre 8 mm | Donnee produit |
| Usage prevu | Intermittent pendant les cycles de lavage | Compatible avec la logique FAT |
| Usage continu 24/24 | non retenu | Le tambour ne tourne pas en permanence |
| Etancheite | non documentee pour immersion | Moteur a garder hors eau et protege des projections |

## Hypotheses electriques de dimensionnement

| Hypothese | Valeur retenue | Commentaire |
| --- | --- | --- |
| Tension moteur | 12 V DC | Alimentation commune basse tension V1 |
| Alimentation disponible | Mean Well NDR-120-12, 12 V, 10 A, 120 W | Alimente aussi automate, capteurs, IHM et accessoires via fusibles dedies |
| Fusible moteur | ATO 7,5 A | Coherent avec un courant de blocage annonce de 6,5 A ; le comportement reel de protection reste a verifier sur montage MVP |
| Relais moteur | HELLA 4RD 933 332-551, 12 V, 15 A inductif | Marge par rapport au fusible et a l'appel moteur attendu |
| Distribution 12 V | Porte-fusibles ATO 4 departs | Moteur 7,5 A, automate 3 A, capteurs/boutons 1 A, ecran/voyants/accessoires 1 A |

Le courant nominal annonce ne suffit pas a lui seul pour valider le dimensionnement. Le screenshot fournisseur donne un courant de blocage de 6,5 A, ce qui rend coherent le fusible ATO 7,5 A en premiere intention. Les cas dimensionnants restant a observer sur le montage MVP sont le demarrage, l'effort en charge avec tambour mouille, les frottements, l'encrassement et le comportement effectif en blocage mecanique.

## Transmission mecanique

La vitesse moteur retenue de 10 rpm est beaucoup plus proche d'une vitesse exploitable pour un FAT que l'ancien candidat SWF mesure a 95 tr/min. La transmission exacte reste toutefois a definir selon :

- le diametre et la fixation de l'organe d'entrainement cote moteur ;
- le diametre et la fixation cote tambour ;
- le couple disponible au tambour ;
- l'accessibilite maintenance ;
- la tolerance aux projections et a l'encrassement.

La vitesse finale du tambour doit etre validee en essai reel. Si l'entrainement est direct, l'ordre de grandeur serait 10 tr/min au tambour, ce qui peut encore etre rapide selon la geometrie reelle. Une reduction supplementaire ou une commande par cycles courts pourra donc rester necessaire selon essais.

## Contraintes de cablage

Le depart moteur est cable en 12 VDC via fusible ATO 7,5 A et relais HELLA 12 V. Le support du relais doit etre concu pour une fixation propre en coffret, sur rail DIN, sans contrainte mecanique sur les cosses.

Le cablage final doit verifier :

- la polarite moteur et le sens de rotation utile ;
- la section de cable du depart moteur ;
- la diode ou protection de roue libre si necessaire cote commande relais ;
- la separation propre entre basse tension et puissance 230 VAC ;
- l'accessibilite du fusible moteur.

## Ancienne option non retenue

Le moteur d'essuie-glace avant SWF 403.835 de Peugeot 106 phase 2 avait ete etudie comme candidat initial. Il etait mesure a environ 95 tr/min a vide sur batterie 12 V. Avec une reduction mecanique initiale 3:1, la vitesse tambour attendue etait d'environ 31,7 tr/min a vide, probablement trop elevee pour le FAT. Ce candidat n'est plus la base de conception V1.

## Conclusion de dimensionnement

Le motorreducteur Fyearfly 12 VDC 10 rpm est retenu pour simplifier la vitesse de rotation et le cablage V1. Le pre-dimensionnement electrique retient une alimentation 12 VDC 10 A commune, un fusible moteur 7,5 A et un relais HELLA 15 A inductif.

Le courant de blocage annonce de 6,5 A repond au besoin de pre-dimensionnement pour la commande definitive du MVP. La conception ne doit toutefois pas dependre uniquement des donnees de la capture produit : le comportement final reste a verifier sur le tambour reel avant de considerer le cablage et la protection comme pleinement valides.

## Points a verifier sur installation reelle

- valider le sens de rotation du moteur ;
- mesurer le courant a vide ;
- mesurer le courant avec tambour a sec ;
- mesurer le courant avec tambour en eau ;
- mesurer le courant au demarrage ;
- realiser un test de blocage controle pour verifier le comportement reel du fusible 7,5 A et de la protection, en reference au courant de blocage annonce de 6,5 A ;
- mesurer la vitesse finale du tambour avec la transmission retenue ;
- valider un cycle de lavage complet sans blocage ni echauffement excessif ;
- verifier la tenue mecanique du support relais HELLA imprime en 3D.
