## Add to WASM_SRC:
src/render/tarot_visual.c

## New test target:
test-tarot-visual: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_tarot_visual.c src/render/tarot_visual.c src/render/color_theory.c src/math/color.c src/systems/kabbalah/tree_geometry.c src/systems/kabbalah/sefirot.c $(UNITY) -o $(BUILD_DIR)/test_tarot_visual $(LDFLAGS)
	./$(BUILD_DIR)/test_tarot_visual

## Add to test: dependency list:
test-tarot-visual

## Add to .PHONY:
test-tarot-visual
