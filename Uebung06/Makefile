CC       = clang
OBJDIR   = build
SRCS     = $(wildcard *.c)              
OBJECTS  = $(SRCS:%.c=$(OBJDIR)/%.o)   
LIBS     = -lm
CFLAGS   = -Wall -Wextra
CPPFLAGS =                            

CommandInterpreter: $(OBJECTS)
	$(CC) -g -o $@ $^ $(LIBS)

Debug: $(OBJECTS)
	$(CC) -g -o $@ $^ $(LIBS)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean run

clean:
	rm -rf $(OBJDIR) CommandInterpreter Debug

run: CommandInterpreter
	./CommandInterpreter
