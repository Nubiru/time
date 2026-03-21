# Time — Minimum Viable Propositions

**What Time IS. What it DOES. What users can DO with it.**

This is the permanent product checklist. Every `/mega` cycle checks: which MVPs are delivered? Which are in progress? Which haven't started? This directs ALL stream work.

---

## LARGE MVPs (15 — the pillars of the experience)

| # | MVP | Description | Status |
|---|-----|-------------|--------|
| L1 | **Today in All Systems** | Open Time → see today's date in 25+ calendar systems simultaneously | Data: ✅ Display: ❌ |
| L2 | **Your Birth Moment** | Enter birthday → see the sky as it was, your natal chart, your Kin, your hexagram, everything | Data: ✅ Display: ❌ |
| L3 | **The Daily Narrative** | Time tells you what today MEANS — convergences, energies, themes across traditions | Engine: ✅ Display: ❌ |
| L4 | **Earth Through Time** | Cinematic 4.5Ga voyage: Pangea → continents, dinosaurs → humans, chemical composition, population counter | Data: partial Display: ❌ |
| L5 | **The Concentric Wheel** | Layered information — zoom from today → cycle → era → cosmos. Fly through depth. 3D card navigation. | Module: ✅ Wired: partial |
| L6 | **Sound of the Spheres** | Planetary frequencies, convergence chords, viewpoint soundscapes. Pianist recordings. | Engine: ✅ Content: ❌ |
| L7 | **21 Languages** | Every text in 21 languages with poetic voice influenced by humanity's greatest minds | UI labels: ✅ 111 keys × 21 langs. Content: ✅ EN 3534 / ES 3533 / PT 3534 keys. Spanish + Portuguese COMPLETE. 2/21 languages done. Wisdom voice: 10 archetypes defined. |
| L8 | **The "Enter Time" Experience** | Click threshold → cinematic zoom → solar system reveals → first hint → discovery | CSS: ✅ Animation: ✅ Wiring: ✅ (commit ff0b1fd). Camera fly-in active on load. Skip on key/touch. |
| L9 | **Personal Space (Diamond Room)** | Your identity across all systems. Birth chart + Kin + hexagram + HD. Customizable 3D space. | Data: ✅ Display: ❌ |
| L10 | **Wave Watch & Earth View** | Tide, surf, weather from astronomical computation. 24h forecast. Surfer's dream. | Data: ✅ Display: ❌ |
| L11 | **6-8 Named 3D Views** | Space View, Earth View, Galaxy View, Personal View, Deep Time View, City View, Room View + more | Registry: ✅ Wired: 2/7 |
| L12 | **City View — News Around You** | Local events, restaurants, shows, circus near your location. Social layer. | ❌ NOT STARTED (decision needed: include or focus on TIME?) |
| L13 | **Meet Someone Like You** | Find people with similar Kin/interests nearby. Privacy-first social matching. | ❌ Concept only |
| L14 | **Voice of Time** | ElevenLabs narration in Gabriel's voice. Toggle: read / voice-on / voice-off. Accessibility. | ❌ NOT STARTED |
| L15 | **Share a Moment** | Capture any moment as shareable URL + text + screenshot. Social virality. | Data: ✅ Display: ❌ |

---

## SMALL MVPs (50+ — the features that make it alive)

### Temporal Axis (BRAIN + DEPTH)

| # | MVP | Status |
|---|-----|--------|
| S1 | Convergence detector (cross-system alignment) | ✅ Built (21 systems) |
| S2 | Convergence ARCHIVE (historical notable dates) | ✅ Built (43 dates) |
| S3 | Convergence FORECAST (upcoming 30 days) | ✅ Built |
| S4 | Daily narrative generation | ✅ Engine built, narrative generic |
| S5 | Hexagram 64 judgment + image text (all 64) | ✅ Extracted from Wilhelm |
| S6 | Interpretation for ALL 33 systems | ✅ 33/33 interpret modules |
| S7 | Wisdom quotes correlated to context | ✅ Engine built, ❌ display |
| S8 | Fun facts with convergence triggers | ✅ Built, ❌ display |
| S9 | Earth geological timeline data | ✅ geo_time, tectonic, fossil |
| S10 | Human population history (109B ever, 8B now) | ✅ Built (`human_population.h/.c`, 49 tests). Display: ❌ needs card adapter |
| S11 | Chemical composition of Earth through time | ❌ NOT BUILT |
| S12 | Pangea → continents cinematic narrative | ❌ NOT BUILT (earth_story queued) |
| S13 | Dinosaur timeline display | ✅ fossil_narrative built, ❌ display |

### Sensory Axis (VISUALS + AUDIO + MOTION)

| # | MVP | Status |
|---|-----|--------|
| S14 | 19 render passes wired | ✅ |
| S15 | Pass scheduling by view + focus + LOD | ✅ |
| S16 | Concentric ring renderer | ✅ wired, ❌ real data flowing |
| S17 | Earth View (globe + coastlines + terminator) | ✅ pass exists |
| S18 | Focus modes (A/K/I/C/D keys) | ✅ wired |
| S19 | "Enter Time" threshold CSS | ✅ built |
| S20 | Zoom animation (threshold → solar system) | ✅ Built + WIRED (commit ff0b1fd). enter_zoom in main_loop. Accessibility: prefers-reduced-motion. |
| S21 | Card fly-through between layers | ✅ Built + WIRED (commits 7f761b4 + 695f3c2). 6 depth stops, spring animation, crossfade. PageDown/PageUp + Escape cancel. |
| S22 | Universal card layout | ✅ WIRED (commit a0c7676). Card backgrounds + text themed per system. card_pass + text_pass render live. Remaining: populate card_content from interpret modules. |
| S23 | Birth moment sky rendering | ✅ Built + Wired (commit bd00931). B key toggles. 25 tests. Default date only — needs onboarding wire. |
| S24 | Audio engine (WebAudio) | ✅ built |
| S25 | Planetary frequency oscillators | ✅ built |
| S26 | Cultural timbres (10 traditions) | ✅ built |
| S27 | Meditation mode (phi breathing) | ✅ built |
| S28 | Audio tested in browser | ❌ NEVER TESTED |
| S29 | Spring physics interactions | ✅ built |
| S30 | Camera fly-to paths | ✅ built |
| S31 | Touch gestures (pinch/swipe/tap) | ✅ wired |
| S32 | Responsive CSS (4 breakpoints) | ✅ built |
| S33 | Kin Maya seal digital assets | 🔄 3/20 generated (DALL-E) |

### Personal Axis (PERSONA + LANGUAGE)

| # | MVP | Status |
|---|-----|--------|
| S34 | Birth profile computation (all systems) | ✅ built |
| S35 | Cosmic fingerprint (identity card) | ✅ built |
| S36 | Cycle tracker (5 personal cycles) | ✅ built |
| S37 | Annual summary ("Spotify Wrapped for time") | ✅ built |
| S38 | User preferences persistence | ✅ built |
| S39 | Privacy framework | ✅ built |
| S40 | Settings panel UI | ✅ Data built (`settings_panel.h/.c`, 54 tests, 4 sections). Display: ❌ needs UI renderer |
| S41 | Birth date entry onboarding | ✅ Data built (`birth_entry.h/.c`, 84 tests, 3-step state machine). Display: ❌ needs wheel UI |
| S42 | "Author of Time" profile card | ✅ Built (`author_card.h/.c`, 38 tests). Display: ❌ needs card renderer |
| S43 | 21 language translations (ES, HE, PT, AR, ZH, FR, DE, JA, HI, KO, RU, TR, ID, BN, VI, TH, FA, MY, SW, AM) | ✅ ALL 21 shipped |
| S44 | Wisdom voice guide (how great minds communicate) | ✅ Built (`data/content/voice_guide.json`, 10 archetypes, 124 keys, commit 537c80a). Display: ❌ |
| S45 | Wisdom display module | ✅ built |
| S46 | 30 narrative templates (5 per thread) | ✅ built |
| S47 | Seasonal greetings | ✅ built |
| S48 | 100 greatest humans deep voice study | ❌ Researched but not fully extracted |
| S49 | ElevenLabs voice-over | ❌ NOT STARTED |
| S50 | Wisdom display WIRED to UI | ❌ NOT WIRED |

### Views & Navigation

| # | MVP | Status |
|---|-----|--------|
| S51 | Space View (solar system from outside) | ✅ wired (default) |
| S52 | Earth View (globe + coastlines + E key) | ✅ pass exists |
| S53 | Galaxy View (Milky Way scale, Shift+5-6) | ✅ pass exists |
| S54 | Personal View (birth chart focus) | ❌ NOT BUILT |
| S55 | Deep Time View (geological timeline cinematic) | ❌ NOT BUILT |
| S56 | City View (local horizon + sky + social) | ❌ NOT BUILT |
| S57 | Room View (personal 3D diamond space) | ✅ Data built (`diamond_room.h/.c`, 79 tests, commit 046bfbf). Crystal from birth profile. Display: ❌ |
| S58 | View selector UI (switch between views) | ❌ NOT BUILT |
| S59 | View transition animations (smooth fly between) | 🔄 Partial — earth_transition wired (E key, commit ff0b1fd), birth_flight wired (B key). Other views need similar choreography. |

### Social & Community

| # | MVP | Status |
|---|-----|--------|
| S60 | Local events feed (restaurants, shows, circus) | ❌ Decision needed |
| S61 | Kin discovery (find similar profiles nearby) | ❌ Needs backend decision |
| S62 | "Get to know the Author" profile card | ❌ NOT BUILT |
| S63 | Share URL with JD + view + systems | ✅ share_moment built |
| S64 | Social preview (Open Graph meta tags) | ❌ NOT BUILT |

### Earth Science Display

| # | MVP | Status |
|---|-----|--------|
| S65 | Human population counter (109B total, 8B alive) | ❌ NOT BUILT |
| S66 | Earth chemical composition timeline | ❌ NOT BUILT |
| S67 | Pangea → continents cinematic narrative | ❌ earth_story queued |
| S68 | Atmospheric evolution (reducing → oxidizing → ozone → modern) | ❌ climate_history has data |
| S69 | Surf forecast 24h display | ❌ data exists, display NOT BUILT |
| S70 | Wave watch real-time | ❌ data exists, display NOT BUILT |

### Digital Assets

| # | MVP | Status |
|---|-----|--------|
| S71 | 20 Kin Maya seal icons | ✅ 20/20 generated (DALL-E v2). In `.assets/generated/seals/`. Optimization pending. |
| S72 | 13 Kin Maya tone icons | ❌ references exist |
| S73 | 12 Zodiac sign glyphs | 🔄 1/12 generated |
| S74 | 9 Planet icons | 🔄 1/9 generated |
| S75 | 7 Plasma symbols | ❌ references exist |
| S76 | 19 Archetype character art | ❌ references exist |
| S77 | UI card frame designs | ❌ NOT STARTED |

### Infrastructure

| # | MVP | Status |
|---|-----|--------|
| S78 | CI/CD pipeline | ✅ GitHub Actions |
| S79 | E2E browser test | ✅ 5 Playwright tests (commit 2562b6a). Not in CI yet. |
| S80 | Performance benchmarks | ✅ native benchmarks |
| S81 | Service worker (offline) | ✅ built |
| S82 | Deployment to Cloudflare Pages | ❌ NOT DEPLOYED |
| S83 | Domain name | ❌ NOT CHOSEN |
| S84 | Error monitoring (Sentry or similar) | ❌ NOT STARTED |

---

## Score

**Large MVPs**: 2/15 near-delivered (L7 ES+PT COMPLETE, L8 "Enter Time" wired). L7: 3 languages, 3534 keys each.
**Small MVPs**: ~85/120 built, ~35 remaining. 496 tests pass. 110K LOC (src/), 469 .c files.
**Overall**: Infrastructure 94%. Modules 85%. Display 33%. Experience 20%. WASM 365 KB.
**New MVPs added**: S108-S113 (Audio/Voice), S114-S120 (Kin Maya Visual Layouts). Total: 120.
**Wiring gap**: 27 Kin Maya layouts + 33 interpret modules + 18 PERSONA modules = massive data. 0 Kin Maya layouts wired to card_pass.

**MILESTONES (2026-03-20)**:
- Cards visible (a0c7676) + polished (aa3e791)
- "Enter Time" zoom WIRED (ff0b1fd) — camera fly-in active on page load
- localStorage bridge DELIVERED (668cc19) — PERSONA WASM unblocked
- 105K LOC milestone (1d8da43)
- **LANGUAGE SPANISH COMPLETE** (7c8b9da): 3534 EN / 3533 ES keys, 33/33 systems, 100% key coverage
- Brain wiring LIVE (38c850a): headline + convergence_motion in main_loop
- Convergence visualization READY (5867a0d): quad lines + ring glow, dormant until MSG-V012
- 6 world calendar palettes (ff08e8d): Coptic, Ethiopian, Persian, Japanese, Korean, Thai
- 15 E2E tests (e9bd455): 7 pass, 2 skip, 1 fail (SwiftShader)
- BRAIN explain DB: 75 entries, 72 score pairs, 70 stored dates

### NERVE Audit Notes (2026-03-20)
- **BRAIN S85, S86, S87, S96**: ✅ Verified in `brain_stats.h/.c` (commit f3d0042). 28 tests.
- **BRAIN S98, S100**: ✅ Verified `red_thread.h/.c` + `system_correlation.h/.c` (commit 04958a2).
- **BRAIN S99, S101**: ✅ Verified `grand_cycle.h/.c` + `time_entropy.h/.c` (commit f236b1f).
- **S106**: ✅ Verified natal aspects in `birth_sky.c` (commit 96e0168). 6 aspect tests.
- **S23**: ✅ Birth sky wired (commit bd00931). B key works. Default 2000-01-01.
- **Stream test count discrepancies**: MOTION verified 416 RUN_TEST across 20 modules (previous claim 389 was undercount, NERVE's 215 was partial scan). DEPTH claims 1360+, isolated depth tests show 287 (may include cross-system interpret tests).
- **All module code verified as real implementations, not stubs.**

---

## How to Use This File

Every `/mega` cycle: scan this list. Find the highest-impact ❌. Queue it to a stream.
Every stream session: check which MVPs your work addresses. If none — you're building plumbing, not experience.

### Cross-System Intelligence & Discovery (NEW — from vision audit)

| # | MVP | Status |
|---|-----|--------|
| S98 | **Red Thread Finder** — Cross-system invisible connection discovery. "Eclipse + Kin 1 + Rosh Hashanah = unprecedented convergence — here's what each tradition says about it" | ✅ Built (commit 04958a2). `red_thread.h/.c` — `rt_compose()`, `rt_classify()`. Display: ❌ |
| S99 | **Grand Cycle LCM** — Compute LCM of all 21 system periods. "This exact configuration won't recur for X million years." Computable, mind-blowing. | ✅ Built (commit f236b1f). `grand_cycle.h/.c` — 9 built-in cycles. Display: ❌ |
| S100 | **System Correlation Matrix** — 21×21 grid showing which system pairs co-fire most often across decades. Heatmap of resonance. | ✅ Built (commit 04958a2). `system_correlation.h/.c` — `sc_corr_compute()`, `sc_corr_top_pairs()`. Display: ❌ |
| S101 | **Time Entropy Display** — Shannon entropy of today's alignment pattern. "Today's time-entropy: 0.87 — 87% of maximum information." Most rigorous "wow" metric. | ✅ Built (commit f236b1f). `time_entropy.h/.c` — `te_entropy()`, `te_interpret()`. Display: ❌ |
| S102 | **Time Travel Explorer** — Navigate to any date in history, see all 25+ systems at that moment. Historical exploration mode. | ❌ ts_compute works for any JD |
| S103 | **Deep Tzolkin Experience** — 260-day interactive board, wavespell journey, Dreamspell game mechanics. Juan's daily Kin energy page. | 🔄 Data modules: kin_board (260 cells), kin_oracle, kin_wavespell_journey, kin_daily_chrono, kin_pulsar, kin_castle, kin_moon_page, heptad_gate. Display: ❌ |
| S104 | **Calendar Round Flower** — 52-year visualization showing Tzolkin + Haab interlocking cycles. Sacred geometry mandala. | ✅ Built (`kin_round_layout.h/.c`, 24 tests, commit 277dde8). Display: ❌ |

### Personalization & Adaptive Experience (NEW — from vision audit)

| # | MVP | Status |
|---|-----|--------|
| S105 | **Usage-Based Personalization** — Automatic interest profile from behavior. "Visited Kin Maya 4/7 days" → surface Kin content. No forms, no questions. Privacy-first. | ✅ Built (`usage_interest.h/.c`, 62 tests). Display: ❌ |
| S106 | **Aspect Lines at Birth** — Visible planet connection lines rendered in birth moment view. Geometric beauty of your natal sky. | ✅ Built (commit 96e0168). Natal aspects + geo_longitudes in `birth_sky_t`. 25 tests. Display: ❌ |
| S107 | **Procedural Glyph Generation** — Code-generated seals, zodiac, planet icons. "Every pixel earned" alternative/fallback to AI/human art. | ❌ NOT BUILT (design decision needed) |

### Audio & Voice Experience (NEW — from Gabriel vision 2026-03-20)

| # | MVP | Status |
|---|-----|--------|
| S108 | **Seal tone frequencies** — Each of 20 seals plays a unique tone when its card is focused. Map seal→planet→frequency from `audio_data.h`. | ✅ Built (commit 3b02690). Dreamspell Planetary Holon: 20 seals→10 planets, P5/8va multipliers, slot 8. 28 tests. |
| S109 | **Color family soundscapes** — Red=drums, White=wind, Blue=water, Yellow=fire. Background ambient shifts with active seal color. | ❌ AUDIO + pianist |
| S110 | **Archetype voice-over (Gabriel)** — Gabriel records 20 archetype poems. ElevenLabs clones voice for other languages. Toggle: text/voice/off. | ❌ Gabriel records → ElevenLabs |
| S111 | **Convergence chord** — When systems converge, their frequencies resolve into a chord. Rare convergence = richer chord. | ✅ Built (commit 9de7091). Multi-system timbre blend: average freq weighted by score, merged partials. |
| S112 | **Wavespell musical motif** — Each 13-day wavespell has a motif that evolves from tone 1→13. Pianist collaboration. | ❌ LONG-TERM, pianist |
| S113 | **Narration accessibility** — Voice reads daily narrative aloud. Blind users get full experience. Toggle in settings. | ❌ ElevenLabs + PERSONA |

### Kin Maya Visual Layouts (NEW — from visual inventory 2026-03-20)

| # | MVP | Status |
|---|-----|--------|
| S114 | **Oracle cross layout** — 5-kin daily reading: center + guide/analog/antipode/occult. THE daily Kin Maya view. | ✅ Built (`kin_oracle_layout.h/.c`, 31 tests, commit 63b097c). Display: ❌ |
| S115 | **Kin card (complete daily info)** — Seal + tone + color + oracle + wavespell position + castle. | ✅ Built (`kin_cell.h/.c` 24 tests + `kin_card_layout.h/.c` 35 tests, commits 63b097c + bd73cc2). Display: ❌ |
| S116 | **Wavespell skeleton** — 13 connected cells showing current position in 13-day journey. | ✅ Built (`kin_wavespell_layout.h/.c`, 31 tests, commit 63b097c). Display: ❌ |
| S117 | **260-day Harmonic Module board** — Full 20×13 grid with today highlighted. Seals Y, tones X. | ✅ Built (`kin_board_layout.h/.c`, commit bd73cc2). Display: ❌ |
| S118 | **Concentric Tzolkin wheel** — Seals outer, tones inner, color quadrants. Interactive rotation. | ✅ Built (`kin_wheel_layout.h/.c`, commit 277dde8). Display: ❌ |
| S119 | **Calendar Round wheel** — 52-year cycle with kin + Gregorian year. | ✅ Built (`kin_round_layout.h/.c`, commit 277dde8). Display: ❌ |
| S120 | **Telektonon game board** — 28-day prophecy journey. Needs book extraction first. | ✅ Data built: `telektonon_board.h/.c` + `telektonon_journey.h/.c` + `telektonon_cards.h/.c` + `force_lines.h/.c` + `clear_signs.h/.c` (DEPTH). Layout: `kin_telektonon_layout.h/.c` (VISUALS L11). Display: ❌ |

### Statistics & Mathematical Displays (from MEGA audit)

| # | MVP | Status |
|---|-----|--------|
| S85 | **Convergence Year Rating** — rate each year by total convergence strength. "2026 scores 203/365 days with alignments" | ✅ Built (commit f3d0042). `brain_stats.h/.c` — `br_stats_year_score()`. Display: ❌ |
| S86 | **"How rare is today?"** — percentile ranking. "Today is in the top 5% of convergent days this decade" | ✅ Built (commit f3d0042). `brain_stats.h/.c` — `br_stats_percentile()`. Display: ❌ |
| S87 | **The 2012 Analysis** — what ACTUALLY converged on 13.0.0.0.0? How does it compare to other dates? Demystify with data. | ✅ Built (commit f3d0042). Entry in `stored_convergences`. Display: ❌ |
| S88 | **Decade convergence heatmap** — visual grid showing convergence intensity across years. Find meta-patterns. | ✅ Built (commit 84721d7). `decade_heatmap.h/.c` — 12yr×366day grid. 20 tests. Display: ❌ |
| S89 | **Sacred number frequency analysis** — how often do 7, 12, 13, 19, 22, 64, 108, 260, 432 appear across systems on the same day? | ✅ Built (commit 84721d7). `sacred_scan.h/.c` — scans 6 calendars for 13 sacred numbers. 27 tests. Display: ❌ |
| S90 | **Precession clock** — where are we in the 25,772-year cycle? Visual display showing the Great Year position. | ❌ precession_detect.h exists |
| S91 | **Human existence as fraction of Earth** — "Humans have existed for 0.0065% of Earth's history" with visual scale | ✅ Built (`earth_fraction.h/.c`, commit 5f04907, 395 test LOC). Display: ❌ |
| S92 | **"You are alive during..."** — probability of being alive NOW vs any point in history (7.4% of all humans who ever lived are alive today) | ✅ Built (`earth_fraction.h/.c`, commit 5f04907). Display: ❌ |
| S93 | **Heartbeat of humanity** — collective heartbeats since dawn of humans. Running counter. | ❌ biorhythm.h has data |
| S94 | **Convergence calendar for the year** — which days this year will be most aligned? Plan your life around cosmic rhythms. | ✅ BRAIN just built 2026 report! |
| S95 | **Population counter overlay** — live estimate of humans alive, layered on Earth View | ❌ human_population queued |
| S96 | **"Last time this happened"** — for any convergence, find the PREVIOUS occurrence. "The last time these 6 systems aligned was 847 years ago" | ✅ Built (commit f3d0042). `brain_stats.h/.c` — `br_stats_signature_last()`. Display: ❌ |
| S97 | **Monthly convergence forecast** — upcoming 30 days with convergence predictions. "Next strong alignment: April 8" | ❌ convergence_forecast.h exists |
