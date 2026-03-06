CC = gcc
EMCC = emcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic
LDFLAGS = -lm

SRC = src/core/main.c
TEST_SRC = tests/test_runner.c tests/unity/unity.c
BUILD_DIR = build

# Native build (for local testing)
native: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BUILD_DIR)/time $(LDFLAGS)

# WebAssembly build
wasm: $(BUILD_DIR)
	$(EMCC) $(SRC) -o $(BUILD_DIR)/index.html \
		-s USE_WEBGL2=1 \
		-s FULL_ES3=1 \
		-s WASM=1 \
		-Os \
		--shell-file web/index.html

# Run tests with Unity framework
test: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(BUILD_DIR)/test_runner $(LDFLAGS)
	./$(BUILD_DIR)/test_runner

# Start local server
serve: $(BUILD_DIR)
	cd $(BUILD_DIR) && python3 -m http.server 8080

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.wasm $(BUILD_DIR)/*.js $(BUILD_DIR)/time $(BUILD_DIR)/test_runner $(BUILD_DIR)/index.html

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: native wasm test serve clean
