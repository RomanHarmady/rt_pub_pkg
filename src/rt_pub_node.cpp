

#include <iostream>
#include <chrono>
#include <memory>
#include <cstdint>

#include "rttest/rttest.h"
#include "rclcpp/rclcpp.hpp"

#include "std_msgs/msg/int16.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/int64.hpp"





using namespace std::chrono_literals;
using std::placeholders::_1;

uint8_t P_GAIN = 30;
uint32_t pos;
auto message = std_msgs::msg::Int16();
uint16_t mess;



class RTPublisher : public rclcpp::Node
{
public:

    RTPublisher(): Node("rt_pub_node"), count(0)            
    {
        publisher_ = this->create_publisher<std_msgs::msg::Int16>("log_publisher", 10);
        subscriber_ = this->create_subscription<std_msgs::msg::Int16>("encoder_subscriber", 10, std::bind(&RTPublisher::callback, this, std::placeholders::_1));

        timer_ = this->create_wall_timer(1ms, std::bind(&RTPublisher::timer_callback, this));
    }

private:
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<std_msgs::msg::Int16>::SharedPtr subscriber_;
    rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr publisher_;
    size_t count;

    void callback(const std_msgs::msg::Int16::SharedPtr msg)
  {
    pos = msg->data;
  }


    void timer_callback() {                     
        auto start_time = std::chrono::high_resolution_clock::now();
        message.data = 0;
        if(pos >= 30){
            message.data = pos * P_GAIN;
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto execution_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
       // RCLCPP_INFO(get_logger(), "miliseconds: %ld", execution_time);
        message.data = execution_time.count();
        publisher_->publish(message);
        
        
        
    }   
};

int main(int argc, char * argv[])
{
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

    rclcpp::init(argc,argv);
    auto node = std::make_shared<RTPublisher>();

    int cpu_id = 0;
    cpu_set_t cpuset;
    auto spin_thread = std::thread(
        [&](){
            
            rclcpp::spin(node);
        }
    );
    const pid_t thread_ID = getpid();
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_id, &cpuset);
    sched_setaffinity(thread_ID, sizeof(cpu_set_t), &cpuset);
    
    rttest_set_sched_priority(98, SCHED_RR);
    
    spin_thread.join();
    rclcpp::shutdown();
    return 0;
}

