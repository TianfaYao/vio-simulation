#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <opencv2/opencv.hpp>
#include <feature.h>
class Camera
{
private:
    cv::Mat mask_img_;
    cv::Mat tracking_img_;
    std::map<size_t, Feature> cur_land_mark_;
    size_t features_id_;
    int max_features;
    std::string type;
    // 内参
    double track_noise_;
    double fx, fy, cx, cy;
    double k1, k2, k3, p1, p2;
    int width, height;
    int fps_;
    // 外参
    // from C(camera) to I(imu)
    Eigen::Vector3d p_I_C_;
    Eigen::Quaterniond q_I_C_;
    Eigen::Vector2d point2uv(Eigen::Vector3d p);
    Eigen::Vector3d transWorld2Camera(const Eigen::Quaterniond q_W_I, const Eigen::Vector3d p_W_I, const Eigen::Vector3d p_W);

public:
    //
    std::vector<cv::Point2f> features;

    //
    Camera();
    bool isFrontCmaera(Eigen::Vector2d uv)
    {
        bool is_x = uv.x() > 0 && uv.x() < width;
        bool is_y = uv.y() > 0 && uv.y() < height;

        return (is_x && is_y) ? true : false;
    }
    Eigen::Vector2d uv2norm(Eigen::Vector2d uv);
    void randFeature(void);
    Eigen::Vector3d randDepth(cv::Point2f uv, double min, double max);
    void readParam(std::string path);
    cv::Point2f randPoint(void);
    void track(const size_t fream_id, const Eigen::Quaterniond q_W_I, const Eigen::Vector3d p_W_I);
    int fps(void);
    Eigen::Vector3d p_I_C(void);
    Eigen::Quaterniond q_I_C(void);
    size_t features_id();
    void features_id_add();
    std::map<size_t, Feature> curLandMark(void);
    cv::Mat tracking_img();
    cv::Mat mask_img();
    double track_noise();
};
