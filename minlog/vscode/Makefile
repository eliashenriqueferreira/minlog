#CXX_FLAGS := -std=c++17 -ggdb
CXX_FLAGS := -g 

SRC     := src
INCLUDE := inc

LD_FLAGS	:= 
LIBRARIES   := 
EXECUTABLE  := minlogCpp.elf


all: $(EXECUTABLE)

run: clean all
	clear
	./$(EXECUTABLE)

$(EXECUTABLE): main.o minlog.o
	g++ $(LD_FLAGS)  $^ -o $@ $(LIBRARIES)

main.o: $(SRC)/main.cpp	
	g++ $(CXX_FLAGS) -I$(INCLUDE) -c $^
	
minlog.o: $(SRC)/minlog.c
	gcc $(CXX_FLAGS) -I$(INCLUDE) -c $^
	
clean:
	rm -f *.o
	rm -f $(EXECUTABLE) 
	
