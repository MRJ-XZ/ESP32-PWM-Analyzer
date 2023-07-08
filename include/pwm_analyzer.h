#ifndef PWM_ANALYZER_H
#define PWM_ANALYZER_H

#include "Arduino.h"
#include "driver/mcpwm.h"
#define CAP_MASK 0x1

static bool capture_isr(mcpwm_unit_t,mcpwm_capture_channel_id_t,const cap_event_data_t*,void*);
class PWM_Analyzer{
    private:
        mcpwm_capture_config_t capture;
        uint8_t MCPWM_unit = -1;
        uint8_t capture_channel = -1;
        uint8_t input_pwm_pin;
        int frequency;
        double duty_cycle;
        long neg_edge = 0,
            pos_edge = 0,
            value = 0;
        int high = 0,
            low = 0;
        static uint8_t captures;
        void set_captures(uint8_t);
        void reset_captures(uint8_t);
    public:
        friend bool IRAM_ATTR capture_isr(mcpwm_unit_t,mcpwm_capture_channel_id_t,const cap_event_data_t*,void*);    
        PWM_Analyzer(int input_pwm_pin);
        PWM_Analyzer(int input_pwm_pin ,int MCPWM_unit);
        PWM_Analyzer(int input_pwm_pin ,int MCPWM_unit ,int capture_channel);
        uint32_t Get_PWM_frequency();
        double Get_PWM_duty_cycle();
        void Restart();
        void Stop();
        ~PWM_Analyzer();
};

#endif
