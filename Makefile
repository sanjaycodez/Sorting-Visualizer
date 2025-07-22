# Makefile for macOS + Raylib

# APP = SortingVisualizer
# SRC = SortingVisualizer.cpp

APP = SortingVis
SRC = SortingVis.cpp

CXX = g++
CXXFLAGS = -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

all: $(APP)

$(APP): $(SRC)
	$(CXX) $(SRC) -o $(APP) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(APP)
