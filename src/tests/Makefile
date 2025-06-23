CXX := g++
CXXFLAGS := -I../model -Wall -Wextra -std=c++20 -g -DTESTING
LDLIBS := -lgtest -lgtest_main -lpthread

# LDLIBS += -fsanitize=address,undefined
# CXXFLAGS += -fsanitize=address,undefined

MODEL_DIR := ../model/maze
QLEARNING_DIR := ../model/q_learning

OBJ_DIR := build

TEST_SRCS := $(wildcard *.cc)
TEST_OBJS := $(patsubst %.cc,$(OBJ_DIR)/test_%.o,$(TEST_SRCS))

MODEL_SRCS := $(wildcard $(MODEL_DIR)/*.cc)
QLEARNING_SRCS := $(wildcard $(QLEARNING_DIR)/*.cc)

MODEL_OBJS := $(patsubst $(MODEL_DIR)/%.cc,$(OBJ_DIR)/maze_%.o,$(MODEL_SRCS))
QLEARNING_OBJS := $(patsubst $(QLEARNING_DIR)/%.cc,$(OBJ_DIR)/qlearning_%.o,$(QLEARNING_SRCS))

OBJS := $(MODEL_OBJS) $(QLEARNING_OBJS)

TARGET := test_exec

$(TARGET): $(OBJS) $(TEST_OBJS)
	$(CXX) -o $@ $^ $(LDLIBS)

$(OBJ_DIR)/maze_%.o: $(MODEL_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/qlearning_%.o: $(QLEARNING_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/test_%.o: %.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

.PHONY: all test coverage clean clean-coverage valgrind-run

all: clean $(TARGET)

test: clean $(TARGET)
	./$(TARGET)


COVERAGE_FLAGS := -fprofile-arcs -ftest-coverage
COVERAGE_INFO := coverage.info
COVERAGE_REPORT_DIR := ../report


coverage: CXXFLAGS += $(COVERAGE_FLAGS)
coverage: LDLIBS += -lgcov
coverage: clean test
	mkdir -p $(COVERAGE_REPORT_DIR)
	gcovr --root .. --filter ../model/ \
	--html-details $(COVERAGE_REPORT_DIR)/report.html --exclude-branches-by-pattern '.*'

clean: clean-coverage
	# rm -f $(OBJS) $(TARGET)
	rm -rf $(OBJ_DIR) $(TARGET)

clean-coverage:
	rm -rf $(COVERAGE_REPORT_DIR) *.gcov *gcno *gcda


leaks: all
	valgrind \
		--leak-check=full \
		--track-origins=yes \
		--show-leak-kinds=all \
		--trace-children=yes \
		-s \
		./$(TARGET)
