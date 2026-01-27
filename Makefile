CC ?= clang
CFLAGS ?= -std=c11 -Wall -Wextra -O3
DEBUG ?= 0
ifeq ($(DEBUG),0)
	CFLAGS += -s
endif
LDFLAGS ?= -Wl,-Bstatic -L extractor/target/release -l mc_recipe_extractor -Wl,-Bdynamic -lbz2 -lssl -lcrypto -lm
BIN_DIR := bin
SRC := src/mc-utils.c

.PHONY: all extractor noembed mc-util clean debug

all: mc-util
debug: DEBUG=1
debug: mc-util

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
