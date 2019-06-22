#include "Markup.h"

Markup::Markup()
{
}

void Markup::loadMarkup( float scale)
{
    track.push_back(vec3(0, 0, -10));
    track.push_back(vec3(0, 0, -20));
    track.push_back(vec3(5, 0, -25));
    track.push_back(vec3(10, 0, -30));
    track.push_back(vec3(5, 0, -35));

    arrow = new Object();
    arrow->loadFromPath("arrow.obj", track[0], -90 ,0,0, scale);
    position = vec3(track[0]);
    index = 0;
    scaling = scale;
    radius = 40*scale;
}

Object *Markup::getArrow()
{
    return arrow;
}

void Markup::touched()
{
        if (index == track.size())  //koniec wycisgu
        {
            cout<<"KONIEC WYŒCIGU!";
        }
        else
        {
            index++;
            changePosition(track[index]);
        }
}

void Markup::changePosition(vec3 aposition)
{
    position = aposition;
    arrow->setM(aposition, -90, 0, 0, scaling);
}

vec3 Markup::getPosition()
{
    return position;
}

float Markup::getRadius()
{
    return radius;
}
