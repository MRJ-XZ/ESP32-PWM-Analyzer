#include "pwm_analyzer.h"

int PWM_Analyzer::capture_number = 0;
int32_t pos_edge0 = 0,
    neg_edge0 = 0, 
    pos_edge1 = 0,
    neg_edge1 = 0,
    pos_edge2 = 0,
    neg_edge2 = 0,
    value0 = 0,
    value1 = 0,
    value2 = 0,
    low0 = 0,
    high0 = 0,
    low1 = 0,
    high1 = 0,
    low2 = 0,
    high2 = 0;
static void IRAM_ATTR capture_isr0(void*){
    pos_edge0 =  mcpwm_capture_signal_get_value(MCPWM_UNIT_0, MCPWM_SELECT_CAP0);
    neg_edge0 = mcpwm_capture_signal_get_value(MCPWM_UNIT_0, MCPWM_SELECT_CAP1);
    value0 = pos_edge0 - neg_edge0;
    (value0 > 0) ? low0 = value0 : high0 = value0;
}
static void IRAM_ATTR capture_isr1(void*){
    pos_edge1 =  mcpwm_capture_signal_get_value(MCPWM_UNIT_1, MCPWM_SELECT_CAP0);
    neg_edge1 = mcpwm_capture_signal_get_value(MCPWM_UNIT_1, MCPWM_SELECT_CAP1);
    value1 = pos_edge1 - neg_edge1;
    (value1 > 0) ? low1 = value1 : high1 = value1;
}
static void IRAM_ATTR capture_isr2(void*){
    pos_edge2 =  mcpwm_capture_signal_get_value(MCPWM_UNIT_0, MCPWM_SELECT_CAP2);
    neg_edge2 = mcpwm_capture_signal_get_value(MCPWM_UNIT_1, MCPWM_SELECT_CAP2);
    value2 = pos_edge2 - neg_edge2;
    (value2 > 0) ? low2 = value2 : high2 = value2;
}
PWM_Analyzer::PWM_Analyzer(int _input_pwm_pin) : input_pwm_pin(_input_pwm_pin){
    switch(capture_number){
        case 0:
            mcpwm_gpio_init(MCPWM_UNIT_0 , MCPWM_CAP_0 , input_pwm_pin);
            mcpwm_gpio_init(MCPWM_UNIT_0 , MCPWM_CAP_1 , input_pwm_pin);
            mcpwm_capture_enable(MCPWM_UNIT_0, MCPWM_SELECT_CAP0, MCPWM_POS_EDGE, 0);
            mcpwm_capture_enable(MCPWM_UNIT_0, MCPWM_SELECT_CAP1, MCPWM_NEG_EDGE, 0);
            mcpwm_isr_register(MCPWM_UNIT_0, capture_isr0 , nullptr, ESP_INTR_FLAG_IRAM, nullptr);
            *MCPWM_INT_ENA(0) = MCPWM_CAP0_INT_ENA | MCPWM_CAP1_INT_ENA;
            capture_number = 1;
            pwm_number = 0;
            break;
        case 1:
            mcpwm_gpio_init(MCPWM_UNIT_1 , MCPWM_CAP_0 , input_pwm_pin);
            mcpwm_gpio_init(MCPWM_UNIT_1 , MCPWM_CAP_1 , input_pwm_pin);
            mcpwm_capture_enable(MCPWM_UNIT_1, MCPWM_SELECT_CAP0, MCPWM_POS_EDGE, 0);
            mcpwm_capture_enable(MCPWM_UNIT_1, MCPWM_SELECT_CAP1, MCPWM_NEG_EDGE, 0);
            mcpwm_isr_register(MCPWM_UNIT_1, capture_isr1 , nullptr, ESP_INTR_FLAG_IRAM, nullptr);
            *MCPWM_INT_ENA(1) = MCPWM_CAP0_INT_ENA | MCPWM_CAP1_INT_ENA;
            capture_number = 2;
            pwm_number = 1;
            break;
        case 2:
            mcpwm_gpio_init(MCPWM_UNIT_0 , MCPWM_CAP_0 , input_pwm_pin);
            mcpwm_gpio_init(MCPWM_UNIT_0 , MCPWM_CAP_1 , input_pwm_pin);
            mcpwm_capture_enable(MCPWM_UNIT_0, MCPWM_SELECT_CAP0, MCPWM_POS_EDGE, 0);
            mcpwm_capture_enable(MCPWM_UNIT_0, MCPWM_SELECT_CAP1, MCPWM_NEG_EDGE, 0);
            mcpwm_isr_register(MCPWM_UNIT_0, capture_isr0 , nullptr, ESP_INTR_FLAG_IRAM, nullptr);
            *MCPWM_INT_ENA(0) = MCPWM_CAP0_INT_ENA | MCPWM_CAP1_INT_ENA;
            capture_number = 3;
            pwm_number = 0;
            break;
        case 3:
            mcpwm_gpio_init(MCPWM_UNIT_0 , MCPWM_CAP_2 , input_pwm_pin);
            mcpwm_gpio_init(MCPWM_UNIT_1 , MCPWM_CAP_2 , input_pwm_pin);
            mcpwm_capture_enable(MCPWM_UNIT_0, MCPWM_SELECT_CAP2, MCPWM_POS_EDGE, 0);
            mcpwm_capture_enable(MCPWM_UNIT_1, MCPWM_SELECT_CAP2, MCPWM_NEG_EDGE, 0);
            mcpwm_isr_register(MCPWM_UNIT_0, capture_isr2 , nullptr, ESP_INTR_FLAG_IRAM, nullptr);
            *MCPWM_INT_ENA(0) = MCPWM_CAP2_INT_ENA; 
            capture_number = 4;
            pwm_number = 2;          
        default:
            Serial.println("All possible pwm analyzers are occupied .");
            break;
    }
}
PWM_Analyzer::PWM_Analyzer(int _MCPWM_unit , int _input_pwm_pin): MCPWM_unit(_MCPWM_unit) , input_pwm_pin(_input_pwm_pin)
{
    if (MCPWM_unit < 2 && MCPWM_unit > -1){
        if(!MCPWM_unit){
            mcpwm_gpio_init(MCPWM_UNIT_0 , MCPWM_CAP_0 , input_pwm_pin);
            mcpwm_gpio_init(MCPWM_UNIT_0 , MCPWM_CAP_1 , input_pwm_pin);
            mcpwm_capture_enable(MCPWM_UNIT_0, MCPWM_SELECT_CAP0, MCPWM_POS_EDGE, 0);
            mcpwm_capture_enable(MCPWM_UNIT_0, MCPWM_SELECT_CAP1, MCPWM_NEG_EDGE, 0);
            mcpwm_isr_register(MCPWM_UNIT_0, capture_isr0 , nullptr, ESP_INTR_FLAG_IRAM, nullptr);
            *MCPWM_INT_ENA(0) = MCPWM_CAP0_INT_ENA | MCPWM_CAP1_INT_ENA;
            capture_number |= 1;
            pwm_number = 0;
        }
        else{
            mcpwm_gpio_init(MCPWM_UNIT_1 , MCPWM_CAP_0 , input_pwm_pin);
            mcpwm_gpio_init(MCPWM_UNIT_1 , MCPWM_CAP_1 , input_pwm_pin);
            mcpwm_capture_enable(MCPWM_UNIT_1, MCPWM_SELECT_CAP0, MCPWM_POS_EDGE, 0);
            mcpwm_capture_enable(MCPWM_UNIT_1, MCPWM_SELECT_CAP1, MCPWM_NEG_EDGE, 0);
            mcpwm_isr_register(MCPWM_UNIT_1, capture_isr1 , nullptr, ESP_INTR_FLAG_IRAM, nullptr);
            *MCPWM_INT_ENA(1) = MCPWM_CAP0_INT_ENA | MCPWM_CAP1_INT_ENA;            
            capture_number |= 2;
            pwm_number = 1;
        }
    }
    else
        Serial.println("Warning : invalid MCPWM unit choice."); 
}
uint32_t PWM_Analyzer::Get_PWM_frequency(){
    switch(pwm_number){
        case 0:
            frequency = 80000000UL / (low0 - high0);
            return frequency;
        case 1:
            frequency = 80000000UL / (low1 - high1);
            return frequency;
        case 2:
            frequency = 80000000UL / (low2 - high2);
            return frequency;
    }
    return 0;
}
double PWM_Analyzer::Get_PWM_duty_cycle(){
    switch(pwm_number){
        case 0:
            duty_cycle = -high0 * 100 / (low0 - high0);
            return duty_cycle;
        case 1:
            duty_cycle = -high1 * 100 / (low1 - high1);
            return duty_cycle;
        case 2:
            duty_cycle = -high2 * 100 / (low2 - high2);
            return duty_cycle;
    }    
    return 0;
}
void PWM_Analyzer::Restart(){
    switch(pwm_number){
        case 0:
            mcpwm_capture_enable(MCPWM_UNIT_0, MCPWM_SELECT_CAP0, MCPWM_POS_EDGE, 0);
            mcpwm_capture_enable(MCPWM_UNIT_0, MCPWM_SELECT_CAP1, MCPWM_NEG_EDGE, 0);
            *MCPWM_INT_ENA(0) = MCPWM_CAP0_INT_ENA | MCPWM_CAP1_INT_ENA;
            break;
        case 1:
            mcpwm_capture_enable(MCPWM_UNIT_1, MCPWM_SELECT_CAP0, MCPWM_POS_EDGE, 0);
            mcpwm_capture_enable(MCPWM_UNIT_1, MCPWM_SELECT_CAP1, MCPWM_NEG_EDGE, 0);
            *MCPWM_INT_ENA(1) = MCPWM_CAP0_INT_ENA | MCPWM_CAP1_INT_ENA;
            break;
        case 2:
            mcpwm_capture_enable(MCPWM_UNIT_0, MCPWM_SELECT_CAP2, MCPWM_POS_EDGE, 0);
            mcpwm_capture_enable(MCPWM_UNIT_1, MCPWM_SELECT_CAP2, MCPWM_NEG_EDGE, 0);
            *MCPWM_INT_ENA(0) = MCPWM_CAP2_INT_ENA;
            break; 
    }       
}
void PWM_Analyzer::Stop(){
    switch(pwm_number){
        case 0:
            mcpwm_capture_disable(MCPWM_UNIT_0, MCPWM_SELECT_CAP0);
            mcpwm_capture_disable(MCPWM_UNIT_0, MCPWM_SELECT_CAP1);
            *MCPWM_INT_ENA(0) = 0;
            break;
        case 1:
            mcpwm_capture_disable(MCPWM_UNIT_1, MCPWM_SELECT_CAP0);
            mcpwm_capture_disable(MCPWM_UNIT_1, MCPWM_SELECT_CAP1);
            *MCPWM_INT_ENA(1) = 0;
            break;
        case 2:
            mcpwm_capture_disable(MCPWM_UNIT_0, MCPWM_SELECT_CAP2);
            mcpwm_capture_disable(MCPWM_UNIT_1, MCPWM_SELECT_CAP2);
            *MCPWM_INT_ENA(0) = 0;
            break; 
    }   
}