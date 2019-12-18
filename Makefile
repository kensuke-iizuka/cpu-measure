CFLAGS = -std=c++11
OPT = -O2
EXEC_FILE = test

all: run

run: build
	./$(EXEC_FILE)

build: conv2.cc
	g++ $(CFLAGS) -o $(EXEC_FILE) $< $(OPT)

clean:
	rm -f *.o
	rm -f $(EXEC_FILE)
