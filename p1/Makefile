FLAG_LIB		=	-lglfw3 -lGL -lm -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread
FLAG_INCLUDE	=	-I ./include
SRC_UTIL		=	./src/glad.c ./src/stb.cpp
FLAGS			=	$(SRC_UTIL) $(FLAG_LIB) $(FLAG_INCLUDE)

a.out: ./src/test.cpp
	g++ $< $(FLAGS)
#	gcc ./src/test.cpp -lglfw3 -lGL -lm -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread

.PHONY:
run: a.out
	./a.out

.PHONY:
clean: 
	rm -f ./a.out