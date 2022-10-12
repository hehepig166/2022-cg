FLAG_LIB		=	-lglfw3 -lGL -lm -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread
FLAG_INCLUDE	=	-I ./include
SRC_UTIL		=	./src/glad.c
FLAGS			=	$(SRC_UTIL) $(FLAG_LIB) $(FLAG_INCLUDE)

a.out: ./src/test4.cpp
	g++ $< $(FLAGS) -g


.PHONY:
run: a.out
	./a.out 1.txt

.PHONY:
clean:
	rm -f ./a.out