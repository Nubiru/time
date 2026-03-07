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
src/systems/unified/achievement.c
src/systems/unified/calendar_reform.c
src/systems/unified/knowledge_graph.c
src/systems/unified/number_scanner.c
src/systems/earth/seasons.c
src/systems/unified/frequency.c
src/systems/unified/precession_detect.c
src/systems/unified/calendar_politics.c

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

test-achievement: $(BUILD_DIR)
	$(CC) $(CFLAGS) -Isrc tests/systems/unified/test_achievement.c src/systems/unified/achievement.c $(UNITY) -o $(BUILD_DIR)/test_achievement $(LDFLAGS)
	./$(BUILD_DIR)/test_achievement

test-calendar-reform: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/unified/test_calendar_reform.c src/systems/unified/calendar_reform.c $(UNITY) -o $(BUILD_DIR)/test_calendar_reform $(LDFLAGS)
	./$(BUILD_DIR)/test_calendar_reform

test-knowledge-graph: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/unified/test_knowledge_graph.c src/systems/unified/knowledge_graph.c $(UNITY) -o $(BUILD_DIR)/test_knowledge_graph $(LDFLAGS)
	./$(BUILD_DIR)/test_knowledge_graph

test-number-scanner: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/unified/test_number_scanner.c src/systems/unified/number_scanner.c $(UNITY) -o $(BUILD_DIR)/test_number_scanner $(LDFLAGS)
	./$(BUILD_DIR)/test_number_scanner

test-seasons: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/earth/test_seasons.c src/systems/earth/seasons.c $(UNITY) -o $(BUILD_DIR)/test_seasons $(LDFLAGS)
	./$(BUILD_DIR)/test_seasons

test-frequency: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/unified/test_frequency.c src/systems/unified/frequency.c $(UNITY) -o $(BUILD_DIR)/test_frequency $(LDFLAGS)
	./$(BUILD_DIR)/test_frequency

test-precession-detect: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/unified/test_precession_detect.c src/systems/unified/precession_detect.c $(UNITY) -o $(BUILD_DIR)/test_precession_detect $(LDFLAGS)
	./$(BUILD_DIR)/test_precession_detect

test-calendar-politics: $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/systems/unified/test_calendar_politics.c src/systems/unified/calendar_politics.c $(UNITY) -o $(BUILD_DIR)/test_calendar_politics $(LDFLAGS)
	./$(BUILD_DIR)/test_calendar_politics

## Add to test: dependency list:
test-four-worlds test-buddhist test-yuga test-structural-map test-climate-history test-fossil-milestones test-surf-spots test-ski-resorts test-storm-data test-snow-season test-biorhythm test-codon-hexagram test-achievement test-calendar-reform test-knowledge-graph test-number-scanner test-seasons test-frequency test-precession-detect test-calendar-politics

## Add to .PHONY:
test-four-worlds test-buddhist test-yuga test-structural-map test-climate-history test-fossil-milestones test-surf-spots test-ski-resorts test-storm-data test-snow-season test-biorhythm test-codon-hexagram test-achievement test-calendar-reform test-knowledge-graph test-number-scanner test-seasons test-frequency test-precession-detect test-calendar-politics
