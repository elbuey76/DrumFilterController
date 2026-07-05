# ADR-0003 - Choix de la plateforme de contrôle V1

## Statut

Acceptee

## Contexte

Le contrôleur V1 doit piloter un filtre à tambour dans un environnement humide, avec capteurs NPN 12-24 VDC, commandes de relais ou contacteurs, pompe 230 VAC, moteur tambour 12 V DC et IHM locale.

La commande matérielle visée n'est pas une commande d'essais séparée : elle constitue la commande définitive du MVP. La plateforme matérielle du MVP doit donc rester la base de la V2, sans remplacement de la plateforme principale.

Le choix de plateforme a ete tranche après gel du noyau E/S V1. Les critères retenus etaient :

- coût matériel ;
- effort de développement firmware ;
- robustesse électrique ;
- facilite de câblage en coffret ;
- maintenabilité et dépannage ;
- compatibilité avec capteurs CR18-8DN NPN 12-24 VDC ;
- disponibilité des entrées/sorties nécessaires ;
- intégration de l'écran local et des boutons ;
- capacité d'heure fiable en V2 sans remplacement de la plateforme principale, par RTC, temps local conserve, module temps, synchronisation réseau ou equivalent, sans dépendance exclusive à Internet ;
- compatibilité avec une connectivité Wi-Fi V2 sans remplacement de la plateforme principale, par Wi-Fi natif ou module Wi-Fi ajoutable proprement.

Le choix doit rester compatible avec l'architecture electrique V1 documentee dans [ADR-0004 - Architecture electrique V1](ADR-0004-architecture-electrique-v1.md).

## Options candidates

| Option | Avantages attendus | Points d'attention |
| --- | --- | --- |
| Automate compact ou module industriel DIN | Robuste, adapté coffret, E/S industrielles, maintenance plus lisible | Coût potentiellement supérieur, ergonomie de développement à comparer |
| ESP32 ou Arduino avec cartes d'interface | Coût bas, développement souple, écosystème large | Interface 12-24 V et relais a sécuriser, robustesse en environnement moteur/humide a justifier |
| Arduino industriel ou automate programmable leger | Compromis possible entre coût, E/S et développement | Offre exacte à comparer, disponibilité et documentation à vérifier |

## Decision

La plateforme V1 retenue est un KC868-A32. Elle est aussi la plateforme matérielle définitive du MVP et doit rester la base matérielle de la V2.

La tension de commande V1 est fixee a 12 VDC. L'alimentation basse tension retenue est une Mean Well NDR-120-12, 12 VDC, 120 W, 10 A, distribuee par porte-fusibles ATO.

L'horodatage fiable n'est pas une fonction obligatoire du MVP, mais le choix V1 ne doit pas empecher son ajout en V2. Une plateforme candidate doit donc documenter une voie réaliste vers une heure fiable sans remplacement matériel principal. Cette voie ne doit pas reposer uniquement sur Internet : une RTC, un temps local conserve, un module temps ou une capacité equivalente doit rester possible, avec synchronisation réseau seulement comme amelioration si elle est disponible.

La voie d'implementation retenue pour cette heure fiable est documentee par [ADR-0005 - Choix de la RTC pour l'heure fiable](ADR-0005-choix-rtc-heure-fiable.md) : module RTC DS3231 I2C 3,3 V avec batterie rechargeable, synchronisation reseau eventuelle seulement en complement.

La connectivité active n'est pas une fonction obligatoire du MVP, mais le matériel commandé pour le MVP doit être prêt pour une V2 Wi-Fi. Ethernet n'est pas disponible sur site, BLE seul n'a pas la portée nécessaire et SMS n'est pas retenu par défaut pour des raisons de coût. Une plateforme sans Wi-Fi natif doit donc permettre l'ajout propre d'un module Wi-Fi sans remplacement de la plateforme principale.

## Consequences

- Le schema de cablage doit partir du KC868-A32 et de ses relais de sortie.
- Les choix matériels MVP doivent être considérés comme définitifs pour la base V1/V2 ; les validations restantes servent à figer le câblage, les protections, les réglages et l'implantation, pas à commander une plateforme d'essai différente.
- Les sorties de puissance ne sont pas raccordees directement aux charges secteur : elles pilotent des contacteurs ou relais adaptes.
- Les entrées capteurs, boutons, écran et voyants doivent être vérifies contre les interfaces exactes du KC868-A32 avant cablage final.
- L'ecran local V1 retenu est un LCD 2004 / 20x4 I2C 3,3 V, raccorde de preference sur le port d'extension du KC868-A32 avec `GPIO32` / `GPIO33` en I2C logiciel, afin de ne pas ajouter de peripherique sur les bus I2C internes des relais et entrees.
- L'heure fiable V2 est couverte par une RTC DS3231 I2C 3,3 V avec batterie rechargeable ; le schema doit prevoir son raccordement, de preference sur le bus I2C logiciel du port d'extension si la validation banc confirme la cohabitation avec l'ecran.
- La question n'est plus de choisir entre automate compact, module industriel DIN et carte microcontrôleur, mais de documenter proprement les interfaces du KC868-A32.
