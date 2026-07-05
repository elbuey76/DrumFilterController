# NC-0003 - Dimensionnement de la pompe de rinçage

## Objet

Documenter les caractéristiques connues de la pompe de rinçage du FAT, les limites d'interprétation de sa courbe hydraulique et les validations nécessaires pour le point de fonctionnement réel sur la rampe et les buses déjà achetées et fabriquées.

## Données d'identification

| Donnée | Valeur | Source / commentaire |
| --- | --- | --- |
| Marque commerciale | VEVOR | Pompe deja retenue pour la rampe et les buses du FAT |
| Fabricant OEM | Leo Group Pump (Zhejiang) Co., Ltd. | Information fournisseur |
| Modèle | EKJ-802S | Garden Jet Pump |
| Type | Pompe centrifuge de surface auto-amorcante | Pompe de jardin, non immergée |
| Numéro de serie | 221228693910166 | Donnée plaque |
| Code observé | 10EKJ-802S | Donnée fiche technique |

## Données électriques

| Donnée | Valeur | Commentaire |
| --- | --- | --- |
| Alimentation | 220-240 VAC | Monophase |
| Fréquence | 50 Hz | Standard secteur local |
| Puissance indiquee projet | 800 W | Puissance électrique absorbee fournie dans les données d'entrée |
| Puissance fiche Leo | 0,6 kW / 0,8 HP | Donnée fiche technique EKJ-802S |
| Courant fiche Leo | 3,5 A | Cohérent avec une pompe de l'ordre de 0,6 à 0,8 kW sous 230 V |
| Classe de protection électrique | Classe I | Mise à la terre obligatoire |
| Classe d'isolation | Classe F | Donnée fiche technique |
| Marquage CE | Oui | Donnée plaque / fournisseur |

Pour le dimensionnement des relais ou contacteurs, la pompe doit être traitée comme une charge moteur 230 VAC. Le courant nominal ne suffit pas à lui seul : le courant d'appel au démarrage doit être pris en compte dans le choix de l'organe de coupure.

## Données hydrauliques

| Donnée | Valeur | Commentaire |
| --- | --- | --- |
| Débit maximal indique | 3,6 à 3,7 m3/h | Equivalent a environ 60 L/min ; valeur de bout de courbe, sans pression utile élevée |
| Hauteur maximale indiquee | 40 à 43 m | Valeur a débit nul ou très faible selon la courbe |
| Hauteur minimale projet | 2 m | Donnée d'entrée projet |
| Diamètre aspiration | 1 pouce | Donnée fiche technique |
| Diamètre refoulement | 1 pouce | Donnée fiche technique |
| Corps de pompe | AISI 304 | Donnée fiche technique |

Le débit maximal et la hauteur maximale ne sont pas atteignables simultanément. Le point de fonctionnement réel dépendra des buses déjà fabriquées, de la rampe 32 mm, des pertes de charge, de l'aspiration et de la hauteur géométrique.

## Courbe débit / hauteur disponible

| Débit | Débit | Hauteur manometrique |
| ---: | ---: | ---: |
| 0 m3/h | 0 L/min | 43 m |
| 0,6 m3/h | 10 L/min | 39 m |
| 1,2 m3/h | 20 L/min | 34 m |
| 1,8 m3/h | 30 L/min | 28 m |
| 2,4 m3/h | 40 L/min | 21 m |
| 3,0 m3/h | 50 L/min | 12,5 m |
| 3,6 m3/h | 60 L/min | 3 m |

Cette courbe indique qu'une estimation de consommation basée sur 60 L/min serait pessimiste pour l'eau consommée si les buses demandent une pression significative, mais optimiste pour la pression disponible. À titre indicatif, la pompe fournit environ 40 L/min autour de 21 m de hauteur, soit environ 2,1 bar avant pertes locales.

## Limites d'utilisation

| Limité | Valeur | Commentaire |
| --- | --- | --- |
| Température maximale liquide projet | 35 degC | Donnée d'entrée projet |
| Température maximale liquide fiche | 40 degC | Divergence avec la donnée projet ; retenir 35 degC par prudence tant que la plaque exacte n'est pas arbitree |
| Température ambiante maximale fiche | 40 degC | Donnée fiche technique |
| Indice de protection | IPX4 | Protection contre projections, pas immersion ni lavage haute pression |
| Installation | Pompe de surface hors eau | A protéger de la pluie directe, des projections fortes et du gel |

## Conséquences de conception

- La pompe de rinçage est une charge secteur 230 VAC à commander par relais ou contacteur adapté aux moteurs.
- La pompe doit être reliee à la terre et protégée par les protections secteur adéquates du coffret.
- La commande de rinçage doit rester inhibée en niveau critique, maintenance incompatible, capot ouvert si la stratégie retenue l'exige, et défaut critique.
- La pompe ne doit pas être considérée comme auto-diagnostiquee sans pressostat, débitmètre, mesure courant ou retour de marche.
- La consommation d'eau de rinçage doit rester étiquetée comme estimation empirique tant que le débit réel sur la rampe et les buses n'est pas mesure.

## Conclusion de dimensionnement

La pompe VEVOR / Leo EKJ-802S est suffisamment caractérisée pour dimensionner l'architecture de commande secteur définitive du MVP. Elle ne permet pas encore de figer le débit réel sur la rampe et les buses sans mesure hydraulique, car le point de fonctionnement dépendra fortement de la rampe déjà fabriquée, des buses et des pertes de charge.

## Points à vérifier sur installation réelle

- mesurer le débit réel evacue pendant un cycle de rinçage ;
- vérifier la pression ou la qualité de jet aux buses ;
- vérifier l'amorçage et le reamorçage après arrêt ;
- mesurer ou confirmer le courant nominal en fonctionnement ;
- vérifier l'absence d'échauffement anormal sur cycles répétés ;
- ajuster la durée de lavage et l'estimation de consommation d'eau ;
- confirmer la température maximale admissible à retenir pour l'exploitation.
