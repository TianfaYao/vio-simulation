#include <trajectory.h>
#include <util.h>

Trajectory::Trajectory()
{
}
void Trajectory::ellipse(double t)
{
    float ellipse_x = 20;
    float ellipse_y = 20;
    float z = 15;        // z轴做sin运动
    float K1 = 10;       // z轴的正弦频率是x，y的k1倍
    float K = M_PI / 10; // 20 * K = 2pi 　　由于我们采取的是时间是20s, 系数K控制yaw正好旋转一圈，运动一周

    // translation
    // twb:  body frame in world frame
    Eigen::Vector3d position(ellipse_x * cos(K * t), ellipse_y * sin(K * t), z * sin(K1 * K * t));
    Eigen::Vector3d dp(-K * ellipse_x * sin(K * t), K * ellipse_y * cos(K * t), z * K1 * K * cos(K1 * K * t)); // position导数　in world frame
    double K2 = K * K;
    Eigen::Vector3d ddp(-K2 * ellipse_x * cos(K * t), -K2 * ellipse_y * sin(K * t), -z * K1 * K1 * K2 * sin(K1 * K * t)); // position二阶导数

    // Rotation
    double k_roll = 0.1;
    double k_pitch = 0.2;
    Eigen::Vector3d eulerAngles(k_roll * cos(t), k_pitch * sin(t), K * t);   // roll ~ [-0.2, 0.2], pitch ~ [-0.3, 0.3], yaw ~ [0,2pi]
    Eigen::Vector3d eulerAnglesRates(-k_roll * sin(t), k_pitch * cos(t), K); // euler angles 的导数

    //    Eigen::Vector3d eulerAngles(0.0,0.0, K*t );   // roll ~ 0, pitch ~ 0, yaw ~ [0,2pi]
    //    Eigen::Vector3d eulerAnglesRates(0.,0. , K);      // euler angles 的导数
    // body frame in world frame
    p_W_I_ = position;
    Eigen::Matrix3d Rwb = Util::euler2Rotation(eulerAngles);
    // rotation form body frame to world frame
    q_W_I_ = Eigen::Quaterniond(Rwb);

    // anguler in body frame
    gyro_ = Util::eulerRates2bodyRates(eulerAngles) * eulerAnglesRates; //  euler rates trans to body gyro
    Eigen::Vector3d gn(0, 0, -9.81);                                    //  gravity in navigation frame(ENU)   ENU (0,0,-9.81)  NED(0,0,9,81)
    // acc
    acc_ = Rwb.transpose() * (ddp - gn); //  Rbw * Rwn * gn = gs
}

void Trajectory::circle(double t)
{
    float ellipse_x = 20;
    float ellipse_y = 20;
    float z = 0;         // z轴做sin运动
    float K1 = 10;       // z轴的正弦频率是x，y的k1倍
    float K = M_PI / 10; // 20 * K = 2pi 　　由于我们采取的是时间是20s, 系数K控制yaw正好旋转一圈，运动一周

    // translation
    // twb:  body frame in world frame
    Eigen::Vector3d position(ellipse_x * cos(K * t), ellipse_y * sin(K * t), z * sin(K1 * K * t));
    Eigen::Vector3d dp(-K * ellipse_x * sin(K * t), K * ellipse_y * cos(K * t), z * K1 * K * cos(K1 * K * t)); // position导数　in world frame
    double K2 = K * K;
    Eigen::Vector3d ddp(-K2 * ellipse_x * cos(K * t), -K2 * ellipse_y * sin(K * t), -z * K1 * K1 * K2 * sin(K1 * K * t)); // position二阶导数

    // Rotation
    // double k_roll = 0.1;
    // double k_pitch = 0.2;
    double k_roll = 0.;
    double k_pitch = 0.;
    Eigen::Vector3d eulerAngles(k_roll * cos(t), k_pitch * sin(t), K * t);   // roll ~ [-0.2, 0.2], pitch ~ [-0.3, 0.3], yaw ~ [0,2pi]
    Eigen::Vector3d eulerAnglesRates(-k_roll * sin(t), k_pitch * cos(t), K); // euler angles 的导数

    //    Eigen::Vector3d eulerAngles(0.0,0.0, K*t );   // roll ~ 0, pitch ~ 0, yaw ~ [0,2pi]
    //    Eigen::Vector3d eulerAnglesRates(0.,0. , K);      // euler angles 的导数
    // body frame in world frame
    p_W_I_ = position;
    Eigen::Matrix3d Rwb = Util::euler2Rotation(eulerAngles);
    // rotation form body frame to world frame
    q_W_I_ = Eigen::Quaterniond(Rwb);

    // anguler in body frame
    gyro_ = Util::eulerRates2bodyRates(eulerAngles) * eulerAnglesRates; //  euler rates trans to body gyro
    Eigen::Vector3d gn(0, 0, -9.81);                                    //  gravity in navigation frame(ENU)   ENU (0,0,-9.81)  NED(0,0,9,81)
    // acc
    acc_ = Rwb.transpose() * (ddp - gn); //  Rbw * Rwn * gn = gs
}

void Trajectory::line(double t)
{
    float ellipse_x = 20;
    float ellipse_y = 0;
    float z = 0;         // z轴做sin运动
    float K1 = 10;       // z轴的正弦频率是x，y的k1倍
    float K = M_PI / 10; // 20 * K = 2pi 　　由于我们采取的是时间是20s, 系数K控制yaw正好旋转一圈，运动一周
    static double last_t = 0;
    double dt = t - last_t;
    last_t = t;
    LIMIT(dt, 0, 0.1);
    double a_x = 0.5 * sin(K * t);
    static double v_x = 0;
    v_x = v_x + a_x * dt;
    static double p_x = 0;
    p_x = p_x + v_x * dt + 0.5 * a_x * dt * dt;

    // translation
    // twb:  body frame in world frame
    Eigen::Vector3d position(p_x, ellipse_y * sin(K * t), z * sin(K1 * K * t));
    Eigen::Vector3d dp(v_x, K * ellipse_y * cos(K * t), z * K1 * K * cos(K1 * K * t)); // position导数　in world frame
    double K2 = K * K;
    Eigen::Vector3d ddp(a_x, -K2 * ellipse_y * sin(K * t), -z * K1 * K1 * K2 * sin(K1 * K * t)); // position二阶导数

    // Rotation
    double k_roll = 0;
    double k_pitch = 0;

    // double k_roll = 0.1;
    // double k_pitch = 0.2;
    double k_yaw = 0.;
    Eigen::Vector3d eulerAngles(k_roll * cos(t), k_pitch * sin(t), k_yaw * sin(t));       // roll ~ [-0.2, 0.2], pitch ~ [-0.3, 0.3], yaw ~ [0,2pi]
    Eigen::Vector3d eulerAnglesRates(-k_roll * sin(t), k_pitch * cos(t), k_yaw * cos(t)); // euler angles 的导数

    // body frame in world frame
    p_W_I_ = position;
    Eigen::Matrix3d Rwb = Util::euler2Rotation(eulerAngles);
    // rotation form body frame to world frame
    q_W_I_ = Eigen::Quaterniond(Rwb);

    // anguler in body frame
    gyro_ = Util::eulerRates2bodyRates(eulerAngles) * eulerAnglesRates; //  euler rates trans to body gyro
    Eigen::Vector3d gn(0, 0, -9.81);                                    //  gravity in navigation frame(ENU)   ENU (0,0,-9.81)  NED(0,0,9,81)
    // acc
    acc_ = Rwb.transpose() * (ddp - gn); //  Rbw * Rwn * gn = gs
}

void Trajectory::init(int plot_type)
{
    double t = 0;
    switch (plot_type)
    {
    case 1:
        circle(t);
        break;
    case 2:
        line(t);
        break;
    default:
        ellipse(t);
    }
    //
    Eigen::Vector3d ddp;
    ddp.setZero();
    // anguler in body frame
    gyro_.setZero();                 //  euler rates trans to body gyro
    Eigen::Vector3d gn(0, 0, -9.81); //  gravity in navigation frame(ENU)   ENU (0,0,-9.81)  NED(0,0,9,81)
    // acc
    acc_ = q_W_I_.inverse() * (ddp - gn); //  Rbw * Rwn * gn = gs
}
void Trajectory::run(double &time, int plot_type, double init_time)
{
    //初始化阶段
    double t;
    if (time < init_time)
    {
        init(plot_type);
        return;
    }
    //motion阶段
    t = time - init_time;
    switch (plot_type)
    {
    case 1:
        circle(t);
        break;
    case 2:
        line(t);
        break;
    default:
        ellipse(t);
    }
}

Eigen::Quaterniond Trajectory::q_W_I(void)
{
    return q_W_I_;
}
Eigen::Vector3d Trajectory::p_W_I(void)
{
    return p_W_I_;
}
Eigen::Vector3d Trajectory::gyro(void)
{
    return gyro_;
}
Eigen::Vector3d Trajectory::acc(void)
{
    return acc_;
}
Eigen::Vector3d Trajectory::v_W_I(void)
{
    return v_W_I_;
}