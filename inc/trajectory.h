#pragma once
#include <iostream>
#include <math.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

#define LIMIT(x, min, max)                     \
    {                                          \
        x = x < min ? min : x > max ? max : x; \
    }
    
class Trajectory
{
private:
    Eigen::Quaterniond q_W_I_;
    Eigen::Vector3d p_W_I_;
    Eigen::Vector3d gyro_;
    Eigen::Vector3d acc_;
    Eigen::Vector3d v_W_I_;

public:
    Trajectory();
    void init(int plot_type);
    void run(double &time, int plot_type, double init_time);
    void ellipse(double t);
    void circle(double t);
    void line(double t);
    Eigen::Quaterniond q_W_I(void);
    Eigen::Vector3d p_W_I(void);
    Eigen::Vector3d gyro(void);
    Eigen::Vector3d acc(void);
    Eigen::Vector3d v_W_I(void);
};
