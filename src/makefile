CXX = g++
CXXFLAGS = -std=c++17 -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

all: arkanoid

arkanoid: src/main.cpp
	$(CXX) -o $@ $^ $(LDFLAGS) $(CXXFLAGS)

run : arkanoid
	./arkanoid

clean:
	rm -f $(OBJ) arkanoid
