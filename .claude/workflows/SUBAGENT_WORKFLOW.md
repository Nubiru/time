# Subagent Orchestration Workflow

**Version**: 1.0
**Created**: 2026-01-21
**Purpose**: General workflow for orchestrating specialized subagents for any task type (debugging, architecture, audits, git health, testing, etc.)

---

## Overview

This workflow enables systematic task execution by delegating work to specialized subagents. The orchestrator coordinates agents, provides context and constraints, and consolidates results into actionable output.

**Trigger**: User mentions "subagent" or requests parallel task execution

**Pattern**: Orchestrator → Selects Agent(s) → Provides Context → Agents Execute → Consolidate Results

---

## Available Agents

| Agent | Use Cases |
|-------|-----------|
| **planning-specialist** | Deep analysis, research, algorithm comparison, phase planning |
| **code-reviewer** | C code quality, memory safety, compiler warnings, conventions |
| **test-writer** | TDD with Unity framework, test creation, coverage analysis |

---

## Orchestrator Pattern

### Phase 1: Task Analysis (Orchestrator)

**Actions**:
1. Identify task scope and type
2. Select appropriate agent(s) from list above
3. Determine if tasks can run in parallel or must be sequential
4. Gather required context (files, logs, standards, specifications)

**Decision**:
- **Parallel**: Tasks are independent (e.g., git health check + test debugging + audit)
- **Sequential**: Second task depends on first result

---

### Phase 2: Agent Dispatch (Orchestrator)

**For Each Agent**:

1. **Choose Agent Type** based on task nature
2. **Prepare Context**:
   - Mandatory: CLAUDE.md (project overview, environment, constraints)
   - MVP Scope: `docs/MVP_CATALOG.md` — which MVP-XX IDs does this task affect?
   - Standards: Relevant `.context/standards/` files
   - Input: Files, logs, test output, specifications
   - Project Context: MVPs, intentions, architecture if needed
3. **Define Scope**: Clear boundaries and success criteria
4. **Set Constraints**: NO terminal commands, work with provided input only

**Agent Invocation Template**:

```markdown
I'm dispatching you as the <Agent Name> agent for <task description>.

**Mandatory Reading** (Read these FIRST):
1. `.claude/CLAUDE.md` - Project overview, environment, partnership principles
2. `docs/ROADMAP.md` - Current development phase and progress
3. `.context/standards/<relevant-standard>.md` - Domain-specific standards
4. [Additional context files as needed]

**Task Scope**:
<Clear description of what needs to be done>

**MVP Scope**: MVP-XX (from `docs/MVP_CATALOG.md`)

**Context**:
- Problem/Goal: <description>
- Recent Changes: <what changed recently>
- Environment: <relevant environment info>
- Available Input: <list of files, logs, data available>

**Investigation/Execution Protocol**:
1. Read all mandatory files above
2. <Task-specific steps>
3. Analyze systematically (5 Whys, Evidence-First Protocol)
4. Provide specific, actionable output

**Output Requirements**:
- <What format/content is expected>
- File:line references for code changes
- Clear reasoning for recommendations
- Confidence levels for assessments

**Constraints**:
- ❌ DO NOT run terminal commands (npm, node, docker, etc.)
- ❌ DO NOT run ACTIVE git commands (add, commit, push, branch, checkout, merge, rebase)
- ❌ DO NOT generate new test runs or build output
- ❌ DO NOT execute scripts or programs
- ✅ USE provided input files and code reading only
- ✅ USE passive git commands only (status, log, diff, show, blame)
- ✅ ANALYZE existing data and code
- ✅ REQUEST additional input if needed
- ✅ DECLARE DONE when task complete

**Success Criteria**:
<Specific, measurable outcomes>

**Reference**: `.claude/agents/<agent-name>.md`
```

---

### Phase 3: Parallel Agent Execution

**For Independent Tasks**:

Launch multiple agents in a SINGLE message with multiple Task tool calls:

```typescript
// Example: 3 parallel agents
Task 1: git-health-manager - Analyze git repo health
Task 2: debugger - Fix failing test cases
Task 3: architecture-designer - Audit blockchain domain

// All launch simultaneously, run independently
```

**Benefits**:
- 2-3x faster than sequential
- Maximize throughput
- Independent work streams

**When to Use**:
- Tasks have no dependencies
- Each has complete context
- Results can be consolidated independently

---

### Phase 4: Agent Execution (Subagent)

**Subagent Actions**:

1. **Read Mandatory Files**:
   - CLAUDE.md (project identity, environment, constraints, partnership principles)
   - ROADMAP.md (current phase, progress)
   - Relevant standards (TDD, architecture, conventions, etc.)

2. **Analyze Context**:
   - Read provided files (logs, test output, code)
   - Search codebase for relevant files (Glob, Grep, Read tools)
   - Understand current state and recent changes

3. **Execute Task**:
   - Apply systematic methodologies (5 Whys, Evidence-First)
   - Identify root causes or design solutions
   - Distinguish facts from assumptions
   - State confidence levels explicitly

4. **Generate Output**:
   - Specific recommendations with file:line references
   - Code snippets where applicable
   - Clear reasoning for each recommendation
   - Trade-offs and alternatives considered

5. **Declare Status**:
   - "DONE" if task complete with actionable output
   - "NEED INPUT: <specific request>" if more data required

**Agent Output Format**:

```markdown
## Task: <Task Name>

**Analysis Summary**:
<Key findings, 2-3 sentences>

**Root Cause / Recommendation**:
<Specific cause or design decision with evidence>

**Changes Required** (if applicable):

1. File: `path/to/file.c:123`
   - Change: <what to change>
   - Reason: <why this change>
   - Confidence: <high/medium/low>

2. File: `path/to/file.c:456`
   - Change: <what to change>
   - Reason: <why this change>
   - Confidence: <high/medium/low>

**Trade-offs** (if applicable):
- Option A: <pros/cons>
- Option B: <pros/cons>
- Recommendation: <which and why>

**Status**: DONE | NEED INPUT: <specific request>
```

---

### Phase 5: Consolidation (Orchestrator)

**Orchestrator Actions**:

1. **Wait for Completion**: All agents finish
2. **Read Agent Outputs**: Review each agent's findings
3. **Validate Against Evidence**: Cross-check with available data
4. **Consolidate Results**: Combine findings, resolve conflicts
5. **Present Summary**: Clear, actionable report to user

**Consolidation Format**:

```markdown
## Subagent Investigation Complete

### Results Summary

**Agent 1** (<agent-type>): <task>
- Findings: <key discoveries>
- Recommendations: <main recommendations>
- Confidence: <overall confidence>

**Agent 2** (<agent-type>): <task>
- Findings: <key discoveries>
- Recommendations: <main recommendations>
- Confidence: <overall confidence>

**Agent 3** (<agent-type>): <task>
- Findings: <key discoveries>
- Recommendations: <main recommendations>
- Confidence: <overall confidence>

### Consolidated Recommendations

1. **<Priority Level>**: <recommendation>
   - Source: Agent(s) <which agent(s)>
   - Impact: <what this enables>
   - Timeline: <estimated effort>

2. **<Priority Level>**: <recommendation>
   - Source: Agent(s) <which agent(s)>
   - Impact: <what this enables>
   - Timeline: <estimated effort>

### Next Steps

- [ ] <Action 1>
- [ ] <Action 2>
- [ ] <Action 3>

### Validation Required (if applicable)

Please run: `<commands>`
Expected: <expected outcome>
```

---

### Phase 6: Implementation (Orchestrator)

**Orchestrator Actions**:

1. **Apply Changes**: Implement agent recommendations systematically
2. **Update Documentation**: Related docs, standards, learning log
3. **Request Validation**: User runs tests/build/checks
4. **Update Todo List**: Track progress and completion

**Implementation Pattern**:
- Apply changes in logical order (dependencies first)
- One change at a time for complex fixes
- Verify mentally before moving to next
- Request user validation for terminal commands

---

## Example Workflows

### Example 1: Parallel Triple Agent (Git + Tests + Audit)

**User**: "Run subagent workflow for git health, fix test failures, and audit blockchain domain"

**Orchestrator**:
1. Identifies 3 independent tasks
2. Launches 3 agents in parallel (single message):
   - git-health-manager: Analyze repo health
   - debugger: Fix test failures from quality-checks.md
   - architecture-designer: Audit blockchain domain architecture

**Agent Execution** (parallel):
- Each reads CLAUDE.md + relevant standards
- Each analyzes their specific domain
- Each generates independent output

**Consolidation**:
- Git Health: 3 recommendations (branch cleanup, merge conflicts, commit history)
- Test Fixes: 5 test failures identified with specific fixes
- Audit: 8 findings (6 strengths, 2 improvement areas)

**Result**: Comprehensive analysis across 3 domains in single iteration

---

### Example 2: Sequential Architecture Decision

**User**: "Design caching strategy using subagent"

**Orchestrator**:
1. Identifies architecture decision task
2. Launches architecture-designer agent with:
   - Context: Current caching needs, performance requirements
   - Standards: ARCHITECTURE.md, DESIGN_PATTERNS.md
   - Input: Existing caching code, performance metrics

**Agent Execution**:
- Reads project architecture standards
- Analyzes current implementation
- Researches caching patterns (Redis, in-memory, hybrid)
- Evaluates trade-offs

**Output**:
- Recommendation: Hybrid strategy (Redis + in-memory)
- Implementation plan with file locations
- Trade-offs clearly documented
- Migration strategy

**Implementation**: Orchestrator applies recommendation with user approval

---

### Example 3: Debugging with Standards

**User**: "Debug Electrum circuit breaker issue using subagent"

**Orchestrator**:
1. Task: Debug Electrum connection race condition
2. Agent: debugger
3. Context:
   - Logs: backend-20260120-183105.log
   - Standards: TDD_METHODOLOGY.md, EVIDENCE_PROTOCOL.md
   - Issue: ACTIVE_ISSUES.md #7

**Agent Execution**:
- Reads TDD methodology and evidence protocol
- Analyzes logs for circuit breaker pattern
- Identifies race condition (5 failures in 1 second)
- Provides specific fix with file:line references

**Output**:
- Root cause: Socket connects before readiness check
- Fix: Add connection confirmation before service starts
- Confidence: High (clear evidence in logs)

**Implementation**: Orchestrator applies fix, requests user validation

---

## Agent Selection Guide

| Task Type | Primary Agent | Notes |
|-----------|---------------|-------|
| Algorithm research | planning-specialist | Compare approaches, analyze reference materials |
| Code quality review | code-reviewer | Memory safety, compiler warnings, conventions |
| Test creation (TDD) | test-writer | Write tests first, Unity framework |
| Phase planning | planning-specialist | Break roadmap phases into tasks |
| Architecture decisions | planning-specialist | Evaluate design trade-offs |

---

## Agent Constraints (CRITICAL)

### What Agents MUST NOT Do
- ❌ Run terminal commands (npm, node, docker, etc.)
- ❌ Run ACTIVE git commands (branch, checkout, add, commit, push, merge, rebase, etc.)
- ❌ Generate new test runs or build output
- ❌ Execute scripts or programs
- ❌ Modify files without orchestrator approval
- ❌ Make assumptions without evidence
- ❌ Skip reading PROJECT_SPEC.md, CLAUDE.md, and relevant standards

### What Agents MUST Do
- ✅ Read CLAUDE.md FIRST (understand what the Time project is)
- ✅ Read ROADMAP.md (understand current phase)
- ✅ Read relevant standards for their domain
- ✅ Work with provided input (logs, test output, files)
- ✅ Use Read, Glob, Grep tools for code analysis
- ✅ Use PASSIVE git commands only (status, log, diff, show, blame) - never active commands
- ✅ Apply systematic methodologies (5 Whys, Evidence-First)
- ✅ Provide specific, actionable output with file:line references
- ✅ State confidence levels explicitly
- ✅ Distinguish facts from assumptions
- ✅ Request additional input if needed
- ✅ Declare DONE when task complete

---

## Integration with Commands

### `/proceed` Command
When user says `/proceed to <task>` with "subagent" mentioned:
1. Orchestrator identifies task requirements
2. Selects appropriate agent(s)
3. Dispatches with proper context and constraints
4. Consolidates results
5. Implements or presents recommendations

### Explicit Agent Requests
User can specify agent type:
- "Use architecture-designer subagent for..."
- "Debug with subagent..."
- "Run git-health-manager subagent..."

### Multiple Agents
User can request parallel execution:
- "Run subagents for git health, tests, and audit"
- "Use 3 subagents to analyze..."

---

## Success Metrics

**Agent Performance**:
- 95%+ accuracy in analysis and recommendations
- 100% of recommendations include file:line references
- 0 terminal commands executed
- Clear DONE/NEED INPUT status
- Confidence levels stated explicitly

**Orchestrator Performance**:
- Clear agent selection reasoning
- Proper context preparation (CLAUDE.md + standards + input)
- Accurate consolidation of findings
- Actionable next steps for user

**Workflow Efficiency**:
- <5 minutes agent dispatch time
- <10 minutes execution time (simple tasks)
- <30 minutes execution time (complex tasks)
- Single iteration to resolution (minimal back-and-forth)

---

## Common Pitfalls

### Pitfall 1: Agent Runs Active Git Commands
**Problem**: Agent tries to run `git add`, `git commit`, `git push`, `git checkout`, `git branch`
**Prevention**: Explicitly state "❌ DO NOT run ACTIVE git commands" in dispatch. Only passive commands (status, log, diff, show, blame) allowed.
**Recovery**: Kill agent immediately, relaunch with clearer constraints

### Pitfall 2: Agent Runs Terminal Commands
**Problem**: Agent tries to run `make test` or `make native` directly
**Prevention**: Explicitly state "❌ DO NOT run terminal commands" in dispatch
**Recovery**: Kill agent, relaunch with clearer constraints

### Pitfall 3: Agent Doesn't Read Mandatory Files
**Problem**: Agent doesn't understand what the Time project is, environment, or constraints
**Prevention**: Make CLAUDE.md and ROADMAP.md first mandatory reading items
**Recovery**: Remind agent to read CLAUDE.md and ROADMAP.md, may need to relaunch

### Pitfall 4: Vague Recommendations
**Problem**: Agent says "fix the code" without specifics
**Prevention**: Require "file:line - change - reason" format in dispatch
**Recovery**: Request specific locations and changes

### Pitfall 5: Missing Standards Context
**Problem**: Agent doesn't follow project conventions
**Prevention**: Always include relevant standards in mandatory reading
**Recovery**: Provide standards, may need to re-analyze

### Pitfall 6: Sequential When Parallel Possible
**Problem**: Running agents one at a time when they could run in parallel
**Prevention**: Identify independent tasks upfront
**Recovery**: Launch remaining agents in parallel

---

## Version History

**v1.0** (2026-01-21):
- Initial general workflow documentation
- Covers all task types (debugging, architecture, git, audits, etc.)
- Parallel agent execution pattern
- Agent selection guide
- Integration with /proceed command
- Based on successful Issue #9, #10, #11 orchestration

---

**Next**: Extend with additional agent types, refine templates based on usage patterns
