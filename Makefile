PROG = gateKeeper.exe
SRC_DIR = src
INCLUDE_DIR = include
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(SRC_DIR)/%.o)
CFLAGS = -g -Wall -Wextra -Wconversion -pedantic -std=c11 -I$(INCLUDE_DIR)
LIBS = -lcomctl32 -lgdi32 -luser32

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(PROG)

.PHONY: all clean