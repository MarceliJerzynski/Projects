#ifndef ENEMY_H
#define ENEMY_H

#include "Car.h"

class Enemy : public Car
{
    public:
        void AI();
    protected:

    private:
        unsigned int target;

#endif // ENEMY_H
