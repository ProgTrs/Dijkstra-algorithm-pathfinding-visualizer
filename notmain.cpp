#include <vector>

#include "graphics.hpp"
#include <cmath>

using namespace genv;
using namespace std;

int W = 400;
int H = 400;

struct Snow {
    double x, y;
    //Snow(){}
    Snow(double _x, double _y) {
        x = _x;
        y = _y;
    }
};

vector<Snow> init_snowfall(int n) {
    vector<Snow> snowfall;
    for (int i = 0; i < n; i++) {
        Snow s(rand()%W, rand()%H);

        snowfall.push_back(s);
    }
    return snowfall;
}

void snowing(vector<Snow> v) {
    for (int i = 0; i < v.size(); i++) {
        Snow s = v[i];
        gout<<move_to(s.x,s.y)<<dot;
    }
}

/*
void filled_circle(double x, double y, double rad) {
    for (int i = -rad; i <= rad; i++) {
        for (int j = -rad; j<= rad; j++) {
            if (i*i+ j*j < rad*rad)
                gout<< move_to(x+i,y+j)<< dot;
        }
    }
    gout<<refresh;
}

void empty_circle(double cur_x, double cur_y, double rad) {

    for (double angle = 0; angle <=2.0 * M_PI ; angle += 0.01) {
        double x = cur_x + rad * cos(angle);
        double y = cur_y + rad * sin(angle);
        gout<<move_to(x,y)<<dot;

    }
    gout<<refresh;
}
*/

int main()
{

    gout.open(W,H);
    vector<Snow> snowfall = init_snowfall(W);
    snowing(snowfall);
    gout<<refresh;


    event ev;
    while(gin >> ev) {
        gout<<refresh;
    }


    return 0;
}

