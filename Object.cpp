#include "Object.h"

Object::Object()
{
}

void Object::loadFromPath(string path,vec3 aposition, float rotX, float rotY, float rotZ, float ascale)
{
    OBJLoader loader;
    loader.load(path);
    position = aposition;
    setM(aposition, rotX, rotY, rotZ, ascale);
    verts = loader.getVerts();
    normals = loader.getNormals();
    colors = loader.getColors();
    vertexCount = loader.getVertexCount();
}

void Object::loadFromLoader(OBJLoader loader, vec3 aposition, float rotX, float rotY, float rotZ, float ascale)
{
    position = aposition;
    setM(aposition, rotX, rotY, rotZ, ascale);
    verts = loader.getVerts();
    normals = loader.getNormals();
    colors = loader.getColors();
    vertexCount = loader.getVertexCount();
}

mat4 Object::getM()
{
    return M;
}


float * Object::getVerts()
{
    return verts;
}

float * Object::getNormals()
{
    return normals;
}

float * Object::getColors()
{
    return colors;
}
unsigned int Object::getVertexCount()
{
    return vertexCount;
}

vec3 Object::getPosition()
{
    return position;

}

void Object::setPosition(vec3 aposition)
{
    setM(aposition, rotationX, rotationY, rotationZ, scaling);
}

void Object::turn(float rot)
{
    setM(position, rotationX, rotationY+rot, rotationZ, scaling);
}

void Object::move(float dc)
{
    float dx = -dc * sin(rotationY*3.14f/180.0f);
    float dz = -dc * cos(rotationY*3.14f/180.0f);
    position.x+=dx;
    position.z+=dz;
    setM(position, rotationX, rotationY, rotationZ, scaling);
}
float Object::getRotationY()
{
    return (rotationY-180)*3.14f/180.0f;
}

void Object::setRotation(float rotX, float rotY, float rotZ)
{
   if (rotX != NULL) rotationX = rotX;
   if (rotY != NULL) rotationY = rotY;
   if (rotZ != NULL) rotationZ = rotZ;
   setM(position, rotationX, rotationY, rotationZ, scaling);
}

void Object::render(mat4 V, mat4 P, ShaderProgram *sp)
{
    UniformAllMatrix4(M,V, P, sp);
    sendAttributes(verts,normals,colors, sp);
    glDrawArrays(GL_TRIANGLES,0,vertexCount); //Narysuj obiekt
    disableAttributes(sp);
}

void Object::UniformAllMatrix4(mat4 M,mat4 V, mat4 P, ShaderProgram *sp)
{
    glUniformMatrix4fv(sp->u("P"),1,false,value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,value_ptr(V));
    glUniformMatrix4fv(sp->u("M"),1,false,value_ptr(M));
}

void Object::disableAttributes(ShaderProgram *sp)
{
    glDisableVertexAttribArray(sp->a("vertex"));  //Wy��cz przesy�anie danych do atrybutu vertex
    glDisableVertexAttribArray(sp->a("normal"));  //Wy��cz przesy�anie danych do atrybutu normal
    glDisableVertexAttribArray(sp->a("color"));  //Wy��cz przesy�anie danych do atrybutu color
}

void Object::sendAttributes(float *verts, float *normals, float *colors, ShaderProgram *sp)
{
    glEnableVertexAttribArray(sp->a("vertex"));  //W��cz przesy�anie danych do atrybutu vertex
    glVertexAttribPointer(sp->a("vertex"),4,GL_FLOAT,false,0,verts); //Wska� tablic� z danymi dla atrybutu vertex

    glEnableVertexAttribArray(sp->a("normal"));  //W��cz przesy�anie danych do atrybutu normal
    glVertexAttribPointer(sp->a("normal"),4,GL_FLOAT,false,0,normals); //Wska� tablic� z danymi dla atrybutu normal

    glEnableVertexAttribArray(sp->a("color"));  //W��cz przesy�anie danych do atrybutu color
    glVertexAttribPointer(sp->a("color"),4,GL_FLOAT,false,0,colors); //Wska� tablic� z danymi dla atrybutu color
}

void Object::setM(vec3 aposition, float rotX, float rotY, float rotZ, float ascale)
{
    position = aposition;
    M=mat4(1.0f);
    M=translate(M, position);
    M=rotate(M, rotY*3.14f/180.0f, vec3(0.0f, 1.0f, 0.0f));
    M=rotate(M, rotX*3.14f/180.0f, vec3(1.0f, 0.0f, 0.0f));
    M=rotate(M, rotZ*3.14f/180.0f, vec3(0.0f, 0.0f, 1.0f));
    M=scale(M, vec3(ascale, ascale, ascale));
    rotationY = rotY;
    rotationX = rotX;
    rotationZ = rotZ;
    scaling=ascale;
}

void Object:: rotateX(float angle)
{
    rotationX += angle;
   // M = rotate(M, rotationX*3.14f/180.0f, vec3(1.0f, 0.0f, 0.0f));
   setM(position, rotationX, rotationY,rotationZ, scaling);
}
