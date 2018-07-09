//5709612005 Sakonphat Buayam
//assignment 1
//Computer Graphics 2 CS397
//semester 2/2016


#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <algorithm>
#include <typeinfo>

using namespace std;

//**********global data*********** 
int width = 600, height = 600;
const int clipmin = 0;
const int clipmax = 600;
const char *name = "Assignment1";

struct Point {
	float x;
	int y;
};
struct M {
	float x;
	int y;
};
// update index of point 
//int index = 6; //for Default
int index = 0;
// keep andy point
Point points[10];

// keep dashedLine Rectangle 
Point clipPointLeft[2], clipPointRight[2], clipPointBottom[2], clipPointTop[2];

//update mouseClick
int countClick = 0;

//*******************END global data 


//***************** Linkedlist
class Node {
public:
	int yMax;
	float xMin;
	M m;
	Node* next;
};
class Linkedlist {

public:
	Node* edgeTableArray[clipmax];
	Linkedlist() {

		for (int i = clipmin; i <= clipmax; i++) {
			edgeTableArray[i] = NULL;
		}
	}
	void addSorted(Point pointsA, Point pointsB) {
		//cout << "iniii" << endl;
		int ymax = max(pointsA.y, pointsB.y);
		int ymin = min(pointsA.y, pointsB.y);

		int xmin;
		if (pointsA.y != ymax) {
			xmin = pointsA.x;
		}
		else if (pointsB.y != ymax) {
			xmin = pointsB.x;
		}
		

		for (int i = clipmin; i <= clipmax; i++) {
			if (i == ymin) {
				if (edgeTableArray[i] == NULL) {
					edgeTableArray[i] = new Node();
					edgeTableArray[i]->yMax = ymax;
					edgeTableArray[i]->xMin = xmin;
					edgeTableArray[i]->m.x = pointsA.x-pointsB.x;
					edgeTableArray[i]->m.y = pointsA.y - pointsB.y;
					edgeTableArray[i]->next = NULL;
					//cout << "ggg" << endl;
				}
				else {
					//cout << "kkkkkkk" << endl;
					Node* temp = edgeTableArray[i];
					while (temp != NULL) {
						if (ymax >= temp->yMax) {
							//cout << ymax << endl;
							//cout << temp->yMax << endl;
							temp->next = new Node();
							temp->next->yMax = ymax;
							temp->next->xMin = xmin;
							temp->next->m.x = pointsA.x - pointsB.x;
							temp->next->m.y = pointsA.y - pointsB.y;
							temp->next->next = NULL;
							//cout << "aaaaaaa" << endl;
							break;
						}
						else if (ymax < temp->yMax) {
							//cout << ymax << endl;
							//cout << temp->yMax << endl;
							Node* temp2 = new Node();
							temp2->yMax = ymax;
							temp2->xMin = xmin;
							temp2->m.x = pointsA.x - pointsB.x;
							temp2->m.y = pointsA.y - pointsB.y;
							temp2->next = temp;
							edgeTableArray[i] = new Node();
							edgeTableArray[i] = temp2;
							//cout << "bbbbbb" << endl;
							break;
						}
						else if ((ymax > temp->yMax) && (ymax < temp->next->yMax)) {
							Node* temp2 = new Node();
							temp2->yMax = ymax;
							temp2->xMin = xmin;
							temp2->m.x = pointsA.x - pointsB.x;
							temp2->m.y = pointsA.y - pointsB.y;
							temp2->next = temp->next;
							temp->next = temp2;
							//cout << "cccccc" << endl;
							break;
						}
						temp = temp->next;
					}
				}
			}
			
		}
	}
	Node* getEdgeTable(int y) {
		Node* temp = edgeTableArray[y];
		return temp;
	}
	void print() {
		Node* temp;
		cout << "printLinkedlist---------------->" << endl << endl;
		for (int i = clipmin; i <= clipmax; i++) {
			temp = edgeTableArray[i];
			cout << "EdgeTable " << i << " = " << temp << endl;
			while (temp != NULL) {
				cout << "Scanline Y = " << i << endl;
				cout << "Y max = " << temp->yMax << endl;
				cout << "X min = " << temp->xMin << endl;
				cout << "M = " << temp->m.x << "/" << temp->m.y << endl;

				temp = temp->next;
			}
		}
		cout << "-------------------------------->" << endl << endl;
	}
};
class Linkedlist2 {
private:
	Node* head;
public:
	Linkedlist2() {
		head = NULL;
	}
	void addSorted(Node* edgeTable) {
		if (head == NULL) {
			head = new Node();
			head->yMax = edgeTable->yMax;
			head->xMin = edgeTable->xMin;
			head->m.x = edgeTable->m.x;
			head->m.y = edgeTable->m.y;
			head->next = NULL;
		}
		else {
			Node* temp = head;
			while (temp != NULL) {
				if (temp->next == NULL) {
					temp->next = new Node();
					temp->next->yMax = edgeTable->yMax;
					temp->next->xMin = edgeTable->xMin;
					temp->next->m.x = edgeTable->m.x;
					temp->next->m.y = edgeTable->m.y;
					temp->next->next = NULL;
					break;
				}
				else if(edgeTable->xMin <= temp->xMin){
					Node* temp2 = new Node();
					temp2->yMax = edgeTable->yMax;
					temp2->xMin = edgeTable->xMin;
					temp2->m.x = edgeTable->m.x;
					temp2->m.y = edgeTable->m.y;
					temp2->next = temp;
					head = new Node();
					head = temp2;
					break;
				}
				else if ( (edgeTable->xMin >= temp->xMin) && (edgeTable->xMin <= temp->next->xMin) ) {
					Node* temp2 = new Node();
					temp2->yMax = edgeTable->yMax;
					temp2->xMin = edgeTable->xMin;
					temp2->m.x = edgeTable->m.x;
					temp2->m.y = edgeTable->m.y;
					temp2->next = temp->next;
					temp->next = temp2;
					break;
				}
				temp = temp->next;
			}
		}
	}
	void sortedXAtY() {
		Node* temp = head;
		while (temp != NULL && temp->next != NULL) {
			if (temp->xMin == temp->next->xMin) {
				if (temp->yMax > temp->next->yMax) {
					//mem
					Node* temp2 = new Node();
					temp2->yMax = temp->yMax;
					temp2->xMin = temp->xMin;
					temp2->m.x = temp->m.x;
					temp2->m.y = temp->m.y;
					//swap
					temp->yMax = temp->next->yMax;
					temp->xMin = temp->next->xMin;
					temp->m.x = temp->next->m.x;
					temp->m.y = temp->next->m.y;
					//swap 
					temp->next->yMax = temp2->yMax;
					temp->next->xMin = temp2->xMin;
					temp->next->m.x = temp2->m.x;
					temp->next->m.y = temp2->m.y;
				}
			}
			temp = temp->next;
		}
	}
	
	Node* getHeadAET() {
		Node* temp = head;
		return temp;
	}
	void removeNode(int y) {
		cout << "Delete" << endl;
		Node *temp;
		Node *tempDel;

		if (head->yMax == y) {
			
			tempDel = head;
			
			head = tempDel->next;
			delete tempDel;
			return;
		}

		temp = head;
		tempDel = head->next;

		while (tempDel != NULL) {
			if (tempDel->yMax == y) {
				
				temp->next = tempDel->next;
	
				delete tempDel;
				break;
			}
			temp = tempDel;
			tempDel = tempDel->next;
		}
	}
	void print() {
		Node* temp = head;
		cout << "Print AET--------------------->"<< endl;
		while (temp != NULL) {
			//cout << "Scanline Y = " << y << endl;
			cout << "Y max = " << temp->yMax << endl;
			cout << "X min = " << temp->xMin << endl;
			cout << "M = " << temp->m.x << "/" << temp->m.y << endl <<endl;
			temp = temp->next;
		}
		cout << "------------------------------>" << endl<< endl;
	}

};


Linkedlist* list = new Linkedlist();


void OneLine(float firstVertexX, float firstVertexY, float secondVertexX, float secondVertexY) {
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex2f(firstVertexX, firstVertexY);
	glVertex2f(secondVertexX, secondVertexY);
	glEnd();
}

void clippingRectangular() {
	// store clipPointLeft
	clipPointLeft[0].x = 150.0f;
	clipPointLeft[0].y = 450.0f;

	clipPointLeft[1].x = 150.0f;
	clipPointLeft[1].y = 150.0f;

	// store clipPointRight
	clipPointRight[0].x = 450.0f;
	clipPointRight[0].y = 450.0f;

	clipPointRight[1].x = 450.0f;
	clipPointRight[1].y = 150.0f;

	// store clipPointBottom
	clipPointBottom[0].x = 150.0f;
	clipPointBottom[0].y = 150.0f;

	clipPointBottom[1].x = 450.0f;
	clipPointBottom[1].y = 150.0f;

	// store clipPointTop
	clipPointTop[0].x = 150.0f;
	clipPointTop[0].y = 450.0f;

	clipPointTop[1].x = 450.0f;
	clipPointTop[1].y = 450.0f;

	glEnable(GL_LINE_STIPPLE);

	// dashed 1 is Left line
	glLineStipple(1, 0x00FF);
	OneLine(clipPointLeft[0].x, clipPointLeft[0].y, clipPointLeft[1].x, clipPointLeft[1].y);

	// dashed 2 is Right line
	glLineStipple(1, 0x00FF);
	OneLine(clipPointRight[0].x, clipPointRight[0].y, clipPointRight[1].x, clipPointRight[1].y);

	// dashed 4 is Bottom line
	glLineStipple(1, 0x00FF);
	OneLine(clipPointBottom[0].x, clipPointBottom[0].y, clipPointBottom[1].x, clipPointBottom[1].y);

	// dashed 3 is Top line
	glLineStipple(1, 0x00FF);
	OneLine(clipPointTop[0].x, clipPointTop[0].y, clipPointTop[1].x, clipPointTop[1].y);

	glDisable(GL_LINE_STIPPLE);
	//glFlush();
	
}

void drawPolygon(int x1, int y1, int x2, int y2) {

	//Draw a polygon that has no self-intersection 
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();

	//glutSwapBuffers();
}
void writePixel(int x,int y) {
	//cout << "Draw Pixel X =" << x <<"   Y ="<< y << endl;
	
	//glPointSize(3);
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	//Change position y
	//glVertex2i(x, clipmax-y);

	glVertex2i(x, y);
	glEnd();
}
void myMouseFunc(int button, int state, int x, int y)
{
	//Change position mouse
	int Y = height - y;
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && countClick == 0) {
		points[index].x = x;
		points[index].y = Y;
		//points[index].y = y;
		cout << x << " " << y << endl;
		cout <<"Point index "<<index<<" = "<< points[index].x << "," << points[index].y << " "<<endl;

		//update
		countClick++;
		index++;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && countClick == 1) {
		points[index].x = x;
		points[index].y = Y;
		//points[index].y = y;

		cout << "Point index " << index << " = " << points[index].x << "," << points[index].y << endl;
		//Change position mouse
		//drawPolygon(points[0].x, clipmax-points[0].y, points[1].x, clipmax -points[1].y);
		//drawPolygon(points[index - 1].x, clipmax - points[index - 1].y, points[index].x, clipmax - points[index].y);

		drawPolygon(points[0].x,  points[0].y, points[1].x,  points[1].y);
		drawPolygon(points[index - 1].x,  points[index - 1].y, points[index].x,  points[index].y);

		list->addSorted(points[0], points[1]);
		//update
		countClick++;
		index++;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && countClick > 1) {

		points[index].x = x;
		points[index].y = Y;
		//points[index].y = y;

		cout << "Point index " << index << " = " << points[index].x << "," << points[index].y << endl;
		//Change position mouse
		//drawPolygon(points[index - 1].x, clipmax - points[index - 1].y, points[index].x, clipmax - points[index].y);

		drawPolygon(points[index - 1].x,  points[index - 1].y, points[index].x, points[index].y);
		list->addSorted(points[index-1], points[index]);
		
		//update
		countClick++;
		index++;
	}
	
	glutPostRedisplay();
}

void draw() {
	clippingRectangular();
	
	glFlush();
}
void setDefualt() {
	int multiPic = 20;
	points[0].x = 2* multiPic;
	//points[0].y = 600-(3 * 10);
	points[0].y = 3* multiPic;

	points[1].x = 7* multiPic;
	//points[1].y = 600-(1 * 10);
	points[1].y = 1* multiPic;

	points[2].x = 13* multiPic;
	//points[2].y = 600 - (5 * 10);
	points[2].y = 5 * multiPic;

	points[3].x = 13* multiPic;
	//points[3].y = 600 - (11 * 10);
	points[3].y = 11* multiPic;

	points[4].x = 7* multiPic;
	//points[4].y = 600 - (7*10);
	points[4].y = 7* multiPic;

	points[5].x = 2* multiPic;
	//points[5].y = 600 - (9*10);
	points[5].y = 9* multiPic;

	index = 6;
	cout << "Points-------------------->" << endl;
	for (int i = 0; i < index; i++) {
		cout << "Point " << i + 1 << " : " << " X = " << points[i].x << " Y = " << points[i].y << endl;
	}
	cout << "-------------------------->" << endl << endl;

	list->addSorted(points[0], points[1]);
	list->addSorted(points[1], points[2]);
	list->addSorted(points[2], points[3]);
	list->addSorted(points[3], points[4]);
	list->addSorted(points[4], points[5]);
	list->addSorted(points[5], points[0]);

	drawPolygon(points[0].x, points[0].y, points[1].x, points[1].y);
	drawPolygon(points[1].x, points[1].y, points[2].x, points[2].y);
	drawPolygon(points[2].x, points[2].y, points[3].x, points[3].y);
	drawPolygon(points[3].x, points[3].y, points[4].x, points[4].y);
	drawPolygon(points[4].x, points[4].y, points[5].x, points[5].y);
	drawPolygon(points[5].x, points[5].y, points[0].x, points[0].y);

	//list->print();
	glutPostRedisplay();
}
void fillPolygon() {

	cout<<"fillPolygon"<< endl;

	int ymin = 600;
	int ymax = 0;
	int xmin = 0;
	int xmax = 0;
	float m = 0;
	
	int count = 0;
	//calulate Y min index
	for (int i = 0; i < index; i++) {
		if (points[i].y < ymin) {
			ymin = points[i].y;
		}
	}
	//calculate Y max index
	for (int i = 0; i < index; i++) {
		if (points[i].y > ymax) {
			ymax = points[i].y;
		}
	}

	cout << "Ymin = " << ymin << endl;
	cout << "Ymax = " << ymax << endl << endl;
	
	//AET
	Linkedlist2* list2 = new Linkedlist2();
	Linkedlist2* listTemp = new Linkedlist2();
	//Linkedlist2* result = new Linkedlist2();
	//list->print();
	Node * tempET;
	Node * tempAET;
	Node * tail;
	Node * tempForDel;

	for (int i = ymin; i <= ymax; i++) {
		
		tempET = list->getEdgeTable(i);
		//cout << "tempET = " << tempET->xMin << endl;

		//Move ET to AET
		while (tempET != NULL) {
			list2->addSorted(tempET);
			tempET = tempET->next;
		}

		//update y at x == x
		list2->sortedXAtY();

		cout << endl;
		cout << "Scanline Y = " << i << endl;
		list2->print();


		tempAET = list2->getHeadAET();
		tail = tempAET->next;
		//cout << "tempAET = " << tempAET->yMax << endl;
		//cout << "tempAET = " << tail->yMax << endl;
		//list2->removeNode(tempAET->yMax);
		//list2->print();

		//Fill Pixel
		while ((tempAET != NULL) && (tail != NULL)) {
			//check scanline y == yMax
			tempET = list->getEdgeTable(i);
			//cout << "tempET = " << tempET << endl;
			//cout << "tempAET = " << tempAET << endl;
			if (tail->next != NULL) {
				if (tail->next->next == NULL) {
					if (((tempET->yMax == tempAET->yMax) && (tempET->xMin == tempAET->xMin))) {
						tempAET = tempAET->next;
						tail = tail->next;
					}
					if (((tempET->yMax == tail->yMax) && (tempET->xMin == tail->xMin))) {
						//cout << "tempET = " << tempET << endl;
						tail = tail->next;
					}
				}
			}

	

			//Check type int or float
			//Ceil or Floor
			//float Left
			float left = tempAET->xMin;				
			float right = tail->xMin;

			if (floor(left) != tempAET->xMin) {
				float a = tempAET->xMin;
				xmin = (int)ceil(a);
				cout << "Xmin = " << xmin << endl;
			}
			//int Left
			else {
				float a = tempAET->xMin;
				xmin = (int)a;				
				cout << "Xmin = " << xmin << endl;
			}

			//float Right
			if (floor(right) != tail->xMin) {
				float a = tail->xMin;					
				xmax = (int)floor(a);
				cout << "Xmax = " << xmax << endl;
			}
			//int Right
			else {
				float a = tail->xMin;					
				xmax = (int)(a - 1);
				cout << "Xmax = " << xmax << endl;
			}

		
			cout << "x at [" << xmin << " , " << xmax << "]" << endl;
			//Send to fill color
			for (int j = xmin; j <= xmax; j++) {
				writePixel(j, i);
			}
			//
			if (tail->next !=NULL) {
				if (tail->next->next != NULL) {
					tempAET = tail->next;
					tail = tempAET->next;
				}
				else {
					break;
				}
			}
			else if (tail->next == NULL) {
				break;
			}
			
			
		}
		
		//Remove Y=Ymax
		tempAET = list2->getHeadAET();
		while (tempAET != NULL) {
			if (i == tempAET->yMax) {
				tempForDel = tempAET;
				tempAET = tempAET->next;
				list2->removeNode(tempForDel->yMax);

			}
			else {
				tempAET = tempAET->next;
			}
		}
		
		////Update X
		tempAET = list2->getHeadAET();
		while (tempAET != NULL) {
			m = (tempAET->m.x) / (tempAET->m.y);
			tempAET->xMin = (tempAET->xMin)+m;

			tempAET = tempAET->next;
		}

	}
	glutPostRedisplay();
}

void Resized(int W, int H) {
	glViewport(0, 0, W, H);
}

void selectMenu(int val) {

	//Draw
	if (val == 1) {
		//Change position mouse
		//drawPolygon(points[index-1].x, clipmax-points[index-1].y, points[0].x, clipmax-points[0].y);

		drawPolygon(points[index - 1].x,  points[index - 1].y, points[0].x, points[0].y);

		list->addSorted(points[index-1], points[0]);
		//cout << "Point index " << index << " = " << points[index-1].x << "," << points[index-1].y << " " << endl;
		glutPostRedisplay();
	}

	//PrintLinkedlist
	if (val == 2) {
		list->print();
	}

	//Fill colors
	if (val == 3) {
		fillPolygon();
	}

	//setDefuat
	if (val == 4) {
		setDefualt();
	}
}

void create_menu() {
	glutCreateMenu(selectMenu);
	glutAddMenuEntry("Draw last point", 1);
	glutAddMenuEntry("print Linkedlist", 2);
	glutAddMenuEntry("Fill Polygon", 3);
	glutAddMenuEntry("Defualt Polygon", 4);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void Init(void) {
	//glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	gluOrtho2D(clipmin, clipmax, clipmin, clipmax); //0,600,0,600
}
int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(width, height);
	glutCreateWindow(name);
	glutDisplayFunc(draw);
	glutMouseFunc(myMouseFunc);
	glutReshapeFunc(Resized);
	create_menu();
	Init();
	glutMainLoop();
	return 0;
}
