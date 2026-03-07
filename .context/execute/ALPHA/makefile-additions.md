## Add to WASM_SRC:
src/render/star_catalog_ext.c

## New test target:
test-star-catalog-ext: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_star_catalog_ext.c src/render/star_catalog_ext.c $(UNITY) -o $(BUILD_DIR)/test_star_catalog_ext $(LDFLAGS)
	./$(BUILD_DIR)/test_star_catalog_ext

## Add to test: dependency list:
test-star-catalog-ext

## Add to .PHONY:
test-star-catalog-ext
