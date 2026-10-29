#ifndef GPIO_PRIVATE_H
#define GPIO_PRIVATE_H
#include"../../lib/std_types.h"

typedef struct{
    u32 moder;
    u32 otyper;
    u32 ospeedr;
    u32 pupdr;
    u32 idr;
    u32 odr;
    u32 bsrr;
    u32 lckr;
    u32 afrl;
    u32 afrh;
} gpio_register_t;

#define ALTERNATE_FUNCTION_LOW_MAX_PIN 7

#define GPIO_A ((volatile gpio_register_t*) 0x40020000)
#define GPIO_B ((volatile gpio_register_t*) 0x40020400)
#define GPIO_C ((volatile gpio_register_t*) 0x40020800)
#define GPIO_D ((volatile gpio_register_t*) 0x40020C00)
#define GPIO_E ((volatile gpio_register_t*) 0x40021000)
#define GPIO_H ((volatile gpio_register_t*) 0x40021C00)

#endif