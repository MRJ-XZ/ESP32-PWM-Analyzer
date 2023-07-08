#include "pwm_analyzer.h"

uint8_t PWM_Analyzer::captures = 0b000000;

static bool IRAM_ATTR capture_isr(mcpwm_unit_t, mcpwm_capture_channel_id_t, const cap_event_data_t *data, void* user){
    PWM_Analyzer* pwm = (PWM_Analyzer*)user;
    if (data->cap_edge == MCPWM_POS_EDGE)
        pwm->pos_edge = data->cap_value;
    else
        pwm->neg_edge = data->cap_value;
    pwm->value = pwm->pos_edge - pwm->neg_edge;
    (pwm->value > 0) ? pwm->low = pwm->value : pwm->high = pwm->value;
    return pdTRUE;
}
void PWM_Analyzer::set_captures(uint8_t input){
    captures |= CAP_MASK << input;
}
void PWM_Analyzer::reset_captures(uint8_t input){
    uint8_t temp = ~captures | (CAP_MASK << input);
    captures = ~temp;
}
PWM_Analyzer::PWM_Analyzer(int _input_pwm_pin) : input_pwm_pin(_input_pwm_pin){
        for(int i=0;i<6;i++){
            if(captures >> i & CAP_MASK)
                continue;
            else{
                capture.cap_prescale = 1;
                capture.cap_edge = MCPWM_BOTH_EDGE;
                capture.capture_cb = capture_isr;
                capture.user_data = (void*)this;
                switch(i){
                    case 0:
                        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM_CAP_0, input_pwm_pin); 
                        mcpwm_capture_enable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP0, &capture);
                        this->MCPWM_unit = 0;
                        this->capture_channel = 0;
                        set_captures(0);
                        return;
                    case 1:
                        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM_CAP_1, input_pwm_pin); 
                        mcpwm_capture_enable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP1, &capture);
                        this->MCPWM_unit = 0;
                        this->capture_channel = 1;
                        set_captures(1);
                        return;
                    case 2:
                        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM_CAP_2, input_pwm_pin); 
                        mcpwm_capture_enable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP2, &capture);
                        this->MCPWM_unit = 0;
                        this->capture_channel = 2;
                        set_captures(2);
                        return;
                    case 3:
                        mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM_CAP_0, input_pwm_pin);  
                        mcpwm_capture_enable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP0, &capture);
                        this->MCPWM_unit = 1;
                        this->capture_channel = 0;
                        set_captures(3);
                        return;
                    case 4:
                        mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM_CAP_1, input_pwm_pin); 
                        mcpwm_capture_enable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP1, &capture);
                        this->MCPWM_unit = 1;
                        this->capture_channel = 1;
                        set_captures(4);
                        return;  
                    case 5:
                        mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM_CAP_2, input_pwm_pin); 
                        mcpwm_capture_enable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP2, &capture);
                        this->MCPWM_unit = 1;
                        this->capture_channel = 2;
                        set_captures(5);
                        return;                      
                }
            }
        }       
        log_e("All possible pwm analyzers are occupied .");
}
PWM_Analyzer::PWM_Analyzer(int _input_pwm_pin ,int _MCPWM_unit): MCPWM_unit(_MCPWM_unit) , input_pwm_pin(_input_pwm_pin)
{
    if (MCPWM_unit < 2 && MCPWM_unit > -1){
        if(!MCPWM_unit){
            for(int i=0;i<3;i++){
                if(captures >> i & CAP_MASK)
                    continue;
                else{
                    capture.cap_prescale = 1;
                    capture.cap_edge = MCPWM_BOTH_EDGE;
                    capture.capture_cb = capture_isr;   
                    capture.user_data = (void*)this;
                    switch(i){
                        case 0:
                            mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM_CAP_0, input_pwm_pin); 
                            mcpwm_capture_enable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP0, &capture);
                            this->MCPWM_unit = 0;
                            this->capture_channel = 0;
                            set_captures(0);
                            return;
                        case 1:
                            mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM_CAP_1, input_pwm_pin); 
                            mcpwm_capture_enable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP1, &capture);
                            this->MCPWM_unit = 0;
                            this->capture_channel = 1;
                            set_captures(1);
                            return;
                        case 2:
                            mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM_CAP_2, input_pwm_pin); 
                            mcpwm_capture_enable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP2, &capture);
                            this->MCPWM_unit = 0;
                            this->capture_channel = 2;
                            set_captures(2);
                            return;
                    }                 
                }
            }
            log_e("No free pwm analyzer in MCPWM_UNIT_0 is available.");
        }
        else{
            for(int i=0;i<3;i++){
                if(captures >> i+3 & CAP_MASK)
                    continue;
                else{
                    capture.cap_prescale = 1;
                    capture.cap_edge = MCPWM_BOTH_EDGE;
                    capture.capture_cb = capture_isr;  
                    capture.user_data = (void*)this; 
                    switch(i){
                        case 0:
                            mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM_CAP_0, input_pwm_pin); 
                            mcpwm_capture_enable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP0, &capture);
                            this->MCPWM_unit = 1;
                            this->capture_channel = 0;
                            set_captures(3);
                            return;
                        case 1:
                            mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM_CAP_1, input_pwm_pin); 
                            mcpwm_capture_enable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP1, &capture);
                            this->MCPWM_unit = 1;
                            this->capture_channel = 1;
                            set_captures(4);
                            return;
                        case 2:
                            mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM_CAP_2, input_pwm_pin); 
                            mcpwm_capture_enable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP2, &capture);
                            this->MCPWM_unit = 1;
                            this->capture_channel = 2;
                            set_captures(5);
                            return;
                    }                 
                }
            }
            log_e("No free pwm analyzer in MCPWM_UNIT_1 is available.");
        }
    }
    else
        log_e("Invalid MCPWM unit choice. Initialization ABORTED."); 
}
PWM_Analyzer::PWM_Analyzer(int _input_pwm_pin ,int _MCPWM_unit ,int _capture_channel)
    : MCPWM_unit(_MCPWM_unit) , input_pwm_pin(_input_pwm_pin) , capture_channel(_capture_channel)
{
    if((MCPWM_unit < 2 && MCPWM_unit > -1) && (capture_channel < 3 && capture_channel > -1)){
        if(captures >> (MCPWM_unit * 3 + capture_channel) & CAP_MASK)
            log_e("Selected MCPWM_UNIT and MCPWM_CAP have previous config. setting new config.");
        capture.cap_prescale = 1;
        capture.cap_edge = MCPWM_BOTH_EDGE;
        capture.capture_cb = capture_isr; 
        capture.user_data = (void*)this;

    }
}
uint32_t PWM_Analyzer::Get_PWM_frequency(){
    if(high == low == 0){
        log_e("No pwm signal detected.");
        return -1;
    }
    else{
        frequency = 80000000UL / (low - high);
        return frequency;
    }
}
double PWM_Analyzer::Get_PWM_duty_cycle(){ 
    if(high == low == 0){
        log_e("No pwm signal detected.");
        return -1;
    }
    else{
        duty_cycle = -high * 100.0 / (low - high);
        return duty_cycle;
    }
}
void PWM_Analyzer::Restart(){
    switch(MCPWM_unit * 3 + capture_channel){
        case 0:
            mcpwm_capture_enable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP0, &capture);
            return;
        case 1:
            mcpwm_capture_enable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP1, &capture);
            return;
        case 2:
            mcpwm_capture_enable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP2, &capture);
            return;
        case 3:
            mcpwm_capture_enable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP0, &capture);
            return;
        case 4:
            mcpwm_capture_enable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP1, &capture);
            return;
        case 5:
            mcpwm_capture_enable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP2, &capture);
            return;
    }      
}
void PWM_Analyzer::Stop(){
    switch(MCPWM_unit * 3 + capture_channel){
        case 0:
            mcpwm_capture_disable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP0);
            return;
        case 1:
            mcpwm_capture_disable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP1);
            return;
        case 2:
            mcpwm_capture_disable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP2);
            return;
        case 3:
            mcpwm_capture_disable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP0);
            return;
        case 4:
            mcpwm_capture_disable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP1);
            return;
        case 5:
            mcpwm_capture_disable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP2);
            return;
    }
}
PWM_Analyzer::~PWM_Analyzer(){
    switch(MCPWM_unit * 3 + capture_channel){
        case 0:
            mcpwm_capture_disable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP0);
            reset_captures(0);
            return;
        case 1:
            mcpwm_capture_disable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP1);
            reset_captures(1);
            return;
        case 2:
            mcpwm_capture_disable_channel(MCPWM_UNIT_0, MCPWM_SELECT_CAP2);
            reset_captures(2);
            return;
        case 3:
            mcpwm_capture_disable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP0);
            reset_captures(3);
            return;
        case 4:
            mcpwm_capture_disable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP1);
            reset_captures(4);
            return;
        case 5:
            mcpwm_capture_disable_channel(MCPWM_UNIT_1, MCPWM_SELECT_CAP2);
            reset_captures(5);
            return;
    }
}