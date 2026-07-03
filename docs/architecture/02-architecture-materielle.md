# Architecture materielle

## Blocs materiels

| Bloc | Role | Options envisagees |
| --- | --- | --- |
| Carte de controle | Execute la logique de lavage et securite | Choix a brainstormer : automate compact / module industriel DIN ou carte microcontroleur type ESP32 / Arduino industriel, selon cout, effort de developpement, robustesse, maintenabilite, capacite d'heure fiable V2 et compatibilite Wi-Fi V2 sans remplacement de plateforme |
| Entrees capteurs | Detectent niveau de lavage, niveau critique, capot, temperature bassin et temperature ambiante | Flotteurs, capteurs pression, inductifs, contacts secs, sondes de temperature |
| Sorties puissance | Pilotent pompe, moteur et prises auxiliaires | Relais, contacteurs, variateur, module relais opto-isole |
| Interface locale | Permet conduite, signalisation et diagnostic | Boutons, voyants, ecran simple |
| Communication distante | Option V2 pour supervision et notifications a distance | Wi-Fi cible ; BLE seul insuffisant, Ethernet non disponible sur site, SMS non retenu par defaut |
| Temps fiable | Support futur d'horodatage V2 | RTC, temps local conserve, module temps, synchronisation reseau ou capacite equivalente selon plateforme ; ne pas dependre exclusivement d'Internet |
| Alimentation | Fournit basse tension stable | Tension de commande ouverte a ce stade : 12 V ou 24 V, avec 12 V separe pour le moteur tambour si besoin |

## Composants materiels deja choisis

| Sous-ensemble | Choix retenu | Impact de conception |
| --- | --- | --- |
| Toile de filtration tambour | Inox 74 microns | Fixe la finesse de filtration mecanique de reference |
| Capteurs de niveau | CR18-8DN | Imposent une interface d'entree compatible NPN, 12-24 VDC, 3 fils |
| Moteur tambour candidat | Moteur d'essuie-glace avant SWF 403.835, 12 V DC | Impose une alimentation 12 V suffisamment dimensionnee, une commande petite vitesse et une protection surintensite/blocage |
| Pompe de rincage | VEVOR / Leo EKJ-802S, 220-240 VAC, 800 W indique projet | Impose une commande secteur adaptee a une charge moteur et une mesure du debit reel aux buses |

## Donnees hydrauliques d'entree

L'installation cible a controler comprend un FAT avec :

- une emprise interne totale de 78 cm x 47 cm ;
- un trop-plein physique fixe a 30,5 cm de hauteur d'eau ;
- un compartiment eau propre de 62 cm x 47 cm contenant un tambour de 31 cm de diametre sur 57 cm de longueur utile ;
- deux entrees de 110 mm : une bonde de fond et un skimmer ;
- deux sorties de 110 mm pour conserver le flux hydraulique ;
- une goutiere d'evacuation des dechets rinces vers un tuyau de 100 mm ;
- un report de niveau cote eau propre via un tube de 32 mm ;
- une rampe d'aspersion en 32 mm avec buses.

La rotation du tambour est envisagee avec un moteur d'essuie-glace avant SWF 403.835 de Peugeot 106 phase 2, utilise en petite vitesse, en 12 V DC et en fonctionnement intermittent. La transmission prevue est un pignon moteur de 10 cm vers un engrenage tambour de 30 cm, soit une reduction 3:1.

Le rincage est envisage avec une pompe de surface VEVOR / Leo EKJ-802S en 220-240 VAC. La courbe disponible indique environ 3,6 m3/h a tres faible hauteur utile et environ 2,4 m3/h a 21 m ; le debit effectif aux buses devra etre mesure sur la rampe reelle.

Le FAT sera installe dans un local de filtration maconne, isole en XPS 5 cm, sans pluie directe sur le FAT. Un capot transparent relevable est prevu au-dessus du petit batiment pour permettre de voir le tambour tourner sans ouvrir le FAT ; ce capot et le couvercle transparent designent la meme piece physique. Sa matiere et son niveau d'isolation restent a definir.

Ces donnees doivent etre prises en compte pour les choix de capteurs, l'implantation du niveau de lavage cote eau propre, l'ajout d'une mesure de temperature bassin, l'ajout d'une mesure de temperature ambiante local et les contraintes de debit autour du filtre.

La liste de signaux a prevoir pour le prototype est detaillee dans [04-table-entrees-sorties.md](04-table-entrees-sorties.md).

## Chaine hydraulique de reference

```mermaid
flowchart LR
    Bassin["Bassin 186 x 411 x 108 cm"]
    BF["Bonde de fond 110 mm"]
    SK["Skimmer 110 mm"]
    FAT["FAT gravitaire"]
    IBC["IBC 1000 L + tapis japonais"]
    PF["Pompe filtration"]
    Retour["Retour bassin 63 mm"]
    PD["Pompe decoration"]
    Deco["Cascade / mur d'eau / lame d'eau"]
    UV["UV hors tambour (apres pompe)"]
    Waste["Goutiere + evacuation 100 mm"]

    Bassin --> BF --> FAT
    Bassin --> SK --> FAT
    FAT --> IBC --> PF --> UV --> Retour --> Bassin
    IBC --> PD --> Deco
    FAT --> Waste
```

## Interfaces mecaniques et instrumentation

| Sous-ensemble | Interface connue | Impact de conception |
| --- | --- | --- |
| Tube de report de niveau | 32 mm, bouche en partie haute avec event de 1 mm | Permet une fixation protegee des deux capteurs cote eau propre EP_LAVAGE et EP_CRITIQUE, et facilite le nettoyage |
| Capteurs de niveau | CR18-8DN, M18, distance ajustable 8 mm, sortie NPN, alimentation 12-24 VDC, 10 mA, DC 3 fils | Necessitent un support mecanique adapte et des entrees compatibles ou conditionnees |
| Goutiere de trop-plein | seuil fixe a 30,5 cm | Fixe la cote maximale exploitable pour les seuils de pilotage |
| Support du FAT | a fabriquer | Conditionne tout le regime gravitaire par rapport au bassin |
| Capot | a creer avec capteur d'ouverture | Ajoute une entree de securite supplementaire |
| Joint a levre tambour | a poser | Indispensable pour separer correctement eau sale et eau propre |
| Moteur tambour | SWF 403.835, 12 V DC, connecteur 5 broches | Brochage, vitesse, courant et sens de rotation a valider avant schema definitif |
| Transmission tambour | Pignon 10 cm vers engrenage 30 cm | Reduction 3:1, vitesse tambour estimee 13 a 20 tr/min avant mesure |
| Protection moteur tambour | Fusible initial 10 a 15 A et protection materielle surintensite/blocage obligatoire | Le retour defaut vers l'automate est optionnel en V1 si le module choisi le fournit simplement |
| Pompe de rincage | VEVOR / Leo EKJ-802S, raccords 1 pouce, IPX4, classe I | Pompe 230 VAC de surface, a proteger electriquement et a maintenir hors immersion |
| Rampe de rincage | Tuyau 32 mm + buses | Le choix des buses fixera le point debit/pression reel de la pompe |
| Sonde temperature bassin | sonde numerique etanche type DS18B20 candidate | A implanter dans le bassin ou une zone tres representative de l'eau du bassin ; alerte informative en V1 avec seuils initiaux < 4 deg C et > 28 deg C |
| Sonde temperature ambiante local | sonde numerique simple candidate | Fournit une mesure representative de l'air du local de filtration ; alerte informative en V1 avec seuils initiaux < 2 deg C et > 40 deg C |
| IHM locale | ecran texte ou petit afficheur, commandes physiques, voyants MARCHE et ALARME | L'ecran porte le detail ; voyant MARCHE vert et voyant ALARME rouge sont retenus en V1, voyant LAVAGE jaune ou ambre optionnel |
| Liaison distante | option V2 Wi-Fi | Le materiel MVP doit etre pret pour une V2 Wi-Fi sans remplacement de plateforme principale ; la notification ne doit pas compromettre le fonctionnement local |
| Horloge fiable | capacite plateforme V2 obligatoire, implementation MVP optionnelle | La plateforme V1 doit permettre une heure fiable en V2 sans remplacement materiel principal, par RTC, temps local conserve, module temps, synchronisation reseau ou equivalent, sans dependance exclusive a Internet |
| Position tambour | option V1.1 a etudier | Peut aider pour l'indexation et certains diagnostics avances |

L'UV est retenu hors tambour dans la chaine de filtration, apres la pompe principale. Il reste asservi a la filtration autorisee et a l'absence de EP_CRITIQUE ; il n'est pas coupe sur un defaut FAT non critique si la filtration reste autorisee.

## Schema de principe

```mermaid
flowchart TB
    subgraph BT[Basse tension]
        Alim[Alimentation isolee]
        MCU[Carte de controle]
        Inputs[Entrees capteurs]
        UI[Commandes, ecran local et voyants complementaires]
    end

    subgraph P[Puissance]
        Prot[Protections electriques]
        K1[Commande pompe]
        K2[Commande moteur]
        K3[Commande prises auxiliaires]
    end

    Alim --> MCU
    Inputs --> MCU
    UI <--> MCU
    MCU --> K1
    MCU --> K2
    MCU --> K3
    Prot --> K1
    Prot --> K2
    Prot --> K3
```

## Decisions materielles a prendre

- tension de commande : 12 V ou 24 V, decision ouverte ;
- type de carte de controle, a comparer selon cout, effort de developpement, robustesse et maintenabilite ;
- nombre de capteurs CR18-8DN et implantation exacte sur le tube de report ;
- brochage exact du moteur tambour SWF 403.835 et isolation propre des fonctions parking en V1, sauf contrainte de brochage simple ;
- alimentation 12 V moteur, calibre fusible et protection materielle surintensite/blocage ;
- commande secteur de la pompe de rincage, protection moteur et raccordement a la terre ;
- debit ou pression de rincage de reference apres essais sur la rampe et les buses ;
- mesure terrain de la cote support FAT avant fabrication, afin d'aligner trop-plein physique et niveau hydraulique cible du bassin ;
- calcul final de la geometrie des ouvertures du tambour avant decoupe ou percage, avec objectif 0,20 a 0,23 m2 de surface filtrante utile ;
- type de sonde de temperature bassin et implantation exacte ;
- type de sonde de temperature ambiante local et implantation exacte ;
- type d'IHM locale : LED, ecran ou combinaison ;
- nombre de voyants, couleurs et signification ;
- architecture Wi-Fi V2 : Wi-Fi natif ou module Wi-Fi ajoutable proprement sans remplacement de plateforme principale ;
- architecture de notification pour une V2 : embarquee, serveur local, service mail ou service push simple ;
- besoin ou non d'un capteur de position tambour ;
- strategie materielle d'indexation du tambour hors lavage pour V1.1 ;
- methode empirique d'estimation de la consommation d'eau de rincage pour V1.1 ou V2 ;
- compatibilite native ou conditionnement des entrees pour capteurs NPN 12-24 VDC ;
- interface d'entree necessaire pour la sonde de temperature ;
- interface d'entree necessaire pour la sonde de temperature ambiante ;
- nombre de prises auxiliaires a couper et puissance par voie ;
- choix relais/contacteurs/variateur ;
- niveau de protection du coffret dans un local humide non expose a la pluie directe ;
- connecteurs et borniers.
