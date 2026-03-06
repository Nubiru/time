# Time — Visual Design & UX Architecture

**Last Updated**: 2026-03-06
**Status**: Draft — evolving as we build

---

## Core Principle

Time is one thing viewed through seven lenses. The artwork is not seven separate screens — it is one continuous experience where layers reveal themselves naturally, like peeling an onion or tuning a radio between stations.

**The metaphor**: A mandala. The center is the Sun. Rings radiate outward. Each ring is a knowledge system. All rings are always present; focus shifts between them.

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

### Transitions

- Mode switch: camera interpolates position over 0.8 seconds (smoothstep)
- Overlay panels fade in over 0.3 seconds
- 3D scene dims (multiplied by 0.3) when a focus panel is active
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

### Color Palette (preliminary)

| Element | Color | Hex |
|---------|-------|-----|
| Background | Near-black | #080808 |
| Sun | Warm yellow | #FFD933 |
| Stars | White/blue-white | #E8E8FF |
| Orbit trails | Planet color at 40% | — |
| Zodiac ring | Translucent white | #FFFFFF20 |
| Fire signs (Ari,Leo,Sag) | Red | #CC3333 |
| Earth signs (Tau,Vir,Cap) | Green | #33AA55 |
| Air signs (Gem,Lib,Aqu) | Yellow | #CCAA33 |
| Water signs (Can,Sco,Pis) | Blue | #3355CC |
| Kin card background | Matches seal color | — |
| HUD text | Dim white | #AAAAAA |

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
