#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <dos.h>

void drawCircle(const int x1, const int y1, const int r,const COLORS color) {
    int currentX = 0, currentY = r;
    int decisionVariable = 1 - r;
    while(currentX <= currentY) {
        //draw points in each quadrant
        putpixel(x1+currentX, y1+currentY, color);
        putpixel(x1+currentX, y1-currentY, color);
        putpixel(x1-currentX, y1+currentY, color);
        putpixel(x1-currentX, y1-currentY, color);
        putpixel(x1+currentY, y1+currentX, color);
        putpixel(x1+currentY, y1-currentX, color);
        putpixel(x1-currentY, y1+currentX, color);
        putpixel(x1-currentY, y1-currentX, color);
        delay(100); //for animation effect
        if (decisionVariable<0) {
            decisionVariable += 2 * currentX + 3;
        } else {
            decisionVariable += 2 * currentX - 2*currentY + 5;
            currentY--;
        }
        currentX++;
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TC\\BGI");
    drawCircle(300, 250, 50, RED);
    getch();
    closegraph();
    return 0;
}
