.default: all

all: GameEngineIO.o GameEngineCallback.o GameEngine.o Player.o MosaicStorage.o Mosaic.o BrokenTiles.o Factory.o Tile.o Node.o LinkedList.o Input.o Menu.o main.o assign2

GameEngineIO.o: ./GameEngine/GameEngineIO.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c ./GameEngine/GameEngineIO.cpp 

GameEngineCallback.o: ./GameEngine/GameEngineCallback.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c ./GameEngine/GameEngineCallback.cpp	
 
GameEngine.o: ./GameEngine/GameEngine.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c ./GameEngine/GameEngine.cpp
 
Player.o: ./Player/Player.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c ./Player/Player.cpp
 
MosaicStorage.o: ./Player/MosaicStorage.cpp 
	g++ -Wall -Werror -std=c++14 -g -O -c ./Player/MosaicStorage.cpp

Mosaic.o: ./Player/Mosaic.cpp 
	g++ -Wall -Werror -std=c++14 -g -O -c ./Player/Mosaic.cpp 

BrokenTiles.o: ./Player/BrokenTiles.cpp 
	g++ -Wall -Werror -std=c++14 -g -O -c ./Player/BrokenTiles.cpp 

Factory.o: ./Components/Factory.cpp 
	g++ -Wall -Werror -std=c++14 -g -O -c ./Components/Factory.cpp 

Tile.o: ./Components/Tile.cpp 
	g++ -Wall -Werror -std=c++14 -g -O -c ./Components/Tile.cpp 

Node.o: ./Components/Node.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c ./Components/Node.cpp

LinkedList.o: ./Components/LinkedList.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c ./Components/LinkedList.cpp
 
Input.o: Input.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c Input.cpp
 
Menu.o: Menu.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c Menu.cpp
 
main.o: main.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c main.cpp

assign2: 
	g++ -Wall -Werror -std=c++14 -g -O -o assign2 GameEngineIO.o GameEngineCallback.o GameEngine.o Player.o MosaicStorage.o Mosaic.o BrokenTiles.o Factory.o Tile.o Node.o LinkedList.o Input.o Menu.o main.o

clean:
	rm -f assign2 *.o
