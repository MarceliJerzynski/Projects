#include "Car.h"

Car::Car()
{
}

void Car::loadFromPath(string pathBody,string pathFLW,string pathFRW,string pathRLW,string pathRRW, float apower, float abpower,vec3 aposition,
         float rotX, float rotY, float rotZ, float ascale)
{
    body=new Object();
    body->loadFromPath(pathBody,aposition, rotX,rotY,rotZ,ascale);

    FLW =new Object();
    FLW->loadFromPath(pathFLW, aposition, rotX, rotY,rotZ,ascale);

    FRW =new Object();
    FRW->loadFromPath(pathFRW, aposition, rotX, rotY,rotZ,ascale);

    RLW =new Object();
    RLW->loadFromPath(pathRLW, aposition, rotX, rotY,rotZ,ascale);

    RRW =new Object();
    RRW->loadFromPath(pathRRW, aposition, rotX, rotY,rotZ,ascale);

    wheel_angle = 0;
    Power = apower;
    temporaryPower = 0;
    backPower = abpower;
    v = 0;
    a = 0;
    goForward = false;
    goBackward = false;
}

vec3 Car::getPosition()
{
    return body->getPosition();
}

void Car::turn(float rot)
{
    body->turn(rot);
    FLW->turn(rot);
    FRW->turn(rot);
    RLW->turn(rot);
    RRW->turn(rot);
}

int Car::isMoving()
{
    if (v < 0.02 && v > -0.02)
    {
        return 0;
    }
    if (v > 0.02)
    {
        return 1;
    } else
    return -1;
}

void Car::move(int going)
{
    if ( going == 1 )
    {   if (temporaryPower < 1)
            temporaryPower += Power;
        else
            temporaryPower = 1;
    }
    else
    if (going == 2 )
    {
        if (temporaryPower > -0.5)
            temporaryPower -= backPower;
        else
            temporaryPower = -0.5;
    }
    else
    {
       temporaryPower = 0;
    }
    float resistance = v/3*2;
    a = temporaryPower - resistance;
    v = v + a/60;
    float s = v;
    cout<<v<<endl;
    body->move(s);
    FLW->move(s);
    FRW->move(s);
    RLW->move(s);
    RRW->move(s);
}

float Car::getRotation()
{
    return body->getRotation();
}

void Car::render(mat4 V, mat4 P, ShaderProgram *sp)
{
    body->render(V, P, sp);
    FLW->render(V, P, sp);
    FRW->render(V, P, sp);
    RLW->render(V, P, sp);
    RRW->render(V, P, sp);
}
