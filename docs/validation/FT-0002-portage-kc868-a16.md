# FT-0002 - Portage firmware KC868-A16

## Objet

Tracer la preuve logicielle du portage de la couche materielle KC868-A32 vers la KC868-A16 ESP32 classique et definir le verrou restant avant tout armement physique.

## Statut

**Logiciel passe ; validation sur carte en attente.** La KC868-A16 cible n'est pas encore disponible. Aucun profil materiel n'est donc marque `validated` et le build `kc868_a16_hw_armed` reste effectivement desarme.

## Preuve automatisee

Commandes de reference :

```powershell
cd firmware
python -m platformio test -e native
python -m platformio run -e kc868_a16_sim -e kc868_a16_hw_safe -e kc868_a16_hw_armed
```

Resultat local du 2026-07-14 :

- 51 tests host-side passes ;
- compilation reussie des trois environnements A16 ;
- aucune image n'est televersee ou publiee par la CI.

La couverture A16 verifie notamment :

- deux banques de huit entrees et deux banques de huit sorties ;
- passage I8/I9 et O8/O9 ;
- polarites d'entree et de sortie configurables ;
- ecriture des deux banques de sorties a OFF avant l'initialisation des entrees ;
- profils candidats standard et REV1.5 tous deux non valides ;
- banque d'entree absente ;
- echec d'ecriture de sortie verrouille jusqu'au redemarrage ;
- refus de toute sortie si un verrou manque : build arme, profil valide, boot OFF, entrees saines ou sorties saines.

## Profils candidats non valides

| Profil | Entrees | Sorties | Usage |
| --- | --- | --- | --- |
| `a16-classic-standard-candidate` | `0x21`, `0x22` | `0x24`, `0x25` | Profil de scan initial par defaut. |
| `a16-rev1.5-green-candidate` | `0x39`, `0x3A` | `0x3C`, `0x3D` | Candidat seulement si la revision et l'etiquette correspondent. |

Le scan ne deduit jamais le role des adresses. Une adresse differente, par exemple `0x3B` observee sur certaines cartes, impose la creation d'un profil correspondant a la carte recue.

## Procedure bloquante a reception

1. Confirmer KC868-A16 ESP32-WROOM-32 classique, revision PCB et absence de confusion avec A16S/A16v3.
2. Flasher `kc868_a16_hw_safe`, sans charge et avec les alimentations de banques de sorties deconnectees.
3. Executer `diag i2c`, relever les quatre PCF8574 et fixer explicitement leurs roles.
4. Alimenter les banques de sorties sans charge et mesurer O1 a O16 au boot ; confirmer l'etat OFF brut et la polarite.
5. Tester I1 a I16 individuellement, puis les CR18-8DN au repos, en detection et avec chaque fil debranche.
6. Creer un profil nomme pour la carte recue, y inscrire revision, adresses, cartographie, polarites et adresses ROM des deux DS18B20.
7. Passer ce profil a `validated=true` uniquement apres archivage des mesures precedentes.
8. Tester `diag output <1-9> pulse <ms>` sans charge, puis avec voyants et bobines une par une apres mesure du courant et validation des suppressions de surtension.

## Auxiliaires

Le bus interne PCF8574 reste sur GPIO4/GPIO5. Un second bus I2C utilise GPIO32/GPIO33 pour le LCD 2004 et la RTC DS3231 ; le bus 1-Wire utilise GPIO14.

Commandes de diagnostic :

- `diag aux i2c` : scan LCD/RTC ;
- `diag rtc` : presence, perte d'alimentation et validite de l'heure ;
- `diag temp` : ROM des sondes detectees et affectations eau/local.

L'absence du LCD ou de la RTC ne bloque pas l'automate. Une sonde DS18B20 non configuree ou absente reste invalide et declenche le comportement A11/A12 existant, sans valeur fictive de remplacement.

## Critere de cloture

FT-0002 ne pourra etre declaree completement passee qu'apres ajout du profil exact de la carte recue, validation physique du boot OFF, des 16 entrees/sorties, des auxiliaires et mise a jour de la checklist go/no-go materiel.

