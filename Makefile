all: compile run


compile:
        gcc -c server.cpp
        gcc -c handlers.cpp

run:
        ./a.out
