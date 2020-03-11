/*
** Question: Apply various 2D transformation on on a 2D object
** (Use homogeneous co-ordinates)
** Author: Sunny Mishra
** Date: 11-03-2020
*/

#include <iostream.h>
#include <conio.h>
#include <graphics.h>
#include <dos.h>
#include <stdlib.h>
#include <math.h>

#define XMIN 320
#define YMIN 240
#define XMAX 640
#define YMAX 480

void matrixMultiplication(double a[][3], double b[][3], double result[][3]);
void drawTriangle(double points[][3], int color);
void translate();
void rotate();
void scale();
void shear();
void reflection();
void reflectionAboutX();
void reflectionAboutY();
void reflectionAboutXY();
void reflectionAboutYX();
void drawCanvas();

int main() {
	int choice;
	while(1) {
		clrscr();
		cout<<"### Main Menu ###"<<endl;
		cout<<"1) Translation"<<endl;
		cout<<"2) Rotation"<<endl;
		cout<<"3) Scale"<<endl;
		cout<<"4) Shear"<<endl;
		cout<<"5) Reflection"<<endl;
		cout<<"Enter 0 to exit..."<<endl;
		cout<<"Enter your choice: ";
		cin>>choice;
		switch(choice) {
			case 0:
				return 0;
			case 1:
				translate();
				break;
			case 2:
				rotate();
				break;
			case 3:
				scale();
				break;
			case 4:
				shear();
				break;
			case 5:
				reflection();
				break;
			default:
				cout<<"Wrong choice try again ...";
				getch();
		}
	}
}

void translate() {
	int gd=DETECT, gm;
	double xFact, yFact;
	double triangle[3][3];
	cout<<"### Enter triangle co-ordinates ###"<<endl;
	for(int i=0; i<3; i++) {
		cout<<"Enter point "<<i+1<<" : ";
		cin>>triangle[i][0]>>triangle[i][1];
		triangle[i][2] = 1;
	}
	cout<<"Enter translation factors (x, y): ";
	cin>>xFact>>yFact;
	double translationMatrix[3][3] = {{1, 0, 0},
									  {0, 1, 0},
									  {xFact, yFact, 1}};
	double result[3][3] = {{0, 0, 0},
						   {0, 0, 0},
						   {0, 0, 0}};
	matrixMultiplication(triangle, translationMatrix, result);
	initgraph(&gd, &gm, "C:\\tc\\bgi");
	drawCanvas();
	drawTriangle(triangle, RED);
	drawTriangle(result, BLUE);
	getch();
	closegraph();
}

void rotate() {
	int gd=DETECT, gm;
	double rotationAngle;
	double triangle[3][3];
	cout<<"### Enter triangle co-ordinates ###"<<endl;
	for(int i=0; i<3; i++) {
		cout<<"Enter point "<<i+1<<" : ";
		cin>>triangle[i][0]>>triangle[i][1];
		triangle[i][2] = 1;
	}
	cout<<"Enter rotation angle : ";
	cin>>rotationAngle;
	rotationAngle *= 3.14/180;
	double translationMatrix[3][3] = {{cos(rotationAngle), sin(rotationAngle), 0},
									  {-sin(rotationAngle), cos(rotationAngle), 0},
									  {0, 0, 1}};
	double result[3][3] = {{0, 0, 0},
						   {0, 0, 0},
						   {0, 0, 0}};
	matrixMultiplication(triangle, translationMatrix, result);
	initgraph(&gd, &gm, "C:\\tc\\bgi");
	drawCanvas();
	drawTriangle(triangle, RED);
	drawTriangle(result, BLUE);
	getch();
	closegraph();
}

void scale() {
	int gd=DETECT, gm;
	double factor;
	double triangle[3][3];
	cout<<"### Enter triangle co-ordinates ###"<<endl;
	for(int i=0; i<3; i++) {
		cout<<"Enter point "<<i+1<<" : ";
		cin>>triangle[i][0]>>triangle[i][1];
		triangle[i][2] = 1;
	}
	cout<<"Enter scaling factor : ";
	cin>>factor;
	double translationMatrix[3][3] = {{factor, 0, 0},
									  {0, factor, 0},
									  {0, 0, 1}};
	double result[3][3] = {{0, 0, 0},
						   {0, 0, 0},
						   {0, 0, 0}};
	matrixMultiplication(triangle, translationMatrix, result);
	initgraph(&gd, &gm, "C:\\tc\\bgi");
	drawCanvas();
	drawTriangle(triangle, RED);
	drawTriangle(result, BLUE);
	getch();
	closegraph();
}

void shear() {
	int gd=DETECT, gm;
	double xFact, yFact;
	double triangle[3][3];
	cout<<"### Enter triangle co-ordinates ###"<<endl;
	for(int i=0; i<3; i++) {
		cout<<"Enter point "<<i+1<<" : ";
		cin>>triangle[i][0]>>triangle[i][1];
		triangle[i][2] = 1;
	}
	cout<<"Enter shear factors (x, y): ";
	cin>>xFact>>yFact;
	double translationMatrix[3][3] = {{1, yFact, 0},
									  {xFact, 1, 0},
									  {0, 0, 1}};
	double result[3][3] = {{0, 0, 0},
						   {0, 0, 0},
						   {0, 0, 0}};
	matrixMultiplication(triangle, translationMatrix, result);
	initgraph(&gd, &gm, "C:\\tc\\bgi");
	drawCanvas();
	drawTriangle(triangle, RED);
	drawTriangle(result, BLUE);
	getch();
	closegraph();
}

void reflectionAboutX() {
	int gd=DETECT, gm;
	double triangle[3][3];
	cout<<"### Enter triangle co-ordinates ###"<<endl;
	for(int i=0; i<3; i++) {
		cout<<"Enter point "<<i+1<<" : ";
		cin>>triangle[i][0]>>triangle[i][1];
		triangle[i][2] = 1;
	}
	double translationMatrix[3][3] = {{1, 0, 0},
									  {0, -1, 0},
									  {0, 0, 1}};
	double result[3][3] = {{0, 0, 0},
						   {0, 0, 0},
						   {0, 0, 0}};
	matrixMultiplication(triangle, translationMatrix, result);
	initgraph(&gd, &gm, "C:\\tc\\bgi");
	drawCanvas();
	drawTriangle(triangle, RED);
	drawTriangle(result, BLUE);
	getch();
	closegraph();
}

void reflectionAboutY() {
	int gd=DETECT, gm;
	double triangle[3][3];
	cout<<"### Enter triangle co-ordinates ###"<<endl;
	for(int i=0; i<3; i++) {
		cout<<"Enter point "<<i+1<<" : ";
		cin>>triangle[i][0]>>triangle[i][1];
		triangle[i][2] = 1;
	}
	double translationMatrix[3][3] = {{-1, 0, 0},
									  {0, 1, 0},
									  {0, 0, 1}};
	double result[3][3] = {{0, 0, 0},
						   {0, 0, 0},
						   {0, 0, 0}};
	matrixMultiplication(triangle, translationMatrix, result);
	initgraph(&gd, &gm, "C:\\tc\\bgi");
	drawCanvas();
	drawTriangle(triangle, RED);
	drawTriangle(result, BLUE);
	getch();
	closegraph();
}

void reflectionAboutXY() {
	int gd=DETECT, gm;
	double triangle[3][3];
	cout<<"### Enter triangle co-ordinates ###"<<endl;
	for(int i=0; i<3; i++) {
		cout<<"Enter point "<<i+1<<" : ";
		cin>>triangle[i][0]>>triangle[i][1];
		triangle[i][2] = 1;
	}
	double translationMatrix[3][3] = {{0, 1, 0},
									  {1, 0, 0},
									  {0, 0, 1}};
	double result[3][3] = {{0, 0, 0},
						   {0, 0, 0},
						   {0, 0, 0}};
	matrixMultiplication(triangle, translationMatrix, result);
	initgraph(&gd, &gm, "C:\\tc\\bgi");
	drawCanvas();
	drawTriangle(triangle, RED);
	drawTriangle(result, BLUE);
	getch();
	closegraph();
}

void reflectionAboutYX() {
	int gd=DETECT, gm;
	double triangle[3][3];
	cout<<"### Enter triangle co-ordinates ###"<<endl;
	for(int i=0; i<3; i++) {
		cout<<"Enter point "<<i+1<<" : ";
		cin>>triangle[i][0]>>triangle[i][1];
		triangle[i][2] = 1;
	}
	double translationMatrix[3][3] = {{0, -1, 0},
									  {-1, 0, 0},
									  {0, 0, 1}};
	double result[3][3] = {{0, 0, 0},
						   {0, 0, 0},
						   {0, 0, 0}};
	matrixMultiplication(triangle, translationMatrix, result);
	initgraph(&gd, &gm, "C:\\tc\\bgi");
	drawCanvas();
	drawTriangle(triangle, RED);
	drawTriangle(result, BLUE);
	getch();
	closegraph();
}


void matrixMultiplication(double a[][3], double b[][3], double result[][3]) {
	for(int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			for(int k=0; k<3; k++) {
				result[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

void reflection() {
	int choice;
	while(1) {
		clrscr();
		cout<<"### Reflection Menu ###"<<endl;
		cout<<"1) Reflection about X axis"<<endl;
		cout<<"2) Reflection about Y axis"<<endl;
		cout<<"3) Reflection about Y=X axis"<<endl;
		cout<<"4) Reflection about Y=-X axis"<<endl;
		cout<<"Enter 0 to return to main menu ..."<<endl;
		cout<<"Enter your choice: ";
		cin>>choice;
		switch(choice) {
			case 0:
				return;
			case 1:
				reflectionAboutX();
				break;
			case 2:
				reflectionAboutY();
				break;
			case 3:
				reflectionAboutXY();
				break;
			case 4:
				reflectionAboutYX();
				break;
			default:
				cout<<"Wrong choice try again ...";
				getch();
		}
	}
}

void drawCanvas() {
	setcolor(WHITE);
	line(0, YMIN, XMAX, YMIN);
	line(XMIN, 0, XMIN, YMAX);
}

void drawTriangle(double points[][3], int color) {
	int currentColor = getcolor();
	setcolor(color);
	line(XMIN+points[0][0], YMIN+points[0][1], XMIN+points[1][0], YMIN+points[1][1]);
	line(XMIN+points[1][0], YMIN+points[1][1], XMIN+points[2][0], YMIN+points[2][1]);
	line(XMIN+points[2][0], YMIN+points[2][1], XMIN+points[0][0], YMIN+points[0][1]);
	setcolor(currentColor);
}
