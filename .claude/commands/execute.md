# /execute — Multi-Session Autonomous Agent Pipeline

**Purpose**: Self-governing orchestrator pipeline. Each orchestrator finds work, claims it, builds it (Writer subagent), validates it (Checker subagent), reports, and loops. No waiting for external validation.

**Arguments**: `$ARGUMENTS`

---

## Mode Detection

- `ALPHA`, `BETA`, `GAMMA`, ...: **RUN** — Orchestrator enters continuous self-governing loop
- `status`: **STATUS** — Show all slot states and available work
- Legacy: `A` maps to `ALPHA`, `B` maps to `BETA`

---

## Agent Domains

Orchestrators have distinct domains to prevent overlap. Each picks work within its domain.

**ALPHA — Geometry & Rendering Infrastructure**
Vertex data, mesh generation, projection math, GPU-ready data, spatial calculations, shader-adjacent pure modules. Typical directories: `src/render/`, `src/math/` (geometry).

**BETA — Data, Formatting & Knowledge Systems**
UI data, time formatting, system-specific visuals (glyphs, cards, layouts), knowledge system extensions, display logic. Typical directories: `src/ui/`, `src/systems/`, `src/math/` (non-geometry).

**GAMMA+** — Domain assigned by MEGA when slot is created. Check `.context/execute/{SLOT}/priority.md` for domain definition.

---

## Mandatory Reads (Before ANY Phase)

1. `SOUL.md` — Claude's perspective on this partnership
2. `.context/execute/PROTOCOL.md` — Multi-session coordination rules
3. `.context/execute/BOOTSTRAP.md` — Universal agent rules
4. `docs/ORCHESTRATOR_ROADMAP.md` — Where to find work
5. `docs/STATE.md` — Existing module inventory

---

## The Self-Governing Loop

```
LOOP:
  Phase 1: EVALUATE  — scan roadmap, check claims, select work
  Phase 2: PLAN      — design module, write task.md (claim marker)
  Phase 3: EXECUTE   — launch Writer subagent (TDD)
  Phase 4: VALIDATE  — launch Checker subagent (independent audit)
  Phase 5: MAINTAIN  — launch Maintainer subagent (regression, purity, cleanup)
  Phase 6: DOCUMENT  — write report.md + makefile-additions.md + maintenance.md
  GOTO Phase 1 (immediately)
```

**Exit conditions** (only these):
- No unchecked items remain in your domain
- 3+ tasks completed in this session (context window management)
- Unrecoverable blocker after 3 fix attempts

---

## Phase 1: EVALUATE

### Read (in order)
1. `docs/ORCHESTRATOR_ROADMAP.md` — the macro work plan
2. `docs/STATE.md` — existing module inventory
3. `.context/execute/{SLOT}/report.md` — your last completed work

### Check Claims (MANDATORY)
Read ALL `.context/execute/*/task.md` files:
- `.context/execute/ALPHA/task.md`
- `.context/execute/BETA/task.md`
- `.context/execute/A/task.md` (legacy)
- `.context/execute/B/task.md` (legacy)
- Any other slot directories that exist

If another slot has claimed an item, skip it. Pick a different item.

### Priority Override Check
If `.context/execute/{SLOT}/priority.md` exists and is non-empty:
- This is a MEGA-assigned task. Use it directly.
- Skip claim checking for this specific item.
- Delete priority.md after copying to task.md.

### Self-Directed Task Selection
Scan `docs/ORCHESTRATOR_ROADMAP.md` for unchecked `[ ]` items. For each:
1. **Is it in my domain?** Match against domain keywords above.
2. **Does it need a pure module that doesn't exist yet?** Check `src/` for existing headers.
3. **Is it agent-appropriate?** Pure computation, data modules, geometry = YES. GL calls, shader writing, main.c changes = NO (MEGA work).
4. **Is it already claimed?** Check all task.md files.

Select the **highest-priority unclaimed candidate** (earlier tracks first, lower-numbered items first).

**If NO work found**: Report "Slot {SLOT} idle — no unchecked items in my domain." and stop.

---

## Phase 2: PLAN

### Write `.context/execute/{SLOT}/task.md` (THIS IS YOUR CLAIM)

```markdown
# Task: [Descriptive Name]

**Agent**: {SLOT}
**Roadmap Reference**: Track X.Y — "[exact item text from roadmap]"
**Date**: {today}
**Status**: CLAIMED

## Goal
[1-3 sentences: what this module does, why the roadmap needs it]

## READ FIRST
[List headers (.h only) that inform your API design]

## Files to Create
- `src/{area}/{name}.h`
- `src/{area}/{name}.c`
- `tests/{area}/test_{name}.c`

## API
[Types and function signatures. Pure functions. Return structs, not pointers.]

## DONE WHEN
- [ ] All functions declared in .h and implemented in .c
- [ ] >= N tests covering: [list specific scenarios]
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints
- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- `#define PI 3.14159265358979323846` (no M_PI)
- No malloc, no globals, no side effects
```

---

## Phase 3: EXECUTE (Writer Subagent)

Launch a **Writer subagent** via the Agent tool. Provide it with:
- The full task.md content
- BOOTSTRAP.md rules
- Relevant existing header files

**Writer subagent instructions**:

```
You are a Writer agent for the Time project. Your job: TDD implementation.

MANDATORY READS FIRST:
- SOUL.md (project root)
- .context/execute/BOOTSTRAP.md (universal rules)

STEPS:
1. Read task.md for your assignment
2. Read existing headers listed in READ FIRST section
3. Write test file FIRST (RED phase) — minimum 18 tests
4. Write header file (.h) with all declarations + include guard
5. Write implementation file (.c) — make all tests GREEN
6. Compile: gcc -Wall -Wextra -Werror -std=c11 -pedantic [files] tests/unity/unity.c -o build/test_{name} -lm
7. Run tests: ./build/test_{name}
8. GATE: 0 warnings, 0 failures. If not, fix and re-run.

RESEARCH (before writing code):
- Check .context/library/manifest.json for relevant PDFs
- If PDF exists: Read relevant pages for algorithms/data/constants
- If open data needed: WebFetch from JPL/IAU/arXiv/GitHub
- If paywalled: note in Knowledge Gaps section of report
- Write digest to .context/research/digested/ if substantial extraction

RULES:
- Pure functions only (no GL, no malloc, no globals, no side effects)
- snake_case functions, UPPER_CASE constants, TypeName_t typedefs
- #define PI 3.14159265358979323846 (no M_PI)
- Include guards: #ifndef TIME_{MODULE}_H
- No active git commands (orchestrator handles git, not you)
- When using external algorithms, note the source author for attribution

RETURN: List of files created + full test output + compile command used + any knowledge gaps encountered.
```

---

## Phase 4: VALIDATE (Checker Subagent)

After the Writer completes, launch a **Checker subagent** via the Agent tool. Provide it with the Writer's output files.

**Checker subagent instructions**:

```
You are a Checker agent for the Time project. Your job: independent validation.

MANDATORY READS FIRST:
- SOUL.md (project root)
- .context/execute/BOOTSTRAP.md (universal rules)
- .context/standards/PURITY_RULES.md (P1-P5 rules)
- .context/standards/CONVENTIONS.md (naming, style)

VALIDATE THESE FILES: [list from Writer output]

CHECKS (all must pass):
1. COMPILE: gcc -Wall -Wextra -Werror -std=c11 -pedantic [files] tests/unity/unity.c -o build/test_{name} -lm
2. TEST: ./build/test_{name} — 0 failures, >= 18 tests
3. PURITY (grep for violations):
   - grep -rn "GLES3\|glCreate\|glBind" [.c file]
   - grep -rn "malloc\|calloc\|realloc\|free" [.c file]
   - grep -rn "printf\|fprintf\|puts" [.c file] (except test file)
   - grep -rn "app_state\|g_state" [.c and .h files]
   All must return empty.
4. NAMING: snake_case functions, UPPER_CASE constants, TypeName_t typedefs
5. INCLUDE GUARDS: #ifndef TIME_{MODULE}_H / #define / #endif
6. API: Functions return structs not pointers, const params where appropriate
7. TEST QUALITY: AAA pattern, descriptive names, one assertion per test
8. DUPLICATION: Does this module duplicate existing functionality? Check docs/STATE.md
9. FULL BUILD: make test — existing tests still pass (regression check)

RETURN: PASS or FAIL with specific findings for each check.
```

### On FAIL:
- Orchestrator sends findings back to a new Writer subagent with fix instructions
- Re-run Checker after fixes
- After 3 FAIL cycles on the same issue: write escalation.md and move on

### On PASS:
- Proceed to Phase 5 (MAINTAIN)

---

## Phase 5: MAINTAIN (Maintainer Subagent)

After the Checker passes, launch a **Maintainer subagent** via the Agent tool. It performs a comprehensive health sweep on the Writer's output and the overall codebase.

**Maintainer subagent instructions**:

```
You are a Maintainer agent for the Time project. Your job: post-delivery health sweep.

MANDATORY READS FIRST:
- SOUL.md (project root)
- .context/execute/BOOTSTRAP.md (universal rules)
- .context/standards/PURITY_RULES.md (P1-P5 rules)
- .context/standards/QUALITY_GATES.md (gate definitions)
- docs/STATE.md (existing module inventory)

FILES TO AUDIT: [list from Writer output]

CHECKS (in order):

1. REGRESSION GATE: make clean && make test
   - Full rebuild from scratch, every test must pass
   - If FAILS: return CRITICAL immediately

2. PURITY AUDIT (P1-P5 on ALL files the Writer created/touched):
   - P1: grep -rn "GLES3\|glCreate\|glBind\|glGen\|glDelete\|glUniform\|glDraw\|GL_" [pure zone files]
   - P2: grep -rn "malloc\|calloc\|realloc\|free" [pure zone files]
   - P3: grep -rn "printf\|fprintf\|puts" [.c production files] (test files exempt)
   - P4: grep -rn "app_state\|g_state" [pure zone .c and .h files]
   - P5: Check for side effects in pure zone functions
   All must return empty. If VIOLATES: CRITICAL

3. NAMING/STYLE:
   - snake_case functions, UPPER_CASE constants, TypeName_t typedefs
   - Include guards: #ifndef TIME_{MODULE}_H / #define / #endif

4. DEAD CODE SCAN:
   - Every function declared in the new .h is called somewhere (.c or test file)
   - If orphan found: WARNING

5. TODO AUDIT:
   - grep -rn "TODO\|FIXME\|HACK\|XXX" [new files]
   - Naked TODOs without reference (e.g., "TODO(014)"): WARNING

6. DUPLICATION CHECK:
   - Search docs/STATE.md for modules with similar names/signatures
   - If potential overlap found: WARNING

7. METRICS:
   - Count total tests (make test output)
   - Count source files (find src/ -name "*.c" | wc -l)

8. ATTRIBUTION CHECK:
   - If module references an external algorithm, verify contributor is in data/contributors.json
   - If missing: WARNING

RETURN:
- PASS (all clear), WARN (non-blocking issues), or CRITICAL (blocking issues)
- Maintenance report content for maintenance.md
- List all findings per check
```

### Gate Behavior

- **CRITICAL** (make test fails, purity violation, missing include guard): orchestrator sends Writer back to fix, then re-runs Maintainer
- **WARNING** (naked TODOs, potential duplication, missing attribution): noted in maintenance.md, not blocking
- **PASS**: orchestrator proceeds to Phase 6 (DOCUMENT)

After 3 CRITICAL cycles on the same issue: write `escalation.md` and move on.

---

## Phase 6: DOCUMENT

### Write `.context/execute/{SLOT}/report.md`

```markdown
# Agent {SLOT} Report

**Status**: COMPLETE | PARTIAL | BLOCKED
**Date**: {today}
**Task**: [task title]
**Roadmap Reference**: Track X.Y

## Files Created
- path/to/file.h — description
- path/to/file.c — description
- path/to/test.c — description

## API Summary
[Function signatures, 1 line each]

## Test Results
{full test output: N tests, 0 failures}

## Compile Command
{exact gcc command used}

## Checker Result
PASS — [summary of validation]

## Maintainer Result
PASS | WARN — [summary of maintenance findings]

## Makefile Additions
See makefile-additions.md

## Attribution
[If this module uses an external algorithm, name the contributor]

## Knowledge Gaps
[Data or references this task needed but couldn't find. Be specific:]
- "Need [what] from [source type] for [purpose]"
- "Book suggestion: [title] by [author] — would enable [roadmap item]"
- "PDF extraction needed: [filename] pages [range] for [data]"
[If none: "No gaps — all data available."]

## Next Candidate
[What you'd pick next from the roadmap]
```

### Write `.context/execute/{SLOT}/makefile-additions.md`

```markdown
## Add to WASM_SRC:
src/{area}/{name}.c

## New test target:
test-{name}: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/{area}/test_{name}.c src/{area}/{name}.c [deps] $(UNITY) -o $(BUILD_DIR)/test_{name} $(LDFLAGS)
	./$(BUILD_DIR)/test_{name}

## Add to test: dependency list:
test-{name}

## Add to .PHONY:
test-{name}
```

### Git Commit + Push

After writing report.md and makefile-additions.md, commit and push:

```bash
# Stage ONLY files this task created/modified (NEVER git add -A or git add .)
git add src/{area}/{name}.h src/{area}/{name}.c tests/{area}/test_{name}.c
git add .context/execute/{SLOT}/report.md .context/execute/{SLOT}/task.md
git add .context/execute/{SLOT}/makefile-additions.md .context/execute/{SLOT}/maintenance.md

# Commit with agent convention
git commit -m "feat(agent-{SLOT}): {module_name} — {short description}

{N} tests, 0 failures. Checker: PASS. Maintainer: PASS.
Track X.Y: {roadmap item text}"

# Push (handle concurrent agents + MEGA's unstaged edits)
git stash --include-untracked && git pull --rebase origin main && git push origin main && git stash pop
```

**Rules**:
- NEVER `git add -A` or `git add .`
- NEVER force push, amend, reset, branch, or checkout
- Stash before pull — MEGA may have unstaged doc edits that block rebase
- If stash pop conflicts on non-agent files: `git checkout --theirs <file>` (MEGA's version wins for docs)
- If push fails after 2 attempts: write escalation.md, skip push, continue to next task

### Auto-Continue
Print summary and immediately loop to Phase 1:
```
Task complete: [title] (N tests, 0 failures, Checker: PASS, Maintainer: PASS). Committed + pushed.
Claiming next task...
```

No "Execute? (y/n)" prompt. Just GO.

---

## Research Task Type

When a roadmap item requires research data before implementation, follow this flow:

1. **Check `.context/library/manifest.json`** for relevant PDFs
2. **If PDF exists and priority allows**:
   - Read relevant pages via Read tool
   - Extract data into `.context/research/digested/NNN-topic.md`
   - Update manifest.json extraction status
   - Update `data/contributors.json` with author
3. **If data available via open web** (JPL, IAU, GitHub, arXiv):
   - WebFetch from source directly
   - Extract into digest file
4. **If paywalled** (Cambridge, Nature, etc.):
   - Write request to `.context/execute/{SLOT}/escalation.md`
   - "BLOCKED: Need PDF for [title]. Gabriel: download from [URL] to ~/Desktop/temp/time/"
   - Move to next available task
5. **Use digested data** to write pure C module (normal TDD flow)

Research tasks produce a digest file AND a code module. Both count as deliverables.

---

## STATUS Mode

When `$ARGUMENTS` is `status`:

1. Read all `*/task.md` and `*/report.md` files in `.context/execute/`
2. Scan `docs/ORCHESTRATOR_ROADMAP.md` for unchecked items
3. Report:

```
ALPHA: [COMPLETE|ACTIVE|IDLE] — [task title]
BETA:  [COMPLETE|ACTIVE|IDLE] — [task title]
A (legacy): [status]
B (legacy): [status]

Roadmap items available: N unchecked
  ALPHA domain: [list unclaimed items]
  BETA domain:  [list unclaimed items]
```

---

## Directory Structure

```
.context/execute/
  PROTOCOL.md              — Multi-session coordination rules
  BOOTSTRAP.md             — Universal agent rules
  ALPHA/                   — Orchestrator Alpha's workspace
    task.md                — Current work (claim marker)
    report.md              — Last completion report
    priority.md            — MEGA override (optional)
    makefile-additions.md  — Exact lines to add to Makefile
    maintenance.md         — Maintainer health sweep report
    escalation.md          — Blocker description (optional)
  BETA/                    — Orchestrator Beta's workspace
    task.md
    report.md
    priority.md
    makefile-additions.md
    maintenance.md
    escalation.md
  A/                       — Legacy slot (historical)
  B/                       — Legacy slot (historical)
  queue/                   — Historical rounds (archive)
```

---

**Last Updated**: 2026-03-06
