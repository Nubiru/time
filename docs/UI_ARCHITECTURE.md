# Time — UI Architecture

**Last Updated**: 2026-03-15
**Status**: Implementation

---

## Design Principles

1. **Canvas IS the interface.** No permanent sidebars. 100% artwork viewport.
2. **Reveal, don't display.** Every panel has a trigger: key, edge proximity, or search.
3. **Teach through use.** No tutorial. "Press ? for controls" fades after 5s.
4. **Animation timing is trust.** All durations from phi: 200ms, 300ms, 500ms, 800ms.
5. **Dark theme is not black theme.** Space-black (#060709), muted whites, frosted glass.

Sources: Tufte (data-ink ratio), Cooper (sovereign posture), Shneiderman (overview first).

---

## Screen Zones

```
+--------------------------------------------------+
|  [HUD: date, time, system data]        [?] help  |
|                                                    |
|              WebGL Canvas (100vw x 100vh)          |
|              pointer-events: none on overlay       |
|                                                    |
|  [Layer     |                        [Toast area]  |
|   Panel]    |                        top-right     |
|  edge-left  |                                      |
|  +-----------time bar (edge-bottom)---------------+|
+--------------------------------------------------+
```

### Zones

| Zone | Position | Trigger | Content |
|------|----------|---------|---------|
| HUD | top-left, fixed | Always visible (H toggles) | Date, JD, sidereal, astro data |
| Help button | top-right corner | Click or ? key | Opens help panel |
| Help panel | centered overlay | ? key | Categorized shortcuts from help_overlay.h |
| Command palette | centered overlay | / key | Searchable action list from command_palette.h |
| Layer panel | left edge slide-in | Mouse proximity or click | Grouped toggles from layer_panel.h |
| Time bar | bottom edge slide-up | Mouse proximity | Play/pause, speed, date from time_control.h |
| Toast area | top-right, stacked | Automatic on actions | Transient feedback from toast_message.h |
| First hint | bottom-center | First load only | "Press ? for controls" fades 5s |

---

## State Machine

Modes are MUTUALLY EXCLUSIVE (from ui_state.h). Escape always returns to DEFAULT. Same key toggles off.

Edge proximity toolbars (time bar, layer panel) are INDEPENDENT of mode.

---

## Animation Specs (golden_layout.h phi values)

| Action | Duration | Easing |
|--------|----------|--------|
| Panel open/close | 382ms (phi^-2 s) | ease-out |
| Edge toolbar reveal | 300ms | ease-out |
| Toast fade-out | 500ms | linear |
| First hint fade | 1618ms (phi s) | ease-in |

---

## Color Specs (theme.h Cosmos)

| Token | RGB | Usage |
|-------|-----|-------|
| --bg-space | 6, 7, 9 | Deep space background |
| --bg-surface | 12, 16, 28 | Panel frosted glass bg |
| --text-primary | 230, 230, 235 | Readable text |
| --text-secondary | 180, 180, 190 | Secondary text |
| --brand-primary | 255, 217, 140 | Solar gold accents |
| --brand-secondary | 51, 191, 204 | Celestial teal |

---

## Data Flow

```
Pure C Data Modules          C Bridge (EM_ASM)         DOM
-------------------          -----------------         ---
help_overlay.h      --->     ui_push_help()    --->    #help-panel innerHTML
command_palette.h   --->     ui_push_palette() --->    #cmd-palette innerHTML
layer_panel.h       --->     ui_push_layers()  --->    #layer-panel innerHTML
time_control.h      --->     ui_push_time()    --->    #time-bar innerHTML
toast_message.h     --->     ui_push_toasts()  --->    #toast-area innerHTML
ui_state.h          --->     ui_push_state()   --->    CSS class toggling
```
