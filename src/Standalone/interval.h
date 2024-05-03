#ifndef INTERVAL_H
#define INTERVAL_H

class FInterval
{
public:
    double Min;
    double Max;

    FInterval();
    FInterval(double MinIn, double MaxIn);

    double Size();
    bool Contains(double X);
    bool Surrounds(double X);
    double Clamp(double X) const;

    static const FInterval Empty;
    static const FInterval Universe;
};

#endif // INTERVAL_H