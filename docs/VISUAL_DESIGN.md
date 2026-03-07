# Time — Visual Design & UX Architecture

**Last Updated**: 2026-03-06
**Status**: Draft — evolving as we build

---

## Core Principle

Time is one thing viewed through seven lenses. The artwork is not seven separate screens — it is one continuous experience where layers reveal themselves naturally, like peeling an onion or tuning a radio between stations.

**The metaphor**: A mandala. The center is the Sun. Rings radiate outward. Each ring is a knowledge system. All rings are always present; focus shifts between them.

---

## The Golden Number (phi = 1.618...)

**Every visual proportion in Time derives from phi.** No magic numbers. No arbitrary choices. The golden ratio governs:

- **Typography**: Font size scale where each level = previous * phi^-1
- **Spacing**: Padding/margin scale where each step = previous * phi
- **Layout**: Screen divided at golden section points. Cards are golden rectangles.
- **Animation**: Timing scale from micro (90ms) to epic (2618ms) via phi powers
- **Opacity**: Layer cascade: 1.0, 0.618, 0.382, 0.236, 0.146...
- **Rings**: Inner/outer radius ratio = phi. Concentric rings grow by phi.
- **Camera**: Zoom levels spaced by phi. Crossfade widths = phi^-1.
- **Colors**: Hues spaced by the golden angle (137.508 degrees)

**Why**: phi appears in sunflower spirals, galaxy arms, DNA helix proportions, the Parthenon, the Great Pyramid. Time proves that one number governs nature, art, astronomy, and sacred geometry. The UI IS the proof.

**Implementation**: `src/ui/golden_layout.h` — single source of truth for all visual proportions.

---

## Color System — Psychologically Meaningful, Mathematically Derived

**No random color choices.** Every color has psychological intent and mathematical derivation.

### Format Standard
All colors: **normalized float [0.0-1.0] RGBA**. This is WebGL's native format.
- sRGB for human-authored values (stored in palettes)
- Linear space for all math in shaders
- `ct_to_linear()` / `ct_to_srgb()` for conversion

### Color Psychology
Each hue range communicates neurologically:
- **Red (0-15)**: Energy, urgency, activation (Mars, Fire signs, Tzolkin Red family)
- **Orange (30)**: Warmth, optimism (Sun, Leo, Jupiter)
- **Yellow-Green (75)**: Clarity, intellect, growth (Mercury, Air signs)
- **Green (120)**: Harmony, balance, nature (Venus, Earth signs, Wood element)
- **Blue-Green (170)**: Tranquility, depth, water (Water signs, Buddhist calm)
- **Blue (210)**: Trust, reliability, infinity (Sky, Hebrew tekhelet, Islamic lapis)
- **Indigo (250)**: Wisdom, intuition, cosmos (I Ching depth, Kabbalah)
- **Violet (280)**: Spirituality, transformation (Human Design intuition)
- **Neutral grays**: Objectivity, time itself (Gregorian system)

### Golden Angle Harmony
Instead of arbitrary hue steps, color palettes use the golden angle (137.508 deg) for spacing. Nature uses this for optimal packing (sunflower seeds). We use it for maximally distinct yet harmonious color sets.

### System-Specific Palettes
Each knowledge system gets a psychologically appropriate palette:
- **Astronomy**: cool blue + warm gold (sky + sun)
- **Astrology**: element-coded (fire=red, earth=brown, air=yellow, water=blue)
- **Tzolkin**: 4 directional colors per Arguelles
- **I Ching**: yin/yang duality (deep indigo + ivory)
- **Chinese**: Wu Xing elements (wood=green, fire=red, earth=gold, metal=silver, water=black)
- **Human Design**: body-energy (amber, crimson, teal, violet)
- **Kabbalah**: 4 Worlds (Atziluth=gold, Briah=silver, Yetzirah=indigo, Assiah=olive)
- **Hindu**: sacred (saffron, vermillion, turmeric)
- **Buddhist**: dharma (ochre, gold, deep blue)
- **Hebrew**: Torah (royal blue, gold, parchment)
- **Islamic**: geometric (emerald, gold, lapis lazuli)
- **Geology**: Earth (sienna, amber, slate, jade)

### Depth & Atmosphere
- Space is NOT pure #000000 — it's deep blue-black (HSL 230, 0.20, 0.03) for richness
- Depth fog blends from warm foreground to cold space-black
- Star glow is warm white (~5500K blackbody)

### Accessibility
- WCAG AA contrast ratio (>= 4.5:1) enforced for all text
- `ct_ensure_contrast()` auto-adjusts text colors against backgrounds

**Implementation**: `src/render/color_theory.h` — psychologically meaningful color system.

---

## View Architecture

### The Three Depths

```
DEPTH 1: COSMOS (far)          Stars, constellations, deep sky
                                The container — always present as background
                                Toggle: S key

DEPTH 2: SOLAR SYSTEM (mid)    Sun, planets, orbits, ecliptic plane
                                The core visual — the astronomical truth
                                Always visible, scales with zoom

DEPTH 3: OVERLAYS (near)       Text, symbols, cards, wheels
                                Knowledge system interpretations
                                Screen-space, camera-independent
```

### The Ecliptic Band

The zodiac lives ON the ecliptic plane as a translucent ring around the Sun. 12 segments, colored subtly. Planet glyphs sit on this ring at their ecliptic longitude. This connects astronomy (planet positions) to astrology (zodiac signs) visually — they are the SAME data, viewed differently.

```
                    ♋ Cancer
              ♊           ♌
         ♉                     ♍
     ♈ -------- ☉ Sun -------- ♎
         ♓                     ♏
              ♒           ♐
                    ♑ Capricorn
```

---

## Screen Layout

```
┌─────────────────────────────────────────────┐
│ [Gregorian Date] [JD] [GST]    [Speed: 1x] │  <- HUD bar (top)
│                                              │
│                                              │
│            3D Solar System View              │  <- Main canvas
│           (with ecliptic/zodiac              │
│            ring when zoomed)                 │
│                                              │
│                                              │
│ ┌──────┐                        ┌──────────┐│
│ │ KIN  │                        │ ASTRO    ││  <- Side cards
│ │ CARD │                        │ WHEEL    ││     (toggle)
│ │      │                        │          ││
│ └──────┘                        └──────────┘│
│                                              │
│ [Tone: 9 Solar] [Seal: Blue Hand] [Hex: 42] │  <- Info bar (bottom)
└─────────────────────────────────────────────┘
```

### HUD (always visible, minimal)
- Top-left: Gregorian date + time
- Top-center: Julian Day number
- Top-right: Time speed indicator
- All text in WebGL (bitmap font atlas)
- Toggle: H key

### Side Cards (togglable panels)
- **Left**: Tzolkin Kin Card — current day's seal + tone + color
- **Right**: Astrology snapshot — zodiac wheel or aspect table
- **Bottom**: I Ching hexagram, Chinese calendar info
- Each card is a screen-space quad with its own mini-renderer
- Toggle individually or show all
- Future: cards slide in/out with smooth animation

### Info Bar (bottom)
- One-line summary of all active systems
- "Solar 9 · Blue Hand · Kin 207 · Hex 42 · Year of the Snake"
- Always visible when HUD is on

---

## Navigation Model

### Focus Modes

| Key | Mode | Camera Behavior | Visible Layers |
|-----|------|----------------|----------------|
| Esc | Overview | Wide zoom, all visible | Everything at default opacity |
| A | Astrology Focus | Zoom to ecliptic ring | Zodiac wheel prominent, planets as glyphs |
| K | Kin/Tzolkin Focus | Zoom out, Kin card large | Kin card fills left half, 3D dimmed |
| I | I Ching Focus | Zoom out, hexagram large | Hexagram display, trigrams explained |
| C | Chinese Cal Focus | Zoom out, Chinese card | Stem+Branch+Animal display |
| D | Human Design Focus | Zoom out, bodygraph | Gates and channels from planet positions |

### Transitions (phi-timed)

- Mode switch: camera interpolates over 0.618s (`gl_timing.slow`) — smoothstep
- Overlay panels fade in over 0.236s (`gl_timing.normal`) — snappy but smooth
- 3D scene dims to phi^-2 opacity (0.382) when a focus panel is active
- Card stagger: each successive card delays by fibonacci-diminishing intervals
- Press same key again or Esc to return to overview

### Scale Levels & Knowledge System Homes

7 logarithmic zoom levels. Each knowledge system has a "home" scale where it is most prominent. Use Shift+0-6 to jump between levels.

| Scale | Name | Distance | Home Systems |
|-------|------|----------|-------------|
| 0 | Personal Chart | 0.01 - 0.5 | Human Design, Numerology |
| 1 | Earth-Moon | 0.5 - 2.0 | Astrology (houses, ascendant), Planetary Hours |
| 2 | Inner Solar System | 2.0 - 8.0 | Zodiac wheel, Aspects, Dignity |
| 3 | Solar System (default) | 8.0 - 80.0 | Astronomy (planets, orbits), Chinese Calendar |
| 4 | Solar Neighborhood | 80 - 500 | Precession, Sidereal time |
| 5 | Milky Way | 500 - 5000 | I Ching, Tzolkin (cosmic cycles) |
| 6 | Observable Universe | 5000 - 50000 | Cosmic time, Age of universe |

### Render Layers & Scale Visibility

8 render layers, each visible within a specific scale range. Opacity fades at boundaries (20% ramp zone).

| Layer | Scale Range | Notes |
|-------|-------------|-------|
| Stars | 3-6 | Background star field, appears at Solar scale |
| Galaxy | 4-6 | Milky Way spiral, deep zoom only |
| Orbits | 1-4 | Orbital paths, fades at galaxy scale |
| Planets | 0-4 | Sun + 8 planets, fades at neighborhood scale |
| Zodiac Ring | 1-3 | Ecliptic wheel, inner to solar scale |
| Labels | 0-3 | Text labels, near scales only |
| Cards | 0-2 | Knowledge system info cards, personal/earth/inner |
| HUD | 0-6 | Always visible (time data overlay) |

---

## Visual Language

### Color Palette

Governed by `color_theory.h`. All values in normalized float [0.0-1.0].

| Element | Source | Psychological Intent |
|---------|--------|---------------------|
| Background | `ct_space_black()` | Deep blue-black — alive, not dead |
| Sun | `ct_star_glow()` | Warm gold — 5500K blackbody warmth |
| Stars | `star_colors.h` (spectral) | True stellar colors from B-V index |
| Orbit trails | `color_palette.h` at phi^-1 opacity | Subtle presence, not visual noise |
| Zodiac ring | Element-coded via `ct_system_primary(ASTROLOGY)` | Fire/Earth/Air/Water meaning |
| Kin card | `ct_system_primary(TZOLKIN)` | 4 directional families |
| HUD text | `ct_role_color(TEXT_SECONDARY)` | Readable but not dominant |
| Danger/error | `ct_role_color(DANGER)` | Red — neurological urgency |
| Cards surface | `ct_role_color(SURFACE)` | Subtle elevation from space |

### Typography (planned)

- HUD: Monospace bitmap font (clean, technical feel)
- Cards: Proportional SDF font (readable, warm)
- Symbols: Custom MSDF atlas (astrology glyphs, Mayan seals)

### The Five Seal Colors (Tzolkin)

| Color | Seals | Meaning |
|-------|-------|---------|
| Red | Dragon, Serpent, Moon, Skywalker, Earth | Initiate |
| White | Wind, Worldbridger, Dog, Wizard, Mirror | Refine |
| Blue | Night, Hand, Monkey, Eagle, Storm | Transform |
| Yellow | Seed, Star, Human, Warrior, Sun | Ripen |

---

## Implementation Phases

### Phase 4 (Time Display) — Current
- [ ] Bitmap font atlas (stb_truetype.h)
- [ ] HUD text rendering (Gregorian, JD, GST, speed)
- [ ] H key toggles HUD visibility

### Phase 5 (Astrology Layer)
- [ ] Zodiac ring on ecliptic plane (12 colored segments)
- [ ] Planet glyphs on zodiac ring at ecliptic longitude
- [ ] A key focuses astrology view
- [ ] Aspect lines between planets (conjunction, opposition, trine, square)

### Phase 6 (Knowledge Systems)
- [ ] Kin card renderer (seal icon + tone number + color)
- [ ] K key focuses Tzolkin view
- [ ] I Ching hexagram renderer (6 lines, broken/unbroken)
- [ ] Chinese calendar display (animal + element + stem + branch)
- [ ] Human Design bodygraph (simplified)

### Phase 7 (Visual Integration)
- [ ] Smooth camera transitions between focus modes
- [ ] Layer dimming when focused
- [ ] Card slide-in/out animations
- [ ] Star field (BSC5 point sprites)
- [ ] Consistent color palette enforcement
- [ ] 60fps with all layers active

---

## Open Design Questions

1. **Zodiac ring**: Flat on ecliptic plane or always screen-facing?
2. **Kin card art**: Procedural GLSL or pre-baked texture atlas of 20 seals?
3. **I Ching mapping**: Which method maps JD to hexagram? (needs card 018+)
4. **Sound**: Will we add audio? (not in current roadmap)
5. **Mobile**: Touch controls? Pinch zoom? (Phase 8)
6. **Info density**: How much text is too much for an artwork?

---

## Inspiration

- **dark.netflix.io**: Layers of time, dark aesthetic, interconnected views
- **Stellarium**: Astronomical accuracy, overlays toggle on/off
- **100,000 Stars (Chrome)**: Point sprite stars, zoom transitions
- **Traditional Tzolkin wheel art**: Circular, mandala-like, color-coded by seal family
