## Add to WASM_SRC:
src/render/star_field.c

## New test target:
test-star-field: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_star_field.c src/render/star_field.c src/render/star_catalog.c src/render/star_catalog_ext.c src/render/star_colors.c src/render/constellation.c $(UNITY) -o $(BUILD_DIR)/test_star_field $(LDFLAGS)
	./$(BUILD_DIR)/test_star_field

## Add to test: dependency list:
test-star-field

## Add to .PHONY:
test-star-field
