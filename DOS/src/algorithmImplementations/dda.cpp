#include <iostream.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <math.h>


int main() {
	int gd=DETECT, gm;
	int x1=100, y1=100, x2=500, y2=50, dx, dy;
	float currentX, currentY;
	float slope;
	cout<<"enter cordinates of first point(x1, y1): ";
	cin>> x1 >> y1;
	cout<<"enter cordinates of seconf point(x2, y2): ";
	cin>> x2 >> y2;
	dx = (x2 - x1);
	dy = (y2 - y1);
	slope = (float)dy/(float)dx;
	initgraph(&gd, &gm, "C:\\TC\\BGI");
	if (abs(slope)<=1) {
		currentX = x1;
		currentY = y1;
		for(int i=0; i<dx; i++) {
			putpixel(currentX, currentY, RED);
			currentX += 1;
			currentY += slope;
		}
	} else {
		currentX = x1;
		currentY = y1;
		for(int i=0; i<dy; i++) {
			putpixel(currentX, currentY, RED);
			currentY += 1;
			currentX += 1 / slope;
		}
	}
	getch();
	closegraph();
	return 0;
}