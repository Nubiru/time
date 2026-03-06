# /restore - Restore from Snapshot

Restore agent context from a previously saved snapshot.

## When to Use

- **After failed experiment**: Tried new approach that didn't work
- **After breaking changes**: Refactoring broke tests, need to revert
- **Compare approaches**: Restore old state to compare with new
- **Recover from mistakes**: Accidentally deleted important context

## What It Does

1. **Loads** snapshot from storage
2. **Creates backup** of current context (before restore)
3. **Replaces** current context with snapshot state
4. **Reports** what changed

## Usage

```bash
# Restore from named snapshot
/restore pre-refactor

# List available snapshots first
/restore --list

# Restore with confirmation prompt
/restore pre-refactor --confirm

# Restore and compare changes
/restore pre-refactor --diff
```

## Example Output

```
🔄 Restoring from Snapshot

Snapshot: pre-refactor
Created: 2 hours ago (2025-11-15 12:45)
Size: 42,891 tokens
Purpose: Before authentication refactoring

⚠️ Current context will be backed up to:
   .context/agent-context/metrics/backups/pre-restore-2025-11-15-14-50.json

Changes to apply:
  + Will add 18 entries (old code)
  - Will remove 42 entries (refactored code)
  ~ Will modify 15 entries

Continue? [y/N]: y

✅ Restore Complete

Before: 39,102 tokens
After:  42,891 tokens
Change: +3,789 tokens (+9.7%)

Context restored to state from 2 hours ago.
Backup saved for safety.

Undo with: /restore pre-restore-2025-11-15-14-50
```

## Interactive Mode

```bash
/restore --list

📸 Available Snapshots (5)

1. pre-refactor          (2 hours ago)  42KB  [refactor, auth]
2. milestone-sprint-23   (1 day ago)    38KB  [milestone, stable]
3. pre-database-migration (3 days ago)  45KB  [database, risky]

Select snapshot to restore [1-3]: 1

Loading snapshot: pre-refactor...
[Shows changes and asks for confirmation]
```

## Safety

- **Always creates backup** before restoring
- **Confirmation required** by default (unless `--force`)
- **Shows diff** before applying changes
- **Reversible**: Backup can be restored

## Comparison Output

```
📊 Restore Preview

Current state vs. Snapshot "pre-refactor":

Files that will change:
  authentication.ts:
    Current:  235 lines (new implementation)
    Snapshot: 198 lines (old implementation)

  user.service.ts:
    Current:  142 lines
    Snapshot: 178 lines

Context entries:
  + 18 entries will be added
  - 42 entries will be removed
  ~ 15 entries will be modified

Tokens: 39,102 → 42,891 (+9.7%)
```

## Restore Workflow

```
Current State
     ↓
[Backup created automatically]
     ↓
Snapshot Loaded
     ↓
Changes Applied
     ↓
Restored State
```

If something goes wrong:
```bash
# Restore from the automatic backup
/restore pre-restore-2025-11-15-14-50
```

## Performance

- **Load snapshot**: <100ms
- **Create backup**: <100ms
- **Apply changes**: <50ms
- **Total**: <250ms

## Related Commands

- `/snapshot <name>` - Create new snapshot
- `/snapshot list` - View available snapshots
- `/snapshot diff <a> <b>` - Compare snapshots
- `/compact --undo` - Undo last compaction (similar to restore)

## Technical Details

**Implementation**: `.context/agent-context/src/management/snapshot-system.ts`
**Backup location**: `.context/agent-context/metrics/backups/`
**Snapshot location**: `.context/agent-context/metrics/snapshots/`
**Max backups**: 10 per agent (oldest deleted)

## Flags

- `--list` - Show available snapshots before restoring
- `--confirm` - Ask for confirmation (default)
- `--force` - Skip confirmation (dangerous!)
- `--diff` - Show detailed diff before restoring
- `--dry-run` - Show what would change without applying

## Troubleshooting

**"Snapshot not found"**: Use `/restore --list` to see available snapshots
**"Backup failed"**: Check disk space and permissions (restore aborted for safety)
**"Snapshot corrupted"**: File may be damaged, try different snapshot
**"Cannot restore"**: Check file permissions

## Safety Warnings

⚠️ **Use `--force` carefully** - Skips confirmation and safety checks
⚠️ **Check diff first** - Use `--diff` to preview changes
⚠️ **Old snapshots** - States >7 days old may not match current code
⚠️ **Context size** - Large snapshots may exceed token limits

## Best Practices

1. **Review diff** before restoring (`--diff` flag)
2. **Create snapshot** of current state before restore
3. **Test after restore** - Run tests to verify state
4. **Clean up** old snapshots regularly
5. **Name snapshots well** - Use descriptive names like `pre-refactor` not `snapshot1`
