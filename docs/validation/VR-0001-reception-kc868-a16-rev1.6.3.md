# VR-0001 - Reception et identification KC868-A16 REV.1.6.3

## Objet

Tracer l'identification de la carte effectivement recue et les premiers constats non destructifs avant toute alimentation de charge ou armement des sorties.

## Carte recue

| Champ | Constat | Statut |
| --- | --- | --- |
| Reference serigraphiee | `KC868-A16` | Confirme |
| Revision PCB | `REV.1.6.3` | Confirme |
| Famille | A16 classique a ESP32-WROOM-32 et sorties MOSFET | Confirme par serigraphie et inspection visuelle ; ni A16S ni A16v3 |
| Date de reception / constat | 2026-07-22 | Trace |
| Firmware de banc | `kc868_a16_hw_safe` | Utilise |
| Armement des sorties | `HARDWARE_OUTPUTS_ARMED=0`, profil non valide | Confirme par diagnostic |

La photo de reception est conservee dans les pieces jointes de la tache Codex. Elle montre la serigraphie `KC868-A16 REV.1.6.3` et le module ESP32-WROOM-32.

## Nomenclature physique retenue

La serigraphie de la carte est la reference de cablage :

- `X1` a `X16` : entrees digitales physiques ;
- `Y1` a `Y16` : sorties MOSFET physiques ;
- `GND` du bornier d'entrees : retour des essais en contact sec ;
- `12V/24V DC Input` ou `VIN` : alimentation des groupes de sorties, a laisser deconnectee lors des premiers essais d'E/S sans charge.

Dans le firmware et les documents fonctionnels, `I1` a `I9` et `O1` a `O9` restent des **alias logiques**. Ils se raccordent respectivement a `X1` a `X9` et `Y1` a `Y9`. Les voies `X10` a `X16` et `Y10` a `Y16` sont reservees en V1.

## Resultat du premier demarrage securise

Le scan interne et les diagnostics suivants ont ete obtenus avec la carte alimentee, l'USB connecte, aucune charge raccordee et les groupes de sorties non alimentes :

```text
I2C interne : 0x21, 0x22, 0x24, 0x25
Entrees : Raw 0xFF 0xFF ; banques 1 et 2 OK
Sorties : boot OFF OK ; Raw sorties 0xFF 0xFF ; banques 1 et 2 OK
Armement : demande NO ; profil valide NO ; sorties effectivement armees NO
```

Le premier scan a identifie les quatre adresses mais pas leur role. Les essais en contact sec ont ensuite confirme `X1` -> `MANU_RINCAGE` avec le profil generique, puis `X9` -> `EP_LAVAGE`, `X10` -> `EP_CRITIQUE` et `X16` -> `MANU_TAMBOUR`. Ils prouvent l'ordre reel des banques d'entrees : `0x22` porte `X1` a `X8`, puis `0x21` porte `X9` a `X16`.

Apres selection du profil REV.1.6.3, les neuf entrees fonctionnelles sont confirmees dans l'ordre attendu : `X1` `EP_LAVAGE`, `X2` `EP_CRITIQUE`, `X3` `CAPOT_OUVERT`, `X4` `MODE_AUTO`, `X5` `MODE_MAINTENANCE`, `X6` `RESET_ALARME`, `X7` `TEST_LAVAGE`, `X8` `MANU_TAMBOUR`, `X9` `MANU_RINCAGE`. Les voies reservees sont aussi confirmees par le diagnostic brut : `X10` `0xFF 0xFD`, `X11` `0xFF 0xFB`, `X12` `0xFF 0xF7`, `X13` `0xFF 0xEF`, `X14` `0xFF 0xDF`, `X15` `0xFF 0xBF`, `X16` `0xFF 0x7F`.

Le firmware selectionne donc le profil non valide `a16-rev1.6.3-inputs-confirmed-candidate` : `GPIO4/GPIO5`, entrees `0x22` / `0x21`, sorties candidates `0x24` / `0x25`. Les quatre adresses repondent et aucune erreur I2C n'est memorisee. L'ecriture OFF a ete acceptee avant toute commande de sortie. Les roles, polarites et niveaux electriques des sorties restent non verifies.

Les deux banques de sorties ont ensuite ete alimentees en 12 V, sans charge. Au demarrage du build safe, la tension mesuree entre chaque borne `Y1` a `Y16` et le 0 V d'alimentation est comprise entre **100 mV et 200 mV**. Cette mesure confirme physiquement l'etat OFF au boot sur les 16 sorties. Elle ne valide pas encore la commande ON, la polarite active ni l'affectation individuelle des sorties.

Le 2026-07-22, le build dedie `kc868_a16_hw_output_test` a ensuite ete utilise, toujours sans charge. Les commandes `diag output 1 pulse 1000` a `diag output 9 pulse 1000` ont produit **12 V pendant 5 s** sur la seule borne cible `Y1` a `Y9` (la limite du build de banc est 5 000 ms). Les voyants des quinze autres voies sont restes eteints lors de chaque essai. Cette preuve confirme la polarite active, l'isolement entre voies et la correspondance `O1-Y1` a `O9-Y9`. Les sorties reservees `Y10-Y16` restent a tester individuellement avant validation complete du profil.

## Prochaines preuves requises

1. Flasher l'environnement dedie `kc868_a16_hw_output_test`. Il force les commandes de l'automate a OFF et n'autorise qu'une impulsion serie explicite de 5 s maximum, sans rendre le profil `validated`.
2. Tester individuellement les sorties reservees `Y10` a `Y16`, sans charge, avant toute validation complete de la banque de sortie.
3. Completer le profil firmware `a16-rev1.6.3-inputs-confirmed-candidate` avec la cartographie et les polarites actives de sortie mesurees ; le maintenir `validated=false` a ce stade.
4. Tester ensuite chaque voyant et chaque bobine apres mesure de courant et verification des suppressions de surtension.
5. Completer separement LCD, RTC et DS18B20.

Le profil ne pourra passer a `validated=true` qu'apres les preuves ci-dessus et mise a jour de la checklist go/no-go.
