all: prog
prog: prog.o
	gcc -o $@ $^ -nostartfiles -no-pie
%.o: %.asm  
	nasm -f elf64 -o $@ $<
clean:
	rm -rf prog.o prog
