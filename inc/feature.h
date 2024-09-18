#pragma once
#include <iostream>
#include <Eigen/Core>

class Feature
{
private:
    size_t id_;
    // Eigen::Vector3d p_C;
    Eigen::Vector3d p_W_;

public:
    std::map<size_t, Eigen::Vector2d> obs;
    Eigen::Vector3d p_W(void)
    {
        return p_W_;
    }
    Feature(size_t id, size_t fream_id, Eigen::Vector3d p_W, Eigen::Vector2d uv)
    {
        id_ = id;
        p_W_ = p_W;
        obs.insert(std::make_pair(fream_id, uv));
    }
};
