CC = gcc
SRC_DIR = CCDSALG_S22_MartinJavierEleydo-20250727T110158Z-1-001/CCDSALG_S22_MartinJavierEleydo/src
TEST_DIR = CCDSALG_S22_MartinJavierEleydo-20250727T110158Z-1-001/CCDSALG_S22_MartinJavierEleydo/test
INCLUDE_DIRS := $(shell find $(SRC_DIR) -type d)
CFLAGS = -Wall -Wextra -pedantic -std=c11 $(addprefix -I,$(INCLUDE_DIRS))
MAIN_SRC := $(shell find $(SRC_DIR) -name '*.c')
MAIN_BIN := main
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_BINS := $(patsubst %.c,%,$(TEST_SRCS))

.PHONY: all clean tests

all: $(MAIN_BIN)

$(MAIN_BIN): $(MAIN_SRC)
	$(CC) $(CFLAGS) $^ -o $@

tests: $(TEST_BINS)

$(TEST_BINS): %: %.c $(MAIN_SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(MAIN_BIN) $(TEST_BINS)
