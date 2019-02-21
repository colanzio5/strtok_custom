
CC=g++ # define the compiler to use
TARGET=mush # define the name of the executable
SOURCES=main.cpp command.cpp tokenizer.cpp
CFLAGS=-O3 
LFLAGS=-lreadline

# define list of objects
OBJSC=$(SOURCES:.c=.o)
OBJS=$(OBJSC:.cpp=.o)

# the target is obtained linking all .o files
all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(TARGET) 

purge: clean
	rm -f $(TARGET)

clean:
	rm -f *.o
