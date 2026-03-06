# /review Command

## Purpose

Invoke Code Reviewer for C code quality audit with prioritized, actionable feedback.

## Usage

```
/review [file/module]
```

## Examples

```
/review src/math/julian.c
/review src/systems/tzolkin/
/review src/render/shader.c
/review recent changes
```

## What Happens

### Step 1: Read Source Files

Read every file in the review scope. A review that doesn't read the code is a guess.

### Step 2: C-Specific Analysis

Check against:
- Compiler compliance: Would `-Wall -Wextra -Werror -std=c11 -pedantic` pass?
- Memory safety: Buffer overflows, use-after-free, leaks, dangling pointers
- Correctness: Does the algorithm produce correct results?
- Purity: Are math/systems functions side-effect-free?
- Conventions: Naming, includes, guards, comments (per CONVENTIONS.md)
- Performance: Malloc in render loop? Unnecessary copies?
- GLSL: Correct prefixes (u_, a_, v_), shader compilation

### Step 3: Prioritized Feedback

Rank issues by severity:
- **Critical**: Memory safety, undefined behavior, incorrect results
- **Important**: Performance, missing error handling, convention violations
- **Minor**: Style, naming, comment quality

### Step 4: Report

Output feedback with:
```
SEVERITY: [Critical | Important | Minor]
LOCATION: file:line
ISSUE: What's wrong
FIX: How to fix (with C code example)
```

## When to Use

- After implementing a function or module
- Before drafting a commit message
- Periodic code health checks

---

**Last Updated**: 2026-03-05
