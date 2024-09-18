#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/PointCloud.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <imu.h>
#include <camera.h>
#include <trajectory.h>
#include <simulation/uv.h>
#include <simulation/feature.h>
#include <util.h>
class Simulink
{
private:
    nav_msgs::Path ground_true_trajectory;
    ros::Publisher ground_true_trajectory_puber;
    ros::Publisher ground_true_pose_puber;
    ros::Publisher ground_true_imu_puber;
    ros::Publisher ground_true_points_puber;
    ros::Publisher ground_true_all_points_puber;
    ros::Publisher ground_true_feature_puber;
    tf::TransformBroadcaster world2Camera;
    // image_transport::Publisher tracking_img_puber;
    // image_transport::Publisher mask_img_puber;

    ros::Publisher tracking_img_puber;
    ros::Publisher mask_img_puber;
    //
    void trajectoryPublish(nav_msgs::Path &path, ros::Publisher &puber, Eigen::Quaterniond q_W_I, Eigen::Vector3d p_W_I);
    void posePublish(ros::Publisher &puber, Eigen::Quaterniond q_W_I, Eigen::Vector3d p_W_I);
    void imuPublish(ros::Publisher &puber, Eigen::Quaterniond q_W_I, Eigen::Vector3d acc, Eigen::Vector3d gyro);
    void pointsPublish(ros::Publisher &puber, sensor_msgs::PointCloud &cloud, std::vector<Eigen::Vector3d> p_Ws);
    void featurePublish(std::map<size_t, Feature> &cur_land_mark, Camera camera, size_t fream_id);

public:
    void spin(int argc, char **argv);
};

void Simulink::trajectoryPublish(nav_msgs::Path &path, ros::Publisher &puber, Eigen::Quaterniond q_W_I, Eigen::Vector3d p_W_I)
{
    path.header.frame_id = "map";
    path.header.stamp = ros::Time::now();
    geometry_msgs::PoseStamped pose_3d_stamped;
    pose_3d_stamped.header.frame_id = "map";
    pose_3d_stamped.header.stamp = ros::Time::now();
    pose_3d_stamped.pose.position.x = p_W_I.x();
    pose_3d_stamped.pose.position.y = p_W_I.y();
    pose_3d_stamped.pose.position.z = p_W_I.z();

    pose_3d_stamped.pose.orientation.w = q_W_I.w();
    pose_3d_stamped.pose.orientation.x = q_W_I.x();
    pose_3d_stamped.pose.orientation.y = q_W_I.y();
    pose_3d_stamped.pose.orientation.z = q_W_I.z();

    path.poses.push_back(pose_3d_stamped);
    puber.publish(path);
}

void Simulink::posePublish(ros::Publisher &puber, Eigen::Quaterniond q_W_I, Eigen::Vector3d p_W_I)
{
    geometry_msgs::PoseStamped pose_3d_stamped;
    pose_3d_stamped.header.frame_id = "map";
    pose_3d_stamped.header.stamp = ros::Time::now();
    pose_3d_stamped.pose.position.x = p_W_I.x();
    pose_3d_stamped.pose.position.y = p_W_I.y();
    pose_3d_stamped.pose.position.z = p_W_I.z();

    pose_3d_stamped.pose.orientation.w = q_W_I.w();
    pose_3d_stamped.pose.orientation.x = q_W_I.x();
    pose_3d_stamped.pose.orientation.y = q_W_I.y();
    pose_3d_stamped.pose.orientation.z = q_W_I.z();
    puber.publish(pose_3d_stamped);
}

void Simulink::imuPublish(ros::Publisher &puber, Eigen::Quaterniond q_W_I, Eigen::Vector3d acc, Eigen::Vector3d gyro)
{
    sensor_msgs::Imu imu;
    imu.header.frame_id = "map";
    imu.header.stamp = ros::Time::now();
    imu.orientation.w = q_W_I.w();
    imu.orientation.x = q_W_I.x();
    imu.orientation.y = q_W_I.y();
    imu.orientation.z = q_W_I.z();
    // acc
    imu.angular_velocity.x = gyro.x();
    imu.angular_velocity.y = gyro.y();
    imu.angular_velocity.z = gyro.z();
    // gyro
    imu.linear_acceleration.x = acc.x();
    imu.linear_acceleration.y = acc.y();
    imu.linear_acceleration.z = acc.z();
    puber.publish(imu);
}

void Simulink::pointsPublish(ros::Publisher &puber, sensor_msgs::PointCloud &cloud, std::vector<Eigen::Vector3d> p_Ws)
{

    cloud.header.stamp = ros::Time::now();
    cloud.header.frame_id = "map";
    cloud.channels.resize(1);
    cloud.channels[0].name = "rgb";
    cloud.channels[0].values.resize(p_Ws.size());
    cloud.points.resize(p_Ws.size());
    for (int i = 0; i < p_Ws.size(); ++i)
    {
        cloud.points[i].x = p_Ws[i].x();
        cloud.points[i].y = p_Ws[i].y();
        cloud.points[i].z = p_Ws[i].z();
    }
    puber.publish(cloud);
}
void Simulink::featurePublish(std::map<size_t, Feature> &cur_land_mark, Camera camera, size_t fream_id)
{
    simulation::feature features;
    features.stamp = ros::Time::now();
    for (auto l : cur_land_mark)
    {
        std::map<size_t, Eigen::Vector2d>::iterator it = l.second.obs.find(fream_id);
        if (it != l.second.obs.end())
        {
            simulation::uv uv;

            Eigen::Vector2d xy = camera.uv2norm(it->second);

            uv.id = l.first;
            // uv.x = it->second.x();
            // uv.y = it->second.y();

            uv.x = xy.x();
            uv.y = xy.y();

            features.data.push_back(uv);
        }
        else
        {
            std::cout << RED << "feature not connect for this fream" << std::endl;
        }
    }
    ground_true_feature_puber.publish(features);
}

void Simulink::spin(int argc, char **argv)
{
    std::cout << "argc:" << argc << std::endl;
    int plot_type = 0;
    if (argc >= 2)
    {
        plot_type = (argv[1][0] - '0');
        std::cout << "plot_type:" << plot_type << std::endl;
    }

    ros::NodeHandle n("~");
    std::cout << "imu:" << std::endl;
    Imu imu;
    imu.readParam("../cfg/imu.yaml");
    Camera camera;
    camera.readParam("../cfg/camera.yaml");
    Trajectory trajectory;
    double t = 0;
    ground_true_trajectory_puber = n.advertise<nav_msgs::Path>("/simulink/gt_path", 1);
    ground_true_pose_puber = n.advertise<geometry_msgs::PoseStamped>("/simulink/gt_pose", 1);
    ground_true_imu_puber = n.advertise<sensor_msgs::Imu>("/simulink/gt_imu", 1);
    ground_true_points_puber = n.advertise<sensor_msgs::PointCloud>("/simulink/gt_points", 1);
    ground_true_all_points_puber = n.advertise<sensor_msgs::PointCloud>("/simulink/gt_all_points", 1);
    ground_true_feature_puber = n.advertise<simulation::feature>("/simulink/features", 1);
    // image_transport::ImageTransport it(n);
    // tracking_img_puber = it.advertise("/simulink/tracking_img", 10);
    // mask_img_puber = it.advertise("/simulink/mask_img", 10);

    tracking_img_puber = n.advertise<sensor_msgs::Image>("/simulink/tracking_img", 10);
    mask_img_puber = n.advertise<sensor_msgs::Image>("/simulink/mask_img", 10);
    std::vector<Eigen::Vector3d> all_p_Ws;
    sensor_msgs::PointCloud all_points;
    size_t fream_id = 0;
    ros::Rate r(imu.fps());
    int count = 0;
    while (ros::ok())
    {
        bool met = r.sleep();
        ros::spinOnce();
        count++;
        t += 1.0 / static_cast<double>(imu.fps());
        trajectory.run(t, plot_type, 2.0);
        //
        Eigen::Vector3d p_W_I = trajectory.p_W_I();
        Eigen::Quaterniond q_W_I = trajectory.q_W_I();
        Eigen::Vector3d a = trajectory.acc();
        Eigen::Vector3d w = trajectory.gyro();
        imu.addNoise(a, w);
        trajectoryPublish(ground_true_trajectory, ground_true_trajectory_puber, q_W_I, p_W_I);
        posePublish(ground_true_pose_puber, q_W_I, p_W_I);
        imuPublish(ground_true_imu_puber, q_W_I, imu.am(), imu.wm());
        //时间没到直接跳过
        int rate = (imu.fps() / camera.fps());
        if (count % rate != 0)
        {
            continue;
        }
        fream_id++;
        //发布点云
        std::vector<Eigen::Vector3d> p_Ws;
        camera.track(fream_id, q_W_I, p_W_I);
        for (auto l : camera.curLandMark())
        {
            p_Ws.push_back(l.second.p_W());
            all_p_Ws.push_back(l.second.p_W());
        }
        sensor_msgs::PointCloud points;
        pointsPublish(ground_true_points_puber, points, p_Ws);
        pointsPublish(ground_true_all_points_puber, all_points, all_p_Ws);
        //发布相机到world tf 变换
        Eigen::Quaterniond q_W_C = q_W_I * camera.q_I_C();
        Eigen::Vector3d p_W_C = p_W_I + q_W_I * camera.p_I_C();
        tf::Transform camera2Woeld = tf::Transform(tf::Quaternion(q_W_C.x(), q_W_C.y(), q_W_C.z(), q_W_C.w()), tf::Vector3(p_W_C.x(), p_W_C.y(), p_W_C.z()));
        world2Camera.sendTransform(tf::StampedTransform(camera2Woeld, ros::Time::now(), "map", "cam"));
        //发布tracking img
        sensor_msgs::ImagePtr track_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", camera.tracking_img()).toImageMsg();
        tracking_img_puber.publish(track_msg);
        // 发布mask_img
        sensor_msgs::ImagePtr mask_msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", camera.mask_img()).toImageMsg();
        mask_img_puber.publish(mask_msg);
        //
        auto land_mark = camera.curLandMark();
        featurePublish(land_mark, camera, fream_id);
    }
}

int main(int argc, char **argv)
{
    std::cout << "argc:" << argc << std::endl;
    ros::init(argc, argv, "simulink");
    Simulink simulink;
    simulink.spin(argc, argv);
    return 0;
}