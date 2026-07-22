# ADR-0012 - Migration de la plateforme KC868-A32 vers KC868-A16

## Statut

Acceptee

## Contexte

Le noyau fonctionnel V1 utilise neuf entrees digitales et neuf sorties digitales. Le KC868-A32 commande le 2026-07-06 fournit 32 entrees et 32 relais, mais son cout, son encombrement et sa reserve de 23 voies inutilisees de chaque type sont disproportionnes par rapport au besoin du controleur FAT.

La KC868-A16 ESP32 classique analysee fournit 16 entrees digitales optocouplees, 16 sorties P-MOSFET 12/24 VDC, trois GPIO capteurs, Ethernet, Wi-Fi et RS485. Son format est nettement plus compact et son cout d'achat est sensiblement inferieur. Les 16 entrees et 16 sorties laissent sept voies libres de chaque type apres affectation du noyau V1.

L'A16 n'est toutefois pas une A32 reduite : ses sorties ne sont pas des contacts secs de relais. Elles fournissent une tension continue 12/24 V et ne doivent jamais commuter directement du 230 VAC. La pompe de rincage est deja associee a un contacteur Schneider TeSys LC1D18P7 dont la bobine est en 230 VAC ; une interface supplementaire devient donc obligatoire.

Les captures de l'offre A16 montrent plusieurs revisions materielles, notamment REV1.4 et REV1.6. Les adresses I2C des expanseurs PCF8574 peuvent varier selon la revision livree. Le firmware bas niveau deja prepare pour l'A32 ne peut donc pas etre reutilise tel quel. La carte recue le 2026-07-22 est identifiee `KC868-A16 REV.1.6.3` ; son scan initial repond aux adresses standard. Les essais en contact sec ont confirme `0x22 = X1-X8` et `0x21 = X9-X16`, mais la polarite et les bornes physiques des sorties ne sont pas encore validees.

## Decision

La plateforme materielle cible du MVP et de la V2 devient la **Kincony KC868-A16 ESP32 classique a sorties MOSFET**.

Cette decision concerne l'A16 basee sur ESP32-WROOM-32. Elle ne couvre pas automatiquement l'A16S a relais ni l'A16v3 basee sur ESP32-S3, qui exigeraient une nouvelle verification materielle et un portage firmware distinct.

Le KC868-A32 deja recu ou en cours de reception est abandonne comme plateforme cible. Son achat reste trace dans le journal des commandes. Sa destination finale, retour fournisseur, revente, conservation comme banc d'essai ou reutilisation dans un autre projet, reste a decider sans impact sur l'architecture FAT.

La tension de commande reste fixee a 12 VDC avec la Mean Well NDR-120-12. L'alimentation principale de l'A16 et les deux arrivees `12V/24V DC Input` de ses banques de sorties doivent etre alimentees et protegees selon le schema final, avec un 0 V commun.

Les sorties A16 sont affectees comme suit en premiere intention :

| Alias firmware | Borne physique REV.1.6.3 | Signal | Interface cible |
| --- | --- | --- | --- |
| O1 | Y1 | `CMD_TAMBOUR` | Bobine 12 VDC du relais HELLA, sous reserve de mesure de courant et de suppression de surtension adaptee |
| O2 | Y2 | `CMD_RINCAGE` | Bobine 12 VDC d'un relais d'interface ; son contact commande la bobine 230 VAC du LC1D18P7 |
| O3 | Y3 | `CMD_POMPE_FILTRATION` | Bobine 12 VDC du contacteur TOMZN |
| O4 | Y4 | `CMD_POMPE_DECO` | Bobine 12 VDC du contacteur TOMZN |
| O5 | Y5 | `CMD_UV` | Bobine 12 VDC du contacteur TOMZN |
| O6 | Y6 | `CMD_MISE_A_NIVEAU` | Bobine 12 VDC du contacteur TOMZN |
| O7 | Y7 | `VOYANT_MARCHE` | Voyant LED 12 VDC |
| O8 | Y8 | `VOYANT_LAVAGE` | Voyant LED 12 VDC |
| O9 | Y9 | `VOYANT_ALARME` | Voyant LED 12 VDC |

Chaque charge raccordee directement a une sortie MOSFET doit rester sous la limite publiee de 500 mA par voie. Le courant reel des bobines HELLA et TOMZN doit etre mesure. Une diode de roue libre ou une suppression equivalente doit etre prevue sur chaque bobine DC si elle n'est pas deja integree, en respectant la polarite.

Le relais d'interface de rinçage reste a choisir. Il devra au minimum posseder :

- une bobine 12 VDC compatible avec une sortie A16 et consommant moins de 500 mA ;
- un support ou un boitier rail DIN adapte au coffret ;
- un contact 230 VAC avec une categorie d'emploi publiee pour charge electromagnetique, cible `AC-15 >= 1 A sous 230 VAC` ;
- une suppression de surtension adaptee cote bobine 12 VDC ;
- un reperage et une separation propres entre la commande 12 VDC et le circuit de bobine 230 VAC.

Les capteurs CR18-8DN restent raccordes en 12 VDC avec sortie NPN vers les entrees digitales A16 : marron sur `+12 VDC`, bleu sur `0 V`, noir vers la borne physique `Xn`. Le sens logique, la stabilite et les cas de rupture de fil doivent etre valides sur banc avec la carte recue. Les alias firmware V1 sont `I1` a `I9`, respectivement `X1` a `X9`.

Le raccordement cible des auxiliaires utilise les trois GPIO capteurs de l'A16 : `GPIO32` et `GPIO33` pour un bus I2C auxiliaire separe, porte par le second controleur `TwoWire` de l'ESP32 et partage par le LCD 2004 et la RTC DS3231, et `GPIO14` pour un bus 1-Wire commun aux deux DS18B20. Cette affectation reste soumise au controle du brochage de la revision recue et aux essais de coexistence, de niveaux 3,3 V et de longueur de cable.

### Constat de reception REV.1.6.3

Le firmware `kc868_a16_hw_safe` a ete execute sur la carte recue. Le bus interne `GPIO4/GPIO5` a detecte `0x21`, `0x22`, `0x24` et `0x25`; les deux banques d'entrees et les deux banques de sorties repondent. Les contacts secs confirment le role des entrees : `0x22 = X1-X8`, `0x21 = X9-X16`. L'ecriture boot OFF `0xFF` est confirmee par I2C. Le build de banc `kc868_a16_hw_output_test` a valide sans charge la polarite active et la commande isolee de `Y1-Y16`; le profil reste non valide et les sorties d'exploitation effectivement desarmees. Le detail des preuves et des limites est consigne dans [VR-0001](../validation/VR-0001-reception-kc868-a16-rev1.6.3.md).

## Consequences

- L'ADR-0003 est remplacee pour le choix de plateforme.
- Les clauses de l'ADR-0004 qui imposaient l'A32 et la commande directe de la bobine 230 VAC du LC1D18P7 sont remplacees par la presente decision ; les protections, departs et principes de separation restants demeurent applicables.
- Le schema electrique doit etre repris autour des sorties P-MOSFET 12 VDC de l'A16.
- Un relais d'interface 12 VDC vers 230 VAC doit etre choisi, commande et valide avant le cablage de la pompe de rincage.
- Le firmware metier, la machine a etats et le simulateur restent reutilisables.
- La couche HAL KC868 doit etre reconfiguree ou reecrite pour deux banques d'entrees et deux banques de sorties, les adresses I2C reelles de la revision recue, la polarite des sorties MOSFET et un etat sur au demarrage.
- Aucun environnement materiel ne doit armer les sorties tant que le scan I2C, la cartographie, la polarite et le test `toutes sorties OFF` ne sont pas valides sur banc.
- La documentation d'architecture, d'achats, de validation et de firmware doit utiliser l'A16 comme cible courante tout en conservant l'historique d'achat de l'A32.

## Alternatives considerees

- Conserver la KC868-A32 : solution fonctionnelle et simple pour commuter la bobine 230 VAC, mais rejetee comme plateforme cible a cause du cout et de l'encombrement disproportionnes.
- Choisir une A16S a relais : plus proche de l'A32 pour les sorties, mais elle ne correspond pas a l'offre economique analysee et doit faire l'objet d'une comparaison distincte.
- Choisir une A16v3 ESP32-S3 : materiel plus recent, mais portage firmware et brochage differents ; non retenue sans analyse complementaire.
- Remplacer le LC1D18P7 par un contacteur a bobine DC : techniquement possible, mais le LC1D18P7 est deja commande ; l'ajout d'un seul relais d'interface est retenu comme modification minimale.
