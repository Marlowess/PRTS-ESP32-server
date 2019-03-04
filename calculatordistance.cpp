#include "calculatordistance.h"
#include <math.h>

#define A -59 //RSSI 1 meter distance
#define N 2.7 //signal propagation constant

CalculatorDistance::CalculatorDistance(){}

double CalculatorDistance::RSSItoDinstance(short RSSI){
    //cout << "distanza " << pow(10, (A-RSSI)/(10*N)) <<"\n";
    return (double) pow(10, (A-RSSI)/(10*N));
}

short CalculatorDistance::CalculeDistance(float x1, float y1, float x2, float y2){
    short dx, dy;
    dx = pow((short)(x1 - x2), 2);
    dy = pow((short)(y1 - y2), 2);
    return pow(dx + dy, 0.5);
}

float CalculatorDistance::Slope(float x1, float y1, float x2, float y2){
    return(y1 - y2) / (x1-x2);
}

void CalculatorDistance::Avg(float x1, float y1, float x2, float y2, float *xavg, float *yavg){
    *xavg = x1 + (x1 - x2) / 2;
    *yavg = y1 + (y1 - y2) / 2;
}

void CalculatorDistance::getPosition(int rssi1, int rssi2, int rssi3, int rssi4, double *x, double *y){
    //it is assumed to receive 0 as last parameters
    int n_ESP=0, i, j, ESP_used[4];
    double d[4], q[4][4], qf[4];
    // d is a vector which contains the distances from the ESPs
    for(i=0; i<4; i++){
        ESP_used[i]=0;
        d[i] = 0;
    }


    if (rssi1 != 0){
        d[0] = RSSItoDinstance(rssi1);
        ESP_used[0]=1;
        n_ESP++;
    }
    if (rssi2 != 0){
        d[1] = RSSItoDinstance(rssi2);
        ESP_used[1]=1;
        n_ESP++;
    }
    if (rssi3 != 0){
        d[2] = RSSItoDinstance(rssi3);
        ESP_used[2]=1;
        n_ESP++;
    }
    if (rssi4 != 0){
        d[3] = RSSItoDinstance(rssi4);
        ESP_used[3]=1;
        n_ESP++;
    }    
    qDebug() << "RSSI_1: " << rssi1 << "  RSSI_2: " << rssi2;
    qDebug() << "DISTANZE ";
    for(i=0; i<n_ESP; i++)
        qDebug() << d[i] << " ";
    //acquisition of ESP's positions

    //q is a matrix that contains the coordinates of the sides of the square built around the ESPs
    for (i = 0; i < 4; i++){
        if(ESP_used[i]==1){
            q[i][0] = (double) ESP_position[i][0] - d[i];
            //cout << "q[i][0] " << q[i][0] << "\n";
            q[i][1] = (double) ESP_position[i][1] - d[i];
            //cout << "q[i][1] " << q[i][1] << "\n";
            q[i][2] = (double) ESP_position[i][0] + d[i];
            //cout << "q[i][2] " << q[i][2] << "\n";
            q[i][3] = (double) ESP_position[i][1] + d[i];
            //cout << "q[i][3] " << q[i][3] << "\n";
            qf[0] = q[i][0];
            qf[1] = q[i][1];
            qf[2] = q[i][2];
            qf[3] = q[i][3];
        }
    }

    /*
    cout << "MATRICE q\n";
    for(i=0; i<4; i++)
    {
        for(j=0; j<4; j++)
        {
            cout << q[i][j]<<" ";
        }
        cout << "\n";
    }
    */

    //inizializzazione del vettore qf
//    qf[0] = q[0][0];
//    qf[1] = q[0][1];
//    qf[2] = q[0][2];
//    qf[3] = q[0][3];
    for (i = 0; i < 4; i++){
        if (q[i][0] > qf[0] && ESP_used[i]==1)
            qf[0] = q[i][0];
        if (q[i][1] > qf[1] && ESP_used[i]==1)
            qf[1] = q[i][1];
        if (q[i][2] < qf[2] && ESP_used[i]==1)
            qf[2] = q[i][2];
        if (q[i][3] < qf[3] && ESP_used[i]==1)
            qf[3] = q[i][3];
    }
    //cout << "dentro funzione" << (qf[0] + qf[2]) / 2 << " " << (qf[1] + qf[3]) / 2 << "\n";
    *x = (qf[0] + qf[2]) / 2;
    *y = (qf[1] + qf[3]) / 2;
}

void CalculatorDistance::setBoardsPosition(int index, int x, int y){
    ESP_position[index][0] = x;
    ESP_position[index][1] = y;
}

void CalculatorDistance::printBoardPositions(){
    for(int i = 0; i < 4; i++)
        qDebug() << "Board " << i << ": " << ESP_position[i][0] << ", " << ESP_position[i][1];
}
