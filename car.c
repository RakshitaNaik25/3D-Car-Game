#include"car.h"
float car_crash_angle=0.0;

Gbps_Car* CreateCar(GLuint* cartexture)
{
    Gbps_Car* c=malloc(sizeof(Gbps_Car));
    return c;
}

void RenderCar(Gbps_Car Car)
{
    //

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();//for the current car
    glRotatef(car_crash_angle,1.0,0,0);
    glTranslatef(Car.x,0.6,-Car.y);//car translate
    glRotatef(-Car.rot,0,1,0);//car rotate
    
    // Realistic Metallic Paint & Widebody for Player Car
    if(Car.isPlayer) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, GBPS_texArr[GBPS_TEXTURE_CAR1]);

        // Widebody Transformation
        glPushMatrix();
        glScalef(1.2f, 1.0f, 1.1f); // Make it wide and mean

        // Metallic Burgundy 
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor3f(1.0f, 0.2f, 0.4f); // Metallic Burgundy

        GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat mat_shininess[] = { 110.0f };
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

        glmDraw(car1, GLM_SMOOTH | GLM_TEXTURE);
        glPopMatrix();
        
        // Reset env for wheels
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glColor3f(1.0f, 1.0f, 1.0f);
    } else {
        glBindTexture(GL_TEXTURE_2D, GBPS_texArr[GBPS_TEXTURE_CAR1]);
        glmDraw(car1, GLM_SMOOTH | GLM_TEXTURE);
    }

    //tyredraw
    glRotated(-90,0,1,0);
    glBindTexture(GL_TEXTURE_2D, GBPS_texArr[GBPS_TEXTURE_TYRE]);

    // Front Left
    glPushMatrix();
    glTranslatef(-1.205f * (Car.isPlayer ? 1.2f : 1.0f), -0.277f, 0.733f);
    glRotatef(-(Car.rot*2)+90,0,1,0);
    glRotatef(Car.TyreRot,-1,0,0);
    glmDraw(tyre1,GLM_SMOOTH | GLM_TEXTURE);
    glPopMatrix();

    // Front Right
    glPushMatrix();
    glTranslatef(-1.206f * (Car.isPlayer ? 1.2f : 1.0f), -0.277f, -0.731f);
    glRotatef(-(Car.rot*2)+270,0,1,0);
    glRotatef(Car.TyreRot,1,0,0);
    glmDraw(tyre1,GLM_SMOOTH | GLM_TEXTURE);
    glPopMatrix();

    // Rear Left (Giant Wheels for Player)
    glPushMatrix();
    glTranslatef(1.155f * (Car.isPlayer ? 1.1f : 1.0f), -0.277f, 0.731f * (Car.isPlayer ? 1.2f : 1.0f));
    if(Car.isPlayer) glScalef(1.5f, 1.5f, 1.5f);
    glRotatef(90,0,1,0);
    glRotatef(Car.TyreRot,-1,0,0);
    glmDraw(tyre1,GLM_SMOOTH | GLM_TEXTURE);
    glPopMatrix();

    // Rear Right (Giant Wheels for Player)
    glPushMatrix();
    glTranslatef(1.154f * (Car.isPlayer ? 1.1f : 1.0f), -0.277f, -0.733f * (Car.isPlayer ? 1.2f : 1.0f));
    if(Car.isPlayer) glScalef(1.5f, 1.5f, 1.5f);
    glRotatef(270,0,1,0);
    glRotatef(Car.TyreRot,1,0,0);
    glmDraw(tyre1,GLM_SMOOTH | GLM_TEXTURE);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
