#include <string>
#include <stdlib.h>
#include <GL/glut.h>
#include <cmath>

struct RgbColor
{
  float r;
  float g;
  float b;
};

struct Planet
{
  float x = 0;
  float y = 0;
  int satellites = 0;
  int rings = 0;
  float rotation_time = 0;
  float translation_time = 0;
  float orbit_radius = 0;
  RgbColor *color;
};

struct PolarCoordinate
{
  float x;
  float y;
};

PolarCoordinate convertToPolarCoordinate(float radius, float angle)
{
  PolarCoordinate coordinate;
  coordinate.x = radius * cos(angle);
  coordinate.y = radius * sin(angle);

  return coordinate;
}

// Calcula os pontos da orbita com base em coordenadas polares
void drawOrbit(GLfloat radius)
{
  float deltaTheta = 0.1f;

  glBegin(GL_LINE_LOOP);

  glColor3f(1.0f, 1.0f, 1.0f);

  for (float angle = 0; angle < 2 * M_PI; angle += deltaTheta)
  {
    PolarCoordinate coordinate = convertToPolarCoordinate(radius, angle);
    glVertex2f(coordinate.x, coordinate.y);
  }
  glEnd();
}

void draw()
{
  glClear(GL_COLOR_BUFFER_BIT);
  float radius = 150;

  for (int i = 0; i < 8; i++)
  {

    drawOrbit(radius);
    radius += 50;
  }
  glFlush();
}

void adjustWindow(GLsizei width, GLsizei height)
{

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
  gluOrtho2D(-width, width, -height, height);
}

void initialize()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(int argc, char *argv[])
{

  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  glutInitWindowPosition(100, 100);

  glutInitWindowSize(800, 800);

  glutCreateWindow("Sistema Solar");

  glutReshapeFunc(adjustWindow);

  glutDisplayFunc(draw);

  initialize();

  glutMainLoop();

  return 0;
}