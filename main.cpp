#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Global flag for shadow pass
bool shadowPass = false;

// Global flag for cube auto rotation
bool autoRotateCube = false;

const int WIDTH = 800, HEIGHT = 600;
float camX = 0.0f, camY = 1.5f, camZ = 8.0f;
float centerX = 0.0f, centerY = 1.0f, centerZ = 0.0f;
int selected = 1;

float cubePos[] = {-2.0f, 0.3f, 0.0f};
float cubeRotX = 0, cubeRotY = 0, cubeScale = 0.8f;

float pyrPos[] = {0.0f, 0.0f, 2.0f};
float pyrRotX = 0, pyrRotY = 0, pyrScale = 1.0f;
float sphPos[] = {2.0f, 0.5f, 0.0f};
float sphRotX = 0, sphRotY = 0, sphScale = 1.0f;

GLuint faceTex[6];

void makeFaceTexture(int faceIndex)
{
    const int size = 64;
    unsigned char data[size * size * 3];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int idx = (i * size + j) * 3;
            bool stripe = ((i / 8 + j / 8) % 2 == 0);
            switch(faceIndex)
            {
            case 0:
                data[idx + 0] = stripe ? 255 : 100;
                data[idx + 1] = 0;
                data[idx + 2] = 0;
                break;
            case 1:
                data[idx + 0] = 0;
                data[idx + 1] = stripe ? 255 : 100;
                data[idx + 2] = 0;
                break;
            case 2:
                data[idx + 0] = 0;
                data[idx + 1] = 0;
                data[idx + 2] = stripe ? 255 : 100;
                break;
            case 3:
                data[idx + 0] = stripe ? 255 : 150;
                data[idx + 1] = stripe ? 255 : 150;
                data[idx + 2] = 0;
                break;
            case 4:
                data[idx + 0] = stripe ? 255 : 100;
                data[idx + 1] = 0;
                data[idx + 2] = stripe ? 255 : 100;
                break;
            case 5:
                data[idx + 0] = 0;
                data[idx + 1] = stripe ? 255 : 100;
                data[idx + 2] = stripe ? 255 : 100;
                break;
            }
        }
    }
    glBindTexture(GL_TEXTURE_2D, faceTex[faceIndex]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void buildCubeTextures()
{
    glGenTextures(6, faceTex);
    for (int i = 0; i < 6; i++)
    {
        makeFaceTexture(i);
    }
}

void setupLighting()
{
    GLfloat lightPos[] = {0.0f, 5.0f, -10.0f, 1.0f};  // sun's location
    GLfloat ambient[]  = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void drawSun()
{
    glPushMatrix();
    glTranslatef(0.0f, 5.0f, -10.0f);
    GLfloat yellow[] = {1.0f, 1.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, yellow);
    glutSolidSphere(0.5, 32, 32);
    GLfloat noEmission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
    glPopMatrix();
}



void drawGround()
{
    glDisable(GL_LIGHTING);
    glColor3f(0.3f, 0.7f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(-50, 0, -50);
    glVertex3f(50, 0, -50);
    glVertex3f(50, 0, 50);
    glVertex3f(-50, 0, 50);
    glEnd();
    glEnable(GL_LIGHTING);
}

// Draw the Cube with 6 Textures (modified for shadowPass)
void drawCube()
{
    glPushMatrix();
    glTranslatef(cubePos[0], cubePos[1], cubePos[2]);
    glRotatef(cubeRotX, 1, 0, 0);
    glRotatef(cubeRotY, 0, 1, 0);
    glScalef(cubeScale, cubeScale, cubeScale);

    // Only enable texture mapping if we're not in the shadow pass
    if (!shadowPass)
        glEnable(GL_TEXTURE_2D);

    GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);

    // Face 0: +Z
    glBindTexture(GL_TEXTURE_2D, faceTex[0]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1, -1, 1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1, -1, 1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1,  1, 1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1,  1, 1);
    glEnd();

    // Face 1: -Z
    glBindTexture(GL_TEXTURE_2D, faceTex[1]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1, -1, -1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1, -1, -1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1,  1, -1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1,  1, -1);
    glEnd();

    // Face 2: +Y
    glBindTexture(GL_TEXTURE_2D, faceTex[2]);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1, 1,  1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1, 1,  1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1, 1, -1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1, 1, -1);
    glEnd();

    // Face 3: -Y
    glBindTexture(GL_TEXTURE_2D, faceTex[3]);
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1, -1, -1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1, -1, -1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1, -1,  1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1, -1,  1);
    glEnd();

    // Face 4: +X
    glBindTexture(GL_TEXTURE_2D, faceTex[4]);
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1, -1,  1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1, -1, -1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1,  1, -1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1,  1,  1);
    glEnd();

    // Face 5: -X
    glBindTexture(GL_TEXTURE_2D, faceTex[5]);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1, -1, -1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1, -1,  1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1,  1,  1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1,  1, -1);
    glEnd();

    if (!shadowPass)
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

// Pyramid
void drawPyramid()
{
    glPushMatrix();
    glTranslatef(pyrPos[0], pyrPos[1], pyrPos[2]);
    glRotatef(pyrRotY, 0, 1, 0);
    glRotatef(pyrRotX, 1, 0, 0);
    glScalef(pyrScale, pyrScale, pyrScale);
    GLfloat purples[4][4] =
{
    {0.96f, 0.27f, 0.27f, 1.0f},  // F54545
    {0.996f, 0.41f, 0.12f, 0.0f}, // FE691E
    {1.0f, 0.54f, 0.02f, 1.0f},   // FF8A05
    {1.0f, 0.64f, 0.01f, 1.0f}    // FFA203
};



    glBegin(GL_TRIANGLES);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, purples[0]);
    glVertex3f(0, 1, 0);
    glVertex3f(-1, 0, 1);
    glVertex3f(1, 0, 1);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, purples[1]);
    glVertex3f(0, 1, 0);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 0, -1);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, purples[2]);
    glVertex3f(0, 1, 0);
    glVertex3f(1, 0, -1);
    glVertex3f(-1, 0, -1);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, purples[3]);
    glVertex3f(0, 1, 0);
    glVertex3f(-1, 0, -1);
    glVertex3f(-1, 0, 1);
    glEnd();

    GLfloat base[] = {0.1f, 0.4f, 0.1f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, base);
    glBegin(GL_QUADS);
    glVertex3f(-1, 0, 1);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 0, -1);
    glVertex3f(-1, 0, -1);
    glEnd();

    glPopMatrix();
}

// Sphere
void drawSphere()
{
    glPushMatrix();
    glTranslatef(sphPos[0], sphPos[1], sphPos[2]);
    glRotatef(sphRotY, 0, 1, 0);
    glRotatef(sphRotX, 1, 0, 0);
    glScalef(sphScale, sphScale, sphScale);

    GLfloat rainbow[] = {1.0f, 0.0f, 1.0f, 1.0f}; // start with purple-pink
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, rainbow);




    glutSolidSphere(0.5, 32, 32);
    glPopMatrix();
}

void drawShadowMatrix(GLfloat lightPos[4], GLfloat groundPlane[4])
{
    GLfloat dot = groundPlane[0]*lightPos[0] +
                  groundPlane[1]*lightPos[1] +
                  groundPlane[2]*lightPos[2] +
                  groundPlane[3]*lightPos[3];

    GLfloat shadowMat[16];
    for (int i = 0; i < 16; i++) shadowMat[i] = 0;

    shadowMat[0]  = dot - lightPos[0] * groundPlane[0];
    shadowMat[4]  = 0.0f - lightPos[0] * groundPlane[1];
    shadowMat[8]  = 0.0f - lightPos[0] * groundPlane[2];
    shadowMat[12] = 0.0f - lightPos[0] * groundPlane[3];

    shadowMat[1]  = 0.0f - lightPos[1] * groundPlane[0];
    shadowMat[5]  = dot - lightPos[1] * groundPlane[1];
    shadowMat[9]  = 0.0f - lightPos[1] * groundPlane[2];
    shadowMat[13] = 0.0f - lightPos[1] * groundPlane[3];

    shadowMat[2]  = 0.0f - lightPos[2] * groundPlane[0];
    shadowMat[6]  = 0.0f - lightPos[2] * groundPlane[1];
    shadowMat[10] = dot - lightPos[2] * groundPlane[2];
    shadowMat[14] = 0.0f - lightPos[2] * groundPlane[3];

    shadowMat[3]  = 0.0f - lightPos[3] * groundPlane[0];
    shadowMat[7]  = 0.0f - lightPos[3] * groundPlane[1];
    shadowMat[11] = 0.0f - lightPos[3] * groundPlane[2];
    shadowMat[15] = dot - lightPos[3] * groundPlane[3];

    glMultMatrixf(shadowMat);
}

void display()
{
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, centerX, centerY, centerZ, 0, 1, 0);

    setupLighting();
    drawSun();
    drawGround();
    drawCube();
    drawPyramid();
    drawSphere();

    GLfloat lightPos[] = {0.0f, 5.0f, -10.0f, 1.0f};
    GLfloat groundPlane[] = {0.0f, 1.0f, 0.0f, 0.0f};

    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glColor4f(122.0f/255.0f, 149.0f/255.0f, 152.0f/255.0f, 0.5f);

    shadowPass = true;
    glPushMatrix();
    drawShadowMatrix(lightPos, groundPlane);
    drawCube();
    drawPyramid();
    drawSphere();
    glPopMatrix();
    shadowPass = false;

    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / h, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    float* rotXPtr;
    float* rotYPtr;
    if (selected == 1)
    {
        rotXPtr = &cubeRotX;
        rotYPtr = &cubeRotY;
    }
    else if (selected == 2)
    {
        rotXPtr = &pyrRotX;
        rotYPtr = &pyrRotY;
    }
    else
    {
        rotXPtr = &sphRotX;
        rotYPtr = &sphRotY;
    }
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case '1':
        selected = 1;
        break;
    case '2':
        selected = 2;
        break;
    case '3':
        selected = 3;
        break;
    case 'w':
        camZ -= 0.3f;
        break;
    case 's':
        camZ += 0.3f;
        break;
    case 'a':
        camX -= 0.3f;
        break;
    case 'd':
        camX += 0.3f;
        break;
    case 'q':
        camY += 0.3f;
        break;
    case 'e':
        camY -= 0.3f;
        break;
    case '+':
        if (selected == 1) cubeScale += 0.1f;
        if (selected == 2) pyrScale += 0.1f;
        if (selected == 3) sphScale += 0.1f;
        break;
    case '-':
        if (selected == 1) cubeScale -= 0.1f;
        if (selected == 2) pyrScale -= 0.1f;
        if (selected == 3) sphScale -= 0.1f;
        break;
    // For rotation:
    case 'r':
        (*rotYPtr) += 5.0f;
        break;
    case 't':
        (*rotYPtr) -= 5.0f;
        break;
    case 'f':
        (*rotXPtr) += 5.0f;
        break;
    case 'g':
        if (selected == 1)
            // Toggle auto rotation for the cube when it is selected.
            autoRotateCube = !autoRotateCube;
        else
            (*rotYPtr) -= 5.0f;
        break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
    float* pos;
    if (selected == 1)
    {
        pos = cubePos;
    }
    else if (selected == 2)
    {
        pos = pyrPos;
    }
    else
    {
        pos = sphPos;
    }
    switch (key)
    {
    case GLUT_KEY_UP:
        pos[2] -= 0.2f;
        break;
    case GLUT_KEY_DOWN:
        pos[2] += 0.2f;
        break;
    case GLUT_KEY_LEFT:
        pos[0] -= 0.2f;
        break;
    case GLUT_KEY_RIGHT:
        pos[0] += 0.2f;
        break;
         // New: Move Up (Y+)
    case GLUT_KEY_PAGE_UP:
        pos[1] += 0.2f; // up (Y+)
        break;

    //  New: Move Down (Y–)
    case GLUT_KEY_PAGE_DOWN:
        pos[1] -= 0.2f; // down (Y–)
        break;
    }
    glutPostRedisplay();
}

void update(int value)
{
    // If auto rotation is enabled, update cube's rotation continuously.
    if (autoRotateCube)
    {
        cubeRotY += 1.0f; // Adjust this value to change rotation speed
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Final FreeGLUT 3D Scene - Skybox Enabled");

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    buildCubeTextures();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
