# Makefile for Heat Exchanger Project

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = heat_exchanger
SOURCES = main.cpp fluid_properties.cpp dimensionless_numbers.cpp \
          heat_transfer_correlations.cpp heat_exchanger_geometry.cpp \
          thermal_calculations.cpp numerical_solver.cpp
HEADERS = fluid_properties.h heat_exchanger_geometry.h dimensionless_numbers.h \
          heat_transfer_correlations.h thermal_calculations.h numerical_solver.h
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	@if exist *.o del *.o
	@if exist $(TARGET).exe del $(TARGET).exe
	@if exist temperature_profile.csv del temperature_profile.csv
	@if exist convergence_study.csv del convergence_study.csv
	@if exist heat_transfer_summary.txt del heat_transfer_summary.txt

# Run the program
run: $(TARGET)
	$(TARGET).exe

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Help target
help:
	@echo Available targets:
	@echo   all     - Build the heat exchanger program
	@echo   debug   - Build with debug information
	@echo   clean   - Remove build files and output
	@echo   run     - Build and run the program
	@echo   help    - Show this help message

.PHONY: all clean run debug help
