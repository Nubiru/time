# /research Command

## Purpose
Deep investigation with web access to research external information, investigate technologies, compare approaches, and synthesize findings into comprehensive research reports.

## Usage
```
/research [topic or question]
```

## Examples
```
/research best practices for JWT implementation 2024
/research PostgreSQL vs MongoDB for user data storage
/research latest Express.js security vulnerabilities
/research current state of Web3 authentication
/research React Server Components benefits and drawbacks
/research Kubernetes vs Docker Swarm for microservices
/research OWASP Top 10 security recommendations for Node.js
```

## What Happens

### Step 1: Research Phase (30-60 seconds)
1. **Web Search**: Use `web_search` tool to gather external information
   - Search for best practices, documentation, comparisons
   - Look for recent articles, official docs, Stack Overflow discussions
   - Prioritize authoritative sources (official docs, well-known blogs, academic papers)

2. **Multi-Source Synthesis**: Collect from 3-5 different sources
   - Ensure diversity of perspectives
   - Cross-reference recommendations
   - Identify common patterns and disagreements

### Step 2: Internal Cross-Reference (10-20 seconds)
1. Check against internal standards:
   - `.context/standards/protocols/DESIGN_PATTERNS.md` - Pattern alignment
   - `.context/standards/protocols/CONVENTIONS.md` - Coding standards
   - `.context/standards/protocols/QUALITY_GATES.md` - Quality requirements
   - `docs/architecture/ARCHITECTURE.md` - Project architecture (if exists)

2. Identify conflicts or synergies with current project

### Step 3: Analysis & Synthesis (20-30 seconds)
1. **Pros/Cons Analysis**: For each option or approach
2. **Trade-offs**: Performance, security, maintainability, cost
3. **Recommendation**: Based on project context
4. **References**: URLs, dates, source credibility

### Step 4: Report Generation (10-15 seconds)
1. Generate comprehensive research report
2. Save to `.context/active/right-now/RESEARCH_[Topic]_[Date].md`
3. Return summary to user

**Total Time**: 1-2 minutes

## Output

**Research Report File**:
`.context/active/right-now/RESEARCH_[Topic]_[Date].md`

**Report Structure**:
```markdown
# Research Report: [Topic]

**Date**: [YYYY-MM-DD]
**Researcher**: Cursor Agent (with web access)
**Status**: COMPLETE

## Executive Summary
[2-3 sentence high-level findings]

## Research Question
[Original question or topic]

## Findings

### Source 1: [Source Name]
**URL**: [https://...]
**Date**: [publication date]
**Credibility**: [Official docs / Industry blog / Academic / Community]

**Key Points**:
- [Key point 1]
- [Key point 2]

### Source 2: [Source Name]
[...]

## Synthesis

### Common Recommendations
[What all sources agree on]

### Divergent Opinions
[Where sources disagree and why]

### Industry Trends
[Current direction, what's gaining traction]

## Pros & Cons

### Option A: [Approach 1]
**Pros**:
- [Benefit 1]
- [Benefit 2]

**Cons**:
- [Drawback 1]
- [Drawback 2]

### Option B: [Approach 2]
[...]

## Recommendation for This Project

**Recommended Approach**: [A / B / Hybrid]

**Rationale**:
1. [Reason 1 based on project context]
2. [Reason 2 based on findings]
3. [Reason 3 considering trade-offs]

**Alignment with Internal Standards**:
- `.context/standards/protocols/DESIGN_PATTERNS.md`: [alignment notes]
- `.context/standards/protocols/QUALITY_GATES.md`: [compliance notes]

## Implementation Considerations
[Practical notes for if/when this is implemented]

## Next Steps
[What to do after this research]

## References
1. [Source 1 full citation]
2. [Source 2 full citation]
[...]

---

**Research Validity**: [Date], valid for ~3-6 months (tech changes fast)
**Follow-Up Research**: [Topics requiring deeper investigation]
```

## When to Use

**Use `/research` when**:
- ✅ Need external information (not in codebase/docs)
- ✅ Evaluating technology options (library, framework, tool)
- ✅ Looking for best practices (industry standards)
- ✅ Security considerations (vulnerabilities, OWASP recommendations)
- ✅ Performance optimization techniques
- ✅ Architectural pattern comparisons
- ✅ Current state of technology (what's the latest?)
- ✅ Before making major technical decisions

**Don't use `/research` when**:
- ❌ Information is in internal docs (use `/ask` instead)
- ❌ Need immediate decision (use `/consider` for analysis)
- ❌ Want formal proposal (use `/propose` after research)
- ❌ Ready to implement (use `/task` with research as input)
- ❌ Simple internal questions (use `/ask`)

## Web Access

**Tool**: `web_search` ⭐

**Permissions**: YES - This command has web access

**Search Strategy**:
1. Use specific, technical queries
2. Include year for time-sensitive topics (e.g., "2024" for latest)
3. Search multiple perspectives (official docs, blogs, discussions)
4. Verify source credibility

**Source Credibility Hierarchy**:
1. **Official Documentation** (highest credibility)
   - Language/framework official docs
   - OWASP, NIST, W3C standards

2. **Industry Leaders** (high credibility)
   - Martin Fowler, Kent Beck, Google Engineering blogs
   - Microsoft Docs, Mozilla MDN
   - Thoughtworks Technology Radar

3. **Community Consensus** (medium-high credibility)
   - Stack Overflow (high-voted answers)
   - GitHub discussions (popular repos)
   - Dev.to, Medium (verified authors)

4. **General Articles** (medium credibility)
   - Tech blogs (less established)
   - Tutorial sites
   - Personal blogs (cross-reference needed)

## Integration

**References**:
- `.context/standards/protocols/DESIGN_PATTERNS.md` - Cross-reference patterns
- `.context/standards/protocols/CONVENTIONS.md` - Align with coding standards
- `.context/standards/protocols/QUALITY_GATES.md` - Check compliance
- `docs/architecture/ARCHITECTURE.md` - Project context

**Output Location**:
- `.context/active/right-now/` - Research reports stored here

**Workflow Integration**:
```
/research [topic]  →  Research report generated
  ↓
/consider [should we use option A or B?]  →  Decision analysis
  ↓
/propose [implementation approach]  →  Formal proposal
  ↓
/task [implement per proposal]  →  Execution
```

**Agents**:
- Orchestrator (coordinates research)
- (Future) Research Specialist agent (dedicated to this command)

## Examples

### Example 1: Technology Comparison

**User Request**:
```
/research PostgreSQL vs MongoDB for user data storage
```

**Research Process**:
1. Web searches:
   - "PostgreSQL vs MongoDB 2024 comparison"
   - "PostgreSQL advantages user data"
   - "MongoDB advantages user data"
   - "PostgreSQL MongoDB performance benchmarks"

2. Sources found (example):
   - Official PostgreSQL docs
   - Official MongoDB docs
   - DigitalOcean comparison article
   - Stack Overflow discussion (2024)
   - AWS database selection guide

3. Internal cross-reference:
   - Check current architecture (relational? document?)
   - Check CONVENTIONS.md for data modeling preferences

**Output File**: `.context/active/right-now/RESEARCH_PostgreSQL_vs_MongoDB_2025-10-12.md`

**Summary to User**:
```
✅ Research complete: PostgreSQL vs MongoDB for user data

**Key Findings**:
- PostgreSQL: Better for structured data, ACID compliance, complex queries
- MongoDB: Better for flexible schema, horizontal scaling, rapid iteration

**Recommendation for This Project**: PostgreSQL

**Rationale**:
1. User data is structured (name, email, roles)
2. ACID compliance critical for authentication
3. Complex relational queries needed (user-role-permission)
4. Team familiar with SQL (lower learning curve)

**Full Report**: .context/active/right-now/RESEARCH_PostgreSQL_vs_MongoDB_2025-10-12.md

**Next Steps**:
- Review full report
- Use `/consider` to analyze trade-offs in project context
- Use `/propose` to create implementation plan
```

---

### Example 2: Security Best Practices

**User Request**:
```
/research OWASP Top 10 security recommendations for Node.js 2024
```

**Research Process**:
1. Web searches:
   - "OWASP Top 10 2024"
   - "Node.js security best practices OWASP"
   - "Express.js security middleware 2024"
   - "JWT security vulnerabilities Node.js"

2. Sources:
   - OWASP official site
   - Node.js Security Working Group
   - Snyk security blog
   - npm security advisories

3. Cross-reference:
   - Check current auth implementation
   - Validate against QUALITY_GATES.md security requirements

**Output**: Comprehensive security report with actionable recommendations

---

### Example 3: Framework Evaluation

**User Request**:
```
/research React Server Components benefits and drawbacks
```

**Research Process**:
1. Web searches:
   - "React Server Components official docs"
   - "React Server Components pros cons 2024"
   - "React Server Components performance benchmarks"
   - "When to use React Server Components"

2. Sources:
   - React official docs
   - Vercel blog (creators of Next.js)
   - Kent C. Dodds blog
   - Real-world case studies

3. Synthesis:
   - Benefits: Performance, SEO, reduced client bundle
   - Drawbacks: Complexity, limited ecosystem, learning curve
   - Trade-offs: Server dependency vs client flexibility

**Output**: Report with recommendation based on project type and team experience

---

## Success Metrics

**Research Quality**:
- ≥3 authoritative sources per research report
- ≥80% of sources are high or medium-high credibility
- Recommendations aligned with project context
- All claims have citations

**Coverage**:
- Common recommendations identified
- Divergent opinions noted and explained
- Trade-offs clearly articulated
- Implementation considerations included

**Timeliness**:
- Research completes in <2 minutes
- Sources dated within last 2 years (or noted if older)

**Actionability**:
- Clear recommendation provided
- Next steps defined
- Integration with other commands smooth (`/consider` → `/propose` → `/task`)

## Tool Permissions

- **Read**: All SOP docs, system docs, codebase (for context)
- **Write**: `.context/active/right-now/RESEARCH_*.md`
- **Web Access**: YES ⭐ (`web_search` tool)
- **Execute**: None (research only, no code changes)
- **Escalate**: To `/consider` for decision analysis, `/propose` for formal proposal

## Anti-Patterns

### ❌ Anti-Pattern 1: Research Without Context
**Problem**: Researching without checking project needs
**Example**: Researching every database without knowing requirements
**Why Bad**: Wastes time, irrelevant recommendations
**Fix**: Understand project context first, then research specific options

### ❌ Anti-Pattern 2: Single-Source Research
**Problem**: Relying on one source
**Example**: Only checking official docs, missing community feedback
**Why Bad**: Incomplete picture, missing real-world drawbacks
**Fix**: Always use 3-5 diverse sources

### ❌ Anti-Pattern 3: Outdated Research
**Problem**: Using old sources (>2 years in fast-moving tech)
**Example**: 2019 article on React hooks
**Why Bad**: Tech evolves fast, old advice may be obsolete
**Fix**: Prioritize recent sources, note publication dates

### ❌ Anti-Pattern 4: Research Without Recommendation
**Problem**: Presenting facts without conclusion
**Example**: "Option A has X, Option B has Y" (no recommendation)
**Why Bad**: User still doesn't know what to choose
**Fix**: Always provide recommendation based on project context

## Validation Checklist

**Before Starting Research**:
- [ ] Research question is clear
- [ ] Not already documented internally (checked with `/ask`)
- [ ] Relevant to current project needs

**During Research**:
- [ ] 3-5 sources collected
- [ ] Sources are credible (official docs, industry leaders)
- [ ] Sources are recent (<2 years for most tech)
- [ ] Multiple perspectives represented

**Before Generating Report**:
- [ ] Common recommendations identified
- [ ] Divergent opinions explained
- [ ] Trade-offs clearly articulated
- [ ] Recommendation aligns with project context
- [ ] All claims have citations
- [ ] Next steps defined

## Quick Reference

| Research Type | Sources Needed | Time | Web Search Examples |
|---------------|----------------|------|---------------------|
| Technology comparison | 4-5 | 90s | "[Tech A] vs [Tech B] 2024" |
| Best practices | 3-4 | 60s | "[Topic] best practices [year]" |
| Security | 3-4 | 90s | "OWASP [topic]", "[framework] security" |
| Performance | 4-5 | 90s | "[topic] performance benchmarks" |
| Framework evaluation | 4-5 | 90s | "[framework] pros cons [year]" |

---

**Last Updated**: 2025-10-14
**Command Type**: Research with Web Access ⭐
**Tool Permissions**: Read (all docs), Write (active/right-now/), Web Access (web_search tool), Escalate (to /consider, /propose)
**Integration**: Part of research → consider → propose → task workflow

