#include "Force.h"

Force::Force(Vector vector) : Vector::Vector(vector)
{
}

Force::Force(const Force& force) : Vector::Vector(dynamic_cast<const Vector&>(force))
{
}

Force::~Force(void)
{
}
