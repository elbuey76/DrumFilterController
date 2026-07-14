# NC-0002 - Dimensionnement de la motorisation du tambour

## Objet

Documenter les hypotheses de dimensionnement retenues pour le moteur de rotation du tambour du FAT et les validations necessaires pour le MVP definitif.

## Decision de reference

La motorisation tambour V1 retenue est un motorreducteur Fyearfly 12 VDC 10 rpm. Ce choix remplace le candidat initial de moteur d'essuie-glace SWF 403.835.

Le moteur est alimente par le rail 12 VDC issu d'une alimentation Mean Well NDR-120-12, 120 W, 10 A. Son depart est protege par un fusible ATO 5 A et commande par un relais HELLA 4RD 933 332-551, 12 V, donne pour 15 A en charge inductive. Un support sera imprime en 3D pour permettre le montage propre du relais sur rail DIN.

## Donnees d'entree

| Donnee | Valeur | Source / commentaire |
| --- | --- | --- |
| Type de moteur | Motorreducteur a vis sans fin | Piece retenue pour entrainer le tambour |
| Marque | Fyearfly | Donnee capture produit |
| Tension nominale | 12 V DC | Rail basse tension V1 |
| Vitesse selectionnee | 10 rpm | Variante retenue sur la capture produit |
| Couple nominal annonce, variante 10 rpm | 70 kg.cm, soit environ 6,9 N.m | Donnee fournisseur issue de la capture transmise, a confirmer par mesure |
| Couple maximal annonce, variante 10 rpm | 70 kg.cm, soit environ 6,9 N.m | Valeur identique au couple nominal sur la capture ; a traiter avec prudence |
| Courant nominal annonce | < 1,6 A | Donnee produit, a confirmer par mesure |
| Courant de blocage / d'arret annonce | 6,5 A | Donnee produit issue du screenshot fournisseur transmis, utile pour pre-dimensionner le fusible |
| Sortie mecanique | Arbre en D, diametre 8 mm | Donnee produit |
| Usage prevu | Intermittent pendant les cycles de lavage | Compatible avec la logique FAT |
| Usage continu 24/24 | non retenu | Le tambour ne tourne pas en permanence |
| Etancheite | non documentee pour immersion | Moteur a garder hors eau et protege des projections |

## Donnees fournisseur issues de la capture produit

Source conservee dans le depot : [capture Fyearfly 12 V motor speed options](../assets/fyearfly-12v-motor-speed-options.png).

La capture fournisseur transmise indique huit variantes de vitesse pour le meme type de motorreducteur 12 VCC : 400, 200, 130, 50, 30, 20, 12 et 10 tr/min. Elle indique aussi un arbre en D de diametre 8 mm, un courant nominal inferieur ou egal a 1,6 A et un courant de blocage de 6,5 A pour toutes les variantes.

| Vitesse nominale | Couple nominal annonce | Couple max annonce | Courant nominal annonce | Courant blocage annonce |
| ---: | ---: | ---: | ---: | ---: |
| 400 tr/min | 3,4 kg.cm | 8,5 kg.cm | <= 1,6 A | 6,5 A |
| 200 tr/min | 6,2 kg.cm | 16 kg.cm | <= 1,6 A | 6,5 A |
| 130 tr/min | 10 kg.cm | 25 kg.cm | <= 1,6 A | 6,5 A |
| 50 tr/min | 20 kg.cm | 50 kg.cm | <= 1,6 A | 6,5 A |
| 30 tr/min | 40 kg.cm | 70 kg.cm | <= 1,6 A | 6,5 A |
| 20 tr/min | 60 kg.cm | 70 kg.cm | <= 1,6 A | 6,5 A |
| 12 tr/min | 70 kg.cm | 70 kg.cm | <= 1,6 A | 6,5 A |
| 10 tr/min | 70 kg.cm | 70 kg.cm | <= 1,6 A | 6,5 A |

Pour les calculs mecaniques, 1 kg.cm est interprete comme 1 kgf.cm, soit environ 0,098 N.m. Le couple annonce pour la variante 10 tr/min vaut donc environ 6,9 N.m en sortie de motorreducteur. Cette valeur est utile pour pre-dimensionner les engrenages, mais elle ne doit pas remplacer les essais sur le tambour reel : la capture produit ne precise pas les conditions de mesure, le service admissible, l'echauffement, ni la duree acceptable au couple maximal.

## Hypotheses electriques de dimensionnement

| Hypothese | Valeur retenue | Commentaire |
| --- | --- | --- |
| Tension moteur | 12 V DC | Alimentation commune basse tension V1 |
| Alimentation disponible | Mean Well NDR-120-12, 12 V, 10 A, 120 W | Alimente aussi automate, capteurs, IHM et accessoires via fusibles dedies |
| Fusible moteur | ATO 5 A | Retenu comme calibre plus restrictif que l'hypothese 7,5 A afin de mieux couvrir le blocage annonce a 6,5 A ; a verifier imperativement au demarrage et en charge |
| Relais moteur | HELLA 4RD 933 332-551, 12 V, 15 A inductif | Marge par rapport au fusible et a l'appel moteur attendu |
| Distribution 12 V | Porte-fusibles ATO 4 departs | Moteur 5 A, automate 3 A, capteurs/boutons 1 A, ecran/voyants/accessoires 1 A |

Le courant nominal annonce ne suffit pas a lui seul pour valider le dimensionnement. Le screenshot fournisseur donne un courant de blocage de 6,5 A, ce qui rend l'hypothese 7,5 A trop peu protectrice contre un blocage. Le calibre 5 A est donc retenu pour essai, sous reserve de ne pas declencher au demarrage ni en charge normale.

Le couple annonce de 70 kg.cm pour la variante 10 tr/min donne un ordre de grandeur favorable, mais doit rester une donnee fournisseur non validee. Les cas dimensionnants restant a observer sur le montage MVP sont le demarrage, l'effort en charge avec tambour mouille, les frottements, l'encrassement et le comportement effectif en blocage mecanique.

## Transmission mecanique

La vitesse moteur retenue de 10 rpm est beaucoup plus proche d'une vitesse exploitable pour un FAT que l'ancien candidat SWF mesure a 95 tr/min. La transmission exacte reste toutefois a definir selon :

- le diametre et la fixation de l'organe d'entrainement cote moteur ;
- le diametre et la fixation cote tambour ;
- le couple disponible au tambour ;
- l'accessibilite maintenance ;
- la tolerance aux projections et a l'encrassement.

La vitesse finale du tambour doit etre validee en essai reel. Si l'entrainement est direct, l'ordre de grandeur serait 10 tr/min au tambour, ce qui peut encore etre rapide selon la geometrie reelle. Une reduction supplementaire ou une commande par cycles courts pourra donc rester necessaire selon essais.

### Incidence pour le choix des engrenages

Le tambour existant fait 31 cm de diametre, soit une circonference d'environ 97,4 cm. Avec le moteur 10 tr/min, un entrainement direct donnerait environ 16,2 cm/s en vitesse peripherique du tambour.

| Reduction externe | Vitesse tambour | Vitesse peripherique | Temps pour 1 tour | Couple theorique au tambour hors pertes |
| ---: | ---: | ---: | ---: | ---: |
| 1:1 | 10,0 tr/min | 16,2 cm/s | 6 s | 1,0 x couple moteur |
| 1,5:1 | 6,7 tr/min | 10,8 cm/s | 9 s | 1,5 x couple moteur |
| 2:1 | 5,0 tr/min | 8,1 cm/s | 12 s | 2,0 x couple moteur |
| 3:1 | 3,3 tr/min | 5,4 cm/s | 18 s | 3,0 x couple moteur |

Si la plage observee sur des FAT du commerce est retenue comme repere, soit environ 8 a 14 cm/s de vitesse peripherique, une reduction 2:1 place le tambour au bas de cette plage. Une reduction 3:1 favorise davantage le couple et la tolerance aux frottements, mais sort de cette plage avec une vitesse d'environ 5,4 cm/s et implique des cycles de lavage plus longs pour presenter toute la circonference aux buses.

Avec le couple fournisseur annonce de 70 kg.cm, soit environ 6,9 N.m, une reduction 2:1 donnerait un couple theorique d'environ 13,7 N.m avant pertes externes. En tenant compte d'un rendement mecanique prudent de 80 a 90 % pour les engrenages, l'ordre de grandeur exploitable au tambour serait environ 11 a 12 N.m, soit une force tangentielle d'environ 70 a 80 N au rayon 15,5 cm. Ces valeurs restent a confirmer par un essai de traction ou de rotation sur le tambour reel.

### Hypothese constructive retenue pour la transmission

L'hypothese constructive de travail pour la transmission tambour est un engrenage droit imprime en PETG, module 4, rapport 2:1, avec une petite roue de 30 dents cote moteur et une grande couronne de 60 dents cote tambour.

| Element | Valeur de travail | Commentaire |
| --- | ---: | --- |
| Type | Engrenages droits | Solution simple a imprimer et a regler |
| Matiere engrenages | PETG | A valider en usure, temperature locale et tenue a l'humidite |
| Module | 4 | Dents robustes et tolerantes pour impression 3D |
| Angle de pression | 20 degres | Valeur retenue, standard moderne et plus favorable a la resistance du pied de dent que 14,5 degres |
| Largeur denture | 25 mm | Largeur confortable pour le couple attendu |
| Pignon moteur | 30 dents | Monte sur moyeu aluminium pour arbre en D 8 mm |
| Couronne tambour | 60 dents | Imprimee en 3 segments de 120 degres, 20 dents par segment |
| Rapport | 2:1 | 10 tr/min moteur vers 5 tr/min tambour |
| Entraxe theorique | 180 mm | (120 + 240) / 2 ; support moteur a rendre reglable, plage utile indicative 178 a 182 mm |

Dimensions geometriques principales :

| Element | Diametre primitif | Diametre exterieur approx. |
| --- | ---: | ---: |
| Pignon 30 dents module 4 | 120 mm | 128 mm |
| Couronne 60 dents module 4 | 240 mm | 248 mm |

Autres cotes de reference pour la couronne :

| Element | Valeur | Commentaire |
| --- | ---: | --- |
| Diametre de pied de dent couronne, approx. | 230 mm | Valeur theorique indicative pour denture standard a developpante ; conserver le profil genere par le generateur d'engrenage |
| Alesage central couronne | 160 mm | Laisse environ 35 mm de matiere radiale entre l'alesage et le fond de dent |
| Cercle de percage fixation | 198 mm | Rayon 99 mm ; compromis entre les marges cote alesage et cote denture |
| Nombre de vis | 12 x M4 | 4 vis par secteur de 120 degres |

Cette solution donne une vitesse tambour d'environ 5 tr/min, soit 8,1 cm/s en vitesse peripherique pour le tambour de 31 cm de diametre. Le temps pour un tour complet de tambour est d'environ 12 s.

### Efforts mecaniques de reference

Avec le couple fournisseur annonce de 6,9 N.m en sortie du motorreducteur et le pignon moteur de diametre primitif 120 mm, le rayon primitif cote pignon vaut 60 mm.

```text
Effort tangentiel denture nominal = 6,9 N.m / 0,060 m
                                  = environ 115 N
                                  = environ 11,7 kgf
```

En tenant compte des a-coups, du joint a levre, de la toile mouillee, de l'encrassement et d'un debut de blocage possible, la transmission doit etre consideree avec une marge de calcul pratique d'environ 2 a 3 fois l'effort nominal, soit environ 250 a 350 N au niveau de la denture.

Avec un angle de pression de 20 degres, l'effort radial d'engrenement vaut environ :

```text
Effort radial nominal = 115 N x tan(20 degres)
                      = environ 42 N
```

Le moyeu aluminium sur l'arbre en D reprend principalement le couple transmis et limite l'ecrasement ou l'usure du PETG au niveau de l'arbre. Il ne supprime pas l'effort radial applique au motorreducteur par l'engrenement. Le pignon doit donc etre place aussi pres que possible du carter du motorreducteur afin de limiter le porte-a-faux ; un palier de soutien externe reste une option si les essais montrent une flexion, du bruit, ou un effort excessif sur l'arbre.

### Couronne tambour en trois segments

La grande couronne de 60 dents est prevue en trois segments imprimes de 120 degres, soit 20 dents par segment. Les segments seront visses sur une plaque laterale du tambour. La plaque laterale doit assurer le centrage de la couronne ; les vis seules ne doivent pas etre le seul moyen de centrage.

Principes de conception retenus :

- 4 vis par segment, soit 12 vis inox M4 au total, sur le cercle de percage de diametre 198 mm ;
- dans chaque secteur, placer les vis a 15, 45, 75 et 105 degres depuis une jonction : 30 degres entre vis et 15 degres de marge aux deux jonctions ;
- trou de passage PETG diametre 4,5 mm ; rondelle large M4 diametre exterieur 12 mm cote PETG ;
- si une face affleurante est necessaire, utiliser un lamage cylindrique a fond plat diametre 13,5 a 14 mm, profondeur environ 5,2 mm ; ne pas utiliser de fraisure conique dans le PETG ;
- utiliser des vis traversantes avec ecrou frein cote metal, ou un taraudage dans une plaque metallique ; ne pas reprendre le couple avec un filetage dans le PETG ;
- conserver un reglage d'entraxe cote motorreducteur pour ajuster le jeu d'engrenement ;
- placer le pignon aussi pres que possible du carter du motorreducteur pour limiter le porte-a-faux de l'arbre.

#### Alignement par queues d'aronde axiales

Les trois secteurs sont relies par des queues d'aronde glissant dans le sens axial, c'est-a-dire sur l'epaisseur de 25 mm de la couronne. Cette orientation permet de presenter le dernier secteur axialement et de l'engager simultanement entre les deux autres, contrairement a une queue d'aronde qui demanderait un coulissement tangentiel sur une couronne deja fermee.

Chaque secteur comporte un cote male et un cote femelle, avec la meme orientation autour de la couronne. Les queues d'aronde servent a realigner les segments et les dents ; le couple global reste repris par le serrage des vis sur la plaque laterale.

| Cote du profil de queue d'aronde | Valeur retenue | Commentaire |
| --- | ---: | --- |
| Profondeur tangentielle male | 8 mm | Penetration du male dans la rainure voisine |
| Largeur radiale au col | 10 mm | Au plan de jonction |
| Largeur radiale en tete | environ 14,3 mm | Resultat de 10 + 2 x 8 x tan(15 degres) |
| Angle des flancs | 15 degres par flanc | Soit 30 degres inclus |
| Longueur de guidage axiale | 25 mm | Sur toute l'epaisseur de la couronne |
| Jeu de la rainure femelle | +0,30 mm par flanc | A ajuster par coupon d'essai selon l'imprimante PETG |
| Jeu au fond de rainure | +0,40 mm | Evite de bloquer sur le fond |
| Entree de glissiere | Chanfrein 1 x 45 degres | Sur le male et l'entree de la femelle |
| Aretes longitudinales | R0,3 mm ou chanfrein 0,3 x 45 degres | Casse les bavures sans arrondir les flancs actifs a 15 degres |

Le profil est centre dans les 35 mm de matiere radiale disponibles entre l'alesage diametre 160 mm et le fond de dent diametre environ 230 mm. Avec une tete de queue d'aronde d'environ 14,3 mm, il reste environ 10 mm de marge radiale de chaque cote.

Objectif de montage pour la couronne :

```text
Faux-rond radial vise : < 0,5 mm
Faux-rond acceptable pour premiers essais : environ 1 mm
Au-dela : risque de points durs, bruit et usure acceleree
```

### Parametres d'impression de depart

Les parametres ci-dessous sont des valeurs de depart pour les prototypes PETG, a ajuster selon l'imprimante, la buse et les essais.

| Parametre | Valeur de depart |
| --- | --- |
| Matiere | PETG |
| Orientation | Engrenages imprimes a plat |
| Buse | 0,6 mm preferee, 0,4 mm acceptable |
| Hauteur de couche | 0,24 a 0,28 mm avec buse 0,6 mm |
| Perimetres | 8 |
| Remplissage | 60 a 80 % |
| Motif de remplissage | Gyroid ou motif continu equivalent |
| Couches pleines haut/bas | 6 a 8 |
| Vitesse paroi externe | Environ 25 a 35 mm/s |
| Position de couture | Cote alesage interieur, jamais dans le fond de dent |
| Largeur denture | 25 mm |
| Renforts | Si le slicer le permet : 100 % localement autour des vis et dans la couronne sous les dents |

Les segments doivent etre imprimes a plat, grande face sur le plateau. Cette orientation place les couches dans le plan des efforts de flexion des dents. Le filament PETG doit etre seche avant impression. Un remplissage a 100 % sur toute la piece n'est pas impose : les perimetres, la geometrie du pied de dent et les renforts locaux sont plus importants, avec moins de risque de deformation sur les grands secteurs.

Le raccordement flanc/fond de dent doit recevoir un conge de rayon 0,8 a 1,0 mm, sans depasser R1,2 mm et sans modifier le profil actif des flancs de dent. Un petit chanfrein ou rayon de 0,3 a 0,5 mm peut etre applique aux aretes axiales externes des dents, aux entrees de trous et aux aretes de queue d'aronde pour supprimer les bavures d'impression. Un coupon d'essai incluant 3 ou 4 dents, une jonction a queue d'aronde et un trou M4 est a realiser avant l'impression des trois secteurs definitifs.

## Contraintes de cablage

Le depart moteur est cable en 12 VDC via fusible ATO 5 A et relais HELLA 12 V. Le support du relais doit etre concu pour une fixation propre en coffret, sur rail DIN, sans contrainte mecanique sur les cosses.

Le cablage final doit verifier :

- la polarite moteur et le sens de rotation utile ;
- la section de cable du depart moteur ;
- la diode ou protection de roue libre si necessaire cote commande relais ;
- la separation propre entre basse tension et puissance 230 VAC ;
- l'accessibilite du fusible moteur.

## Ancienne option non retenue

Le moteur d'essuie-glace avant SWF 403.835 de Peugeot 106 phase 2 avait ete etudie comme candidat initial. Il etait mesure a environ 95 tr/min a vide sur batterie 12 V. Avec une reduction mecanique initiale 3:1, la vitesse tambour attendue etait d'environ 31,7 tr/min a vide, probablement trop elevee pour le FAT. Ce candidat n'est plus la base de conception V1.

## Conclusion de dimensionnement

Le motorreducteur Fyearfly 12 VDC 10 rpm est retenu pour simplifier la vitesse de rotation et le cablage V1. La capture fournisseur annonce 70 kg.cm de couple pour cette variante, soit environ 6,9 N.m, mais cette valeur doit etre confirmee par essais. Le pre-dimensionnement electrique retient une alimentation 12 VDC 10 A commune, un fusible moteur 5 A a valider par essais et un relais HELLA 15 A inductif.

Le courant de blocage annonce de 6,5 A repond au besoin de pre-dimensionnement pour la commande definitive du MVP. La conception ne doit toutefois pas dependre uniquement des donnees de la capture produit : le comportement final reste a verifier sur le tambour reel avant de considerer le cablage et la protection comme pleinement valides.

## Points a verifier sur installation reelle

- valider le sens de rotation du moteur ;
- mesurer le courant a vide ;
- mesurer le courant avec tambour a sec ;
- mesurer le courant avec tambour en eau ;
- mesurer le courant au demarrage ;
- realiser un test de blocage controle pour verifier le comportement reel du fusible 5 A et de la protection, en reference au courant de blocage annonce de 6,5 A ;
- estimer le couple necessaire au tambour par essai de traction, a sec puis mouille, avec toile et joint en place ;
- mesurer la vitesse finale du tambour avec la transmission retenue ;
- valider un cycle de lavage complet sans blocage ni echauffement excessif ;
- verifier la tenue mecanique du support relais HELLA imprime en 3D.
