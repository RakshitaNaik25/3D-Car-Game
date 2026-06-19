#ifndef GAME_H
#define GAME_H
#include"common.h"
#include"car.h"
#include"linkedlist.h"
#include"object.h"


extern float gbps_camDist;//distance of camera from the car
extern int gbps_aziy;//rotation about y axis
extern int gbps_elex;//elevation from the x axis
extern float gbps_camUp;//up camera
extern int gbps_rtLtKey;
extern int gbps_upDownKey;
extern int gbps_TotalOpponentCars;
extern int game_over;
extern long gbps_locct;//last opponent car created time
extern long gbps_lsoct;//last side object created time

extern long gbps_score;

extern long elapsed_time,total_time;//some time storing variables
extern int nitro_active;
extern float nitro_fuel;
extern float nitro_boost;

extern double gbps_road_loop;

extern Gbps_Car PlayerCar;
extern gbpsLinkedList OpponentCars;
extern gbpsLinkedList SideObjects;

void gbps_EnterGame();
void gbps_GameLoop(int val);
void gbps_LeaveGame();
void gbps_PauseGame();
void gbps_ResumeGame();
void gbps_RenderGame();
void UpdateGameLogic();
void UpdatePlayerCar();
void UpdateOpponentCars();

//keyboard and mouse handling functions
void gbps_GameSpecialKey(int key, int x, int y);
void gbps_GameSpecialKeyUp(int key, int x, int y);
void gbps_GameAsciiKey(unsigned char key, int x, int y);
void gbps_GameAsciiKeyUp(unsigned char key, int x, int y);
void gbps_GameMouse(int button,int state,int x,int y);
void gbps_GameMouseWheel(int button, int dir, int x, int y);
void gbps_GameMouseMove(int x,int y);


void game_changeCamera(int x,int y);


void DEBUG_drwCord(void);
#endif
