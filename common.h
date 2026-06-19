#ifndef GBPS_COMMON_H
#define GBPS_COMMON_H
/*
common.h
includes all the necessary global header files and constant definitions. All the c files may include this common header.
It also contains declaration for some common functions
copyright (c) 2011 GBPS Team
*/
#include<stdio.h>
#include<GL/freeglut.h>
#include "SOIL.h"
#include <stdlib.h>
#include<time.h>
#include"glm.h"
#include"audio.h"

#define GBPS_WINDOW_HEIGHT 480
#define GBPS_WINDOW_WIDTH 640

#define GBPS_fps 60
#define GBPS_nf 1000/GBPS_fps

//float car_crash_angle=0.0;
extern int gbps_GamePlaying;//is the game play on? 0-off, 1-pause, 2-running 3-Game over
//textures
extern GLuint GBPS_texArr[20];//no of textures required

//models
extern GLuint tex_app;
extern GLMmodel* car1 ;
extern GLMmodel* tyre1;
//extern float car_crash_angle=0.0;
enum gbps_Textures{
    GBPS_TEXTURE_CAR1=0,
    GBPS_TEXTURE_ROAD,
    GBPS_TEXTURE_TYRE,
    GBPS_TEXTURE_MENUBACK,
    GBPS_TEXTURE_MENU1,
    GBPS_TEXTURE_MENU2,
    GBPS_intro,
    GBPS_TEXTURE_SURR,
    GBPS_TEXTURE_SKY,
    GBPS_TEXTURE_TREE,
    GBPS_TEXTURE_SPEED,
    GBPS_TEXTURE_PTR,
    GBPS_TEXTURE_GO};

typedef enum {
    CAM_CHASE = 0,
    CAM_TOP_DOWN,
    CAM_HOOD,
    CAM_DRIVE,
    CAM_ORBIT,
    CAM_COUNT
} Gbps_CameraMode;

extern Gbps_CameraMode gbps_CurrentCameraMode;
extern float target_camDist, target_aziy, target_elex, target_camUp;
extern float cur_camDist, cur_aziy, cur_elex, cur_camUp;
extern long highscore;
extern int gbps_WindowHeight,gbps_WindowWidth;

void gbps_PrintText(float x, float y, float r, float g, float b, void* font, char *string);
#endif
