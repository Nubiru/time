# /compact - Context Compression Command

Compress your current agent context to approximately 20% of its original size while retaining critical information.

## When to Use

- Context approaching token limits (80K+ tokens)
- Performance degrading due to large context
- Need to focus on current task without historical noise
- Before starting new major feature (clean slate)

## What It Does

1. **Analyzes** all context entries and scores them by:
   - **Recency**: Recent entries score higher
   - **Relevance**: Errors, decisions, active work score higher
   - **Position**: Current task context scores higher

2. **Compresses** by:
   - **Keeping**: High-priority entries (recent work, decisions, blockers)
   - **Summarizing**: Medium-priority entries (compress to key points)
   - **Removing**: Low-priority entries (old logs, completed tasks)

3. **Creates backup** before compacting (can undo)

4. **Reports**:
   - Tokens before/after
   - Compression ratio
   - Quality score (how much info retained)

## Usage

```bash
# Basic usage - compress current context
/compact

# Undo last compaction (restore backup)
/compact --undo

# Dry run (see what would be removed without doing it)
/compact --dry-run

# Aggressive compression (target 10% instead of 20%)
/compact --aggressive
```

## Example Output

```
🗜️ Context Compression Report

Before: 87,432 tokens (87% of limit)
After:  16,891 tokens (17% of limit)
Ratio:  80.7% reduction
Quality: 91/100 (excellent retention)

✅ Kept:
  - Last 50 messages (current work)
  - 12 architecture decisions
  - 3 active blockers
  - Error logs from last hour

📝 Summarized:
  - 200 older messages → 15 key points
  - Test results → pass/fail summary

🗑️ Removed:
  - 300+ old debug logs
  - Completed task checklists
  - Redundant clarifications

Backup saved to: .context/backups/compact-2025-11-15-14-30.json
Undo with: /compact --undo
```

## Safety

- **Always creates backup** before compacting
- **Undo capability** restores previous state
- **Quality score** warns if too much info lost
- **Dry run** lets you preview before executing

## Performance

- **Speed**: <500ms for 200KB context
- **Compression**: Typically 70-85% reduction
- **Quality**: Aims for 85%+ quality score

## Related Commands

- `/snapshot pre-compact` - Save checkpoint before compacting
- `/context show` - View current context size and metrics
- `/token budget` - Check token usage and budget

## Technical Details

**Implementation**: `.context/agent-context/src/management/compact-command.ts`
**Algorithm**: Position + recency + relevance scoring
**Backup location**: `.context/agent-context/metrics/backups/`
**Max backups**: 10 per agent (oldest deleted)

## Troubleshooting

**"Quality score too low"**: Use `--dry-run` first, may need manual cleanup
**"Nothing to compress"**: Context already small (<10KB)
**"Backup failed"**: Check disk space and permissions
**Lost important info**: Use `/compact --undo` immediately
