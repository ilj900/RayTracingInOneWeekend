#ifndef INTERVAL_H
#define INTERVAL_H

class FInterval
{
public:
    float Min;
    float Max;

    FInterval();
    FInterval(float MinIn, float MaxIn);

    float Size();
    bool Contains(float X);
    bool Surrounds(float X);
    float Clamp(float X) const;

    static const FInterval Empty;
    static const FInterval Universe;
};

#endif // INTERVAL_H