CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -Wshadow -fsanitize=address -fsanitize=undefined
DEPS = Dungeon.cpp Enemy.cpp Fountain.cpp GameCharacter.cpp Generator.cpp Item.cpp Main.cpp NPC.cpp Object.cpp Player.cpp RandomUtil.cpp Record.cpp Room.cpp
OBJ = Dungeon.o Enemy.o Fountain.o GameCharacter.o Generator.o Item.o Main.o NPC.o Object.o Player.o RandomUtil.o Record.o Room.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

game: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)
