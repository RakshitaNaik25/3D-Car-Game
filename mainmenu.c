#include "mainmenu.h"
#include <math.h>
#include "game.h"
#include "audio.h" 

char* MenuOptions[]= {"PLAY RACE","SOUND: ON ","EXIT GAME"};
// MainMenuPos will store Y-coordinates for the buttons
float MainMenuPos[]= {1.0, -0.5, -2.0}; 
int initial_flag=1;
int start=1;
int i;
float alphaa=0.0;
int selection;
float menu_cur_pos; // Smooth animation tracker
int extreme_low, extreme_up;
float pulse_alpha = 0.0;
int pulse_dir = 1;
float target_menu_pos;

// Helper to draw a glassmorphism style box
void gbps_DrawGlassBox(float x, float y, float width, float height, int isSelected) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Background: Dark translucent gray
    if (isSelected) {
        glColor4f(0.1f, 0.15f, 0.2f, 0.85f); // Slightly clearer/brighter if selected
    } else {
        glColor4f(0.05f, 0.05f, 0.05f, 0.6f); // Darker, more transparent if not
    }
    
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
    
    // Border: Neon Glow logic
    glLineWidth(isSelected ? 3.0f : 1.0f);
    
    if (isSelected) {
        // Pulse effect for selected item
        float glow = 0.7f + (pulse_alpha * 0.3f);
        glColor4f(0.0f, 1.0f, 1.0f, glow); // Neon Cyan
    } else {
        glColor4f(0.3f, 0.3f, 0.3f, 0.5f); // Subtle gray border
    }
    
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
    
    glLineWidth(1.0f);
}

void update_pulse()
{
    if (pulse_dir == 1) {
        pulse_alpha += 0.03f; // Faster pulse
        if (pulse_alpha >= 1.0) pulse_dir = -1;
    } else {
        pulse_alpha -= 0.03f;
        if (pulse_alpha <= 0.0) pulse_dir = 1;
    }
}

void gbps_EnterMainMenu()
{
    selection=0;
    menu_cur_pos=MainMenuPos[selection];
    target_menu_pos=menu_cur_pos;
    gbps_PositionReset();
    glutDisplayFunc(gbps_RenderMainMenu);
    glutSpecialFunc(gbps_MainMenuSpecialKey);
    glutKeyboardFunc(gbps_MainMenuAsciiKey);
    gpbs_MainMenuLoop(0);
}

void gbps_LeaveMainMenu()
{
    glutSpecialFunc(NULL);
    glutKeyboardFunc(NULL);
}

void gbps_PositionReset()
{
    selection=0;
    menu_cur_pos=MainMenuPos[selection];
    target_menu_pos=menu_cur_pos;
    extreme_low=0;
    extreme_up=2; 
}

void gbps_mm_EnterEvents()
{
    if(start == 1)
    {
        alphaa = 1.1;
        initial_flag = 2;
        start = 2;
        return;
    }
    if (initial_flag < 2) return;

    gbps_PlayMenuSelectSound(); // Play select sound
    
    // Small delay or just execute
    if(selection==0) // Play
    {
        // Give a moment for the sound to start? No, instant is better.
        gbps_LeaveMainMenu();
        gbps_EnterGame();
    }
    else if(selection==1) // Sound
    {
        gbps_ToggleAudio();
    }
    else if(selection==2) // Exit
    {
        exit(0);
    }
}

void gbps_RenderMainMenu()
{
    update_pulse();
    
    if(initial_flag == 1) {
        alphaa += 0.02;
        if(alphaa >= 1.0) { initial_flag = 2; start = 2; }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Clean Projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(45, (float)gbps_WindowWidth/gbps_WindowHeight, 1.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // INTRO SCREEN
	if(start==1){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, GBPS_texArr[GBPS_intro]);
		glColor4f(1.0, 1.0, 1.0, alphaa);
		glBegin(GL_QUADS);
		    glTexCoord2f(0,0); glVertex2f(-5.5,-4.0);
		    glTexCoord2f(1,0); glVertex2f( 5.5,-4.0);
		    glTexCoord2f(1,1); glVertex2f( 5.5, 4.0);
		    glTexCoord2f(0,1); glVertex2f(-5.5, 4.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	else {
        // MAIN MENU BACKGROUND
        if (gbps_AudioEnabled) MenuOptions[1] = "SOUND: ON ";
        else MenuOptions[1] = "SOUND: OFF";

	    glEnable(GL_TEXTURE_2D);
	    glBindTexture(GL_TEXTURE_2D, GBPS_texArr[GBPS_TEXTURE_MENUBACK]);
        glColor4f(1.0, 1.0, 1.0, 1.0);
	    glBegin(GL_QUADS);
    		glTexCoord2f(0.0,0); glVertex2f(-10.0,-7.0);
    		glTexCoord2f(1.0,0); glVertex2f( 10.0,-7.0);
    		glTexCoord2f(1.0,1.0);glVertex2f( 10.0, 7.0);
    		glTexCoord2f(0.0,1.0);glVertex2f(-10.0, 7.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);

        // RENDER MENU ITEMS
        // Left align: x = -6.5
        float menuX = -6.5f;
        float boxWidth = 5.0f;
        
        for(i=0; i<3; i++) {
            float y = MainMenuPos[i];
            int isSelected = (selection == i);
            
            // Adjust box animation
            float currentWidth = boxWidth;
            float currentX = menuX;
            
            if (isSelected) {
                // Subtle pop out
                currentX += 0.2f;
                currentWidth += 0.2f;
            }

            // Draw Background Box
            gbps_DrawGlassBox(currentX, y - 0.2f, currentWidth, 1.0f, isSelected);

            // Draw Text
            // Color logic: Selected = White with Blue Glow, Unselected = Gray/White
            float r, g, b;
            void* fontToUse;
            
            if (i == 0) { // PLAY BUTTON (Hero)
                fontToUse = GLUT_BITMAP_TIMES_ROMAN_24; // Larger
                if (isSelected) {
                    r=1.0f; g=1.0f; b=1.0f; // White
                } else {
                    r=0.8f; g=0.8f; b=0.8f; // Light Gray
                }
            } else { // Other buttons
                 fontToUse = GLUT_BITMAP_HELVETICA_18; // Smaller
                 if (isSelected) {
                    r=1.0f; g=1.0f; b=1.0f; 
                 } else {
                    r=0.7f; g=0.7f; b=0.7f;
                 }
            }
            
            // Text position centering within box
            // Simple offset for now
            gbps_PrintText(currentX + 0.5f, y + 0.2f, r, g, b, fontToUse, MenuOptions[i]);
        }

        // 3. SPECIAL THANKS CREDIT (Bottom Right)
        float credit_alpha = 0.4f + (pulse_alpha * 0.3f);
        // Shadow
        gbps_PrintText(4.6, -6.6, 0.0, 0.0, 0.0, GLUT_BITMAP_HELVETICA_12, "Special Thanks: Antigravity");
        // Main Text
        gbps_PrintText(4.5, -6.5, 0.0, 1.0, 1.0, GLUT_BITMAP_HELVETICA_12, "Special Thanks: Antigravity");
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
	glutSwapBuffers();
}

void gbps_MainMenuSpecialKey(int key, int x, int y)
{
    int prev_selection = selection;
    
    switch (key)
    {
    case GLUT_KEY_UP :
        if(selection>extreme_low) selection--;
        else selection=extreme_up;
        break;
    case GLUT_KEY_DOWN :
        if(selection<extreme_up) selection++;
        else selection=extreme_low;
        break;
    }
    
    if (prev_selection != selection) {
        gbps_PlayMenuMoveSound();
        target_menu_pos=MainMenuPos[selection]; // Update target for smooth scroll if we were using it visually
    }
}

void gbps_MainMenuAsciiKey(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: exit(0); break;
    case 13: gbps_mm_EnterEvents(); break;
    }
}

void gpbs_MainMenuLoop(int val)
{
    // Simplified: Just redraw. Smooth scrolling logic was for camera primarily or simple text movement.
    // We are redrawing frames constantly for the pulse effect.
    glutPostRedisplay();
    glutTimerFunc(GBPS_nf,gpbs_MainMenuLoop,0);
}
