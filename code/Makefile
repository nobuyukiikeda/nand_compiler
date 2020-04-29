gcc_options = -std=c++17 -Wall --pedantic-errors

program : main.cpp utils.h Parser.cpp CodeWriter.cpp
	g++ $(gcc_options) $< -o $@

run : program
	./program

clean :
	rm -f ./program
	rm -f ./all.h.gch

.PHONY : run clean