## Makefile Changes (MODIFIED existing targets, not new ones)

### Updated test-color-palette (add color_theory.c dependency):
```
test-color-palette: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_color_palette.c src/render/color_palette.c src/render/color_theory.c src/math/color.c $(UNITY) -o $(BUILD_DIR)/test_color_palette $(LDFLAGS)
	./$(BUILD_DIR)/test_color_palette
```

### Updated test-aspect-lines (add color_palette.c, color_theory.c, color.c dependencies):
```
test-aspect-lines: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_aspect_lines.c src/render/aspect_lines.c src/render/color_palette.c src/render/color_theory.c src/math/color.c $(UNITY) -o $(BUILD_DIR)/test_aspect_lines $(LDFLAGS)
	./$(BUILD_DIR)/test_aspect_lines
```

### Updated test-render-layers (add golden_layout.c dependency):
```
test-render-layers: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_render_layers.c src/render/render_layers.c src/render/camera_scale.c src/math/easing.c src/ui/golden_layout.c $(UNITY) -o $(BUILD_DIR)/test_render_layers $(LDFLAGS)
	./$(BUILD_DIR)/test_render_layers
```
