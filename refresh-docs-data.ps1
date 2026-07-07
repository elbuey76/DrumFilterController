$ErrorActionPreference = "Stop"

$paths = @(
  "README.md",
  "docs/specification/README.md",
  "docs/specification/01-objectifs-et-perimetre.md",
  "docs/specification/02-exigences-fonctionnelles.md",
  "docs/specification/03-securite-et-contraintes.md",
  "docs/specification/04-questions-ouvertes.md",
  "docs/specification/05-donnees-installation-et-fat-existant.md",
  "docs/architecture/README.md",
  "docs/architecture/01-vue-ensemble.md",
  "docs/architecture/02-architecture-materielle.md",
  "docs/architecture/03-architecture-logicielle.md",
  "docs/architecture/04-table-entrees-sorties.md",
  "docs/calculs/README.md",
  "docs/calculs/NC-0001-dimensionnement-pompe-filtration.md",
  "docs/calculs/NC-0002-dimensionnement-motorisation-tambour.md",
  "docs/calculs/NC-0003-dimensionnement-pompe-rincage.md",
  "docs/calculs/NC-0004-dimensionnement-eclairage-exterieur.md",
  "docs/decisions/README.md",
  "docs/decisions/_template.md",
  "docs/decisions/ADR-0001-structure-documentation.md",
  "docs/decisions/ADR-0002-dimensionnement-pompe-filtration-principale.md",
  "docs/decisions/ADR-0003-choix-plateforme-controle-v1.md",
  "docs/decisions/ADR-0004-architecture-electrique-v1.md",
  "docs/decisions/ADR-0005-choix-rtc-heure-fiable.md",
  "docs/decisions/ADR-0006-choix-sondes-temperature.md",
  "docs/decisions/ADR-0007-choix-contact-capot.md",
  "docs/decisions/ADR-0008-choix-voyants-ihm-locale.md",
  "docs/decisions/ADR-0009-choix-selecteur-mode-auto-maintenance.md",
  "docs/decisions/ADR-0010-choix-boutons-poussoirs-ihm-locale.md",
  "docs/decisions/ADR-0011-strategie-validation-materielle-v1.md",
  "docs/achats/README.md",
  "docs/achats/liste-course-materiel-v1.md",
  "docs/achats/commandes-2026-07-06.md",
  "docs/backlog/README.md",
  "docs/backlog/backlog.md",
  "docs/firmware/README.md",
  "docs/firmware/plan-firmware-v0.1.md",
  "docs/validation/README.md",
  "docs/validation/plan-tests-exigences.md",
  "docs/validation/checklist-go-no-go-materiel-v1.md"
)

$map = [ordered]@{}

foreach ($path in $paths) {
  $absolutePath = Join-Path (Get-Location) $path
  $map[$path] = [System.IO.File]::ReadAllText($absolutePath, [System.Text.Encoding]::UTF8)
}

$json = $map | ConvertTo-Json -Depth 3 -Compress
$content = "window.DOC_CONTENT = $json;`n"
[System.IO.File]::WriteAllText((Join-Path (Get-Location) "docs-data.js"), $content, [System.Text.Encoding]::UTF8)

Write-Host "docs-data.js mis à jour."
