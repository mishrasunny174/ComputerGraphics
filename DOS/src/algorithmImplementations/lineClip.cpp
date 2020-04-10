#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <iostream.h>
#include <conio.h>
#include <dos.h>

#define TOP 0x8
#define BOTTOM 0x4
#define RIGHT 0x2
#define LEFT 0x1

struct point_t {
	float x,y;
};

struct frame_t {
	float xMin,yMin,xMax,yMax;
};

struct line_t {
	point_t p1, p2;
};

line_t clipLine(frame_t f,line_t l);

unsigned int assignPositionalFlag(point_t p, frame_t f) {
	unsigned int ret = 0;
	if (p.x < f.xMin) ret |= TOP;
	if (p.y < f.yMin) ret |= LEFT;
	if (p.x > f.xMax) ret |= BOTTOM;
	if (p.y > f.yMax) ret |= RIGHT;
	return ret;
}

void swapPoints(line_t &l) {
	point_t temp;
	temp = l.p1;
	l.p1 = l.p2;
	l.p2 = temp;
}

void drawLine(line_t l, int color) {
	int currentColor = getcolor();
	setcolor(color);
	line(l.p1.x, l.p1.y, l.p2.x, l.p2.y);
	setcolor(currentColor);
}

void drawFrame(frame_t f, int color) {
	int currentColor = getcolor();
	setcolor(color);
	rectangle(f.xMin, f.yMin, f.xMax, f.yMax);
	setcolor(currentColor);
}


point_t leftIntersect(frame_t f, line_t l);
point_t rightIntersect(frame_t f, line_t l);
point_t topIntersect(frame_t f, line_t l);
point_t bottomIntersect(frame_t f, line_t l);


int main() {
	int gd=DETECT, gm;
	frame_t f;
	line_t l;
	clrscr();
	cout<<"Enter xMin: ";
	cin>>f.xMin;
	cout<<"Enter xMax: ";
	cin>>f.xMax;
	cout<<"Enter yMin: ";
	cin>>f.yMin;
	cout<<"Enter yMax: ";
	cin>>f.yMax;
	cout<<"Enter p1 of line (x y): ";
	cin>>l.p1.x>>l.p1.y;
	cout<<"Enter p2 of line (x y): ";
	cin>>l.p2.x>>l.p2.y;
	clrscr();
	initgraph(&gd, &gm, "C:\\TC\\BGI");
	drawFrame(f, RED);
	drawLine(l, WHITE);
	line_t clippedLine = clipLine(f, l);
	drawLine(clippedLine, BLUE);
	getch();
	closegraph();
	return 0;
}


point_t leftIntersect(frame_t f, line_t l) {
	point_t ret;
	float slope;
	if ((l.p2.x - l.p1.x) != 0)
		slope = (l.p2.y - l.p1.y)/(l.p2.x - l.p1.x);
	else
		slope = 99999.00;
	ret.y = f.yMin;
	ret.x = l.p1.x + (f.yMin-l.p1.y)/slope;
	return ret;
}

point_t rightIntersect(frame_t f, line_t l) {
	point_t ret;
	float slope;
	if ((l.p2.x - l.p1.x) != 0)
		slope = (l.p2.y - l.p1.y)/(l.p2.x - l.p1.x);
	else
		slope = 99999.00;
	ret.y = f.yMax;
	ret.x = l.p1.x + (f.yMax - l.p1.y)/slope;
	return ret;
}

point_t topIntersect(frame_t f, line_t l) {
	point_t ret;
	float slope;
	if ((l.p2.x - l.p1.x) != 0)
		slope = (l.p2.y - l.p1.y)/(l.p2.x - l.p1.x);
	else
		slope = 99999.00;
	ret.x = f.xMin;
	ret.y = l.p1.y + slope*(f.xMin - l.p1.x);
	return ret;
}

point_t bottomIntersect(frame_t f, line_t l) {
	point_t ret;
	float slope;
	if ((l.p2.x - l.p1.x) != 0)
		slope = (l.p2.y - l.p1.y)/(l.p2.x - l.p1.x);
	else
		slope = 99999.00;
	ret.x = f.xMax;
	ret.y = l.p1.y + slope*(f.xMax - l.p1.x);
	return ret;
}

line_t clipLine(frame_t f, line_t l) {
	line_t ret;
	unsigned int flagP1 = assignPositionalFlag(l.p1, f), flagP2 = assignPositionalFlag(l.p2, f);
	if(flagP1 == 0 && flagP2 == 0)
		return l;
	else if (flagP1 != 0 && flagP2 != 0) {
		ret.p1.x = 0;
		ret.p1.y = 0;
		ret.p2.x = 0;
		ret.p2.y = 0;
		return ret;
	} else {
		if(flagP1 == 0) {
			swapPoints(l);
			return clipLine(f, l);
		}
		else {
			if(flagP1 & LEFT) {
				l.p1 = leftIntersect(f, l);
				return clipLine(f, l);
			}
			else if(flagP1 & RIGHT) {
				l.p1 = rightIntersect(f, l);
				return clipLine(f, l);
			} else if(flagP1 & TOP) {
				l.p1 = topIntersect(f, l);
				return clipLine(f, l);
			} else {
				l.p1 = bottomIntersect(f, l);
				return clipLine(f, l);
			}
		}
	}

}