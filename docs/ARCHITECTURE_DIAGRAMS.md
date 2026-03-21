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

*Diagrams follow the [C4 model](https://c4model.com/) using [Mermaid](https://mermaid.js.org/) syntax. Render in any Mermaid-compatible viewer (GitHub, VS Code, etc.).*
