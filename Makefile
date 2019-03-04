# GNU Makefile
# using implicit rules
# generate targets for every .c file (only standalone .c commands)

CFLAGS=-Wall
CC=gcc
RM=/bin/rm -f

archive = tp6_src

sources = \
Makefile \
squdpclient.c \
squdpserv.c \
simplecl.c \
simplesv.c \
sqserv.h \
SqTCPServ.java \

# generate targets, only standalone .c file
execs = $(filter %.c,$(sources))
classes = $(filter %.java,$(sources))
targets = $(execs:.c=) $(classes:.java=.class)

all: $(targets)

SqTCPServ.class: SqTCPServ.java
	javac SqTCPServ.java

.PHONY : clean zip test

zip: $(sources)
	@echo "building zip archive" $(addsuffix .zip,$(archive))
	@$(RM) $(addsuffix .zip,$(archive)) ; mkdir $(archive) ; cp $(sources) $(archive) ; zip -r $(archive) $(archive) ; $(RM) -r $(archive)

clean:
	$(RM) $(targets)
