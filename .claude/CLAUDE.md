# CLAUDE.md — Time Project

**Version**: 1.0 **Last Updated**: 2026-03-05

---

## Project Identity

**Time** is a visual, interactive, three-dimensional digital artwork that displays time through multiple cultural and astronomical systems. It renders astronomy, astrology, Mayan Kin/Tzolkin, I Ching, Human Design, Chinese calendar, and sidereal time as layered visual experiences.

**Tech Stack**: C11 + Emscripten/WebAssembly + raw WebGL2 + hand-written GLSL shaders. No frameworks, no engines.

**Philosophy**: Every pixel earned. No abstractions we don't understand. The code is part of the artwork.

---

## Environment Architecture

**Development (Linux - HERE)**: Write code, compile natively with gcc, run tests. Compile to WASM with emcc. Serve locally with Python HTTP server.

**Deployment (Web)**: WASM binary served from static hosting. Runs in any modern browser with WebGL2 support.

**No database. No backend services. No Node.js. No npm.**

**Workflow**: Write code HERE -> Compile -> Test natively -> Build WASM -> Test in browser -> Commit/push to git

---

## Critical Rules

- No npm, no node, no TypeScript, no JavaScript build tools
- All source code is C11 and GLSL
- No branching — all work on `main`

### Git Authority

**MEGA session**: Passive git only (status, log, diff, show, blame). Drafts commits in `.context/active/commits/NEXT_COMMIT.md` via `/commit` for Gabriel to review.

**Orchestrator sessions (ALPHA, BETA, ...)**: Authorized to commit and push after Phase 5 MAINTAIN passes. Rules:
- Only commit files the agent created or modified in the current task
- `git add` specific files by name — NEVER `git add -A` or `git add .`
- Commit message format: `feat(agent-{SLOT}): {module} — {description}`
- Push to `origin main` after commit
- If push fails (other agent pushed first): `git pull --rebase origin main && git push origin main`
- NEVER force push, NEVER amend, NEVER reset, NEVER branch, NEVER checkout
- Commit happens in Phase 6 DOCUMENT, after Maintainer PASS

---

## Evidence-First Protocol

**BEFORE ANY ACTION:**
1. Read `SOUL.md` — Claude's perspective on this partnership (equal weight to CLAUDE.md)
2. Read `docs/ROADMAP.md` — 9-phase roadmap with milestones
3. Read `docs/DECISIONS.md` — Architecture Decision Records
4. Read `docs/LEARNING_LOG.md` — Past mistakes and lessons
5. Read `.context/standards/CONVENTIONS.md` — C coding standards

**MEGA SESSION — ALSO DO ON STARTUP:**
6. Scan `~/Desktop/temp/time/inbox/` for uncatalogued books
7. If inbox has files: catalog in `docs/checklists/books.md` + `.context/library/manifest.json`, move to `new/`

**Core Rules**:
- Check sources, cite file paths
- State confidence levels explicitly
- Distinguish facts from assumptions
- Never state assumptions as facts

---

## Knowledge System Layers

Seven time systems, each in its own domain under `src/systems/`:

| System | Directory | Key Data |
|--------|-----------|----------|
| Astronomy | `src/systems/astronomy/` | JPL orbital elements, Kepler's equation |
| Astrology | `src/systems/astrology/` | Zodiac divisions, planetary dignities |
| Tzolkin (Mayan) | `src/systems/tzolkin/` | 20 day signs x 13 tones, GMT correlation 584283 |
| I Ching | `src/systems/iching/` | 64 hexagrams, King Wen sequence |
| Chinese Calendar | `src/systems/chinese/` | Heavenly Stems, Earthly Branches, lunar phases |
| Human Design | `src/systems/human_design/` | Gates, channels, types, authority |
| Gregorian | `src/systems/gregorian/` | Standard calendar, Julian Day conversion |

Research files: `docs/knowledge-systems/`, `.context/research/`

---

## Build Commands

| Command | What it does |
|---------|-------------|
| `make native` | Compile with gcc for local testing |
| `make wasm` | Compile with emcc for WebAssembly |
| `make test` | Compile and run tests with gcc + Unity |
| `make clean` | Remove build artifacts |
| `make serve` | Start local HTTP server on :8080 |

**Scripts**: `scripts/build.sh`, `scripts/serve.sh`, `scripts/clean.sh`, `scripts/build-tests.sh`

---

## C Development Context

Gabriel's first C project. Experience in JS/TS. Linux host with 4GB RAM, i5.

**Compiler flags**: `-Wall -Wextra -Werror -std=c11 -pedantic`

**Naming conventions**:
- `snake_case` for functions and variables
- `UPPER_CASE` for constants and macros
- `TypeName_t` for typedefs

**Testing**: Unity test framework (vendored in `tests/unity/`)

**Anti-patterns to watch**:
- Buffer overflows (always check array bounds)
- Use-after-free (clear pointers after free)
- Uninitialized variables (compiler flags catch most)
- Memory leaks in non-render paths (no malloc in render loop)
- Forgetting null terminators on strings

---

## Data Sources

| Source | Usage | Notes |
|--------|-------|-------|
| JPL Orbital Elements | Planetary positions | Keplerian approximation for inner planets |
| GMT Correlation 584283 | Mayan calendar | Most widely accepted correlation constant |
| King Wen Sequence | I Ching hexagram ordering | Traditional 64-hexagram arrangement |
| IAU GST Algorithm | Greenwich Sidereal Time | Standard formula from Meeus |

**Attribution**: When using external knowledge (algorithms, formulas, calendar systems), add the contributor to `data/contributors.json`. Every human whose work feeds this codebase is honored.

---

## Partnership Principle

**I am an expert who double checks. Gabriel and I both strive for accuracy.**

**Core Behaviors**: Double-check before claiming "fixed", read full files after changes, test mentally, accuracy over speed, verify twice

**Anti-Patterns**: Claiming "fixed" without verification, assuming without checking, rushing to satisfy

**If uncertain: SAY "Let me verify this first" and actually do it.**

---

## Version Policy

**All files in Time remain at version 1.0 until first deployable build is published.**

This applies to all documentation, code, test, and configuration files.

**Rationale**: Version increments signify published releases. Pre-launch changes are iterative improvements within v1.0 development phase.

---

## Scientific Method

**7 Stages**: Observation -> Question -> Hypothesis -> Prediction (tests first) -> Experimentation (make tests pass) -> Analysis -> Conclusion

**TDD Integration**: Prediction=write tests, Experimentation=pass tests, Analysis=validate

**Reference**: `.context/standards/SCIENTIFIC_METHOD.md`

---

## Quality Gates

| Gate | Criteria |
|------|----------|
| Compilation | C compiles with zero warnings (`-Wall -Wextra -Werror`) |
| Tests | All Unity tests pass |
| WASM Build | `make wasm` succeeds without errors |
| Performance | 60fps in browser (measured, not assumed) |
| Memory | No leaks in render loop (valgrind clean on native) |

---

## Terminal Safeguards

**Git**: See "Git Authority" above. MEGA drafts only. Orchestrators commit+push after Maintainer PASS.

**Build**: Use Makefile targets, not raw gcc/emcc commands (unless debugging build issues)

---

## Communication & Coordination

**Human <-> Agent**: Goals -> questions -> approach -> approval -> updates -> escalation

**Commit messages**: MEGA drafts in `.context/active/commits/NEXT_COMMIT.md` for Gabriel. Orchestrators commit directly after Maintainer PASS.

**Staging actions**: Agent writes commands -> `.context/active/staging/actions.md` -> Gabriel executes

---

## Context Management

**CLAUDE.md**: Project hub (this file). Read every session.
**SOUL.md**: Claude's own perspective. Read every session alongside CLAUDE.md.
**ROADMAP.md**: Development phases and progress.
**DECISIONS.md**: Architecture Decision Records.
**CONVENTIONS.md**: C coding standards.
**LEARNING_LOG.md**: Lessons learned.
**METRICS.md**: Quantitative project snapshot (run `/refresh` to update).
**STATE.md**: Pure vs stateful module inventory.
**USER_CONTROLS.md**: Keyboard/mouse controls reference.
**VISUAL_DESIGN.md**: UI/UX architecture, view modes, screen layout.
**ENVIRONMENT.md**: Reproducible host setup checklist.
**books.md**: Living catalog of all reference books, PDFs, acquisition targets. MEGA maintains.
**reference-library.md**: Full 78-file catalog with extraction priority and status.
**manifest.json**: `.context/library/manifest.json` — structured PDF catalog for agent access.
**contributors.json**: `data/contributors.json` — attribution database for every human whose work feeds this codebase.

**Verbosity**: KISS. Simple, terse, actionable. No fluff.

---

## Development Context Structure

```
.context/
  active/
    commits/     NEXT_COMMIT.md
    staging/     actions.md
    right-now/   Current issues, task specs
  standards/     CONVENTIONS.md, ARCHITECTURE.md, SCIENTIFIC_METHOD.md, etc.
  research/      Project genesis, literature, research notes
  archive/       Historical content
```

---

## Multi-Session Architecture

This project runs N parallel Claude Code sessions simultaneously. See `.context/execute/PROTOCOL.md` for coordination rules.

Each session reads CLAUDE.md but operates according to its role layer below.

### Role Layers & Priority Horizons

```
MEGA (primary session — Gabriel's direct partner)
  Horizon: LONG-TERM first, mid-term second, short-term least
  Focus: Vision, roadmap, architecture, knowledge pipeline, books.md,
         SOUL.md, integration, mentoring, monitoring agents
  Does NOT: Write code modules (delegates to agents)
  Priority: Future roadmap > agent coordination > mid-term planning
            > short-term fixes. Writing code yourself is the LAST resort.
  Manages: docs/ORCHESTRATOR_ROADMAP.md, docs/checklists/books.md,
           .context/library/manifest.json, SOUL.md, CLAUDE.md
  Reads: Agent report.md files -> translates Knowledge Gaps into
         books.md entries + roadmap growth

ALPHA (orchestrator — geometry & rendering infrastructure)
  Horizon: MID-TERM — current + next phase rendering needs
  Focus: Vertex data, mesh generation, projection math, GPU-ready data,
         spatial calculations, star catalogs, constellation data
  Concern: Accuracy of astronomical data, geometric correctness,
           render pipeline readiness
  Directories: src/render/, src/math/ (geometry)

BETA (orchestrator — data, formatting & knowledge systems)
  Horizon: MID-TERM — current + next phase knowledge system needs
  Focus: UI data, time formatting, system-specific visuals, knowledge
         system extensions, display logic, calendar algorithms
  Concern: Cultural accuracy, data completeness, format correctness
  Directories: src/ui/, src/systems/, src/math/ (non-geometry)

GAMMA+ (orchestrator — domain assigned by MEGA)
  Horizon: MID-TERM — as defined in priority.md
  Focus: Whatever MEGA assigns or what roadmap offers
  Check: .context/execute/{SLOT}/priority.md for domain definition

Writer subagent (launched by orchestrator)
  Horizon: SHORT-TERM — this task only
  Focus: TDD implementation. Tests -> header -> code -> compile -> pass.
  Concern: Code quality, test coverage, compilation, purity

Checker subagent (launched by orchestrator)
  Horizon: SHORT-TERM — this task only
  Focus: Independent validation of Writer output
  Concern: Standards compliance, purity rules, naming, duplication

Maintainer subagent (launched by orchestrator)
  Horizon: SHORT-TERM — this task + regression
  Focus: Health sweep, regression gate, metrics, attribution
  Concern: Codebase integrity, dead code, naked TODOs, attribution
```

Each layer reads CLAUDE.md but focuses on its own horizon. MEGA never gets pulled into short-term implementation details. Agents never make long-term architectural decisions.

---

## Specialized Agents

1. **Planning Specialist** — Deep analysis, research, roadmap planning (`.claude/agents/planning-specialist.md`)
2. **Code Reviewer** — C code quality, memory safety, compiler warnings (`.claude/agents/code-reviewer.md`)
3. **Test Writer** — TDD with Unity, test coverage (`.claude/agents/test-writer.md`)

---

**Owner**: Gabriel + Claude
