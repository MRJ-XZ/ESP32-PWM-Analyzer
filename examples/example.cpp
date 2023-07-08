#include "Arduino.h"
#include "pwm_analyzer.h"

/*
    ESP32-PWM-Analyzer  is a program that analyzes an input pwm signal using MCPWM module.
    By: Mohammadreza jafari
    Date: July 8th, 2023
    License: This code is public domain so feel free to change any part of it.
    All suggestions are welcome.
*/


/*  Creating one object with no consideration so program itself setups modules.
    lets assume gpio35 is input signal pin.*/
PWM_Analyzer first_pwm(35);

/*  Creating another object with specifying MCPWM_UNIT. 
    lets assume we want to analyze input signal on gpio34 using MCPWM_UNIT_0. */
PWM_Analyzer second_pwm(34 , 0);

PWM_Analyzer* third;
void setup(){
    Serial.begin(9600);
    /*  Creating a dynamic object of PWM_Analyzer. specifying MCPWM_UNIT and capture_channel.*/
    third = new PWM_Analyzer(33 , 0 , 2);
    //  waiting a bit for signal to become more stable.
    vTaskDelay(2000);

    //  Reading the frequency of signal on gpio35. 
    Serial.println(first_pwm.Get_PWM_frequency());

    //  Reading the duty cycle of signal on gpio34.
    Serial.println(second_pwm.Get_PWM_duty_cycle());
    
    //  Stop analyzing temporarily.
    second_pwm.Stop();
}
int counter = 0;
bool flag = true;
void loop(){
    if(flag){
        if(third->Get_PWM_duty_cycle() > 70.0){
            /*  deleting object that you no longer need it. */
            third->~PWM_Analyzer();
            flag = false;
        }
    }
    if(counter > 10000)
        /*  Starting to analyze again. */
        second_pwm.Restart();
    Serial.println(second_pwm.Get_PWM_duty_cycle());
    counter++;
}