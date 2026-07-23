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

Le firmware selectionne donc le profil non valide `a16-rev1.6.3-inputs-confirmed-candidate` : `GPIO4/GPIO5`, entrees `0x22` / `0x21`, sorties `0x24` / `0x25`. Les quatre adresses repondent et aucune erreur I2C n'est memorisee. L'ecriture OFF a ete acceptee avant toute commande de sortie.

Les deux banques de sorties ont ensuite ete alimentees en 12 V, sans charge. Au demarrage du build safe, la tension mesuree entre chaque borne `Y1` a `Y16` et le 0 V d'alimentation est comprise entre **100 mV et 200 mV**. Cette mesure confirme physiquement l'etat OFF au boot sur les 16 sorties. Elle ne valide pas encore la commande ON, la polarite active ni l'affectation individuelle des sorties.

Le 2026-07-22, le build dedie `kc868_a16_hw_output_test` a ensuite ete utilise, toujours sans charge. Les commandes `diag output 1 pulse 1000` a `diag output 16 pulse 1000` ont produit **12 V pendant 5 s** sur la seule borne cible `Y1` a `Y16` (la limite du build de banc est 5 000 ms). Les voyants des quinze autres voies sont restes eteints lors de chaque essai. Cette preuve confirme la polarite active et l'isolement entre voies sur les 16 sorties, ainsi que la correspondance `O1-Y1` a `O9-Y9` ; `Y10-Y16` sont confirmees comme voies reservees dans l'ordre physique.

## Validation auxiliaire RTC + LCD

Le 2026-07-23, la RTC DS3231 a ete testee seule sur le bus I2C auxiliaire `Wire1`, configure sur `HT1` / `GPIO32` pour `SDA` et `HT2` / `GPIO33` pour `SCL`. Elle repond a l'adresse `0x68`; sa lecture d'heure, son etat et les essais de banc ont ete declares conformes.

Le LCD 2004 I2C a ensuite ete alimente en `3,3 V` et raccorde en parallele sur les memes lignes `SDA`/`SCL`. Le niveau de repos mesure sur chacune des deux lignes est **3,287 V**. Aucun pull-up externe n'a ete ajoute. Le scan du programme de banc `kc868_a16_rtc_lcd_test` retourne :

```text
--- SCAN I2C HT1/HT2 ---
Trouve : 0x27
Trouve : 0x68
Nombre de peripheriques : 2
```

Le LCD est donc confirme a l'adresse `0x27`, la RTC a `0x68`, et leur cohabitation immediate sur le meme bus est validee : les informations de test et l'heure RTC sont lisibles sur l'ecran. Le programme de banc maintient explicitement les sorties `Y` a l'etat OFF.

## Prochaines preuves requises

1. Tester chaque voyant et chaque bobine apres mesure de courant et verification des suppressions de surtension.
2. Valider les entrees avec les organes reels : CR18-8DN, contact capot, selecteur et boutons.
3. Completer les essais d'endurance du bus I2C partage et l'integration de l'affichage IHM complet.

Pour les DS18B20 retenues, le brochage confirme par leur documentation est rouge = `VCC`, noir = `GND`, jaune = `DATA`. Le fil jaune doit aller au bornier utilisateur `HT3` / `GPIO3` de la carte, qui correspond a `GPIO14` dans le firmware, avec une resistance de pull-up `4,7 kΩ` entre ce signal et `3.3 V`. La borne `X14` est une entree digitale optocouplee et ne doit pas etre utilisee.

Le 2026-07-23, les deux DS18B20 ont ete detectees sur le bus 1-Wire `GPIO14` : `284C19CC000000B2` et `28A3AFC800000062`. Leur alimentation 3,3 V, masse commune, signal et pull-up sont donc fonctionnels. Leur association physique confirmee est `TEMP_BASSIN = 28A3AFC800000062` et `TEMP_LOCAL = 284C19CC000000B2`; ces ROM sont inscrites dans le profil REV.1.6.3, sans jamais dependre de l'ordre de decouverte. Apres reflash du build safe, `diag temp` a affiche les deux valeurs valides et le rechauffement doux de chaque sonde a fait varier uniquement la ligne de son role attendu : lecture et affectation sont validees.

L'essai de deconnexion des deux sondes est valide : `diag temp` affiche `Sondes detectees: 0`, conserve les ROM configurees et retourne `TEMP_BASSIN: INVALIDE` ainsi que `TEMP_LOCAL: INVALIDE`. Aucune valeur fictive n'est injectee apres perte du bus.

Le profil ne pourra passer a `validated=true` qu'apres les preuves restantes ci-dessus et mise a jour de la checklist go/no-go.
