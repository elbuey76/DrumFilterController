# ADR-0007 - Choix du contact de capot

## Statut

Acceptee

## Contexte

Le capot transparent du FAT doit etre surveille localement afin d'interdire le lavage automatique, la rotation tambour et le rincage quand il est ouvert. Le capot ouvert est prioritaire sur les demandes operateur et doit etre interprete de facon fail-safe : capot ouvert, fil coupe ou connecteur debranche doivent conduire a l'etat `CAPOT_OUVERT`.

La detection doit rester simple, lisible au depannage et robuste en ambiance humide protegee avec projections possibles.

## Decision

Le capteur retenu pour `CAPOT_OUVERT` est un interrupteur de fin de course industriel OMRCH `ME-8104`, momentane, avec contact `1NO1NC`.

Le montage doit utiliser le contact qui donne le comportement fonctionnel suivant, quel que soit le marquage `NO` ou `NC` au repos du composant :

| Etat reel | Etat electrique attendu |
| --- | --- |
| Capot ferme | Boucle fermee vers l'entree automate |
| Capot ouvert | Boucle ouverte |
| Fil coupe ou connecteur debranche | Boucle ouverte |

Le choix exact entre les bornes `NO` et `NC` doit donc etre valide au multimetre une fois la mecanique montee. L'objectif est `capot ferme = contact ferme`, pas l'usage aveugle d'une borne nommee `NC`.

Le raccordement cible est un contact sec vers `GND` sur une entree digitale du KC868-A32 :

```text
INPUT_Dx  ---- contact capot ---- GND
```

La partie fixe du batiment ou du châssis porte le fin de course. Le capot transparent porte ou actionne une petite patte/came reglable. Le contact doit se fermer seulement lorsque le capot est réellement en position fermee, pas simplement pose ou entrouvert.

## Consequences

- Le schema doit reserver une entree digitale KC868-A32 pour `CAPOT_OUVERT`.
- La validation banc et mecanique doit verifier le sens logique, les bornes a utiliser, l'ouverture en fil coupe, l'ouverture en connecteur debranche et la repetabilite de fermeture.
- Le montage doit proteger le fin de course des projections directes autant que possible, malgre le boitier machine annonce etanche.
- Le firmware conserve le filtrage deja retenu : ouverture confirmee rapidement apres 100 a 500 ms, fermeture stable 1 a 2 s avant reautorisation.
- L'ouverture pendant une action dangereuse reste une alarme bloquante ; l'ouverture hors action dangereuse reste un etat de maintenance informatif.

## Alternatives considerees

- Micro-switch nu type Omron SS : acceptable sur banc ou dans un boitier protege, mais non retenu pour le montage final expose car moins protege contre humidite, poussiere et projections.
- Capteur magnetique reed : possible, mais moins lisible au depannage et plus ambigu selon les versions `NO/NC` annoncees ; risque de leurre par aimant.
- Fin de course industriel a levier/galet : prefere en general si la geometrie du capot l'exige, mais le `ME-8104` est retenu comme reference achetee/candidate V1.
