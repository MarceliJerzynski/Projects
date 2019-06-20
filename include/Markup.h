#ifndef MARKUP_H
#define MARKUP_H

#include "Object.h"
#include "Car.h"
#include <iostream>
using namespace std;
class Markup
{
    public:
        Markup(vec3 aposition, float scale);
        Object getArrow();
        bool touched(Car car);
        void changePosition( vec3 aposition);
        vec3 getPosition();
    protected:

    private:
        Object arrow;
        float scaling;
        float Radius = 8;
        vec3 position;
};

#endif // MARKUP_H
