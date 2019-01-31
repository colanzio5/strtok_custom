# credit -> https://github.com/lostella/SimpleMakefile/blob/master/Makefile

CC=g++ # define the compiler to use
TARGET=tokenizer # define the name of the executable
SOURCES=tokenizer.cpp functions.cpp
CFLAGS=-O3 
LFLAGS=

# define list of objects
OBJSC=$(SOURCES:.c=.o)
OBJS=$(OBJSC:.cpp=.o)

# the target is obtained linking all .o files
all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(TARGET) -lreadline

purge: clean
	rm -f $(TARGET)

clean:
	rm -f *.o
