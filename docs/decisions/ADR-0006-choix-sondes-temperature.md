# ADR-0006 - Choix des sondes de temperature

## Statut

Acceptee

## Contexte

Le controleur V1 doit mesurer deux temperatures informatives :

- `TEMP_BASSIN`, representative de l'eau du bassin ;
- `TEMP_LOCAL`, representative de l'air du local de filtration.

Ces mesures servent aux alertes locales V1, a l'affichage, puis aux statistiques et notifications V1.1/V2. Elles ne doivent pas bloquer les fonctions de filtration, lavage, UV ou mise a niveau en V1.

Les contraintes principales sont :

- compatibilite avec le KC868-A16 / ESP32 en logique 3,3 V ;
- cablage simple et maintenable ;
- comportement robuste dans un environnement humide ;
- achat simple de pieces identiques si cela ne degrade pas la fonction ;
- detection claire d'une perte de mesure.

## Decision

Les deux mesures `TEMP_BASSIN` et `TEMP_LOCAL` utilisent le meme type de sonde : DS18B20 etanche en tube inox, cable 3 fils, longueur cible 3 m.

Le choix d'utiliser deux sondes identiques est retenu pour simplifier l'achat, le stock de rechange, le cablage, le firmware et les essais sur banc.

Le raccordement cible est en mode 3 fils, alimente en 3,3 V, sans alimentation parasite :

| Fil fonctionnel | Raccordement |
| --- | --- |
| `VCC` | `3.3 V` |
| `DATA` | `GPIO14` de la KC868-A16 en premiere intention, avec les deux sondes sur un bus 1-Wire commun |
| `GND` | `GND / 0 V` commun |
| Pull-up | `4,7 kΩ` entre `DATA` et `3.3 V` |

Les couleurs de fils annoncees par les vendeurs ne sont pas considerees fiables. Le brochage reel de chaque lot doit etre verifie sur banc avant raccordement definitif.

Si des GPIO libres sont disponibles, la preference est de placer `TEMP_BASSIN` et `TEMP_LOCAL` sur deux bus 1-Wire separes. Cela evite qu'un defaut de cable de la sonde bassin perturbe la mesure local. Si les GPIO disponibles sont limites, un bus 1-Wire commun avec les deux sondes reste acceptable pour la V1, les alertes temperature etant informatives.

## Consequences

- La sonde bassin doit etre implantee dans une eau representative : arrivee gravitaire avant pompe/UV ou bassin en zone brassee, ombragee et accessible.
- La sonde local, bien qu'etanche, doit etre montee dans l'air du local sur un petit support ventile, hors coffret, loin de l'alimentation, des contacteurs, des pompes, du soleil et du volume chaud sous capot.
- Le firmware doit identifier les sondes par leur adresse 1-Wire unique ou par bus separe, afin de ne pas inverser eau et local.
- La perte de sonde eau affiche `A11 - SONDE EAU ABSENTE`.
- La perte de sonde local affiche `A12 - SONDE LOCAL ABSENTE`.
- Les seuils initiaux restent configurables : eau basse < 4 deg C, eau haute > 28 deg C, local bas < 2 deg C, local haut > 40 deg C.
- Une lecture invalide, absente ou incoherente ne bloque pas le fonctionnement V1.

## Alternatives considerees

- DS18B20 etanche pour l'eau et capteur different pour l'air : non retenu pour simplifier l'achat et le firmware.
- DHT11/DHT22 : non retenus, moins adaptes a un environnement de coffret/local humide et sans avantage pour la mesure d'eau.
- NTC analogique : non retenue, car elle demande une entree analogique, une calibration et une interpretation plus sensibles au cablage.
- PT100/PT1000 : robuste mais surdimensionne pour des alertes informatives de bassin/local.
- Sonde I2C type SHT31/SHT35 pour le local : interessante pour humidite/condensation future, mais reportee V1.1/V2 si ce besoin devient reel.
