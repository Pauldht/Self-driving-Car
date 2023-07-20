CC = gcc
EXEC = bin
OPT = -O0 
#check automatically if include files that c files depend on has been changed
DEPFLAGS = -MP -MD

SRCDIRS = src
INCDIRS = . ./include/

CPPFLAGS = $(foreach D,$(INCDIRS),-I$(D))
CFLAGS = -Wall -Wextra -g -lSDL2 -lSDL2_gfx -lSDL2_ttf -lm $(CPPFLAGS) $(OPT) $(DEPFLAGS)

CFILES = $(foreach D,$(SRCDIRS),$(wildcard $(D)/*.c))
OBJECTS = $(patsubst %.c,%.o,$(CFILES))
DEPFILES = $(patsubst %.c,%.d,$(CFILES))

all : $(EXEC)

$(EXEC) : $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY:
	clean

clean :
	rm -rf $(OBJECTS) $(DEPFILES) $(EXEC)

-include $(DEPFILES)
