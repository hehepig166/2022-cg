FLAG_LIB		=	./glfw3.dll -lm  
FLAG_INCLUDE	=	-I ./include
SRC_UTIL		=	./src/glad.c
FLAGS			=	$(FLAG_LIB) $(FLAG_INCLUDE) $(SRC_UTIL) 

SRC_MAIN		=	./src/test3.cpp
#SRC_MAIN		=	./src/test_portal.cpp

target: $(SRC_MAIN)
	g++ $< $(FLAGS) -O3

static: $(SRC_MAIN)
	g++ $< $(FLAGS) -O3 -static


.PHONY:
run: a.exe
	./a.exe

.PHONY:
clean: 
	del a.exe