#update on every project

CFILES=main.cpp Client.cpp 
BIN=irc_server

#update to match c or cpp

CC=c++
CEXT=.cpp
CPPFLAGS=-std=c++98

#update if needed

CFLAGS=-Wall -Wextra -Werror -MD -I $(INCLDIR)
SRCDIR=src
SUBDIR=/.  

#shouldn't need to update

RM=rm -rf
MD=mkdir -p
OBJDIR=.obj
INCLDIR=incl

OBJ_FILES=$(addprefix $(OBJDIR)/,$(CFILES:$(CEXT)=.o))
DEP_FILES=$(addprefix $(OBJDIR)/,$(CFILES:$(CEXT)=.d))
SUB_DIR=$(addprefix $(OBJDIR),$(SUBDIR))

all: $(BIN)

re: fclean all

fclean: clean
	$(RM) $(BIN)

clean:
	$(RM) $(OBJDIR)

$(BIN): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%$(CEXT) Makefile | $(SUB_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

$(SUB_DIR):
	$(MD) $@

source:
	sed "s#^SUBDIR=/.*#SUBDIR=/. $$(find src -type d | sed "s#^src##g" | tr "\n" " ")#g" Makefile -i
	sed "s#^CFILES=.*#CFILES=$$(find src -type f -name "*$(CEXT)" | sed "s#^src/##g" | tr "\n" " ")#g" Makefile -i

.PHONY: all clean fclean re

-include $(DEP_FILES)
