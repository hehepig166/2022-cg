FLAG_LIB		=	./glfw3.dll -lm  
FLAG_INCLUDE	=	-I ./include
SRC_UTIL		=	./src/glad.c
FLAGS			=	$(FLAG_LIB) $(FLAG_INCLUDE) $(SRC_UTIL) 

a.exe: ./src/test3.cpp
	g++ $< $(FLAGS)


.PHONY:
run: a.exe
	./a.exe

.PHONY:
clean: 
	del a.exe