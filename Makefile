all:
	g++ -Wall node.cpp main.cpp -o huff

test: $(Make) all
	./huff test.txt
