#include"common.h"

int gbps_WindowHeight=GBPS_WINDOW_HEIGHT,
gbps_WindowWidth=GBPS_WINDOW_WIDTH;
long highscore=0;
int gbps_GamePlaying;
GLuint GBPS_texArr[20];
GLuint tex_app;
GLMmodel* car1;
GLMmodel* tyre1;
void gbps_PrintText(float x, float y, float r, float g, float b, void* font, char *string)
{
  glColor3f( r, g, b );
  glRasterPos2f(x, y);
  int len, i;
  len = (int)strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}
