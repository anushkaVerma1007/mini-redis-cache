# Mini-Redis Cache Makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2
SRCDIR = src
INCDIR = include
TESTDIR = tests
OBJDIR = obj

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Test files
TEST_SOURCES = $(wildcard $(TESTDIR)/*.cpp)
TEST_EXECUTABLES = $(TEST_SOURCES:$(TESTDIR)/%.cpp=%)

# Main executable
TARGET = mini-redis

# Default target
all: $(TARGET)

# Create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Link main executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile and run tests
test: test_cache test_lru
	./test_cache
	./test_lru

test_cache: $(TESTDIR)/test_cache.cpp $(filter-out $(OBJDIR)/main.o, $(OBJECTS))
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $^ -o $@

test_lru: $(TESTDIR)/test_lru.cpp $(filter-out $(OBJDIR)/main.o, $(OBJECTS))
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $^ -o $@

# Run the main program
run: $(TARGET)
	./$(TARGET)

# perf: $(TARGET)
# 	@echo "Running performance test..."
# 	@(echo "FLUSH"; \
# 	  echo "SET perf:test performance_value"; \
# 	  for i in {1..1000}; do echo "SET key$$i value$$i"; done; \
# 	  echo "STATS"; \
# 	  echo "QUIT") | ./$(TARGET)

#Performance test
perf: $(TARGET)
	@echo "Running performance test..."
	@bash -c '(echo "FLUSH"; \
	  echo "SET perf:test performance_value"; \
	  for i in {1..1000}; do echo "SET key$$i value$$i"; done; \
	  echo "STATS"; \
	  echo "QUIT") | ./$(TARGET)'

# Clean build files
clean:
	rm -rf $(OBJDIR) $(TARGET) $(TEST_EXECUTABLES)

# Install (copy to /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

# Debug build
debug: CXXFLAGS += -DDEBUG -g3
debug: $(TARGET)

# Release build
release: CXXFLAGS += -O3 -DNDEBUG
release: clean $(TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  all      - Build the main executable"
	@echo "  test     - Build and run all tests"
	@echo "  run      - Build and run the main program"
	@echo "  perf     - Run performance test"
	@echo "  clean    - Remove build files"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build optimized release version"
	@echo "  help     - Show this help"

.PHONY: all test run perf clean install uninstall debug release help
