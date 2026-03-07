# Knowledge Mining Queue

**Owner**: MEGA (prioritizes), DELTA (executes)
**Updated**: 2026-03-06

---

## How This Works

1. MEGA adds books to this queue with extraction targets
2. **Any orchestrator** (GAMMA preferred, DELTA, ALPHA, BETA) picks the top unextracted entry
3. Agent reads `.context/research/EXTRACTION_QUESTIONS.md` for specific questions per book
4. Agent reads the PDF (survey first, then targeted chapters)
5. Agent writes digest to `.context/research/digested/NNN-topic.md`
6. Agent updates this file with extraction status
7. Agent updates `data/contributors.json` with author attribution
8. Agent reports: what was extracted, which roadmap tracks it feeds
9. MEGA reviews digests and routes knowledge to roadmap items

**Extraction Questions**: `.context/research/EXTRACTION_QUESTIONS.md` contains 3-8 specific questions per book, tied to roadmap tracks. The stronger the questions, the stronger the extracted data. Agents MUST read this file before extracting.

**GAMMA is primary extractor** after completing current code task. DELTA handles overflow.

---

## Priority Levels

- **P0**: Blocks current agent work (task cannot proceed without this data)
- **P1**: Enables next round of agent tasks
- **P2**: Enrichment (improves existing modules or enables future tracks)
- **P3**: Background (nice-to-have, extract when idle)

---

## Queue

### P0 — Blocks Current Work

| Book | Location | Extract What | Feeds Tracks | Status |
|------|----------|-------------|-------------|--------|
| Aslaksen — The Mathematics of the Chinese Calendar | `new/` | Precise lunisolar algorithms, intercalation rules, solar terms | 11, 14-16 | **EXTRACTED** — digest 024 (pp. 1-28: 5 rules, leap month algorithm, Y2033 problem, zhong qi distances, CNY date rules) |
| Meeus — Astronomical Algorithms | `new/` | Computus (Easter), obliquity polynomial, moon phases | 29.6, 14, 15 | **PARTIAL+** — digest 029 (Ch8 Easter/Computus both Julian+Gregorian, Ch22 nutation+Laskar obliquity polynomial+63-term table, Ch21 precession rigorous method, Ch49 moon phases complete algorithm with all correction terms). Remaining: Ch9 Jewish/Moslem, Ch15 rise/set, Ch18 conjunctions, Ch54 eclipses |
| Arguelles — The Mayan Factor | `new/` | Dreamspell oracle algorithm, 13:20 matrix, wavespell | 31.1-31.3, 29.2 | **EXTRACTED** — digest 027 (Ch3-4 pp.65-108, Appendices pp.205-211: 13:20 matrix, 20 day signs, 13 tones, Loom of Maya, Long Count, Haab, calendar round, harmonic numbers, year bearers, burner cycles) |
| Arguelles — Time and the Technosphere | `new/` | Law of Time formulas, 13-Moon calendar math | 29.2, 29.5 | **EXTRACTED** — digest 028 (Ch3 pp.35-53 Law of Time formulas, Ch6 pp.99-117 13-Moon calendar + reform history, App I pp.200-206 7-point Gregorian critique, App IV pp.229-239 Nineteen Code + Quranic 19=260 proof) |

### P1 — Enables Next Round

| Book | Location | Extract What | Feeds Tracks | Status |
|------|----------|-------------|-------------|--------|
| Tufte — Envisioning Information | `new/` | Data visualization principles, small multiples, layering | 29.4, 30 | **EXTRACTED** — digest 034 (all 6 chapters: escaping flatland techniques, micro/macro readings, layering/separation with 1+1=3 effect, small multiples rules, Imhof's 4 color rules, 20-30 color limit, nature's palette, data-ink ratio, space-time grid narratives) |
| Hazen — The Story of Earth | `new/` | 4.6 Ga narrative, mineral evolution, co-evolution of life/geology | 20-25, 29.3 | **EXTRACTED** — digest 025 (Ch7 GOE), digest 026 (Ch9+Ch10 Snowball/Green Earth), digest 030 (Ch8 Boring Billion + Ch11 Future Earth: solar timeline, ocean evaporation, Milankovitch, asteroid/volcano frequencies, mineral evolution plateau) |
| Brannen — The Ends of the World | `new/` | Big Five extinction narratives, climate data | 29.3, 20.4 | NOT STARTED |
| Bryson — A Short History of Nearly Everything | `new/` | Deep time storytelling, Earth history data | 29.3, 20 | NOT STARTED |
| Albers — Interaction of Color | `new/` | Color interaction theory, simultaneous contrast | 37.4, all renderers | NOT STARTED |
| Itten — The Art of Color (if acquired) | — | Color wheel theory, 7 contrast types | 37, color_theory | NOT ACQUIRED |
| Calendrical Calculations | `new/` | All calendar algorithms (already used for Hebrew) | 14-16, 17, 29 | **EXTRACTED** — digest 031 (Ch7 Islamic: 30-year leap cycle, epoch, formulas; Ch15 Persian: astronomical + arithmetic 2820-year cycle; Ch19 Chinese: 24 solar terms, leap month, sexagesimal, 2033 problem; Ch16 Bahai: 19×19 structure, arithmetic + astronomical; Ch9 Easter: Orthodox + Gregorian Computus; Ch4 Coptic/Ethiopic: epoch, months, formulas). Remaining: Ch10/Ch20 Hindu (large, needs dedicated session) |

### P2 — Enrichment

| Book | Location | Extract What | Feeds Tracks | Status |
|------|----------|-------------|-------------|--------|
| Crowley — 777 | `new/` | Correspondence tables: numbers → colors → planets → paths | 18, 19, 44.2 | **EXTRACTED** — digest 032 (Tables I-VI: complete 4 color scales King/Queen/Emperor/Empress for 34 rows, 22 Major Arcana attributions with Golden Dawn titles, Lightning Flash sequence, path-Sephirah connections, cross-ref with tree_geometry.h = FULL MATCH, Tarot naming discrepancies noted, Tsaddi/He swap documented, Sephirotic colors, precious stones, astronomical spheres, chakras, lunar mansions, mystic numbers) |
| Crowley — The Book of Thoth | `new/` | Tarot ↔ Tree of Life ↔ Hebrew letters ↔ time cycles | 18, 19, 44.2 | **EXTRACTED** — digest 033 (22 Major Arcana complete with Thoth names, Hebrew letters, paths, attributions; Tsaddi/He swap documented; 16 court cards with decan mappings + I Ching hexagram cross-refs; 40 minor arcana Thoth names + Sephirotic assignments; YHVH/Four Worlds formula; Three Aeons precessional time-cycles; card design descriptions for Track 44.2) |
| Outer Wheel of Time (Kalachakra) | `new/` | Buddhist cosmological time cycles, Vajrayana cosmology | 16 | NOT STARTED — **JUST ACQUIRED!** |
| Bhaskara I — Commentary on Aryabhatiya | `new/` | Ancient Indian astronomical algorithms, equation of center | 29.1 | NOT STARTED |
| Surya Siddhanta | `new/` | Indian planetary model, epicycles, constants | 29.1, astronomy | NOT STARTED |
| Schönberger — I Ching & Genetic Code | `new/` | 64-codon ↔ 64-hexagram mapping | 27.4 | NOT STARTED |
| Sacred Geometry (Lundy) | `new/` | Geometric constructions, number patterns | 26, 19 | NOT STARTED |
| Color science books (9 total) | `new/` | Gamut mapping, perceptual uniformity, CIE standards | 37, color_theory | NOT STARTED |
| Designing the User Interface | `new/` | UI/UX principles, interaction patterns | 30.4, UI | NOT STARTED |
| Chakras reference | `new/` | 7 chakras: colors, frequencies, Sanskrit, body mapping | 37.8 | NOT STARTED |
| Aveni — Skywatchers of Ancient Mexico | `new/` | Mayan astronomical observations, Venus tables | 29.1, 11 | NOT STARTED |

### P3 — Background

| Book | Location | Extract What | Feeds Tracks | Status |
|------|----------|-------------|-------------|--------|
| Imbrie — Ice Ages | (if acquired) | Milankovitch cycles, orbital parameter formulas | 22, 20.4 | NOT ACQUIRED |
| Steel — Marking Time | (if acquired) | Calendar reform politics, who controls time | 29.2, 29.5 | NOT ACQUIRED |
| Aveni — Empire of Time | (if acquired) | Cross-cultural timekeeping, power structures | 29.2 | NOT ACQUIRED |

---

## Extraction Output Format

Each extraction produces a digest file in `.context/research/digested/`:

```
Filename: NNN-topic.md  (next sequential number)
```

Content:
```markdown
# Digest: [Topic]

**Source**: [Book Title] by [Author], pp. [pages]
**Extracted by**: DELTA / ALPHA / BETA / MEGA
**Date**: YYYY-MM-DD
**Feeds**: Track X.Y, Track Z.W

## Key Data
[Formulas, constants, algorithms, data tables]

## Algorithms (pseudocode → C candidates)
[Step-by-step algorithms ready for C implementation]

## Data Tables (→ static const arrays)
[Tables of constants, dates, names, values]

## Cultural Context
[Why this matters, how it connects to other systems]

## Implementation Notes
[Struct designs, function signatures, test scenarios]
```

---

## Completed Extractions (26 digests)

001-005: WebGL pipeline (context, shaders, buffers, GLSL, render loop)
006-008: Astronomy (Julian Day, Kepler, sidereal time)
009-010: Math (vec3/mat4, coordinate transforms)
014-016: Astronomy (orbital elements, 3D camera, sphere rendering)
023: Practical astronomy planets
024: Chinese Calendar algorithms (Aslaksen — 5 rules, leap month, Y2033, CNY dates)
025: Great Oxidation Event (Hazen Ch7 — GOE timeline, O2 curve, mineral evolution, surface color)
026: Snowball Earth + Green Earth (Hazen Ch9+10 — 3 snowball episodes, O2 to 15%, Cambrian explosion, supercontinents)
027: Mayan Factor Harmonic Module (Arguelles Ch3-4 + Appendices — 13:20 matrix, 20 signs, 13 tones, Loom of Maya, Long Count, Haab, calendar round, harmonic numbers, burner cycles)
028: Law of Time + Calendar Reform + Nineteen Code (Arguelles Technosphere Ch3+Ch6+AppI+AppIV — T(E)=Art, 12:60 vs 13:20, 13-Moon math, 7-point Gregorian critique, reform history, Quranic 19=260 proof, Nineteen Strand)
029: Meeus Computus + Obliquity + Precession + Moon Phases (Ch8 Easter Julian+Gregorian, Ch22 nutation+Laskar 10th-degree obliquity, Ch21 rigorous precession, Ch49 complete moon phase algorithm)
030: Boring Billion + Future Earth (Hazen Ch8+Ch11 — 1.85-0.85 Ga stasis, Canfield Ocean, mineral evolution plateau, solar timeline to red giant, ocean evaporation +1-2 Gy, Milankovitch cycles, asteroid/volcano frequencies, supercontinent projections)
031: Calendrical Calculations (Ch7 Islamic 30-year leap cycle + epoch + formulas, Ch15 Persian astronomical + arithmetic 2820-year cycle, Ch19 Chinese 24 solar terms + leap month + sexagesimal + 2033 problem, Ch16 Bahai 19×19 structure, Ch9 Easter Orthodox + Gregorian Computus, Ch4 Coptic/Ethiopic epochs + formulas)
032: Crowley 777 Correspondences (Tables I-VI: 4 color scales, 22 Tarot attributions, Lightning Flash, tree cross-reference, astronomical spheres, chakras, mystic numbers)
033: Crowley Book of Thoth (22 Major Arcana Thoth names, court card decan mappings + I Ching cross-refs, minor arcana Sephirotic table with Thoth names, YHVH/Four Worlds, Three Aeons time-cycles, card design descriptions)
034: Tufte Envisioning Information (6 chapters: escaping flatland, micro/macro, layering/separation, small multiples, Imhof 4 color rules, 20-30 color limit, 1+1=3 effect, data-ink ratio, space-time narratives)
aveni-harmony: Commensuration, Venus 8:5, sacred numbers

---

## Notes

- Context window is the bottleneck: a 400-page book needs 20+ Read calls
- DELTA sessions should focus on ONE BOOK per session, extract thoroughly
- Better to deeply extract 3 books than skim 20
- Always update contributors.json after extraction
- Fiction books (Cowl, Emerald Magic, Interdimensional Travel) are NOT in this queue
