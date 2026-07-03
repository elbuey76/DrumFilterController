# Controleur de filtre a tambour pour bassin

Ce depot accueillera la documentation, les schemas, les decisions de conception et le logiciel d'un controleur de filtre a tambour pour bassin. L'objectif est de construire un systeme fiable, maintenable et documente, base autant que possible sur des cartes electroniques du commerce, avec la possibilite de concevoir une carte dediee si le besoin devient justifie.

## Organisation du depot

| Dossier | Role |
| --- | --- |
| `docs/specification/` | Specification fonctionnelle, exigences, contraintes et questions ouvertes. |
| `docs/calculs/` | Notes de calcul de dimensionnement hydraulique, mecanique et electrique. |
| `docs/architecture/` | Architecture systeme, materielle et logicielle, avec diagrammes et emplacements pour visuels. |
| `docs/decisions/` | Journal de decisions de type ADR pour garder l'historique des choix importants. |
| `docs/backlog/` | Backlog de cadrage, prototype, firmware, documentation et validation. |
| `docs/validation/` | Plan de tests, tracabilite exigences/preuves et resultats de validation. |

Les documents de specification et d'architecture sont rediges en Markdown pour pouvoir combiner texte, tableaux, diagrammes Mermaid et images stockees dans les dossiers `assets/`.

## Objectifs du projet

Le controleur doit automatiser le nettoyage d'un filtre a tambour en surveillant le niveau d'eau dans le FAT, puis en declenchant une rotation du tambour et une pompe de rincage lorsque le seuil de lavage est atteint. Il doit aussi proteger l'installation en detectant les defauts, couper des prises auxiliaires en cas de niveau bas, permettre un fonctionnement manuel, gerer des modes maintenance, degrade et test, et redemarrer proprement apres une coupure de courant.

Les objectifs principaux sont :

- automatiser les cycles de lavage du tambour ;
- detecter un seuil de niveau haut declenchant le lavage et un seuil de niveau bas de securite ;
- limiter la consommation d'eau et d'energie ;
- proteger la pompe, le moteur et le bassin en cas de defaut ;
- permettre des modes auto, manuel, maintenance, degrade et test avec comportements explicites ;
- garder une architecture comprehensible et reparable ;
- preparer une evolution vers journalisation, ecran local, supervision reseau et notifications a distance.

## Documents de depart

- [Specification fonctionnelle](docs/specification/README.md)
- [Notes de calcul](docs/calculs/README.md)
- [Architecture](docs/architecture/README.md)
- [Journal de decisions](docs/decisions/README.md)
- [Backlog](docs/backlog/backlog.md)
- [Validation](docs/validation/README.md)

## Prochaine etape

Consolider les cotes physiques du FAT, fixer les seuils hydrauliques sur le report de niveau, figer les modes de fonctionnement et la reprise apres coupure, puis creer les premieres decisions d'architecture concernant la carte de controle, les capteurs et les sorties de puissance.
