CCC = g++
FLAGS = -g -Wall -Wextra
GRBPATH = /opt/gurobi952/

main: main.cpp bb.o node.o cg.o data.o
	g++ main.cpp bb.o node.o cg.o data.o -o main -I$(GRBPATH)/linux64/include -L$(GRBPATH)/linux64/lib -lgurobi_c++ -lgurobi95
bb.o: src/bb.cpp
	$(CCC) $(FLAGS) -I$(GRBPATH)/linux64/include -L$(GRBPATH)/linux64/lib -lgurobi_c++ -lgurobi95 -c src/bb.cpp
node.o: src/node.cpp
	$(CCC) $(FLAGS) -I$(GRBPATH)/linux64/include -L$(GRBPATH)/linux64/lib -lgurobi_c++ -lgurobi95 -c src/node.cpp
cg.o: src/cg.cpp
	$(CCC) $(FLAGS) -I$(GRBPATH)/linux64/include -L$(GRBPATH)/linux64/lib -lgurobi_c++ -lgurobi95 -c src/cg.cpp
data.o: src/data.cpp
	g++ -O3 -Wall -DNDEBUG -std=c++11 -c src/data.cpp