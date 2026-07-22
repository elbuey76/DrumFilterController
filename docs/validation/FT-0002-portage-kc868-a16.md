# FT-0002 - Portage firmware KC868-A16

## Objet

Tracer la preuve logicielle du portage de la couche materielle KC868-A32 vers la KC868-A16 ESP32 classique et definir le verrou restant avant tout armement physique.

## Statut

**Logiciel passe ; validation sur carte en cours.** La carte recue est une **KC868-A16 REV.1.6.3**. Aucun profil materiel n'est encore marque `validated` et le build `kc868_a16_hw_armed` reste effectivement desarme.

## Preuve automatisee

Commandes de reference :

```powershell
cd firmware
python -m platformio test -e native
python -m platformio run -e kc868_a16_sim -e kc868_a16_hw_safe -e kc868_a16_hw_armed
# Banc de sorties local seulement : jamais utilise par la CI ni en exploitation.
python -m platformio run -e kc868_a16_hw_output_test
```

Resultat local du 2026-07-22 :

- 55 tests host-side passes ;
- compilation reussie des trois environnements A16 ;
- aucune image n'est televersee ou publiee par la CI.

La couverture A16 verifie notamment :

- deux banques de huit entrees et deux banques de huit sorties ;
- passage I8/I9 et O8/O9 ;
- polarites d'entree et de sortie configurables ;
- ecriture des deux banques de sorties a OFF avant l'initialisation des entrees ;
- profils candidats standard, REV1.5 et REV1.6.3 tous non valides ;
- ordre physique complet des entrees REV.1.6.3 : `X1-X9` fonctionnelles et `X10-X16` verifiees par valeurs brutes ;
- banque d'entree absente ;
- echec d'ecriture de sortie verrouille jusqu'au redemarrage ;
- refus de toute sortie si un verrou manque : build arme, profil valide, boot OFF, entrees saines ou sorties saines.

## Environnement d'impulsions de test des sorties

`kc868_a16_hw_output_test` est un environnement de banc distinct. Il conserve `HARDWARE_OUTPUTS_ARMED=0` et force donc les demandes du controleur a OFF a chaque boucle. Il ne permet qu'une commande serie `diag output <1-16> pulse <ms>`, bornee a 5 000 ms dans ce build de banc (1 000 ms dans les autres builds) et refusee si le boot OFF, les deux banques d'entrees, les deux banques de sorties ou le verrou de defaut ne sont pas sains.

Le profil REV.1.6.3 reste `validated=false` : son indicateur separe d'impulsions de diagnostic est ouvert uniquement parce que les entrees physiques et le boot OFF des 16 sorties ont ete prouvés. Cet environnement n'est ni un build d'exploitation, ni une autorisation de raccorder une charge.

## Reception REV.1.6.3 et premier demarrage securise

Le 2026-07-22, le build `kc868_a16_hw_safe` a ete execute avec une KC868-A16 REV.1.6.3 alimentee et sans charge. Le scan interne a detecte `0x21`, `0x22`, `0x24` et `0x25` sur `GPIO4/GPIO5`; les deux banques d'entrees et les deux banques de sorties sont saines. Les ecritures boot OFF ont retourne `0xFF 0xFF` et aucune sortie ne peut etre armee dans cet environnement. Les groupes de sorties ont ensuite ete alimentes en 12 V : chaque borne Y1-Y16 mesure 100 a 200 mV au boot, ce qui valide l'etat OFF physique sans charge.

La serigraphie de la carte utilise `X1` a `X16` pour les entrees et `Y1` a `Y16` pour les sorties MOSFET. Les termes `I1` a `I9` et `O1` a `O9` du firmware restent des alias fonctionnels : `I1` = `X1` et `O1` = `Y1`, dans l'ordre jusqu'a 9. Les essais par contact sec ont confirme que `0x22` porte `X1` a `X8` et `0x21` porte `X9` a `X16` : un profil REV.1.6.3 dedie inverse donc l'ordre des deux banques d'entrees par rapport au candidat generique. Les neuf fonctions X1-X9 et les valeurs brutes de X10-X16 sont confirmees. Le banc sans charge confirme aussi le boot OFF, la polarite active et l'activation isolee de `Y1-Y16`.

La fiche de reception detaillee est [VR-0001](VR-0001-reception-kc868-a16-rev1.6.3.md).

## Profils candidats non valides

| Profil | Entrees | Sorties | Usage |
| --- | --- | --- | --- |
| `a16-classic-standard-candidate` | `0x21`, `0x22` | `0x24`, `0x25` | Profil de scan initial par defaut. |
| `a16-rev1.5-green-candidate` | `0x39`, `0x3A` | `0x3C`, `0x3D` | Candidat seulement si la revision et l'etiquette correspondent. |
| `a16-rev1.6.3-inputs-confirmed-candidate` | `0x22` (`X1-X8`), `0x21` (`X9-X16`) | `0x24`, `0x25` | Profil selectionne pour les builds materiels ; E/S confirmees a vide, reste non valide tant que les charges et auxiliaires ne sont pas verifies. |

Le scan ne deduit jamais le role des adresses. Une adresse differente, par exemple `0x3B` observee sur certaines cartes, impose la creation d'un profil correspondant a la carte recue.

## Procedure bloquante a reception

1. Confirmer KC868-A16 ESP32-WROOM-32 classique, revision PCB et absence de confusion avec A16S/A16v3.
2. Flasher `kc868_a16_hw_safe`, sans charge et avec les alimentations de banques de sorties deconnectees.
3. Executer `diag i2c`, relever les quatre PCF8574 et fixer explicitement leurs roles ; pour la REV.1.6.3 recue, `0x22 = X1-X8` et `0x21 = X9-X16` sont confirmes.
4. Etat OFF au boot physiquement confirme sur Y1 a Y16, sans charge. Flasher `kc868_a16_hw_output_test`, puis tester l'etat ON et la polarite avec les impulsions diagnostiques controlees.
5. Les contacts secs X1 a X16 sont confirmes. Tester ensuite les CR18-8DN au repos, en detection et avec chaque fil debranche.
6. Completer le profil nomme de la carte recue avec la polarite des sorties et les adresses ROM des deux DS18B20 ; l'ordre des banques d'entrees REV.1.6.3 est deja confirme.
7. Passer ce profil a `validated=true` uniquement apres archivage des mesures precedentes.
8. Tester `diag output <1-16> pulse <ms>` sans charge, puis avec voyants et bobines une par une apres mesure du courant et validation des suppressions de surtension.

## Auxiliaires

Le bus interne PCF8574 reste sur GPIO4/GPIO5. Un second bus I2C utilise GPIO32/GPIO33 pour le LCD 2004 et la RTC DS3231 ; le bus 1-Wire utilise GPIO14.

Commandes de diagnostic :

- `diag aux i2c` : scan LCD/RTC ;
- `diag rtc` : presence, perte d'alimentation et validite de l'heure ;
- `diag temp` : ROM des sondes detectees et affectations eau/local.

L'absence du LCD ou de la RTC ne bloque pas l'automate. Une sonde DS18B20 non configuree ou absente reste invalide et declenche le comportement A11/A12 existant, sans valeur fictive de remplacement.

## Critere de cloture

FT-0002 ne pourra etre declaree completement passee qu'apres validation physique du boot OFF et des 16 sorties, des auxiliaires et mise a jour de la checklist go/no-go materiel.
