# Contrôleur de filtre à tambour pour bassin

Ce dépôt accueillera la documentation, les schémas, les décisions de conception et le logiciel d'un contrôleur de filtre à tambour pour bassin. L'objectif est de construire un système fiable, maintenable et documenté, basé autant que possible sur des cartes électroniques du commerce, avec la possibilité de concevoir une carte dédiée si le besoin devient justifié.

## Organisation du dépôt

| Dossier | Rôle |
| --- | --- |
| `docs/specification/` | Spécification fonctionnelle, exigences, contraintes et questions ouvertes. |
| `docs/calculs/` | Notes de calcul de dimensionnement hydraulique, mécanique et électrique. |
| `docs/architecture/` | Architecture système, matérielle et logicielle, avec diagrammes et emplacements pour visuels. |
| `docs/decisions/` | Journal de décisions de type ADR pour garder l'historique des choix importants. |
| `docs/achats/` | Listes de course, nomenclatures et points de commande matériel. |
| `docs/backlog/` | Backlog de cadrage, prototype, firmware, documentation et validation. |
| `docs/validation/` | Plan de tests, traçabilité exigences/preuves et résultats de validation. |

Les documents de spécification et d'architecture sont rédigés en Markdown pour pouvoir combiner texte, tableaux, diagrammes Mermaid et images stockées dans les dossiers `assets/`.

## Objectifs du projet

Le contrôleur doit automatiser le nettoyage d'un filtre à tambour en surveillant le niveau d'eau dans le FAT, puis en déclenchant une rotation du tambour et une pompe de rinçage lorsque le seuil de lavage est atteint. Il doit aussi protéger l'installation en détectant les défauts, couper des prises auxiliaires en cas de niveau bas, permettre un fonctionnement manuel, gérer des modes maintenance, dégradé et test, et redémarrer proprement après une coupure de courant.

Les objectifs principaux sont :

- automatiser les cycles de lavage du tambour ;
- détecter un seuil de niveau haut déclenchant le lavage et un seuil de niveau bas de sécurité ;
- limiter la consommation d'eau et d'énergie ;
- protéger la pompe, le moteur et le bassin en cas de défaut ;
- permettre des modes auto, manuel, maintenance, dégradé et test avec comportements explicites ;
- garder une architecture compréhensible et réparable ;
- préparer une évolution vers journalisation, écran local, supervision réseau et notifications à distance.

## Documents de départ

- [Spécification fonctionnelle](docs/specification/README.md)
- [Notes de calcul](docs/calculs/README.md)
- [Architecture](docs/architecture/README.md)
- [Journal de décisions](docs/decisions/README.md)
- [Achats](docs/achats/README.md)
- [Backlog](docs/backlog/backlog.md)
- [Validation](docs/validation/README.md)

## Prochaine étape

Consolider les cotes physiques du FAT, fixer les seuils hydrauliques sur le report de niveau, figer les modes de fonctionnement et la reprise après coupure, puis créer les premières décisions d'architecture concernant la carte de contrôle, les capteurs et les sorties de puissance.
