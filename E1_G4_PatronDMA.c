#define SRAM_SIZE       ((uint32_t) 0x00005000)
#define SRAM_BASE       ((uint32_t) 0x20000000)
#define STACKINIT       ((interrupt_t)(SRAM_BASE+SRAM_SIZE))

typedef int             int32_t;
typedef short           int16_t;
typedef char            int8_t;
typedef unsigned int    uint32_t;
typedef unsigned short  uint16_t;
typedef unsigned char   uint8_t;

typedef void(*interrupt_t)(void);

typedef union {
    uint8_t  byte[4];
    uint16_t hword[2];
    uint32_t word;
} word_t;

typedef word_t page[0x400/sizeof(uint32_t)];

// Memory map - Dejo las declaraciones de las direcciónes iguales a lo usados en el código "DMA", junto al Devmap.
// Se podrian limpiar ya que muchas no se usan en este ejercicio.

enum {TIM2  = 0, TIM3  = 1, TIM4  = 2 };
enum {GPIOA = 0, GPIOB = 1, GPIOC = 2, GPIOD = 3, GPIOE = 4, GPIOF = 5 };
enum {DMA1  = 0 };
enum {CHN1  = 0, CHN2  = 1, CHN3  = 2, CHN4  = 3, CHN5 = 4, CHN6  = 5, CHN7 = 6, CHN8 = 7 };
enum {ADC1  = 0 };
struct {
    union {
        struct {
            uint32_t CR1;
            uint32_t CR2;
            uint32_t SMCR;
            uint32_t DIER;
            uint32_t SR;
            uint32_t EGR;
            uint32_t CCMR1;
            uint32_t CCMR2;
            uint32_t CCER;
            uint32_t CNT;
            uint32_t PSC;
            uint32_t ARR;
            uint32_t RES1;
            uint32_t CCR1;
            uint32_t CCR2;
            uint32_t CCR3;
            uint32_t CCR4;
            uint32_t BDTR;
            uint32_t DCR;
            uint32_t DMAR;
        } REGs;
        page reserved;
    } TIMs[3];

    word_t reserved1[(0x40002800-0x40000c00)/sizeof(word_t)];
    page RTC;
    page WWDG;
    page IWDG;
    word_t reserved2[(0x40003800-0x40003400)/sizeof(word_t)];
    page SPI2;
    word_t reserved3[(0x40004400-0x40003c00)/sizeof(word_t)];
    page USART[2];
    word_t reserved4[(0x40005400-0x40004c00)/sizeof(word_t)];
    page I2C[2];
    page USB;
    page USBCAN_SRAM;
    page bxCAN;
    word_t reserved5[(0x40006c00-0x40006800)/sizeof(word_t)];
    page BKP;
    page PWR;
    word_t reserved6[(0x40010000-0x40007400)/sizeof(word_t)];

    page AFIO;
    page EXTI;

    union {
        struct {
            uint32_t CRL;
            uint32_t CRH;
            uint32_t IDR;
            uint32_t ODR;
            uint32_t BSRR;
            uint32_t BRR;
            uint32_t LCKR;
        } REGs;
        page reserved;
    } GPIOs[5];
    word_t reserved7[(0x40012400-0x40011C00)/sizeof(word_t)];
    union {
        struct {
            uint32_t SR;
            uint32_t CR1;
            uint32_t CR2;
            uint32_t SMPR1;
            uint32_t SMPR2;
            uint32_t JOFR;
            uint32_t HTR;
            uint32_t LTR;
            uint32_t SQR1;
            uint32_t SQR2;
            uint32_t SQR3;
            uint32_t JSQR;
            uint32_t JDR;
            uint32_t DR;
        } REGs;
        page reserved;
    } ADC[2];
    page TIM1;
    page SPI1;
    word_t reserved8[(0x40013800-0x40013400)/sizeof(word_t)];
    union  {
        struct {
            uint32_t SR;
            uint32_t DR;
            uint32_t BRR;
            uint32_t CR1;
            uint32_t CR2;
            uint32_t CR3;
            uint32_t GTPR;
        } REGs;
        page reserved;
    } USART1;
    word_t reserved9[(0x40020000-0x40013C00)/sizeof(word_t)];
    union {
        struct {
            uint32_t ISR;
            uint32_t IFCR;
            struct {
                uint32_t CCR;
                uint32_t CNDTR;
                uint32_t CPAR;
                uint32_t CMAR;
                uint32_t RESERVED;
            } CHN[8];
        } REGs;
        page reserved;
    } DMAs[1];
    word_t reservedA[(0x40021000-0x40020400)/sizeof(word_t)];

    union {
        struct {
            uint32_t CR;
            uint32_t CFGR;
            uint32_t CIR;
            uint32_t APB2RSTR;
            uint32_t APB1RSTR;
            uint32_t AHBENR;
            uint32_t APB2ENR;
            uint32_t APB1ENR;
            uint32_t BDCR;
            uint32_t CSR;
            uint32_t AHBRSTR;
            uint32_t CFGR2;
        } REGs;
        page reserved;
    } RCC;
    word_t reservedB[(0x40022000-0x40021400)/sizeof(word_t)];

    union {
        struct {
            uint32_t ACR;
            uint32_t KEYR;
            uint32_t OPTKEYR;
            uint32_t SR;
            uint32_t CR;
            uint32_t AR;
            uint32_t reserved;
            uint32_t OBR;
            uint32_t WRPR;
        } REGs;
        page reserved;
    } FLASH;
} volatile *const DEVMAP = (void *) 0x40000000;

struct {
    word_t reversed0[(0xe000e010-0xe0000000)/sizeof(word_t)];
    union {
        struct {
            uint32_t CSR;
            uint32_t RVR;
            uint32_t CVR;
            uint32_t CALIB;
        } REGs;
    } SYSTICK;
    word_t reversed1[(0xe000e100-0xe000e020)/sizeof(word_t)];
    union {
        struct {
            uint32_t ISER[8];
            uint32_t RES0[24];
            uint32_t ICER[8];
            uint32_t RES1[24];
            uint32_t ISPR[8];
            uint32_t RES2[24];
            uint32_t ICPR[8];
            uint32_t RES3[24];
            uint32_t IABR[8];
            uint32_t RES4[56];
            uint8_t  IPR[240];
            uint32_t RES5[644];
            uint32_t STIR;
        } REGs;
    } NVIC;
} volatile *const CTX = ((void *) 0xE0000000);

// --- IRQs (no usados, los dejo por compatibilidad) ---
enum IRQs {
    IRQ_DMA1CHN2  = 12,
    IRQ_ADC1_2    = 18,
    IRQ_TIM2      = 28,
    IRQ_USART1    = 37,
};

// Solo main: sin prototipos/handlers de interrupción
int  main(void);

// Vector: solo SP y Reset (el resto en cero)
const interrupt_t vector_table[] __attribute__ ((section(".vtab"))) = {
    STACKINIT,                          // 0x0000_0000 Stack Pointer
    (interrupt_t) main,                 // 0x0000_0004 Reset
    0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

//Matriz de datos a transferir por DMA, 256 valores de 32 bits, reproducimos 1 ciclo por segundo "256 ticks de reloj"
uint32_t data[256];

int main(void)
{
    // Separo la matriz en secciones de 32 valores para formar un patron 
    for (int i = 0; i < 32; i++){
        data[i] = 0x00000000;
    }
    for (int i = 32; i < 32*2; i++){
        data[i] = 0x00002000;
    }
    for (int i = 32*2; i < 32*3; i++){
        data[i] = 0x00000000;
    }
    for (int i = 32*3; i < 32*4; i++){
        data[i] = 0x00000000;
    }
    for (int i = 32*4; i < 32*5; i++){
        data[i] = 0x00002000;
    }
    for (int i = 32*5; i < 32*6; i++){
        data[i] = 0x00000000;
    }
    for (int i = 32*6; i < 32*7; i++){
        data[i] = 0x00002000;
    }
    for (int i = 32*7; i < 256; i++){
        data[i] = 0x00000000;
    }

    // PCLK code
    DEVMAP->RCC.REGs.CR   |= (1 << 16);                      // Habilito HSE
    while (!(DEVMAP->RCC.REGs.CR & (1 << 17)));              // Espero a que HSE esté listo

    DEVMAP->RCC.REGs.CR   &= ~(1 << 24);                     // Disable PLL
    DEVMAP->RCC.REGs.CFGR |= (0b0111 << 18);                 // PLL x9 -> 72 MHz
    DEVMAP->RCC.REGs.CFGR |=  (1 << 16);                     // PLL source = HSE
    DEVMAP->RCC.REGs.CR   |=  (1 << 24);                     // Enable PLL
    while (!(DEVMAP->RCC.REGs.CR & (1 << 25)));              // Espero a que PLL esté listo

    DEVMAP->FLASH.REGs.ACR |= (0b010 << 0);                  // FLASH WS = 2
    DEVMAP->RCC.REGs.CFGR  |= (0b0000 << 4);                 // AHB presc = /1 (72 MHz)
    DEVMAP->RCC.REGs.CFGR  |= (0b100  << 8);                 // APB1 presc = /2 (36 MHz)

    DEVMAP->RCC.REGs.CFGR |= (0b10 << 0);                    // SYSCLK = PLL
    while (!(DEVMAP->RCC.REGs.CFGR & (0b10 << 2)));          // Espero a que SYSCLK esté en PLL

    // DMA code
    DEVMAP->RCC.REGs.APB2ENR |= (1 << 4);                    // Enable GPIOC clock
    DEVMAP->RCC.REGs.APB1ENR |= (1 << 0);                    // Enable TIM2 clock
    DEVMAP->RCC.REGs.AHBENR  |= (1 << 0);                    // Enable DMA1 clock

    DEVMAP->GPIOs[GPIOC].REGs.CRL  = 0x33333333;             // Bajo GPIOC output
    DEVMAP->GPIOs[GPIOC].REGs.CRH  = 0x33333333;             // Alto GPIOC output

    DEVMAP->DMAs[DMA1].REGs.CHN[CHN2].CNDTR = sizeof(data)/sizeof(uint32_t); // Transfer size
    DEVMAP->DMAs[DMA1].REGs.CHN[CHN2].CMAR  = (uint32_t) data;               // Memory source
    DEVMAP->DMAs[DMA1].REGs.CHN[CHN2].CPAR  = (uint32_t) &DEVMAP->GPIOs[GPIOC].REGs.ODR; // Peripheral dest (se mantiene ODR)

    DEVMAP->DMAs[DMA1].REGs.CHN[CHN2].CCR  = 0;              // Reset CCR
    DEVMAP->DMAs[DMA1].REGs.CHN[CHN2].CCR &= ~(1 << 14);     // MEM2MEM=0
    DEVMAP->DMAs[DMA1].REGs.CHN[CHN2].CCR |=  (0b11 << 12);  // PL=Very High
    DEVMAP->DMAs[DMA1].REGs.CHN[CHN2].CCR |=  (0b10 << 10);  // MSIZE=32-bit
    DEVMAP->DMAs[DMA1].REGs.CHN[CHN2].CCR |=  (0b10 << 8);   // PSIZE=32-bit
    DEVMAP->DMAs[DMA1].REGs.CHN[CHN2].CCR |=  (1 << 7);      // MINC=1
    DEVMAP->DMAs[DMA1].REGs.CHN[CHN2].CCR &= ~(1 << 6);      // PINC=0
    DEVMAP->DMAs[DMA1].REGs.CHN[CHN2].CCR |=  (1 << 5);      // CIRC=1
    DEVMAP->DMAs[DMA1].REGs.CHN[CHN2].CCR |=  (1 << 4);      // DIR=1 (read from memory)

    DEVMAP->DMAs[DMA1].REGs.CHN[CHN2].CCR |= (1 << 0);       // Enable DMA

    // TIM2
    DEVMAP->TIMs[TIM2].REGs.CR1  = 0x0000;                   // Reset CR1
    DEVMAP->TIMs[TIM2].REGs.PSC   = (72e6/8)/(sizeof(data)/sizeof(data[0]))-1; // fCK_PSC/(PSC+1)
    DEVMAP->TIMs[TIM2].REGs.ARR   = 8-1;
    DEVMAP->TIMs[TIM2].REGs.DIER |= (1 << 14);               // TDE (Establece DMA request on update)
    DEVMAP->TIMs[TIM2].REGs.DIER |= (1 <<  8);               // UDE (Update DMA request enable)

    DEVMAP->TIMs[TIM2].REGs.CR1  |= (1 << 0);                // Enable TIM2

    for(;;);

    return 0;
}
