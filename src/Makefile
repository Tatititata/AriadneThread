CXX = g++
MOC = /usr/lib/qt6/libexec/moc

BUILD_DIR := build

CXXFLAGS := -std=c++20 -Wall -Wextra -Werror
CXXFLAGS += $(shell pkg-config --cflags Qt6Widgets Qt6OpenGLWidgets Qt6Gui Qt6Core Qt6Network)
LDFLAGS := $(shell pkg-config --libs Qt6Widgets Qt6OpenGLWidgets Qt6Gui Qt6Core Qt6Network) -lGL
CXXFLAGS += -O2 -DNDEBUG

MODEL_SRCS := $(wildcard model/maze/*.cc)
LEARNING_SRCS := $(wildcard model/q_learning/*.cc)
SERVER_SRCS := $(wildcard server/*.cc)
UI_SRCS := $(wildcard ui/*.cc)

SRC := $(MODEL_SRCS) $(SERVER_SRCS) $(UI_SRCS) $(LEARNING_SRCS)
OBJ := $(addprefix $(BUILD_DIR)/,$(SRC:.cc=.o))

MOC_HEADERS := \
    model/q_learning/q_learning.h \
    server/tcpserver.h \
    ui/main_window.h \
    ui/maze_widget.h \
    ui/cave_widget.h \
    ui/maze_draw.h \
    ui/path_draw.h \
    ui/cave_draw.h \
    ui/bonus_draw.h \
    ui/qlearning_dialog.h

MOC_OBJS := $(addprefix $(BUILD_DIR)/,$(MOC_HEADERS:.h=.moc.o))

.PHONY: all clean tests maze cl cppcheck_cpp gcov_report valgrind dvi pdf dist

all: maze srv

srv: $(filter $(BUILD_DIR)/server/%.o $(BUILD_DIR)/model/maze/%.o,$(OBJ)) $(filter $(BUILD_DIR)/server/%.moc.o,$(MOC_OBJS))
	$(CXX) $^ -o $@ $(LDFLAGS)

maze: $(filter $(BUILD_DIR)/ui/%.o $(BUILD_DIR)/model/maze/%.o $(BUILD_DIR)/model/q_learning/%.o,$(OBJ)) $(filter $(BUILD_DIR)/ui/%.moc.o $(BUILD_DIR)/model/q_learning/%.moc.o,$(MOC_OBJS))
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.moc.cc: %.h
	@mkdir -p $(dir $@)
	$(MOC) $< -o $@

$(BUILD_DIR)/%.moc.o: $(BUILD_DIR)/%.moc.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) maze srv
	$(MAKE) -C tests/ clean
	rm -rf docs
	rm -f maze_cpp_src.tar.gz

cl:
	cp ../materials/linters/.clang-format .clang-format
	find . -type f -name "*.cc" -o -name "*.h"  -o -name "*.js" | xargs clang-format -i
	rm -f .clang-format

cppcheck_cpp:
	cppcheck --enable=all --force --error-exitcode=1 --suppress=unusedFunction --std=c++20 --suppress=missingIncludeSystem --inconclusive --language=c++ --check-level=exhaustive --verbose \
	-Dslots= -Dsignals= -DQ_OBJECT= \
	../src

tests:
	$(MAKE) -C tests/ test

gcov_report:
	$(MAKE) -C tests/ coverage

valgrind:
	$(MAKE) -C tests/ leaks

dist: clean
	@echo "Creating source distribution archive from src/ ..."
	@rm -f maze_cpp_src.tar.gz
	@tar czf maze_cpp_src.tar.gz ../src
	@echo "Archive created at maze_cpp_src.tar.gz"

dvi:
	doxygen Doxyfile
	@xdg-open docs/html/index.html

pdf: dvi
	cd docs/latex && make
	@xdg-open docs/latex/refman.pdf

BINDIR := bin

install: all
	@echo "Installing to $(BINDIR)"
	mkdir -p $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)
	rm -rf ./$(TARGET)

uninstall:
	@echo "Removing from $(BINDIR)"
	rm -f $(BINDIR)/$(TARGET)
	rmdir --ignore-fail-on-non-empty $(BINDIR) || true