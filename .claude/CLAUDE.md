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

- NEVER run ACTIVE git commands (add, commit, push, branch, checkout, merge, rebase) — draft commit messages in `.context/active/commits/NEXT_COMMIT.md` via `/commit`
- ONLY PASSIVE git commands allowed (status, log, diff, show, blame)
- No npm, no node, no TypeScript, no JavaScript build tools
- All source code is C11 and GLSL
- Gabriel handles git init/commit/push himself

---

## Evidence-First Protocol

**BEFORE ANY ACTION:**
1. Read `docs/ROADMAP.md` — 9-phase roadmap with milestones
2. Read `docs/DECISIONS.md` — Architecture Decision Records
3. Read `docs/LEARNING_LOG.md` — Past mistakes and lessons
4. Read `.context/standards/CONVENTIONS.md` — C coding standards

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

**Git**: NEVER run active git commands — draft commit messages in `.context/active/commits/NEXT_COMMIT.md`, only Gabriel executes

**Build**: Use Makefile targets, not raw gcc/emcc commands (unless debugging build issues)

---

## Communication & Coordination

**Human <-> Agent**: Goals -> questions -> approach -> approval -> updates -> escalation

**Commit messages**: Agent drafts -> `.context/active/commits/NEXT_COMMIT.md` (via `/commit` command) -> Gabriel reviews, stages, commits, pushes

**Staging actions**: Agent writes commands -> `.context/active/staging/actions.md` -> Gabriel executes

---

## Context Management

**CLAUDE.md**: Project hub (this file). Read every session.
**ROADMAP.md**: Development phases and progress.
**DECISIONS.md**: Architecture Decision Records.
**CONVENTIONS.md**: C coding standards.
**LEARNING_LOG.md**: Lessons learned.

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

## Specialized Agents

1. **Planning Specialist** — Deep analysis, research, roadmap planning (`.claude/agents/planning-specialist.md`)
2. **Code Reviewer** — C code quality, memory safety, compiler warnings (`.claude/agents/code-reviewer.md`)
3. **Test Writer** — TDD with Unity, test coverage (`.claude/agents/test-writer.md`)

---

**Owner**: Gabriel + Claude
