# NC-0005 - Dimensionnement du coffret electrique

## Objet

Cette note dimensionne le coffret electrique du controleur FAT V1 a partir des
equipements actuellement retenus. Elle fixe :

- la matiere du coffret pour ne pas degrader le Wi-Fi de la KC868-A16 ;
- le nombre d'emplacements equivalents sur rail DIN ;
- une implantation de principe separant le 230 VAC et la basse tension ;
- une dimension minimale et une dimension recommandee avant achat.

Un emplacement modulaire est compte sur une largeur de **18 mm**. Les appareils
industriels qui ne suivent pas exactement ce pas sont arrondis a l'emplacement
superieur. Ce bilan sert au choix du coffret, mais ne remplace pas le schema
electrique, la verification des distances constructeur ni la revue 230 VAC par
une personne competente.

## Contraintes d'entree

- Coffret installe dans un local technique humide, avec condensation et
  projections possibles : **IP65 cible**.
- Enveloppe **imperativement non metallique** pour le Wi-Fi : polyester renforce,
  polycarbonate ou ABS industriel adapte a l'environnement.
- KC868-A16 REV.1.6.3 : boitier annonce a `186 x 106 mm`, soit 10,33 modules de
  largeur, comptes **11 emplacements**.
- Porte-fusibles ATO sur support imprime 3D : **3 emplacements**, valeur fournie
  par la CAO.
- Relais HELLA sur support imprime 3D : **2 emplacements**, valeur fournie par la
  CAO.
- Alimentation Mean Well NDR-120-12 : largeur 40 mm, soit 2,22 modules, comptes
  **3 emplacements**. Le constructeur recommande aussi 40 mm libres au-dessus,
  20 mm au-dessous, 5 mm sur les cotes, ou 15 mm vis-a-vis d'une source chaude.
- Contacteur Schneider LC1D18P7 : largeur 45 mm, soit 2,5 modules, comptes
  **3 emplacements**.
- Le sectionneur VCDN20 sera monte en facade du coffret. Il possede une face
  avant de 60 x 60 mm et un corps situe derriere la porte. Il occupe donc
  **0 emplacement DIN**, mais son volume arriere et son alignement doivent etre
  reserves dans l'implantation de la porte.
- Les quatre contacteurs TOMZN TOCT1-25Z sont comptes a 18/19 mm chacun, soit
  **1 emplacement par contacteur**.
- Le relais d'interface 12 VDC / 230 VAC n'est pas encore choisi. Une largeur de
  **1 emplacement** est reservee ; cette hypothese devra etre controlee sur la
  reference finale.
- Les disjoncteurs divisionnaires sont comptes comme des appareils 1P+N de
  **1 emplacement**. Si la gamme finalement achetee utilise 2 emplacements par
  depart, il faudra ajouter 6 emplacements au bilan.

## Bilan des appareils sur rail DIN

| Zone | Equipement | Quantite | Emplacements unitaires | Total |
| --- | --- | ---: | ---: | ---: |
| Facade | Sectionneur TeSys VCDN20 | 1 | 0 | 0 |
| Tete 230 VAC | Interrupteur differentiel 2P 30 mA type A | 1 | 2 | 2 |
| Protections | Disjoncteurs des 6 departs locaux | 6 | 1 | 6 |
| Puissance | Contacteur Schneider LC1D18P7 | 1 | 3 | 3 |
| Puissance | Contacteurs TOMZN TOCT1-25Z | 4 | 1 | 4 |
| Interface | Relais d'interface rincage 12 VDC / 230 VAC | 1 | 1 | 1 |
| Alimentation | Mean Well NDR-120-12 | 1 | 3 | 3 |
| Distribution 12 VDC | Porte-fusibles ATO sur support imprime | 1 | 3 | 3 |
| Tambour | Relais HELLA sur support imprime | 1 | 2 | 2 |
| Controle | KC868-A16 dans son boitier DIN | 1 | 11 | 11 |
|  | **Sous-total appareils sur rail DIN** |  |  | **35** |

Le sous-total fonctionnel est donc de **35 emplacements equivalents**, soit
environ 630 mm de longueur de rail cumulee. Cette valeur ne doit pas etre
interpretee comme une seule rangee : les zones secteur, puissance, basse tension
et radio doivent etre reparties.

## Borniers et accessoires de rail

Un bornier est compte pour chaque conducteur qui traverse la limite du coffret.
Les organes de facade et le cablage interne ne sont pas comptes ici. Les huit
prises physiques correspondent a sept cables sortants, car les deux bulleurs
partagent un meme cable et les deux prises maintenance un autre.

| Famille | Detail | Gris 230 V | Bleu neutre | Vert/jaune | Blanc TBT | Total |
| --- | --- | ---: | ---: | ---: | ---: | ---: |
| Arrivee maison | 1 cable `3G2,5 mm2` | 1 | 1 | 1 | 0 | 3 |
| Puissance sortante | 7 cables : 4 prises commandées, bulleurs, maintenance, eclairage | 7 | 7 | 7 | 0 | 21 |
| Moteur tambour | 1 cable 12 VDC, positif et 0 V | 0 | 0 | 0 | 2 | 2 |
| Niveaux | 2 capteurs CR18-8DN, 3 fils chacun | 0 | 0 | 0 | 6 | 6 |
| Capot | 1 contact sec, 2 fils | 0 | 0 | 0 | 2 | 2 |
| Temperatures | 2 sondes DS18B20, 3 fils chacune | 0 | 0 | 0 | 6 | 6 |
| **Besoin V1 monte** |  | **8** | **8** | **8** | **16** | **40** |

Avec une largeur type de 5,2 mm, les 40 borniers occupent `208 mm`. En ajoutant
environ 25 mm pour les butees de fin, separateurs et pontages, la zone V1 occupe
environ `233 mm`, soit **13 emplacements DIN** de 18 mm.

La liste de course retient exactement **8 gris 230 V, 8 bleus, 8 vert/jaune et
16 blancs TBT**. Le bleu reste reserve au neutre 230 V ; le blanc repere tous
les raccordements de tres basse tension, sans confusion avec le secteur. Les
eventuelles pieces de rechange restent a conserver hors rail dans une boite de
maintenance.

## Total et reserve

| Poste | Emplacements equivalents |
| --- | ---: |
| Appareils V1 sur rail DIN | 35 |
| Borniers V1 et accessoires | 13 |
| Sous-total implante | **48** |
| Reserve minimale d'evolution, environ 20 % | **10** |
| **Capacite cible minimale a acheter** | **58** |

La dimension du coffret reste principalement imposee par les goulottes, la zone
radio, les rayons de courbure, les appareils non modulaires et les organes de
facade. Le projet conserve donc un coffret industriel a platine plutot qu'un
simple tableau modulaire domestique, meme si le seul total DIN est plus faible.

## Bilan des liaisons hors coffret et de facade

Les equipements livres avec une fiche europeenne ne sont pas recables : ils se
branchent sur des prises 16 A 2P+T placees dans le local. Les prises commandées
sont alimentees par les contacteurs deja comptes sur rail DIN ; elles n'ajoutent
donc aucun module DIN.

### Prises 16 A du local : 8 prises physiques

| Groupe | Prises | Commande |
| --- | ---: | --- |
| Pompe de rincage | 1 | Contacteur Schneider, commande automate |
| Pompe de filtration | 1 | Contacteur TOMZN, commande automate |
| Pompe decoration | 1 | Contacteur TOMZN, commande automate |
| UV | 1 | Contacteur TOMZN, commande automate |
| Bulleur bassin | 1 | Permanente, hors controleur |
| Bulleur bio | 1 | Permanente, hors controleur |
| Maintenance | 2 | Permanentes, sur le meme depart 16 A |
| **Total** | **8** | 4 commandées + 4 permanentes |

La mise a niveau automatique n'est pas incluse dans ce compte : son appareil
final n'est pas encore choisi. Une prise commandée ou un cable direct sera
ajoute a ce moment-la.

### Presse-etoupes

Le plan de cablage retient **13 cables sortants actifs**, repartis ainsi :

| Famille | Cables sortants | Nombre |
| --- | --- | ---: |
| Prises commandées | Rincage, filtration, decoration, UV | 4 |
| Prises permanentes | Groupe bulleurs, groupe maintenance | 2 |
| Eclairage exterieur | Depart eclairage | 1 |
| Moteur tambour | Câble 12 VDC moteur | 1 |
| Signaux terrain | 2 niveaux, capot, sonde bassin, sonde local | 5 |
| **Total sortant actif** |  | **13** |

Ajouter un presse-etoupe pour le cable d'arrivee `3G2,5 mm2` venant du tableau
maison et un presse-etoupe de reserve pour la future mise a niveau ou une
evolution. Le panier doit donc contenir **15 presse-etoupes au total** : 1 M25
pour l'arrivee, 8 M20 pour moteur et puissance sortante, 5 M16 pour les signaux,
et 1 M20 de reserve. Les diametres reels des cables doivent etre mesures avant
commande pour verifier ces tailles.

### Liens fonctionnels vers la facade

La facade comporte **10 fonctions** : le sectionneur, le LCD, le selecteur,
quatre boutons et trois voyants. Le sectionneur est relie mecaniquement a son
corps interne ; il ne consomme pas de conducteurs de commande de facade.

Le cablage de la porte peut etre realise avec un cable souple de commande
**16 conducteurs de 0,25 ou 0,34 mm2**. Treize conducteurs sont utiles : 3 pour
le LCD (`3,3 V`, `SDA`, `SCL`), 6 pour le selecteur et les quatre boutons, 3
pour les voyants et un seul `0 V` commun. Les trois ames restantes sont des
reserves. Le `0 V` commun est admis pour le LCD, les contacts secs et les
voyants de faible courant ; aucun courant moteur ni bobine de puissance ne doit
revenir par ce faisceau de porte.

### Liens fonctionnels sortants hors prises

Hors prises, il y a **7 liaisons fonctionnelles actives** : moteur tambour,
deux capteurs de niveau, contact capot, sonde bassin, sonde local et eclairage
exterieur. La mise a niveau ajoute une huitieme liaison lorsqu'elle sera definie.
Le cable d'arrivee maison n'est pas compte parmi ces sorties.

## Implantation de principe

L'implantation cible utilise **quatre rails DIN de 450 mm utiles minimum**.
Chaque rail offre environ 25 emplacements de 18 mm, soit environ 100 emplacements
cumules avant prise en compte des butees.

| Rail | Zone proposee | Charge indicative |
| --- | --- | ---: |
| Rail 1, haut | Interrupteur differentiel et 6 disjoncteurs | 8 modules |
| Rail 2 | LC1D18, 4 TOMZN et relais d'interface | 8 modules |
| Rail 3 | Mean Well, porte-fusibles, HELLA et KC868-A16 | 19 modules |
| Rail 4, bas | Borniers terrain, PE, neutre et passages BT | 13 modules |

La repartition exacte du rail 3 peut etre scindee si la chaleur de l'alimentation
ou les faisceaux approchent l'antenne. La KC868-A16 doit etre placee a une
extremite de rail, antenne orientee vers une paroi plastique exterieure et loin
des contacteurs, du rail inutilise, des goulottes chargees et des conducteurs
230 VAC.

Prevoir au minimum :

- une goulotte verticale secteur et une goulotte verticale basse tension
  distinctes ;
- des goulottes horizontales laissant les distances de ventilation de
  l'alimentation ;
- une zone sans metal ni faisceau d'au moins **15 mm dans toutes les directions
  autour de l'antenne ESP32** ;
- la place arriere necessaire au VCDN20 aligne sur la poignee de porte ;
- l'acces aux cosses du relais HELLA et aux fusibles ATO sans depose d'un autre
  appareil.

## Choix du coffret

### Minimum technique

Un coffret plastique IP65 d'environ **600 x 600 x 300 mm** peut etre envisage
uniquement apres une implantation CAO ou un montage a blanc complet. Il sera
serre en hauteur pour quatre rails, les goulottes, le VCDN20 de porte et la
reserve.

### Recommandation d'achat

La cible recommandee est :

- **coffret mural plastique IP65, 800 x 600 x 300 mm environ** ;
- polyester renforce ou polycarbonate industriel, resistant aux UV si le local
  peut recevoir du soleil ;
- porte pleine usinable pour le VCDN20, le LCD, le selecteur, les quatre boutons
  et les trois voyants ;
- platine de fond **isolante de preference** ou, si elle est metallique, avec
  l'antenne de l'A16 placee au-dela de son bord ou suffisamment eloignee ;
- quatre rails DIN de 450 a 500 mm utiles ;
- goulottes et entree de cables par le bas ;
- event respirant adapte a l'IP ou autre traitement documente de la condensation.

Une reference coherente a mettre en tete de comparaison est le **Schneider
PanelSeT PLM NSYPLM86BG** : enveloppe polyester, porte pleine, IP66/IK10,
dimensions exterieures `847 x 636 x 300 mm`, profondeur utile annoncee 275 mm
et platine bakelite isolante incluse. La disponibilite et le prix en France
doivent etre verifies avant commande. La variante `NSYPLM86PG` avec platine
metallique n'est pas le premier choix pour le Wi-Fi.

La profondeur de **300 mm** est retenue pour conserver de la marge derriere les
organes de porte et devant les appareils profonds. Le VCDN20 est annonce avec
une profondeur totale pouvant atteindre environ 123,5 mm selon son montage, le
LC1D18P7 fait 86 mm de profondeur et le NDR-120-12 fait 113,5 mm.

## Conditions Wi-Fi

Le plastique du coffret est necessaire mais ne suffit pas. La KC868-A16 classique
utilise un ESP32-WROOM-32 a antenne PCB integree. Les recommandations Espressif
demandent une zone libre d'au moins 15 mm autour de l'antenne et une validation
de la portee sur le produit final.

Avant de percer le coffret :

1. identifier physiquement l'extremite antenne du module ESP32 dans la KC868-A16 ;
2. orienter cette extremite vers l'exterieur du coffret, idealement en haut ou sur
   un cote ;
3. ne pas placer de platine metallique, rail, goulotte, contacteur ou faisceau
   dans la zone de 15 mm autour de l'antenne ;
4. fermer la porte et tester le RSSI, la stabilite et le debit Wi-Fi a
   l'emplacement definitif ;
5. conserver Ethernet comme solution de repli si la couverture du local est
   insuffisante.

## Points bloquants avant commande definitive

- Mesurer la longueur utile reelle des rails du coffret choisi, pas seulement
  ses dimensions exterieures.
- Confirmer que la platine, les quatre rails et les goulottes tiennent dans la
  reference exacte.
- Mesurer la KC868-A16 recue avec ses clips, connecteurs et rayons de courbure.
- Confirmer la largeur du relais d'interface de rincage.
- Confirmer si les disjoncteurs choisis sont bien des 1P+N d'un module.
- Faire un gabarit 1:1 ou une implantation CAO de la porte pour verifier les
  interferences entre VCDN20, LCD, boutons, voyants et appareils interieurs.
- Faire relire le schema et l'implantation 230 VAC avant mise sous tension.

## Sources dimensionnelles

- [Kincony - dimensions KC868-A16, 186 x 106 mm](https://www.kincony.com/forum/showthread.php?pid=17869&tid=1685)
- [Mean Well - fiche NDR-120, dimensions et distances d'installation](https://www.meanwell.com/Upload/PDF/NDR-120/NDR-120-SPEC-CN.PDF)
- [Schneider Electric - fiche LC1D18P7, 45 x 77 x 86 mm](https://iportal.se.com/Contents/docs/SQD-LC1D18P7_DATASHEET.PDF)
- [Schneider Electric - VCDN20, montage rail et poignee de porte](https://www.se.com/de/de/product/VCDN20/notaus-hauptschalter-d225mm-3p-690v-20a-griff-rot-abschlie%C3%9Fbar/)
- [Schneider PanelSeT PLM NSYPLM86BG - coffret polyester avec platine bakelite](https://www.ilecsys.co.uk/manufacturers/schneider/panelset-plm/nsyplm86bg)
- [Espressif - implantation et zone libre de 15 mm autour de l'antenne](https://docs.espressif.com/projects/esp-hardware-design-guidelines/en/latest/esp32/pcb-layout-design.html)
