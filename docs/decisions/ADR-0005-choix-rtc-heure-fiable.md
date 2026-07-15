# ADR-0005 - Choix de la RTC pour l'heure fiable

## Statut

Acceptee

## Contexte

Le MVP doit rester exploitable sans connectivite active, mais la plateforme V1 doit permettre une heure fiable en V2 sans remplacement du materiel principal et sans dependance exclusive a Internet.

L'heure fiable servira surtout aux fonctions de diagnostic et d'exploitation :

- horodatage du mini-journal persistant, notamment `A15 - CAPOT OUVERT LONG`, `EP_CRITIQUE`, lavage inefficace, capot dangereux, redemarrage apres coupure et acquittements reussis ;
- affichage ou consultation du dernier lavage ;
- statistiques par heure et par jour ;
- test journalier automatique V1.1 ;
- programmation horaire de la pompe decoration V1.1/V2 ;
- notifications et synthese quotidienne V2.

Les temporisations de securite et de cycle, comme EP_CRITIQUE, EP_LAVAGE, capot ouvert, duree lavage, anti-redemarrage et timeouts, ne doivent pas dependre d'une heure civile modifiable.

## Decision

La voie retenue pour l'heure fiable est un module RTC DS3231 haute precision, I2C, compatible 3,3 V, avec batterie rechargeable livree avec le module.

Le module RTC doit etre alimente en 3,3 V cote KC868-A16 / ESP32. Les lignes I2C `SDA` et `SCL` doivent rester tirees au 3,3 V, sans pull-up vers 5 V.

Le raccordement cible est le bus I2C auxiliaire separe de la KC868-A16 via `GPIO32` / `GPIO33`, porte par le second controleur `TwoWire` de l'ESP32 et deja retenu pour l'ecran LCD 2004 I2C, sous reserve de validation du brochage de la revision recue. L'adresse attendue de la RTC DS3231 est `0x68`, distincte des adresses LCD candidates `0x27` ou `0x3F`.

Si les essais montrent qu'un bus partage LCD + RTC est instable, trop sensible aux longueurs de cable ou aux pull-up des modules, la RTC devra etre placee sur un bus I2C separe si des GPIO libres le permettent.

Le Wi-Fi/NTP pourra etre ajoute en V2 pour mettre a l'heure ou corriger la RTC lorsque le reseau est disponible. Il ne remplace pas la RTC comme unique source d'heure fiable.

## Consequences

- Le schema V1 doit prevoir un connecteur ou une implantation pour le module DS3231, meme si l'usage logiciel complet de l'heure est reporte.
- La validation banc doit verifier la presence de la RTC a l'adresse `0x68`, la conservation de l'heure apres coupure de l'alimentation principale, le type rechargeable de la batterie livree et l'absence de pull-up I2C vers 5 V.
- La RTC peut permettre d'horodater le dernier evenement du mini-journal si elle est integree simplement dans le MVP.
- En cas d'heure RTC invalide ou non initialisee, le controleur doit continuer a fonctionner localement ; les evenements sont alors conserves avec compteurs persistants et indication d'heure inconnue.
- Les temporisations de securite et de lavage doivent rester basees sur des timers monotones internes, pas sur l'heure RTC.
- Une future synchronisation reseau doit seulement ajuster le temps civil ; elle ne doit pas bloquer le fonctionnement local.

## Alternatives considerees

- Horloge interne ESP32 seule : non retenue comme source fiable apres coupure secteur.
- NTP seul via Wi-Fi : non retenu comme unique source, car la solution ne doit pas dependre exclusivement d'Internet.
- Module RTC PCF8563 ou PCF8523 : possible, mais moins prioritaire que DS3231 pour la precision et l'ecosysteme disponible.
- Pas de RTC en V1 : acceptable logiciellement pour le MVP, mais le cablage doit au moins laisser une voie claire vers la DS3231 pour eviter une reprise materielle inutile.
