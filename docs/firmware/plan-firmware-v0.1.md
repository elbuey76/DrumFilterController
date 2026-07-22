# Plan de codage — Firmware MVP V0.1 du contrôleur FAT KC868-A16

## Objectif

Obtenir une première version firmware autonome, simulable sans matériel, puis raccordable progressivement à la KC868-A16 dès réception.

> Migration materielle : l'[ADR-0012](../decisions/ADR-0012-migration-kc868-a32-vers-a16.md) remplace l'A32 par une A16 ESP32 classique. La logique metier et le simulateur restent valides, mais la couche bas niveau A32 deja codee doit etre reprise selon les taches B-236 a B-239 avant tout armement des sorties physiques.

Cette première version priorise :

- la sécurité ;
- la machine à états ;
- les temporisations ;
- la logique lavage ;
- les alarmes ;
- l'IHM locale minimale ;
- la séparation entre logique métier et matériel réel.

## 1. Cible de la première version

Version visée : **firmware MVP V0.1**.

Cette version doit permettre de tester :

- démarrage sûr ;
- lecture logique des entrées simulées ;
- transitions entre modes ;
- cycle de lavage automatique ;
- test lavage manuel ;
- commandes manuelles tambour/rinçage ;
- arrêt sur capot ouvert ;
- arrêt sur niveau critique ;
- défaut lavage inefficace ;
- reset alarme ;
- affichage textuel simulé ;
- sorties calculées mais pas encore câblées au matériel réel.

### Ce que cette version ne fait pas encore

À exclure de V0.1 :

- Wi-Fi ;
- notifications distantes ;
- Home Assistant ;
- statistiques avancées ;
- test journalier automatique ;
- indexation périodique tambour ;
- programmation pompe décoration ;
- mesure réelle DS18B20 ;
- RTC réelle ;
- LCD réel ;
- relais réels KC868 ;
- persistance complète.

## 2. Choix technique recommandé

Environnement recommandé :

- PlatformIO ;
- framework Arduino ;
- langage C++ ;
- cible ESP32 / KC868-A16 classique.

Structure proposée :

```text
firmware/
  platformio.ini
  src/
    main.cpp

    app/
      Types.h
      Config.h
      Config.cpp
      Controller.h
      Controller.cpp
      StateMachine.h
      StateMachine.cpp
      Safety.h
      Safety.cpp
      WashCycle.h
      WashCycle.cpp
      AlarmManager.h
      AlarmManager.cpp
      Timers.h
      Timers.cpp

    hal/
      InputService.h
      InputService.cpp
      OutputService.h
      OutputService.cpp
      DisplayService.h
      DisplayService.cpp
      ClockService.h
      ClockService.cpp
      TemperatureService.h
      TemperatureService.cpp

    sim/
      SerialSimulator.h
      SerialSimulator.cpp
```

Principe de séparation :

- `app/` : logique métier indépendante du matériel ;
- `hal/` : accès matériel ou faux accès matériel ;
- `sim/` : simulateur série temporaire.

## 3. Étape 1 — Créer le squelette firmware

### Objectif

Avoir un projet qui compile, démarre, boucle et affiche son état dans le moniteur série.

### Fichiers à créer

- `firmware/platformio.ini`
- `firmware/src/main.cpp`
- `firmware/src/app/Types.h`
- `firmware/src/app/Config.h`
- `firmware/src/app/Config.cpp`
- `firmware/src/app/Controller.h`
- `firmware/src/app/Controller.cpp`

### Contenu attendu dans `Types.h`

```cpp
enum class SystemState {
  BOOT,
  AUTO_WAIT,
  WASH_AUTO,
  POST_WASH,
  SAFETY_PAUSE,
  RETRY_PAUSE,
  MANUAL,
  MAINTENANCE,
  TEST_WASH,
  DEGRADED,
  FAULT
};

struct InputsSnapshot {
  bool epLavage;
  bool epCritique;
  bool capotOuvert;

  bool modeAuto;
  bool modeMaintenance;

  bool btnReset;
  bool btnTestLavage;
  bool btnManuTambour;
  bool btnManuRincage;

  bool tempBassinValid;
  bool tempLocalValid;
  float tempBassinC;
  float tempLocalC;
};

struct OutputsCommand {
  bool cmdTambour;
  bool cmdRincage;
  bool cmdPompeFiltration;
  bool cmdPompeDeco;
  bool cmdUv;
  bool cmdMiseANiveau;

  bool voyantMarche;
  bool voyantLavage;
  bool voyantAlarme;
};

struct ControllerStatus {
  SystemState state;
  const char* message;
  const char* alarmCode;
};
```

### Contenu attendu dans `Config.h`

```cpp
struct Config {
  unsigned long epLavageDelayMs = 10000;
  unsigned long epCritiqueDebounceMs = 1000;

  unsigned long capotOpenDebounceMs = 300;
  unsigned long capotCloseStableMs = 1500;
  unsigned long capotLongOpenMs = 10UL * 60UL * 1000UL;

  unsigned long washMinMs = 10000;
  unsigned long washMaxMs = 45000;
  unsigned long residualRotationMs = 3000;
  unsigned long antiRestartMs = 60000;
  unsigned long retryPauseMs = 60000;

  uint8_t maxWashAttempts = 3;

  float tempBassinLowC = 4.0;
  float tempBassinHighC = 28.0;
  float tempLocalLowC = 2.0;
  float tempLocalHighC = 40.0;
};
```

### Critère de fin

Le projet compile et affiche au démarrage :

```text
FAT Controller V0.1
State: BOOT
```

## 4. Étape 2 — Implémenter le contrôleur central

### Objectif

Créer une boucle unique :

```text
inputs -> controller.update() -> outputs + status
```

### Fichiers concernés

- `firmware/src/app/Controller.h`
- `firmware/src/app/Controller.cpp`
- `firmware/src/app/Types.h`

### Interface cible

```cpp
class Controller {
public:
  Controller(const Config& config);

  void begin(unsigned long nowMs);
  OutputsCommand update(const InputsSnapshot& inputs, unsigned long nowMs);
  ControllerStatus status() const;

private:
  Config config_;
  SystemState state_;
  ControllerStatus status_;
};
```

### Comportement minimal

Au démarrage :

- toutes les sorties dangereuses doivent être à `false` ;
- le contrôleur passe par `BOOT` ;
- il choisit ensuite l'état selon les priorités :
  1. `EP_CRITIQUE` actif -> `FAULT` ;
  2. capot ouvert -> `MAINTENANCE` ;
  3. mode maintenance -> `MAINTENANCE` ;
  4. `EP_LAVAGE` actif seul -> `DEGRADED` ou `WASH_AUTO` selon stratégie retenue ;
  5. sinon -> `AUTO_WAIT`.

### Critère de fin

Avec des entrées simulées nominales :

```text
State: AUTO_WAIT
Tambour: OFF
Rincage: OFF
Filtration: ON
UV: ON
```

Avec `EP_CRITIQUE = true` :

```text
State: FAULT
Tambour: OFF
Rincage: OFF
Filtration: OFF
UV: OFF
Alarm: A01 - NIVEAU CRITIQUE
```

## 5. Étape 3 — Ajouter le simulateur série

### Objectif

Pouvoir tester la logique sans KC868.

### Fichiers concernés

- `firmware/src/sim/SerialSimulator.h`
- `firmware/src/sim/SerialSimulator.cpp`
- `firmware/src/main.cpp`

### Commandes série à prévoir

```text
help
status

lavage on
lavage off

critique on
critique off

capot open
capot close

auto
maintenance

reset
test
tambour on
tambour off
rincage on
rincage off

temp eau 20
temp local 18
temp eau lost
temp local lost
```

### Exemple d'utilisation

```text
> status
State: AUTO_WAIT
Level: OK
Alarm: none

> lavage on
EP_LAVAGE = ON

> status
State: WASH_AUTO
Tambour: ON
Rincage: ON
Voyant lavage: ON
```

### Critère de fin

On peut forcer tous les scénarios principaux depuis le moniteur série.

## 6. Étape 4 — Implémenter la sécurité capot

### Objectif

Faire du capot une sécurité prioritaire.

### Fichiers concernés

- `firmware/src/app/Safety.h`
- `firmware/src/app/Safety.cpp`
- `firmware/src/app/Controller.cpp`

### Règles

#### Capot ouvert hors action dangereuse

Si capot ouvert et aucun lavage en cours :

```text
State: MAINTENANCE
Message: MAINTENANCE - CAPOT OUVERT
Tambour: OFF
Rincage: OFF
Filtration: selon sécurité niveau
UV: selon sécurité filtration
```

#### Capot ouvert pendant lavage ou commande manuelle

Si capot ouvert pendant tambour ou rinçage :

```text
State: FAULT
Alarm: A03 - CAPOT OUVERT DANGER
Tambour: OFF
Rincage: OFF
```

#### Capot ouvert trop longtemps

Après 10 minutes :

```text
Alarm: A15 - CAPOT OUVERT LONG
Voyant alarme: ON fixe
```

`A15` doit disparaître automatiquement après fermeture stable.

### Critère de fin

Scénario à simuler :

```text
auto
lavage on
attendre départ lavage
capot open
```

Résultat attendu :

```text
State: FAULT
Alarm: A03 - CAPOT OUVERT DANGER
Tambour: OFF
Rincage: OFF
```

## 7. Étape 5 — Implémenter la logique niveaux

### Objectif

Gérer `EP_LAVAGE` et `EP_CRITIQUE`.

### Fichiers concernés

- `firmware/src/app/Safety.h`
- `firmware/src/app/Safety.cpp`
- `firmware/src/app/Controller.cpp`

### Règles

| Cas | Entrées | Réaction |
| --- | --- | --- |
| Niveau normal | `EP_LAVAGE = false`, `EP_CRITIQUE = false` | Niveau OK. |
| Demande lavage | `EP_LAVAGE = true`, `EP_CRITIQUE = false` | Demande lavage après temporisation. |
| Niveau critique | `EP_CRITIQUE = true` | Arrêt sécurité immédiat après anti-rebond court. |
| Incohérence capteurs | `EP_CRITIQUE = true`, `EP_LAVAGE = false` | `A02 - CAPTEURS NIVEAU INCOHÉRENTS`, puis `FAULT`. |

### Critère de fin

Scénarios simulables :

```text
lavage on
=> WASH_AUTO après délai

critique on
=> FAULT A01

lavage off + critique on
=> FAULT A02
```

## 8. Étape 6 — Implémenter le cycle de lavage automatique

### Objectif

Gérer un cycle complet borné.

### Fichiers concernés

- `firmware/src/app/WashCycle.h`
- `firmware/src/app/WashCycle.cpp`
- `firmware/src/app/Controller.cpp`

### États impliqués

- `AUTO_WAIT`
- `WASH_AUTO`
- `POST_WASH`
- `SAFETY_PAUSE`
- `RETRY_PAUSE`
- `DEGRADED`
- `FAULT`

### Règles de cycle

#### Départ lavage

Conditions :

- mode `AUTO` ;
- capot fermé ;
- `EP_CRITIQUE` absent ;
- aucun défaut critique ;
- `EP_LAVAGE` confirmé après délai.

Actions :

```text
cmdTambour = ON
cmdRincage = ON
voyantLavage = ON
```

#### Durée mini

Le lavage dure au moins 10 secondes, même si `EP_LAVAGE` revient normal avant.

#### Arrêt normal

Après durée mini, si `EP_LAVAGE = false` :

```text
cmdRincage = OFF
cmdTambour = ON pendant rotation résiduelle
State: POST_WASH
```

Puis :

```text
cmdTambour = OFF
State: SAFETY_PAUSE
```

Puis :

```text
State: AUTO_WAIT
```

#### Durée maxi

Si `EP_LAVAGE` reste actif jusqu'à 45 secondes :

- arrêter lavage ;
- si tentative restante : `RETRY_PAUSE` ;
- sinon : défaut lavage inefficace.

#### Tentatives max

Après 3 tentatives inefficaces :

```text
State: DEGRADED ou FAULT selon gravité
Alarm: A04 - LAVAGE INEFFICACE
cmdTambour = OFF
cmdRincage = OFF
```

Pour V0.1 :

- `EP_CRITIQUE` absent -> `DEGRADED` ;
- `EP_CRITIQUE` présent -> `FAULT`.

### Critères de fin

Scénario nominal :

```text
lavage on
attendre départ lavage
lavage off après 15 s
```

Résultat :

```text
WASH_AUTO -> POST_WASH -> SAFETY_PAUSE -> AUTO_WAIT
```

Scénario échec :

```text
lavage on permanent
```

Résultat :

```text
3 tentatives
A04 - LAVAGE INEFFICACE
State: DEGRADED
```

## 9. Étape 7 — Implémenter le mode manuel

### Objectif

Autoriser les commandes à action maintenue sous sécurité.

### Fichiers concernés

- `firmware/src/app/Controller.cpp`
- `firmware/src/app/Safety.cpp`

### Règles

#### `MANU_TAMBOUR`

Autorisé si :

- mode maintenance ou manuel ;
- capot fermé ;
- `EP_CRITIQUE` absent ;
- pas de défaut critique ;
- bouton maintenu.

Action :

```text
cmdTambour = ON tant que bouton maintenu
```

#### `MANU_RINCAGE`

Même logique :

```text
cmdRincage = ON tant que bouton maintenu
```

#### Refus

Si capot ouvert :

```text
Message: MANU REFUSÉ - CAPOT
```

Pas d'alarme bloquante si aucune sortie n'a démarré.

Si niveau critique :

```text
Message: MANU REFUSÉ - SÉCURITÉ
```

### Critère de fin

Scénario :

```text
maintenance
tambour on
```

Résultat :

```text
cmdTambour = ON
```

Puis :

```text
capot open
```

Résultat :

```text
cmdTambour = OFF
State: FAULT ou MAINTENANCE selon si action dangereuse était active
```

## 10. Étape 8 — Implémenter le test lavage

### Objectif

Créer un cycle test autonome, borné, sans relance multiple.

### Fichiers concernés

- `firmware/src/app/WashCycle.cpp`
- `firmware/src/app/Controller.cpp`

### Préconditions

Le test est autorisé si :

- mode `AUTO` ou `MAINTENANCE` ;
- capot fermé ;
- `EP_CRITIQUE` absent ;
- capteurs cohérents ;
- aucun défaut critique bloquant.

### Refus

Capot ouvert :

```text
A13 - TEST REFUSÉ CAPOT
```

Sécurité active :

```text
A14 - TEST REFUSÉ SÉCURITÉ
```

### Déroulement

Un seul cycle :

```text
cmdTambour = ON
cmdRincage = ON
durée bornée
pas de relance multiple
```

### Verdicts

Si `EP_LAVAGE` était inactif au départ :

```text
TEST OK - CYCLE EXÉCUTÉ
```

Si `EP_LAVAGE` était actif au départ et revient normal :

```text
TEST OK - NIVEAU OK
```

Si `EP_LAVAGE` reste actif :

```text
TEST ÉCHEC - LAVAGE INEFFICACE
```

### Critère de fin

Scénario :

```text
auto
test
```

Résultat :

```text
TEST_LAVAGE -> AUTO_WAIT
Message: TEST OK - CYCLE EXÉCUTÉ
```

Avec capot ouvert :

```text
capot open
test
```

Résultat :

```text
A13 - TEST REFUSÉ CAPOT
Aucune sortie active
```

## 11. Étape 9 — Implémenter les alarmes V1 minimales

### Objectif

Centraliser les alarmes et messages.

### Fichiers concernés

- `firmware/src/app/AlarmManager.h`
- `firmware/src/app/AlarmManager.cpp`
- `firmware/src/app/Types.h`

### Alarmes minimales V0.1

| Code | Message |
| --- | --- |
| A01 | NIVEAU CRITIQUE |
| A02 | CAPTEURS NIVEAU INCOHÉRENTS |
| A03 | CAPOT OUVERT DANGER |
| A04 | LAVAGE INEFFICACE |
| A05 | RESET REFUSÉ |
| A11 | SONDE EAU ABSENTE |
| A12 | SONDE LOCAL ABSENTE |
| A13 | TEST REFUSÉ CAPOT |
| A14 | TEST REFUSÉ SÉCURITÉ |
| A15 | CAPOT OUVERT LONG |

### Types d'alarmes

```cpp
enum class AlarmSeverity {
  INFO,
  WARNING,
  DEGRADED,
  BLOCKING
};
```

### Règles reset

Le reset est accepté seulement si :

- `EP_CRITIQUE` absent ;
- capteurs cohérents ;
- capot fermé si défaut capot dangereux ;
- `EP_LAVAGE` revenu normal si défaut lavage maintenu.

Sinon :

```text
A05 - RESET REFUSÉ
message cause courte
```

### Critère de fin

Le simulateur peut afficher :

```text
Alarm active: A04 - LAVAGE INEFFICACE
Reset refused: EP_LAVAGE ACTIF
```

## 12. Étape 10 — Ajouter l'affichage simulé

### Objectif

Préparer le futur LCD sans dépendre du vrai écran.

### Fichiers concernés

- `firmware/src/hal/DisplayService.h`
- `firmware/src/hal/DisplayService.cpp`

### Interface

```cpp
class DisplayService {
public:
  void begin();
  void render(const ControllerStatus& status, const InputsSnapshot& inputs, const OutputsCommand& outputs);
};
```

### Affichage série temporaire

Exemple :

```text
MODE: AUTO
NIVEAU: OK
LAVAGE: REPOS
ALARME: -
TEMP EAU: 18.5 C
```

Ou :

```text
MODE: DEFAUT
NIVEAU: CRITIQUE
LAVAGE: INHIBE
ALARME: A01 NIVEAU CRITIQUE
```

### Critère de fin

À chaque changement d'état, le moniteur série affiche une vue synthétique proche de ce qui sera affiché sur LCD 20x4.

## 13. Étape 11 — Reprendre la couche KC868 pour la KC868-A16 sans l'activer

### Objectif

Préparer le futur raccordement matériel sans bloquer la V0.1.

### Fichiers concernés

- `firmware/src/hal/InputService.h`
- `firmware/src/hal/InputService.cpp`
- `firmware/src/hal/OutputService.h`
- `firmware/src/hal/OutputService.cpp`

### Principe

Créer deux modes :

```cpp
#define USE_SIM_INPUTS 1
#define USE_KC868_IO 0
```

Pour l'instant :

```cpp
USE_SIM_INPUTS = 1
USE_KC868_IO = 0
```

Quand la carte arrive :

```cpp
USE_SIM_INPUTS = 0
USE_KC868_IO = 1
```

### Interface entrées

```cpp
class InputService {
public:
  void begin();
  InputsSnapshot read(unsigned long nowMs);
};
```

### Interface sorties

```cpp
class OutputService {
public:
  void begin();
  void apply(const OutputsCommand& outputs);
  void allOff();
};
```

### Critère de fin

Le firmware peut être compilé en mode simulation sans dépendre des broches réelles.

## 14. Étape 12 — Tests manuels à faire dans le simulateur

### Scénarios obligatoires V0.1

| Scénario | Entrées / action | Résultat attendu |
| --- | --- | --- |
| 1 — Boot nominal | `EP_LAVAGE = false`, `EP_CRITIQUE = false`, capot fermé, `AUTO = true` | `AUTO_WAIT`, filtration ON, UV ON, tambour OFF, rinçage OFF, aucune alarme. |
| 2 — Boot capot ouvert | Capot ouvert | `MAINTENANCE`, message `MAINTENANCE - CAPOT OUVERT`, tambour OFF, rinçage OFF. |
| 3 — Boot niveau critique | `EP_CRITIQUE = true` | `FAULT`, `A01 - NIVEAU CRITIQUE`, toutes sorties dangereuses OFF. |
| 4 — Lavage nominal | `EP_LAVAGE = true`, puis `EP_LAVAGE = false` après 15 s | `WASH_AUTO`, `POST_WASH`, `SAFETY_PAUSE`, `AUTO_WAIT`. |
| 5 — Lavage inefficace | `EP_LAVAGE = true` permanent, `EP_CRITIQUE = false` | 3 tentatives, `A04 - LAVAGE INEFFICACE`, `DEGRADED`, tambour OFF, rinçage OFF, filtration maintenue si sécurité OK. |
| 6 — Capot ouvert pendant lavage | `WASH_AUTO` actif, capot ouvert | `FAULT`, `A03 - CAPOT OUVERT DANGER`, tambour OFF, rinçage OFF. |
| 7 — Test lavage refusé capot | Capot ouvert, `TEST_LAVAGE` appuyé | `A13 - TEST REFUSÉ CAPOT`, aucune sortie dangereuse. |
| 8 — Reset refusé | `A01` actif, `EP_CRITIQUE` encore true, `RESET` appuyé | `A05 - RESET REFUSÉ`, message `RESET REFUSÉ - EP_CRITIQUE ACTIF`. |

## 15. Première version livrable attendue

À la fin de ce plan, on doit avoir un **firmware V0.1** avec :

- projet PlatformIO compilable ;
- machine à états fonctionnelle ;
- simulateur série ;
- sorties simulées ;
- alarmes principales ;
- lavage automatique ;
- test lavage ;
- mode maintenance ;
- commandes manuelles ;
- gestion capot ;
- gestion niveau critique ;
- reset alarme ;
- affichage série synthétique ;
- aucune dépendance au matériel reçu.

## État réel V0.1 au 2026-07-07

Le socle firmware V0.1 est déjà présent en mode simulateur et validation host-side.

Modules implémentés :

- `Controller` pour l'orchestration `inputs -> update -> outputs + status` ;
- `Safety` pour les temporisations et états capot / EP_CRITIQUE / incohérence niveau ;
- `WashCycle` pour lavage automatique, durée mini/maxi, rotation résiduelle, anti-redémarrage, retry, test lavage et A04 ;
- `AlarmManager` pour les alarmes principales A01, A02, A03, A04, A05, A11, A12, A13, A14 et A15 ;
- `PersistentStore` pour le mini-journal V1 avec compteurs, dernier événement global et flag A15 actif ;
- `PreferencesPersistentStore` pour la persistance ESP32 NVS/Preferences ;
- simulateur série avec commandes de niveau, capot, modes, boutons et températures ;
- commande simulateur `journal` pour afficher le mini-journal ;
- affichage série synthétique via `DisplayService` ;
- abstraction `InputService` / `OutputService`, sans sorties KC868-A16 activees.

Preuves locales :

- `python -m platformio test -e native` : 54 tests passes sur 54, dont cartographie, profils, defauts I2C A16 et verrou d'impulsions de diagnostic ;
- `python -m platformio run -e kc868_a16_sim -e kc868_a16_hw_safe -e kc868_a16_hw_armed` : trois builds ESP32 A16 reussis ; le profil candidat reste `validated=false`, y compris dans le build arme.

Écarts restants avant de considérer la V0.1 complète :

- validation matérielle KC868-A16 REV.1.6.3 recuee : ordre des entrees confirme (`0x22 = X1-X8`, `0x21 = X9-X16`), sorties MOSFET `Y1-Y16`, polarites et sens logiques restant a mesurer ; le scan initial `0x21/0x22/0x24/0x25` est consigne dans VR-0001 ;
- LCD 2004 I2C réel, RTC DS3231 réelle et sondes DS18B20 réelles ;
- seuils température bas/haut, au-delà de la perte de mesure simulée A11/A12 ;
- temporisation dédiée de reprise UV après EP_CRITIQUE ;
- validation réelle de la persistance NVS sur carte ;
- validation banc/installation des sécurités.

## 16. Validation de la KC868-A16 recue

La carte recue est une **KC868-A16 REV.1.6.3**. Le build `kc868_a16_hw_safe` a confirme le bus interne `GPIO4/GPIO5`, les adresses `0x21`, `0x22`, `0x24` et `0x25`, l'ordre `0x22 = X1-X8` puis `0x21 = X9-X16`, et l'ecriture brute OFF sur les deux banques. Le profil reste non valide jusqu'aux mesures physiques de [VR-0001](../validation/VR-0001-reception-kc868-a16-rev1.6.3.md).

On passe maintenant a **firmware V0.2 materiel**.

Travaux V0.2 :

- tester les entrees `X1-X16` par contact sec vers le GND du bornier ;
- verifier le sens logique et consigner la cartographie ;
- alimenter les groupes `VIN` sans charge et mesurer `Y1-Y16` au boot ;
- commander les sorties MOSFET a vide seulement apres validation du profil ;
- vérifier que toutes les sorties restent OFF au boot ;
- brancher LCD 2004 ;
- tester RTC DS3231 ;
- tester DS18B20 ;
- remplacer progressivement le simulateur par la couche réelle ;
- garder le simulateur activable pour les tests futurs.

## 17. Ordre exact de codage recommandé

Ordre proposé :

1. `firmware/platformio.ini` ;
2. `src/main.cpp` minimal ;
3. `app/Types.h` ;
4. `app/Config.h` ;
5. `app/Controller` skeleton ;
6. `sim/SerialSimulator` ;
7. `StateMachine` boot / auto / maintenance / fault ;
8. `Safety` `EP_CRITIQUE` / capot ;
9. `WashCycle` auto ;
10. retry / lavage inefficace ;
11. manual commands ;
12. test lavage ;
13. `AlarmManager` ;
14. `DisplayService` série ;
15. `InputService` / `OutputService` abstraits ;
16. nettoyage et scénarios de validation.

## 18. Décision de conception importante

Il faut absolument éviter de mettre la logique directement dans `loop()`.

À éviter :

```cpp
void loop() {
  if (digitalRead(...)) {
    digitalWrite(...);
  }
}
```

À faire :

```cpp
void loop() {
  unsigned long now = millis();

  InputsSnapshot inputs = inputService.read(now);
  OutputsCommand outputs = controller.update(inputs, now);

  outputService.apply(outputs);
  displayService.render(controller.status(), inputs, outputs);

  simulator.poll(inputs);
}
```

Comme ça, le firmware reste :

- testable ;
- maintenable ;
- portable ;
- simulable ;
- raccordable à la KC868 plus tard.

## 19. Résumé

La première version doit être :

```text
Firmware V0.1 = logique complète + simulation série + aucune dépendance matériel réel
```

Puis :

```text
Firmware V0.2 = raccordement progressif a la KC868-A16 apres portage HAL et validation des sorties desarmees
```

La priorité n'est pas encore de piloter les vrais relais, mais de verrouiller la logique :

```text
entrées simulées -> décisions sûres -> sorties simulées
```

Une fois cette base propre, l'arrivée du matériel servira à remplacer la simulation par les vraies entrées/sorties, sans réécrire toute la logique.
