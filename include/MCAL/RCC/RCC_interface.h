#ifndef RCC_INTERFACE_H
#define RCC_INTERFACE_H
#include"RCC_private.h"



typedef enum{ RCC_OK , RCC_TIMEOUT, RCC_NULLPTR , RCC_INCORRECT_VALUE , RCC_CLK_IN_USE ,RCC_PLL_SRC_NOT_READY,RCC_CLK_NOT_READY, RCC_NOK} rcc_error_t;
typedef enum{ RCC_HSI , RCC_HSE, RCC_PLL , RCC_CLK_COUNT} rcc_clock_t;
typedef enum{ RCC_ENABLE , RCC_DISABLE } rcc_enDis_t;
typedef enum{ RCC_NOT_READY , RCC_READY } rcc_clk_status_t;
typedef enum{ RCC_RESET_NONE = 0, RCC_RESET_POWER_ON, RCC_RESET_BROWN_OUT, RCC_RESET_IWDG,RCC_RESET_WWDG,RCC_RESET_SOFTWARE,RCC_RESET_LOW_POWER,RCC_RESET_PIN} rcc_reset_cause_t;
typedef enum { RCC_AHB_DIV1 = 0x0, RCC_AHB_DIV2 = 0x8, RCC_AHB_DIV4 = 0x9, RCC_AHB_DIV8 = 0xA,RCC_AHB_DIV16 = 0xB, RCC_AHB_DIV64 = 0xC, RCC_AHB_DIV128 = 0xD,
    RCC_AHB_DIV256 = 0xE, RCC_AHB_DIV512 = 0xF } rcc_ahb_div_t;
    
    typedef enum { RCC_APB_DIV1 = 0x0, RCC_APB_DIV2 = 0x4, RCC_APB_DIV4 = 0x5, RCC_APB_DIV8 = 0x6, RCC_APB_DIV16 = 0x7 } rcc_apb_div_t;
    
    /*
    the following enum is for controling the peripheral clk each value encode 2 things in it which register(or bus) 
    and which bit inside it
    value = 32 * bus + bit
    */
   
   typedef enum{
       /*AHB1*/
       RCC_GPIOA = 0,
       RCC_GPIOB = 1,
       RCC_GPIOC = 2,
       RCC_GPIOD = 3,
       RCC_GPIOE = 4,
       RCC_GPIOH = 7,
       RCC_CRC = 12,
       RCC_DMA1 = 21,
       RCC_DMA2 = 22,
       
       /*AHB2*/
       RCC_OTGFS = 32 + 7,
       
       /*APB1*/
       RCC_TIM2 = 64 + 0,
       RCC_TIM3 = 64 + 1,
       RCC_TIM4 = 64 + 2,
       RCC_TIM5 = 64 + 3,
       RCC_WWDG = 64 + 11,
       RCC_SPI2 = 64 + 14,
       RCC_SPI3 = 64 + 15,
       RCC_USART2 = 64 + 17,
       RCC_I2C1 = 64 + 21,
       RCC_I2C2 = 64 + 22,
       RCC_I2C3 = 64 + 23,
       RCC_PWR = 64 + 28,

       /*APB2*/
       RCC_TIM1 = 96 + 0,
       RCC_USART1 = 96 + 4,
       RCC_USART6 = 96 + 5,
       RCC_ADC1 = 96 + 8,
       RCC_SDIO = 96 + 11,
       RCC_SPI1 = 96 + 12,
       RCC_SPI4 = 96 + 13,
       RCC_SYSCFG = 96 + 14,
       RCC_TIM9 = 96 + 16,
       RCC_TIM10 = 96 + 17,
       RCC_TIM11 = 96 + 18
    } rcc_periph_t;

/*the following defines are used in the control clk peripheral function*/
#define RCC_BUS_AHB1 0
#define RCC_BUS_AHB2 1
#define RCC_BUS_APB1 2
#define RCC_BUS_APB2 3
/**********************************************/
rcc_error_t rcc_controlClock(rcc_clock_t clk , rcc_enDis_t enDis);
    
rcc_error_t rcc_getSystemClk(rcc_clock_t* clk);

rcc_error_t rcc_getClkStatus(rcc_clock_t clk, rcc_clk_status_t* clkSt);

rcc_error_t rcc_pllCfg(rcc_clock_t clk,u32 m,u32 n,u8 p,u8 q);

rcc_error_t rcc_getResetCause(rcc_reset_cause_t *cause);

rcc_error_t rcc_ahb_busScaler(u16 divider);
 
rcc_error_t rcc_apb_busScaler(u16 divider);

rcc_error_t rcc_apb1_busScaler(u16 divider); // apb1 for low

rcc_error_t rcc_apb2_busScaler(u16 divider); // apb2 for high

rcc_error_t rcc_ctrl_peripheral_clk(rcc_periph_t periph,rcc_enDis_t enDis);

#endif