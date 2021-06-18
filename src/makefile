CFLAGS = #-Wall
FLEX = lexer
BISON = parser
OBJS = symbs.o print.o emit.o model.o expression.o
EXEC = compil
# Executable
$(EXEC): $(BISON).o lex.yy.o $(OBJS)
	gcc -o compil $(BISON).o lex.yy.o $(OBJS)  -ly -ll # use the flag -lfl on macOS -lfl on Linux and Windows

symbs.o: symbs.c symbs.h print.h model.h emit.h expression.h
	gcc -o symbs.o -c symbs.c $(CFLAGS)

emit.o: emit.c emit.h expression.h
	gcc -o emit.o -c emit.c $(CFLAGS)

expression.o: expression.c expression.h
	gcc -o expression.o -c expression.c $(CFLAGS)

print.o: print.c print.h model.h
	gcc -o print.o -c print.c $(CFLAGS)

model.o: model.c model.h
	gcc -o model.o -c model.c $(CFLAGS)

# Bison
$(BISON).o: $(BISON).c $(BISON).h
	gcc -o $(BISON).o -c $(BISON).c $(CFLAGS)

$(BISON).c $(BISON).h: $(BISON).y
	bison -d -o $(BISON).c $(BISON).y

# Flex
lex.yy.o: lex.yy.c $(BISON).h
	gcc -o lex.yy.o -c lex.yy.c $(CFLAGS)

lex.yy.c: lexer.lex
	flex lexer.lex

clean:
	-rm -f *.asm *.o lex.yy.c $(BISON).c $(BISON).h $(BISON).output $(EXEC)