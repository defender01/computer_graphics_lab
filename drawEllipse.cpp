/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include<math.h>

static int slices = 16;
static int stacks = 16;
int flag1 = 0,flag2=0, flag3=0, x00,y00,x11=-500,y11=400,radius=40,addx1=1,addy1=1,addx2=-1,addy2=-1,mass1=1,mass2=1;
/* GLUT callback Handlers */

static void resize(int width, int height){
    const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-320, 319, -240, 239, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

int calculateHypotenous(int dx,int dy){
    return sqrt((dx*dx + dy*dy));
}

void mouse(int button, int state, int mousex, int mousey)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        int mx = mousex;
        int my = mousey;
        //printf(" c1 %d %d\n",mx-320,-my+240);
        glutPostRedisplay();
        if(flag1==0){
            flag1=1;
            x00 = mx-320;
            y00 = -my+240;
        }
        else{
            //flag1=0;
            flag3 = 1;
            x11 = mx-320;
            y11 = -my+240;
            addx1 = (x11 - x00);
            addy1 = (y11 - y00);
            int d = calculateHypotenous(addx1,addy1);
            addx1 = 2*(addx1)/d;
            addy1 = 2*(addy1)/d;
            //printf("%d %d\n",x11,y11);
        }

        // else if(flag2==0){
        //     flag2=1;
        //     x11 = mx-320;
        //     y11 = -my+240;
        // }
    }
}

void myMouseMove( int mousex, int mousey)
{
int mx = mousex;
int my = mousey;
if(flag3 ==0){
    flag2 = 1;
    x11 = mx-320;
    y11 = -my+240;
//printf("%d %d\n",tempX,tempY);
}

glutPostRedisplay();
}


void draw8way(int x,int y,int cx, int cy){
            glVertex2i(x+cx, y+cy);
            glVertex2i(y+cx, x+cy);
            glVertex2i(-y+cx, x+cy);
            glVertex2i(-x+cx, y+cy);
            glVertex2i(-x+cx, -y+cy);
            glVertex2i(-y+cx, -x+cy);
            glVertex2i(y+cx, -x+cy);
            glVertex2i(x+cx, -y+cy);
    }

void draw4way(int x,int y,int cx, int cy){
            glVertex2i(x+cx, y+cy);
            glVertex2i(x+cx, -y+cy);
            glVertex2i(-x+cx, y+cy);
            glVertex2i(-x+cx, -y+cy);
    }

void drawCircle1(int a,int b,int r){
    int x=0,y=r, d=5-4*r;
    draw8way(x,y,a,b);
    while(x<y){
        if(d<0){
            x++;
            d+= 8*x + 12;
        }
        else{
            x++,y--;
            d+= 8*x - 8*y + 20;
        }
        draw8way(x,y,a,b);        
    }
    while(x<y){
        if(d<0){
            x++;
            d+= 8*x + 12;
        }
        else{
            x++,y--;
            d+= 8*x - 8*y + 20;
        }
        draw8way(x,y,a,b);        
    }
}
void drawEllipse(int cx, int cy, int a, int b){
    int x=0, y=b, d=4*b*b-4*a*a*b+a*a;
    draw4way(x,y,cx,cy);
    while(a*a*(2*y-1)>2*b*b*(x+1)){
        if(d<0){//dE            
            x++;
            d+=4*b*b*(2*x+3);
        }
        else{//dSE        
            x++,y--;
            d+=4*(b*b*(2*x+3)+a*a*(-2*y+2));
        }
        draw4way(x,y,cx,cy);        
    }
    while(y>0){
        if(d<0){//dSE            
            x++,y--;
            d+=4*(b*b*(2*x+2)+a*a*(-2*y+3));
        }
        else{            
            y--;
            d+=4*a*a*(-2*y+3);
        }
        draw4way(x,y,cx,cy);        
    }
}


bool collisionDetected(int x0,int y0,int x1,int y1){
    return ((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) < 4*radius*radius);
}
int calculateRadius(int x0,int y0,int x1,int y1){
    return sqrt((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0));
}



int a= 60, b=60;
int sx=0, sy=240-b,ex=0, ey= -(240-b), timeVal=0, mx=10000000;
bool down=1,up=0,circle =1;

static void display(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    // glBegin(GL_LINES);
    // glVertex2i(-320, 0);
    // glVertex2i(319, 0);
    // glVertex2i(0, -240);
    // glVertex2i(0, 239);
    // glEnd();



    glBegin(GL_POINTS);

   
    if(down){
        if(a!=b && !circle){
            b++;
            sy--;
        }
        else if(abs(-240-sy)==b && b>20){
            b--;
         }
         else circle=1;

        if(abs(-240-sy)==20){
            circle=0;
            down=0;
            up=1;
            }
        else sy--;
    }
    else{
        if(a!=b && !circle){
            b++;
            sy++;
        }
        else if(abs(240-sy)==b && b>20){
            b--;
        }
        else circle=1;

        if(abs(240-sy)==20){
            circle=0;
            down=1;
            up=0;
            }
        else sy++;
        
    }    

    drawEllipse(0,sy,a,b);

    if(down)timeVal++;
    else timeVal--;
    for(int j=0; j<mx-timeVal*70000;j++)
        {}


    glEnd();
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Experiment 01");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc( myMouseMove);
    glutMainLoop();



    return EXIT_SUCCESS;
}