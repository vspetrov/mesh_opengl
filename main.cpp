#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "mesh.hpp"
#include <GL/glut.h>
using namespace std;

#include <GL/glut.h>  // GLUT, include glu.h and gl.h

/* Global variables */
char title[] = "3D Shapes";

/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_FLAT);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Create light components
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    GLfloat position[] = { -1, 0, 1, 0 };

    GLfloat position2[] = { 1, 0.5, 1, 0 };
   // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    // glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
    // glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
    // glLightfv(GL_LIGHT1, GL_POSITION, position2);


    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    float specReflection[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
}

Mesh *mesh;
/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

    // Render a pyramid consists of 4 triangles
    glLoadIdentity();                  // Reset the model-view matrix
    glTranslatef(.0f, 0.0f, -3.5f);  // Move left and into the screen

    glColor3f(0,1,0);
    auto facets = mesh->getBoundaryFacets();
    glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
    for (auto &f : facets){
        auto points = f.getPoints();
        double *n = f.getNormal();
        glNormal3f(n[0],n[1],n[2]);
        for (int i=0; i<3; i++) {
            double *c = points[i]->getC();
            glVertex3f(c[0],c[1],c[2]);
        }
    }
    glEnd();   // Done drawing the pyramid



#if 0
    auto points = mesh->getPoints();
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
    for (auto &p : points) {
        double* c = p.getC();
        glVertex3f(c[0],c[1],c[2]);

    }
    glEnd();
#endif
    glEnable(GL_LIGHTING);
    glFlush();
    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;

   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);

   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    string mesh_file(argv[1]);
    mesh = new Mesh(mesh_file);
    mesh->normalizeForDrawing();

    for (auto &f : mesh->getBoundaryFacets())
        f.calcNormal();
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
    glutInitWindowSize(800, 800);   // Set the window's initial width & height
    glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    initGL();                       // Our own OpenGL initialization
    glutMainLoop();                 // Enter the infinite event-processing loop

    return 0;
}
