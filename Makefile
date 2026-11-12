# ==========================================
# NEXORA - Build System
# ==========================================

.RECIPEPREFIX = >

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
INCLUDE = -I src

BUILD_DIR = build
CORE = src/core/directory.c
MAIN = src/main.c

APP = $(BUILD_DIR)/nexora

TEST_SOURCES = $(wildcard tests/test_*.c)
TEST_NAMES = $(patsubst tests/%.c,%,$(TEST_SOURCES))
TEST_BINS = $(addprefix $(BUILD_DIR)/,$(TEST_NAMES))

all: $(APP)

$(APP): $(MAIN) $(CORE)
> mkdir -p $(BUILD_DIR)
> $(CC) $(CFLAGS) $(INCLUDE) $(MAIN) $(CORE) -o $(APP)

tests: $(TEST_BINS)

$(BUILD_DIR)/test_%: tests/test_%.c $(CORE)
> mkdir -p $(BUILD_DIR)
> $(CC) $(CFLAGS) $(INCLUDE) $< $(CORE) -o $@

test: tests
> @for test in $(TEST_BINS); do \
>     echo ""; \
>     echo "========== $$test =========="; \
>     ./$$test || exit 1; \
> done

clean:
> rm -rf $(BUILD_DIR)

.PHONY: all tests test clean
