## Add to WASM_SRC:
src/render/planet_pack.c

## New test target:
test-planet-pack: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/render/test_planet_pack.c src/render/planet_pack.c src/systems/astronomy/planets.c src/systems/astronomy/orbit.c src/math/kepler.c src/systems/astronomy/planet_data.c src/render/atmo_ring.c $(UNITY) -o $(BUILD_DIR)/test_planet_pack $(LDFLAGS)
	./$(BUILD_DIR)/test_planet_pack

## Add to test: dependency list:
test-planet-pack

## Add to .PHONY:
test-planet-pack
