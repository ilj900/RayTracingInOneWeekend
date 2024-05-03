#ifndef RAY_H
#define RAY_H

#include "vector3.h"

struct FRay
{
public:
    FRay();
    FRay(const FVector3& OriginIn, const FVector3& DirectionIn);

    const FPoint3& GetOrigin() const;
    const FVector3& GetDirection() const;

    FPoint3 At(double T)  const;

private:
    FPoint3 Origin;
    FVector3 Direction;
};


#endif // RAY_H