CFLAGS += -I.. -I./include -Os 
WFLAGS = -Wall -Wextra -Wpedantic
CPPFLAGS += $(CFLAGS) --std=c++17
ifeq ($(OS),Windows_NT)
	LDFLAGS += -s -lopengl32 -lgdi32
	RMCMD = del 
else
	RMCMD = rm -f
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		LDFLAGS += -framework OpenGL -framework Cocoa
	else ifeq ($(UNAME_S),Linux)
		LDFLAGS += -s -lGLU -lGL -lX11
	endif
endif

all:
	gcc -c tigr.c -o tigr.o $(CFLAGS) 
	gcc -c test.c -o test.o $(CFLAGS) 
	gcc tigr.o test.o -o test $(LDFLAGS)

clean:
	$(RMCMD) test *.o
