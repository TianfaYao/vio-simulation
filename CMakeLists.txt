cmake_minimum_required(VERSION 2.8.12)
project(simulation)


add_compile_options(-std=c++11)
set(CMAKE_BUILD_TYPE Release)


set(os_path ${PROJECT_SOURCE_DIR}/../../../package/slam/)
message( "${PROJECT_SOURCE_DIR}:" ${PROJECT_SOURCE_DIR})
option(BULID_EXE " BULID_EXE" OFF)
option(BULID_SHARED " BULID_SHARED" OFF)

if(BULID_SHARED)
  set( BUILD_TYPE    SHARED) 	
else()
  set( BUILD_TYPE    STATIC) 
endif()
message("builld type:"${BUILD_TYPE})

find_package(Eigen3 REQUIRED)

find_package(OpenCV REQUIRED)
find_package(PCL REQUIRED)
# find_package (glog  REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  geometry_msgs
  pcl_ros
  tf
  message_filters
  cv_bridge
  image_transport
  eigen_conversions 
  tf_conversions 
  nav_msgs
  message_generation
)
add_message_files(
  FILES
  uv.msg
  feature.msg
)
generate_messages(DEPENDENCIES 
std_msgs
)

include_directories(
  inc
  src
  ${OpenCV_INCLUDE_DIR}
  ${EIGEN3_INCLUDE_DIR}
  ${CERES_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/thirdparty/generate_msgs/devel/include

)


link_directories(${PCL_LIBRARY_DIRS})
set(LINK_LIBS           ${OpenCV_LIBRARIES} ${PCL_LIBRARIES} ${catkin_LIBRARIES}  )
message("LINK_LIBS:", ${LINK_LIBS})



# add exe object 
add_executable(${PROJECT_NAME}_node 
src/imu.cc
src/camera.cc
src/main.cc 
src/trajectory.cc 
)

target_link_libraries(${PROJECT_NAME}_node
  ${LINK_LIBS}
)


