# Commandes du 2026-07-06

## Objet

Ce document trace les achats deja passes pour le controleur FAT V1, a partir des captures Amazon et AliExpress du 2026-07-06, ainsi que le materiel deja disponible hors commande.

Il sert de photo d'inventaire avant reception, controle des references et preparation du commit de documentation. Les prix et delais ci-dessous sont des informations de commande, pas des criteres techniques de validation.

## Decision posterieure concernant la plateforme

L'achat de la KC868-A32 reste trace ci-dessous comme fait historique. L'[ADR-0012](../decisions/ADR-0012-migration-kc868-a32-vers-a16.md) abandonne ensuite l'A32 comme plateforme cible du FAT au profit d'une KC868-A16 ESP32 classique, plus compacte et suffisante pour les neuf entrees et neuf sorties V1.

L'A32 ne doit donc pas etre integree au schema cible. Sa destination, retour fournisseur, revente, conservation comme banc d'essai ou reutilisation, reste a decider. La migration ajoute au panier un relais d'interface a bobine 12 VDC et contact 230 VAC pour commander la bobine du Schneider LC1D18P7.

La plateforme cible est desormais disponible : la carte recue le 2026-07-22 est une **KC868-A16 REV.1.6.3**. Son premier demarrage avec le firmware desarme a confirme le bus I2C interne et quatre peripheriques (`0x21`, `0x22`, `0x24`, `0x25`), sans encore valider les niveaux electriques. La fiche de reception [VR-0001](../validation/VR-0001-reception-kc868-a16-rev1.6.3.md) fait foi pour son identification et les essais restants.

## Materiel deja disponible

| Article | Etat | Commentaire |
| --- | --- | --- |
| Pompe de rincage VEVOR / Leo EKJ-802S | Deja disponible | Reference conservee pour la V1 ; essais hydrauliques a realiser sur la rampe et les buses reelles. |
| Sectionneur local | Deja disponible | A inspecter avant montage : reference exacte, etat mecanique, calibre, nombre de poles, verrouillage et compatibilite coffret. |

## Commandes Amazon

| Fournisseur | Article | Reference / caracteristiques notees | Quantite | Total commande | Livraison annoncee | Reference commande |
| --- | --- | --- | ---: | ---: | --- | --- |
| Amazon | Alimentation rail DIN | Mean Well NDR-120-12, 120 W, 12 VDC, 10 A | 1 | 37,79 EUR | 2026-07-15 au 2026-07-17 | 402-9583482-0403529 |
| Amazon | Contacteur pompe de rincage | Schneider Electric TeSys LC1D18P7, 3P, AC-3 18 A, bobine 230 VAC | 1 | 36,99 EUR | 2026-07-15 au 2026-07-17 | 402-6510415-8758724 |
| Amazon | Motorreducteur tambour | Moteur DC 12 V reversible, reducteur a vis sans fin, variante 10 rpm | 1 | 41,68 EUR avec relais HELLA | 2026-07-10 annonce comme vendredi | 402-6370014-3744350 |
| Amazon | Relais moteur tambour | HELLA 4RD 933 332-551, relais inverseur 12 V, 5 poles | 1 | inclus commande ci-dessus | 2026-07-10 annonce comme vendredi | 402-6370014-3744350 |

## Commandes AliExpress

| Fournisseur / boutique | Article | Reference / caracteristiques notees | Quantite commandee | Total commande | Etat capture | Reference commande |
| --- | --- | --- | ---: | ---: | --- | --- |
| Boutique electric Store | Boutons poussoirs 22 mm | Lot visible : jaune, noir, bleu ; variantes type XB2-10BN | 3 | 22,55 EUR | En attente de livraison | 3074437520116235 |
| ZiJun Automatic Store | Carte de controle | Kincony KC868-A32, ESP32, variante A32G | 1 | 121,99 EUR | En attente de livraison | 3074437520276235 |
| Boutique officielle DIANQI | Voyants LED 16 mm 12 V | Jaune, rouge, vert | 3 | 15,26 EUR | Partiellement expediee | 3074437520296235 |
| ZHRIVOER Electrical Store | Selecteur 22 mm | XB2-BD25, 2 positions maintenues, 1NO1NC | 1 | 6,09 EUR | En attente de livraison | 3074437520156235 |
| StarLanguage Store | Porte-fusibles ATO | Boite fusibles lame ATO, variante 6 voies | 1 | 7,49 EUR | En attente de livraison | 3074437520176235 |
| Shop1104001366 Store | Fins de course | Mini interrupteurs de fin de course a levier/rouleau, variante ME8104 | A verifier a reception | 9,16 EUR | Traitement | 3074437520336235 |
| Aitver Electronics Store | Horloge temps reel | Module RTC DS3231, 3,3 V / 5 V, avec batterie | 1 | 8,98 EUR | Traitement | 3074437520196235 |
| Ring Star Electronics Store | Ecran local | LCD 2004 / 20x4, 3,3 V, bleu, I2C | 1 | 13,17 EUR | Traitement | 3074437520356235 |
| TOMZN Official Store | Contacteurs auxiliaires | TOMZN TOCT1, 2P, 25 A, bobine DC 12 V, 1NO 1NC | 4 | 44,72 EUR | En attente de livraison | 3074437520216235 |
| Boutique officielle XSxMALL | Fusibles ATO / ATC | Lots de fusibles auto lame, plusieurs calibres visibles | A verifier a reception | 12,27 EUR | Partiellement expediee | 3074437520236235 |

## Points de controle a reception

| Article | Controle a faire avant montage |
| --- | --- |
| Mean Well NDR-120-12 | Verifier entree 230 VAC, sortie 12 VDC, reglage tension, bornier, fixation DIN et absence de choc transport. |
| KC868-A32 | Verifier la reference et l'etat uniquement pour decider retour, revente, banc d'essai ou reutilisation ; ne pas la monter comme controleur FAT cible. |
| Schneider LC1D18P7 | Confirmer bobine 230 VAC, reperage A1/A2, puissance contacts et encombrement ; choisir le relais d'interface 12 VDC / 230 VAC compatible avec sa charge de bobine. |
| TOMZN TOCT1 | Confirmer bobine 12 VDC, type de contact 1NO1NC, polarite eventuelle, consommation bobine et echauffement. |
| Motorreducteur 12 V 10 rpm | Mesurer courant a vide, courant en charge, courant au demarrage, courant en blocage controle, sens de rotation et couple utile. |
| Relais HELLA | Confirmer brochage, type de contact, diode/resistance eventuelle, fixation et besoin de support rail DIN imprime. |
| Porte-fusibles et fusibles | Confirmer nombre de voies, calibres reels recus, tenue mecanique, possibilite de montage propre en coffret. |
| Boutons, selecteur et voyants | Controler couleurs, contacts NO/NC, profondeur de facade, joints, tension LED 12 V et logique des contacts. |
| Fins de course ME8104 | Identifier bornes NO/NC au multimetre et choisir le cablage donnant capot ferme = boucle fermee. |
| DS3231 et LCD 2004 | Verifier alimentation 3,3 V, adresses I2C, pull-up I2C, cohabitation sur le bus I2C auxiliaire separe `GPIO32` / `GPIO33`. |

## Ecarts et achats restant probables

- Les boutons commandes semblent couvrir bleu, jaune et un noir. La liste V1 prevoit deux boutons noirs (`MANU_TAMBOUR` et `MANU_RINCAGE`) ; verifier a reception si un second noir est inclus ou a commander.
- Le porte-fusibles commande est une variante 6 voies, alors que l'architecture retient 4 departs 12 VDC. Les deux voies supplementaires peuvent rester en reserve si le montage mecanique est propre.
- Les quantites exactes de fins de course et de fusibles doivent etre confirmees a reception, car les captures ne suffisent pas a figer le stock exploitable.
- Restent a choisir ou confirmer : coffret, differentiel, disjoncteurs, borniers, goulottes, presse-etoupes, fils, embouts, reperage, supports rail DIN imprimes et elements mecaniques/hydrauliques.
