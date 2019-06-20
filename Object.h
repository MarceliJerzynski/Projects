#ifndef OBJECT_H
#define OBJECT_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "OBJLoader.h"
#include "shaderprogram.h"

using namespace std;
using namespace glm;
class Object
{
    public:
        Object();
        void loadFromPath(string path, vec3 position, float rotX, float rotY, float rotZ, float scale);
        void loadFromLoader(OBJLoader loader, vec3 position, float rotX, float rotY, float rotZ, float scale);
        mat4 getM();
        float * getVerts();
        float * getNormals();
        float * getColors();
        float getRotation();
        unsigned int getVertexCount();
        vec3 getPosition();
        void turn(float rot);
        void move(float dc);
        void render(mat4 V, mat4 P, ShaderProgram *sp);
        void setM(vec3 aposition, float rotX, float rotY, float rotZ, float scale);

    protected:

        mat4 M;
        float *verts;
        float *normals;
        float *colors;
        unsigned int vertexCount;
        vec3 position;
        float rotation;
        float scaling;

    private:
        void sendAttributes(float *verts, float *normals, float *colors, ShaderProgram *sp);
        void disableAttributes(ShaderProgram *sp);
        void UniformAllMatrix4(mat4 M,mat4 V, mat4 P, ShaderProgram *sp);
};

#endif // OBJECT_H
