#ifndef CONTROLLERS_H
#define CONTROLLERS_H
#include "pose.h"
#include <QString>
#include <deque>
using namespace std;
// Constants required by generateControl, directly copied from most recent version of kgpkubs.
const float MAX_BOT_LINEAR_VEL_CHANGE  = 5;
const float MAX_BOT_SPEED              = 80.0;
const int BOT_POINT_THRESH             = 147;
const int CLEARANCE_PATH_PLANNER       = 400;

const int timeLCMs = 16;
const double timeLC = timeLCMs*0.001;
const int NUMTICKS = 300;
#define SGN(x) (((x)>0)?1:(((x)<0)?(-1):0))
namespace Controllers {

void kgpkubs(Pose initialPose, Pose finalPose, int &vl, int &vr, double prevSpeed);
void CMU(Pose s, Pose e, int &vl, int &vr, double prevSpeed);
void PController(Pose s, Pose e, int &vl, int &vr, double prevSpeed);
void PolarBased(Pose s, Pose e, int &vl, int &vr, double prevSpeed);
void PolarBidirectional(Pose s, Pose e, int &vl, int &vr, double prevSpeed);
// functions for GA
void PolarBasedGA(Pose s, Pose e, int &vl, int &vr, double k1, double k2, double k3); // for use in GA
}
typedef void(*FType)(Pose, Pose, int &, int &, double);
typedef std::pair<QString, FType> FPair;



class ControllerWrapper { // a wrapper to implement controller for a robot. Currently able to handle packet delay.
    FType fun;
    deque<pair<int,int> > uq; // controls queue. .first = vl, .second = vr
    int k;                    // the num of packet delay
public:
    ControllerWrapper(FType fun, int k):fun(fun), k(k) {
        for(int i = 0; i < k; i++)
            uq.push_back(make_pair<int,int>(0,0));
    }
    void genControls(Pose s, Pose e, int &vl, int &vr) {
        Pose x = s;
        for(deque<pair<int,int> >::iterator it = uq.begin(); it != uq.end(); it++) {
            x.updateNoDelay(it->first, it->second, timeLC);
        }
        pair<int,int> previousControl = uq.size()?uq.at(uq.size()-1):make_pair<int,int>(0,0);
        double prevSpeed = max(fabs(previousControl.first), fabs(previousControl.second));
        (*fun)(x, e, vl, vr, prevSpeed);
        uq.push_back(make_pair<int,int>(vl, vr));
        uq.pop_front();
    }
};
#endif // CONTROLLERS_H
