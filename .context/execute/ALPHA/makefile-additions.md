## Add to WASM_SRC:
src/render/decan_stars.c

## New test target:
test-decan-stars: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_decan_stars.c src/render/decan_stars.c $(UNITY) -o $(BUILD_DIR)/test_decan_stars $(LDFLAGS)
	./$(BUILD_DIR)/test_decan_stars

## Add to test: dependency list:
test-decan-stars

## Add to .PHONY:
test-decan-stars
