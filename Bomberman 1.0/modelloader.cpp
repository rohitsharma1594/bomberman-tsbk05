#include <lib3ds/types.h>
#include <lib3ds/file.h>
#include <lib3ds/mesh.h>
#include <lib3ds/material.h>
#include "modelloader.h"
#include "freeglut.h"
#include <stdlib.h>
#include <math.h>

/*
	Code for loading a 3ds model from http://www.csc.calpoly.edu/~hitchner/CPE476.S2006/lib3dsQuickStart.html
*/

ModelLoader::ModelLoader(const char* filename):Model() {

    Lib3dsFile *file=lib3ds_file_load(filename);
    if (!file) {
        printf("ModelLoader :: Failed to load file %s", filename);
        exit(1);
    }

    listId = glGenLists(1);
    if (listId == 0) {
        printf("ModelLoader :: Could not generate display list id.\n");
        exit(1);
    }

    glNewList(listId, GL_COMPILE);
    // insert any OpenGL commands of your own.

    glBegin(GL_TRIANGLES);
    for (Lib3dsMesh *mesh = file->meshes; mesh; mesh = mesh->next)  {    // draw ALL meshes of ALL objects

        for (unsigned p = 0; p < mesh->faces; p++) {    // for each face of the mesh
            Lib3dsFace *f = &mesh->faceL[p];         // get pointer to current face
            Lib3dsMaterial *mat = 0;             // get pointer to face's material (if exists)
            if (f->material[0]) {
                mat = lib3ds_file_material_by_name(file, f->material);
            }

            if (mat) {
                static GLfloat a[4]= {
                                         0.0f, 0.0f, 0.0f, 1.0f
                                     };
                float s;
                glMaterialfv(GL_FRONT, GL_AMBIENT, a);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
                s = pow(2.0f, 10.0f*mat->shininess);
                if (s>128.0f) {
                    s=128.0f;
                }
                glMaterialf(GL_FRONT, GL_SHININESS, s);
            } else {
                Lib3dsRgba a={0.2f, 0.2f, 0.2f, 1.0f};
                Lib3dsRgba d={0.8f, 0.8f, 0.8f, 1.0f};
                Lib3dsRgba s={0.0f, 0.0f, 0.0f, 1.0f};
                glMaterialfv(GL_FRONT, GL_AMBIENT, a);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
                glMaterialfv(GL_FRONT, GL_SPECULAR, s);
            }
            glNormal3fv(f->normal);
            for (unsigned i = 0; i < 3; i++) {    // for each vertex of face (triangle)
                glVertex3fv(mesh->pointL[f->points[i]].pos);
            }    // end vertices
        }    // end face
    }    // end mesh

    glEnd();    // end GL_TRIANGLES
    glEndList();    // end display list
    lib3ds_file_free(file);

    printf("Done loading model!\n");
}

void ModelLoader::draw() {
    glCallList(listId);
}

