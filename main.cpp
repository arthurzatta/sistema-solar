#include <string>
#include <stdlib.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <array>
#include <functional>

using namespace std;

struct RgbColor
{
  float r;
  float g;
  float b;
};

struct Planet
{
  std::string name = "";
  float x = 0;
  float y = 0;
  int satellites = 0;
  int rings = 0;
  float rotation_time = 0;
  float translation_time = 0;
  float radius = 0;
  RgbColor color;
};

struct PolarCoordinate
{
  float x;
  float y;
};

float planet_angles[8] = {};
float planet_position[8] = {};
float planet_radius[8] = {8, 7, 16, 14, 19, 15, 15, 15};
int qtd_rings[8] = {0, 0, 0, 0, 1, 4, 2, 1};
int qtd_satellites[8] = {0, 0, 1, 2, 1, 1, 1, 2};
float rotation_time[8] = {};
float translation_time[8] = {87, 225, 365, 687, 1200, 3000, 8400, 1640};

PolarCoordinate
convertToPolarCoordinate(float radius, float angle)
{
  PolarCoordinate coordinate;
  coordinate.x = radius * cos(angle);
  coordinate.y = radius * sin(angle);

  return coordinate;
}

void drawCircle(GLfloat radius, GLenum mode)
{

  float deltaTheta = 0.01f;

  glBegin(mode);

  for (float angle = 0; angle < 2 * M_PI; angle += deltaTheta)
  {
    PolarCoordinate coordinate = convertToPolarCoordinate(radius, angle);
    glVertex2f(coordinate.x, coordinate.y);
  }

  glEnd();
}

void drawOrbit(GLfloat radius)
{
  glColor3f(1.0f, 1.0f, 1.0f);
  glLineWidth(1.0f);
  drawCircle(radius, GL_LINE_LOOP);
}

void drawSun(GLfloat radius)
{
  glColor3f(255.0f, 255.0f, 0.0f);
  drawCircle(radius, GL_POLYGON);
}

void drawPlanet(GLfloat planet_radius, RgbColor color, int qtd_rings, int qtd_satellites, int *satellites)
{
  float ring_radius = planet_radius + 10;

  // Desenha os aneis
  if (qtd_rings > 0)
  {

    for (int i = 0; i < qtd_rings; i++)
    {
      glColor3f(1.0f, 1.0f, 1.0f);
      drawCircle(ring_radius, GL_LINE_LOOP);

      ring_radius += 10;
    }
  }

  ring_radius = planet_radius + 10;

  // Desenha os satelites
  for (int j = 0; j < qtd_satellites; j++)
  {

    PolarCoordinate coordinate = convertToPolarCoordinate(ring_radius, 1);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(coordinate.x, coordinate.y, 0);
    drawCircle(6, GL_POLYGON);
    glTranslatef(-coordinate.x, -coordinate.y, 0);

    if (satellites[j] > 1)
    {
      PolarCoordinate coordinate = convertToPolarCoordinate(ring_radius, 180);

      glColor3f(1.0f, 1.0f, 1.0f);
      glTranslatef(coordinate.x, coordinate.y, 0);
      drawCircle(6, GL_POLYGON);
      glTranslatef(-coordinate.x, -coordinate.y, 0);
    }
  }

  glColor3f(color.r, color.g, color.b);
  drawCircle(planet_radius, GL_POLYGON);
}

void drawPlanetInPosition(float position, float angle, float planet_radius, RgbColor color, int qtd_rings, int qtd_satellites, int *satellites_rings)
{
  PolarCoordinate coordinate = convertToPolarCoordinate(position, angle);
  glTranslatef(coordinate.x, coordinate.y, 0);

  drawPlanet(planet_radius, color, qtd_rings, qtd_satellites, satellites_rings);

  glTranslatef(-coordinate.x, -coordinate.y, 0);
}

void draw()
{

  float radius = 130;
  float angle = 0;

  glViewport(0, 0, 1000, 1000);

  glClear(GL_COLOR_BUFFER_BIT);

  drawSun(80);

  for (int i = 0; i < 8; i++)
  {
    drawOrbit(radius);

    planet_position[i] = radius;

    radius += 100;
  }

  drawPlanetInPosition(planet_position[0], planet_angles[0], planet_radius[0], {237.0f, 125.0f, 49.0f}, qtd_rings[0], qtd_satellites[0], {});

  drawPlanetInPosition(planet_position[1], planet_angles[1], planet_radius[1], {132.0f, 60.0f, 12.0f}, qtd_rings[1], qtd_satellites[1], {});

  int earth_sat[] = {1};
  drawPlanetInPosition(planet_position[2], planet_angles[2], planet_radius[2], {0.0f, 112.0f, 192.0f}, qtd_rings[2], qtd_satellites[2], earth_sat);

  int mars_sat[] = {0, 2};
  drawPlanetInPosition(planet_position[3], planet_angles[3], planet_radius[3], {1.0f, 0.0f, 0.0f}, qtd_rings[3], qtd_satellites[3], mars_sat);

  int jup_sat[] = {1};
  drawPlanetInPosition(planet_position[4], planet_angles[4], planet_radius[4], {255.0f, 192.0f, 0.0f}, qtd_rings[4], qtd_satellites[4], mars_sat);

  int saturn_sat[] = {0, 1, 0, 0};
  drawPlanetInPosition(planet_position[5], planet_angles[5], planet_radius[5], {191.0f, 144.0f, 0.0f}, qtd_rings[5], qtd_satellites[5], saturn_sat);

  int urano_sat[] = {0, 1};
  drawPlanetInPosition(planet_position[6], planet_angles[6], planet_radius[6], {84.0f, 130.0f, 53.0f}, qtd_rings[6], qtd_satellites[6], saturn_sat);

  int netuno_sat[] = {2};
  drawPlanetInPosition(planet_position[7], planet_angles[7], planet_radius[7], {180.0f, 199.0f, 231.0f}, qtd_rings[7], qtd_satellites[7], netuno_sat);

  glutSwapBuffers();
}

void translationMercury(int angle_index)
{
  planet_angles[0] += 0.1;
  glutPostRedisplay();
  glutTimerFunc(translation_time[0], translationMercury, 1);
}

void translationVenus(int angle_index)
{
  planet_angles[1] += 0.1;
  glutPostRedisplay();
  glutTimerFunc(translation_time[1], translationVenus, 1);
}
void translationEarth(int angle_index)
{
  planet_angles[2] += 0.1;
  glutPostRedisplay();
  glutTimerFunc(translation_time[2], translationEarth, 1);
}
void translationMars(int angle_index)
{
  planet_angles[3] += 0.1;
  glutPostRedisplay();
  glutTimerFunc(translation_time[3], translationMars, 1);
}
void translationJupyter(int angle_index)
{
  planet_angles[4] += 0.1;
  glutPostRedisplay();
  glutTimerFunc(translation_time[4], translationJupyter, 1);
}
void translationSaturn(int angle_index)
{
  planet_angles[5] += 0.1;
  glutPostRedisplay();
  glutTimerFunc(translation_time[5], translationSaturn, 1);
}

void translationUranus(int angle_index)
{
  planet_angles[6] += 0.1;
  glutPostRedisplay();
  glutTimerFunc(translation_time[6], translationUranus, 1);
}

void translationNeptuno(int angle_index)
{
  planet_angles[7] += 0.1;
  glutPostRedisplay();
  glutTimerFunc(translation_time[7], translationNeptuno, 1);
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

  glutInitWindowSize(1000, 1000);

  glutCreateWindow("Sistema Solar");

  glutReshapeFunc(adjustWindow);

  glutDisplayFunc(draw);

  translationMercury(1);
  translationVenus(1);
  translationEarth(1);
  translationMars(1);
  translationJupyter(1);
  translationSaturn(1);
  translationUranus(1);
  translationNeptuno(1);

  initialize();

  glutMainLoop();

  return 0;
}