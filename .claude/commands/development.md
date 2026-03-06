# /development — Load Current Work State

## Purpose

Load the current development state — what phase we're in, what's done, what's next, what's blocking.

## Usage

```
/development
```

## What Happens

1. Read `docs/ROADMAP.md` — current phase and progress
2. Read `.context/active/right-now/` — any active task specs or issues
3. Read `.context/active/commits/NEXT_COMMIT.md` — pending commit
4. Scan `src/` and `tests/` — what code exists
5. Check build state:
   - Does `make native` work? (check if main.c compiles)
   - Does `make wasm` work? (check if emcc is available)
   - Do tests pass? (check test_runner.c)

## Output

```
# Development State

Phase: [N] - [Phase Name] [IN PROGRESS / COMPLETE]
Progress: [X/Y] criteria met

## What Exists
- Source files: [list]
- Test files: [list]
- Shaders: [list]

## Current Focus
- [Active task or next unchecked roadmap criterion]

## Blockers
- [Any blocking issues]

## Pending
- Commit: [drafted / empty]
```

## When to Use

- Starting a new work session
- Resuming after a break
- Need orientation on where we are

---

**Last Updated**: 2026-03-05
