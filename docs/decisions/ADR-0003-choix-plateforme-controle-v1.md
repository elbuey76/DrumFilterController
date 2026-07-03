# ADR-0003 - Choix de la plateforme de controle V1

## Statut

En cours

## Contexte

Le controleur V1 doit piloter un filtre a tambour dans un environnement humide, avec capteurs NPN 12-24 VDC, commandes de relais ou contacteurs, pompe 230 VAC, moteur tambour 12 V DC et IHM locale.

Le choix de plateforme n'est pas encore tranche. Il doit etre compare selon :

- cout materiel ;
- effort de developpement firmware ;
- robustesse electrique ;
- facilite de cablage en coffret ;
- maintenabilite et depannage ;
- compatibilite avec capteurs CR18-8DN NPN 12-24 VDC ;
- disponibilite des entrees/sorties necessaires ;
- integration de l'ecran local et des boutons ;
- capacite d'heure fiable en V2 sans remplacement de la plateforme principale, par RTC, temps local conserve, module temps, synchronisation reseau ou equivalent, sans dependance exclusive a Internet.
- compatibilite avec une connectivite Wi-Fi V2 sans remplacement de la plateforme principale, par Wi-Fi natif ou module Wi-Fi ajoutable proprement.

La ponderation exacte des criteres reste a brainstormer avec l'utilisateur. Une matrice comparative chiffree sera preparee le moment venu, apres gel du nombre minimal d'entrees/sorties V1.

## Options candidates

| Option | Avantages attendus | Points d'attention |
| --- | --- | --- |
| Automate compact ou module industriel DIN | Robuste, adapte coffret, E/S industrielles, maintenance plus lisible | Cout potentiellement superieur, ergonomie de developpement a comparer |
| ESP32 ou Arduino avec cartes d'interface | Cout bas, developpement souple, ecosysteme large | Interface 12-24 V et relais a securiser, robustesse en environnement moteur/humide a justifier |
| Arduino industriel ou automate programmable leger | Compromis possible entre cout, E/S et developpement | Offre exacte a comparer, disponibilite et documentation a verifier |

## Decision attendue

Choisir la plateforme apres comparaison cout / effort / robustesse, avant schema de cablage definitif.

La tension de commande 12 V ou 24 V reste ouverte a ce stade. Le moteur tambour conserve une alimentation 12 V dediee si le moteur d'essuie-glace est retenu.

L'horodatage fiable n'est pas une fonction obligatoire du MVP, mais le choix V1 ne doit pas empecher son ajout en V2. Une plateforme candidate doit donc documenter une voie realiste vers une heure fiable sans remplacement materiel principal. Cette voie ne doit pas reposer uniquement sur Internet : une RTC, un temps local conserve, un module temps ou une capacite equivalente doit rester possible, avec synchronisation reseau seulement comme amelioration si elle est disponible.

La connectivite active n'est pas une fonction obligatoire du MVP, mais le materiel choisi doit etre pret pour une V2 Wi-Fi. Ethernet n'est pas disponible sur site, BLE seul n'a pas la portee necessaire et SMS n'est pas retenu par defaut pour des raisons de cout. Une plateforme sans Wi-Fi natif doit donc permettre l'ajout propre d'un module Wi-Fi sans remplacement de la plateforme principale.

La recherche de references materielles candidates commence apres gel des E/S V1 afin d'eviter de comparer des plateformes sur une base incomplete.
