#include"mcal/rcc/RCC_private.h"
#include"mcal/rcc/RCC_interface.h"
#include"LIB/bit_math.h"


extern u32 hse_freq; // note this to be added in config sumilating clock (external clock not fully handled)

rcc_error_t rcc_selectSystemClock(rcc_clock_t clk){
    if(clk >= RCC_CLK_COUNT) return RCC_INCORRECT_VALUE;
    
    u8 rdybit = (clk == RCC_HSI) ? 1u : (clk == RCC_HSE)? 17u :25u;
    if(!GET_BIT(RCC_Reg->CR,rdybit)) return RCC_CLK_NOT_READY;

    u32 reg = RCC_Reg->CFGR;
    reg &=~ (0x3UL);
    reg |= clk;
    RCC_Reg->CFGR = reg;

    u32 counter = 1000000UL;
    while((((RCC_Reg->CFGR >> 2) & 0x3u) != clk) && counter) counter--;

    return (((RCC_Reg->CFGR >> 2) & 0x3u) == clk) ? RCC_OK : RCC_TIMEOUT;
}

rcc_error_t rcc_controlClock(rcc_clock_t clk , rcc_enDis_t enDis){
    if(clk < 0 || clk > 2 || enDis < 0 || enDis > 1) return RCC_INCORRECT_VALUE;

    //check first if the clk is in use
    u32 sws = ((RCC_Reg->CFGR >> 2) & 0x03);
    if(enDis == RCC_DISABLE){
        if(sws == clk) return RCC_CLK_IN_USE;
    }

    
    u8 pllSrc = GET_BIT(RCC_Reg->PLLCFGR , 22);
    //check if using pll and check which one is feeding (hsi or hse)
    if(enDis == RCC_DISABLE && sws == RCC_PLL){
        if(pllSrc == clk) return RCC_CLK_IN_USE;
    }

    if(clk == RCC_PLL && enDis == RCC_ENABLE){
        u8 srcReady = pllSrc ? GET_BIT(RCC_Reg->CR,17) : GET_BIT(RCC_Reg->CR,1);
        if(!srcReady) return RCC_PLL_SRC_NOT_READY;   /* new error code */
    }

    u32 counter = 1000000UL;
    switch (clk)
    {
    case RCC_HSI:
        if(enDis == RCC_ENABLE){
            SET_BIT(RCC_Reg->CR,0);
            while(!GET_BIT(RCC_Reg->CR,1) && counter) counter--;
        }else{
            CLR_BIT(RCC_Reg->CR,0);
            while(GET_BIT(RCC_Reg->CR,1) && counter) counter--;
        }
        break;
    case RCC_HSE:
        if(enDis == RCC_ENABLE){
            SET_BIT(RCC_Reg->CR,16);
            while(!GET_BIT(RCC_Reg->CR,17) && counter) counter--;
        }else{
            CLR_BIT(RCC_Reg->CR,16);
            while(GET_BIT(RCC_Reg->CR,17) && counter) counter--;
        }
        break;
    case RCC_PLL:
        if(enDis == RCC_ENABLE){
            SET_BIT(RCC_Reg->CR,24);
            while(!GET_BIT(RCC_Reg->CR,25) && counter) counter--;
        }else{
            CLR_BIT(RCC_Reg->CR,24);
            while(GET_BIT(RCC_Reg->CR,25) && counter) counter--;
        }
        break;
    default:
        return RCC_NOK;
        break;
    }

    if(counter == 0) return RCC_TIMEOUT;
    else return RCC_OK;
}


// rcc_clock_t rcc_getSystemClk(void){
//     return ((RCC_Reg->CFGR >> 2) & 0x3);
// }

rcc_error_t  rcc_getClkStatus(rcc_clock_t clk, rcc_clk_status_t* clkSt){
    if(clkSt == NULL) return RCC_NULLPTR;

    switch (clk)
    {
    case RCC_HSI:
        *clkSt = GET_BIT(RCC_Reg->CR,1);
        break;

    case RCC_HSE:
        *clkSt = GET_BIT(RCC_Reg->CR,17);
        break;

    case RCC_PLL:
        *clkSt = GET_BIT(RCC_Reg->CR,25);
        break;

    default:
        return RCC_NOK;
        break;
    }

    return RCC_OK;
}

rcc_error_t rcc_getResetCause(rcc_reset_cause_t *cause)
{
    if (cause == NULL) return RCC_NULLPTR;

    u32 csr = RCC_Reg->CSR;

    if(GET_BIT(csr, 27)) *cause = RCC_RESET_POWER_ON;
    else if(GET_BIT(csr, 25)) *cause = RCC_RESET_BROWN_OUT;
    else if(GET_BIT(csr, 29)) *cause = RCC_RESET_IWDG;
    else if(GET_BIT(csr, 30)) *cause = RCC_RESET_WWDG;
    else if(GET_BIT(csr, 28)) *cause = RCC_RESET_SOFTWARE;
    else if(GET_BIT(csr, 31)) *cause = RCC_RESET_LOW_POWER;
    else if(GET_BIT(csr, 26)) *cause = RCC_RESET_PIN;
    else *cause = RCC_RESET_NONE;
    
    SET_BIT(RCC_Reg->CSR, 24);//clear all flags next time
    
    return RCC_OK;
}


rcc_error_t rcc_pllCfg(rcc_clock_t clk, u32 m, u32 n, u8 p, u8 q)
{
    if (GET_BIT(RCC_Reg->CR, 24)) return RCC_CLK_IN_USE;

    u32 freq;
    if(clk == RCC_HSI) freq = RCC_HSI_FREQ;
    else if(clk == RCC_HSE) freq = hse_freq;
    else return RCC_INCORRECT_VALUE;

    if(m < PLL_M_MIN || m > PLL_M_MAX) return RCC_INCORRECT_VALUE;
    if(n < PLL_N_MIN || n > PLL_N_MAX) return RCC_INCORRECT_VALUE;
    if(q < PLL_Q_MIN || q > PLL_Q_MAX) return RCC_INCORRECT_VALUE;
    if(p != 2 && p != 4 && p != 6 && p != 8) return RCC_INCORRECT_VALUE;

    if (freq % m != 0) return RCC_INCORRECT_VALUE;

    u32 fvco_in = freq / m;
    if (fvco_in < VCO_IN_MIN || fvco_in > VCO_IN_MAX) return RCC_INCORRECT_VALUE;

    u32 fvco_out = fvco_in * n;
    if (fvco_out < VCO_OUT_MIN || fvco_out > VCO_OUT_MAX) return RCC_INCORRECT_VALUE;

    if (fvco_out % q != 0 || (fvco_out / q) != USB_FREQ) return RCC_INCORRECT_VALUE;

    if ((fvco_out / p) > SYSCLK_MAX) return RCC_INCORRECT_VALUE;

    u32 tmp = RCC_Reg->PLLCFGR;

    tmp &= ~(0x3FUL);                     
    tmp |= m;

    tmp &= ~(0x1FFUL << 6);              
    tmp |= (n << 6);

    tmp &= ~(0x3UL << 16);
    tmp |= ((u32)((p / 2) - 1) << 16);

    tmp &= ~(1UL << 22);
    if (clk == RCC_HSE) tmp |= (1UL << 22);

    tmp &= ~(0xFUL << 24);
    tmp |= ((u32)q << 24);

    RCC_Reg->PLLCFGR = tmp;

    return RCC_OK;
}

rcc_error_t rcc_ahb_busScaler(u16 divider)
{
    u32 code;

    switch (divider)
    {
    case 1:   code = RCC_AHB_DIV1;   break;
    case 2:   code = RCC_AHB_DIV2;   break;
    case 8:   code = RCC_AHB_DIV8;   break;   
    case 16:  code = RCC_AHB_DIV16;  break;   
    case 4:   code = RCC_AHB_DIV4;   break;   
    case 64:  code = RCC_AHB_DIV64;  break;   
    case 128: code = RCC_AHB_DIV128; break;   
    case 256: code = RCC_AHB_DIV256; break;   
    case 512: code = RCC_AHB_DIV512; break;   
    default:  return RCC_INCORRECT_VALUE;
    }

    u32 tmp = RCC_Reg->CFGR;
    tmp &= ~(0xFUL << 4);      
    tmp |= (code << 4);
    RCC_Reg->CFGR = tmp;

    return RCC_OK;
}

rcc_error_t rcc_apb1_busScaler(u16 divider)
{
    u32 code;

    switch (divider)
    {
    case 1:  code = RCC_APB_DIV1;  break;
    case 2:  code = RCC_APB_DIV2;  break;
    case 4:  code = RCC_APB_DIV4;  break;
    case 8:  code = RCC_APB_DIV8;  break;
    case 16: code = RCC_APB_DIV16; break;
    default: return RCC_INCORRECT_VALUE;
    }

    u32 tmp = RCC_Reg->CFGR;
    tmp &= ~(0x7UL << 10);     
    tmp |= (code << 10);
    RCC_Reg->CFGR = tmp;

    return RCC_OK;
}

rcc_error_t rcc_apb2_busScaler(u16 divider)
{
    u32 code;

    switch (divider)
    {
    case 1:  code = RCC_APB_DIV1;  break;
    case 2:  code = RCC_APB_DIV2;  break;
    case 4:  code = RCC_APB_DIV4;  break;
    case 8:  code = RCC_APB_DIV8;  break;
    case 16: code = RCC_APB_DIV16; break;
    default: return RCC_INCORRECT_VALUE;
    }

    u32 tmp = RCC_Reg->CFGR;
    tmp &= ~(0x7UL << 13);
    tmp |= (code << 13);
    RCC_Reg->CFGR = tmp;

    return RCC_OK;
}


rcc_error_t rcc_ctrl_peripheral_clk(rcc_periph_t periph,rcc_enDis_t enDis){
    u16 bus = periph / 32;
    u16 bit = periph % 32;
    rcc_error_t status = RCC_OK;
    
    switch (bus)
    {
        //use the ahb1
    case RCC_BUS_AHB1:
        if(enDis == RCC_ENABLE) SET_BIT(RCC_Reg->AHB1ENR,bit);
        else CLR_BIT(RCC_Reg->AHB1ENR,bit);
        break;

        //use the ahb2
    case RCC_BUS_AHB2:
        if(enDis == RCC_ENABLE) SET_BIT(RCC_Reg->AHB2ENR,bit);
        else CLR_BIT(RCC_Reg->AHB2ENR,bit);
        break;

        //use the apb1
    case RCC_BUS_APB1:
        if(enDis == RCC_ENABLE) SET_BIT(RCC_Reg->APB1ENR,bit);
        else CLR_BIT(RCC_Reg->APB1ENR,bit);
        break;

        //use the apb2
    case RCC_BUS_APB2:
        if(enDis == RCC_ENABLE) SET_BIT(RCC_Reg->APB2ENR,bit);
        else CLR_BIT(RCC_Reg->APB2ENR,bit);
        break;
    
    default:
        status = RCC_INCORRECT_VALUE;
        break;
    }

    return status;
}

rcc_error_t rcc_getSystemClk(rcc_clock_t *clk)
{
    if (clk == NULL) return RCC_NULLPTR;

    u32 sws = (RCC_Reg->CFGR >> 2) & 0x3UL;

    switch (sws)
    {
    case 0:  *clk = RCC_HSI; break;
    case 1:  *clk = RCC_HSE; break;
    case 2:  *clk = RCC_PLL; break;
    default: return RCC_NOK;
    }

    return RCC_OK;
}