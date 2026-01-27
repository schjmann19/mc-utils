CC ?= clang
CFLAGS ?= -std=c11 -Wall -Wextra -O2
LDFLAGS ?= -Wl,-Bstatic -L extractor/target/release -l mc_recipe_extractor -Wl,-Bdynamic -lbz2
BIN_DIR := bin
SRC := src/mc-utils.c

.PHONY: all extractor noembed mc-util clean

all: mc-util

# build extractor with cargo
extractor:
	@if [ -d extractor ]; then \
		cd extractor && cargo build --release; \
	else \
		echo "No extractor/ directory, skipping"; \
	fi

# build mc-util with static linked rust lib
mc-util: extractor $(BIN_DIR) $(SRC)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/mc-util $(SRC) $(LDFLAGS)

# build mc-util without embedded extractor
noembed: extractor $(BIN_DIR) $(SRC)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/mc-util $(SRC) $(LDFLAGS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(BIN_DIR)/mc-util
	(cd extractor && cargo clean) || true
