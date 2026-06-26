# ADR-0001 - Structurer la documentation projet en Markdown

## Statut

Acceptée

## Contexte

Le projet doit documenter des spécifications, des choix d'architecture, des décisions et un backlog. La documentation doit rester simple à modifier tout en permettant l'ajout de visuels.

## Décision

Utiliser une arborescence `docs/` composée de fichiers Markdown pour la spécification, l'architecture, le journal de décisions et le backlog. Les visuels seront stockés dans des sous-dossiers `assets/` et les schémas pourront être rédigés directement en Mermaid lorsque c'est pertinent.

## Conséquences

- La documentation est lisible directement dans le dépôt.
- Les diagrammes simples peuvent vivre à côté du texte.
- Les images, croquis et exports restent versionnés.
- Une convention devra être maintenue pour éviter la dispersion des documents.

## Alternatives considérées

- Un document unique : plus simple au départ, mais moins maintenable.
- Un wiki externe : pratique pour la lecture, mais moins proche du code et de l'historique Git.
