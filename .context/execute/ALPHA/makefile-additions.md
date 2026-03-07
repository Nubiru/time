## Add to WASM_SRC:
src/render/zodiac_pack.c

## New test target:
test-zodiac-pack: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_zodiac_pack.c src/render/zodiac_pack.c src/render/ring_geometry.c src/math/arc_geometry.c src/render/cusp_lines.c src/render/aspect_lines.c src/render/glyph_batch.c src/render/billboard.c src/render/color_palette.c src/render/color_theory.c src/math/color.c src/math/vec3.c src/math/mat4.c $(UNITY) -o $(BUILD_DIR)/test_zodiac_pack $(LDFLAGS)
	./$(BUILD_DIR)/test_zodiac_pack

## Add to test: dependency list:
test-zodiac-pack

## Add to .PHONY:
test-zodiac-pack
