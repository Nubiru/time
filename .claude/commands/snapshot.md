# /snapshot - Save Context Checkpoint

Create a named snapshot of your current agent context state for later restoration.

## When to Use

- **Before risky operations**: Major refactoring, architecture changes
- **Before experiments**: Trying new approach, testing hypothesis
- **Milestone checkpoints**: End of sprint, after major feature
- **Known good states**: After tests pass, after successful deployment

## What It Does

1. **Captures** complete agent context:
   - All context entries
   - Token counts
   - Active tasks
   - Recent decisions

2. **Stores** with metadata:
   - Timestamp
   - Purpose/description
   - Tags (optional)
   - Agent ID
   - Context size

3. **Manages** snapshots:
   - Auto-expires after 30 days
   - Max 50 snapshots per agent
   - Searchable by name/tags/date

## Usage

```bash
# Create snapshot with descriptive name
/snapshot pre-refactor

# Create with purpose description
/snapshot pre-api-redesign --purpose "Before breaking API changes"

# Create with tags
/snapshot milestone-v1 --tags release,stable,production

# List all snapshots
/snapshot list

# Show snapshot details
/snapshot show pre-refactor

# Compare two snapshots
/snapshot diff pre-refactor post-refactor

# Delete old snapshot
/snapshot delete old-experiment
```

## Example Output

```
📸 Snapshot Created

Name: pre-refactor
Purpose: Before authentication refactoring
Timestamp: 2025-11-15 14:45:23
Agent: orchestrator-main
Size: 42,891 tokens
Entries: 237 context entries

Tags: refactor, auth, risky
Expires: 2025-12-15 (30 days)

Restore with: /restore pre-refactor
Delete with: /snapshot delete pre-refactor
```

## Snapshot List Example

```
📸 Available Snapshots (5)

1. pre-refactor          (2 hours ago)  42KB  [refactor, auth]
2. milestone-sprint-23   (1 day ago)    38KB  [milestone, stable]
3. pre-database-migration (3 days ago)  45KB  [database, risky]
4. after-tests-pass      (5 days ago)   31KB  [testing, green]
5. weekly-backup         (7 days ago)   40KB  [backup, weekly]

Total: 196KB storage used
Auto-cleanup: Expires after 30 days
Max snapshots: 50 per agent
```

## Snapshot Comparison Example

```
📊 Snapshot Diff: pre-refactor → post-refactor

Changes:
  + Added 42 entries
  - Removed 18 entries
  ~ Modified 15 entries

Size: 42KB → 39KB (-7%)
Tokens: 42,891 → 39,102 (-8.8%)

Modified entries:
  - authentication.ts: 127 lines changed
  - user.service.ts: 43 lines changed
  - auth.test.ts: 89 lines changed
```

## Safety

- **Read-only**: Snapshots never modify current state
- **Isolated storage**: Separate from active context
- **Auto-expiration**: Old snapshots cleaned up automatically
- **Max limit**: Prevents unlimited storage growth

## Storage

- **Location**: `.context/agent-context/metrics/snapshots/`
- **Format**: JSON with compression
- **Max size**: 1MB per snapshot
- **Max count**: 50 per agent
- **Retention**: 30 days

## Performance

- **Create**: <100ms for 50KB context
- **Restore**: <200ms
- **Compare**: <50ms
- **List**: <10ms

## Related Commands

- `/restore <name>` - Restore from snapshot
- `/compact` - Compress before snapshotting (smaller snapshots)
- `/context show` - View current context before snapshot

## Technical Details

**Implementation**: `.context/agent-context/src/management/snapshot-system.ts`
**Storage**: File-based JSON (one file per snapshot)
**Metadata**: Separate index file for fast listing
**Cleanup**: Daily job removes expired snapshots

## Troubleshooting

**"Snapshot limit reached"**: Delete old snapshots or wait for auto-expiration
**"Snapshot too large"**: Run `/compact` first to reduce size
**"Cannot create snapshot"**: Check disk space and permissions
**"Snapshot not found"**: Use `/snapshot list` to see available snapshots
