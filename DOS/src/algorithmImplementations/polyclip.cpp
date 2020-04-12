#include <iostream.h>
#include <conio.h>
#include <graphics.h>
#include <stdio.h>
#include <dos.h>


struct mouse_status_t {
	int x;
	int y;
	int mouseStatus;
};

struct point_t {
	float x, y;
};

struct frame_t {
	float  xMin, yMin, xMax, yMax;
};

// mouse handlers
void initMouse() {
	REGS r;
	r.x.ax = 0x0;
	int86(0x33, &r, &r);
}

void showMousePointer() {
	REGS r;
	r.x.ax = 0x1;
	int86(0x33, &r, &r);
}

void hideMousePointer() {
	REGS r;
	r.x.ax = 0x2;
	int86(0x33, &r, &r);
}

mouse_status_t getMouseStatus() {
	REGS r;
	mouse_status_t ret;
	r.x.ax = 0x3;
	int86(0x33, &r, &r);
	ret.x = r.x.cx;
	ret.y = r.x.dx;
	ret.mouseStatus = r.x.bx;
	return ret;
}
// functions to calculate intercept point
inline point_t topIntercept(point_t p1, point_t p2, frame_t f);
inline point_t leftIntercept(point_t p1, point_t p2, frame_t f);
inline point_t bottomIntercept(point_t p1, point_t p2, frame_t f);
inline point_t rightIntercept(point_t p1, point_t p2, frame_t f);

// functions to actually clip the polygon
void lClip(point_t polygon[], int &n, frame_t f);
void rClip(point_t polygon[], int &n, frame_t f);
void tClip(point_t polygon[], int &n, frame_t f);
void bClip(point_t polygon[], int &n, frame_t f);
void polyClip(point_t polygon[], int &n, frame_t f);
inline int isInsideLeft(point_t p, frame_t f);
inline int isInsideRight(point_t p, frame_t f);
inline int isInsideTop(point_t p, frame_t f);
inline int isInsideBottom(point_t p, frame_t f);

// main event loop
void eventLoop();

// drawing helper function
void drawFrame(frame_t f, int color) {
	int currentColor = getcolor();
	setcolor(color);
	rectangle(f.xMin, f.yMin, f.xMax, f.yMax);
	setcolor(currentColor);
}

void drawPolygon(point_t polygon[], int n, int color) {
	int currentColor = getcolor();
	setcolor(color);
	for(int i=0; i<n; i++) {
		line(polygon[i].x, polygon[i].y, polygon[(i+1)%n].x, polygon[(i+1)%n].y);
	}
	setcolor(currentColor);
}

int main() {
	int gd=DETECT, gm;
	initgraph(&gd, &gm, "C:\\tc\\bgi");
	cleardevice();
	eventLoop();
	getch();
	closegraph();
	return 0;
}

void eventLoop() {
	point_t polygon[100];
	int n = 0;
	frame_t f;
	f.xMin = 50;
	f.yMin = 50;
	int frameInputCount = 0;
	mouse_status_t m;
	initMouse();
	showMousePointer();
	outtextxy(10,10, "DRAW POLYGON AND THEN RIGHT CLICK TO SET FRAME MAX");
	while(1) {
		m = getMouseStatus();
		if (m.mouseStatus == 0x1) {
			polygon[n].x = m.x;
			polygon[n].y = m.y;
			n++;
			cleardevice();
			outtextxy(10, 10, "DRAW POLYGON AND THEN RIGHT CLICK TO SET FRAME MAX");
			drawPolygon(polygon, n, WHITE);
			sleep(1);
		}
		if (m.mouseStatus == 0x2) {
			f.xMax = m.x;
			f.yMax = m.y;
			sleep(1);
			break;
		}
	}
	cleardevice();
	drawFrame(f, RED);
	drawPolygon(polygon, n, WHITE);
	getch();
	polyClip(polygon, n, f);
	cleardevice();
	drawFrame(f, RED);
	drawPolygon(polygon, n, WHITE);
	hideMousePointer();
}

int isInsideLeft(point_t p, frame_t f) {
	return (p.y >= f.yMin);
}

int isInsideBottom(point_t p, frame_t f) {
	return (p.x <= f.xMax);
}

int isInsideRight(point_t p, frame_t f) {
	return (p.y <= f.yMax);
}

int isInsideTop(point_t p, frame_t f) {
	return p.x >= f.xMin;
}

void polyClip(point_t polygon[], int &n, frame_t f) {
	lClip(polygon, n, f);
	tClip(polygon, n, f);
	rClip(polygon, n, f);
	bClip(polygon, n, f);
}

void lClip(point_t polygon[], int &n, frame_t f) {
	point_t resultantPoints[100];
	int resultantSize = 0;
	point_t p1, p2, intersectPoint;
	for (int i=0; i<n; i++) {
		p1 = polygon[i];
		p2 = polygon[(i+1)%n];
		if(isInsideLeft(p1, f) && isInsideLeft(p2, f)) {
			resultantPoints[resultantSize++] = p2;
		} else if (isInsideLeft(p1, f) && (!isInsideLeft(p2, f))) {
			intersectPoint = leftIntercept(p1, p2, f);
			resultantPoints[resultantSize++] = intersectPoint;
		} else if((!isInsideLeft(p1, f)) && isInsideLeft(p2, f)) {
			intersectPoint = leftIntercept(p1, p2, f);
			resultantPoints[resultantSize++] = intersectPoint;
			resultantPoints[resultantSize++] = p2;
		} else {
			continue;
		}
	}
	for(i=0 ; i < resultantSize; i++) {
		polygon[i] = resultantPoints[i];
	}
	n = resultantSize;
}

void rClip(point_t polygon[], int &n, frame_t f) {
	point_t resultantPoints[100];
	int resultantSize = 0;
	point_t p1, p2, intersectPoint;
	for (int i=0; i<n; i++) {
		p1 = polygon[i];
		p2 = polygon[(i+1)%n];
		if(isInsideRight(p1, f) && isInsideRight(p2, f)) {
			resultantPoints[resultantSize++] = p2;
		} else if (isInsideRight(p1, f) && (!isInsideRight(p2, f))) {
			intersectPoint = rightIntercept(p1, p2, f);
			resultantPoints[resultantSize++] = intersectPoint;
		} else if((!isInsideRight(p1, f)) && isInsideRight(p2, f)) {
			intersectPoint = rightIntercept(p1, p2, f);
			resultantPoints[resultantSize++] = intersectPoint;
			resultantPoints[resultantSize++] = p2;
		} else {
			continue;
		}
	}
	for(i=0 ;i< resultantSize; i++) {
		polygon[i] = resultantPoints[i];
	}
	n = resultantSize;
}

void tClip(point_t polygon[], int &n, frame_t f) {
	point_t resultantPoints[100];
	int resultantSize = 0;
	point_t p1, p2, intersectPoint;
	for (int i=0; i<n; i++) {
		p1 = polygon[i];
		p2 = polygon[(i+1)%n];
		if(isInsideTop(p1, f) && isInsideTop(p2, f)) {
			resultantPoints[resultantSize++] = p2;
		} else if (isInsideTop(p1, f) && (!isInsideTop(p2, f))) {
			intersectPoint = topIntercept(p1, p2, f);
			resultantPoints[resultantSize++] = intersectPoint;
		} else if((!isInsideTop(p1, f)) && isInsideTop(p2, f)) {
			intersectPoint = topIntercept(p1, p2, f);
			resultantPoints[resultantSize++] = intersectPoint;
			resultantPoints[resultantSize++] = p2;
		} else {
			continue;
		}
	}
	for(i=0 ;i< resultantSize; i++) {
		polygon[i] = resultantPoints[i];
	}
	n = resultantSize;
}

void bClip(point_t polygon[], int &n, frame_t f) {
	point_t resultantPoints[100];
	int resultantSize = 0;
	point_t p1, p2, intersectPoint;
	for (int i=0; i<n; i++) {
		p1 = polygon[i];
		p2 = polygon[(i+1)%n];
		if(isInsideBottom(p1, f) && isInsideBottom(p2, f)) {
			resultantPoints[resultantSize++] = p2;
		} else if (isInsideBottom(p1, f) && (!isInsideBottom(p2, f))) {
			intersectPoint = bottomIntercept(p1, p2, f);
			resultantPoints[resultantSize++] = intersectPoint;
		} else if((!isInsideBottom(p1, f)) && isInsideBottom(p2, f)) {
			intersectPoint = bottomIntercept(p1, p2, f);
			resultantPoints[resultantSize++] = intersectPoint;
			resultantPoints[resultantSize++] = p2;
		} else {
			continue;
		}
	}
	for(i=0 ;i< resultantSize; i++) {
		polygon[i] = resultantPoints[i];
	}
	n = resultantSize;
}




point_t topIntercept(point_t p1, point_t p2, frame_t f) {
	point_t ret;
	float slope;
	if ((p2.x - p1.x) != 0)
		slope = (p2.y - p1.y)/(p2.x - p1.x);
	else
		slope = 9999.0;
	ret.x = f.xMin;
	ret.y = p1.y + slope*(f.xMin - p1.x);
	return ret;
}

point_t bottomIntercept(point_t p1, point_t p2, frame_t f) {
	point_t ret;
	float slope;
	if ((p2.x - p1.x) != 0)
		slope = (p2.y - p1.y)/(p2.x - p1.x);
	else
		slope = 9999.0;
	ret.x = f.xMax;
	ret.y = p1.y + slope*(f.xMax - p1.x);
	return ret;
}

point_t leftIntercept(point_t p1, point_t p2, frame_t f) {
	point_t ret;
	float slope;
	if ((p2.x - p1.x) != 0)
		slope = (p2.y - p1.y)/(p2.x - p1.x);
	else
		slope = 9999.0;
	ret.x = p1.x + (f.yMin - p1.y)/slope;
	ret.y = f.yMin;
	return ret;
}

point_t rightIntercept(point_t p1, point_t p2, frame_t f) {
	point_t ret;
	float slope;
	if ((p2.x - p1.x) != 0)
		slope = (p2.y - p1.y)/(p2.x - p1.x);
	else
		slope = 9999.0;
	ret.x = p1.x + (f.yMax - p1.y)/slope;
	ret.y = f.yMax;
	return ret;
}