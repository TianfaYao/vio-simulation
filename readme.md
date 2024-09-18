<!--
 * @Descripttion: 
 * @version: 1.0
 * @Author: Tianfa
 * @Date: 2024-09-17 09:52:20
 * @LastEditors: Tianfa
 * @LastEditTime: 2024-09-18 04:45:13
-->
##  VIO Monte Carlo Simulation

### 1.INTRODUCTION
  - 构建仿真环境的核心在于：
    - 设计并解析运动轨迹，以提取关键运动参数（包括位姿、线速度和特征）。
    - 利用这些参数进行系统初始化。
    - 通过持续跟踪，恢复相机轨迹、定位空间地标、估计IMU偏差、校准IMU与相机外参 
    - 的外参以及相机的内参。
    - 利用仿真数据评估算法，指导其迭代优化。
    - 验证传感器噪声的可接受范围，辅助传感器的选择。

### 2.RELATED WORK

### 3.RESULT
- 加速度曲线
![img](doc/img/acc_noise.png)
- 角速度曲线
![img](doc/img/anguler.png)
- 做圆周运动+pitch+roll 正弦运动仿真
![img](doc/img/circle.png)
- 纯圆周运动仿真
- 直线运动
![img](doc/img/ol.png)
![img](doc/img/ocircle.png)
- 直线运动+pitch+roll 正弦运动仿真
![img](doc/img/l.png)
- 直线加z轴正弦运动+pitch+roll正弦运动
![img](doc/img/lz.png)
- 直线加z轴正弦运动+pitch+roll正弦运动+y余弦运动
![img](doc/img/yz_plot.png)
- 直线加z轴正弦运动
![img](doc/img/z_plot.png)
- xy椭圆+z正弦+roll pitch正弦运动
![img](doc/img/vio-simulink.png)

### 4.REFERENCE




### 5.APPENDIX


    run
        mkdir build 
        cmake ..
        make 
        source  devel/setup.bash 
        rosrun simulation  simulation_node 0 # 0:all 1:circle  2:line
    rosrun rviz rviz -d simulink.rviz

    note:刚开始5s系统处于静止状态,可以作为静止初始化