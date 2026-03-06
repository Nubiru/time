# Reference Library — Time Project

Master catalog of all collected reference materials.

**Locations**:
- `~/Desktop/temp/time/` — Original collection (36 files)
- `~/Desktop/temp/time/newly-converted/` — Converted from non-PDF formats (15 files)
- `~/Desktop/temp/time/new/` — Newest acquisitions (27 files)

**Extraction**: Use `/pdf <path> [mode]` to extract data from any PDF.

---

## Extraction Priority Key

- **P0**: Extract NOW — blocks current rendering work (star data, constellation data)
- **P1**: Extract SOON — needed for Phase 5-6 knowledge systems
- **P2**: Extract LATER — enrichment, design polish, creative coding

---

## New Acquisitions (~/Desktop/temp/time/new/)

| # | File | Category | Priority | Extracted |
|---|------|----------|----------|-----------|
| 1 | 21st century C.pdf | Programming | P2 | [ ] |
| 2 | Ascendentes En Astrologia 1 Y 2.pdf | Astrology | P1 | [ ] |
| 3 | A Submm View of the Universe.pdf | Astronomy/Survey | P2 | [ ] |
| 4 | Calendario Astrologico con las Fases de la Luna.pdf | Astrology/Calendar | P1 | [ ] |
| 5 | Cosmic Windows Sky Surveys.pdf | Astronomy/Survey | P2 | [ ] |
| 6 | Designing Interfaces.pdf | Design/UI | P2 | [ ] |
| 7 | Galaxy Colors in Various Photometric Band Systems.pdf | Astronomy/Color | P1 | [ ] |
| 8 | Generative AI Through Python.pdf | Programming | P2 | [ ] |
| 9 | IAS volume 216 Cover and Front matter.pdf | Astronomy/Reference | P2 | [ ] |
| 10 | Imaging the Cosmic Microwave Background.pdf | Cosmology | P2 | [ ] |
| 11 | Inteligencia Planetaria.pdf | Philosophy | P2 | [ ] |
| 12 | LAS LUNAS - El refugio de la memoria.pdf | Calendar/Lunar | P1 | [ ] |
| 13 | Maps of the Cosmos - CMB.pdf | Cosmology | P2 | [ ] |
| 14 | Maps of the Millimetre Sky from BOOMERanG.pdf | Cosmology | P2 | [ ] |
| 15 | On Establishing an International Service for Cometary Observations.pdf | Astronomy/Ephemerides | P2 | [ ] |
| 16 | Simulations of the Intergalactic Medium.pdf | Cosmology | P2 | [ ] |
| 17 | SPECTRAL CLASSIFICATION OF 40 RED VARIABLES.pdf | Astronomy/Stellar | P2 | [ ] |
| 18 | The Calendar - 5000-year Struggle.mobi | Calendar/History | P1 | [ ] |
| 19 | The Complete I Ching - Alfred Huang.epub | I Ching | P1 | [ ] |
| 20 | The Definitive Book of Human Design.pdf | Human Design | P0 | [ ] |
| 21 | The Design of Everyday Things.pdf | Design/UX | P2 | [ ] |
| 22 | The Digitized First Byurakan Survey - DFBS.pdf | Astronomy/Survey | P1 | [ ] |
| 23 | The Elements of Typographic Style.pdf | Design/Typography | P2 | [ ] |
| 24 | The I Ching or Book of Changes.pdf | I Ching | P1 | [ ] |
| 25 | THE INTERNATIONAL INFORMATION BUREAU ON ASTRONOMICAL EPHEMERIDES.pdf | Astronomy/Ephemerides | P2 | [ ] |
| 26 | The Nature of Code.pdf | Creative Coding | P1 | [ ] |
| 27 | The Numerology of the I Ching.djvu | I Ching | P1 | [ ] |

---

## Original Collection (~/Desktop/temp/time/)

Already cataloged in `docs/checklists/books.md`. Key items:

| File | Category | Priority | Extracted |
|------|----------|----------|-----------|
| Astronomical Algorithms.pdf | Astronomy/Math | P0 | [x] partial (via NotebookLM) |
| Calendrical Calculations.pdf | Calendar/Math | P1 | [x] partial |
| Computer graphics Principles and practice.pdf | Graphics | P1 | [ ] |
| Mathematics for 3D Game Programming.pdf | Math/Graphics | P1 | [ ] |
| Practical Astronomy with your Calculator.pdf | Astronomy | P1 | [x] partial |
| The C programming language.pdf | Programming | P2 | [ ] |
| WebGL Programming Guide.pdf | Graphics/WebGL | P1 | [x] partial |
| Jung on Astrology.pdf | Astrology/Jung | P1 | [ ] |
| Synchronicity.pdf | Psychology/Jung | P2 | [ ] |
| The Mayan Factor.pdf | Tzolkin | P1 | [x] partial |
| The Book of Shaders.pdf | Shaders | P1 | [x] partial |

Arguelles collection (16 files): All P1 for Tzolkin system. Partially extracted via NotebookLM.

---

## Extraction Workflow

```
1. Check this catalog for P0 items
2. /pdf <path> survey       — Quick assessment (2 min)
3. /pdf <path> data         — Extract tables/constants/algorithms
4. /pdf <path> extract "Q"  — Answer specific implementation questions
5. Mark [x] in Extracted column when done
6. Agent incorporates extracted data into pure C modules
```

---

## P0 Extraction Queue (blocks current work)

1. **The Definitive Book of Human Design.pdf** — Gate/channel data for Phase 6 HD system
2. **Astronomical Algorithms.pdf** — Remaining algorithms not yet implemented (precession, nutation, star positions)
3. **Star catalog data** — Need bright star RA/Dec/mag/B-V for Track 7.5 (may need external source: Yale BSC5)

---

**Total files**: 78 (36 original + 15 converted + 27 new)
**Formats**: PDF (majority), .epub (3), .mobi (1), .djvu (1), .doc (8)
**PDF-readable by /pdf**: ~60 files
**Non-PDF**: Convert first or use NotebookLM

---

**Last Updated**: 2026-03-06
