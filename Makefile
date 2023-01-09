SHELL := /bin/bash

ALL_FLAGS= -std=c++17 -Wpedantic -Wall -Wextra -Wconversion -Weffc++ -Wstrict-null-sentinel -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wsign-promo -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -Wold-style-cast -Wzero-as-null-pointer-constant -pthread
CC = g++-9

.PHONY: all, clean, mrproper

.SUFFIXES:

all: main

main: main.cpp
	$(CC) $(ALL_FLAGS) $^ -o  $@

clean:
	rm -rf *.o

mrproper:clean
	rm -rf main
