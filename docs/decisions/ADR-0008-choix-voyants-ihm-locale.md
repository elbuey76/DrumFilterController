# ADR-0008 - Choix des voyants de l'IHM locale

## Statut

Acceptée

## Contexte

L'IHM locale V1 repose principalement sur un écran LCD 2004 / 20x4 I2C 3,3 V. Les voyants physiques ne doivent pas remplacer l'écran, mais donner une lecture immédiate des états principaux en façade du coffret.

Le coffret est installé dans un local technique protégé de la pluie directe, mais humide, avec condensation et projections possibles. Les organes de façade doivent donc rester adaptés à cet environnement et cohérents avec le coffret IP55 minimum, préférence IP65.

La distribution basse tension V1 dispose d'un départ 12 VDC dédié `Ecran, voyants, accessoires`, protégé par fusible 1 A. Utiliser des voyants 12 VDC évite d'ajouter du 230 VAC en façade pour une simple signalisation.

## Décision

Les voyants physiques retenus pour le montage MVP sont des voyants LED de façade étanches, diamètre 16 mm, alimentation 12 VDC :

- 1 voyant vert `VOYANT_MARCHE` ;
- 1 voyant rouge `VOYANT_ALARME` ;
- 1 voyant jaune `VOYANT_LAVAGE`.

Le voyant rouge reste fixe en V1 pour toute alarme active, défaut actif ou `A15 - CAPOT OUVERT LONG`. Aucun clignotement n'est retenu en V1.

Le voyant jaune est retenu pour le montage MVP même si la fonction restait optionnelle dans le besoin minimal initial. L'écran reste la source du détail des modes, alarmes et messages.

## Conséquences

- La façade du coffret doit prévoir trois perçages de 16 mm pour les voyants.
- Les voyants sont raccordés côté 12 VDC, sur le départ `Ecran, voyants, accessoires`.
- Le schema electrique devra commander chaque voyant 12 VDC par une sortie MOSFET KC868-A16, apres verification du courant, de la polarite et de l'etat OFF au demarrage.
- Les références finales doivent confirmer la compatibilité 12 VDC, la présence d'un joint de façade, l'indice d'étanchéité annoncé et le mode de raccordement arrière.
- Les voyants 220 VAC et 24 V ne sont pas retenus pour l'IHM locale V1.

## Alternatives considérées

- Voyants 22 mm : plus standards en façade industrielle, mais plus encombrants. Non retenus ici au profit du format 16 mm demandé pour le montage.
- Voyants 220 VAC : évitent une interface basse tension, mais ajoutent inutilement du secteur en façade pour une signalisation simple.
- Voyants 24 VDC : cohérents avec certains coffrets industriels, mais l'architecture V1 distribue déjà du 12 VDC pour l'IHM et les accessoires.
- Voyants minimum vert + rouge seulement : conforme au besoin minimal, mais le voyant jaune est finalement retenu pour rendre le lavage/test visible d'un coup d'oeil.
