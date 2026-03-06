# Scientific Method - Guide

## TDD Integration with Scientific Method

The Scientific Method naturally aligns with Test-Driven Development:

| Scientific Method Stage | TDD Stage              | Description                         |
| ----------------------- | ---------------------- | ----------------------------------- |
| **Prediction**          | Write Tests            | Define expected behavior as tests   |
| **Experimentation**     | RED → GREEN → REFACTOR | Implement solution to pass tests    |
| **Analysis**            | Validate Tests Pass    | Confirm solution meets expectations |
| **Conclusion**          | Accept/Iterate         | Ship or try new hypothesis          |

**Key Insight**: Prediction stage = Writing tests. Experimentation stage = Making tests pass.

This approach ensures every implementation is:

- **Hypothesis-driven**: Clear reason for every change
- **Testable**: Success criteria defined upfront
- **Measurable**: Objective validation of outcomes
- **Documented**: Knowledge captured for future reference

---

## Integration with Boilerplate

### Agent Integration

All agents reference this Scientific Method framework:

**Orchestrator** (`.claude/agents/orchestrator.md`):

- Ensures all tasks follow 7-stage process
- Validates each stage before proceeding
- Escalates if stages skipped

**Test-Writer** (`.claude/agents/test-writer.md`):

- Executes Stage 4 (Prediction): Write tests first
- Enforces TDD cycle: RED → GREEN → REFACTOR
- Validates test coverage meets criteria

**Code-Reviewer** (`.claude/agents/code-reviewer.md`):

- Validates Stage 5 (Experimentation): Implementation matches tests
- Checks Stage 6 (Analysis): Tests pass, metrics met
- Ensures Stage 7 (Conclusion): Documentation complete

**Architecture-Designer** (`.claude/agents/architecture-designer.md`):

- Applies Scientific Method to design decisions
- Creates ADRs following Stage 7 format
- Validates hypotheses with proof-of-concept

**Database** (`.claude/agents/database.md`):

- Uses Scientific Method for query optimization
- Stages: Observe slow query → Hypothesize index → Test performance → Analyze results

**Refactoring-Migration** (`.claude/agents/refactoring-migration.md`):

- Treats refactoring as experiment
- Hypothesis: Refactor will improve maintainability
- Prediction: Tests pass, no regressions
- Experiment: Implement refactor
- Analysis: Measure impact
- Conclusion: Accept or revert

### Command Integration

**`/task` Command**:

- Stage 1-2: Problem identification and scoping
- Stage 3: Solution approach in task spec
- Stage 4: Test strategy definition
- Stage 5: Implementation plan
- Stage 6-7: Validation and handoff

**`/plan` Command**:

- Stage 1-2: Architecture problem definition
- Stage 3: Design hypothesis
- Stage 4: Architecture validation criteria
- Stage 5: Proof-of-concept implementation
- Stage 6-7: Architecture decision and documentation

**`/review` Command**:

- Stage 1: Observe code quality issues
- Stage 2: Define review scope
- Stage 3: Hypothesize improvements
- Stage 4: Define review criteria
- Stage 5: Conduct review
- Stage 6: Analyze findings
- Stage 7: Provide recommendations

---

## Anti-Patterns (What NOT to Do)

### ❌ Anti-Pattern 1: Skip Observation

**Problem**: Jump straight to solution without understanding problem
**Example**: "Let's add caching" without knowing why it's slow
**Fix**: Always start with Stage 1 (Observation)

### ❌ Anti-Pattern 2: Vague Hypothesis

**Problem**: "This should work" without specific predictions
**Example**: "Using Redis will make it faster"
**Fix**: "Redis will reduce response time from 2s to 200ms for 95% of requests"

### ❌ Anti-Pattern 3: No Test Design

**Problem**: Implement without defining success criteria
**Example**: Deploy and hope it works
**Fix**: Always define measurable success criteria in Stage 4

### ❌ Anti-Pattern 4: Ignore Results

**Problem**: Don't analyze if hypothesis was correct
**Example**: Deploy and forget
**Fix**: Always analyze results in Stage 6

### ❌ Anti-Pattern 5: No Learning

**Problem**: Don't document what worked/didn't work
**Example**: Repeat same mistakes
**Fix**: Always document learnings in Stage 7

### ❌ Anti-Pattern 6: Skip Stages for "Simple" Tasks

**Problem**: "This is too simple for scientific method"
**Example**: Quick bug fix without process
**Fix**: Even 5-minute tasks benefit from abbreviated process

---

## Examples

### Example 1: Bug Fix (5 minutes)

**Stage 1 - Observation**:

```
OBSERVATION: User reports "Cannot save profile changes"
EVIDENCE: Error log shows "ValidationError: email format invalid"
CONTEXT: Started after email validation update
```

**Stage 2 - Question**:

```
QUESTION: Why is email validation failing for valid emails?
SCOPE: Profile save functionality only
CONSTRAINTS: Must maintain existing email format requirements
```

**Stage 3 - Hypothesis**:

```
HYPOTHESIS: Email regex pattern is too restrictive
PREDICTION: Current regex rejects valid emails with special characters
RISK: May allow invalid emails, mitigated by testing
```

**Stage 4 - Prediction**:

```
PREDICTION: Updated regex will accept valid emails and reject invalid ones
SUCCESS CRITERIA: All valid email formats save successfully
TEST: Try saving with various email formats
```

**Stage 5 - Experiment**:

```
EXPERIMENT: Update email regex pattern
IMPLEMENTATION: Changed from /^[^@]+@[^@]+$/ to /^[^\s@]+@[^\s@]+\.[^\s@]+$/
TEST: Tried saving with test@example.com, user+tag@domain.co.uk
```

**Stage 6 - Analysis**:

```
ANALYSIS: Hypothesis CORRECT
RESULTS: All valid emails now save successfully
UNEXPECTED: None
CONCLUSION: Fix works as predicted
```

**Stage 7 - Conclusion**:

```
CONCLUSION: ACCEPT - Deploy fix
DECISION: Deploy to production
LEARNINGS: Email regex was too simple, need more comprehensive pattern
NEXT STEPS: Add comprehensive email validation tests
```

### Example 2: Feature Development (1 week)

**Stage 1 - Observation**:

```
OBSERVATION: Users need to reset passwords but current flow is broken
EVIDENCE: 40% of password reset attempts fail, support tickets increasing
CONTEXT: Current system sends reset links to old email addresses
```

**Stage 2 - Question**:

```
QUESTION: How can we implement secure password reset that works reliably?
SCOPE: Password reset flow, not account recovery
CONSTRAINTS: Must be secure, work with existing auth system
```

**Stage 3 - Hypothesis**:

```
HYPOTHESIS: JWT-based reset tokens with email verification will solve the problem
PREDICTION: 95% of reset attempts will succeed, no security vulnerabilities
RISK: JWT complexity, mitigated by using proven library
```

**Stage 4 - Prediction**:

```
PREDICTION: New reset flow will be more reliable and secure
SUCCESS CRITERIA:
- 95% of reset attempts succeed
- No security vulnerabilities found in audit
- User satisfaction >4.0/5.0
TEST: Implement in staging, run security audit, user testing
```

**Stage 5 - Experiment**:

```
EXPERIMENT: Implement JWT-based password reset
IMPLEMENTATION:
- Generate JWT tokens with 1-hour expiry
- Send reset emails with secure links
- Validate tokens on reset page
- Update password in database
TEST: 100 test users, security audit, load testing
```

**Stage 6 - Analysis**:

```
ANALYSIS: Hypothesis PARTIALLY CORRECT
RESULTS:
- 98% of reset attempts succeed (exceeded prediction)
- Security audit found 1 minor issue (fixed)
- User satisfaction 4.2/5.0 (met criteria)
UNEXPECTED: Some users prefer SMS over email
```

**Stage 7 - Conclusion**:

```
CONCLUSION: ACCEPT - Deploy with improvements
DECISION: Deploy to production, add SMS option in next iteration
LEARNINGS: JWT approach works well, users want multiple reset options
NEXT STEPS: Monitor production, plan SMS integration
```

### Example 3: Performance Optimization (2 days)

**Stage 1 - Observation**:

```
OBSERVATION: API response times are slow during peak hours
EVIDENCE: p95 latency = 2.5s (target <500ms), database CPU at 90%
CONTEXT: Slow queries identified in user search functionality
```

**Stage 2 - Question**:

```
QUESTION: How can we reduce API response times to <500ms during peak hours?
SCOPE: User search API only, not other endpoints
CONSTRAINTS: Must maintain search accuracy, no breaking changes
```

**Stage 3 - Hypothesis**:

```
HYPOTHESIS: Adding database indexes and query optimization will solve the problem
PREDICTION: Response time will drop to <500ms, database CPU <70%
RISK: Index maintenance overhead, mitigated by monitoring
```

**Stage 4 - Prediction**:

```
PREDICTION: Optimized queries will meet performance targets
SUCCESS CRITERIA:
- p95 latency <500ms
- Database CPU <70%
- Search accuracy maintained
TEST: Deploy to staging, run load test for 2 hours
```

**Stage 5 - Experiment**:

```
EXPERIMENT: Add indexes and optimize queries
IMPLEMENTATION:
- Added composite index on (status, created_at, name)
- Optimized query to use index scan instead of table scan
- Added query result caching
TEST: 2-hour load test with production-like data
```

**Stage 6 - Analysis**:

```
ANALYSIS: Hypothesis CORRECT
RESULTS:
- p95 latency = 350ms (met target)
- Database CPU = 45% (exceeded target)
- Search accuracy = 99.8% (maintained)
UNEXPECTED: Caching provided additional 20% improvement
```

**Stage 7 - Conclusion**:

```
CONCLUSION: ACCEPT - Deploy to production
DECISION: Deploy optimization, enable caching
LEARNINGS: Indexes + caching combination very effective
NEXT STEPS: Monitor production, consider caching other slow queries
```

---

## Summary

### Key Takeaways

1. **Always Start with Evidence**: Never guess, always observe first
2. **Make Testable Predictions**: Every hypothesis must be measurable
3. **Document Everything**: Knowledge is your most valuable asset
4. **Learn from Results**: Every experiment teaches something
5. **Iterate and Improve**: Use learnings to make better hypotheses

### Best Practices

- **Use abbreviated process for small tasks** (5-15 minutes)
- **Use full process for complex tasks** (1+ days)
- **Always document learnings** for future reference
- **Share knowledge** with team through ADRs and documentation
- **Review and improve** your scientific method application regularly

### Integration Points

- **TDD**: Prediction stage = Write tests
- **Code Review**: Validate all stages completed
- **Architecture**: Use for design decisions
- **Debugging**: Systematic approach to problem solving
- **Learning**: Document what works and what doesn't

---

**Quick Reference**: `.context/0.SOP/scientific-method-quick-ref.md`

---

**Remember**: The Scientific Method is not just a process—it's a mindset. Apply it consistently to become a more effective developer.
