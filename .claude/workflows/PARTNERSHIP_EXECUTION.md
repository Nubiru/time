# Partnership Execution Workflow

**Version**: 1.0
**Type**: Behavioral Pattern
**Purpose**: Systematic approach to complex problem-solving through human-AI partnership

---

## Philosophy

Complex work succeeds through:
- **Clear problem definition** before solution design
- **Understanding existing code** before writing new code
- **Incremental phases** with explicit boundaries
- **Continuous tracking** via todo lists
- **Consistency** with project standards
- **Post-execution rituals** that capture knowledge

---

## Workflow Phases

### Phase 0: Context Gathering (Conditional)

**Trigger**: Problem is unclear, solution space is unknown, or existing code is unfamiliar

**Actions** (select as needed):
- `/research` - Gather external knowledge, best practices, prior art
- `/review` - Analyze existing codebase patterns, architecture, conventions
- `/consider` - Evaluate multiple approaches before committing
- Debug investigation - When problem symptoms don't reveal root cause

**Exit Criteria**: Sufficient understanding to define the problem precisely

---

### Phase 1: Problem Definition

**Purpose**: Establish shared understanding of WHAT needs to change

**Actions**:
1. State the problem in user's terms (not technical terms)
2. Identify the gap between current state and desired state
3. Clarify scope boundaries (what is IN, what is OUT)
4. Surface assumptions and validate with user
5. Agree on success criteria (how will we know it's done?)

**Output**: Clear problem statement with measurable success criteria

**Anti-patterns**:
- Jumping to solution before understanding problem
- Assuming requirements without validation
- Scope creep through unstated assumptions

---

### Phase 2: Solution Design

**Purpose**: Define the solution algorithm before touching code

**Actions**:
1. Design the solution at conceptual level (data flow, state changes, interactions)
2. Identify components that need modification or creation
3. Determine dependency order (what must happen first?)
4. Break into discrete phases with clear boundaries
5. Identify risks and unknowns
6. **[MANDATORY] Data Flow Integration Check** (see below)

**Output**: Solution algorithm as ordered phases with explicit dependencies

**Principle**: The plan should be understandable without code knowledge

#### Data Flow Integration Check (MANDATORY for features needing external data)

**Before designing ANY feature that needs external data (RPC, API, etc.):**

1. **ASK**: "Does similar data already flow through this system?"
   - Example: "Does Bitcoin Core mempool data already get polled somewhere?"

2. **FIND**: Locate the existing state service for this data source
   - `BlockchainStateService` for Bitcoin Core data
   - `ElectrumStateService` for Electrum data
   - Pattern: `backend/src/application/blockchain-state/` or similar

3. **DECIDE**: Integration approach
   - **Option A**: Add polling to existing state service (preferred)
   - **Option B**: Read from existing cache populated by state service
   - **Option C**: Create new state service following the pattern (only if truly new data source)
   - **WRONG**: Create adapter/provider that makes direct external calls

4. **DOCUMENT**: In the plan, explicitly state:
   ```markdown
   ## Data Source Integration
   - **External data needed**: getrawmempool RPC
   - **Existing state service**: BlockchainStateService
   - **Integration approach**: Add getrawmempool to existing polling cycle
   - **Consumer access**: Read from cache key `l1:mempool:raw:verbose`
   ```

**Red Flag Questions** (if answer is YES, plan needs revision):
- Does this create a new adapter/provider that calls external services?
- Does this component have its own caching separate from state services?
- Would this cause two components calling the same external endpoint?

**Reference**: `.claude/workflows/STATE_MACHINE_AUDIT_TEMPLATE.md` R23: Centralized Data Flow Pattern

---

### Phase 3: Codebase Study

**Purpose**: Understand existing code BEFORE writing new code

**Actions**:
1. Read files that will be modified
2. Identify existing patterns to follow
3. Find integration points (where does new code connect?)
4. Understand type contracts and interfaces
5. Note any inconsistencies or technical debt to avoid propagating

**Output**: Mental model of how solution fits into existing architecture

**Principle**: New code should look like it was written by the same author as existing code

---

### Phase 4: Incremental Execution

**Purpose**: Implement solution through tracked, discrete steps

**Actions**:
1. Create todo list with all implementation steps
2. Mark current task as `in_progress` before starting
3. Complete one task fully before moving to next
4. Mark task `completed` immediately upon finishing
5. Maintain clear phase boundaries (backend → types → handlers → frontend)
6. Verify each phase works before proceeding

**Principles**:
- One task `in_progress` at a time
- Never batch completions
- Each phase should be independently committable
- Read before write (always read existing file before modifying)

**Pattern for Complex Changes**:
```
Backend Types → Backend Service → Backend Handler/Wiring →
Frontend Types → Frontend Service → Frontend Hook → Frontend Component
```

---

### Phase 5: Post-Execution (Conditional)

**Trigger**: Implementation complete, ready for handoff

**Select applicable actions**:

| Action | When to Apply |
|--------|---------------|
| `/document` | Architectural changes, new systems, complex flows |
| `/audit` | Security-sensitive, performance-critical, or public-facing |
| QA Validation | Changes that can break existing functionality |
| Pattern Extraction | Novel solutions that could apply elsewhere |
| Logs to Watch For | Changes requiring production validation |
| Commit Message | Always (provide comprehensive, structured message) |
| Learning Logs | Mistakes made, corrections received, insights gained |

**Principle**: Post-execution is not optional polish—it's how knowledge transfers across sessions

---

## Todo List Protocol

**Creation**:
- Create at start of Phase 4
- Each item is a discrete, completable unit
- Items follow dependency order
- Include both `content` (imperative) and `activeForm` (present continuous)

**Maintenance**:
- Update in real-time as work progresses
- Only ONE item `in_progress` at any time
- Mark complete IMMEDIATELY when done
- Add discovered subtasks as they emerge
- Remove items that become irrelevant

**Completion**:
- All items `completed` before Phase 5
- If blocked, document blocker and proceed with what's possible

---

## Quality Checkpoints

### Before Each Phase Transition

- [ ] Current phase deliverables complete
- [ ] No assumptions made without validation
- [ ] Existing patterns followed
- [ ] Todo list reflects current state

### Before Commit

- [ ] All todo items completed
- [ ] QA checks pass (lint, types, tests)
- [ ] Changes match original problem scope
- [ ] Post-execution rituals applied

---

## Partnership Principles

**Human provides**:
- Problem context and requirements
- Scope decisions and priorities
- Validation of assumptions
- Corrections when approach diverges

**AI provides**:
- Systematic breakdown of complex problems
- Comprehensive exploration of solution space
- Consistent execution through phases
- Documentation and knowledge capture

**Shared responsibility**:
- Clear communication about state and progress
- Explicit validation at phase boundaries
- Course correction when needed
- Learning extraction for future sessions

---

## Activation

This workflow activates implicitly when:
- User presents a complex, multi-step problem
- Problem requires both understanding and implementation
- Multiple files or systems need coordinated changes

This workflow activates explicitly via:
- User mentions "partnership workflow" or "systematic approach"
- User requests phased execution with tracking

---

## Integration with Other Workflows

| Related Workflow | Integration Point |
|------------------|-------------------|
| `/research` | Phase 0 - External knowledge gathering |
| `/review` | Phase 0 - Codebase pattern analysis |
| `/consider` | Phase 2 - Solution alternatives |
| `/plan` | Phase 2 - Detailed implementation planning |
| `/document` | Phase 5 - Architecture documentation |
| `/audit` | Phase 5 - Quality and security review |
| `LOG_EXTRACTION_VALIDATION` | Phase 5 - Production validation patterns |

---

## Success Indicators

**Process Health**:
- Clear phase boundaries maintained
- Todo list accurately reflects state
- No assumptions stated as facts
- Corrections integrated immediately

**Output Quality**:
- New code follows existing patterns
- Changes are minimal and focused
- All QA checks pass
- Documentation captures knowledge

**Partnership Health**:
- User feels informed throughout
- No surprises at completion
- Learning captured for future sessions
- Mutual understanding of what was achieved

---

**Last Updated**: 2026-01-21
**Origin**: Extracted from successful Priority #1 Full Blockchain History implementation session
