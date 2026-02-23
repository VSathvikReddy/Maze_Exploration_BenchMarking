SRC_FILES = src/player.cpp src/helper.cpp src/tile.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)

compile: render.o $(OBJ_FILES)
	g++ render.o $(OBJ_FILES) -lsfml-graphics -lsfml-window -lsfml-system

run:
	./a.out

render.o:render.cpp script.cpp Include/player.hpp Include/tile.hpp Include/helper.hpp Include/values.hpp
	g++ -c render.cpp -o render.o
src/player.o:src/player.cpp Include/player.hpp Include/values.hpp
	g++ -c src/player.cpp -o src/player.o
src/helper.o:src/helper.cpp Include/tile.hpp Include/player.hpp Include/helper.hpp Include/values.hpp
	g++ -c src/helper.cpp -o src/helper.o
src/tile.o:src/tile.cpp Include/tile.hpp Include/values.hpp  
	g++ -c src/tile.cpp -o src/tile.o

setup:
	sudo apt update
	sudo apt install libsfml-dev

image: utils/image_merger.o
	@read -p "This will modify textues? (y/n) " ans; \
	if [ "$$ans" != "y" ]; then \
		echo "Aborted."; \
		exit 1; \
	fi
	g++ utils/image_merger.o -lsfml-graphics -lsfml-window -lsfml-system
	./a.out
	rm a.out