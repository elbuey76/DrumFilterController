# NC-0003 - Dimensionnement de la pompe de rincage

## Objet

Documenter les caracteristiques connues de la pompe de rincage du FAT, les limites d'interpretation de sa courbe hydraulique et les validations necessaires avant dimensionnement definitif des buses, protections et consommations d'eau.

## Donnees d'identification

| Donnee | Valeur | Source / commentaire |
| --- | --- | --- |
| Marque commerciale | VEVOR | Pompe deja retenue pour les buses du FAT |
| Fabricant OEM | Leo Group Pump (Zhejiang) Co., Ltd. | Information fournisseur |
| Modele | EKJ-802S | Garden Jet Pump |
| Type | Pompe centrifuge de surface auto-amorcante | Pompe de jardin, non immergee |
| Numero de serie | 221228693910166 | Donnee plaque |
| Code observe | 10EKJ-802S | Donnee fiche technique |

## Donnees electriques

| Donnee | Valeur | Commentaire |
| --- | --- | --- |
| Alimentation | 220-240 VAC | Monophase |
| Frequence | 50 Hz | Standard secteur local |
| Puissance indiquee projet | 800 W | Puissance electrique absorbee fournie dans les donnees d'entree |
| Puissance fiche Leo | 0,6 kW / 0,8 HP | Donnee fiche technique EKJ-802S |
| Courant fiche Leo | 3,5 A | Coherent avec une pompe de l'ordre de 0,6 a 0,8 kW sous 230 V |
| Classe de protection electrique | Classe I | Mise a la terre obligatoire |
| Classe d'isolation | Classe F | Donnee fiche technique |
| Marquage CE | Oui | Donnee plaque / fournisseur |

Pour le dimensionnement des relais ou contacteurs, la pompe doit etre traitee comme une charge moteur 230 VAC. Le courant nominal ne suffit pas a lui seul : le courant d'appel au demarrage doit etre pris en compte dans le choix de l'organe de coupure.

## Donnees hydrauliques

| Donnee | Valeur | Commentaire |
| --- | --- | --- |
| Debit maximal indique | 3,6 a 3,7 m3/h | Equivalent a environ 60 L/min ; valeur de bout de courbe, sans pression utile elevee |
| Hauteur maximale indiquee | 40 a 43 m | Valeur a debit nul ou tres faible selon la courbe |
| Hauteur minimale projet | 2 m | Donnee d'entree projet |
| Diametre aspiration | 1 pouce | Donnee fiche technique |
| Diametre refoulement | 1 pouce | Donnee fiche technique |
| Corps de pompe | AISI 304 | Donnee fiche technique |

Le debit maximal et la hauteur maximale ne sont pas atteignables simultanement. Le point de fonctionnement reel dependra des buses, de la rampe 32 mm, des pertes de charge, de l'aspiration et de la hauteur geometrique.

## Courbe debit / hauteur disponible

| Debit | Debit | Hauteur manometrique |
| ---: | ---: | ---: |
| 0 m3/h | 0 L/min | 43 m |
| 0,6 m3/h | 10 L/min | 39 m |
| 1,2 m3/h | 20 L/min | 34 m |
| 1,8 m3/h | 30 L/min | 28 m |
| 2,4 m3/h | 40 L/min | 21 m |
| 3,0 m3/h | 50 L/min | 12,5 m |
| 3,6 m3/h | 60 L/min | 3 m |

Cette courbe indique qu'une estimation de consommation basee sur 60 L/min serait pessimiste pour l'eau consommee si les buses demandent une pression significative, mais optimiste pour la pression disponible. A titre indicatif, la pompe fournit environ 40 L/min autour de 21 m de hauteur, soit environ 2,1 bar avant pertes locales.

## Limites d'utilisation

| Limite | Valeur | Commentaire |
| --- | --- | --- |
| Temperature maximale liquide projet | 35 degC | Donnee d'entree projet |
| Temperature maximale liquide fiche | 40 degC | Divergence avec la donnee projet ; retenir 35 degC par prudence tant que la plaque exacte n'est pas arbitree |
| Temperature ambiante maximale fiche | 40 degC | Donnee fiche technique |
| Indice de protection | IPX4 | Protection contre projections, pas immersion ni lavage haute pression |
| Installation | Pompe de surface hors eau | A proteger de la pluie directe, des projections fortes et du gel |

## Consequences de conception

- La pompe de rincage est une charge secteur 230 VAC a commander par relais ou contacteur adapte aux moteurs.
- La pompe doit etre reliee a la terre et protegee par les protections secteur adequates du coffret.
- La commande de rincage doit rester inhibee en niveau critique, maintenance incompatible, capot ouvert si la strategie retenue l'exige, et defaut critique.
- La pompe ne doit pas etre consideree comme auto-diagnostiquee sans pressostat, debitmetre, mesure courant ou retour de marche.
- La consommation d'eau de rincage doit rester etiquetee comme estimation empirique tant que le debit reel aux buses n'est pas mesure.

## Conclusion de dimensionnement

La pompe VEVOR / Leo EKJ-802S est suffisamment caracterisee pour dimensionner l'architecture de commande secteur et lancer les essais de rincage. Elle ne permet pas encore de figer le debit reel aux buses sans essai hydraulique, car le point de fonctionnement dependra fortement de la rampe, des buses et des pertes de charge.

## Points a verifier sur installation reelle

- mesurer le debit reel evacue pendant un cycle de rincage ;
- verifier la pression ou la qualite de jet aux buses ;
- verifier l'amorcage et le reamorcage apres arret ;
- mesurer ou confirmer le courant nominal en fonctionnement ;
- verifier l'absence d'echauffement anormal sur cycles repetes ;
- ajuster la duree de lavage et l'estimation de consommation d'eau ;
- confirmer la temperature maximale admissible a retenir pour l'exploitation.
