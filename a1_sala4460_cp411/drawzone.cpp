/*
CP411 A1
Author: Bruno Salapic
*/

// OpenGL and C headers
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// other include if you split the your design into several files, it is a good practice to do so


// C++ headers for file I/O
#include <fstream>
#include <iostream>
using namespace std;
#define GL_BGR_EXT 0x80E0

// function prototype
//#define some constant here
#define  MaxNumObj 100

// global variables
GLsizei winWidth = 640, winHeight = 480; // variables and initial for width and height of display window
GLint tool = 0, type = 1, style = 1, selected = -1, selection = 0, move = 0, holddown = 0,
xbegin, ybegin, ptCtr = 0, numObj = 0;
GLfloat red = 1.0, green = 0.0, blue = 0.0;

// data structures, you define the structures for the drawing objects, you can use the data structure given below
class modelObject {
public:
GLint x1, y1, x2, y2, t, s; // t is for types of object: 1 for rectangle, 2 for cicle; s for drawing styles: 1 for filled, 2 for outline
GLfloat r, g, b; // RGB color
} list[100]; // this is an array data structure, you can also use link list for storing objects

class Point { // testing for drawing a sequence of points
public:
GLint x, y;
GLfloat r, g, b; // RGB color
} points[100];

// function section
// initial function to set up OpenGL state variable other than default.
void init(void) {
glClearColor(1.0, 1.0, 1.0, 0.0); // Set display-window color to white
glMatrixMode(GL_PROJECTION);
gluOrtho2D(0.0, winWidth, winHeight, 0.0);
glColor3f(1.0, 0.0, 0.0);
glFlush();
}

/* Drawing a point for testing */
void setPixel(GLint x, GLint y) {
glPointSize(3.0);
glBegin(GL_POINTS);
glVertex2i(x, y);
glEnd();
}
void setRect( modelObject list){
	 glBegin(GL_LINE_LOOP);
	 glColor3f(list.r, list.g, list.b);
	            glVertex2i(list.x1, list.y1);
	            glVertex2i(list.x1, list.y2);
	            glVertex2i(list.x2, list.y2);
	            glVertex2i(list.x2, list.y1);
	            glEnd();
 }

void setRectfill (modelObject list){
	 glBegin(GL_QUADS);
	 glColor3f(list.r, list.g, list.b);
	            glVertex2i(list.x1,list.y1);
	            glVertex2i(list.x1,list.y2);
	            glVertex2i(list.x2, list.y2);
	            glVertex2i(list.x2, list.y1);
	            glEnd();
 }
void circlepoints (modelObject list){
	 glColor3f(list.r, list.g, list.b);
	int x3 = sqrt(((list.x2-list.x1)^2)+((list.y2-list.y1)^2));
	int y3 = 0;
	int radiuserror=1-x3;
	while (x3>=y3){
	setPixel(x3 + list.x1, y3 + list.y1);
	setPixel(y3 + list.x1, x3 + list.y1);
	setPixel(-x3 + list.x1, y3 + list.y1);
	setPixel(-y3 + list.x1, x3 + list.y1);
	setPixel(-x3 + list.x1, -y3 + list.y1);
	setPixel(-y3 + list.x1, -x3 + list.y1);
	setPixel(x3+ list.x1, -y3 + list.y1);
	setPixel(y3 + list.x1, -x3 + list.y1);

	y3++;
	if (radiuserror<0)
	{radiuserror+=2*y3+1;

	}
	else{
		x3--;
		radiuserror+=2*(y3-x3+1);
	}
}
}
void filledcircle(modelObject list ){
	glColor3f(list.r, list.g, list.b);
	GLint x3 = sqrt(((list.x2-list.x1)^2)+((list.y2-list.y1)^2));
		GLfloat angle;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(list.x1,list.y1);
	for (angle = 1.0f;angle <361.0f; angle +=0.2){
		list.x2=list.x1+sin(angle)*x3;
		list.y2=list.y1+cos(angle)*x3;
		glVertex2f(list.x2,list.y2);

	}
glEnd();
}


void Delete (GLint j)   // j is an index for the selected object
{	if (selection==1){
		GLint i;
   	   for ( i = j; i<numObj; i++){
   		   list[i].x1 = list[i+1].x1;
   		   list[i].y1 = list[i+1].y1;
   		   list[i].x2 = list[i+1].x2;
   		   list[i].y2 = list[i+1].y2;
   		   list[i].r = list[i+1].r;
   		   list[i].g = list[i+1].g;
   		   list[i].b = list[i+1].b;
   		   list[i].t = list[i+1].t;
   		   list[i].s = list[i+1].s;
   	   }
     numObj--;
    selection = 0;  // selection = 1 if an object is selected, 0 otherwise

}
}
void move2back(GLint j){
	if (selection==1){
		GLint i;
		modelObject temp=list[j];
		for(i=j;i>0;i--){
			list[i].x1 = list[i+1].x1;
			      list[i].y1 = list[i-1].y1;
			      list[i].x2 = list[i-1].x2;
			      list[i].y2 = list[i-1].y2;
			      list[i].r = list[i-1].r;
			      list[i].g = list[i-1].g;
			      list[i].b = list[i-1].b;
			      list[i].t = list[i-1].t;
			      list[i].s = list[i-1].s;
		}
		list[0]=temp;

	}
	selected=0;glutPostRedisplay();
}
void move2front(GLint j){
	if (selection==1){
		modelObject temp=list[j];
		Delete(j);
		numObj++;
		list[numObj]=temp;

	}
	selection=1;
	selected = numObj;
	glutPostRedisplay();

}


// this function draws the list of objects
void drawList() {
	GLint i;
	glClear(GL_COLOR_BUFFER_BIT);// Clear display window
	for ( i = 1;i <= numObj; i++){
		selected=i;
		if (list[i].t==1){
			if(list[i].s==1){
				setRectfill(list[i]);
			}
			else{
				setRect(list[i]);
			}
		}
		else{

			if (list[i].s==1){
				filledcircle(list[i]);
			}
			else{
				circlepoints(list[i]) ;
			}

		}
	}

}

void mouseDraw(GLint button, GLint action, GLint xMouse, GLint yMouse) {

	if (button == GLUT_LEFT_BUTTON && selection == 0){
		 if (action == GLUT_DOWN && button != GLUT_RIGHT_BUTTON && numObj < MaxNumObj -1) {
	         numObj++;
	         holddown=1;
	         list[numObj].x1 = xMouse;
	         list[numObj].y1 = yMouse;
	         list[numObj].x2 = xMouse;
	         list[numObj].y2 = yMouse;
	         list[numObj].t = type;
	         list[numObj].s = 2;
	         list[numObj].r = red;
	         list[numObj].g = green;
	         list[numObj].b = blue;

	                glutPostRedisplay();
	     } else if (action == GLUT_UP && button != GLUT_RIGHT_BUTTON && holddown == 1) {
	         holddown=0;
	         list[numObj].x2 = xMouse;
	         list[numObj].y2 = yMouse;
	         list[numObj].s = style;

	         glutPostRedisplay();
	     }
	}
    if (button == GLUT_LEFT_BUTTON && selection==1){
	    GLint i;
		GLint xnow=xMouse;
		GLint ynow=yMouse;
		if(move == 1 && selected > -1){

	    }else {
		   if (action == GLUT_DOWN && button != GLUT_RIGHT_BUTTON){
			   for( i = numObj ; i >= 0 ; i--){
				   if (list[i].t == 1){
						  if(xnow < max(list[i].x1,list[i].x2) && xnow > min(list[i].x1,list[i].x2) && ynow < max(list[i].y1,list[i].y2) && ynow < max(list[i].y1,list[i].y2)){
							 selected=i;
							 glutPostRedisplay();
							 break;
						  }

				   }else {
						if (sqrt(((list[i].x2-list[i].x1)^2)+((list[i].y2-list[i].y1)^2)) >= sqrt(((list[i].x2-xnow)^2)+((list[i].y2-ynow)^2))){
							selected=i;
						    glutPostRedisplay();
							break;
						}
				   }
			   }
		   }
	    }
	glFlush();
    }
}

void savingfile ( ) {
	FILE *outFile;
	outFile = fopen("output.xml", "w");
	char tmp[500];
	GLint i,xtop,ytop, objWidth, objHeight; //store calculated height and width

	//write header data for SVG
	sprintf(tmp, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
	fputs(tmp, outFile);
	sprintf(tmp, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n");
	fputs(tmp, outFile);
	sprintf(tmp, "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
	fputs(tmp, outFile);
	sprintf(
	       tmp,
	       "<svg width=\"%d\" height=\"%d\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n",
	            winWidth, winHeight);
	fputs(tmp, outFile);

	for (i=0;i<=numObj;i++){
	    if (list[i].t == 1) {
	        xtop=min(list[i].x1,list[i].x2);
	    	ytop=min(list[i].y1,list[i].y2);
	    	objWidth=max(list[i].x1,list[i].x2)-min(list[i].x1,list[i].x2);
	    	objHeight=max(list[i].y1,list[i].y2)-min(list[i].y1,list[i].y2);
	    	if (list[i].s == 1) {
	    	      sprintf(tmp,"<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(%d,%d,%d);stroke-width:1;stroke:rgb(%d,%d,%d)\"/>\n",xtop, ytop, objWidth, objHeight,(int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255),(int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255));
	    	      fputs(tmp, outFile);
	       }else {
	    	    sprintf(
	    	            tmp,
	    	            "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:none;stroke-width:1;stroke:rgb(%d,%d,%d)\"/>\n",
	    	             xtop, ytop, objWidth, objHeight,
	    	            	   (int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255));
	    	            	    fputs(tmp, outFile);}
	    }
	    else {
	    	 GLint rad;
	    	 rad=sqrt(((list[i].x2-list[i].x1)^2)+((list[i].y2-list[i].y1)^2));
	    	 xtop=list[i].x1;
	    	 ytop=list[i].y1;
	    	 if (list[i].s==1){
	    	     sprintf(
	    	     tmp,
	    	     "<circle cx=\"%d\" cy=\"%d\" rad=\"%d\" style=\"fill:rgb(%d,%d,%d);stroke-width:1;stroke:rgb(%d,%d,%d)\"/>\n",
	    	     xtop, ytop, rad,
	    	     (int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255),
	    	     (int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255));
	    	 }
	    	 else{
	    	      sprintf(
	    	      tmp,
	    	      "<circle cx=\"%d\" cy=\"%d\" rad=\"%d\" style=\"fill:rgb(%d,%d,%d);stroke-width:1;stroke:rgb(%d,%d,%d)\"/>\n",
	    	      xtop, ytop, rad,
	    	      (int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255));
	    	}
	    }
	  }
	  sprintf(tmp, "</svg>");
	  fputs(tmp, outFile);
	  fclose(outFile);
}

void callsvg(){
		FILE *in;
	    char int_chr;
	    char prev;
	    numObj = 0;
	    selection = 0;
	    move = 0;

	    glutPostRedisplay();

	    in = fopen("output.xml", "r");


	    do {
	            prev = int_chr;
	            int_chr = fgetc(in);

	            if (int_chr == 'r' && prev == '<') {
	                //parse rect

	                numObj++;
	                list[numObj].t = 1;


	    fscanf(in, "ect x=\" %d \" y=\" %d \" width=\" %d \" height=\" %d \" style=\"fill:",
	                        &list[numObj].x1, &list[numObj].y1, &list[numObj].x2,
	                        &list[numObj].y2);
	                list[numObj].x2 += list[numObj].x1;
	                list[numObj].y2 += list[numObj].y1;


	                if ((int_chr = fgetc(in)) == 'r') {
	                    fscanf(in, "gb( %f , %f , %f )", &list[numObj].r,
	                            &list[numObj].g, &list[numObj].b);
	                    list[numObj].s = 1;
	                } else {
	                    fscanf(in, "one;stroke-width:1;stroke:rgb( %f , %f , %f )",
	                            &list[numObj].r, &list[numObj].g, &list[numObj].b);
	                    list[numObj].s = 2;
	                }

	                //convert colors back to float
	                list[numObj].r = list[numObj].r / 255;
	                list[numObj].g = list[numObj].g / 255;
	                list[numObj].b = list[numObj].b / 255;


	            }
	            else if (int_chr=='c' && prev == '<'){
	            	GLint rad;
	            	numObj++;
	            	list[numObj].t=2;
	            	fscanf(in, "ircle x=\" %d \" y=\" %d \" rad=\" %d \"  style=\"fill:",
	            		                        &list[numObj].x1, &list[numObj].y1, &rad);
	            	list[numObj].x2+=list[numObj].x1+rad;
	            	list[numObj].y2+=list[numObj].y1;
	            	if((int_chr=fgetc(in))=='r'){
	            		fscanf(in, "gb( %f , %f , %f )",
	            				&list[numObj].r,
	                            &list[numObj].g, &list[numObj].b);
	                    list[numObj].s = 1;
	            	}
	            	else{
	            		fscanf(in, "one;stroke-width:1;stroke:rgb( %f , %f , %f )",
	            			    &list[numObj].r, &list[numObj].g, &list[numObj].b);
	            			     list[numObj].s = 2;
	            	}
	            	list[numObj].r = list[numObj].r / 255;
	            	list[numObj].g = list[numObj].g / 255;
	            	list[numObj].b = list[numObj].b / 255;

	            }

	    } while  (int_chr != EOF);

	        fclose(in);
	        glutPostRedisplay();
	    }
void SaveBitmap(const char *ptrcFileName, int nX, int nY, int nWidth, int nHeight)
{
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;

    unsigned char *ptrImage = (unsigned char*) malloc(sizeof(unsigned char)*nWidth*nHeight*3 + (4-(3*nWidth)%4)*nHeight);


     FILE *ptrFile = fopen(ptrcFileName, "wb");

        //read pixels from framebuffer
        glReadPixels(nX, nY, nWidth, nHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, ptrImage );


        // set memory buffer for bitmap header and informaiton header
         memset(&bf, 0, sizeof(bf));
        memset(&bi, 0, sizeof(bi));

       // configure the headers with the give parameters

bf.bfType = 0x4d42;
        bf.bfSize = sizeof(bf) + sizeof(bi) + nWidth*nHeight*3 + (4-(3*nWidth)%4)*nHeight;
        bf.bfOffBits = sizeof(bf) + sizeof(bi);
        bi.biSize = sizeof(bi);
        bi.biWidth = nWidth + nWidth%4;
        bi.biHeight = nHeight;
        bi.biPlanes = 1;
        bi.biBitCount = 24;
        bi.biSizeImage = nWidth*nHeight*3 + (4-(3*nWidth)%4)*nHeight;


    // to files
       fwrite(&bf, sizeof(bf), 1, ptrFile);
        fwrite(&bi, sizeof(bi), 1, ptrFile);
        fwrite(ptrImage, sizeof(unsigned char), nWidth*nHeight*3 + (4-(3*nWidth)%4)*nHeight, ptrFile);

        fclose(ptrFile);
        free(ptrImage);

}

// this function takes the mouse position while moving mouse, use this for intermediate drawing
void Motion(GLint x, GLint y) {
	if(holddown == 1){
	 list[numObj].x2 = x;
	   list[numObj].y2 = y;}
glutPostRedisplay();
glFlush();
}

// reshape function for resized the window
void winReshapeFcn(GLint newWidth, GLint newHeight) {
/* Reset viewport and projection parameters */
glViewport(0, 0, newWidth, newHeight);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0.0, GLdouble(newWidth), GLdouble(newHeight), 0.0);
/* Reset display-window size parameters. */
winWidth = newWidth;
winHeight = newHeight;
drawList();
glFlush();
}


void mainMenu (GLint menuOption)
{
   switch (menuOption) {
   case 1: numObj =0 ; selection = 0; move = 0; break;
   case 2: exit(0);
   }
   glutPostRedisplay ( );
}
void colorSubMenu (GLint colorOption){
     switch (colorOption) {
    case 1:
    red = 0.0;
    green = 0.0;
    blue = 1.0;


     break;
    case 2:
    red = 0.0;
    green = 1.0;
    blue = 0.0;

     break;
    case 3:
   red = 1.0;
   green = 0.0;
   blue = 0.0;
 break;
     }
    glutPostRedisplay ( );
}
void editSubMenu( GLint editoption){
	switch(editoption){
	case 1:
		selection = 1;
		break;
	case 2:
		selection=0;
		break;
	case 3:
		move2front(selected);
		break;
	case 4:
		move2back(selected);
		break;
	case 5:
		Delete(selected);
		break;
	}
	glutPostRedisplay();
}
void shapeSubMenu (GLint typeoption ){
     switch (typeoption) {
    case 1:
    type = 1; break;
    case 2:
    type = 2;

     }
    glutPostRedisplay ( );
}
void styleSubMenu (GLint styleoption )
{
     switch (styleoption) {
    case 1:
    style = 1; break;
    case 2:
    style = 2;

     }
    glutPostRedisplay ( );
}

void fileSubMenu(GLint fileoption){
	switch(fileoption){
	case 1:
		savingfile();break;
	case 2:
	callsvg();break;
	case 3:

		SaveBitmap("output.bmp",0,0,winWidth,winHeight);break;
	}
}

// main function
int main(int argc, char** argv) {
// declare submenu variables and set menu here

glutInit(&argc, argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
glutInitWindowPosition(100, 100);
glutInitWindowSize(winWidth, winHeight);
glutCreateWindow("SimpleDraw Sample for Assignment 1 (brunos program)");




init();

GLint color_subMenu;

	color_subMenu = glutCreateMenu (colorSubMenu);

	glutAddMenuEntry ("Red", 3);
	glutAddMenuEntry ("Green", 2);
	glutAddMenuEntry ("Blue", 1);
GLint files_subMenu;
files_subMenu = glutCreateMenu (fileSubMenu);
	glutAddMenuEntry (" savesvg ",1);
	glutAddMenuEntry (" import from svg ",2);
	glutAddMenuEntry (" export bitmap ",3);
GLint edit_subMenu;
	edit_subMenu= glutCreateMenu (editSubMenu);
	glutAddMenuEntry (" select ",1);
	glutAddMenuEntry (" deselect ",2);
	glutAddMenuEntry (" move to front ",3);
	glutAddMenuEntry (" move to back ",4);
	glutAddMenuEntry (" delete ",5);
GLint shape_subMenu;
	shape_subMenu= glutCreateMenu (shapeSubMenu);
	glutAddMenuEntry (" rectangle ",1);
	glutAddMenuEntry (" circle ",2);
	GLint style_subMenu;
	style_subMenu= glutCreateMenu (styleSubMenu);
	glutAddMenuEntry (" filled ",1);
	glutAddMenuEntry (" empty ",2);
	glutCreateMenu (mainMenu); // Create main pop-up menu.
	glutAddMenuEntry (" New ", 1);
	glutAddSubMenu ("edit",edit_subMenu);
	glutAddSubMenu (" Colors ", color_subMenu);
	glutAddSubMenu ("shape ",shape_subMenu);
	glutAddSubMenu ("style", style_subMenu);
	glutAddSubMenu ("files", files_subMenu);
	glutAddMenuEntry (" Quit", 2);






	glutAttachMenu (GLUT_RIGHT_BUTTON);

// register call back funtions
	glutDisplayFunc(drawList);
glutReshapeFunc(winReshapeFcn);
glutMouseFunc(mouseDraw);
glutMotionFunc(Motion);

glutAttachMenu (GLUT_RIGHT_BUTTON);

glutMainLoop();
}
