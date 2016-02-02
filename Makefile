PROGRAMS = myshell

.PHONY: all clean

all: $(PROGRAMS)

%: %.c
	gcc -Wall -O2 $^ -o $@

clean:
	rm -rf *~ $(PROGRAMS)
