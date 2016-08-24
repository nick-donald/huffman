all:
	g++ -Wall node.cpp main.cpp -o huff

debug:
	g++ -Wall -g node.cpp main.cpp -o huff
	gdb --args ./huff test2.txt

test: $(Make) all
	./huff test2.txt
