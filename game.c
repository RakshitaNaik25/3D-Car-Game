#include"game.h"
#include <math.h>
#include "mainmenu.h"
//keyhandling
void UpdateSideObjects();
void render_paused_overlay();
int gbps_rtLtKey=0;
int gbps_upDownKey=0;
int gbps_TotalOpponentCars=7;
float pos;
int game_over=0;
float gbps_camDist;
int gbps_aziy;
int gbps_elex;
float gbps_camUp;
long gbps_locct;
long gbps_lsoct;
long gbps_score;
long elapsed_time, total_time;
gbpsLinkedList SideObjects;
int nitro_active=0;
float nitro_fuel=100.0f;
float nitro_boost=1.0f;
double gbps_road_loop;
Gbps_Car PlayerCar;
gbpsLinkedList OpponentCars;

Gbps_CameraMode gbps_CurrentCameraMode = CAM_CHASE;
float target_camDist, target_aziy, target_elex, target_camUp;
float cur_camDist, cur_aziy, cur_elex, cur_camUp;

void gbps_EnterGame()
{
    game_over=0;
    gbps_GamePlaying=2;//game playing
    glutSpecialFunc(gbps_GameSpecialKey);
    glutSpecialUpFunc(gbps_GameSpecialKeyUp);
    glutKeyboardFunc(gbps_GameAsciiKey);
    glutKeyboardUpFunc(gbps_GameAsciiKeyUp);

    glutMouseFunc(gbps_GameMouse);
    glutMotionFunc(gbps_GameMouseMove);
    glutMouseWheelFunc(gbps_GameMouseWheel);

    glutDisplayFunc(gbps_RenderGame);


    gbps_road_loop=1.0;

    //set the camera position
    gbps_CurrentCameraMode = CAM_CHASE;
    gbps_aziy = 0;
    gbps_elex = 21;
    gbps_camDist = 10;
    gbps_camUp = -2;

    target_aziy = gbps_aziy;
    target_elex = gbps_elex;
    target_camDist = gbps_camDist;
    target_camUp = gbps_camUp;

    cur_aziy = gbps_aziy;
    cur_elex = gbps_elex;
    cur_camDist = gbps_camDist;
    cur_camUp = gbps_camUp;

    game_changeCamera(0,0);

    total_time=0;


    //initialzie the player car
    //set a texture for player car
    PlayerCar.CarTexture=GBPS_TEXTURE_CAR1;
    PlayerCar.vel=1;
    PlayerCar.acc=0.03;//zero acceleration at first
    PlayerCar.x=0;
    PlayerCar.y=0;//player car is at the origin
    PlayerCar.isPlayer=1; // Set as player
    PlayerCar.TyreRot=0;
    PlayerCar.rot=0;
    gbps_score=0;

    //initialize opponent cars linked list
    OpponentCars.Head=OpponentCars.Tail=NULL;
    OpponentCars.NoItems=0;
    gbps_locct=gbps_lsoct=total_time;
    nitro_active=0;
    nitro_fuel=100.0f;
    nitro_boost=1.0f;

    gbps_GameLoop(0);

}

void gbps_LeaveGame()
{
    gbps_GamePlaying=0;//game not playing
    gbps_StopAllSounds(); // Stop all audio when leaving game
    
    glutSpecialFunc(NULL);
    glutSpecialUpFunc(NULL);
    glutKeyboardFunc(NULL);

    glutMouseFunc(NULL);
    glutMotionFunc(NULL);
    glutMouseWheelFunc(NULL);

    //delete all the cars and objects
    while(OpponentCars.NoItems>0)
    {
        free((Gbps_Car*)PopItem(&OpponentCars));
    }
    while(SideObjects.NoItems>0)
    {
        free((GBPS_Tree*)PopItem(&SideObjects));
    }

}
void gbps_GameSpecialKey(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        PlayerCar.vel+=1;
        gbps_upDownKey=1;

        break;
    case GLUT_KEY_DOWN:
        PlayerCar.vel-=0.5;
        gbps_upDownKey=-1;

        break;
    case GLUT_KEY_LEFT:
        gbps_rtLtKey=-1;

        break;
    case GLUT_KEY_RIGHT:
        gbps_rtLtKey=1;
        break;
    }
}
void gbps_GameSpecialKeyUp(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        PlayerCar.vel-=1;
        gbps_upDownKey=0;
        break;
    case GLUT_KEY_DOWN:
        PlayerCar.vel+=0.5;
        gbps_upDownKey=0;
        break;
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
        gbps_rtLtKey=0;
        break;
    }
}
void gbps_GameAsciiKey(unsigned char key, int x, int y)
{
    if(game_over==1)
    {
        if(key==13)
        {
             gbps_LeaveGame();
             gbps_EnterMainMenu();
        }
        return;
    }
    switch (key)
    {
    case 27:
        //TODO: open a sub menu
        break;
    case 'w':
        gbps_camUp-=0.1;
        game_changeCamera(0,0);
        break;
    case 's':
        gbps_camUp+=0.1;
        game_changeCamera(0,0);
        break;
    case 'p':
    case ' ':
        if (gbps_GamePlaying == 2) {
            gbps_GamePlaying = 1;
            gbps_PauseAllSounds();
        } else if (gbps_GamePlaying == 1) {
            gbps_GamePlaying = 2;
            gbps_ResumeAllSounds();
        }
        break;
    case 'n':
    case 'N':
        nitro_active = 1;
        break;
    case 'c':
    case 'C':
        gbps_CurrentCameraMode = (gbps_CurrentCameraMode + 1) % CAM_COUNT;
        // Reset some defaults if needed when switching
        if (gbps_CurrentCameraMode != CAM_ORBIT) {
            target_aziy = 0;
            target_camUp = -2;
        }
        break;
    default:
        break;

    }
}

void gbps_GameAsciiKeyUp(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'n':
    case 'N':
        nitro_active = 0;
        break;
    }
}

void gbps_GameMouse(int button,int state,int x,int y)
{
    //nothing to do here
}
void gbps_GameMouseWheel(int button, int dir, int x, int y)
{
    if (dir > 0)
    {
        gbps_camDist--;
        game_changeCamera(0,0);
    }
    else
    {
        gbps_camDist++;
        game_changeCamera(0,0);
    }

    return;
}
void gbps_GameMouseMove(int x,int y)
{
    game_changeCamera(x,y);
}


// Here is the function
void glutPrint(float x, float y, LPVOID font, char* text, float r, float g, float b, float a)
{
    if(!text || !strlen(text)) return;
    glColor4f(r/255.0f, g/255.0f, b/255.0f, a); // Convert 0-255 to 0.0-1.0
    glRasterPos2f(x, y);
    while (*text)
    {
        glutBitmapCharacter(font, *text);
        text++;
    }
}


void RenderScore()
{
    char sc[100];
    float margin = 20.0f;
    float line_height = 25.0f;

    // Use bright yellow/white for readability
    sprintf(sc, "HIGH SCORE: %ld", highscore);
    // Align to top-right corner
    glutPrint(gbps_WindowWidth - 180, gbps_WindowHeight - margin, GLUT_BITMAP_HELVETICA_18, sc, 255, 255, 0, 1.0);

    sprintf(sc, "SCORE: %ld", gbps_score);
    glutPrint(gbps_WindowWidth - 180, gbps_WindowHeight - margin - line_height, GLUT_BITMAP_HELVETICA_18, sc, 255, 255, 255, 1.0);
}

void gbps_RenderGame()
{
    glLoadIdentity();
    int i;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    // Dynamic FOV for speed effect
    float target_fov = 45.0f + (nitro_boost - 1.0f) * 40.0f; 
    gluPerspective(target_fov, (float)gbps_WindowWidth/gbps_WindowHeight, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);

    // Interpolate camera values for smoothness
    float lerp_speed = 0.15f * (elapsed_time / 16.66f); // Normalize to 60fps
    if (lerp_speed > 1.0f) lerp_speed = 1.0f;

    // Camera Mode Logic
    switch(gbps_CurrentCameraMode) {
        case CAM_CHASE:
            target_camDist = 10.0f;
            target_elex = 21.0f;
            target_aziy = 0.0f;
            target_camUp = -2.0f;
            break;
        case CAM_TOP_DOWN:
            target_camDist = 25.0f;
            target_elex = 90.0f;
            target_aziy = 0.0f;
            target_camUp = 0.0f;
            break;
        case CAM_HOOD:
            target_camDist = 1.5f;
            target_elex = 5.0f;
            target_aziy = 0.0f;
            target_camUp = -1.2f;
            break;
        case CAM_DRIVE:
            target_camDist = 4.0f;
            target_elex = 10.0f;
            target_aziy = 0.0f;
            target_camUp = -1.5f;
            break;
        case CAM_ORBIT:
            // Mouse controlled, target reflects last mouse interaction
            break;
    }

    cur_camDist += (target_camDist - cur_camDist) * lerp_speed;
    cur_elex += (target_elex - cur_elex) * lerp_speed;
    cur_aziy += (target_aziy - cur_aziy) * lerp_speed; // Note: wrapping might be needed for full 360
    cur_camUp += (target_camUp - cur_camUp) * lerp_speed;

    gbps_camDist = cur_camDist;
    gbps_elex = (int)cur_elex;
    gbps_aziy = (int)cur_aziy;

    glTranslated(0.0, cur_camUp, -cur_camDist);//dist
    //glRotated(GBPS_twi, 0.0, 0.0, 1.0);//twist at line of sight
    glRotated(cur_elex, 1.0, 0.0, 0.0);//elivation angle from x axis yz plane
    glRotated(cur_aziy, 0.0, 1.0, 0.0);//azimuth angle from y axis xy plane


    glMatrixMode(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glColor3f(1,1,1);



    GLUquadricObj *qObj = gluNewQuadric();
    gluQuadricNormals(qObj, GLU_SMOOTH);
    gluQuadricTexture(qObj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, GBPS_texArr[GBPS_TEXTURE_SKY]);
    glPushMatrix();
    glRotatef(180.0,1.0,0,0);
    gluSphere(qObj, 80.0f, 24, 24);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //Draw road


    glPushMatrix();
    glEnable(GL_TEXTURE_2D);

    glTranslatef(0,gbps_road_loop,0);
    glBindTexture(GL_TEXTURE_2D, GBPS_texArr[GBPS_TEXTURE_ROAD]);

    glBegin(GL_QUADS);

    // glNormal3d(0,1,0);
    glColor3f(1,1,1);

    glTexCoord2f(0.0, 0.0);
    glVertex3d(4,0,-200);

    glTexCoord2f(1.0, 0.0);
    glVertex3d(-4,0,-200);

    glTexCoord2f(1.0, 4.0);
    glVertex3d(-4,0,200);

    glTexCoord2f(0.0, 4.0);
    glVertex3d(4,0,200);

    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);


    glBindTexture(GL_TEXTURE_2D, GBPS_texArr[GBPS_TEXTURE_SURR]);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0,0.0);
    glVertex3f(-100.0,-0.3,-1000.0);
    glTexCoord2d(0.0,50.0);
    glVertex3f(-100.0,-0.3,1000.0);
    glTexCoord2d(10.0,50.0);
    glVertex3f(100.0,-0.3,1000.0);
    glTexCoord2d(10.0,0.0);
    glVertex3f(100.0,-0.3,-1000.0);
    //glTranslatef(0.0,0.0,a);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    // DEBUG_drwCord();


    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // DEBUG_drwCord();
    ListItem* CurrentTree=SideObjects.Tail;
    for(i=0; i<SideObjects.NoItems; i++)
    {
        RenderTree(*((GBPS_Tree*)CurrentTree->Data));
        CurrentTree=CurrentTree->Prev;
    }
    RenderCar(PlayerCar);
    ListItem* CurrentCar=OpponentCars.Head;

    // //printf("Rendering total %d opponent cars\n",OpponentCars.NoItems);
    for(i=0; i<OpponentCars.NoItems; i++)
    {
        RenderCar(*((Gbps_Car*)CurrentCar->Data));
        CurrentCar=CurrentCar->Next;
    }
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopMatrix(); // End 3D scene rendering

    // ==========================================
    // 2D HUD OVERLAY PASS (ORTHOGRAPHIC)
    // ==========================================
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, gbps_WindowWidth, 0, gbps_WindowHeight);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    RenderScore();
    render_speedometer();

    if(game_over==1)
    {
        // For game over, we can use a centered 2D quad
        render_game_over();
    }
    else if(gbps_GamePlaying == 1)
    {
        render_paused_overlay();
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_DEPTH_TEST);
    glutSwapBuffers();
}

void UpdateGameLogic()
{
    //calculate the elapsed time since lasat update
    elapsed_time=GBPS_nf;//elapsed seconds
    total_time+=elapsed_time;

    //printf("elapsed time %d and total time=%d\n",elapsed_time,total_time);
    UpdateOpponentCars();
    UpdateSideObjects();
    UpdatePlayerCar();

    //collision detection code here
    //out car is 2 unit wide and 4.5 unit in length
    ListItem* CurrentCar=OpponentCars.Head;
    int i;
    for(i=0; i<OpponentCars.NoItems; i++)
    {
        Gbps_Car* CurCar=(Gbps_Car*)CurrentCar->Data;
        if(fabs(CurCar->x - PlayerCar.x) < 2)
        {
            if(fabs(CurCar->y - PlayerCar.y) < 4.5)
            {
                //collision
                //printf("%f and %f and %f and %f\n\n\n",CurCar->x, PlayerCar.x,CurCar->y , PlayerCar.y);
                gbps_PlayCrashSound();  // Play crash sound
                gbps_GamePlaying=1;
                game_over=1;
                PlayerCar.vel=0;
                gbps_PlayGameOverSound();  // Play game over sound
                if(gbps_score > highscore)
                {
                    highscore=gbps_score;
                    FILE* file=fopen("score.txt","wb");
                    fwrite(&gbps_score,sizeof(long),1,file);
                    fclose(file);
                }
                break;
            }
        }
        CurrentCar=CurrentCar->Next;
    }

}
void UpdatePlayerCar()
{
    ////printf("intial vel %f new vel %f\n",PlayerCar.vel,PlayerCar.vel+PlayerCar.acc*(elapsed_time/1000.0));
    PlayerCar.vel+=PlayerCar.acc*(elapsed_time/1000.0);
    
    // Update engine sound based on car speed
    gbps_UpdateEngineSound(PlayerCar.vel);
    
    //we dont move the car forward, we move the road backward

    // Nitro Logic
    if(nitro_active && nitro_fuel > 0) {
        nitro_boost += 0.05f;
        if(nitro_boost > 1.8f) nitro_boost = 1.8f;
        nitro_fuel -= 0.5f;
        if(nitro_fuel < 0) nitro_fuel = 0;
    } else {
        nitro_boost -= 0.02f;
        if(nitro_boost < 1.0f) nitro_boost = 1.0f;
        if(!nitro_active) nitro_fuel += 0.1f; // Slow recharge
        if(nitro_fuel > 100.0f) nitro_fuel = 100.0f;
    }

    float dist=PlayerCar.vel*nitro_boost*(elapsed_time/1000.0);
    ////printf("Increase in distance %f\n",dist);
    //calculate rotation of tyre
    gbps_score+=(int)(dist*100);

    PlayerCar.TyreRot+=((dist)/(2.0*3.1415*.01))*360.0;
    while(PlayerCar.TyreRot>360.0)
        PlayerCar.TyreRot-=360.0;
    // //printf("Rotation=%f - %f\n",PlayerCar.TyreRot,dist);
    gbps_road_loop-=dist;

    if (gbps_road_loop < 0.0)gbps_road_loop = (1+gbps_road_loop);


    if((gbps_rtLtKey)&&(fabs(PlayerCar.x)<3))
    {
        float side_speed = 9.0f * (elapsed_time/1000.0f); // Adjust for 60fps
        PlayerCar.x += gbps_rtLtKey * side_speed;
        PlayerCar.rot += gbps_rtLtKey * (120.0f * (elapsed_time/1000.0f));

        if(fabs(PlayerCar.rot)>MAX_CAR_ROT) PlayerCar.rot=((PlayerCar.rot<0)?-1:1)*MAX_CAR_ROT;
        if(fabs(PlayerCar.x)>MAX_CAR_SIDE_POS) PlayerCar.x=((PlayerCar.x<0)?-1:1)*(MAX_CAR_SIDE_POS-0.01);
    }
    //auto torate back
    if((!gbps_rtLtKey)&&(fabs(PlayerCar.rot)>0.1))
    {
        PlayerCar.rot+=((PlayerCar.rot>0)?-1:1)*(120.0f * (elapsed_time/1000.0f));
        if(fabs(PlayerCar.rot)<2)PlayerCar.rot=0;
    }

}

void UpdateSideObjects()
{
    int i;
    if(total_time-gbps_lsoct > 20)//at least 500 ms seconds interval between creating objects
    {
        //probability of creating a new object is 1 out of 2
        if( (rand()%2) < 1)
        {
            gbps_lsoct=total_time;
            GBPS_Tree* obj=malloc(sizeof(GBPS_Tree));  // Fixed: correct size
            AddItem(&SideObjects,(void*)obj);
            //set the position and other parameters of the new object

            if(rand()%2 ==0)
                obj->x=15+(sin(rand()%90*3.14/180)*30);//right side
            else
                obj->x=-15-(sin(rand()%90*3.14/180)*30);//right side
            obj->y=200;//always start at 200 :)
        }
    }
    //update all the object to new position
    ListItem* CurrentObject=SideObjects.Head;
    for(i=0; i<SideObjects.NoItems; i++)
    {
        //update the tree position
        float dist;
        dist=PlayerCar.vel*elapsed_time/15.0;
        ((GBPS_Tree*)CurrentObject->Data)->y-=fabs(dist);//new position
        CurrentObject=CurrentObject->Next;
    }

    //remove the cars that are too back
    while(SideObjects.NoItems>0 &&  ((GBPS_Tree*)(SideObjects.Head)->Data)->y < -100)
    {
        free((GBPS_Tree*)PopItem(&SideObjects));
    }
}

void UpdateOpponentCars()
{
    int i;
    //see if there are enough opponent cars. If not, Create them

    if(OpponentCars.NoItems < gbps_TotalOpponentCars)
    {
        if(total_time-gbps_locct > 2000)//at least 2 seconds interval between creating cars
        {
            srand(time(0));
            if( (rand()%gbps_TotalOpponentCars) <= (gbps_TotalOpponentCars-OpponentCars.NoItems))
            {
                gbps_locct=total_time;
                Gbps_Car* nc=malloc(sizeof(Gbps_Car));
                AddItem(&OpponentCars,(void*)nc);
                nc->x=((rand()%6)-3);//any value from -3 to +3
                nc->y=100;//always start at 100 :)
                nc->vel=(1.0*PlayerCar.vel)/2.0; // Half speed for more challenge
                nc->acc=0;
                nc->CarTexture=GBPS_TEXTURE_CAR1;
                nc->rot=nc->TyreRot=0;
                nc->isPlayer=0; // Set as not player
                i=rand()%2;
                switch(i)
                {
                case 0: nc->type=CAR_CONSTANT_VEL; break;
                case 1: nc->type=CAR_CONSTANT_VEL_MOVING; nc->state1=(nc->x > 0)?0:1; break;
                }
            }
        }
    }
    //update all the cars to new position
    ListItem* CurrentCar=OpponentCars.Head;
    for(i=0; i<OpponentCars.NoItems; i++)
    {
        //update the car position blah blah blah
        float dist;
        ((Gbps_Car*)CurrentCar->Data)->vel+=((Gbps_Car*)CurrentCar->Data)->acc*(elapsed_time/1000.0);
        dist=(PlayerCar.vel-((Gbps_Car*)CurrentCar->Data)->vel)*(elapsed_time/15.0);//distance is calculated by computing relative velocity
        ((Gbps_Car*)CurrentCar->Data)->y-=fabs(dist);//new position
        //printf("New position %f , %f , %f\n\n",((Gbps_Car*)CurrentCar->Data)->y,PlayerCar.vel,((Gbps_Car*)CurrentCar->Data)->vel);
        //calculate rotation of tyre
        ((Gbps_Car*)CurrentCar->Data)->TyreRot+=((fabs(dist))/(2.0*3.1415*.04))*360.0;
        while(((Gbps_Car*)CurrentCar->Data)->TyreRot>360.0)
            ((Gbps_Car*)CurrentCar->Data)->TyreRot-=360.0;
        //getch();
        if(((Gbps_Car*)CurrentCar->Data)->type==CAR_CONSTANT_VEL_MOVING)
        {
            //getch();
            if(((Gbps_Car*)CurrentCar->Data)->state1==0)
            {
                float side_move = 6.3f * (elapsed_time/1000.0f); // Adjust for 60fps
                ((Gbps_Car*)CurrentCar->Data)->x -= side_move;
                if(((Gbps_Car*)CurrentCar->Data)->x < -2.7)
                    ((Gbps_Car*)CurrentCar->Data)->state1=1;
                //printf("decreasing....\n\n\n\n");
            }
            else
            {
                float side_move = 6.3f * (elapsed_time/1000.0f); // Adjust for 60fps
                ((Gbps_Car*)CurrentCar->Data)->x += side_move;
                if(((Gbps_Car*)CurrentCar->Data)->x >2.7)
                    ((Gbps_Car*)CurrentCar->Data)->state1=0;
            }
        }
        CurrentCar=CurrentCar->Next;
    }

    //remove the cars that are too back
    while(OpponentCars.NoItems>0 &&  ((Gbps_Car*)(OpponentCars.Head)->Data)->y < -100)
    {
        free((Gbps_Car*)PopItem(&OpponentCars));
    }
}

void gbps_GameLoop(int val)
{
    if(gbps_GamePlaying==2)
    {
        //update game logic
        UpdateGameLogic();
        glutPostRedisplay();
        glutTimerFunc(GBPS_nf,gbps_GameLoop,0);
    }
    else if(gbps_GamePlaying == 1)//pause
    {
        glutPostRedisplay();
        glutTimerFunc(GBPS_nf,gbps_GameLoop,0);
    }
}

void game_changeCamera(int x,int y)
{
    static int prevx,prevy;
    int difx=x-prevx;
    int dify=y-prevy;
    prevx=x;
    prevy=y;

    if (gbps_CurrentCameraMode == CAM_ORBIT) {
        if (difx>0) target_aziy++;
        else if (difx<0) target_aziy--;
        
        if (dify>0) target_elex++;
        else if (dify<0) target_elex--;

        if (target_elex>90) target_elex=90;
        if (target_elex<5) target_elex=5;
    }

    if (difx == 0 && dify == 0) { // Mouse wheel or internal update
        if (target_camDist<5) target_camDist=5;
        if (target_camDist>30) target_camDist=30;
    }
}

void render_paused_overlay()
{
    // Semi-transparent black overlay
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(gbps_WindowWidth, 0);
        glVertex2f(gbps_WindowWidth, gbps_WindowHeight);
        glVertex2f(0, gbps_WindowHeight);
    glEnd();

    // "PAUSED" text
    char* text = "PAUSED";
    float text_width = 120.0f; // Approximate
    glutPrint(gbps_WindowWidth/2 - text_width/2, gbps_WindowHeight/2, GLUT_BITMAP_TIMES_ROMAN_24, text, 255, 255, 255, 1.0f);
    
    char* subtext = "Press SPACE to Resume";
    glutPrint(gbps_WindowWidth/2 - 100, gbps_WindowHeight/2 - 30, GLUT_BITMAP_HELVETICA_12, subtext, 200, 200, 200, 1.0f);
}
