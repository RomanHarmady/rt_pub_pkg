cmake_minimum_required(VERSION 3.5)
project(rt_pubsub_pkg)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies

find_package(rclcpp REQUIRED)
find_package(rttest REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
#find_package(rosidl_default_generators REQUIRED)

set(dependencies  "rclcpp")
add_executable(rt_pub_node src/rt_pub_node.cpp)





ament_target_dependencies(rt_pub_node rclcpp rttest std_msgs)

install(TARGETS
        rt_pub_node

        DESTINATION lib/${PROJECT_NAME}
        )

ament_package()
