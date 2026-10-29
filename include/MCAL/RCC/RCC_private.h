#ifndef RCC_PRIVATE_H
#define RCC_PRIVATE_H

#include "../../lib/std_types.h"

#define RCC_HSI_FREQ       16000000UL
#define PLL_M_MIN          2
#define PLL_M_MAX          63
#define PLL_N_MIN          192
#define PLL_N_MAX          432
#define PLL_Q_MIN          2
#define PLL_Q_MAX          15
#define VCO_IN_MIN         1000000UL
#define VCO_IN_MAX         2000000UL
#define VCO_OUT_MIN        192000000ULL
#define VCO_OUT_MAX        432000000ULL
#define SYSCLK_MAX         84000000ULL
#define USB_FREQ           48000000ULL

typedef struct
{
    u32 CR;             
    u32 PLLCFGR;
    u32 CFGR;
    u32 CIR;
    u32 AHB1RSTR;
    u32 AHB2RSTR;
    u32 Reserved0[2];
    u32 APB1RSTR;
    u32 APB2RSTR;
    u32 Reserved1[2];
    u32 AHB1ENR;
    u32 AHB2ENR;
    u32 Reserved2[2];
    u32 APB1ENR;
    u32 APB2ENR;
    u32 Reserved3[2];
    u32 AHB1LPENR;
    u32 AHB2LPENR;
    u32 Reserved4[2];
    u32 APB1LPENR;
    u32 APB2LPENR;
    u32 Reserved5[2];
    u32 BDCR;
    u32 CSR;
    u32 Reserved6[2];   
    u32 SSCGR;          
    u32 PLLI2SCFGR;
    u32 Reserved7;
    u32 DCKCFGR;
} RCC_reg_t;

#define RCC_Reg ((volatile RCC_reg_t *) 0x40023800)

#endif