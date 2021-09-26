all:
	g++ ./source/main.cpp -O2
valgrind:
	g++ ./source/main.cpp -O2
	for i in ./test/*.txt; do echo $$(basename $$i); valgrind ./a.out < $$i; done;
testing:
	g++ ./source/main.cpp -O2
	for i in ./test/*.txt; do echo $$(basename $$i); ./a.out < $$i; done;
	cat ./test/answers
debug_mode:
	g++ ./source/main.cpp -O0 -g