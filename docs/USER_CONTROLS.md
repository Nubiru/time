# Time — User Controls

**Last Updated**: 2026-03-22
**Status**: Living document. Updated when new interactions are added.

---

## Mouse / Touch

| Action | Effect |
|--------|--------|
| Click + drag | Rotate camera around scene |
| Scroll wheel | Zoom in / out (spring momentum) |
| Click anywhere | Dismiss Enter Time screen (first visit) |
| 3-finger tap | Open help panel (mobile) |

---

## Focus Modes — System Views

| Key | System | What It Shows |
|-----|--------|--------------|
| K | Kin Maya | Oracle cross (5-kin) + wavespell strip (13 cells) |
| I | I Ching | Hexagram lines (6 solid/broken) + judgment + trigram names |
| A | Astrology | Zodiac wheel + natal chart geometry + sign arcs |
| D | Human Design | Sun/Earth gate cards + bodygraph (when zoomed) |
| C | Chinese | Stem-branch + cycle year + animal |
| T | Kabbalah | Tree of Life (10 Sefirot + 22 paths) + 231 Gates mandala |
| Escape | Exit focus | Return to overview (all systems visible) |

---

## Time Navigation

| Key | Effect | Feel |
|-----|--------|------|
| Space | Pause / resume simulation | Instant toggle |
| Left arrow | -1 day | Smooth exponential approach (~0.3s) |
| Right arrow | +1 day | Smooth exponential approach |
| Up arrow | +1 month (30 days) | Smooth |
| Down arrow | -1 month | Smooth |
| Shift+Up | +1 year (365 days) | Smooth |
| Shift+Down | -1 year | Smooth |
| N | Return to NOW (real-time) | Smooth slide to current JD. Resets speed to 1.0. Exits birth sky if active. |
| 1 | Real-time speed | 1 day/day |
| 2 | Fast | 60x |
| 3 | Faster | 3600x |
| 4 | Day per second | 86400x |
| 5 | 10 days per second | 864000x |
| 0 | Pause | 0x |
| - | Reverse time direction | ×(-1) |

---

## Views & Panels

| Key | Panel/View | What It Does |
|-----|-----------|-------------|
| ? | Help panel | Keyboard shortcuts list (scrollable) |
| , | Settings panel | Theme, speed, social, location. Frosted glass right panel. |
| P | Birth form | "When did you arrive?" Date picker → 7-system identity. localStorage persistent. |
| B | Birth sky | Smooth time-scrub to birth JD. Stars/planets shift to birth moment. Press again to return. |
| E | Earth View | Toggle Earth globe view. JS↔WASM bridge. |
| L | Locale toggle | Cycle through available languages (EN→ES→PT→AR→HE→ZH→HI→FR→JA→DE→RU→KO). Toast feedback. |
| M | Audio toggle | Unmute/mute audio. Planetary harmonics + focus timbres + convergence chords. |
| R | Personal Room | Diamond Room (when implemented). |
| Escape | Close panel | Closes any open panel (help, settings, birth form). |

---

## Depth Navigation

| Key | Effect |
|-----|--------|
| PageDown | Descend one depth layer (surface → context → cycle → cosmic) |
| PageUp | Ascend one depth layer |
| Card crossfade | Layers blend smoothly during transition |

---

## Scale Navigation

| Key | Scale Level |
|-----|-------------|
| Shift+0 | Personal Chart |
| Shift+1 | Earth-Moon |
| Shift+2 | Inner Solar System |
| Shift+3 | Full Solar System |
| Shift+4 | Solar Neighborhood |
| Shift+5 | Milky Way |
| Shift+6 | Observable Universe |

---

## Visual Toggles

| Key | Effect |
|-----|--------|
| T | Kabbalah focus (Tree of Life + Gates) |
| H | Toggle HUD overlay |

---

## Timeline Scrubber

| Action | Effect |
|--------|--------|
| Drag time bar | Scrub through time (spring settle on release) |
| Click "Now" button | Smooth return to current real time |
| Click "Pause" button | Toggle pause |

---

## Design Principles

- Single key per action (no modifiers except Shift for scale/time jumps)
- Focus keys (K/I/A/D/C/T) are mutually exclusive — pressing one exits the previous
- All time navigation uses smooth exponential approach (no instant jumps)
- Panels are mutually exclusive — opening one closes others
- All user preferences persist to localStorage
- Audio starts muted — user enables with M
- Birth data persists across sessions

---

## For Agents Building New Interactions

When adding a new keyboard shortcut:
1. Check this file for conflicts
2. Update this file with the new key
3. Update `web/index.html` help panel content
4. Update `src/core/input.c` key handler
5. Add to `help_overlay.c` shortcut list
6. Commit all 4 files together
