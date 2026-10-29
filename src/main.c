#include "MCAL/RCC/RCC_interface.h"
#include"MCAL/GPIO/GPIO_interface.h"

u16 buttonPressed(void)
{
    gpio_pinState_t  A,B,C;

    GPIO_enu_PinRead(GPIO_PORTB, GPIO_PIN0, &A);
    GPIO_enu_PinRead(GPIO_PORTB, GPIO_PIN1, &B);
    GPIO_enu_PinRead(GPIO_PORTB, GPIO_PIN3, &C);

    for(volatile u32 i =0; i<100000 ; i++);

    u16 value = 0;
    if(A == GPIO_LOW) value += 1;
    if(B == GPIO_LOW) value += 2;
    if(C == GPIO_LOW) value += 4;

    return value;
}

void LED(u16 value)
{
    gpio_pinState_t PIN;
  
    GPIO_enu_PinRead(GPIO_PORTA, value, &PIN);
    if(PIN == GPIO_HIGH)
    {
         GPIO_enu_PinWrite(GPIO_PORTA, value,GPIO_LOW);
    }
    else
    {
        GPIO_enu_PinWrite(GPIO_PORTA, value,GPIO_HIGH);
    }
}



int main(){
    rcc_error_t res;

    res = rcc_ctrl_peripheral_clk(RCC_GPIOA, RCC_ENABLE);
    if (res != RCC_OK) return -1;

    res = rcc_ctrl_peripheral_clk(RCC_GPIOB, RCC_ENABLE);
    if (res != RCC_OK) return -1;

    res = rcc_ctrl_peripheral_clk(RCC_GPIOC, RCC_ENABLE);
    if (res != RCC_OK) return -1;


    gpio_struct_pin_config_t inita1 = {
        .port = GPIO_PORTA,
        .pin = GPIO_PIN1,
        .mode = GPIO_OUTPUT_MODE,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_OUTPUT_SPEED_MEDIUM,
        .alternateFunction = GPIO_AF0
    };

    gpio_struct_pin_config_t inita2 = {
        .port = GPIO_PORTA,
        .pin = GPIO_PIN2,
        .mode = GPIO_OUTPUT_MODE,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_OUTPUT_SPEED_MEDIUM,
        .alternateFunction = GPIO_AF0
    };

    gpio_struct_pin_config_t inita3 = {
        .port = GPIO_PORTA,
        .pin = GPIO_PIN3,
        .mode = GPIO_OUTPUT_MODE,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_OUTPUT_SPEED_MEDIUM,
        .alternateFunction = GPIO_AF0
    };

    gpio_struct_pin_config_t inita4 = {
        .port = GPIO_PORTA,
        .pin = GPIO_PIN4,
        .mode = GPIO_OUTPUT_MODE,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_OUTPUT_SPEED_MEDIUM,
        .alternateFunction = GPIO_AF0
    };

    gpio_struct_pin_config_t inita5 = {
        .port = GPIO_PORTA,
        .pin = GPIO_PIN5,
        .mode = GPIO_OUTPUT_MODE,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_OUTPUT_SPEED_MEDIUM,
        .alternateFunction = GPIO_AF0
    };

    gpio_struct_pin_config_t inita6 = {
        .port = GPIO_PORTA,
        .pin = GPIO_PIN6,
        .mode = GPIO_OUTPUT_MODE,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_OUTPUT_SPEED_MEDIUM,
        .alternateFunction = GPIO_AF0
    };

    gpio_struct_pin_config_t inita7 = {
        .port = GPIO_PORTA,
        .pin = GPIO_PIN7,
        .mode = GPIO_OUTPUT_MODE,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_OUTPUT_SPEED_MEDIUM,
        .alternateFunction = GPIO_AF0
    };

    gpio_struct_pin_config_t initb0 = {
        .port = GPIO_PORTB,
        .pin = GPIO_PIN0,
        .mode = GPIO_INPUT_MODE,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_OUTPUT_SPEED_MEDIUM,
        .pup_pdown = GPIO_PULLUP,
        .alternateFunction = GPIO_AF0
    };

    gpio_struct_pin_config_t initb1 = {
        .port = GPIO_PORTB,
        .pin = GPIO_PIN1,
        .mode = GPIO_INPUT_MODE,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_OUTPUT_SPEED_MEDIUM,
        .pup_pdown = GPIO_PULLUP,
        .alternateFunction = GPIO_AF0
    };

    gpio_struct_pin_config_t initb2 = {
        .port = GPIO_PORTB,
        .pin = GPIO_PIN3,
        .mode = GPIO_INPUT_MODE,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_OUTPUT_SPEED_MEDIUM,
        .pup_pdown = GPIO_PULLUP,
        .alternateFunction = GPIO_AF0
    };

    GPIO_enu_PinInit(&inita1);
    GPIO_enu_PinInit(&inita2);
    GPIO_enu_PinInit(&inita3);
    GPIO_enu_PinInit(&inita4);
    GPIO_enu_PinInit(&inita5);
    GPIO_enu_PinInit(&inita6);
    GPIO_enu_PinInit(&inita7);

    GPIO_enu_PinInit(&initb0);
    GPIO_enu_PinInit(&initb1);
    GPIO_enu_PinInit(&initb2);

    while(1){
       u16 LED_Number = buttonPressed();
       LED(LED_Number);
    }

}