# Prompts Flow — Research Extraction Pipeline

**Location**: `.context/research/prompts/NNN-topic.md`

---

## Extraction Methods

### Method 1: Agent Direct (preferred for PDFs we have)
Agent reads PDF via Read tool, extracts answers, writes to prompt file and/or digest.
```
Agent claims research task -> reads manifest.json -> reads PDF pages
-> writes answers in prompt file -> writes digest to .context/research/digested/
-> updates manifest.json extraction status -> updates contributors.json
```

### Method 2: NotebookLM (for multi-source synthesis, non-PDF formats)
Gabriel uses NotebookLM chat mode. One question at a time, paste answer back.
```
Open prompt file -> copy Q into NotebookLM -> paste answer under A
-> repeat -> mark status done
```

### Method 3: WebFetch (for open data)
Agent fetches data directly from JPL, IAU, arXiv, GitHub catalogs.
```
Agent identifies data need -> WebSearch/WebFetch -> writes digest
-> uses data in module implementation
```

Any method can answer any card. Agent Direct is fastest when PDFs are available.

---

## Phase 1: Hello WebGL Triangle

| # | Topic | Source Book | Questions | Status |
|---|-------|------------|-----------|--------|
| 001 | WebGL context & canvas setup | WebGL Programming Guide | 4 | done |
| 002 | Shader compilation & linking | WebGL Programming Guide | 4 | done |
| 003 | Vertex buffers & drawing | WebGL Programming Guide | 4 | done |
| 004 | GLSL ES fundamentals | WebGL Guide + Book of Shaders | 3 | done |
| 005 | Render loop & performance | WebGL Programming Guide | 3 | done |

**Total**: 18 questions across 5 cards — ALL DONE

---

## Phase 2: Math Foundations

| # | Topic | Source Book | Questions | Status |
|---|-------|------------|-----------|--------|
| 006 | Julian Day Number | Astronomical Algorithms (Meeus) | 4 | done |
| 007 | Kepler's equation | Astronomical Algorithms (Meeus) | 4 | done |
| 008 | Sidereal time (GST/LST) | Astronomical Algorithms (Meeus) | 4 | done |
| 009 | vec3/mat4 operations | Math for 3D (Lengyel) | 4 | done |
| 010 | Coordinate transforms | Astronomical Algorithms (Meeus) | 3 | done |

**Total**: 19 questions across 5 cards — ALL DONE

---

## Phase 3: First Visual Layer

| # | Topic | Source Book | Questions | Status |
|---|-------|------------|-----------|--------|
| 014 | Orbital elements & planetary positions | Meeus + Practical Astronomy | 4 | done |
| 015 | 3D camera & projection | Lengyel + Computer Graphics | 3 | done |
| 016 | Sphere rendering & basic lighting | Computer Graphics + WebGL Guide | 3 | done |
| 017 | Ecliptic-equatorial transforms | Meeus + Practical Astronomy | 3 | done |
| 023 | Practical planetary position calculation | Practical Astronomy (Duffett-Smith) | 4 | in progress |

**Total**: 17 questions across 5 cards

---

## Phase 4-5: Time Display + Astrology

| # | Topic | Source Book | Questions | Status |
|---|-------|------------|-----------|--------|
| 013 | Jung's synchronicity & astrology | Jung on Astrology + Synchronicity | 4 | pending |
| 024 | Multi-calendar conversions | Calendrical Calculations (Reingold) | 4 | pending |

**Total**: 8 questions across 2 cards

---

## Phase 4-7: UI/UX & Visual Design

| # | Topic | Source Book | Questions | Status |
|---|-------|------------|-----------|--------|
| 025 | Camera & navigation patterns | Computer Graphics + WebGL Guide | 3 | pending |
| 026 | Toggle & layer management | Book of Shaders + Computer Graphics | 3 | pending |
| 027 | Whitespace & visual composition | Book of Shaders + web research | 3 | pending |

**Total**: 9 questions across 3 cards

---

## Phase 6: Knowledge Systems (Arguelles / Tzolkin)

| # | Topic | Source Book | Questions | Status |
|---|-------|------------|-----------|--------|
| 011 | Dreamspell core mechanics | Mayan Factor + 13 Moons + Encantamiento | 5 | pending |
| 012 | Law of Time (12:60 vs 13:20) | Time and the Technosphere | 3 | pending |
| 018 | 260 Postulates of Dynamics of Time | 260 Postulates (Arguelles) | 3 | pending |
| 019 | Earth Ascending — holonomic recollection | Earth Ascending (Arguelles) | 3 | pending |
| 020 | Telektonon — game of prophecy | Telektonon (Arguelles) | 3 | pending |
| 021 | Manifesto for the Noosphere | Manifesto for the Noosphere (Arguelles) | 3 | pending |
| 022 | La Sonda de Arcturo (Arcturus Probe) | La Sonda De Arcturo (Arguelles) | 3 | pending |

**Total**: 23 questions across 7 cards

---

## Extraction Priority

**Now** (Phase 3 complete, Phase 4 starting):
- 023 (finish — Practical Astronomy planetary positions)
- 024 (Calendrical Calculations — for Phase 4 time display)
- 013 (Jung — for Phase 5 astrology layer)

**UI/UX research** (feed from Computer Graphics, Book of Shaders, web):
- 025 (Camera & navigation patterns)
- 026 (Toggle & layer management)
- 027 (Whitespace & visual composition)

**Background / anytime**:
- 011 → 012 → 018 → 019 → 020 → 021 → 022 (Arguelles collection)

---

## Summary

| Status | Cards | Questions |
|--------|-------|-----------|
| Done | 14 | 51 |
| In progress | 1 | 4 |
| Pending | 12 | 39 |
| **Total** | **27** | **94** |

---

**Last Updated**: 2026-03-06
