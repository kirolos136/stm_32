#ifndef GPIO_INTERFACE_H
#define GPIO_INTERFACE_H

#include"../../lib/std_types.h"

typedef enum{
    GPIO_OK,
    GPIO_NOK,
    GPIO_INVALID_PORT,
    GPIO_INVALID_PIN,
    GPIO_INVLID_MODE,
    GPIO_INVALID_OTYPE,
    GPIO_INVALID_SPEED,
    GPIO_INVALID_PUPD,
    GPIO_INVALID_AF,
    GPIO_INVALID_STATE,
    GPIO_NULL_PTR
} gpio_errorStatus_t;

typedef enum{
    GPIO_PORTA,
    GPIO_PORTB,
    GPIO_PORTC,
    GPIO_PORTD,
    GPIO_PORTE,
    GPIO_PORTH,
    GPIO_PORT_COUNT
}gpio_port_t;

typedef enum{
    GPIO_PIN0,
    GPIO_PIN1,
    GPIO_PIN2,
    GPIO_PIN3,
    GPIO_PIN4,
    GPIO_PIN5,
    GPIO_PIN6,
    GPIO_PIN7,
    GPIO_PIN8,
    GPIO_PIN9,
    GPIO_PIN10,
    GPIO_PIN11,
    GPIO_PIN12,
    GPIO_PIN13,
    GPIO_PIN14,
    GPIO_PIN15,
    GPIO_PIN_COUNT
}gpio_pin_t;

typedef enum{
    GPIO_INPUT_MODE,
    GPIO_OUTPUT_MODE,
    GPIO_ALTERNATE_FUNCTION_MODE,
    GPIO_ANALOG,
    GPIO_MODE_COUNT
}gpio_mode_t;


typedef enum{
    GPIO_OUTPUT_PUSHPULL,
    GPIO_OUTPUT_OPENDRAIN,
    GPIO_OUTPUT_TYPE_COUNT
}gpio_otype_t;

typedef enum{
    GPIO_OUTPUT_SPEED_LOW,
    GPIO_OUTPUT_SPEED_MEDIUM,
    GPIO_OUTPUT_SPEED_HIGH,
    GPIO_OUTPUT_SPEED_VERYHIGH,
    GPIO_OUTPUT_SPEED_COUNT
}gpio_output_speed_t;


typedef enum{
    GPIO_NO_PULLUP_NO_PULLDOWN,
    GPIO_PULLUP,
    GPIO_PULLDOWN,
    GPIO_PULL_RESERVED,
    GPIO_PUP_PDOWN_COUNT
}gpio_pullup_pulldown_t;

typedef enum {
    GPIO_AF0,
    GPIO_AF1,
    GPIO_AF2,
    GPIO_AF3,
    GPIO_AF4,
    GPIO_AF5,
    GPIO_AF6,
    GPIO_AF7,
    GPIO_AF8,
    GPIO_AF9,
    GPIO_AF10,
    GPIO_AF11,
    GPIO_AF12,
    GPIO_AF13,
    GPIO_AF14,
    GPIO_AF15,
    GPIO_AF_COUNT
} gpio_alternateFunction_t;

typedef enum{
    GPIO_LOW,
    GPIO_HIGH,
    GPIO_STATE_COUNT
} gpio_pinState_t;

typedef struct{
    gpio_port_t port;
    gpio_pin_t pin;
    gpio_mode_t mode;
    gpio_otype_t outputType;
    gpio_output_speed_t speed;
    gpio_pullup_pulldown_t pup_pdown;
    gpio_alternateFunction_t alternateFunction;
} gpio_struct_pin_config_t;

gpio_errorStatus_t GPIO_enu_PinInit(gpio_struct_pin_config_t* ref_pinConfig);

gpio_errorStatus_t GPIO_enu_PinWrite(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_pinState_t cpy_state);

gpio_errorStatus_t GPIO_enu_PinRead(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_pinState_t *ref_readState);

gpio_errorStatus_t GPIO_enu_PortWrite(gpio_port_t cpy_port, u16 cpy_state);

gpio_errorStatus_t GPIO_enu_PortRead(gpio_port_t cpy_port, u16 *ref_state);

gpio_errorStatus_t GPIO_enu_CfgSpeed(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_output_speed_t cpy_speed);

gpio_errorStatus_t GPIO_enu_CfgPUPDResistor(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_pullup_pulldown_t cpy_pupd);

gpio_errorStatus_t GPIO_enu_CfgPinMode(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_mode_t cpy_mode);

gpio_errorStatus_t GPIO_enu_CfgPinOutType(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_otype_t cpy_outputType);

gpio_errorStatus_t GPIO_enu_CfgAlternativeFnc(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_alternateFunction_t cpy_alternateFunction);


#endif