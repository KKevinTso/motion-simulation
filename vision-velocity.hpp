#ifndef VISIONVELOCITY_HPP
#define VISIONVELOCITY_HPP
// For calculating bot, ball velocity (in ticks) from vision data
// useful for:
// * measuring actuation + transmission delay of velocity signal
// * measuring disparity in target and actual velocity at some point of time.
// *
#include "pose.h"
namespace VisionVelocity {
struct BotPose {
    float x, y;
    float theta;
    BotPose():x(0), y(0), theta(0) {}
    BotPose(float x, float y, float theta): x(x), y(y), theta(theta) {}
    // NOTE: Velocity calculation for simulation should only call this! it must NOT be called if not simulating!!!
    BotPose(const Pose &p): x(p.x()), y(p.y()), theta(p.theta()) {}
};

// takes in Strategy coordinates, returns in ticks
void calcBotVelocity(BotPose p1, BotPose p2, float timeMs, float &vl, float &vr);
// takes in Strategy coordinates, returns in Strategy coordinates per second.
void calcBallVelocity(int delx, int dely, float timeMs, float &vx, float &vy);
}

typedef VisionVelocity::BotPose BotPose;
//void calcBallVelocity(int px, int py, int nx, int ny, float )
#endif // VISIONVELOCITY_HPP
