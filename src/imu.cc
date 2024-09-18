#include <imu.h>
#include <opencv2/opencv.hpp>
#include <glog/logging.h>
#include <util.h>
#include <random>
Imu::Imu()
{
    fps_ = 200;
    am_.setZero();
    g_.setZero();
    ba_.setZero();
    acc_noise_sigma_ = 0;
    acc_bias_sigma_ = 0;
    // gyro
    wm_.setZero();
    bg_.setZero();
    gyro_noise_sigma_ = 0;
    gyro_bias_sigma_ = 0;
}
void Imu::readParam(std::string path)
{
    cv::FileStorage fs(path, cv::FileStorage::READ);
    if (!fs.isOpened())
    {
         std::cout << "can not open file:" << path << std::endl;
        exit(0);
    }
    std::cout << RED << "start read imu param from :" << path << NONE;
    //    acc
    fs["imu.fps"] >> fps_;
    fs["imu.ba"][0] >> ba_[0];
    fs["imu.ba"][1] >> ba_[1];
    fs["imu.ba"][2] >> ba_[2];

    fs["imu.g"][0] >> g_[0];
    fs["imu.g"][1] >> g_[1];
    fs["imu.g"][2] >> g_[2];

    fs["imu.acc_noise_sigma"] >> acc_noise_sigma_;
    fs["imu.acc_bias_sigma"] >> acc_bias_sigma_;
    //     gyro
    fs["imu.bg"][0] >> bg_[0];
    fs["imu.bg"][1] >> bg_[1];
    fs["imu.bg"][2] >> bg_[2];
    fs["imu.gyro_noise_sigma"] >> gyro_noise_sigma_;
    fs["imu.gyro_bias_sigma"] >> gyro_bias_sigma_;
    // acc
    std::cout << RED << "fps:" << fps_ << NONE;
    std::cout << RED << "ba:" << ba_.transpose() << NONE;
    std::cout << RED << "g:" << g_.transpose() << NONE;
    std::cout << RED << "acc_noise_sigma:" << acc_noise_sigma_ << NONE;
    std::cout << RED << "acc_bias_sigma:" << acc_bias_sigma_ << NONE;
    // gyro
    std::cout << RED << "bg:" << bg_.transpose() << NONE;
    std::cout << RED << "gyro_noise_sigma:" << gyro_noise_sigma_ << NONE;
    std::cout << RED << "gyro_bias_sigma:" << gyro_bias_sigma_ << NONE;
    std::cout << RED << "successfully read imu param. " << NONE;
}
// acc 无噪声加速度
// gyro 无噪声角速度
void Imu::addNoise(Eigen::Vector3d acc, Eigen::Vector3d gyro)
{
    std::random_device rd;
    std::default_random_engine generator_(rd());
    std::normal_distribution<double> noise(0.0, 1.0);
    double inv_sqrt_stampe = sqrt(1.0 / fps());
    // 角速度
    Eigen::Vector3d noise_gyro(noise(generator_), noise(generator_), noise(generator_));
    Eigen::Matrix3d gyro_sqrt_cov = gyro_noise_sigma_ * Eigen::Matrix3d::Identity();
    wm_ = gyro + gyro_sqrt_cov * noise_gyro / inv_sqrt_stampe + bg();
    //角速度biase
    Eigen::Vector3d noise_gyro_bias(noise(generator_), noise(generator_), noise(generator_));
    bg_ += gyro_noise_sigma_ * inv_sqrt_stampe * noise_gyro_bias;
    //线加速度
    Eigen::Vector3d noise_acc(noise(generator_), noise(generator_), noise(generator_));
    Eigen::Matrix3d acc_sqrt_cov = acc_noise_sigma_ * Eigen::Matrix3d::Identity();
    am_ = acc + acc_sqrt_cov * noise_acc / inv_sqrt_stampe + ba();
    //线加速度biase
    Eigen::Vector3d noise_acc_bias(noise(generator_), noise(generator_), noise(generator_));
    bg_ += acc_noise_sigma_ * inv_sqrt_stampe * noise_acc_bias;
}

int Imu::fps(void)
{
    return fps_;
}
Eigen::Vector3d Imu::am(void)
{
    return am_;
}
Eigen::Vector3d Imu::g(void)
{
    return am_;
}
Eigen::Vector3d Imu::ba(void)
{
    return ba_;
}
Eigen::Vector3d Imu::wm(void)
{
    return wm_;
}
Eigen::Vector3d Imu::bg(void)
{
    return bg_;
}