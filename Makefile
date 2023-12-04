PROG = gateKeeper.exe
SRCS = gateKeeper.c tagHandler.c randomPass.c timestamp.c arduinoHandler.c sendPassToArduino.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -g -Wall -Wextra -Wconversion -pedantic -std=c11
LIBS = -lcomctl32 -lgdi32 -luser32

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(PROG)

.PHONY: all clean