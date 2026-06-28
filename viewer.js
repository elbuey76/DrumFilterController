const DOC_SECTIONS = [
  {
    id: "racine",
    title: "Racine",
    description: "Point d'entree du depot et vision projet.",
    accent: "Vue generale",
    files: [
      { path: "README.md", label: "README", blurb: "Contexte global et organisation du depot." }
    ]
  },
  {
    id: "specification",
    title: "Specification",
    description: "Ce que le systeme doit faire, ses contraintes et ses questions ouvertes.",
    accent: "Exigences",
    files: [
      { path: "docs/specification/README.md", label: "README", blurb: "Structure de la specification." },
      { path: "docs/specification/01-objectifs-et-perimetre.md", label: "Objectifs et perimetre", blurb: "Cible du projet et limites du MVP." },
      { path: "docs/specification/02-exigences-fonctionnelles.md", label: "Exigences fonctionnelles", blurb: "Fonctions attendues, modes et alarmes." },
      { path: "docs/specification/03-securite-et-contraintes.md", label: "Securite et contraintes", blurb: "Protections, repli et environnement." },
      { path: "docs/specification/04-questions-ouvertes.md", label: "Questions ouvertes", blurb: "Points a arbitrer ou a confirmer." },
      { path: "docs/specification/05-donnees-installation-et-fat-existant.md", label: "Donnees installation", blurb: "Geometrie, hydraulique et composants." }
    ]
  },
  {
    id: "architecture",
    title: "Architecture",
    description: "Comment le systeme est structure materiellement et logiciellement.",
    accent: "Structure",
    files: [
      { path: "docs/architecture/README.md", label: "README", blurb: "Sommaire de l'architecture." },
      { path: "docs/architecture/01-vue-ensemble.md", label: "Vue d'ensemble", blurb: "Contexte systeme et principes." },
      { path: "docs/architecture/02-architecture-materielle.md", label: "Architecture materielle", blurb: "Blocs physiques, capteurs et puissance." },
      { path: "docs/architecture/03-architecture-logicielle.md", label: "Architecture logicielle", blurb: "Machine a etats, modules et logique." }
    ]
  },
  {
    id: "calculs",
    title: "Calculs",
    description: "Hypotheses de dimensionnement et raisonnement technique.",
    accent: "Dimensionnement",
    files: [
      { path: "docs/calculs/README.md", label: "README", blurb: "Convention des notes de calcul." },
      { path: "docs/calculs/NC-0001-dimensionnement-pompe-filtration.md", label: "NC-0001 Pompe filtration", blurb: "Debit cible et justification hydraulique." }
    ]
  },
  {
    id: "decisions",
    title: "Decisions",
    description: "Historique des arbitrages importants et rationale.",
    accent: "ADR",
    files: [
      { path: "docs/decisions/README.md", label: "README", blurb: "Convention du journal de decisions." },
      { path: "docs/decisions/ADR-0001-structure-documentation.md", label: "ADR-0001 Documentation", blurb: "Choix du format Markdown." },
      { path: "docs/decisions/ADR-0002-dimensionnement-pompe-filtration-principale.md", label: "ADR-0002 Pompe principale", blurb: "Direction prise pour le dimensionnement." }
    ]
  },
  {
    id: "backlog",
    title: "Backlog",
    description: "Travaux a cadrer, prototype, firmware, validation et V2.",
    accent: "Plan d'action",
    files: [
      { path: "docs/backlog/README.md", label: "README", blurb: "Convention des priorites." },
      { path: "docs/backlog/backlog.md", label: "Backlog projet", blurb: "Actions MVP, V2 et validations." }
    ]
  }
];

const docTree = document.getElementById("doc-tree");
const docMap = document.getElementById("doc-map");
const heroStats = document.getElementById("hero-stats");
const readerTitle = document.getElementById("reader-title");
const readerPath = document.getElementById("reader-path");
const readerStatus = document.getElementById("reader-status");
const readerContent = document.getElementById("reader-content");
const searchInput = document.getElementById("doc-search");
const reloadButton = document.getElementById("reload-doc");
const openSourceButton = document.getElementById("open-source");
const embeddedDocs = globalThis.DOC_CONTENT || {};
let mermaidInitialized = false;

const flatDocs = DOC_SECTIONS.flatMap((section) =>
  section.files.map((file) => ({ ...file, sectionId: section.id, sectionTitle: section.title }))
);

let activePath = "README.md";

function countStats() {
  const mdCount = flatDocs.filter((file) => file.path.endsWith(".md")).length;
  const sectionCount = DOC_SECTIONS.length;
  const v2Refs = flatDocs.filter((file) => /V2/i.test(file.blurb)).length;

  return [
    { value: sectionCount, label: "Sections" },
    { value: mdCount, label: "Docs Markdown" },
    { value: "MVP", label: "Cible principale" },
    { value: "V2", label: "Extensions distantes" }
  ];
}

function renderStats() {
  heroStats.innerHTML = countStats()
    .map(
      (stat) => `
        <article class="stat-card">
          <strong>${escapeHtml(String(stat.value))}</strong>
          <span>${escapeHtml(stat.label)}</span>
        </article>
      `
    )
    .join("");
}

function renderTree(filter = "") {
  const query = filter.trim().toLowerCase();

  docTree.innerHTML = DOC_SECTIONS.map((section) => {
    const items = section.files.filter((file) => {
      if (!query) return true;
      return `${file.label} ${file.path} ${file.blurb}`.toLowerCase().includes(query);
    });

    if (!items.length) return "";

    return `
      <details class="tree-group" ${section.id === "racine" || query ? "open" : ""}>
        <summary>
          <span>${escapeHtml(section.title)}</span>
          <span>${items.length}</span>
        </summary>
        <div class="tree-items">
          ${items
            .map(
              (file) => `
                <button class="doc-link ${file.path === activePath ? "active" : ""}" type="button" data-open="${escapeHtml(
                  file.path
                )}">
                  <strong>${escapeHtml(file.label)}</strong>
                  <span class="doc-item-meta">${escapeHtml(file.blurb)}</span>
                </button>
              `
            )
            .join("")}
        </div>
      </details>
    `;
  }).join("");
}

function renderMap() {
  docMap.innerHTML = DOC_SECTIONS.map((section) => {
    const topFiles = section.files.slice(0, 4);
    return `
      <article class="map-card">
        <div>
          <span class="tag">${escapeHtml(section.accent)}</span>
          <h3>${escapeHtml(section.title)}</h3>
          <p>${escapeHtml(section.description)}</p>
        </div>
        <div class="map-links">
          ${topFiles
            .map(
              (file) => `
                <button class="mini-link" type="button" data-open="${escapeHtml(file.path)}">
                  ${escapeHtml(file.label)}
                </button>
              `
            )
            .join("")}
        </div>
      </article>
    `;
  }).join("");
}

function setStatus(message, type = "") {
  readerStatus.className = `reader-status${type ? ` ${type}` : ""}`;
  readerStatus.textContent = message;
}

async function openDocument(path) {
  activePath = path;
  readerTitle.textContent = docLabelForPath(path);
  readerPath.textContent = path;
  renderTree(searchInput.value);
  setStatus(`Chargement de ${path}...`);

  try {
    let raw = embeddedDocs[path];

    if (typeof raw !== "string") {
      const response = await fetch(path);
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}`);
      }
      raw = await response.text();
    }

    readerContent.innerHTML = renderMarkdown(raw, path);
    readerTitle.textContent = extractTitle(raw) || docLabelForPath(path);
    renderMermaidDiagrams();
    setStatus(`Document charge : ${path}`);
  } catch (error) {
    readerContent.innerHTML = `
      <h2>Chargement impossible</h2>
      <p>Le navigateur n'a pas pu lire <code>${escapeHtml(path)}</code>.</p>
      <p>La source embarquee est absente et la lecture directe du fichier a aussi echoue.</p>
      <pre><code>python -m http.server 8000
# puis ouvrir http://localhost:8000/</code></pre>
    `;
    setStatus(
      `Impossible de charger ${path}. Lance un serveur statique local si besoin.`,
      "error"
    );
    console.error(error);
  }
}

function docLabelForPath(path) {
  return flatDocs.find((file) => file.path === path)?.label || path.split("/").pop() || path;
}

function extractTitle(markdown) {
  const match = markdown.match(/^#\s+(.+)$/m);
  return match ? match[1].trim() : "";
}

function renderMarkdown(markdown, sourcePath) {
  const lines = markdown.replace(/\r\n/g, "\n").split("\n");
  const blocks = [];
  let i = 0;

  while (i < lines.length) {
    const line = lines[i];

    if (!line.trim()) {
      i += 1;
      continue;
    }

    if (line.startsWith("```")) {
      const lang = line.slice(3).trim();
      const codeLines = [];
      i += 1;
      while (i < lines.length && !lines[i].startsWith("```")) {
        codeLines.push(lines[i]);
        i += 1;
      }
      i += 1;
      const code = codeLines.join("\n");
      if (lang.toLowerCase() === "mermaid") {
        const encoded = encodeURIComponent(code);
        blocks.push(`
          <div class="mermaid-placeholder" data-mermaid="${encoded}">
            <strong>Diagramme Mermaid</strong>
            <div class="diagram-host"></div>
            <pre><code>${escapeHtml(code)}</code></pre>
          </div>
        `);
      } else {
        blocks.push(`<pre><code>${escapeHtml(code)}</code></pre>`);
      }
      continue;
    }

    if (/^#{1,6}\s+/.test(line)) {
      const level = line.match(/^#+/)[0].length;
      const content = line.replace(/^#{1,6}\s+/, "");
      blocks.push(`<h${level}>${renderInline(content, sourcePath)}</h${level}>`);
      i += 1;
      continue;
    }

    if (/^>\s?/.test(line)) {
      const quoteLines = [];
      while (i < lines.length && /^>\s?/.test(lines[i])) {
        quoteLines.push(lines[i].replace(/^>\s?/, ""));
        i += 1;
      }
      blocks.push(`<blockquote>${quoteLines.map((item) => renderInline(item, sourcePath)).join("<br>")}</blockquote>`);
      continue;
    }

    if (isTableStart(lines, i)) {
      const tableLines = [];
      tableLines.push(lines[i], lines[i + 1]);
      i += 2;
      while (i < lines.length && /^\|.*\|$/.test(lines[i].trim())) {
        tableLines.push(lines[i]);
        i += 1;
      }
      blocks.push(renderTable(tableLines, sourcePath));
      continue;
    }

    if (/^\s*([-*])\s+/.test(line)) {
      const items = [];
      while (i < lines.length && /^\s*([-*])\s+/.test(lines[i])) {
        items.push(lines[i].replace(/^\s*[-*]\s+/, ""));
        i += 1;
      }
      blocks.push(`<ul>${items.map((item) => `<li>${renderInline(item, sourcePath)}</li>`).join("")}</ul>`);
      continue;
    }

    if (/^\s*\d+\.\s+/.test(line)) {
      const items = [];
      while (i < lines.length && /^\s*\d+\.\s+/.test(lines[i])) {
        items.push(lines[i].replace(/^\s*\d+\.\s+/, ""));
        i += 1;
      }
      blocks.push(`<ol>${items.map((item) => `<li>${renderInline(item, sourcePath)}</li>`).join("")}</ol>`);
      continue;
    }

    const paragraph = [];
    while (
      i < lines.length &&
      lines[i].trim() &&
      !/^#{1,6}\s+/.test(lines[i]) &&
      !lines[i].startsWith("```") &&
      !/^>\s?/.test(lines[i]) &&
      !/^\s*([-*])\s+/.test(lines[i]) &&
      !/^\s*\d+\.\s+/.test(lines[i]) &&
      !isTableStart(lines, i)
    ) {
      paragraph.push(lines[i].trim());
      i += 1;
    }

    blocks.push(`<p>${renderInline(paragraph.join(" "), sourcePath)}</p>`);
  }

  return blocks.join("\n");
}

function isTableStart(lines, index) {
  const current = lines[index]?.trim();
  const next = lines[index + 1]?.trim();
  return Boolean(
    current &&
      next &&
      /^\|.*\|$/.test(current) &&
      /^\|\s*[-:| ]+\|$/.test(next)
  );
}

function renderTable(lines, sourcePath) {
  const rows = lines.map((line) =>
    line
      .trim()
      .replace(/^\|/, "")
      .replace(/\|$/, "")
      .split("|")
      .map((cell) => cell.trim())
  );

  const header = rows[0];
  const body = rows.slice(2);

  return `
    <table>
      <thead>
        <tr>${header.map((cell) => `<th>${renderInline(cell, sourcePath)}</th>`).join("")}</tr>
      </thead>
      <tbody>
        ${body
          .map((row) => `<tr>${row.map((cell) => `<td>${renderInline(cell, sourcePath)}</td>`).join("")}</tr>`)
          .join("")}
      </tbody>
    </table>
  `;
}

function renderInline(text, sourcePath) {
  let output = escapeHtml(text);

  output = output.replace(/`([^`]+)`/g, "<code>$1</code>");
  output = output.replace(/\*\*([^*]+)\*\*/g, "<strong>$1</strong>");
  output = output.replace(/\*([^*]+)\*/g, "<em>$1</em>");
  output = output.replace(/\[([^\]]+)\]\(([^)]+)\)/g, (_, label, target) => {
    const resolved = resolveDocPath(sourcePath, target);
    if (resolved && resolved.toLowerCase().endsWith(".md")) {
      return `<button class="inline-doc-link" type="button" data-open="${escapeHtml(resolved)}">${escapeHtml(label)}</button>`;
    }
    return `<a href="${escapeHtml(target)}">${escapeHtml(label)}</a>`;
  });

  return output;
}

function resolveDocPath(sourcePath, target) {
  if (!target || /^([a-z]+:)?\/\//i.test(target) || target.startsWith("#")) {
    return target;
  }

  if (target.startsWith("/")) {
    return target.slice(1);
  }

  const sourceParts = (sourcePath || "").split("/");
  sourceParts.pop();

  for (const segment of target.split("/")) {
    if (!segment || segment === ".") continue;
    if (segment === "..") {
      sourceParts.pop();
      continue;
    }
    sourceParts.push(segment);
  }

  return sourceParts.join("/");
}

function escapeHtml(value) {
  return value
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#39;");
}

async function renderMermaidDiagrams() {
  const mermaid = globalThis.mermaid;
  if (!mermaid || !readerContent || typeof readerContent.querySelectorAll !== "function") {
    return;
  }

  if (!mermaidInitialized) {
    mermaid.initialize({
      startOnLoad: false,
      securityLevel: "loose",
      theme: "default"
    });
    mermaidInitialized = true;
  }

  const nodes = readerContent.querySelectorAll("[data-mermaid]");

  for (let index = 0; index < nodes.length; index += 1) {
    const container = nodes[index];
    const host = container.querySelector(".diagram-host");
    const encoded = container.getAttribute("data-mermaid");
    const source = encoded ? decodeURIComponent(encoded) : "";

    if (!host || !source) continue;

    try {
      const diagramId = `mermaid-diagram-${Date.now()}-${index}`;
      const result = await mermaid.render(diagramId, source);
      host.innerHTML = result.svg;
      container.classList.add("is-rendered");
    } catch (error) {
      console.error("Mermaid render failed", error);
    }
  }
}

document.addEventListener("click", (event) => {
  const button = event.target.closest("[data-open]");
  if (!button) return;
  const path = button.getAttribute("data-open");
  if (path) {
    openDocument(path);
  }
});

searchInput.addEventListener("input", () => {
  renderTree(searchInput.value);
});

reloadButton.addEventListener("click", () => {
  openDocument(activePath);
});

openSourceButton.addEventListener("click", async () => {
  try {
    await navigator.clipboard.writeText(activePath);
    setStatus(`Chemin copie : ${activePath}`);
  } catch {
    setStatus(`Chemin courant : ${activePath}`);
  }
});

window.addEventListener("mermaid-ready", () => {
  renderMermaidDiagrams();
});

renderStats();
renderTree();
renderMap();
openDocument(activePath);
