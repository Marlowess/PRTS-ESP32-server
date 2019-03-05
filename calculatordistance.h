#ifndef CALCULATORDISTANCE_H
#define CALCULATORDISTANCE_H

#include <iostream>
#include <QDebug>
class CalculatorDistance{
public:
    CalculatorDistance();
    //void getPosition(int rssi1, int rssi2, int rssi3, int rssi4, double *x, double *y);
    int getPosition(int* rssi, double *x, double *y);
    void setBoardsPosition(int index, int x, int y, QString mac);
    void printBoardPositions();
    int getBoardVec(QString mac);

private:
    int ESP_position[4][2] = {{0, 0},{0, 0},{0, 0},{0, 0}};

    short CalculeDistance(float x1, float y1, float x2, float y2);
    double RSSItoDinstance(short RSSI);
    void Avg(float x1, float y1, float x2, float y2, float *xavg, float *yavg);
    float Slope(float x1, float y1, float x2, float y2);
    QMap<QString, int> boardMap;

};

#endif // CALCULATORDISTANCE_H
