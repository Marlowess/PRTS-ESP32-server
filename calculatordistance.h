#ifndef CALCULATORDISTANCE_H
#define CALCULATORDISTANCE_H


class CalculatorDistance
{
public:
    CalculatorDistance();
    void getPosition(int rssi1, int rssi2, int rssi3, int rssi4, double *x, double *y);

private:
    int ESP_position[4][2];

    short CalculeDistance(float x1, float y1, float x2, float y2);
    double RSSItoDinstance(short RSSI);
    void Avg(float x1, float y1, float x2, float y2, float *xavg, float *yavg);
    float Slope(float x1, float y1, float x2, float y2);
};

#endif // CALCULATORDISTANCE_H
