#include "Car.h"

Car::Car()
{
}

void Car::loadFromPath(string pathBody,string pathChassis,string pathHeadlit,string pathLicense,string pathWheel, string texPathBody,string texPathChassis,string texPathHeadlit,string texPathLicense, string texPathWheel, float apower, float abpower,vec3 aposition,
            float rotX, float rotY, float rotZ, float ascale)
{
//Ladowanie poszczegolnych obiektow
//----------------------------------------------------------------------------------------------------------------------
    markup = new Markup();

    body=new Object();
    body->loadFromPath(pathBody,texPathBody,aposition, rotX,rotY,rotZ,ascale);

    chassis=new Object();
    chassis->loadFromPath(pathChassis,texPathChassis,aposition, rotX,rotY,rotZ,ascale);

    headlit=new Object();
    headlit->loadFromPath(pathHeadlit,texPathHeadlit,aposition, rotX,rotY,rotZ,ascale);

    license=new Object();
    license->loadFromPath(pathLicense,texPathLicense,aposition, rotX,rotY,rotZ,ascale);

    FRW =new Object();
    FRW->loadFromPath(pathWheel,texPathWheel, aposition, rotX, rotY - 180,rotZ,ascale);

    RLW =new Object();
    RLW->loadFromPath(pathWheel,texPathWheel, aposition, rotX, rotY,rotZ,ascale);

    RRW =new Object();
    RRW->loadFromPath(pathWheel,texPathWheel, aposition, rotX, rotY - 180,rotZ,ascale);

    FLW =new Object();
    FLW->loadFromPath(pathWheel,texPathWheel, aposition, rotX, rotY,rotZ,ascale);

//----------------------------------------------------------------------------------------------------------------------



//Ustalanie wartosci poczatkowych
//----------------------------------------------------------------------------------------------------------------------
    Power = apower;
    temporaryPower = 0;
    backPower = abpower;
    v = 0;
    a = 0;
    goForward = false;
    goBackward = false;
    wheelRotation = rotZ;
//----------------------------------------------------------------------------------------------------------------------
}

vec3 Car::getPosition()
{
    return body->getPosition();
}

void Car::turnLeft()
{
    body->turn(turn_angle);
    chassis->turn(turn_angle);
    license->turn(turn_angle);
    headlit->turn(turn_angle);

    FLW->turn(turn_angle);
    FRW->turn(turn_angle);
    RLW->turn(turn_angle);
    RRW->turn(turn_angle);
}

void Car::turnRight()
{
    body->turn(-turn_angle);
    chassis->turn(-turn_angle);
    license->turn(-turn_angle);
    headlit->turn(-turn_angle);

    FLW->turn(-turn_angle);
    FRW->turn(-turn_angle);
    RLW->turn(-turn_angle);
    RRW->turn(-turn_angle);
}

void Car::turnWheelLeft()
{
    if ( FLW -> getRotationY() - body -> getRotationY() < max_wheel_angle*3.14f/180.0f)
    {
        FLW -> turn(turn_wheel_angle);
        FRW -> turn(turn_wheel_angle);
    }
}

void Car::turnWheelRight()
{
     if   (FLW -> getRotationY() - body -> getRotationY() >-max_wheel_angle*3.14f/180.0f)
    {
        FLW -> turn(-turn_wheel_angle);
        FRW -> turn(-turn_wheel_angle);
    }
}

int Car::isMoving()
{

//zatrzymanie samochodu
//----------------------------------------------------------------------------------------------------------------------
    if (v < 0.02 && v > -0.02)
    {
        return 0;
    }
    if (v > 0.02)
    {
        return 1;
    } else
    return -1;

//----------------------------------------------------------------------------------------------------------------------
}


void Car::move(int going)
//going = 1 <- gracz naciska W
//going = 2 <- gracz naciska S
//going = 0 <- gracz nie trzyma nic
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
    if (going == 0)
    {
        resistance = resistance*3;
    }


    a = temporaryPower - resistance;
    v = v + a/60;
    float s = v;
    body->move(s);
    chassis->move(s);
    license->move(s);
    headlit->move(s);

    vec3 aposition;
    aposition.x = 0.786 * cos(-body->getRotationY()) - 1.257*sin(-body->getRotationY()) + body->getPosition().x;
    aposition.y = body->getPosition().y + 0.334;
    aposition.z = 0.786 * sin(-body->getRotationY()) + 1.257*cos(-body->getRotationY()) + body->getPosition().z;

    FLW->setPosition(aposition);

    aposition.x = -0.786 * cos(-body->getRotationY()) - 1.257*sin(-body->getRotationY()) + body->getPosition().x;
    aposition.y = body->getPosition().y + 0.334;
    aposition.z = -0.786 * sin(-body->getRotationY()) + 1.257*cos(-body->getRotationY()) + body->getPosition().z;

    FRW->setPosition(aposition);

    aposition.x = -0.786 * cos(-body->getRotationY()) + 1.353*sin(-body->getRotationY()) + body->getPosition().x;
    aposition.y = body->getPosition().y + 0.334;
    aposition.z = -0.786 * sin(-body->getRotationY()) - 1.353*cos(-body->getRotationY()) + body->getPosition().z;

    RRW->setPosition(aposition);

    aposition.x = 0.786 * cos(-body->getRotationY()) + 1.353*sin(-body->getRotationY()) + body->getPosition().x;
    aposition.y = body->getPosition().y + 0.334;
    aposition.z = 0.786 * sin(-body->getRotationY()) - 1.353*cos(-body->getRotationY()) + body->getPosition().z;

    RLW->setPosition(aposition);


//operacje krêcenia ko³em
//----------------------------------------------------------------------------------------------------------------------
FLW->rotateX(-v*100);
FRW->rotateX(-v*100);
RLW->rotateX(-v*100);
RRW->rotateX(-v*100);

//----------------------------------------------------------------------------------------------------------------------
}

float Car::getRotation()
{
    return body->getRotationY();
}

float Car::getWheelRotation()
{
    return FLW ->getRotationY() - body -> getRotationY();
}

Markup * Car::getMarkup()
{
    return markup;
}

bool Car::checkpointReached()
{
     if ((body->getPosition().x - markup->getPosition().x)*(body->getPosition().x - markup->getPosition().x)
        + (body->getPosition().z - markup->getPosition().z)*(body->getPosition().z - markup->getPosition().z) <= markup->getRadius()*markup->getRadius()) //znaki w drugim zamienione bo z jest odwrotnie
    {
        markup->touched();
        return true;
    }

    return false;
}

void Car::render(mat4 V, mat4 P, ShaderProgram *sp)
{
    body->render(V, P, sp);
    chassis->render(V, P, sp);
    headlit->render(V, P, sp);
    license->render(V, P, sp);
    FLW->render(V, P, sp);
    FRW->render(V, P, sp);
    RLW->render(V, P, sp);
    RRW->render(V, P, sp);
}

void Car::AI()
{
    move(1); //rusz do przodu
    float a = tan(body->getRotationY());
    float b = body->getPosition().z - a*body->getPosition().x;
    if (markup->getPosition().z < a*body->getPosition().x + b )
    {
        turnLeft();
        turnWheelLeft();
    } else
    {
        turnRight();
        turnWheelRight();
    }

}
