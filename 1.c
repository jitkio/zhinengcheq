#include "stm32f10x.h"           
#include "bsp_GeneralTim.h"  
#include "delay.h"
#include "control.h"

#define centre_pwm     1570    // 舵机居中值，约1500左右

// PWM占空比定义，控制转向舵机和电机
#define angle_PWM      TIM2->CCR2   // 转向舵机PWM占空比，控制转向
#define motor1_PWM     TIM3->CCR1   // 1号电机PWM占空比，控制速度
#define motor2_PWM     TIM3->CCR2   // 2号电机PWM占空比，控制速度

// 新增两个传感器的定义，对应引脚需要根据实际连接修改
#define SENSOR_LEFT    PBin(3)      // 新增的最左侧传感器
#define SENSOR_RIGHT   PBin(9)      // 新增的最右侧传感器

int error = 0;     // 定义误差变量
int stop_flag = 0; // 定义停止标志
int stop_jishu;    // 定义计数器

// 转向舵机控制函数
void angle_control(int n)
{
    angle_PWM = centre_pwm + n;
}

// 主函数
int main(void)
{
    DelayInit();        // 延时函数初始化
    GENERAL_TIM_Init(); // 定时器初始化
    LED_Config();    
    angle_control(0);   
    
    motor1_PWM = 0; // 电机速度初始值
    motor2_PWM = 0;
    
    while(1)
    {      
        // 读取七个传感器的状态，简化示例为直接读取PBin(x)，实际应根据实际引脚调整
        int sensor_state[7] = {
            SENSOR_LEFT, 
            PBin(4), PBin(5), PBin(6), 
            PBin(7), PBin(8), 
            SENSOR_RIGHT
        };

        // 简化逻辑：避障和循迹结合
        // 根据传感器状态调整电机速度和方向
        // 这里只是一个基本示例，具体逻辑需要根据实际需求调整
        int left_speed = 60, right_speed = 60; // 默认速度
        for (int i = 0; i < 7; i++) {
            if (sensor_state[i] == 1) { // 检测到障碍物
                // 根据传感器位置调整速度或方向，下面的逻辑需要根据实际调整
                if (i < 3) { // 障碍物在左边
                    right_speed += 20; // 右转
                } else if (i > 3) { // 障碍物在右边
                    left_speed += 20; // 左转
                } else {
                    // 障碍物正前方，停车或调头逻辑，需要根据实际情况设计
                    left_speed = 0;
                    right_speed = 0;
                    break; // 退出循环
                }
            }
        }

        // 应用调整后的速度
        motor1_PWM = left_speed;
        motor2_PWM = right_speed;

        // 延时，根据需要调整
        DelayMs(100);
    }
}
