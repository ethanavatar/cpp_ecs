CXX=clang
CXX_FLAGS=-Wall -Wextra -Wpedantic -O2 -g -std=c++20
OUT_DIR=bin

LINK_FLAGS=-static -m64 -lm -lpthread -lstdc++
WINDOWS_LIBS=

CXX_FILES=main.cc\
	world.cc component_storage.cc component_handle.cc\
	scanner.cc parser.cc

.PHONY: all clean run

all:
	mkdir -p $(OUT_DIR)
	$(CXX) $(CXX_FLAGS) -o $(OUT_DIR)/a $(CXX_FILES) -I. $(LINK_FLAGS) $(WINDOWS_LIBS)

run: all
	$(OUT_DIR)/a

clean:
	rm -rf $(OUT_DIR)
