#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>

#define NONE "\e[0m \n"   //清除颜色，之后的打印为正常输出，之前的不受影响
#define RED "\e[1;31m"    //鲜红
#define YELLOW "\e[1;33m" //鲜黄
#define BLUE "\e[0;34m"   //深蓝，暗蓝

class Util
{
private:
public:
    // euler2Rotation:   body frame to interitail frame
    static Eigen::Matrix3d euler2Rotation(Eigen::Vector3d eulerAngles)
    {
        double roll = eulerAngles(0);
        double pitch = eulerAngles(1);
        double yaw = eulerAngles(2);

        double cr = cos(roll);
        double sr = sin(roll);
        double cp = cos(pitch);
        double sp = sin(pitch);
        double cy = cos(yaw);
        double sy = sin(yaw);

        Eigen::Matrix3d RIb;
        RIb << cy * cp, cy * sp * sr - sy * cr, sy * sr + cy * cr * sp,
            sy * cp, cy * cr + sy * sr * sp, sp * sy * cr - cy * sr,
            -sp, cp * sr, cp * cr;
        return RIb;
    }

    static Eigen::Matrix3d eulerRates2bodyRates(Eigen::Vector3d eulerAngles)
    {
        double roll = eulerAngles(0);
        double pitch = eulerAngles(1);

        double cr = cos(roll);
        double sr = sin(roll);
        double cp = cos(pitch);
        double sp = sin(pitch);

        Eigen::Matrix3d R;
        R << 1, 0, -sp,
            0, cr, sr * cp,
            0, -sr, cr * cp;

        return R;
    }

    Util()
    {
    }
};
