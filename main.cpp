#include <bits/stdc++.h>
#include <GL/glut.h>
#include <GL/gl.h>
#define COLUMNS 40
#define ROWS 40
#define FPS 10
#define UP 1
#define DOWN -1
#define RIGHT 2
#define LEFT -2
#define MAX 60
bool gameOver = false, gold = true, rock = true;
int goldX, goldY, rockX, rockY;
int shovel_len = 1, score = 0;
#define SHELLSCRIPT "\
#/bin/bash \n\
zenity --error --text='Try Again!' --title='GameOver' \n\
"
int axis = 0;
void DisplayCallback();         // to make swap to buffers cuz it not auto haha!
void ReshapeCallback(int, int); // to min widow or resize (operatons)
void initGrid(int, int);
void drawGrid();
void drawRock();
void TimerCallback(int);
void KeyboardCallback(int, int, int); // key pos of mouse time key pressed
void randomm(int &x, int &y);
//############# game ###############
int gridX, gridY;
short sDirection = RIGHT; // default deirection of shovel
int posX[60] = {20}, posY[60] = {20};

void initGrid(int x, int y)
{
  gridX = x;
  gridY = y;
}
void unit(int, int);

void drawGrid()
{
  for (int x = 0; x <= gridX; x++)
  {
    for (int y = 0; y <= gridY; y++)
    {
      unit(x, y);
    }
  }
}

void unit(int x, int y) // draw 4 squares to get grid
{
  if (x == 0 || y == 0 || x == gridX - 1 || y == gridY - 1) // the box at the left | bottom
  {
    glLineWidth(3.0);
    glColor3f(1.0, 0.0, 0.0);
  }
  else
  {
    glLineWidth(1.0);
    glColor3f(1, 0.50, 0.40);
  }

  glBegin(GL_LINE_LOOP); // draw closed figure
  glVertex2f(x, y);
  glVertex2f(x + 1, y);
  glVertex2f(x + 1, y + 1);
  glVertex2f(x, y + 1);
  glEnd();
}

void init() // for change the df color black
{
  glClearColor(1, 0.50, 0.40, 1.0);
  initGrid(COLUMNS, ROWS);
}

void Drawshovel()
{

  if (sDirection == UP)
  {
    posY[0]++;
    axis = 1;
  }
  else if (sDirection == DOWN)
  {
    posY[0]--;
    axis = 2;
  }
  else if (sDirection == RIGHT)
  {
    posX[0]++;
    axis = 3;
  }
  else if (sDirection == LEFT)
  {
    axis = 4;
    posX[0]--;
  }
  for (int i = 0; i < shovel_len; i++)
  {

    glColor3f(0.5, 0.5, 0.5);
    glRectd(posX[i], posY[i], posX[i] + 1.5, posY[i] + 2);

    if (axis == 1)
    {
      glColor3f(1, 0, 0);
      glRectd(posX[i] + 0.5, posY[i], posX[i] + 1, posY[i] - 4);
    }
    else if (axis == 3)
    {
      glColor3f(1, 0, 0);
      glRectd(posX[i], posY[i] + 0.6, posX[i] - 4, posY[i] + 1.2);
    }
    else if (axis == 4)
    {
      glRectd(posX[i], posY[i], posX[i] + 1.5, posY[i] + 2);
      glColor3f(1, 0, 0);
      glRectd(posX[i] + 1.5, posY[i] + 0.6, posX[i] + 5.5, posY[i] + 1.2);
    }
    else
    {
      glColor3f(1, 0, 0);

      glRectd(posX[i] + 0.5, posY[i] + 2, posX[i] + 1, posY[i] + 6);
    }
  }
  if (posX[0] == 0 || posX[0] == gridX - 1 || posY[0] == 0 || posY[0] == gridY - 1)
  {
    gameOver = true;
  }
  if (posX[0] == goldX && posY[0] == goldY)
  {
    score++;
    if (shovel_len > MAX)
    {
      shovel_len = MAX;
    }
    gold = true;
  }
}
void drawRock()
{
  if (rock)
  {
    randomm(rockX, rockY);
    printf("%d %d\t ", rockX, rockY);
  }
  rock = false;
  glColor3f(1, 0, 1.0);
  glRectf(rockX, rockY, rockX + 2, rockY + 2);
}
void Drawgold()
{
  if (gold)
  {
    randomm(goldX, goldY);
  }
  gold = false;
  glColor3f(1.0, 1.0, 0.0);
  glRectf(goldX, goldY, goldX + 1, goldY + 1);
}

void randomm(int &x, int &y)
{
  int maxX = gridX - 2, maxY = gridY - 2; // from 0 to 38
  int minn = 1;
  x = minn + rand() % (maxX - minn);
  y = minn + rand() % (maxY - minn);
}
//############# game ###################

//################### main #######################
int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // swap buffers (display && work)
  glutInitWindowPosition(500, 150);
  glutInitWindowSize(1024, 600);
  glutCreateWindow("SNEAKY!");
  glutDisplayFunc(DisplayCallback);
  glutReshapeFunc(ReshapeCallback);
  glutTimerFunc(0, TimerCallback, 0);
  glutSpecialFunc(KeyboardCallback); // keyboard
  init();
  glutMainLoop(); // all proccess will be start when window dispayed
  return 0;
}

void DisplayCallback()
{
  glClear(GL_COLOR_BUFFER_BIT); // clear df color
  drawGrid();                   // grid drawed in every frame
  for (int i = 0; i < 6; i++)
  {

    drawRock();
    rock = true;
  }
  rock = false;
  Drawshovel();
  Drawgold();
  glutSwapBuffers(); // black cuz it's the default !
  if (gameOver)
  {
    system(SHELLSCRIPT);
    exit(0);
  }
}

void ReshapeCallback(int weight, int hight) // hold area of our window
{
  glViewport(0, 0, (GLsizei)weight, (GLsizei)hight); // h and w of rectangle from 0.0 to the actually new hight after and before minimized
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                            // load identity cuz it not
  glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0); // to use 3d or 2d && -1 is like comig out of screen to your side and 1.0 is like going inside the screen
  glMatrixMode(GL_MODELVIEW);                  // cuz the model view will take part from modelview matrix
}

void TimerCallback(int)
{
  glutPostRedisplay();
  glutTimerFunc(1000 / FPS, TimerCallback, 0); // dispaly fun called 10 times in one sec to know some delay is adde (warning high fps if unknown val)
}
void KeyboardCallback(int key, int, int)
{
  if (key == GLUT_KEY_UP && key != DOWN)
  {
    sDirection = UP;
  }
  else if (key == GLUT_KEY_DOWN && key != UP)
  {
    sDirection = DOWN;
  }
  else if (key == GLUT_KEY_RIGHT && key != LEFT)
  {
    sDirection = RIGHT;
  }
  else if (key == GLUT_KEY_LEFT && key != RIGHT)
  {
    sDirection = LEFT;
  }
}
