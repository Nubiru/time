# Agent Bootstrap — Multi-Session Autonomous Pipeline

You are an agent on the Time project. You operate autonomously using a self-governing loop with Writer + Checker subagent separation.

**Your command**: `/execute {YOUR_SLOT}` (ALPHA, BETA, GAMMA, ...)

**This project runs N parallel Claude sessions.** See `.context/execute/PROTOCOL.md` for coordination rules.

---

## How You Work

You are NOT given micro-tasks. You **find your own work** from the orchestrator's roadmap, **claim it** (preventing other sessions from duplicating), **design your own modules** using TDD, **validate independently** via Checker subagent, and **document the results**.

The MEGA session (primary Claude) provides macro-level direction in `docs/ORCHESTRATOR_ROADMAP.md`. You provide micro-level expansion: API design, test planning, implementation, validation, documentation.

---

## Your Self-Governing Loop

```
LOOP:
  1. EVALUATE  — scan roadmap, check all claims, select unclaimed work
  2. PLAN      — write task.md (this IS your claim marker)
  3. EXECUTE   — launch Writer subagent (TDD: tests -> header -> implementation)
  4. VALIDATE  — launch Checker subagent (compile, test, purity, standards)
  5. MAINTAIN  — launch Maintainer subagent (regression, purity, cleanup, metrics)
  6. DOCUMENT  — write report.md + makefile-additions.md + maintenance.md
  GOTO 1 (immediately — no waiting, no asking permission)
```

Exit only when: no work remains, 3+ tasks completed, or unrecoverable blocker.

Full protocol: `.claude/commands/execute.md`

---

## Claim Protocol (MANDATORY)

Before starting any work:
1. Read ALL `.context/execute/*/task.md` files
2. If another slot has claimed the item you want, pick a different item
3. Write your task.md FIRST — this is your claim
4. Only then begin Phase 3

Files are the lock. First to write task.md wins.

---

## Writer + Checker + Maintainer Separation

**Phase 3 (EXECUTE)**: Launch a Writer subagent. It writes tests, header, implementation, compiles, runs tests.

**Phase 4 (VALIDATE)**: Launch a SEPARATE Checker subagent. It independently compiles, runs tests, checks purity (P1-P5), checks naming/style, checks for duplication. Returns PASS/FAIL.

**Phase 5 (MAINTAIN)**: Launch a SEPARATE Maintainer subagent. It runs `make clean && make test` (regression), re-checks purity on all touched files, scans for dead code, naked TODOs, duplication, and missing attribution. Returns PASS/WARN/CRITICAL. See `.context/standards/QUALITY_GATES.md` for gate definitions.

Three perspectives. Writer creates, Checker validates, Maintainer sweeps.

---

## Research Task Type

When a roadmap item requires research before implementation:

1. Check `.context/library/manifest.json` for relevant PDFs
2. If PDF exists: Read relevant pages, extract to `.context/research/digested/NNN-topic.md`
3. If open web data available (JPL, IAU, arXiv, GitHub): WebFetch directly
4. If paywalled: write `.context/execute/{SLOT}/escalation.md` and move to next task
5. Use digested data for normal TDD flow

Research produces a digest file AND a code module. Update `data/contributors.json` for all sources.

---

## Universal Rules

1. **Read SOUL.md + CLAUDE.md first** — Every session, every agent, every time
2. **Read headers only** — When researching existing code, read `.h` files, not `.c` files (unless debugging)
3. **Pure code only** — No GL, no malloc, no globals, no side effects (P1-P5 rules)
4. **Strict compilation** — `gcc -Wall -Wextra -Werror -std=c11 -pedantic` — zero warnings, zero failures
5. **Git after Maintainer PASS** — Commit + push your files in Phase 6. NEVER `git add -A`. NEVER force push, amend, branch, or checkout. See execute.md Phase 6 for exact commands.
6. **Stay in your domain** — ALPHA: geometry/render. BETA: data/UI/systems. Others: check priority.md
7. **No M_PI** — Use `#define PI 3.14159265358979323846`
8. **Minimum 18 tests** per module
9. **Do NOT modify files you didn't create** — unless your task explicitly requires it
10. **Claim before work** — Always write task.md before starting implementation
11. **Attribute contributors** — When using an external algorithm, add the author to `data/contributors.json`
12. **Loop continuously** — After documenting, immediately evaluate next task. No "Execute? (y/n)"

---

## Key Files to Read

| File | Purpose |
|------|---------|
| `SOUL.md` | Claude's perspective on this partnership |
| `.context/execute/PROTOCOL.md` | Multi-session coordination rules |
| `docs/ORCHESTRATOR_ROADMAP.md` | Where to find work |
| `docs/STATE.md` | What modules already exist |
| `.context/standards/PURITY_RULES.md` | P1-P5, S1-S2, T1-T2 rules |
| `.context/standards/CONVENTIONS.md` | Naming, style, patterns |
| `.context/execute/{SLOT}/report.md` | Your last completed task |
| `.context/execute/{SLOT}/priority.md` | MEGA override (if exists) |
| `.context/execute/{SLOT}/maintenance.md` | Maintainer health report |
| `.context/standards/QUALITY_GATES.md` | Gate definitions + thresholds |
| `.context/library/manifest.json` | PDF reference library catalog |
| `docs/checklists/books.md` | Living book catalog + acquisition targets |
| `data/contributors.json` | Attribution database |

---

## Report Format

After Phase 5 (MAINTAIN) passes, write `.context/execute/{SLOT}/report.md`:

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
{full test output}

## Compile Command
{exact command}

## Checker Result
PASS — [validation summary]

## Makefile Additions
See makefile-additions.md

## Attribution
[External algorithm source, if any. Update data/contributors.json.]

## Next Candidate
[What you'd pick next from the roadmap]
```

---

## After Reporting: Auto-Continue

Loop back to Phase 1 (EVALUATE). Print:
```
Task complete: [title] (N tests, 0 failures, Checker: PASS).
Claiming next task...
```

Then immediately scan for the next unclaimed roadmap item. No waiting.
