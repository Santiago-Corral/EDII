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


int main(void)
{
    // 1) Habilitar reloj para GPIOA y GPIOC (APB2)
    DEVMAP->RCC.REGs.APB2ENR |= (1 << 2); // GPIOA
    DEVMAP->RCC.REGs.APB2ENR |= (1 << 4); // GPIOC

    // 2) PC13 como salida push-pull 2 MHz (CRH[23:20] = 0b0010)
    uint32_t crh = DEVMAP->GPIOs[GPIOC].REGs.CRH;
    crh &= ~(0xF << 20);   // limpia configuración previa del pin 13
    crh |=  (0x2 << 20);   // salida push-pull, 2 MHz
    DEVMAP->GPIOs[GPIOC].REGs.CRH = crh;

    // 3) PA0 como entrada con pull-up/pull-down (CRL[3:0] = 0b1000)
    uint32_t crl = DEVMAP->GPIOs[GPIOA].REGs.CRL;
    crl &= ~(0xF << 0);    // limpia configuración previa del pin 0
    crl |=  (0x8 << 0);    // 1000: entrada con pull-up/pull-down
    DEVMAP->GPIOs[GPIOA].REGs.CRL = crl; 

    // 4) Seleccionar pull-down: ODR0 = 0 (si quisieras pull-up, sería ODR0=1)
    DEVMAP->GPIOs[GPIOA].REGs.ODR &= ~(1 << 0); // pull-down

    // 5) Bucle principal (PC13 es activo en bajo)
    for (;;)
    {
        if (DEVMAP->GPIOs[GPIOA].REGs.IDR & (1 << 0)) {   // PA0 = 1 -> LED ON
            DEVMAP->GPIOs[GPIOC].REGs.ODR &= ~(1 << 13);  // PC13 = 0
        } else {                                           // PA0 = 0 -> LED OFF
            DEVMAP->GPIOs[GPIOC].REGs.ODR |=  (1 << 13);  // PC13 = 1
        }
    }
}

// Vector de interrupciones
#define SRAM_SIZE ((uint32_t) 0x00005000)
#define SRAM_BASE ((uint32_t) 0x20000000)
#define STACKINIT ((interrupt_t)(SRAM_BASE+SRAM_SIZE))

typedef void(*interrupt_t)(void);
const interrupt_t vector_table[256] __attribute__ ((section(".vtab"))) = {
    STACKINIT,                // 0x0000_0000 Stack Pointer
    (interrupt_t) main,       // 0x0000_0004 Reset
};
