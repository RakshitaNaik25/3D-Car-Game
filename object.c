#include"object.h"
#include"game.h"
#include <math.h>
void RenderTree(GBPS_Tree Tree)
{
    //

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();//for the current object
    glTranslatef(Tree.x,-1.0,-Tree.y);//Object translate

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);     // Turn Blending On
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glRotatef(45.0,0,1.0,0);
    glBindTexture(GL_TEXTURE_2D, GBPS_texArr[GBPS_TEXTURE_TREE]);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0,0.0);
    glVertex3f(-5.0,-5.0,0);
    glTexCoord2d(0.0,1.0);
    glVertex3f(-5.0,5.0,0.0);
    glTexCoord2d(1.0,1.0);
    glVertex3f(5.0,5.0,0.0);
    glTexCoord2d(1.0,0.0);
    glVertex3f(5.0,-5.0,0.0);
    //glTranslatef(0.0,0.0,a);

    glEnd();
    glPushMatrix();
    glRotatef(90.0,0,1.0,0);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0,0.0);
    glVertex3f(-5.0,-5.0,0);
    glTexCoord2d(0.0,1.0);
    glVertex3f(-5.0,5.0,0.0);
    glTexCoord2d(1.0,1.0);
    glVertex3f(5.0,5.0,0.0);
    glTexCoord2d(1.0,0.0);
    glVertex3f(5.0,-5.0,0.0);
    //glTranslatef(0.0,0.0,a);

    glEnd();
    glPopMatrix();


    glDisable(GL_TEXTURE_2D);
    // DEBUG_drwCord();
    glDisable(GL_BLEND);     // Turn Blending On
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}


void render_speedometer(){
    static float aaa=0;
    if(aaa==0.0) aaa=PlayerCar.vel;
    if(aaa>PlayerCar.vel) aaa-=0.05;
    else if(aaa<PlayerCar.vel) aaa+=0.05;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Bottom-Left Anchoring (with safe-margin)
    float base_x = 80.0f;
    float base_y = 80.0f;
    float size = 60.0f;

    // 1. Draw Speedometer Face
    glPushMatrix();
        glTranslatef(base_x, base_y, 0.0f);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, GBPS_texArr[GBPS_TEXTURE_SPEED]);
        glColor4f(1.0, 1.0, 1.0, 0.8); // 80% opacity for non-intrusive look
        glBegin(GL_QUADS);
            glTexCoord2d(0.0,0.0); glVertex2f(-size, -size);
            glTexCoord2d(0.0,1.0); glVertex2f(-size,  size);
            glTexCoord2d(1.0,1.0); glVertex2f( size,  size);
            glTexCoord2d(1.0,0.0); glVertex2f( size, -size);
        glEnd();
    glPopMatrix();

    // 2. Draw Pointer
    glPushMatrix();
        glTranslatef(base_x, base_y, 0.0f);
        float deg = -aaa * 40 + 70;
        if(fabs(deg) > 150) deg = (deg < 0) ? -150 : 150;
        glRotatef(deg, 0, 0, 1.0);
        
        glBindTexture(GL_TEXTURE_2D, GBPS_texArr[GBPS_TEXTURE_PTR]);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        glBegin(GL_QUADS);
            glTexCoord2d(0.0,0.0); glVertex2f(-size, -size);
            glTexCoord2d(0.0,1.0); glVertex2f(-size,  size);
            glTexCoord2d(1.0,1.0); glVertex2f( size,  size);
            glTexCoord2d(1.0,0.0); glVertex2f( size, -size);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}


void render_game_over(){
    static float pqr=0.1;
    if(pqr<1.0){
        pqr+=0.04; 
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glPushMatrix();
        glTranslatef(gbps_WindowWidth/2.0f, gbps_WindowHeight/2.0f, 0.0f);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, GBPS_texArr[GBPS_TEXTURE_GO]);
        
        glColor4f(1.0, 1.0, 1.0, pqr);
        glScalef(pqr, pqr, 1.0);

        // Render quad centered in 2D
        float w = 400.0f; // Professional size in pixels
        float h = 200.0f;
        glBegin(GL_QUADS);
            glTexCoord2d(0.0,0.0); glVertex2f(-w/2, -h/2);
            glTexCoord2d(0.0,1.0); glVertex2f(-w/2,  h/2);
            glTexCoord2d(1.0,1.0); glVertex2f( w/2,  h/2);
            glTexCoord2d(1.0,0.0); glVertex2f( w/2, -h/2);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
