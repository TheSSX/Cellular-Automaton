#Makefile for Cells

CC = g++
CFLAGS = -Wall -std=c++11

cells: cells.cpp
	${CC} ${CFLAGS} cells.cpp -o cells