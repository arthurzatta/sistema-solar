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

float planet_translate_angles[8] = {};
float planet_rotate_angles[8] = {};
float planet_position[8] = {};
float planet_radius[8] = {12, 16, 16, 14, 24, 22, 20, 18};
int qtd_rings[8] = {0, 0, 0, 0, 1, 4, 2, 1};
int qtd_satellites[8] = {0, 0, 1, 2, 1, 1, 1, 2};
float rotation_time[8] = {59, 243, 0.24, 0.2437, 0.0955, 0.1014, 0.17, 0.16};
float translation_time[8] = {87, 225, 365, 687, 1200, 3000, 8400, 1640};

bool show_orbit = true;
bool show_mercury = true;
bool show_venus = true;
bool show_earth = true;
bool show_mars = true;
bool show_jupiter = true;
bool show_saturn = true;
bool show_uranus = true;
bool show_neptune = true;
bool pause_simulation = false;

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
  glColor3f(0.98f, 0.93f, 0.19f);
  drawCircle(radius, GL_POLYGON);
}

void drawPlanet(GLfloat planet_radius, RgbColor color, int qtd_rings, int qtd_satellites, int *satellites)
{
  float ring_radius = planet_radius + 10;

  // Desenha os aneis
  if (qtd_rings > 0 && show_orbit)
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

    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(coordinate.x, coordinate.y, 0);
    drawCircle(6, GL_POLYGON);
    glPopMatrix();

    if (satellites[j] > 1)
    {
      PolarCoordinate coordinate = convertToPolarCoordinate(ring_radius, 180);

      glPushMatrix();
      glColor3f(1.0f, 1.0f, 1.0f);
      glTranslatef(coordinate.x, coordinate.y, 0);
      drawCircle(6, GL_POLYGON);
      glPopMatrix();
    }
  }

  glColor3f(color.r, color.g, color.b);
  drawCircle(planet_radius, GL_POLYGON);
}

void drawPlanetInPosition(float position, float angle, float rotate_angle, float planet_radius, RgbColor color, int qtd_rings, int qtd_satellites, int *satellites_rings)
{
  glPushMatrix();
  PolarCoordinate coordinate = convertToPolarCoordinate(position, angle);
  glTranslatef(coordinate.x, coordinate.y, 0);
  glRotatef(rotate_angle, 0.0f, 0.0f, 0.0f);

  drawPlanet(planet_radius, color, qtd_rings, qtd_satellites, satellites_rings);

  glPopMatrix();
}

void draw()
{

  float radius = 130;
  float angle = 0;

  glViewport(0, 0, 800, 600);

  glClear(GL_COLOR_BUFFER_BIT);

  drawSun(80);

  for (int i = 0; i < 8 && show_orbit; i++)
  {
    drawOrbit(radius);

    planet_position[i] = radius;

    radius += 70;
  }

  if(show_mercury)
    drawPlanetInPosition(planet_position[0], planet_translate_angles[0], planet_rotate_angles[0], planet_radius[0], {0.93f, 0.49f, 0.23f}, qtd_rings[0], qtd_satellites[0], {});
  
  if(show_venus)
    drawPlanetInPosition(planet_position[1], planet_translate_angles[1], planet_rotate_angles[1], planet_radius[1], {0.52f, 0.23f, 0.11f}, qtd_rings[1], qtd_satellites[1], {});

  int earth_sat[] = {1};
  if (show_earth)
    drawPlanetInPosition(planet_position[2], planet_translate_angles[2], planet_rotate_angles[2], planet_radius[2], {0.07f, 0.44f, 0.75f}, qtd_rings[2], qtd_satellites[2], earth_sat);

  int mars_sat[] = {0, 2};
  if (show_mars)
    drawPlanetInPosition(planet_position[3], planet_translate_angles[3], planet_rotate_angles[3], planet_radius[3], {0.92f, 0.27f, 0.24f}, qtd_rings[3], qtd_satellites[3], mars_sat);

  if (show_jupiter)
    drawPlanetInPosition(planet_position[4], planet_translate_angles[4], planet_rotate_angles[4], planet_radius[4], {0.97f, 0.76f, 0.22f}, qtd_rings[4], qtd_satellites[4], mars_sat);

  int saturn_sat[] = {0, 1, 0, 0};
  if (show_saturn)
    drawPlanetInPosition(planet_position[5], planet_translate_angles[5], planet_rotate_angles[5], planet_radius[5], {0.75f, 0.57f, 0.16f}, qtd_rings[5], qtd_satellites[5], saturn_sat);

  if (show_uranus)
    drawPlanetInPosition(planet_position[6], planet_translate_angles[6], planet_rotate_angles[6], planet_radius[6], {0.32f, 0.51f, 0.20f}, qtd_rings[6], qtd_satellites[6], saturn_sat);

  int netuno_sat[] = {2};
  if (show_neptune)
    drawPlanetInPosition(planet_position[7], planet_translate_angles[7], planet_rotate_angles[7], planet_radius[7], {0.71f, 0.78f, 0.90f}, qtd_rings[7], qtd_satellites[7], netuno_sat);

  glutSwapBuffers();
}

void translationPlanet(int angle_index)
{
  if (!pause_simulation)
  {
    planet_translate_angles[angle_index] += 0.1;
  }
  glutPostRedisplay();
  glutTimerFunc(translation_time[angle_index], translationPlanet, angle_index);
}

void rotatePlanet(int angle_index)
{
  if (!pause_simulation)
  {
    planet_rotate_angles[angle_index] += 0.1;
  }
  glutPostRedisplay();
  glutTimerFunc(rotation_time[angle_index], rotatePlanet, angle_index);
}

void adjustWindow(GLsizei width, GLsizei height)
{

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
  gluOrtho2D(-width, width, -height, height);
}

void getKeyboard(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    show_orbit = !show_orbit;
    break;
  case GLUT_KEY_DOWN:
    pause_simulation = !pause_simulation;
    break;
  }
}

void Keyboard(unsigned char key, int x, int y)
{
  if (key == 27)
    exit(0);
  if (key == 49)
    show_mercury = !show_mercury;
  if (key == 50)
    show_venus = !show_venus;
  if (key == 51)
    show_earth = !show_earth;
  if (key == 52)
    show_mars = !show_mars;
  if (key == 53)
    show_jupiter = !show_jupiter;
  if (key == 54)
    show_saturn = !show_saturn;
  if (key == 55)
    show_uranus = !show_uranus;
  if (key == 56)
    show_neptune = !show_neptune;
  if(key == 112)
    pause_simulation = !pause_simulation;
  if(key == 111)
    show_orbit = !show_orbit;
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

  for (int i = 0; i < 8; i++)
  {
    translationPlanet(i);
    rotatePlanet(i);
  }

  glutKeyboardFunc(Keyboard);

  initialize();

  glutMainLoop();

  return 0;
}