cmake_minimum_required(VERSION 2.8.3)
project(calibration_util)
#set(EXECUTABLE_OUTPUT_PATH  "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_PREFIX_PATH  ${CMAKE_PREFIX_PATH} "/opt/Qt5.9/lib/cmake/Qt5Widgets/")
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  dynamic_reconfigure
  image_transport
  message_filters
  message_generation
  roscpp
  rospy
  sensor_msgs
  std_msgs
  std_srvs
)
 
find_package(Qt5Widgets)
find_package(OpenCV 3 REQUIRED)

catkin_package(
   INCLUDE_DIRS include
#  LIBRARIES calibration_util
#  CATKIN_DEPENDS cv_bridge dynamic_reconfigure image_transport message_filters message_generation roscpp rospy sensor_msgs std_msgs std_srvs
#  DEPENDS system_lib
)

set(calibration_util_includes
               ${catkin_INCLUDE_DIRS}
               ${PROJECT_SOURCE_DIR}/include/calibration_util
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${calibration_util_includes})

FILE(GLOB_RECURSE CPP_SRC_LIST "src/*.cpp")

Qt5_WRAP_CPP(calibration_MOCS include/calibration_util/calibration_dialog.h)
Qt5_WRAP_UI(calibration_UIS src/calibration_dialog.ui)
add_executable(calibration_util_node
     ${CPP_SRC_LIST}
     ${calibration_UIS}
     ${calibration_MOCS}
)

target_link_libraries(calibration_util_node
    ${catkin_LIBRARIES}
    ${OpenCV_LIBS}
    #Qt5::Widgets}
)
qt5_use_modules(calibration_util_node Widgets)
 
