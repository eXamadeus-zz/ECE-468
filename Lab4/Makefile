all:
	@echo
	@echo "Usage:"
	@echo "	make test"
	@echo "		Runs test."
	@echo
	@echo "	make verbose"
	@echo "		Runs with iterative information."
	@echo
	@echo "	make clean"
	@echo "		Cleans up the directory"
	@echo

lab4: clean lab4.c
	@gcc -Wall -o0 lab4.c -o run -lm

test: lab4
	@./run

verbose: lab4
	@./run -v

clean:
	-@rm -rf run *.o
