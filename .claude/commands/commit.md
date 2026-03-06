# /commit — Draft or Archive Commit Messages

**Purpose**: Draft professional commit messages for Gabriel to execute, or archive committed messages.

**Arguments**: `$ARGUMENTS`

---

## Git Safety — OFF-LIMITS Commands

**NEVER run**: `git add`, `git commit`, `git push`, `git branch`, `git checkout`, `git merge`, `git rebase`

**Allowed (passive only)**: `git status`, `git diff`, `git log`, `git show`, `git blame`

The agent produces the message — Gabriel executes the commit.

---

## Mode Detection

```
1. Read `.context/active/commits/NEXT_COMMIT.md`
2. Read `git log --oneline -20`
3. If NEXT_COMMIT.md has a non-empty Message section:
   a. Extract the subject line (first line of the ``` block)
   b. Check if that subject line appears in `git log --oneline -20`
   c. If MATCH found -> Mode 2: ARCHIVE
   d. If NO match -> Message exists but not yet committed. Ask Gabriel: draft a new one or keep?
4. If NEXT_COMMIT.md is empty/absent/template-only -> Mode 1: DRAFT
```

---

## Mode 1: DRAFT

**When**: NEXT_COMMIT.md is empty or absent, and there are changes to describe.

### Steps

**Step 1: Gather context**

Run passive git commands:
- `git status` — see staged/unstaged/untracked files
- `git diff --stat` — summary of changes
- `git diff` — full diff (for small changesets) or `git diff --stat` + targeted reads (for large ones)
- `git log --oneline -10` — recent commit style reference

**Step 2: Draft the commit message**

Write to `.context/active/commits/NEXT_COMMIT.md`:

```markdown
# Next Commit

## Message

```
type(scope): subject line (imperative, <72 chars)

- Bullet 1: what changed and why
- Bullet 2: what changed and why

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>
```

## Files

```
git add \
  path/to/file1.c \
  path/to/file2.h
```
```

**Commit message conventions**:
- **Types**: `fix`, `feat`, `refactor`, `perf`, `docs`, `test`, `chore`
- **Scopes**: `math`, `render`, `astronomy`, `astrology`, `tzolkin`, `iching`, `chinese`, `human-design`, `build`, `docs`, `core`
- **Subject**: imperative mood, no period, <72 chars
- **Body**: bullet points, each starting with what changed
- **Co-author**: always include `Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>`
- **Files section**: list files to stage as a copy-pasteable `git add` command

**Step 3: Output summary**

```
Commit message drafted in `.context/active/commits/NEXT_COMMIT.md`

Subject: type(scope): subject line
Files to stage: N files

Gabriel: review the message, then stage + commit + push.
```

---

## Mode 2: ARCHIVE

**When**: The subject line in NEXT_COMMIT.md matches a recent `git log` entry.

### Steps

1. Confirm the match: "Found matching commit: `<hash> <subject>`"
2. Determine archive path: `.context/active/commits/older/YYYY-MM/YYYY-MM-DD.md`
3. Create archive directory if needed
4. Copy NEXT_COMMIT.md content to archive file
5. Clear NEXT_COMMIT.md back to empty template

---

## Edge Cases

- **No changes detected**: "Nothing to commit. Working tree clean."
- **NEXT_COMMIT.md has content but not committed**: Ask Gabriel — "Draft a new one or keep?"
- **Archive directory doesn't exist**: Create it (`mkdir -p`).

---

## Output Constraints

- All output is copy-pasteable
- Never run active git commands
- Evidence-based: read the actual diff, don't guess what changed

---

**Last Updated**: 2026-03-05
