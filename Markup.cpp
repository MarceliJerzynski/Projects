#include "Markup.h"

Markup::Markup()
{
}

void Markup::loadMarkup( float scale)
{
    track.push_back(vec3(0, 0, -15));
    track.push_back(vec3(0, 0, -45));
    track.push_back(vec3(0, 0, -75));
    track.push_back(vec3(0, 0, -105));
    track.push_back(vec3(0, 0, -135));
    track.push_back(vec3(0, 0, -165));
    track.push_back(vec3(0, 0, -195));
    track.push_back(vec3(0, 0, -225));
    track.push_back(vec3(0, 0, -255));
    track.push_back(vec3(0, 0, -285));


    arrow = new Object();
    arrow->loadFromPath("arrow.obj","bricks.png", track[0], -90 ,0,0, scale);
    position = track[0];
    index = 0;
    scaling = scale;
    radius = 20*scale;
}

Object *Markup::getArrow()
{
    return arrow;
}

void Markup::touched()
{
        if (index == track.size() - 1)  //koniec wycisgu
        {
            cout<<"KONIEC WYŒCIGU!";
        }
        else
        {
            index++;
            changePosition(track[index]);
                cout<<"Index = "<<index<<endl;
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

unsigned int Markup::getIndex()
{
    return index;
}
