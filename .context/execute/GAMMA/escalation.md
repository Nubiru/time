# Escalation: Wrong Book — "The Ends of the World"

**Agent**: GAMMA
**Date**: 2026-03-07
**Severity**: BLOCKED (task-level, not session-level)

## Issue

The PDF at `~/Desktop/temp/time/new/The Ends of the World.pdf` is **NOT** Peter Brannen's geological mass extinction book.

**Expected**: Peter Brannen, "The Ends of the World: Volcanic Apocalypses, Lethal Oceans, and Our Quest to Understand Earth's Past Mass Extinctions" (2017, Ecco/HarperCollins). Contains Big Five extinction data, CO2/temperature numbers, recovery timescales.

**Actual**: Déborah Danowski and Eduardo Viveiros de Castro, "The Ends of the World" (2016, Polity Press). A philosophical/anthropological essay about Anthropocene discourse, Gaia theory, Bruno Latour, and speculative realism. Contains zero geological data.

## Impact

- Q1-Q5 in the extraction task CANNOT be answered from this PDF
- Tracks 20.4 (climate_history), 29.3 (earth_drama), 13.3 (geological timeline) lose their planned data enrichment

## Action Needed

Gabriel: The Brannen book needs to be acquired separately. The Danowski/Viveiros de Castro book may have value for Track 29 (earth_drama storytelling) but does not contain extinction science data.

- [ ] Acquire correct Brannen PDF and place in `~/Desktop/temp/time/inbox/`
- [ ] Optionally: rename existing PDF to clarify it's Danowski, not Brannen

## GAMMA Response

Moving to next P1 extraction target. This task marked BLOCKED.
