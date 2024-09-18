
#include <camera.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include <glog/logging.h>
#include <util.h>
#include <random>
#include <ctime>
#include <unistd.h>
Camera::Camera()
{
    features_id_ = 0;
}

void Camera::randFeature(void)
{
    cv::Mat img(height, width, CV_8UC1, cv::Scalar(255));
    int max_features = 150;
    int cunt = 0;
    features.clear();
    while (cunt++ < max_features)
    {
        features.emplace_back(randPoint());
    }
    // std::cout << "count:" << features.size() << std::endl;
    for (auto p : features)
    {
        cv::circle(img, p, 20, cv::Scalar(0), -1);
    }
    cv::imshow("img", img);
    cv::waitKey(30);
}
cv::Point2f Camera::randPoint(void)
{
    std::random_device rd_x;
    std::default_random_engine generator_x(rd_x());

    std::random_device rd_y;
    std::default_random_engine generator_y(rd_y());

    std::uniform_int_distribution<unsigned> rand_x(0.0 + 20, width - 20);
    std::uniform_int_distribution<unsigned> rand_y(0.0 + 20, height - 20);

    float x = rand_x(generator_x);
    float y = rand_y(generator_y);
    return cv::Point2f(x, y);
}

void Camera::track(const size_t fream_id, const Eigen::Quaterniond q_W_I, const Eigen::Vector3d p_W_I)
{
    std::vector<size_t> track_lost;
    std::vector<Eigen::Vector2d> cur_track;
    mask_img_ = cv::Mat(height, width, CV_8UC1, cv::Scalar(255));

    for (auto &l : cur_land_mark_)
    {
        Feature &feature = l.second;
        Eigen::Vector3d p_C = transWorld2Camera(q_W_I, p_W_I, feature.p_W());
        Eigen::Vector2d uv = point2uv(p_C);

        std::random_device rd;
        std::default_random_engine generator_(rd());
        std::normal_distribution<double> noise(0.0, 1.0);
        Eigen::Vector2d uv_noise(noise(generator_), noise(generator_));
        uv_noise = 0.5 * sqrt(track_noise()) * uv_noise;
        uv = uv + uv_noise;

        bool is_track = isFrontCmaera(uv);

        // 没有投影在相机前方
        if (p_C.z() < 0.01)
            is_track = false;
        // 成功
        if (is_track)
        {
            cur_track.emplace_back(uv);
            feature.obs.insert(std::make_pair(fream_id, uv));
        }
        else //失败
        {
            track_lost.emplace_back(l.first);
        }
    }
    //删除跟踪失败的
    for (size_t id : track_lost)
    {
        std::map<size_t, Feature>::iterator it = cur_land_mark_.find(id);
        if (it != cur_land_mark_.end())
        {
            cur_land_mark_.erase(it);
        }
    }
    // 添加mask
    for (auto p : cur_track)
    {
        cv::circle(mask_img_, cv::Point2f(p.x(), p.y()), 40, cv::Scalar(0), -1);
    }
    //  添加新的feature
    int need_add_feature = max_features - cur_land_mark_.size();
    std::cout << "need_add_feature:" << need_add_feature << std::endl;
    while (need_add_feature != 0)
    {
        cv::Point2f uv = randPoint();
        if (mask_img_.at<uchar>(uv) == 0)
        {
            continue;
        }
        else
        {

            // cv::circle(mask_img_, uv, 20, cv::Scalar(0), -1);
            need_add_feature--;
            features_id_add();
            Eigen::Vector3d p_C = randDepth(uv, 90, 100);
            Eigen::Vector3d p_I = p_I_C() + q_I_C() * p_C;
            // P_W=T_W_I*P_I
            Eigen::Vector3d p_W = p_W_I + q_W_I * p_I;
            // new feature
            size_t f_id = features_id();
            Eigen::Vector2d duv(double(uv.x), double(uv.y));
            Feature feature(f_id, fream_id, p_W, duv);
            //
            cur_land_mark_.insert(std::make_pair(f_id, feature));
        }
    }

    tracking_img_ = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
    for (auto l : cur_land_mark_)
    {
        if (l.second.obs.size() < 2)
            continue;
        // std::cout << "l.second.obs.size():" << l.second.obs.size() << std::endl;
        std::map<size_t, Eigen::Vector2d>::iterator it = l.second.obs.end();
        it--;
        cv::Point2d p1(it->second.x(), it->second.y());
        while (1)
        {
            it--;
            cv::Point2d p2(it->second.x(), it->second.y());
            // std::cout << "id:" << it->first << std::endl;
            // std::cout << "p1:" << p1 << std::endl;
            // std::cout << "p2:" << p2 << std::endl;
            cv::line(tracking_img_, p1, p2, cv::Scalar(125, 255 - 5 * l.second.obs.size(), 10 * l.second.obs.size()), 2);
            p1 = p2;
            if (it == l.second.obs.begin())
                break;
        }
        // std::cout << std::endl;
    }
    tracking_img_ = ~tracking_img_;
    // cv::waitKey(30);
}

Eigen::Vector3d Camera::randDepth(cv::Point2f uv, double min, double max)
{
    // 1/z*[u v  1]^T=[fx 0 cx ;
    //                  0 fy cy ;
    //                  0 0 1]*[ x y z]^T
    std::random_device rd_z;
    std::default_random_engine generator_z(rd_z());
    std::uniform_int_distribution<unsigned> rand_z(min * 1000.0, max * 1000.0);
    double z = rand_z(generator_z) / 1000.0;
    double x = (uv.x - cx) / fx;
    double y = (uv.y - cy) / fy;
    return Eigen::Vector3d(x * z, y * z, z);
}

Eigen::Vector2d Camera::point2uv(Eigen::Vector3d p)
{
    double u = fx * (p.x() / p.z()) + cx;
    double v = fy * (p.y() / p.z()) + cy;
    return Eigen::Vector2d(u, v);
}

Eigen::Vector2d Camera::uv2norm(Eigen::Vector2d uv)
{
    float x = (uv.x() - cx) / fx;
    float y = (uv.y() - cy) / fy;

    return Eigen::Vector2d(x, y);
}

Eigen::Vector3d Camera::transWorld2Camera(const Eigen::Quaterniond q_W_I, const Eigen::Vector3d p_W_I, const Eigen::Vector3d p_W)
{
    // p_I = T_I_W * p_W;
    // p_C = T_C_I * p_I;
    Eigen::Vector3d p_I = q_W_I.inverse() * p_W - q_W_I.inverse() * p_W_I;
    Eigen::Vector3d p_C = q_I_C().inverse() * p_I - q_I_C().inverse() * p_I_C();
    return p_C;
}
void Camera::readParam(std::string path)
{
    cv::FileStorage fs(path, cv::FileStorage::READ);
    if (!fs.isOpened())
    {
        std::cout<< "can not open file:" << path << std::endl;
        exit(0);
    }

    fs["camera.type"] >> type;
    fps_ = fs["camera.fps"];
    track_noise_ = fs["camera.track_noise"];
    max_features = fs["camera.max_features"];
    width = fs["camera.width"];
    height = fs["camera.height"];
    fx = fs["camera.fx"];
    fy = fs["camera.fy"];
    cx = fs["camera.cx"];
    cy = fs["camera.cy"];
    k1 = fs["camera.k1"];
    k2 = fs["camera.k2"];
    k3 = fs["camera.k3"];
    p1 = fs["camera.p1"];
    p2 = fs["camera.k2"];

    cv::Mat T_I_C = cv::Mat(4, 4, CV_64F);
    fs["camera.T_I_C"] >> T_I_C;
    Eigen::Matrix4d TIC4x4;
    cv::cv2eigen(T_I_C, TIC4x4);
    p_I_C_ = TIC4x4.block<3, 1>(0, 3);
    q_I_C_ = Eigen::Quaterniond(TIC4x4.block<3, 3>(0, 0));
    std::cout << "R_C_I:\n"
              << q_I_C_.inverse().toRotationMatrix() << std::endl;
    std::cout << "P_C_I:\n"
              << -(q_I_C_.inverse() * p_I_C_) << std::endl;

    std::cout << RED << "Start read camera param..." << std::endl;
    std::cout << RED << "type:" << type << std::endl;
    std::cout << RED << "track_noise:" << track_noise_ << std::endl;
    std::cout << RED << "max_features:" << max_features << std::endl;
    std::cout << RED << "fps:" << fps_ << std::endl;
    std::cout << RED << "width:" << width << std::endl;
    std::cout << RED << "height:" << height << std::endl;
    std::cout << RED << "fx:" << fx << std::endl;
    std::cout << RED << "fy:" << fy << std::endl;
    std::cout << RED << "cx:" << cx << std::endl;
    std::cout << RED << "cy:" << cy << std::endl;
    std::cout << RED << "k1:" << k1 << std::endl;
    std::cout << RED << "k2:" << k2 << std::endl;
    std::cout << RED << "k3:" << k3 << std::endl;
    std::cout << RED << "p1:" << p1 << std::endl;
    std::cout << RED << "p2:" << p2 << std::endl;
    std::cout << RED << "TIC4x4:" << TIC4x4 << std::endl;
    std::cout << RED << "Read camera param successfully" << NONE;
}

int Camera::fps(void)
{
    return fps_;
}
Eigen::Vector3d Camera::p_I_C(void)
{
    return p_I_C_;
};
Eigen::Quaterniond Camera::q_I_C(void)
{
    return q_I_C_;
};
size_t Camera::features_id()
{
    return features_id_;
}
void Camera::features_id_add()
{
    features_id_++;
}

std::map<size_t, Feature> Camera::curLandMark(void)
{
    return cur_land_mark_;
}
cv::Mat Camera::tracking_img()
{
    return tracking_img_;
}
cv::Mat Camera::mask_img()
{
    return mask_img_;
}
double Camera::track_noise()
{
    return track_noise_;
}