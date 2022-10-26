FLAG_LIB		=	./glfw3.dll -lm  
FLAG_INCLUDE	=	-I ./include
SRC_UTIL		=	./src/glad.c
FLAGS			=	$(FLAG_LIB) $(FLAG_INCLUDE) $(SRC_UTIL) 

SRC_MAIN		=	./src/test3.cpp

target: $(SRC_MAIN)
	g++ $< $(FLAGS)

static: $(SRC_MAIN)
	g++ $< $(FLAGS) -static


.PHONY:
run: a.exe
	./a.exe

.PHONY:
clean: 
	del a.exe