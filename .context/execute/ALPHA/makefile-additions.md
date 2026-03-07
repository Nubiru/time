## Add to WASM_SRC:
src/render/megalithic.c

## New test target:
test-megalithic: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_megalithic.c src/render/megalithic.c src/systems/astronomy/solar_events.c $(UNITY) -o $(BUILD_DIR)/test_megalithic $(LDFLAGS)
	./$(BUILD_DIR)/test_megalithic

## Add to test: dependency list:
test-megalithic

## Add to .PHONY:
test-megalithic
