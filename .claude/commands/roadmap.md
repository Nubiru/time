# /roadmap — Track Development Progress

**Purpose**: View, update, plan, or audit progress against the 9-phase roadmap.

**Arguments**: `$ARGUMENTS`

---

## Mode Detection

```
Parse $ARGUMENTS for mode keyword:
- "status" or empty   -> MODE: STATUS (default)
- "update"            -> MODE: UPDATE
- "plan"              -> MODE: PLAN
- "audit"             -> MODE: AUDIT
```

---

## Mandatory Reading

Before any mode:
1. Read `docs/ROADMAP.md` — The roadmap itself
2. Read `.claude/CLAUDE.md` — Project identity and rules

---

## Mode 1: STATUS (default)

**When**: No arguments, or `$ARGUMENTS` contains "status".

### Steps

1. Read `docs/ROADMAP.md` — get phase definitions and "done when" criteria
2. Scan the codebase to determine actual progress:
   - `ls src/` — what source directories/files exist?
   - `ls tests/` — what tests exist?
   - `ls shaders/` — what shaders exist?
   - `ls web/` — what web files exist?
   - Check if `Makefile` has all targets
3. For each phase (0-8), check "done when" criteria against actual codebase state
4. Determine current active phase (first phase with incomplete criteria)

### Output

```
# Roadmap Status

Phase 0: Foundation Setup     [COMPLETE / IN PROGRESS / NOT STARTED]
  [x] make native works
  [ ] make wasm works
  ...

Phase 1: Hello WebGL Triangle [NOT STARTED]
  ...

---
Active Phase: 0
Next milestone: make wasm compiles successfully
```

---

## Mode 2: UPDATE

**When**: `$ARGUMENTS` contains "update".

### Steps

1. Run `git log --oneline -20` (passive) — see recent work
2. Run `git diff --stat` — see uncommitted changes
3. Read `docs/ROADMAP.md` — current checkbox state
4. Compare recent commits/changes against roadmap criteria
5. Update checkboxes in `docs/ROADMAP.md` for completed criteria
6. Update "Current Status" section at bottom of ROADMAP.md
7. Update "Last Updated" date

### Output

```
Roadmap updated:
- Phase 0: marked [x] for "make native compiles"
- Phase 0: marked [x] for "git repo initialized"
- Current Status updated to reflect progress
```

---

## Mode 3: PLAN

**When**: `$ARGUMENTS` contains "plan".

### Steps

1. Run STATUS mode first (determine where we are)
2. Identify the active phase and remaining unchecked criteria
3. Break remaining work into concrete, ordered tasks
4. For each task, specify:
   - What to do (action)
   - Which files to create/modify
   - What C concepts are involved
   - Estimated complexity (small/medium/large)
5. Output as a numbered task list

### Output

```
# Phase 0 — Remaining Tasks

Current progress: 3/10 criteria met

## Next Tasks (in order)

1. Install Emscripten SDK
   - Action: Follow emsdk install guide
   - Verification: `emcc --version`
   - Complexity: small (Gabriel does this manually)

2. Verify WASM build
   - Action: Run `make wasm`, fix any emcc errors
   - Files: Makefile (may need flag adjustments)
   - Complexity: medium

3. ...
```

---

## Mode 4: AUDIT

**When**: `$ARGUMENTS` contains "audit".

### Steps

1. Read `docs/ROADMAP.md` — marked progress
2. Read `git log --oneline -30` — recent commit history
3. Cross-reference: do commits match roadmap phases?
4. Check for scope creep:
   - Are there files/features that don't belong to the active phase?
   - Are we working on Phase 3 things while Phase 1 is incomplete?
5. Check for unmarked progress:
   - Did we complete criteria but forget to update the roadmap?
6. Report findings

### Output

```
# Roadmap Audit

## Alignment
- Commits align with Phase 0: YES
- Scope creep detected: NO

## Unmarked Progress
- Phase 0 criterion "Makefile created" appears complete but not checked

## Recommendations
- Update ROADMAP.md checkboxes
- Stay focused on Phase 0 before moving to Phase 1
```

---

## Output Constraints

- Always show phase number + name
- Use checkboxes `[x]` / `[ ]` for criteria
- Keep output concise — status in <30 lines, plan in <50 lines
- Evidence-based: check actual files, don't guess
- Reference specific files when claiming criteria are met

---

## When to Use

**Use `/roadmap` when**:
- Starting a new session (quick orientation)
- Completing a milestone (update progress)
- Planning next work session (what to do next)
- Periodic check (are we on track?)

**Don't use `/roadmap` when**:
- In the middle of implementing something (just keep coding)
- Need detailed task specs (use `/task` instead)

---

**Last Updated**: 2026-03-05
**Command Type**: Progress Tracking
