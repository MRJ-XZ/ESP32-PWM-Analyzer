/*MIT License

Copyright (c) 2022 Mohammad Reza Jafari

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/
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
