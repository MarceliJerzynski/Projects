#ifndef MARKUP_H
#define MARKUP_H

#include "Object.h"
#include <iostream>
#include <vector>
using namespace std;
class Markup
{
    public:
        Markup();
        void loadMarkup(float scale);
        Object getArrow();
        void touched();
        void changePosition( vec3 aposition);
        vec3 getPosition();
        float getRadius();
    protected:

    private:
        Object arrow;
        float scaling;
        float radius;
        vec3 position;
        int index;
        vector <vec3> track;
};

#endif // MARKUP_H
