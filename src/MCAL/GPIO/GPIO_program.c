#include "mcal/GPIO/GPIO_interface.h"
#include "mcal/GPIO/GPIO_private.h"
#include "LIB/bit_math.h"

static gpio_register_t * const staticArr_gpio_ports[GPIO_PORT_COUNT]={
    GPIO_A , GPIO_B ,GPIO_C , GPIO_D , GPIO_E , GPIO_H
};

gpio_errorStatus_t GPIO_enu_PinInit(gpio_struct_pin_config_t* ref_pinConfig){
    gpio_errorStatus_t enu_LocErrorSt;
    if(ref_pinConfig == NULL) return GPIO_NULL_PTR;

    if(ref_pinConfig->mode >= GPIO_MODE_COUNT) return GPIO_INVLID_MODE;

    enu_LocErrorSt = GPIO_enu_CfgPUPDResistor(ref_pinConfig->port, ref_pinConfig->pin, ref_pinConfig->pup_pdown);
    if(enu_LocErrorSt != GPIO_OK) return enu_LocErrorSt;

    if(ref_pinConfig->mode == GPIO_OUTPUT_MODE || ref_pinConfig->mode == GPIO_ALTERNATE_FUNCTION_MODE){
        enu_LocErrorSt = GPIO_enu_CfgPinOutType(ref_pinConfig->port, ref_pinConfig->pin, ref_pinConfig->outputType);
        if(enu_LocErrorSt != GPIO_OK) return enu_LocErrorSt;

        enu_LocErrorSt = GPIO_enu_CfgSpeed(ref_pinConfig->port, ref_pinConfig->pin, ref_pinConfig->speed);
        if(enu_LocErrorSt != GPIO_OK) return enu_LocErrorSt;
    }

    if(ref_pinConfig->mode == GPIO_ALTERNATE_FUNCTION_MODE){
        enu_LocErrorSt = GPIO_enu_CfgAlternativeFnc(ref_pinConfig->port, ref_pinConfig->pin, ref_pinConfig->alternateFunction);
        if(enu_LocErrorSt != GPIO_OK) return enu_LocErrorSt;
    }

    return GPIO_enu_CfgPinMode(ref_pinConfig->port, ref_pinConfig->pin, ref_pinConfig->mode);
}

gpio_errorStatus_t GPIO_enu_PinWrite(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_pinState_t cpy_state){
    if(cpy_port >= GPIO_PORT_COUNT) return GPIO_INVALID_PORT;
    else if(cpy_pin >= GPIO_PIN_COUNT) return GPIO_INVALID_PIN;
    else if(cpy_state >= GPIO_STATE_COUNT) return GPIO_INVALID_STATE;

    switch (cpy_state)
    {
    case GPIO_HIGH:
        staticArr_gpio_ports[cpy_port]->bsrr = (1UL << cpy_pin);
        break;

    case GPIO_LOW:
        staticArr_gpio_ports[cpy_port]->bsrr = (1UL << (cpy_pin + 16U));
        break;

    default:
        return GPIO_INVALID_STATE;
        break;
    }

    return GPIO_OK;
}

gpio_errorStatus_t GPIO_enu_PinRead(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_pinState_t *ref_readState){

    if(cpy_port >= GPIO_PORT_COUNT ) return GPIO_INVALID_PORT;
    else if(cpy_pin >= GPIO_PIN_COUNT) return GPIO_INVALID_PIN;
    else if(ref_readState == NULL) return GPIO_NULL_PTR;

    *ref_readState = GET_BIT(staticArr_gpio_ports[cpy_port]->idr,cpy_pin);

    return GPIO_OK;
}

gpio_errorStatus_t GPIO_enu_PortWrite(gpio_port_t cpy_port, u16 cpy_state){
    if(cpy_port >= GPIO_PORT_COUNT) return GPIO_INVALID_PORT;

    staticArr_gpio_ports[cpy_port]->odr = cpy_state;

    return GPIO_OK;
}

gpio_errorStatus_t GPIO_enu_PortRead(gpio_port_t cpy_port, u16 *ref_state){
    if(ref_state == NULL) return GPIO_NULL_PTR;
    else if(cpy_port >= GPIO_PORT_COUNT) return GPIO_INVALID_PORT;

    *ref_state = staticArr_gpio_ports[cpy_port]->idr;

    return GPIO_OK;
}

gpio_errorStatus_t GPIO_enu_CfgSpeed(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_output_speed_t cpy_speed){
    if(cpy_port >= GPIO_PORT_COUNT) return GPIO_INVALID_PORT;
    else if(cpy_pin >= GPIO_PIN_COUNT) return GPIO_INVALID_PIN;
    else if (cpy_speed >= GPIO_OUTPUT_SPEED_COUNT) return GPIO_INVALID_SPEED;

    u32 Loc_u32reg = staticArr_gpio_ports[cpy_port]->ospeedr;
    Loc_u32reg &= ~( 0x3UL << (2U * cpy_pin));
    Loc_u32reg |= (cpy_speed << (2U * cpy_pin));
    staticArr_gpio_ports[cpy_port]->ospeedr = Loc_u32reg;

    return GPIO_OK;
}

gpio_errorStatus_t GPIO_enu_CfgPUPDResistor(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_pullup_pulldown_t cpy_pupd){
    if(cpy_port >= GPIO_PORT_COUNT) return GPIO_INVALID_PORT;
    else if(cpy_pin >= GPIO_PIN_COUNT) return GPIO_INVALID_PIN;
    else if (cpy_pupd >= GPIO_PULL_RESERVED) return GPIO_INVALID_PUPD;

    u32 Loc_u32reg = staticArr_gpio_ports[cpy_port]->pupdr;
    Loc_u32reg &= ~( 0x3UL << (2U * cpy_pin));
    Loc_u32reg |= (cpy_pupd << (2U * cpy_pin));
    staticArr_gpio_ports[cpy_port]->pupdr = Loc_u32reg;

    return GPIO_OK;
}

gpio_errorStatus_t GPIO_enu_CfgPinMode(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_mode_t cpy_mode){
    if(cpy_port >= GPIO_PORT_COUNT) return GPIO_INVALID_PORT;
    else if(cpy_pin >= GPIO_PIN_COUNT) return GPIO_INVALID_PIN;
    else if (cpy_mode >= GPIO_MODE_COUNT) return GPIO_INVLID_MODE;

    u32 Loc_u32reg = staticArr_gpio_ports[cpy_port]->moder;
    Loc_u32reg &=~ (0x3UL << (2U * cpy_pin));
    Loc_u32reg |= (cpy_mode << (2U * cpy_pin));
    staticArr_gpio_ports[cpy_port]->moder = Loc_u32reg;

    return GPIO_OK;
}

gpio_errorStatus_t GPIO_enu_CfgPinOutType(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_otype_t cpy_outputType){
    if(cpy_port >= GPIO_PORT_COUNT) return GPIO_INVALID_PORT;
    else if(cpy_pin >= GPIO_PIN_COUNT) return GPIO_INVALID_PIN;
    else if (cpy_outputType >= GPIO_OUTPUT_TYPE_COUNT) return GPIO_INVALID_OTYPE;

    switch (cpy_outputType)
    {
    case GPIO_OUTPUT_PUSHPULL:
        CLR_BIT(staticArr_gpio_ports[cpy_port]->otyper,cpy_pin);
        break;

    case GPIO_OUTPUT_OPENDRAIN:
        SET_BIT(staticArr_gpio_ports[cpy_port]->otyper,cpy_pin);
        break;
    }

    return GPIO_OK;
}

gpio_errorStatus_t GPIO_enu_CfgAlternativeFnc(gpio_port_t cpy_port, gpio_pin_t cpy_pin, gpio_alternateFunction_t cpy_alternateFunction){
    if(cpy_port >= GPIO_PORT_COUNT) return GPIO_INVALID_PORT;
    else if(cpy_pin >= GPIO_PIN_COUNT) return GPIO_INVALID_PIN;
    else if(cpy_alternateFunction >= GPIO_AF_COUNT) return GPIO_INVALID_AF;

    if(cpy_pin <= ALTERNATE_FUNCTION_LOW_MAX_PIN){
        u32 Loc_u32reg = staticArr_gpio_ports[cpy_port]->afrl;
        Loc_u32reg &=~ (0xFUL << (4u * cpy_pin));
        Loc_u32reg |= (cpy_alternateFunction << (4u * cpy_pin));
        staticArr_gpio_ports[cpy_port]->afrl = Loc_u32reg;
    }else{
        u32 Loc_u32reg = staticArr_gpio_ports[cpy_port]->afrh;
        Loc_u32reg &=~ (0xFUL << (4u * (cpy_pin - GPIO_PIN8)));
        Loc_u32reg |= (cpy_alternateFunction << (4u * (cpy_pin - GPIO_PIN8)));
        staticArr_gpio_ports[cpy_port]->afrh = Loc_u32reg;
    }

    return GPIO_OK;
}