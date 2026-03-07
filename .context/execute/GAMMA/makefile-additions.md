## Add to WASM_SRC (SYSTEMS_SRC section):
src/systems/kabbalah/sefirot.c
src/systems/kabbalah/four_worlds.c
src/systems/buddhist/buddhist.c
src/systems/hindu/yuga.c
src/systems/unified/structural_map.c
src/systems/geology/climate_history.c
src/systems/geology/fossil_milestones.c
src/systems/earth/surf_spots.c
src/systems/earth/ski_resorts.c
src/systems/earth/storm_data.c
src/systems/earth/snow_season.c
src/systems/earth/biorhythm.c
src/systems/unified/codon_hexagram.c

## New test targets:
test-four-worlds: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/kabbalah/test_four_worlds.c src/systems/kabbalah/four_worlds.c $(UNITY) -o $(BUILD_DIR)/test_four_worlds $(LDFLAGS)
	./$(BUILD_DIR)/test_four_worlds

test-buddhist: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/buddhist/test_buddhist.c src/systems/buddhist/buddhist.c src/systems/astronomy/lunar.c $(UNITY) -o $(BUILD_DIR)/test_buddhist $(LDFLAGS)
	./$(BUILD_DIR)/test_buddhist

test-yuga: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/hindu/test_yuga.c src/systems/hindu/yuga.c $(UNITY) -o $(BUILD_DIR)/test_yuga $(LDFLAGS)
	./$(BUILD_DIR)/test_yuga

test-structural-map: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/unified/test_structural_map.c src/systems/unified/structural_map.c $(UNITY) -o $(BUILD_DIR)/test_structural_map $(LDFLAGS)
	./$(BUILD_DIR)/test_structural_map

test-climate-history: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/geology/test_climate_history.c src/systems/geology/climate_history.c $(UNITY) -o $(BUILD_DIR)/test_climate_history $(LDFLAGS)
	./$(BUILD_DIR)/test_climate_history

test-fossil-milestones: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/geology/test_fossil_milestones.c src/systems/geology/fossil_milestones.c $(UNITY) -o $(BUILD_DIR)/test_fossil_milestones $(LDFLAGS)
	./$(BUILD_DIR)/test_fossil_milestones

test-surf-spots: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/earth/test_surf_spots.c src/systems/earth/surf_spots.c $(UNITY) -o $(BUILD_DIR)/test_surf_spots $(LDFLAGS)
	./$(BUILD_DIR)/test_surf_spots

test-ski-resorts: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/earth/test_ski_resorts.c src/systems/earth/ski_resorts.c $(UNITY) -o $(BUILD_DIR)/test_ski_resorts $(LDFLAGS)
	./$(BUILD_DIR)/test_ski_resorts

test-storm-data: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/earth/test_storm_data.c src/systems/earth/storm_data.c $(UNITY) -o $(BUILD_DIR)/test_storm_data $(LDFLAGS)
	./$(BUILD_DIR)/test_storm_data

test-snow-season: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/earth/test_snow_season.c src/systems/earth/snow_season.c $(UNITY) -o $(BUILD_DIR)/test_snow_season $(LDFLAGS)
	./$(BUILD_DIR)/test_snow_season

test-biorhythm: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/earth/test_biorhythm.c src/systems/earth/biorhythm.c $(UNITY) -o $(BUILD_DIR)/test_biorhythm $(LDFLAGS)
	./$(BUILD_DIR)/test_biorhythm

test-codon-hexagram: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/unified/test_codon_hexagram.c src/systems/unified/codon_hexagram.c $(UNITY) -o $(BUILD_DIR)/test_codon_hexagram $(LDFLAGS)
	./$(BUILD_DIR)/test_codon_hexagram

## Add to test: dependency list:
test-four-worlds test-buddhist test-yuga test-structural-map test-climate-history test-fossil-milestones test-surf-spots test-ski-resorts test-storm-data test-snow-season test-biorhythm test-codon-hexagram

## Add to .PHONY:
test-four-worlds test-buddhist test-yuga test-structural-map test-climate-history test-fossil-milestones test-surf-spots test-ski-resorts test-storm-data test-snow-season test-biorhythm test-codon-hexagram
