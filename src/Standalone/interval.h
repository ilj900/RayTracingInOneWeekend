#pragma once

class FInterval
{
public:
    double Min;
    double Max;

    FInterval();
    FInterval(double MinIn, double MaxIn);
    FInterval(const FInterval& A, const FInterval& B);

    double Size() const;
    bool Contains(double X);
    bool Surrounds(double X);
    double Clamp(double X) const;
    FInterval Expand(double Delta);

    static const FInterval Empty;
    static const FInterval Universe;

    friend FInterval operator+(const FInterval& Interval, double Value);
    friend FInterval operator+(double Value, const FInterval& Interval);
};
