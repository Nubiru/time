# Time — Project Metrics

**Last refreshed**: 2026-03-06

## Codebase

| Metric | Count |
|--------|-------|
| Source files (.c) | 39 |
| Header files (.h) | 38 |
| Lines of C code | 4570 |
| Lines of test code | 6853 |
| Public functions | 231 |
| Shader files | 2 |
| Scripts | 5 |

## Testing

| Metric | Count |
|--------|-------|
| Test suites | 32 |
| Total tests | 594 |
| Failures | 0 |

### Test Suites

| Suite | Tests | Module |
|-------|-------|--------|
| test_vec3 | 12 | src/math/vec3.c |
| test_mat4 | 10 | src/math/mat4.c |
| test_julian | 10 | src/math/julian.c |
| test_kepler | 11 | src/math/kepler.c |
| test_sidereal | 10 | src/math/sidereal.c |
| test_ecliptic | 11 | src/math/ecliptic.c |
| test_easing | 25 | src/math/easing.c |
| test_arc_geometry | 23 | src/math/arc_geometry.c |
| test_camera | 9 | src/render/camera.c |
| test_camera_scale | 31 | src/render/camera_scale.c |
| test_render_layers | 19 | src/render/render_layers.c |
| test_font_atlas | 22 | src/render/font_atlas.c |
| test_orbit | 28 | src/systems/astronomy/orbit.c |
| test_planets | 9 | src/systems/astronomy/planets.c |
| test_lunar | 21 | src/systems/astronomy/lunar.c |
| test_precession | 23 | src/systems/astronomy/precession.c |
| test_solar_events | 23 | src/systems/astronomy/solar_events.c |
| test_gregorian | 16 | src/systems/gregorian/gregorian.c |
| test_zodiac | 17 | src/systems/astrology/zodiac.c |
| test_aspects | 26 | src/systems/astrology/aspects.c |
| test_observer | 11 | src/systems/astrology/observer.c |
| test_houses | 11 | src/systems/astrology/houses.c |
| test_dignity | 26 | src/systems/astrology/dignity.c |
| test_planetary_hours | 21 | src/systems/astrology/planetary_hours.c |
| test_cross_validation | 25 | (cross-system integration) |
| test_tzolkin | 14 | src/systems/tzolkin/tzolkin.c |
| test_iching | 18 | src/systems/iching/iching.c |
| test_chinese | 18 | src/systems/chinese/chinese.c |
| test_human_design | 20 | src/systems/human_design/human_design.c |
| test_numerology | 28 | src/systems/numerology/numerology.c |
| test_time_hud | 18 | src/ui/time_hud.c |
| test_fmt | 28 | src/ui/fmt.c |

## Algorithms Implemented

| Algorithm | Source | File | Tests |
|-----------|--------|------|-------|
| Julian Day (Gregorian) | Meeus Ch.7 | julian.c | 10 |
| Julian Day (Julian cal) | Meeus Ch.7 | julian.c | (shared) |
| JD round-trip inverse | Meeus Ch.7 | julian.c | (shared) |
| Kepler's equation (Newton-Raphson) | Meeus Ch.30 | kepler.c | 8 |
| Eccentric to true anomaly | Meeus Ch.30 | kepler.c | 3 |
| GMST at 0h UT (IAU 1982) | Meeus Ch.12 | sidereal.c | 3 |
| GAST (simplified nutation) | Meeus Ch.12+22 | sidereal.c | 3 |
| Local Sidereal Time | Meeus Ch.12 | sidereal.c | 4 |
| Mean obliquity of ecliptic | Meeus Ch.22 | ecliptic.c | 3 |
| Ecliptic-to-equatorial transform | Meeus Ch.13 | ecliptic.c | 8 |
| Heliocentric orbital position | Meeus Ch.31 | orbit.c | 28 |
| Solar system snapshot | JPL + Kepler | planets.c | 9 |
| Lunar phase calculation | Meeus Ch.49 | lunar.c | 21 |
| Precession of equinoxes | Meeus Ch.21 | precession.c | 23 |
| Sunrise/sunset calculator | Meeus Ch.15 | solar_events.c | 23 |
| Gregorian date/time formatting | Meeus Ch.7 | gregorian.c | 16 |
| Zodiac sign from longitude | — | zodiac.c | 17 |
| Ptolemaic aspects (5 types) | — | aspects.c | 26 |
| Ascendant (rising degree) | Standard formula | observer.c | 11 |
| Whole Sign houses | — | houses.c | 11 |
| Planetary dignity | Traditional | dignity.c | 26 |
| Planetary hours (Chaldean) | Traditional | planetary_hours.c | 21 |
| Tzolkin Dreamspell calculation | Arguelles | tzolkin.c | 14 |
| I Ching hexagram derivation | King Wen sequence | iching.c | 18 |
| Chinese sexagenary cycle | Traditional | chinese.c | 18 |
| Human Design gate mapping | 64-gate ecliptic wheel | human_design.c | 20 |
| Numerology (digit reduction) | Pythagorean | numerology.c | 28 |
| Easing functions (20 types) | Robert Penner | easing.c | 25 |
| Circle/arc/ring geometry | — | arc_geometry.c | 23 |
| Font atlas layout (glyph UV mapping) | — | font_atlas.c | 22 |
| String formatting utilities (14 formatters) | — | fmt.c | 28 |
| Multi-scale camera (7 levels, log zoom) | — | camera_scale.c | 31 |
| Scale-dependent render layers (8 layers) | — | render_layers.c | 19 |
| Time HUD string formatting | — | time_hud.c | 18 |
| vec3 operations (8 ops) | Lengyel | vec3.c | 12 |
| mat4 operations (9 ops) | Lengyel | mat4.c | 10 |
| Orbital camera (spherical, log zoom) | — | camera.c | 9 |
| UV sphere mesh generation | — | mesh.c | (visual) |

## Roadmap

| Phase | Name | Status |
|-------|------|--------|
| 0 | Foundation Setup | COMPLETE |
| 1 | Hello WebGL Triangle | COMPLETE |
| 2 | Math Foundations | COMPLETE |
| 3 | First Visual Layer | COMPLETE |
| 4 | Time Display | COMPLETE (6/6) |
| 5 | Astrology Layer | IN PROGRESS (4/6) |
| 6 | Knowledge Systems | IN PROGRESS (6/7) |
| 7 | Visual Integration | not started |
| 8 | Deployment | not started |

**Active Phases**: 5 + 6 advancing in parallel
**Phases complete**: 5/9

## Purity

| Metric | Value |
|--------|-------|
| Pure functions | 231 |
| Stateful modules | 5 |
| Mutable globals | 5 (g_state + 4 static helpers) |
| GL contamination in pure code | 0 |
| malloc in math | 0 |

## Build

| Target | Status |
|--------|--------|
| `make native` | PASS |
| `make wasm` | PASS (43 KB) |
| `make test` | PASS (594/594) |
| `make serve` | PASS |
| TODOs in code | 0 |

## Research

| Metric | Count |
|--------|-------|
| Research cards | 27 |
| Cards done | 13 |
| Cards pending | 14 |
| Books acquired | 36 |
| Books pending | 1 |

## File Tree (source only)

```
src/
  core/main.c+h  app_state.c+h
  math/vec3.c+h  mat4.c+h  julian.c+h  kepler.c+h  sidereal.c+h  ecliptic.c+h
       easing.c+h  arc_geometry.c+h
  render/gl_init.c+h  shader.c+h  camera.c+h  camera_scale.c+h  render_layers.c+h
         mesh.c+h  font_atlas.c+h
  systems/astronomy/orbit.c+h  planets.c+h  lunar.c+h  precession.c+h  solar_events.c+h
  systems/gregorian/gregorian.c+h
  systems/astrology/zodiac.c+h  aspects.c+h  observer.c+h  houses.c+h  dignity.c+h
                    planetary_hours.c+h
  systems/tzolkin/tzolkin.c+h
  systems/iching/iching.c+h
  systems/chinese/chinese.c+h
  systems/human_design/human_design.c+h
  systems/numerology/numerology.c+h
  ui/time_hud.c+h  fmt.c+h
tests/
  math/test_vec3  test_mat4  test_julian  test_kepler  test_sidereal  test_ecliptic
       test_easing  test_arc_geometry
  render/test_camera  test_camera_scale  test_render_layers  test_font_atlas
  systems/test_orbit  test_planets  test_lunar  test_precession  test_solar_events
          test_gregorian  test_zodiac  test_aspects  test_observer  test_houses
          test_dignity  test_planetary_hours  test_cross_validation
          test_tzolkin  test_iching  test_chinese  test_human_design  test_numerology
  ui/test_time_hud  test_fmt
shaders/
  basic.vert  basic.frag
```
