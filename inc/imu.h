#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>

class Imu
{
private:
    double time;
    // acc
    int fps_;
    Eigen::Vector3d am_;
    Eigen::Vector3d g_;
    Eigen::Vector3d ba_;
    double acc_noise_sigma_;
    double acc_bias_sigma_;
    // gyro
    Eigen::Vector3d wm_;
    Eigen::Vector3d bg_;
    double gyro_noise_sigma_;
    double gyro_bias_sigma_;
    //
public:
    Imu();
    void readParam(std::string path);
    void addNoise(Eigen::Vector3d acc, Eigen::Vector3d gyro);
    int fps(void);
    Eigen::Vector3d am(void);
    Eigen::Vector3d g(void);
    Eigen::Vector3d ba(void);
    Eigen::Vector3d wm(void);
    Eigen::Vector3d bg(void);
};
