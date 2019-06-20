#include "Markup.h"

Markup::Markup(vec3 aposition, float scale)
{
    arrow.loadFromPath("arrow.obj", aposition, -90 ,0,0, scale);
    position = aposition;
    scaling = scale;
}

Object Markup::getArrow()
{
    return arrow;
}

bool Markup::touched(Car car)
{
    if ((car.getPosition().x - position.x)*(car.getPosition().x - position.x)
        + (-car.getPosition().z + position.z)*(-car.getPosition().z + position.z) <= Radius*Radius) //znaki w drugim zamienione bo z jest odwrotnie
    {
        return true;
    }
}

void Markup::changePosition(vec3 aposition)
{
    position = aposition;
    arrow.setM(aposition, -90, 0, 0, scaling);
}

vec3 Markup::getPosition()
{
    return position;
}
