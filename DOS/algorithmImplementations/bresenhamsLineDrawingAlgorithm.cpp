/*
bresenhams' algorithm for drawing a line.
*/
#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <graphics.h>
#include <dos.h>

//The following function will be used to draw the line when
// the slope is dy/dx < 1
void plotLow(int x0, int y0, int x1, int y1, COLORS color) {
    int dy = y1 - y0;
    int dx = x1 - x0;
    int stepY = 1;
    int decisionParameter;
    int currentX, currentY=y0;
    // if slope is negative then y will either decrement or will remain same.
    if (dy < 0) {
        stepY = -1;
        dy = - dy;     // change back dy to positive as we need dy to be positive
                    // for the calculation of decisionParameter
    }
    decisionParameter = 2*dy - dx;
    for(currentX = x0; currentX <= x1; currentX++) {
        putpixel(currentX, currentY, color);
        if(decisionParameter < 0) {
            decisionParameter += 2 * dy;
        } else {
            decisionParameter += 2 * dy - 2 * dx;
            currentY += stepY;
        }
    }
}

//The following function will be used to draw the line when
// the slope is dy/dx > 1
// this function is basically just dx -> dy and dy -> dx in plotLow function
// as the slope > 1 so we need to decide on the value of x
// if we are going to increment it or not
void plotHigh(int x0, int y0, int x1, int y1, COLORS color) {
    int dy = y1 - y0;
    int dx = x1 - x0;
    int stepX = 1;
    int decisionParameter;
    int currentX=x0, currentY;
    // if slope is negative then x will either decrement or will remain same.
    if (dx < 0) {
        stepX = -1;
        dx = - dx;     // change back dx to positive as we need dx to be positive
                    // for the calculation of decisionParameter
    }
    decisionParameter = 2*dx - dy;
    for(currentY = y0; currentY <= y1; currentY++) {
        putpixel(currentX, currentY, color);
        if(decisionParameter < 0) {
            decisionParameter += 2 * dx;
        } else {
            decisionParameter += 2 * dx - 2 * dy;
            currentX += stepX;
        }
    }
}

void plotLine(int x0, int y0, int x1, int y1, COLORS color) {
    if(abs(y1 - y0) > abs(x1 - x0)) { // call plotHigh if slope > 1
        if(y1 < y0) { // be sure that the y1 > y0
            plotHigh(x1, y1, x0, y0, color);
        } else {
            plotHigh(x0, y0, x1, y1, color);
        }
    } else {
        if (x1 < x0) {
            plotLow(x1, y1, x0, y0, color);
        } else {
            plotLow(x0, y0, x1, y1, color);
        }
    }
}


int main() {
    int gd = DETECT, gm;
    int x0, y0, x1, y1;
    cout<<"Enter (x1 y1): ";
    cin>> x0 >> y0;
    cout<<"Enter (x2 y2): ";
    cin>> x1 >> y1;
    initgraph(&gd, &gm, "C:\\TC\\BGI");
    plotLine(x0, y0, x1, y1, RED);
    getch();
    closegraph();
    return 0;
}
