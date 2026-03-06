CC = gcc
EMCC = emcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic
LDFLAGS = -lm

CORE_SRC = src/core/main.c src/core/app_state.c
RENDER_SRC = src/render/gl_init.c src/render/shader.c src/render/camera.c src/render/camera_scale.c src/render/render_layers.c src/render/mesh.c src/render/color_palette.c src/render/ring_geometry.c src/render/ring_data.c src/render/aspect_lines.c src/render/cusp_lines.c src/render/galaxy_geometry.c src/render/billboard.c
MATH_SRC = src/math/vec3.c src/math/mat4.c src/math/julian.c src/math/kepler.c src/math/sidereal.c src/math/ecliptic.c src/math/easing.c src/math/arc_geometry.c src/math/color.c src/math/bezier.c src/math/sacred_geometry.c src/math/wheel_layout.c src/math/projection.c
SYSTEMS_SRC = src/systems/astronomy/orbit.c src/systems/astronomy/planets.c \
              src/systems/astronomy/lunar.c \
              src/systems/astronomy/precession.c \
              src/systems/astronomy/solar_events.c \
              src/systems/astronomy/cosmic_time.c \
              src/systems/gregorian/gregorian.c src/systems/astrology/zodiac.c \
              src/systems/astrology/aspects.c \
              src/systems/astrology/observer.c \
              src/systems/astrology/houses.c \
              src/systems/astrology/dignity.c \
              src/systems/astrology/planetary_hours.c \
              src/systems/tzolkin/tzolkin.c \
              src/systems/iching/iching.c \
              src/systems/chinese/chinese.c \
              src/systems/numerology/numerology.c \
              src/systems/human_design/human_design.c \
              src/systems/astronomy/moon_nodes.c \
              src/systems/astronomy/retrograde.c \
              src/core/date_parse.c \
              src/ui/time_hud.c src/ui/fmt.c src/ui/animation.c src/ui/glyph_layout.c src/ui/card_data.c src/ui/zodiac_glyphs.c \
              src/ui/hexagram_visual.c src/ui/location_presets.c src/ui/scale_hud.c src/ui/system_scale_map.c src/ui/time_format.c src/ui/card_layout.c \
              src/render/font_atlas.c
WASM_SRC = $(CORE_SRC) $(RENDER_SRC) $(MATH_SRC) $(SYSTEMS_SRC)
UNITY = tests/unity/unity.c
BUILD_DIR = build

# Native build (for local testing — no GL, core + math + camera)
native: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CORE_SRC) $(MATH_SRC) src/render/camera.c -o $(BUILD_DIR)/time $(LDFLAGS)

# WebAssembly build (full render pipeline)
wasm: $(BUILD_DIR)
	$(EMCC) $(WASM_SRC) -o $(BUILD_DIR)/index.html \
		-s USE_WEBGL2=1 \
		-s FULL_ES3=1 \
		-s WASM=1 \
		-Os \
		--shell-file web/index.html
	cp web/style.css $(BUILD_DIR)/style.css

# Run all tests
test: test-vec3 test-mat4 test-julian test-kepler test-sidereal test-camera test-camera-scale test-render-layers test-orbit test-ecliptic test-gregorian test-zodiac test-tzolkin test-planets test-planet-data test-time-hud test-aspects test-chinese test-observer test-houses test-easing test-font-atlas test-cross-validation test-dignity test-lunar test-iching test-arc-geometry test-fmt test-human-design test-planetary-hours test-precession test-solar-events test-numerology test-color test-bezier test-sacred-geometry test-animation test-glyph-layout test-wheel-layout test-color-palette test-card-data test-cosmic-time test-ring-geometry test-aspect-lines test-moon-nodes test-zodiac-glyphs test-projection test-cusp-lines test-galaxy-geometry test-billboard test-date-parse test-ring-data test-astro-wheel-layout test-retrograde test-card-layout test-hexagram-visual test-location-presets test-scale-hud test-system-scale-map test-time-format

# Individual test targets
test-vec3: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/math/test_vec3.c src/math/vec3.c $(UNITY) -o $(BUILD_DIR)/test_vec3 $(LDFLAGS)
	./$(BUILD_DIR)/test_vec3

test-mat4: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/math/test_mat4.c src/math/mat4.c src/math/vec3.c $(UNITY) -o $(BUILD_DIR)/test_mat4 $(LDFLAGS)
	./$(BUILD_DIR)/test_mat4

test-julian: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/math/test_julian.c src/math/julian.c $(UNITY) -o $(BUILD_DIR)/test_julian $(LDFLAGS)
	./$(BUILD_DIR)/test_julian

test-kepler: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/math/test_kepler.c src/math/kepler.c $(UNITY) -o $(BUILD_DIR)/test_kepler $(LDFLAGS)
	./$(BUILD_DIR)/test_kepler

test-sidereal: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/math/test_sidereal.c src/math/sidereal.c $(UNITY) -o $(BUILD_DIR)/test_sidereal $(LDFLAGS)
	./$(BUILD_DIR)/test_sidereal

test-camera: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_camera.c src/render/camera.c src/math/vec3.c src/math/mat4.c $(UNITY) -o $(BUILD_DIR)/test_camera $(LDFLAGS)
	./$(BUILD_DIR)/test_camera

test-orbit: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_orbit.c src/systems/astronomy/orbit.c src/math/kepler.c $(UNITY) -o $(BUILD_DIR)/test_orbit $(LDFLAGS)
	./$(BUILD_DIR)/test_orbit

test-ecliptic: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/math/test_ecliptic.c src/math/ecliptic.c $(UNITY) -o $(BUILD_DIR)/test_ecliptic $(LDFLAGS)
	./$(BUILD_DIR)/test_ecliptic

test-gregorian: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_gregorian.c src/systems/gregorian/gregorian.c src/math/julian.c $(UNITY) -o $(BUILD_DIR)/test_gregorian $(LDFLAGS)
	./$(BUILD_DIR)/test_gregorian

test-zodiac: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_zodiac.c src/systems/astrology/zodiac.c $(UNITY) -o $(BUILD_DIR)/test_zodiac $(LDFLAGS)
	./$(BUILD_DIR)/test_zodiac

test-tzolkin: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_tzolkin.c src/systems/tzolkin/tzolkin.c src/math/julian.c $(UNITY) -o $(BUILD_DIR)/test_tzolkin $(LDFLAGS)
	./$(BUILD_DIR)/test_tzolkin

test-planets: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_planets.c src/systems/astronomy/planets.c src/systems/astronomy/orbit.c src/math/kepler.c $(UNITY) -o $(BUILD_DIR)/test_planets $(LDFLAGS)
	./$(BUILD_DIR)/test_planets

test-planet-data: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_planet_data.c src/systems/astronomy/planet_data.c $(UNITY) -o $(BUILD_DIR)/test_planet_data $(LDFLAGS)
	./$(BUILD_DIR)/test_planet_data

test-time-hud: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/ui/test_time_hud.c src/ui/time_hud.c src/systems/gregorian/gregorian.c src/math/julian.c src/math/sidereal.c src/systems/tzolkin/tzolkin.c src/systems/astrology/zodiac.c src/systems/astronomy/planets.c src/systems/astronomy/orbit.c src/math/kepler.c $(UNITY) -o $(BUILD_DIR)/test_time_hud $(LDFLAGS)
	./$(BUILD_DIR)/test_time_hud

test-aspects: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_aspects.c src/systems/astrology/aspects.c $(UNITY) -o $(BUILD_DIR)/test_aspects $(LDFLAGS)
	./$(BUILD_DIR)/test_aspects

test-chinese: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_chinese.c src/systems/chinese/chinese.c src/math/julian.c $(UNITY) -o $(BUILD_DIR)/test_chinese $(LDFLAGS)
	./$(BUILD_DIR)/test_chinese

test-observer: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_observer.c src/systems/astrology/observer.c $(UNITY) -o $(BUILD_DIR)/test_observer $(LDFLAGS)
	./$(BUILD_DIR)/test_observer

test-houses: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_houses.c src/systems/astrology/houses.c src/systems/astrology/zodiac.c $(UNITY) -o $(BUILD_DIR)/test_houses $(LDFLAGS)
	./$(BUILD_DIR)/test_houses

test-camera-scale: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_camera_scale.c src/render/camera_scale.c src/math/easing.c $(UNITY) -o $(BUILD_DIR)/test_camera_scale $(LDFLAGS)
	./$(BUILD_DIR)/test_camera_scale

test-render-layers: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_render_layers.c src/render/render_layers.c src/render/camera_scale.c src/math/easing.c $(UNITY) -o $(BUILD_DIR)/test_render_layers $(LDFLAGS)
	./$(BUILD_DIR)/test_render_layers

test-easing: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/math/test_easing.c src/math/easing.c $(UNITY) -o $(BUILD_DIR)/test_easing $(LDFLAGS)
	./$(BUILD_DIR)/test_easing

test-font-atlas: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_font_atlas.c src/render/font_atlas.c $(UNITY) -o $(BUILD_DIR)/test_font_atlas $(LDFLAGS)
	./$(BUILD_DIR)/test_font_atlas

test-cross-validation: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_cross_validation.c src/math/julian.c src/math/sidereal.c src/math/ecliptic.c src/math/kepler.c src/systems/astronomy/orbit.c src/systems/astronomy/planets.c src/systems/gregorian/gregorian.c src/systems/astrology/zodiac.c src/systems/astrology/aspects.c src/systems/astrology/observer.c src/systems/astrology/houses.c src/systems/tzolkin/tzolkin.c src/systems/chinese/chinese.c $(UNITY) -o $(BUILD_DIR)/test_cross_validation $(LDFLAGS)
	./$(BUILD_DIR)/test_cross_validation

test-dignity: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_dignity.c src/systems/astrology/dignity.c $(UNITY) -o $(BUILD_DIR)/test_dignity $(LDFLAGS)
	./$(BUILD_DIR)/test_dignity

test-lunar: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_lunar.c src/systems/astronomy/lunar.c src/math/julian.c $(UNITY) -o $(BUILD_DIR)/test_lunar $(LDFLAGS)
	./$(BUILD_DIR)/test_lunar

test-iching: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_iching.c src/systems/iching/iching.c $(UNITY) -o $(BUILD_DIR)/test_iching $(LDFLAGS)
	./$(BUILD_DIR)/test_iching

test-arc-geometry: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/math/test_arc_geometry.c src/math/arc_geometry.c $(UNITY) -o $(BUILD_DIR)/test_arc_geometry $(LDFLAGS)
	./$(BUILD_DIR)/test_arc_geometry

test-fmt: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/ui/test_fmt.c src/ui/fmt.c $(UNITY) -o $(BUILD_DIR)/test_fmt $(LDFLAGS)
	./$(BUILD_DIR)/test_fmt

test-human-design: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_human_design.c src/systems/human_design/human_design.c $(UNITY) -o $(BUILD_DIR)/test_human_design $(LDFLAGS)
	./$(BUILD_DIR)/test_human_design

test-planetary-hours: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_planetary_hours.c src/systems/astrology/planetary_hours.c $(UNITY) -o $(BUILD_DIR)/test_planetary_hours $(LDFLAGS)
	./$(BUILD_DIR)/test_planetary_hours

test-precession: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_precession.c src/systems/astronomy/precession.c $(UNITY) -o $(BUILD_DIR)/test_precession $(LDFLAGS)
	./$(BUILD_DIR)/test_precession

test-solar-events: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_solar_events.c src/systems/astronomy/solar_events.c $(UNITY) -o $(BUILD_DIR)/test_solar_events $(LDFLAGS)
	./$(BUILD_DIR)/test_solar_events

test-numerology: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_numerology.c src/systems/numerology/numerology.c $(UNITY) -o $(BUILD_DIR)/test_numerology $(LDFLAGS)
	./$(BUILD_DIR)/test_numerology

test-color: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/math/test_color.c src/math/color.c $(UNITY) -o $(BUILD_DIR)/test_color $(LDFLAGS)
	./$(BUILD_DIR)/test_color

test-bezier: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/math/test_bezier.c src/math/bezier.c src/math/vec3.c $(UNITY) -o $(BUILD_DIR)/test_bezier $(LDFLAGS)
	./$(BUILD_DIR)/test_bezier

test-sacred-geometry: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/math/test_sacred_geometry.c src/math/sacred_geometry.c src/math/vec3.c $(UNITY) -o $(BUILD_DIR)/test_sacred_geometry $(LDFLAGS)
	./$(BUILD_DIR)/test_sacred_geometry

test-animation: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/ui/test_animation.c src/ui/animation.c src/math/easing.c $(UNITY) -o $(BUILD_DIR)/test_animation $(LDFLAGS)
	./$(BUILD_DIR)/test_animation

test-glyph-layout: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/ui/test_glyph_layout.c src/ui/glyph_layout.c $(UNITY) -o $(BUILD_DIR)/test_glyph_layout $(LDFLAGS)
	./$(BUILD_DIR)/test_glyph_layout

test-wheel-layout: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/math/test_wheel_layout.c src/math/wheel_layout.c $(UNITY) -o $(BUILD_DIR)/test_wheel_layout $(LDFLAGS)
	./$(BUILD_DIR)/test_wheel_layout

test-color-palette: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_color_palette.c src/render/color_palette.c src/math/color.c $(UNITY) -o $(BUILD_DIR)/test_color_palette $(LDFLAGS)
	./$(BUILD_DIR)/test_color_palette

test-card-data: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/ui/test_card_data.c src/ui/card_data.c $(UNITY) -o $(BUILD_DIR)/test_card_data $(LDFLAGS)
	./$(BUILD_DIR)/test_card_data

test-cosmic-time: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_cosmic_time.c src/systems/astronomy/cosmic_time.c $(UNITY) -o $(BUILD_DIR)/test_cosmic_time $(LDFLAGS)
	./$(BUILD_DIR)/test_cosmic_time

test-ring-geometry: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_ring_geometry.c src/render/ring_geometry.c $(UNITY) -o $(BUILD_DIR)/test_ring_geometry $(LDFLAGS)
	./$(BUILD_DIR)/test_ring_geometry

test-aspect-lines: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_aspect_lines.c src/render/aspect_lines.c $(UNITY) -o $(BUILD_DIR)/test_aspect_lines $(LDFLAGS)
	./$(BUILD_DIR)/test_aspect_lines

test-moon-nodes: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_moon_nodes.c src/systems/astronomy/moon_nodes.c $(UNITY) -o $(BUILD_DIR)/test_moon_nodes $(LDFLAGS)
	./$(BUILD_DIR)/test_moon_nodes

test-zodiac-glyphs: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/ui/test_zodiac_glyphs.c src/ui/zodiac_glyphs.c $(UNITY) -o $(BUILD_DIR)/test_zodiac_glyphs $(LDFLAGS)
	./$(BUILD_DIR)/test_zodiac_glyphs

test-projection: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/math/test_projection.c src/math/projection.c src/math/mat4.c src/math/vec3.c $(UNITY) -o $(BUILD_DIR)/test_projection $(LDFLAGS)
	./$(BUILD_DIR)/test_projection

test-cusp-lines: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_cusp_lines.c src/render/cusp_lines.c $(UNITY) -o $(BUILD_DIR)/test_cusp_lines $(LDFLAGS)
	./$(BUILD_DIR)/test_cusp_lines

test-galaxy-geometry: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_galaxy_geometry.c src/render/galaxy_geometry.c $(UNITY) -o $(BUILD_DIR)/test_galaxy_geometry $(LDFLAGS)
	./$(BUILD_DIR)/test_galaxy_geometry

test-billboard: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_billboard.c src/render/billboard.c src/math/mat4.c src/math/vec3.c $(UNITY) -o $(BUILD_DIR)/test_billboard $(LDFLAGS)
	./$(BUILD_DIR)/test_billboard

test-date-parse: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/core/test_date_parse.c src/core/date_parse.c src/math/julian.c $(UNITY) -o $(BUILD_DIR)/test_date_parse $(LDFLAGS)
	./$(BUILD_DIR)/test_date_parse

test-ring-data: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_ring_data.c src/render/ring_data.c src/render/ring_geometry.c $(UNITY) -o $(BUILD_DIR)/test_ring_data $(LDFLAGS)
	./$(BUILD_DIR)/test_ring_data

test-astro-wheel-layout: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_wheel_layout.c src/systems/astrology/wheel_layout.c src/math/vec3.c $(UNITY) -o $(BUILD_DIR)/test_astro_wheel_layout $(LDFLAGS)
	./$(BUILD_DIR)/test_astro_wheel_layout

test-retrograde: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/test_retrograde.c src/systems/astronomy/retrograde.c src/systems/astronomy/planets.c src/systems/astronomy/orbit.c src/math/kepler.c $(UNITY) -o $(BUILD_DIR)/test_retrograde $(LDFLAGS)
	./$(BUILD_DIR)/test_retrograde

test-card-layout: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/ui/test_card_layout.c src/ui/card_layout.c $(UNITY) -o $(BUILD_DIR)/test_card_layout $(LDFLAGS)
	./$(BUILD_DIR)/test_card_layout

test-hexagram-visual: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/ui/test_hexagram_visual.c src/ui/hexagram_visual.c src/systems/iching/iching.c $(UNITY) -o $(BUILD_DIR)/test_hexagram_visual $(LDFLAGS)
	./$(BUILD_DIR)/test_hexagram_visual

test-location-presets: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/ui/test_location_presets.c src/ui/location_presets.c $(UNITY) -o $(BUILD_DIR)/test_location_presets $(LDFLAGS)
	./$(BUILD_DIR)/test_location_presets

test-scale-hud: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/ui/test_scale_hud.c src/ui/scale_hud.c $(UNITY) -o $(BUILD_DIR)/test_scale_hud $(LDFLAGS)
	./$(BUILD_DIR)/test_scale_hud

test-system-scale-map: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/ui/test_system_scale_map.c src/ui/system_scale_map.c $(UNITY) -o $(BUILD_DIR)/test_system_scale_map $(LDFLAGS)
	./$(BUILD_DIR)/test_system_scale_map

test-time-format: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/ui/test_time_format.c src/ui/time_format.c $(UNITY) -o $(BUILD_DIR)/test_time_format $(LDFLAGS)
	./$(BUILD_DIR)/test_time_format

# Build WASM + kill stale server + serve (one command)
dev: wasm
	@kill $$(lsof -t -i:8080) 2>/dev/null || true
	@echo "Serving on http://localhost:8080"
	@cd $(BUILD_DIR) && python3 -m http.server 8080

# Start local server
serve: $(BUILD_DIR)
	cd $(BUILD_DIR) && python3 -m http.server 8080

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)/*

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: native wasm test test-vec3 test-mat4 test-julian test-kepler test-sidereal test-camera test-camera-scale test-render-layers test-orbit test-ecliptic test-gregorian test-zodiac test-tzolkin test-planets test-planet-data test-time-hud test-aspects test-chinese test-observer test-houses test-easing test-font-atlas test-cross-validation test-dignity test-lunar test-iching test-arc-geometry test-fmt test-human-design test-planetary-hours test-precession test-solar-events test-numerology test-color test-bezier test-sacred-geometry test-animation test-glyph-layout test-wheel-layout test-color-palette test-card-data test-cosmic-time test-ring-geometry test-aspect-lines test-moon-nodes test-zodiac-glyphs test-projection test-cusp-lines test-galaxy-geometry test-billboard test-date-parse test-ring-data test-astro-wheel-layout test-retrograde test-card-layout test-hexagram-visual test-location-presets test-scale-hud test-system-scale-map test-time-format dev serve clean
