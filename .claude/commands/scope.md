# Scope Acknowledgment Command

**Purpose**: Read and fully understand provided documentation before proceeding. Ask clarifying questions if anything is unclear.

---

## Instructions

You are being asked to acknowledge scope. This is NOT a request to produce output.

### Step 1: Read Everything Provided

Read ALL documentation, files, and context shared in this prompt completely:
- Task specifications
- Scope documents
- Related code files
- Any linked references

### Step 2: Verify Understanding

For each document, confirm you understand:
- **What IS in scope** (explicit requirements)
- **What is NOT in scope** (boundaries, exclusions)
- **Success criteria** (how to know when done)
- **Constraints** (time, technology, dependencies)

### Step 3: Apply Standards

Cross-reference with established standards:
- `.context/standards/SCIENTIFIC_METHOD.md` - Problem-solving framework
- `.context/standards/QUALITY_GATES.md` - Quality checkpoints
- `.context/standards/CONVENTIONS.md` - Coding standards
- `.context/standards/protocols/CURSOR_SCOPE_CONTROL.md` - Scope boundaries

### Step 4: Identify Gaps

If ANYTHING is unclear, ambiguous, or potentially conflicting:
- **ASK clarifying questions BEFORE proceeding**
- List specific points that need clarification
- Do not assume or guess

### Step 5: Confirm Readiness

Once fully clear, respond with:
1. Brief summary of what you understand the task to be
2. Key boundaries (what you will NOT do)
3. Any questions that need answers before starting
4. "Ready to proceed" when scope is 100% clear

---

## Step 6: Determine Document Type

Identify what you're reading:
- **Task/Plan**: Actionable work with deliverables → Evaluate if task spec needed
- **Scope/Standards**: Reference documentation → Acknowledge understanding
- **Research**: Background information → Summarize key insights

## Step 7: Report Clarity Status

After reading, you MUST report one of:

### If Task/Plan and Questions Remain:
```
**Clarity**: Questions remain (see below)
**Questions**:
1. [Specific question]
2. [Specific question]
```

### If Task/Plan and 100% Clear:
Evaluate complexity and recommend:

**Option A** (Complex task):
```
**Clarity**: 100% clear
**Recommendation**: It is recommended to create a task spec (or X task specs) to cover tasks with more detail. Should I now create these task specs?
```

**Option B** (Simple/already detailed):
```
**Clarity**: 100% clear
**Recommendation**: This task/plan is as clear as can be. A task spec is not necessary. Should I proceed?
```

### If Scope/Standards/Research:
```
**Document Type**: [Scope | Standards | Research]
**Summary**: [Key points understood]
**Status**: Acknowledged - awaiting your next instruction
```

---

## Output Format

```markdown
## Scope Acknowledged

**Document Type**: [Task/Plan | Scope | Standards | Research]

**Summary**: [1-2 sentence summary of what you read]

**In Scope** (if applicable):
- [Key deliverable 1]
- [Key deliverable 2]

**Out of Scope** (if applicable):
- [Explicit exclusion 1]
- [Explicit exclusion 2]

**Clarity**: [100% clear | Questions remain]

**Questions** (if any):
1. [Question needing clarification]

**Recommendation**: [One of the three options above based on document type]
```

---

## Red Flags (STOP and ASK)

- Task mentions features not in provided documentation
- Scope seems larger than documented
- Multiple interpretations possible
- Dependencies not mentioned
- Timeline/effort seems mismatched with scope

---

**Remember**: This command is about UNDERSTANDING, not DOING. Do not start implementation until scope is 100% clear and confirmed.
