#ifndef ALG
#define ALG
#include "buffer_screen.hpp"
#include "point.hpp"
#include <math.h>

#include <cstdio>


class Algorithm{
   public:
   virtual void exec_algorithm(const Point & c, double r)=0; 
};

class DDA: public Algorithm{
    public:
    virtual void exec_algorithm(const Point &c, double r) override final{
        Buffer buff(12*int(ceil(r))+64); 
        int t = 10000;
        auto begTime = glfwGetTime();
        
        while (t--) {
        buff.cnt = 0;
        //TODO: implement MID POINT algorithm to draw a circle here. 
        // c: center of circle
        // r: radius of circle 
        // to put a white pixel on position (x,y) on screen, call "buff.putPixel(x,y);"
        // here x,y are all integers. 
        int x=0, y=-std::ceil(r);
        float d;
        while (-y>=x) {
            buff.putPixel(c.x+x, c.y+y);
            buff.putPixel(c.x-x, c.y+y);
            buff.putPixel(c.x+x, c.y-y);
            buff.putPixel(c.x-x, c.y-y);
            buff.putPixel(c.x+y, c.y+x);
            buff.putPixel(c.x-y, c.y+x);
            buff.putPixel(c.x+y, c.y-x);
            buff.putPixel(c.x-y, c.y-x);
            d = (x+1)*(x+1)+(y+0.5)*(y+0.5)-r*r;
            x++;
            if (d>0) y++;
        }

        // END of TODO
        }
        std::cout <<"func1 dda: "<< (glfwGetTime() - begTime) * 1000 << "ms\n";
        screen.append(buff);
    }
};

class Bresham: public Algorithm{
    public:
    virtual void exec_algorithm(const Point & c, double r) override final{
        Buffer buff(12*int(ceil(r))+64); 
        int t = 10000;  
        auto begTime = glfwGetTime();

        while (t--) {
        buff.cnt=0;
        //TODO: implement some naive algorithm to draw a circle here 
        // c: center of circle 
        // r: radius of circle
        // to put a white pixel on position (x,y) on screen, call "buff.putPixel(x,y);"
        // here x,y are all integers. 
        const float PI = 3.1415926;
        int seg = ceil(std::log(r))*3+3;
        int x1, y1, x2, y2, dx, dy;
        int x=-ceil(r), y=0;
        for (int i=1; i<=seg && x<=0; i++) {
            x1 = x, y1 = y;
            x2 = -ceil(r*std::cos(PI*i/seg));
            y2 = ceil(r*std::sin(PI*i/seg));

            dx = x2-x1;
            dy = y2-y1;
            //printf("%d %d -> %d %d (%d %d)\n", x, y, x2, y2, dx, dy);
            if (dx > dy) {
                while (x!=x2 && x<=0) {
                    //printf("%d %d\n", x, y);
                    buff.putPixel(c.x+x, c.y+y);
                    buff.putPixel(c.x-x, c.y+y);
                    buff.putPixel(c.x+x, c.y-y);
                    buff.putPixel(c.x-x, c.y-y);
                    x++;
                    y = y1 + (x-x1)*dy/dx;
                }
            }
            else {
                while (y!=y2 && x<=0) {
                    //printf("%d %d\n", x, y);
                    buff.putPixel(c.x+x, c.y+y);
                    buff.putPixel(c.x-x, c.y+y);
                    buff.putPixel(c.x+x, c.y-y);
                    buff.putPixel(c.x-x, c.y-y);
                    y++;
                    x = x1 + (y-y1)*dx/dy;
                }
            }
        }

        // END of TODO
        }
        std::cout <<"func2 bresham: "<< (glfwGetTime() - begTime) * 1000 << "ms\n"; 
        screen.append(buff);
    }
};

#endif