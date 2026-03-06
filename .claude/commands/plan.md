# /plan Command

## Purpose

Design architecture for a new module or system, producing an Architecture Decision Record and implementation plan.

## Usage

```
/plan [module/system to design]
```

## Examples

```
/plan orbital mechanics module
/plan WebGL2 render pipeline
/plan zodiac wheel visualization
/plan text rendering system
/plan knowledge system data flow
```

## What Happens

### Step 1: Context Gathering

1. Read `docs/ROADMAP.md` — which phase does this belong to?
2. Read `.context/standards/ARCHITECTURE.md` — how does it fit the layer model?
3. Read relevant existing source files
4. Check `docs/DECISIONS.md` for related past decisions

### Step 2: Architecture Design

1. Define the module's responsibility (single responsibility)
2. Identify layer placement (Math? Systems? Render? Visual?)
3. Design data structures (structs, enums)
4. Define public API (function signatures in `.h` file)
5. Identify dependencies (which other modules needed)
6. Plan memory strategy (stack vs heap)

### Step 3: Test Strategy

Before designing implementation, define:
- What unit tests prove the design works?
- What reference values will be used?
- What edge cases must be handled?

### Step 4: ADR Creation

Write decision record in `docs/DECISIONS.md`:
- What was decided and why
- Alternatives considered
- Consequences (positive and negative)

### Step 5: Implementation Plan

Break into ordered subtasks:
- Each subtask specifies exact file paths
- Each subtask specifies function signatures
- Each subtask has a test criterion

## Output

- ADR entry in `docs/DECISIONS.md`
- Implementation plan in `.context/active/right-now/`

## When to Use

- Designing a new module from scratch
- Making significant architecture choices
- Starting a new roadmap phase

## When NOT to Use

- Simple function additions (use `/task`)
- Bug fixes (use `/fix`)

---

**Last Updated**: 2026-03-05
