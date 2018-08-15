CC=g++
CWARNINGS=-Wall -Wextra
CINCLUDES=-lpthread

webserver: $(shell find src/*.cpp)
	$(CC) $(CWRNINGS) -o webserver $(shell find src/*.cpp) $(CINCLUDES)
