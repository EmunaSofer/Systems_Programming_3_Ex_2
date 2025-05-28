##emuna002@gmail.com
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
LDFLAGS_GUI = -lsfml-graphics -lsfml-window -lsfml-system

SRC = main.cpp Game.cpp Player.cpp Governor.cpp Spy.cpp Baron.cpp General.cpp Judge.cpp Merchant.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = coup_game

TEST_SRC = test.cpp
TEST_OBJ = test.o
TEST_EXEC = test_runner

GUI_SRC = gui.cpp
GUI_EXEC = gui_demo

.PHONY: all clean Main test valgrind gui

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Main: $(EXEC)
	./$(EXEC)

test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): $(TEST_SRC) $(filter-out main.o,$(OBJ))
	$(CXX) $(CXXFLAGS) -o $@ $^

valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC)

gui: $(GUI_SRC) $(filter-out main.o,$(OBJ))
	$(CXX) $(CXXFLAGS) $(GUI_SRC) $(filter-out main.o,$(OBJ)) -o $(GUI_EXEC) $(LDFLAGS_GUI)

clean:
	rm -f $(OBJ) $(EXEC) $(TEST_OBJ) $(TEST_EXEC) $(GUI_EXEC)
