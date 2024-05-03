#ifndef INTERVAL_H
#define INTERVAL_H

class FInterval
{
public:
    double Min;
    double Max;

    FInterval();
    FInterval(double MinIn, double MaxIn);
    FInterval(const FInterval& A, const FInterval& B);

    double Size();
    bool Contains(double X);
    bool Surrounds(double X);
    double Clamp(double X) const;
    FInterval Expand(double Delta);

    static const FInterval Empty;
    static const FInterval Universe;
};

#endif // INTERVAL_H