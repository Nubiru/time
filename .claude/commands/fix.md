# /fix Command

## Purpose

Execute simple, obvious fixes without full Prompt Manager expansion. For trivial corrections that don't require comprehensive task planning.

## Usage

```
/fix [brief description of fix]
```

## Complexity Threshold

**<10/100** - Trivial fixes only (typos, broken links, simple linter errors, formatting issues)

## Examples

```
/fix typo in README line 45 (should be "configuration" not "configration")
/fix broken link in docs/API.md pointing to old domain
/fix linter error in AuthService.ts:23 (missing semicolon)
/fix import path in UserController.ts (should be @/models not ./models)
/fix indentation in package.json
```

## What Happens

### Step 1: Quick Assessment (5 seconds)

1. Identify fix location (file, line number if provided)
2. Validate fix is safe (no breaking changes, no logic changes)
3. Check complexity (must be <10/100 trivial threshold)
4. If complex (>10), escalate to `/task` command

### Step 2: Immediate Execution (10-30 seconds)

1. Read target file
2. Apply fix (search_replace or edit tool)
3. Quick validation (syntax check, no regressions)
4. Conversational confirmation

### Step 3: Validation (5 seconds)

1. Confirm fix applied correctly
2. Check for side effects (imports still valid, references intact)
3. Report success

**Total Time**: <1 minute for most fixes

## Output

**Chat Response** (conversational, no files created):

```
✅ Fixed typo in README.md line 45
   - Changed: "configration" → "configuration"
   - Validated: No side effects
```

**No Task Specification** - Lightweight, immediate execution

## When to Use

**Use `/fix` when**:

- ✅ Typo correction (docs, comments, strings)
- ✅ Broken link fixes (documentation, config files)
- ✅ Simple linter errors (missing semicolon, trailing comma)
- ✅ Import path corrections (straightforward updates)
- ✅ Formatting fixes (indentation, whitespace)
- ✅ Simple variable renames (localized, no breaking changes)
- ✅ Comment updates (clarifications, corrections)

**Don't use `/fix` when**:

- ❌ Logic changes (use `/task` for proper analysis)
- ❌ Multiple files affected (use `/task` for coordination)
- ❌ Breaking changes possible (use `/task` with full testing)
- ❌ Architectural changes (use `/plan` for design)
- ❌ Requires testing (use `/task` with test strategy)
- ❌ Uncertain about impact (use `/task` for safety)

## Decision Logic

```
User requests fix
  ↓
Is complexity < 10? (trivial)
  ↓ YES
Execute immediately with /fix
  ↓ NO
Escalate to /task (full expansion)
```

## Validation Criteria

**Before Executing Fix**:

- [ ] Fix location identified (file + line/section)
- [ ] Change is trivial (typo, link, formatting only)
- [ ] No logic changes involved
- [ ] No breaking changes possible
- [ ] Single file affected (or localized changes)

**After Executing Fix**:

- [ ] Syntax valid (no errors introduced)
- [ ] References still valid (imports, links)
- [ ] Formatting consistent
- [ ] User confirmation received

## Integration

**References**:

- `.cursor/rules` - Command registration (see rules_folder/01-commands/fix.md for details)
- `.claude/agents/prompt-manager.md` - Complexity scoring
- `.context/0.SOP/QUALITY_GATES.md` - Validation standards

**Escalation Path**:

- If complexity > 10 → Redirect to `/task` with explanation
- If uncertainty → Ask user for clarification
- If multiple files → Suggest `/task` for coordination

**Agents**:

- No specific agent invoked (direct execution)
- Escalates to Prompt Manager if complex

## Anti-Patterns

### ❌ Anti-Pattern 1: Logic Changes via /fix

**Problem**: Using `/fix` for business logic changes
**Example**: `/fix user authentication to use JWT instead of sessions`
**Why Bad**: Requires architecture design, testing, migration strategy
**Fix**: Use `/task` with full expansion

### ❌ Anti-Pattern 2: Multi-File Changes via /fix

**Problem**: Using `/fix` for changes spanning multiple files
**Example**: `/fix import paths across entire src/ directory`
**Why Bad**: Requires coordination, validation across modules
**Fix**: Use `/task` for systematic refactoring

### ❌ Anti-Pattern 3: Uncertain Impact

**Problem**: Using `/fix` when unsure of consequences
**Example**: `/fix this error in database migration script`
**Why Bad**: Database changes are high-risk, need full analysis
**Fix**: Use `/task` with proper testing strategy

## Examples

### Example 1: Simple Typo Fix

**User Request**:

```
/fix typo in README.md line 45 - "configration" should be "configuration"
```

**Execution**:

1. Read README.md
2. Find line 45
3. Confirm typo exists
4. Apply fix: `configration` → `configuration`
5. Validate syntax

**Output**:

```
✅ Fixed typo in README.md line 45
   - Changed: "configration" → "configuration"
   - No side effects detected
   - Time: 15 seconds
```

---

### Example 2: Broken Link Fix

**User Request**:

```
/fix broken link in docs/API.md pointing to old-domain.com, should be new-domain.com
```

**Execution**:

1. Read docs/API.md
2. Search for `old-domain.com` references
3. Replace with `new-domain.com`
4. Validate markdown syntax

**Output**:

```
✅ Fixed broken links in docs/API.md
   - Updated: 3 occurrences of old-domain.com → new-domain.com
   - Links now valid
   - Time: 20 seconds
```

---

### Example 3: Linter Error Fix

**User Request**:

```
/fix linter error in src/auth/AuthService.ts:23 - missing semicolon
```

**Execution**:

1. Read src/auth/AuthService.ts
2. Navigate to line 23
3. Confirm missing semicolon
4. Add semicolon
5. Validate TypeScript syntax

**Output**:

```
✅ Fixed linter error in src/auth/AuthService.ts:23
   - Added missing semicolon
   - Syntax validated
   - Time: 12 seconds
```

---

### Example 4: Escalation to /task (Complex)

**User Request**:

```
/fix authentication flow to include 2FA support
```

**Analysis**:

- Complexity: 85/100 (requires architecture, multiple files, testing)
- Not a trivial fix

**Output**:

```
⚠️ This change is too complex for /fix (complexity: 85/100)

**Reason**: Adding 2FA requires:
- Architecture design (session management, token storage)
- Multiple file changes (auth service, middleware, UI components)
- Database schema updates
- Comprehensive testing strategy
- Security review

**Recommendation**: Use `/task` instead:
  `/task Add 2FA support to authentication flow`

This will invoke Prompt Manager for full role-based expansion and proper planning.
```

---

## Success Metrics

**Speed**:

- 90% of fixes complete in <1 minute
- No Prompt Manager overhead (instant execution)

**Accuracy**:

- 95% of fixes require no revision
- No regressions introduced

**Appropriate Use**:

- 100% of `/fix` commands stay within complexity <10
- Escalations handled gracefully (redirect to `/task`)

## Tool Permissions

- **Read**: Target file(s) for fix
- **Write**: Target file(s) only (localized changes)
- **Execute**: Syntax validation (linters, parsers)
- **Escalate**: To `/task` if complexity exceeds threshold
- **Ask**: User for clarification if fix location unclear

## Quick Reference

| Fix Type     | Example                          | Time | Risk |
| ------------ | -------------------------------- | ---- | ---- |
| Typo         | "configration" → "configuration" | 10s  | None |
| Broken link  | old.com → new.com                | 15s  | None |
| Linter error | Add semicolon                    | 10s  | None |
| Import path  | ./models → @/models              | 20s  | Low  |
| Formatting   | Fix indentation                  | 15s  | None |
| Comment      | Update outdated comment          | 10s  | None |

---

**Last Updated**: 2025-10-12
**Command Type**: Quick Execution (no expansion)
**Tool Permissions**: Read (target files), Write (target files), Escalate (to /task if complex)
**Integration**: Direct execution, bypasses Prompt Manager
