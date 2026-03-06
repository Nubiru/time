# Crisis Response Protocol

**Version**: 1.0
**Last Updated**: 2025-10-23
**Applies To**: ALL agents during active incidents

---

## Purpose

Define appropriate agent behavior during crises vs normal work. **SOLVE FIRST, document AFTER.**

---

## What is a Crisis?

### Definition

A crisis is when:
- User is actively blocked and cannot continue work
- Production system is down or degraded
- Build is broken and blocking team
- Critical bug discovered in production
- Time-sensitive deadline pressure

### Crisis Indicators (User Language)

User says:
- "not working"
- "broken"
- "blocked"
- "urgent"
- "need this fixed NOW"
- "production is down"
- "build failed"
- "can't deploy"

### What is NOT a Crisis

**Normal Work**:
- Planning future features
- Architectural discussions
- Code refactoring (non-urgent)
- Documentation improvements
- Performance optimization (non-critical)
- Research and exploration
- Proposals and RFC creation

**Rule of Thumb**: If user can wait 1+ hours for response, it's not a crisis.

---

## Crisis Behavior: SOLVE FIRST

### Time Allocation

- **90%**: Solving the problem
- **10%**: Documentation (only if essential)

### Action Priority

1. **Diagnose** (5 minutes max)
   - Read error message
   - Check logs
   - Review relevant code
   - Identify root cause

2. **Solution** (immediate)
   - Provide fix code
   - Give step-by-step commands
   - Explain briefly WHY fix works
   - Offer quick verification

3. **Verify** (2 minutes)
   - User tests fix
   - Confirm problem resolved
   - If not resolved: Iterate quickly

4. **Document** (optional, AFTER resolution)
   - Update `.context/` files if new knowledge gained
   - Log to LEARNING_LOG.md if mistake was made
   - Add prevention measures

### What to Do in Crisis

✅ **DO**:
- Read error messages immediately
- Check project files for context
- Provide concrete, actionable fixes
- Offer command-line instructions
- Give brief explanations
- Verify solution works

❌ **DON'T**:
- Create PROPOSAL_*.md files
- Write comprehensive documentation
- Suggest architectural refactors (unless directly solving issue)
- Generate project plans
- Discuss alternative approaches (unless current fix fails)
- Ask clarifying questions (unless absolutely necessary)

### Examples

#### Example 1: Build Failure

**❌ WRONG**:
```
User: "The build is broken! Production deploy is blocked!"
Agent: "Let me create a comprehensive DEPLOYMENT_TROUBLESHOOTING.md
       document to help debug this..."
[creates 500-line documentation file]
```

**✅ CORRECT**:
```
User: "The build is broken! Production deploy is blocked!"
Agent: "Let me check the error... I see: 'MODULE_NOT_FOUND: @/lib/auth'.
       Fix: Run `npm install` - looks like dependencies aren't installed.
       Then run `npm run build` again."
```

#### Example 2: Production Bug

**❌ WRONG**:
```
User: "Users can't log in! Site is broken!"
Agent: "Let's discuss the authentication architecture.
       I'll create an ADR for auth patterns..."
```

**✅ CORRECT**:
```
User: "Users can't log in! Site is broken!"
Agent: "Checking auth code... Found issue in lib/auth.ts:45
       - session check is calling wrong method.
       Fix: Change `auth.validate()` to `auth()`.
       Deploy this fix immediately."
```

#### Example 3: Database Connection

**❌ WRONG**:
```
User: "Can't connect to database, app won't start!"
Agent: "Let me write a comprehensive INFRASTRUCTURE.md
       documenting your database setup..."
```

**✅ CORRECT**:
```
User: "Can't connect to database, app won't start!"
Agent: "Checking .env file... DATABASE_URL is missing.
       Add this line to .env:
       DATABASE_URL='postgresql://user:pass@localhost:5432/dbname'
       (Replace with your actual credentials)
       Then restart the app."
```

---

## Normal (Non-Crisis) Behavior

### When NOT in crisis mode:

✅ **DO**:
- Read all relevant documentation
- Check `.context/` files thoroughly
- Propose comprehensive solutions
- Create detailed documentation
- Discuss alternative approaches
- Follow all quality gates
- Write clear ADRs for decisions
- Plan before implementing

✅ **ALSO DO**:
- Ask clarifying questions
- Research best practices
- Consider architectural implications
- Validate assumptions with user
- Create proposals for complex changes

### Time Allocation (Normal Mode)

- **20%**: Understanding requirements
- **30%**: Planning and design
- **30%**: Implementation
- **20%**: Documentation and review

---

## Mode Detection

### How to Detect Crisis Mode

**Check user message for:**
1. Urgency words: "urgent", "broken", "blocked", "NOW"
2. Problem statements: "not working", "failed", "can't"
3. Production context: "prod", "live", "users affected"
4. Time pressure: "ASAP", "immediately", "emergency"

**If 2+ indicators present → Crisis Mode**

### How to Detect Normal Mode

**Check user message for:**
1. Planning words: "should we", "how would", "what if"
2. Future tense: "when we implement", "for the next version"
3. Questions: "why", "how does", "what is"
4. No time pressure language

**If user is exploratory → Normal Mode**

---

## Post-Crisis Protocol

### After Problem Resolved

1. **Confirm** resolution with user
   ```
   Agent: "Is the issue resolved? Can you confirm [X] is working now?"
   ```

2. **Optionally** document learnings
   - If new pattern discovered → Update `.context/1.system/`
   - If mistake made → Log to `LEARNING_LOG.md`
   - If edge case found → Add to documentation

3. **Propose** improvements (NOT during crisis)
   ```
   Agent: "Now that the crisis is resolved, I notice [X] could be improved
          to prevent this in the future. Should I create a proposal?"
   ```

### Learning Log Entry (If Applicable)

If the crisis revealed a knowledge gap or mistake:

```markdown
## [Date] - [Crisis Topic]

**Crisis**: [Brief description of what went wrong]
**Resolution**: [What fixed it]
**Root Cause**: [Why it happened]
**Prevention**: [What was updated to prevent recurrence]
**Time to Resolve**: [X minutes]
```

Example:
```markdown
## 2025-10-23 - Missing Environment Variable

**Crisis**: Production deploy failed - app couldn't start
**Resolution**: Added DATABASE_URL to .env file
**Root Cause**: .env.example wasn't copied to .env in deployment
**Prevention**: Added deployment checklist to INFRASTRUCTURE.md
**Time to Resolve**: 5 minutes
```

---

## Crisis Escalation

### When to Escalate to Human

**Immediate escalation if:**
- Fix attempts fail 3 times
- Root cause unclear after 10 minutes
- Multiple systems affected
- Data loss risk identified
- Security breach suspected

**How to escalate:**
```
Agent: "I've tried [X, Y, Z] but the issue persists. This may require
       human expertise. Here's what I know:
       - Error: [error message]
       - Attempts: [what was tried]
       - Status: [current system state]
       Recommend escalating to [senior developer / DevOps / security team]."
```

---

## Quality Gates During Crisis

### Gates ACTIVE in Crisis:
- ✅ Evidence Protocol (still check sources)
- ✅ Solution correctness (fix must work)
- ✅ Security (don't create vulnerabilities)

### Gates SUSPENDED in Crisis:
- ⏸️ Comprehensive documentation
- ⏸️ ADR creation
- ⏸️ Architectural review
- ⏸️ Code style perfection
- ⏸️ Comprehensive testing (quick test is OK)

**Rationale**: Speed matters in crisis. Quality documentation comes after.

---

## Integration with Evidence Protocol

**Evidence Protocol STILL APPLIES in crisis:**

- ✅ Cite sources for technical claims
- ✅ State confidence levels
- ✅ Distinguish facts from assumptions
- ✅ Say "I don't know" if unsure

**But adjusted for speed:**
- Quick source check (not exhaustive research)
- Provide solution first, explain source after
- Action-oriented (not discussion-oriented)

---

## Examples: Crisis vs Normal

### Scenario 1: Database Query Slow

**Crisis Version** (production affected):
```
User: "Dashboard is timing out! Users can't access reports!"
Agent: "Checking query... Found missing index on orders.created_at.
       Run: CREATE INDEX idx_orders_created_at ON orders(created_at);
       This will fix the timeout immediately."
```

**Normal Version** (planning optimization):
```
User: "Dashboard feels slow, how can we optimize?"
Agent: "Let me analyze query patterns... [reads code]
       I see 3 optimization opportunities:
       1. Add index on orders.created_at
       2. Implement query result caching
       3. Paginate large result sets
       Should I create a proposal with benchmarks?"
```

### Scenario 2: Authentication Not Working

**Crisis Version** (users blocked):
```
User: "Login is broken! Nobody can access the app!"
Agent: "Checking auth.ts... Session secret is undefined.
       Add to .env: SESSION_SECRET='your-secret-here'
       Restart app. Login will work."
```

**Normal Version** (planning auth system):
```
User: "How should we implement authentication?"
Agent: "Let me review options... [research]
       I recommend NextAuth.js because:
       1. [reasons]
       2. [reasons]
       Should I create an ADR documenting this decision?
       Then I'll implement it following our auth patterns."
```

---

## Red Flags: Wrong Mode Activated

### You're in WRONG mode if:

**Crisis mode but shouldn't be:**
- Creating minimal docs when user wants thorough explanation
- Skipping validation when user is exploring options
- Rushing solution when user is learning

**Normal mode but shouldn't be:**
- Writing documentation when production is down
- Asking clarifying questions when fix is obvious
- Discussing alternatives when user needs solution NOW

**Self-check**: Ask yourself "Is the user actively blocked RIGHT NOW?" If yes → Crisis. If no → Normal.

---

## Summary

**Crisis = SOLVE FIRST**
- 90% solution, 10% documentation
- Action over discussion
- Fix immediately, explain briefly
- Document after (optional)

**Normal = PLAN FIRST**
- 20% understanding, 30% planning, 30% implementation, 20% docs
- Discussion and alternatives welcome
- Comprehensive documentation
- Follow all quality gates

**When in doubt: Ask "Is the user blocked right now?" Yes = Crisis. No = Normal.**

---

**Remember: The best documentation in a crisis is a working system. Document after the fire is out.**
