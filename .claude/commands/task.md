# /task Command

## Purpose

Expand a brief prompt into a comprehensive task specification for C/WASM development.

## Usage

```
/task [5-10 word prompt]
```

## Examples

```
/task Implement Julian Day conversion function
/task Add Kepler equation solver with tests
/task Create WebGL2 initialization module
/task Write Tzolkin day sign calculator
/task Add camera rotation with mouse input
```

## What Happens

### Step 1: Source Verification

1. Identify files likely affected by the task
2. Read key source files
3. Record current state (what exists, function signatures, includes)

### Step 2: Task Expansion

1. Break the task into:
   - Requirements (what the function/module must do)
   - Implementation approach (algorithm, data structures)
   - File locations (which `.c` and `.h` files)
   - Test strategy (known reference values, edge cases)
   - Dependencies (what other modules are needed)

### Step 3: Task Specification

Output as `.context/active/right-now/TASK_SPEC_[name].md`:

- Human intent (original prompt)
- Requirements (inputs, outputs, constraints)
- Technical approach (algorithm choice, C patterns)
- Subtasks (atomic, ordered)
- Test cases (with expected values and sources)
- Files to create/modify

## When to Use

- Starting a new function or module
- Need to think through an algorithm before coding
- Complex task spanning multiple files

## When NOT to Use

- Simple, obvious operations (use `/fix` instead)
- Already know exactly what to write

---

**Last Updated**: 2026-03-05
