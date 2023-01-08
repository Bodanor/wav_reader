CC = gcc
CFLAGS = -g #-Wall -g -Wextra -Wno-unused-parameter -Werror=return-type -pedantic -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fpie -Wl,-pie -O2 -pipe -fno-ident 

C_SOURCE := $(wildcard *.c)
C_OBJECTS := $(C_SOURCE:.c=.o)
C_HEADERS := $(wildcard *.h)

wav_loader: $(C_OBJECTS) $(C_HEADERS)
	$(CC) $(CFLAGS) -o $@ $(C_OBJECTS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o
	rm -rf wav_loader
