# Time — User Controls

**Last Updated**: 2026-03-06

## Active Controls

### Mouse

| Action | Effect |
|--------|--------|
| Click + drag | Rotate camera around scene |
| Scroll wheel | Zoom in / out |

### Keyboard — Time

| Key | Effect | Speed |
|-----|--------|-------|
| Space | Toggle pause / resume | — |
| 1 | Real-time | 1 day/day |
| 2 | Fast | 60x (1 min/sec) |
| 3 | Faster | 3600x (1 hr/sec) |
| 4 | Day per second | 86400x |
| 5 | 10 days per second | 864000x |
| 0 | Pause | 0x |
| - | Reverse time direction | ×(-1) |

### Keyboard — Scale Navigation

| Key | Effect | Scale |
|-----|--------|-------|
| Shift+0 | Jump to Personal Chart | 0.01 - 0.5 |
| Shift+1 | Jump to Earth-Moon | 0.5 - 2.0 |
| Shift+2 | Jump to Inner Solar System | 2.0 - 8.0 |
| Shift+3 | Jump to Solar System | 8.0 - 80.0 |
| Shift+4 | Jump to Solar Neighborhood | 80 - 500 |
| Shift+5 | Jump to Milky Way | 500 - 5000 |
| Shift+6 | Jump to Observable Universe | 5000 - 50000 |

### Keyboard — Toggles

| Key | Effect | Status |
|-----|--------|--------|
| T | Toggle orbit trails | **implemented** |
| H | Toggle HUD overlay | **implemented** |
| S | Toggle star field | not implemented |
| G | Toggle grid / ecliptic plane | not implemented |
| L | Toggle labels | not implemented |

## Design Principles

- Single key per toggle (no modifiers needed)
- Number keys control time speed
- Letter keys toggle visual layers
- No menu system yet — keyboard-only for now
- All toggles are boolean fields in `app_state_t`
