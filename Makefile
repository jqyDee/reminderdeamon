CFLAGS = -g
CFLAGS += -D DEBUG

LDFLAGS =

EXECUTABLE_NAME = reminderd
TEST_NAME = reminderd_tests

SRC = src
BIN = bin
OBJ = $(BIN)/obj

SOURCE_FILES = reminder.c storagefile.c times.c linked_list.c
TEST_FILES = unittests.c linked_list.c

# EXECUTABLE
EXECUTABLE_FILES = $(EXECUTABLE_NAME:%=$(BIN)/%)
OBJECT_FILES = $(SOURCE_FILES:%.c=$(OBJ)/%.o)

build: $(EXECUTABLE_FILES)

run: build
	@$(BIN)/$(EXECUTABLE_NAME)

$(OBJECT_FILES): $(OBJ)/%.o: $(SRC)/%.c
	@echo "-->  compiling $<"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ -MMD -c $<
	@echo "---> $< compiled"

$(EXECUTABLE_FILES): $(OBJECT_FILES)
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "===> Build successfull!"

# TESTS
TEST_EXECUTABLE_FILES = $(TEST_NAME:%=$(BIN)/%)
TEST_OBJECT_FILES = $(TEST_FILES:%.c=$(OBJ)/%.o)

run_tests: build_tests
	@$(BIN)/$(TEST_NAME)

build_tests: $(TEST_EXECUTABLE_FILES)

$(TEST_OBJECT_FILES): $(OBJ)/%.o: $(SRC)/%.c
	@echo "-->  compiling $<"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ -MMD -c $<
	@echo "---> $< compiled"

$(TEST_EXECUTABLE_FILES): $(TEST_OBJECT_FILES)
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "===> Build (tests) successfull!"

.PHONY: clean

clean:
	rm -rf $(BIN)
