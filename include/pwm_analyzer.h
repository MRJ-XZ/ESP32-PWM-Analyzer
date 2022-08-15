#ifndef PWM_ANALYZER_H
#define PWM_ANALYZER_H

#include "Arduino.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"

#define MCPWM_INT_ENA(i) (volatile uint32_t *)MCMCPWM_INT_ENA_MCPWM_REG(i)
#define MCPWM_INT_CLR(i) (volatile uint32_t *)MCMCPWM_INT_CLR_MCPWM_REG(i)

class PWM_Analyzer{
    private:
        static int capture_number; // number of mcpwm captures being used
        int pwm_number;
        int MCPWM_unit;
        int input_pwm_pin;
        int frequency;
        int duty_cycle;
    public:
        PWM_Analyzer(int input_pwm_pin);
        PWM_Analyzer(int MCPWM_unit, int input_pwm_pin);
        uint32_t Get_PWM_frequency();
        double Get_PWM_duty_cycle();
        void Restart();
        void Stop();
        ~PWM_Analyzer(){};
};

#endif
