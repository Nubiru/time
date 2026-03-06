# /ask Command

## Purpose
Get information, clarification, or explanation about the codebase, documentation, or development processes. Pure conversational mode (read-only) with optional documentation.

## Usage
```
/ask [your question]
```

## Examples
```
/ask How does the Prompt Manager work?
/ask What's the difference between /task and /plan commands?
/ask Explain the 3-layer context architecture
/ask Where should I put integration tests?
/ask What design patterns are recommended for user authentication?
/ask How do I invoke the Code Reviewer agent?
/ask What are the quality gates for implementation?
```

## What Happens

### Step 1: Context Loading (5-10 seconds)
1. Load relevant standards documentation
2. Load relevant system documentation if exists
3. Load relevant development context if needed
4. Search codebase if question relates to implementation

### Step 2: Information Synthesis (10-20 seconds)
1. Extract relevant information from loaded contexts
2. Cross-reference multiple sources
3. Identify key concepts and relationships
4. Prepare comprehensive explanation

### Step 3: Conversational Response (immediate)
1. Answer question clearly and concisely
2. Provide examples if helpful
3. Reference source documents
4. Suggest related concepts or next steps

### Step 4: Optional Documentation (on explicit request)
1. User can request to save valuable Q&A
2. Saves to `.context/communications/QA_[Topic]_[Date].md`
3. For future reference and knowledge building

**Total Time**: 15-30 seconds

## Output

**Primary Output**: Conversational response (in chat)

**Optional Output**: Saved Q&A document (if explicitly requested or high value)

### Example Response Format

```
**Answer**: [Clear, concise explanation]

**How It Works**:
1. [Step-by-step breakdown if relevant]
2. [...]

**Example**:
[Concrete example if helpful]

**References**:
- `.context/standards/protocols/[relevant-doc].md`
- `.claude/agents/[relevant-agent].md`

**Related Concepts**:
- [Related topic 1]
- [Related topic 2]

**Next Steps** (optional):
- [Suggestion for user's workflow]
```

## When to Use

**Use `/ask` when**:
- ✅ Understanding how something works (system, agent, pattern)
- ✅ Clarifying concepts (architecture, workflows, standards)
- ✅ Finding documentation (where is X documented?)
- ✅ Learning best practices (how should I approach Y?)
- ✅ Understanding command differences (what's the difference between X and Y?)
- ✅ Navigating the boilerplate (where do I put Z?)
- ✅ Quick reference lookups (what are the quality gates?)

**Don't use `/ask` when**:
- ❌ Want to execute a task (use `/task` instead)
- ❌ Need to fix something (use `/fix` for trivial, `/task` for complex)
- ❌ Want to plan architecture (use `/plan` instead)
- ❌ Need code review (use `/review` instead)
- ❌ Want research with web access (use `/research` instead)
- ❌ Need decision analysis (use `/consider` instead)

## Mode: Read-Only

**Important**: `/ask` is read-only (no code changes, no file creation unless explicitly requested)

**Permissions**:
- ✅ Read any standards documentation
- ✅ Read any system documentation
- ✅ Search codebase
- ✅ Reference agents, commands, workflows
- ⚠️ Write only if user explicitly requests Q&A save
- ❌ No code changes
- ❌ No task execution

## Integration

**References**:
- `.context/standards/` - All standards and protocols
- `docs/architecture/` - Project-specific documentation
- `.context/active/` - Current work context
- `.claude/agents/` - Agent definitions
- `.claude/commands/` - Command definitions

**Context Loading Priority**:
1. Standards - Universal standards
2. Architecture - Project-specific
3. Active context - Current work
4. Codebase - Implementation details

**Agents**:
- Orchestrator (Ask mode) - Coordinates response
- No file modifications unless explicitly requested

## Examples

### Example 1: Understanding System Concepts

**User Question**:
```
/ask How does the Prompt Manager work?
```

**Response**:
```
**Answer**: The Prompt Manager is an agent that expands brief user prompts into comprehensive task specifications through multiple software development role perspectives.

**How It Works**:

1. **Receives Brief Prompt**: User provides 5-10 word task description via `/task` command
   Example: "Add user authentication with JWT"

2. **Role-Based Expansion**: Expands through 6 perspectives:
   - Business Analyst → Requirements, success metrics
   - Product Owner → User stories, acceptance criteria
   - Software Architect → Technical approach, patterns
   - Developer → Implementation strategy, subtasks
   - QA/Tester → Test strategy, edge cases
   - DevOps Engineer → Deployment approach, infrastructure

3. **Complexity Scoring**: Assigns 0-100 complexity score
   - 0-30: Simple (Cursor handles)
   - 31-70: Moderate (Cursor + ClaudeCode collaborate)
   - 71-100: Complex (ClaudeCode Opus handles)

4. **Task Specification**: Generates comprehensive task spec in:
   `.context/active/TASK_[ID]_[Name]/TASK_SPEC.md`

5. **Routing Decision**: Hands off to Tool Selector for execution routing

**Example Workflow**:
```
User: /task Add JWT authentication
  ↓
Prompt Manager expands (6 roles)
  ↓
Task spec generated (complexity: 65/100)
  ↓
Tool Selector routes: Research (Cursor) → Planning (ClaudeCode) → Implementation (Cursor + ClaudeCode)
```

**References**:
- `.claude/agents/prompt-manager.md` - Full agent definition
- `.claude/commands/task.md` - Command usage guide
- `.context/standards/protocols/SCIENTIFIC_METHOD.md` - Problem-solving framework used

**Related Concepts**:
- Tool Selector (routing decisions)
- Orchestrator (meta-agent coordination)
- Complexity scoring (0-100 scale)

**Next Steps**:
Try using `/task` with a simple prompt to see Prompt Manager in action!
```

---

### Example 2: Command Comparison

**User Question**:
```
/ask What's the difference between /task and /plan?
```

**Response**:
```
**Answer**: Both create planning documents, but with different scope and expansion:

| Aspect | `/task` | `/plan` |
|--------|---------|---------|
| **Scope** | Complete feature (all phases) | Architecture design only |
| **Expansion** | Full (6 roles) | Focused (Architect role) |
| **Output** | Task spec (requirements, plan, tests, deployment) | Architecture design doc |
| **When to Use** | Starting new feature end-to-end | Need architecture before coding |
| **Complexity** | Any (0-100) | Usually higher (50-100) |
| **Routing** | Tool Selector decides | Goes to Architecture Designer agent |

**Use `/task` when**:
- You want comprehensive planning from requirements to deployment
- You need full role-based expansion (BA, PO, Architect, Dev, QA, DevOps)
- You want automatic routing based on complexity

**Use `/plan` when**:
- You only need architecture design (no requirements, no tests yet)
- You're in design phase (not ready for full implementation)
- You want to focus on technical approach and patterns

**Example Comparison**:

`/task Add caching layer` →
- Requirements (what caching achieves)
- Architecture (cache strategy, tools)
- Implementation plan (step-by-step)
- Test strategy (cache hits, misses, invalidation)
- Deployment approach (configuration, monitoring)

`/plan Add caching layer` →
- Architecture design only (cache strategy, tools, integration)
- Design patterns (Cache-Aside, Write-Through, etc.)
- Technical trade-offs
- No requirements, tests, or deployment details

**References**:
- `.claude/commands/task.md`
- `.claude/commands/plan.md`
- `.claude/agents/prompt-manager.md` (used by /task)
- `.claude/agents/architecture-designer.md` (used by /plan)

**Next Steps**:
Use `/task` for most cases (comprehensive). Use `/plan` when you specifically need architecture design only.
```

---

### Example 3: Navigation Help

**User Question**:
```
/ask Where should I put integration tests?
```

**Response**:
```
**Answer**: Integration tests location depends on your project structure, but here are the standards:

**Recommended Structure** (from CONVENTIONS.md):

```
project-root/
├── src/              # Source code
├── tests/
│   ├── unit/         # Unit tests (isolated)
│   ├── integration/  # Integration tests ⭐ (this is where they go)
│   └── e2e/          # End-to-end tests (full system)
```

**Integration Test Characteristics**:
- Test multiple components together
- May involve database, API calls, external services
- Slower than unit tests (seconds vs milliseconds)
- Test real interactions (not mocked)

**Naming Convention**:
- `*.integration.test.ts` (TypeScript)
- `test_*.py` in `tests/integration/` (Python)
- `*.integration.spec.js` (JavaScript)

**Example Integration Test Location**:
```
tests/integration/
├── auth/
│   ├── login.integration.test.ts
│   └── jwt-verification.integration.test.ts
├── database/
│   ├── user-repository.integration.test.ts
│   └── transaction-rollback.integration.test.ts
└── api/
    ├── user-endpoints.integration.test.ts
    └── auth-endpoints.integration.test.ts
```

**Quality Gates** (from QUALITY_GATES.md):
- Implementation Gate I4: Integration tests written
- Review Gate R2: Tests cover happy path + error cases

**References**:
- `.context/standards/protocols/CONVENTIONS.md` - File organization standards
- `.context/standards/protocols/QUALITY_GATES.md` - Testing requirements
- `.claude/agents/test-writer.md` - Test generation agent

**Next Steps**:
1. Create `tests/integration/` folder if it doesn't exist
2. Use `/test` command to generate integration tests
3. Follow naming convention: `[feature].integration.test.[ext]`
```

---

### Example 4: Saving Valuable Q&A

**User Question**:
```
/ask Explain the 3-layer context architecture

(then user says: "This is really helpful, can you save this for future reference?")
```

**Response**:
```
**Answer**: [comprehensive explanation provided]

✅ Saved to `.context/communications/QA_3-Layer_Context_Architecture_2025-10-12.md`

**Why Saved**:
- Core architectural concept
- Frequently referenced
- Valuable for future team members
- Builds knowledge base

**Future Access**:
- File is in Layer 2 (development artifacts)
- Can be referenced in future conversations
- Consider promoting to Layer 1 (system docs) if universally valuable for project
```

---

## Optional: Q&A Documentation

**When to Save**:
- User explicitly requests: "Save this"
- High-value explanation (core concepts)
- Frequently asked question
- Complex topic requiring detailed explanation

**When NOT to Save**:
- Simple, obvious answers
- Already documented in standards
- One-time question
- Low future reference value

**Saved Q&A Format**:
```markdown
# Q&A: [Topic]

**Date**: 2025-10-12
**Question**: [original user question]
**Context**: [why this was asked]

## Answer

[comprehensive answer provided]

## References

- [source documents]

## Related Questions

- [related topics for further exploration]
```

## Success Metrics

**Response Quality**:
- 90% of answers accurate and complete
- 95% of answers reference correct source documents
- No speculation (only facts from documentation/code)

**Response Time**:
- Simple questions: <15 seconds
- Complex questions: <30 seconds
- Multi-part questions: <45 seconds

**User Satisfaction**:
- Answers are clear and actionable
- Examples provided when helpful
- References enable deeper exploration

**Appropriate Use**:
- 100% read-only (no unintended changes)
- File creation only on explicit request
- Escalations handled (redirect to appropriate command)

## Tool Permissions

- **Read**: All standards docs, system docs, development context, codebase
- **Write**: `.context/communications/QA_*.md` (only on explicit user request)
- **Execute**: None (read-only mode)
- **Escalate**: To appropriate command if user needs execution (task, fix, plan, etc.)
- **Search**: Codebase, documentation (grep, codebase_search)

## Anti-Patterns

### ❌ Anti-Pattern 1: Using /ask for Execution
**Problem**: `/ask` cannot execute tasks
**Example**: `/ask fix the typo in README`
**Why Bad**: `/ask` is read-only
**Fix**: Use `/fix typo in README` instead

### ❌ Anti-Pattern 2: Using /ask for Research (External)
**Problem**: `/ask` has no web access
**Example**: `/ask what are the latest JWT security best practices?`
**Why Bad**: Needs external research, not internal docs
**Fix**: Use `/research latest JWT security best practices` instead

### ❌ Anti-Pattern 3: Using /ask for Decisions
**Problem**: `/ask` provides information, not decision analysis
**Example**: `/ask should I use Redux or Context API?`
**Why Bad**: Needs structured decision framework
**Fix**: Use `/consider Redux vs Context API for state management` instead

## Quick Reference

| Question Type | Example | Response Time | Saves File? |
|---------------|---------|---------------|-------------|
| How does X work? | "How does Prompt Manager work?" | 20s | Optional |
| What's the difference? | "/task vs /plan?" | 15s | No |
| Where do I put X? | "Where do integration tests go?" | 10s | No |
| Best practices for Y? | "Best practices for error handling?" | 25s | Optional |
| Explain concept Z | "Explain 3-layer architecture" | 30s | Optional |

---

**Last Updated**: 2025-10-12
**Command Type**: Conversational (read-only)
**Tool Permissions**: Read (all docs/code), Write (QA docs on request only), Escalate (to other commands)
**Integration**: Orchestrator (Ask mode), no Prompt Manager expansion

