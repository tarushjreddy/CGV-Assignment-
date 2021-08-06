#include <gl/glut.h>
#include <gl/gl.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <math.h>

int time_option=1;
bool dissappear = false;
char buff[200]={0};

const float clockR    = 80.0f,
            clockVol  = 100.0f,

            angle1min = M_PI / 30.0f,

            minStart  = 0.9f,
            minEnd    = 1.0f,

            stepStart = 0.8f,
            stepEnd   = 1.0f;

float angleHour = 0,
      angleMin  = 0,
      angleSec  = 0;



void newLine(float rStart, float rEnd, float angle)
{
    float c = cos(angle), s = sin(angle);
    glVertex2f( clockR*rStart*c, clockR*rStart*s);
    glVertex2f( clockR*rEnd*c, clockR*rEnd*s);
}

void renderbitmap(float x, float y, void *font, char *string)
{
    char *c;
    glRasterPos2f(x,y);
    for(c=string;*c !='\0';c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void digital()
{
    struct timeb tb;
    time_t tim = time(0);
    struct tm* t;
    glColor3f(1.f, 1.f, 1.f);

    if (time_option==1)
    {
        t = localtime(&tim);
        sprintf(buff, "TIME : %2d Hr %2d Min %2d Sec", t->tm_hour, t->tm_min, t->tm_sec);
        renderbitmap(-45,40,GLUT_BITMAP_TIMES_ROMAN_24,buff);
    }
    else if (time_option==2)
    {
        t = gmtime(&tim);
        sprintf(buff, "TIME : %2d Hr %2d Min %2d Sec", t->tm_hour, t->tm_min, t->tm_sec);
        renderbitmap(-45,40,GLUT_BITMAP_TIMES_ROMAN_24,buff);
    }
    else if (time_option==3)
    {
        t = gmtime(&tim);
        sprintf(buff, "TIME : %2d Hr %2d Min %2d Sec", t->tm_hour-7, t->tm_min, t->tm_sec);
        renderbitmap(-45,40,GLUT_BITMAP_TIMES_ROMAN_24,buff);

    }

}


void display(void)
{
   int i;
   float theta;
   glClear(GL_COLOR_BUFFER_BIT);

   if(!dissappear)
   {

          glColor3f(1.0f, 0.0f, 0.0f);
          glLineWidth(5.0f);
          glBegin(GL_LINES);
          for(int i=0; i<360; i++)
          {
                theta = i*3.142/180;
                glVertex2f(85*cos(theta),85*sin(theta));
          }
          glEnd();
          glFlush();

          glBegin(GL_LINES);
          for(int i=0; i<360; i++)
          {
                theta = i*3.142/180;
                glVertex2f(60*cos(theta),60*sin(theta));
          }
          glEnd();
          glFlush();


          glLineWidth(2.0f);
          glEnable(GL_LINE_SMOOTH);
          glEnable(GL_POINT_SMOOTH);
          glEnable(GL_POLYGON_SMOOTH);

          glBegin(GL_LINES);
            for(i=0; i<60; i++)
                {
                if(i%5)
                { // normal minute
                    if(i%5 == 1)
                        glColor3f(1.0f, 0.0f, 0.0f);
                    newLine(minStart, minEnd, i*angle1min);
                }
            else
                {
                glColor3f(0.0f, 0.0f, 1.0f);
                newLine(stepStart, stepEnd, i*angle1min);
              }
            }

          glEnd();

          glLineWidth(2.5f);
          glColor3f(1.0f, 0.0f, 0.0f);
          glBegin(GL_LINES);
            newLine(0.0f, 0.5f, -angleHour+M_PI/2);
            newLine(0.0f, 0.8f, -angleMin+M_PI/2);
          glEnd();

          glLineWidth(1.0f);
          glBegin(GL_LINES);
            newLine(0.0f, 0.8f, -angleSec+M_PI/2);
          glEnd();

     }
     else
     {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(-55, 60);    //left edge upper point
        glVertex2f(-55, 25);   //left edge lower point
        glVertex2f(60, 60);     //right edge upper point
        glVertex2f(60, 25);    //right edge lower point
        glVertex2f(-55, 25);   //bottom edge left point
        glVertex2f(60, 25);    //bottom edge right point
        glVertex2f(-55, 60);    //upper edge left point
        glVertex2f(60, 60);     //upper edge right point
        glEnd();
        digital();
     }

    glutSwapBuffers();

}

void init(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


// Function for displaying the clock appropriately even when the window is resized
void ChangeSize(GLsizei w, GLsizei h)
{

    GLfloat aspectRatio;

        // Prevent a divide by zero
    if(h == 0)
    h = 1;

    // View port to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Establish clipping volume
    aspectRatio = (GLfloat)w / (GLfloat)h;
    if (w <= h)
    glOrtho(-clockVol, clockVol, -clockVol / aspectRatio, clockVol / aspectRatio, 1.0, -1.0);
    else
    glOrtho(-clockVol * aspectRatio, clockVol * aspectRatio, -clockVol, clockVol, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void TimerFunction(int value)
{
    struct timeb tb;
    time_t tim=time(0);
    struct tm* t;

    if (time_option==1)
    {
        t = localtime(&tim);
        ftime(&tb);
        angleSec = (float)(t->tm_sec+ (float)tb.millitm/1000.0f)/30.0f * M_PI;
        angleMin = (float)(t->tm_min)/30.0f * M_PI + angleSec/60.0f;
        angleHour = (float)(t->tm_hour > 12 ? t->tm_hour-12 : t->tm_hour)/6.0f * M_PI+angleMin/12.0f;
    }
    else if (time_option==2)
    {
        t = gmtime(&tim);
        ftime(&tb);
        angleSec = (float)(t->tm_sec+ (float)tb.millitm/1000.0f)/30.0f * M_PI;
        angleMin = (float)(t->tm_min)/30.0f * M_PI + angleSec/60.0f;
        angleHour = (float)(t->tm_hour > 12 ? t->tm_hour-12 : t->tm_hour)/6.0f * M_PI+angleMin/12.0f;
    }
    else if (time_option==3)
    {
        t = gmtime(&tim);
        ftime(&tb);
        angleSec = (float)(t->tm_sec+ (float)tb.millitm/1000.0f)/30.0f * M_PI;
        angleMin = (float)(t->tm_min)/30.0f * M_PI + angleSec/60.0f;
        angleHour = (float)(t->tm_hour-7 > 12 ? t->tm_hour-12-7 : t->tm_hour-7)/6.0f * M_PI+angleMin/12.0f;

    }
    glutPostRedisplay();
    glutTimerFunc(33,TimerFunction, 1);

}

void timezone_opt(int option)
{
    if(option == 1)
    {
        time_option = 1;
    }
    else if(option == 2)
    {
        time_option = 2;
    }
    else if(option == 3)
    {
        time_option = 3;
    }
    glutPostRedisplay();

}

void optionsMenu(int option)
{
    if(option==2)
    {
        if(!dissappear)
            dissappear = true;
    }
    else if(option==1)
    {
        dissappear = false;
    }
    glutPostRedisplay();

}


int main(int argc, char* argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(350,100);
    glutInitWindowSize(650,650);
    glutCreateWindow("Analog-Digital Clock");

    glutDisplayFunc(display);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(33, TimerFunction, 1);

    int timezone_menu = glutCreateMenu(timezone_opt);
    glutAddMenuEntry("IST",1);
    glutAddMenuEntry("GMT",2);
    glutAddMenuEntry("PST",3);


    glutCreateMenu(optionsMenu);
    glutAddMenuEntry("Analog Clock",1);
    glutAddMenuEntry("Digital Clock",2);
    glutAddSubMenu("Time Zone",timezone_menu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    init();

    glutMainLoop();

    return 0;

}
