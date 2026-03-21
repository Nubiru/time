# Architecture Diagrams — C4 Model (Mermaid)

**Version**: 1.0
**Generated**: 2026-03-21 (INFRA session 40)

---

## Level 1: System Context

Who uses Time, and what external systems does it touch?

```mermaid
C4Context
    title Time — System Context

    Person(user, "User", "Opens Time in a browser to experience layered time systems")
    Person(gabriel, "Gabriel", "Developer — writes C code, manages knowledge pipeline")

    System(time, "Time", "Visual, interactive 3D digital artwork displaying time through 16+ cultural/astronomical systems. C11 + WebAssembly + WebGL2.")

    System_Ext(browser, "Web Browser", "WebGL2-capable browser (Chrome, Firefox, Safari)")
    System_Ext(github, "GitHub", "Source repository, CI/CD")
    System_Ext(jpl, "JPL Ephemeris Data", "Planetary orbital elements (embedded)")

    Rel(user, browser, "Opens")
    Rel(browser, time, "Loads WASM + renders WebGL2")
    Rel(gabriel, time, "Develops via Claude Code sessions")
    Rel(gabriel, github, "Push/pull")
    Rel(time, jpl, "Embedded Keplerian elements")
```

---

## Level 2: Container Diagram

What are the major runtime containers inside Time?

```mermaid
C4Container
    title Time — Container Diagram

    Person(user, "User", "Browser")

    System_Boundary(time, "Time WASM Binary (618 KB)") {
        Container(core, "Core", "C11", "Main loop, input handling, HUD, app state. 5 modules.")
        Container(render, "Render Engine", "C11 + GLSL", "19 render passes, shader system, glyph batching, MSDF text. 94 modules.")
        Container(systems, "Knowledge Systems", "C11", "34 calendar/astronomical/cultural computation domains. Pure functions.")
        Container(ui, "UI Layer", "C11", "131 layout modules, card system, theme, focus modes. Pure functions.")
        Container(math, "Math Library", "C11", "16 modules: linear algebra, Julian Day, Kepler, sidereal, color theory.")
        Container(platform, "Platform Bridge", "C11 + EM_ASM", "2 modules: localStorage bridge (native stub + WASM impl).")
    }

    Rel(user, core, "Keyboard/mouse input")
    Rel(core, render, "render_frame_t per frame")
    Rel(core, systems, "JD + observer → system state")
    Rel(render, ui, "Layout structs → positioned quads")
    Rel(systems, math, "Orbital mechanics, calendar math")
    Rel(ui, systems, "System data → card content")
    Rel(core, platform, "localStorage read/write")
```

---

## Level 3: Component — Render Engine

The render engine is the largest container. How do its passes compose?

```mermaid
C4Component
    title Time — Render Engine Components

    Container_Boundary(render, "Render Engine") {
        Component(main_loop, "Main Loop", "main.c", "Builds render_frame_t, calls all passes in order")
        Component(shader, "Shader System", "shader.c", "Compile/link GLSL programs")

        ComponentDb(frame, "render_frame_t", "Per-frame data: JD, matrices, layers, brain visual, theme")

        Component(star_pass, "Star Pass", "star_pass.c", "10K+ stars from catalog, magnitude-based size")
        Component(planet_pass, "Planet Pass", "planet_pass.c", "8 planets + Sun, Keplerian orbits, sqrt distance scale")
        Component(zodiac_pass, "Zodiac Pass", "zodiac_pass.c", "12-sign ecliptic ring with degree markers")
        Component(ring_pass, "Ring Pass", "ring_pass.c", "Concentric depth rings for knowledge systems")
        Component(card_pass, "Card Pass", "card_pass.c", "2D card backgrounds + oracle cross + wavespell strip")
        Component(text_pass, "Text Pass", "text_pass.c", "3D planet labels (bitmap) + 2D MSDF card/oracle text")
        Component(convergence_pass, "Convergence Pass", "convergence_pass.c", "Lines connecting systems when brain detects alignment")
        Component(post_pass, "Post Pass", "post_pass.c", "Full-screen bloom, vignette, color grading")

        Component(other_passes, "11 More Passes", "milkyway, constellation, orbit_trail, moon, saturn, earth, hexagram, bodygraph, tree, deep_sky, diffraction", "Specialized visual layers")
    }

    Rel(main_loop, frame, "Builds")
    Rel(main_loop, star_pass, "draw(frame)")
    Rel(main_loop, planet_pass, "draw(frame)")
    Rel(main_loop, zodiac_pass, "draw(frame)")
    Rel(main_loop, ring_pass, "draw(frame)")
    Rel(main_loop, card_pass, "draw(frame)")
    Rel(main_loop, text_pass, "draw(frame)")
    Rel(main_loop, convergence_pass, "draw(frame)")
    Rel(main_loop, post_pass, "draw(frame)")
```

---

## Level 3: Component — Knowledge Systems

34 cultural/astronomical computation domains, all pure functions.

```mermaid
C4Component
    title Time — Knowledge System Components

    Container_Boundary(systems, "Knowledge Systems (34 domains)") {

        Component(astronomy, "Astronomy", "planets.c, lunar.c, sidereal.c", "Keplerian orbits, lunar phases, GST")
        Component(astrology, "Astrology", "zodiac.c, dignity.c, aspects.c", "Signs, dignities, planetary aspects")
        Component(tzolkin, "Tzolkin/Dreamspell", "dreamspell.c, tzolkin.c, haab.c + 15 more", "260-day cycle, oracle, wavespell, Synchronotron, Telektonon")
        Component(iching, "I Ching", "iching.c, iching_interpret.c", "64 hexagrams, King Wen sequence, judgments")
        Component(chinese, "Chinese", "chinese.c", "Sexagenary cycle, stems, branches, animals")
        Component(hd, "Human Design", "human_design.c, bodygraph.c", "64 gates, 36 channels, incarnation cross")
        Component(hebrew, "Hebrew", "hebrew.c, hebrew_interpret.c", "Lunisolar calendar, Metonic cycle, month meanings")
        Component(islamic, "Islamic", "hijri.c, islamic_interpret.c", "Pure lunar Hijri, month significance")
        Component(buddhist, "Buddhist", "buddhist.c", "Buddhist Era, Uposatha observances")
        Component(hindu, "Hindu/Vedic", "panchanga.c, nakshatra.c", "Panchanga 5 limbs, 27 Nakshatras")
        Component(kabbalah, "Kabbalah", "sefirot.c, tree_of_life.c", "10 Sefirot, 22 paths")
        Component(geology, "Geology", "geo_time.c", "Eons, eras, periods from Julian Day")
        Component(unified, "Unified/Brain", "brain_scan.c, convergence_detect.c", "Cross-system correlation, narrative generation")

        Component(other_sys, "21 More Systems", "coptic, ethiopian, persian, japanese, korean, thai, aztec, bahai, balinese, celtic, chakra, earth, egyptian, french_republican, lao, myanmar, numerology, tamil, tarot, zoroastrian", "World calendars and cultural systems")
    }
```

---

## Level 3: Component — UI Layer

Layout computation, card pipeline, theming.

```mermaid
C4Component
    title Time — UI Layer Components

    Container_Boundary(ui, "UI Layer (131 modules)") {

        Component(card_pipeline, "Card Pipeline", "card_layout.c, card_selector.c, card_style.c, card_data.c, today_card.c", "Zoom-aware card selection → system-specific formatting → themed styling")

        Component(kin_layouts, "Kin Maya Layouts (27)", "kin_oracle_layout, kin_wavespell_layout, kin_board_layout, kin_castle_layout, kin_color_compass + 22 more", "Oracle cross, wavespell strip, 260-board, castles, color compass, Synchronotron, Telektonon")

        Component(daily_layouts, "Daily Layouts (8)", "daily_hebrew/islamic/buddhist/hindu/chinese/iching/hd/transit_layout", "Per-system daily card with positioned slots and rich text")

        Component(visual_layouts, "Visual Layouts (10)", "natal_chart, zodiac_wheel, hexagram, bagua, bodygraph_chart, tree_of_life, zodiac_animals, nakshatra_wheel, prayer_times, dignity_table", "Charts, wheels, graphs for dedicated render passes")

        Component(theme, "Theme System", "theme.c, golden_layout.c, color_theory.c", "Cosmos/Dawn themes, phi-based spacing, golden angle palettes")

        Component(focus, "Focus Modes", "focus_mode.c", "K=Kin, A=Astrology, I=IChing, C=Chinese, D=HD — per-system layer dimming")

        Component(content, "Content/i18n", "content_i18n.c, content_generated.h", "Localized strings, archetype content")
    }

    Rel(card_pipeline, daily_layouts, "today_card_for_system() calls daily compute")
    Rel(focus, card_pipeline, "Focus mode selects system")
    Rel(theme, card_pipeline, "Card colors from theme")
```

---

## Data Flow: One Frame

How data flows from simulation time to pixels on screen.

```mermaid
flowchart LR
    subgraph Input
        JD[Julian Day]
        OBS[Observer lat/lon]
        KEY[Keyboard/mouse]
    end

    subgraph Core
        MAIN[main_loop]
        FRAME[render_frame_t]
    end

    subgraph Systems
        ASTRO[Astronomy]
        TZOL[Tzolkin]
        BRAIN[Brain Scan]
        OTHER[30+ systems]
    end

    subgraph UI
        CARD[Card Pipeline]
        ORACLE[Oracle Layout]
        THEME[Theme]
    end

    subgraph Render
        STARS[star_pass]
        PLANETS[planet_pass]
        CARDS[card_pass]
        TEXT[text_pass]
        CONV[convergence_pass]
        POST[post_pass]
    end

    JD --> MAIN
    OBS --> MAIN
    KEY --> MAIN
    MAIN --> FRAME
    MAIN --> ASTRO
    MAIN --> TZOL
    MAIN --> BRAIN
    MAIN --> OTHER
    ASTRO --> FRAME
    BRAIN --> FRAME
    FRAME --> STARS
    FRAME --> PLANETS
    FRAME --> CARDS
    FRAME --> TEXT
    FRAME --> CONV
    STARS --> POST
    PLANETS --> POST
    CARDS --> POST
    TEXT --> POST
    CONV --> POST
    CARD --> TEXT
    ORACLE --> CARDS
    ORACLE --> TEXT
    THEME --> CARDS
    THEME --> TEXT
```

---

## Build Pipeline

```mermaid
flowchart TD
    SRC[src/ — 481 .c files]
    TESTS[tests/ — 518 test suites]
    CMAKE[CMakeLists.txt + cmake/testing.cmake]

    subgraph Native Build
        GCC[gcc -Wall -Wextra -Werror -std=c11]
        CTEST[ctest -j12]
    end

    subgraph WASM Build
        EMCC[emcc via emcmake]
        WASM[index.wasm — 618 KB]
        HTML[index.html + style.css]
    end

    subgraph Deploy
        SERVE[python3 -m http.server 8080]
        BROWSER[WebGL2 Browser]
    end

    SRC --> CMAKE
    TESTS --> CMAKE
    CMAKE --> GCC --> CTEST
    CMAKE --> EMCC --> WASM
    WASM --> HTML --> SERVE --> BROWSER
```

---

## Stream Coordination Flow

9 streams + MEGA + NERVE — who talks to whom, inbox/outbox routing.

```mermaid
flowchart TD
    subgraph Command["Command Layer"]
        GABRIEL["Gabriel (launches sessions)"]
        MEGA["MEGA (primary — vision, coordination)"]
        NERVE["NERVE (first mate — verify, unblock)"]
    end

    subgraph Builders["Builder Streams"]
        VISUALS["VISUALS — How it LOOKS\n(layouts, passes, shaders)"]
        LANGUAGE["LANGUAGE — How it SPEAKS\n(i18n, fonts, content)"]
        MOTION["MOTION — How it MOVES\n(animation, transitions)"]
        AUDIO["AUDIO — How it SOUNDS\n(planetary harmonics, tones)"]
        DEPTH["DEPTH — How DEEP it goes\n(interpretation, wisdom)"]
        BRAIN["BRAIN — How it THINKS\n(convergence, narrative)"]
        INFRA["INFRA — How it STAYS HEALTHY\n(build, test, wiring, metrics)"]
        PERSONA["PERSONA — How it KNOWS YOU\n(birth profile, preferences)"]
    end

    GABRIEL -->|"/stream {name}"| VISUALS & LANGUAGE & MOTION & AUDIO & DEPTH & BRAIN & INFRA & PERSONA
    GABRIEL -->|"/mega"| MEGA
    GABRIEL -->|"/stream nerve"| NERVE

    MEGA -->|"inbox.md tasks"| VISUALS & LANGUAGE & MOTION & AUDIO & DEPTH & BRAIN & INFRA & PERSONA
    NERVE -->|"verify claims\nwrite to inboxes"| VISUALS & LANGUAGE & INFRA

    VISUALS -->|"outbox → INFRA inbox\n(wire my layouts!)"| INFRA
    LANGUAGE -->|"outbox → INFRA inbox\n(MSDF font ready)"| INFRA
    BRAIN -->|"outbox → DEPTH inbox\n(interpretation needed)"| DEPTH
    INFRA -->|"outbox → MEGA\n(architecture decisions)"| MEGA

    style MEGA fill:#f9d71c,color:#000
    style NERVE fill:#7cb9e8,color:#000
    style INFRA fill:#90ee90,color:#000
```

### File-Based Communication Protocol

```mermaid
flowchart LR
    subgraph StreamA["Stream A"]
        A_STATUS["status.md\n(phase, readiness)"]
        A_OUTBOX["outbox.md\n(sent log)"]
    end

    subgraph StreamB["Stream B"]
        B_INBOX["inbox.md\n(pending messages)"]
        B_STATUS["status.md"]
    end

    subgraph Global["Global Files"]
        CROSS["CROSS.md\n(dependency index)"]
        MVP["MVP.md\n(progress scores)"]
    end

    A_OUTBOX -->|"Stream A writes\ndirectly to B's inbox"| B_INBOX
    A_OUTBOX -.->|"copy for audit"| CROSS
    NERVE -->|"reads all status.md\nverifies claims"| A_STATUS & B_STATUS
    NERVE -->|"updates scores"| MVP
```

---

## Knowledge Pipeline

Book acquisition → extraction → routing → code → display.

```mermaid
flowchart TD
    subgraph Acquisition["1. Acquisition"]
        INBOX_DIR["~/Desktop/temp/time/inbox/\n(Gabriel drops PDFs)"]
        CATALOG["docs/checklists/books.md\n(204 acquired)"]
        MANIFEST[".context/library/manifest.json"]
    end

    subgraph Extraction["2. Extraction"]
        QUESTIONS[".context/research/\nEXTRACTION_QUESTIONS.md"]
        DELTA["DELTA agent\n(picks book, extracts)"]
        DIGESTS[".context/research/digested/\n(85+ digests)"]
    end

    subgraph Routing["3. Routing"]
        MEGA_ROUTE["MEGA reads digest\n→ identifies findings"]
        STREAM_INBOX["Target stream inbox.md\n(specific task spec)"]
    end

    subgraph Code["4. Code"]
        SYSTEM_C["src/systems/{name}/*.c\n(algorithms, data)"]
        UI_C["src/ui/*_layout.c\n(visual layouts)"]
        TESTS_C["tests/**/test_*.c\n(TDD verification)"]
    end

    subgraph Display["5. Display"]
        TODAY_CARD["today_card.c\n(card text enrichment)"]
        CARD_PASS["card_pass.c\n(background quads)"]
        TEXT_PASS["text_pass.c\n(MSDF text overlay)"]
        BROWSER["Browser — user sees it"]
    end

    subgraph Attribution["Attribution"]
        CONTRIBUTORS["data/contributors.json\n(186+ humans honored)"]
    end

    INBOX_DIR -->|"MEGA scans on startup"| CATALOG
    CATALOG --> MANIFEST
    MANIFEST --> QUESTIONS
    QUESTIONS --> DELTA
    DELTA -->|"read PDF, answer questions"| DIGESTS
    DIGESTS --> MEGA_ROUTE
    MEGA_ROUTE -->|"route findings to\nowning stream"| STREAM_INBOX
    STREAM_INBOX -->|"stream builds modules"| SYSTEM_C & UI_C
    SYSTEM_C --> TESTS_C
    UI_C --> TESTS_C
    UI_C --> TODAY_CARD
    TODAY_CARD --> TEXT_PASS
    SYSTEM_C --> CARD_PASS
    TEXT_PASS --> BROWSER
    CARD_PASS --> BROWSER
    DELTA -->|"every author"| CONTRIBUTORS

    style DIGESTS fill:#f0e68c,color:#000
    style BROWSER fill:#90ee90,color:#000
    style CONTRIBUTORS fill:#dda0dd,color:#000
```

---

## Art Asset Pipeline

Reference material → generation → optimization → display.

```mermaid
flowchart LR
    subgraph Reference["1. Reference"]
        BOOKS["Library books\n(Itten, Albers, Tufte)"]
        RESEARCH["Stream research docs\n(visual surveys)"]
    end

    subgraph Generation["2. Generation"]
        PROMPT["DALL-E / Midjourney\nprompt crafting"]
        GABRIEL_GEN["Gabriel generates\nimages externally"]
        RAW["Raw PNG/SVG assets"]
    end

    subgraph Optimization["3. Optimization (ART stream)"]
        COMPRESS["Compress / resize\n(WebGL texture limits)"]
        ATLAS["Atlas packing\n(MSDF, sprite sheets)"]
        C_HEADER["gen_msdf_header.py\n→ C header with pixel data"]
    end

    subgraph Integration["4. Integration (INFRA)"]
        DATA_H["src/render/*_data.h\n(embedded atlas)"]
        PASS_C["Render pass .c\n(GL texture upload)"]
        WASM["WASM binary\n(asset embedded)"]
    end

    BOOKS --> PROMPT
    RESEARCH --> PROMPT
    PROMPT --> GABRIEL_GEN --> RAW
    RAW --> COMPRESS --> ATLAS --> C_HEADER
    C_HEADER --> DATA_H --> PASS_C --> WASM

    style GABRIEL_GEN fill:#f9d71c,color:#000
    style WASM fill:#90ee90,color:#000
```

---

## Experience Verification Pipeline

How we verify that code changes produce visible, correct output.

```mermaid
flowchart TD
    subgraph CodeChange["1. Code Change"]
        WRITER["Writer agent\n(TDD: test → header → code)"]
        BUILD["cmake --build\n(zero warnings)"]
        CTEST["ctest -j12\n(518+ unit/contract tests)"]
    end

    subgraph WASMVerify["2. WASM Verification"]
        WASM_BUILD["emcmake cmake\n→ index.wasm (618 KB)"]
        SERVE["python3 -m http.server 8080"]
        BROWSER["Open in Chrome/Firefox"]
    end

    subgraph VisualCheck["3. Visual Verification"]
        SCREENSHOT["Manual screenshot\nor Playwright E2E"]
        COMPARE["Compare against\nexpected output"]
        PASS_FAIL{Pass?}
    end

    subgraph Feedback["4. Feedback Loop"]
        PASS["✅ Commit + push\nUpdate status.md"]
        FAIL["❌ File bug to\nowning stream inbox"]
        WIRING["🟡 Module works but\ndisplay blank → INFRA"]
    end

    WRITER --> BUILD --> CTEST
    CTEST -->|"all pass"| WASM_BUILD
    CTEST -->|"failure"| WRITER
    WASM_BUILD --> SERVE --> BROWSER
    BROWSER --> SCREENSHOT --> COMPARE --> PASS_FAIL
    PASS_FAIL -->|"yes"| PASS
    PASS_FAIL -->|"no — logic bug"| FAIL
    PASS_FAIL -->|"no — not wired"| WIRING

    style PASS fill:#90ee90,color:#000
    style FAIL fill:#ff6b6b,color:#000
    style WIRING fill:#ffd700,color:#000
```

---

*Diagrams follow the [C4 model](https://c4model.com/) and process flow conventions using [Mermaid](https://mermaid.js.org/) syntax. Render in any Mermaid-compatible viewer (GitHub, VS Code, etc.).*
