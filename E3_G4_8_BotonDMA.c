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

    // ---------- AFIO con REGs nombrados ----------
    union {
        struct {
            uint32_t EVCR;      // 0x00
            uint32_t MAPR;      // 0x04
            uint32_t EXTICR1;   // 0x08
            uint32_t EXTICR2;   // 0x0C
            uint32_t EXTICR3;   // 0x10
            uint32_t EXTICR4;   // 0x14
            uint32_t MAPR2;     // 0x18
        } REGs;
        page reserved;
    } AFIO;

    // ---------- EXTI con REGs nombrados ----------
    union {
        struct {
            uint32_t IMR;       // 0x00
            uint32_t EMR;       // 0x04
            uint32_t RTSR;      // 0x08
            uint32_t FTSR;      // 0x0C
            uint32_t SWIER;     // 0x10
            uint32_t PR;        // 0x14
        } REGs;
        page reserved;
    } EXTI;

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

// --------- NVIC / CTX ----------
#define ENA_IRQ(IRQ) {CTX->NVIC.REGs.ISER[((uint32_t)(IRQ) >> 5)] = (1u << ((uint32_t)(IRQ) & 0x1F));}
#define DIS_IRQ(IRQ) {CTX->NVIC.REGs.ICER[((uint32_t)(IRQ) >> 5)] = (1u << ((uint32_t)(IRQ) & 0x1F));}
#define CLR_IRQ(IRQ) {CTX->NVIC.REGs.ICPR[((uint32_t)(IRQ) >> 5)] = (1u << ((uint32_t)(IRQ) & 0x1F));}

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

// IRQ numbers (útiles)
enum IRQs {
    IRQ_DMA1CHN2  = 12,
    IRQ_ADC1_2    = 18,
    IRQ_TIM2      = 28,
    IRQ_USART1    = 37,
    IRQ_EXTI0     = 6
};

// ------------------------------------------------------------
// PROGRAMA PRINCIPAL
// -----------------------------------------------------------
int main(void)
{
    // 1) Clocks para GPIOA/B/C y AFIO (APB2)
    DEVMAP->RCC.REGs.APB2ENR |= (1 << 2); // habilita GPIOA
    DEVMAP->RCC.REGs.APB2ENR |= (1 << 3); // habilita GPIOB
    DEVMAP->RCC.REGs.APB2ENR |= (1 << 4); // habilita GPIOC
    DEVMAP->RCC.REGs.APB2ENR |= (1 << 0); // habilita AFIO (Alternate Function IO)

    // 2) PC13 salida push-pull 2 MHz (CRH[23:20] = 0b0010)
    uint32_t crh_c = DEVMAP->GPIOs[GPIOC].REGs.CRH;
    crh_c &= ~(0xF << 20); // limpia configuración previa del pin 13
    crh_c |=  (0x2 << 20); // salida push-pull, 2 MHz
    DEVMAP->GPIOs[GPIOC].REGs.CRH = crh_c;

    // 3) PA0 entrada flotante (0100). Si querés PU/PD: usar 1000 y elegir en ODR0.
    uint32_t crl_a = DEVMAP->GPIOs[GPIOA].REGs.CRL;
    crl_a &= ~(0xF << 0); // limpia configuración previa del pin 0
    crl_a |=  (0x4 << 0); // 0100: entrada flotante
    DEVMAP->GPIOs[GPIOA].REGs.CRL = crl_a;

    // PB11 salida push-pull 2 MHz (CRH[15:12] = 0b0010)
    uint32_t crh_b = DEVMAP->GPIOs[GPIOB].REGs.CRH;
    crh_b &= ~(0xF << 12); // limpia configuración previa del pin 11
    crh_b |=  (0x2 << 12); // salida push-pull, 2 MHz
    DEVMAP->GPIOs[GPIOB].REGs.CRH = crh_b;

    // 4) Conectar la línea EXTI0 al pin PA0 en el AFIO
    DEVMAP->AFIO.REGs.EXTICR1 &= ~(0xF << 0); // EXTI0 desde PA0

    // 5) configurar interrupcion externa en EXTI0
    DEVMAP->EXTI.REGs.IMR  |= (1 << 0); // habilito interrupción en línea 0
    DEVMAP->EXTI.REGs.RTSR |= (1 << 0); // flanco ascendente
    DEVMAP->EXTI.REGs.FTSR |= (1 << 0); // flanco descendente
    // Con esto se dispara al apretar y al soltar el botón

    // 6) Habilitar interrupción EXTI0 en el NVIC (IRQ6)
    ENA_IRQ(IRQ_EXTI0); // //“cuando EXTI0 pida interrupción, dejala pasar al núcleo”

    // 7) Loop infinito (la acción sucede en el handler)
    for(;;);

    return 0;
}


// ------------------------------------------------------------
// HANDLER DE INTERRUPCIÓN EXTI0
// ------------------------------------------------------------

void handler_exti0(void)
{
    // Leo PA0: si vale 1 (botón apretado), prendo LED.
    if (DEVMAP->GPIOs[GPIOA].REGs.IDR & (1 << 0)) {
        // Apretado / a 3.3 V -> encender
        DEVMAP->GPIOs[GPIOC].REGs.ODR &= ~(1 << 13); // PC13 = 0 (LED ON)
        DEVMAP->GPIOs[GPIOB].REGs.ODR &= ~(1 << 11); // PB11 = 0
    } else {
        // Suelto / GND -> apagar
        DEVMAP->GPIOs[GPIOC].REGs.ODR |=  (1 << 13); // PC13 = 1 (LED OFF)
        DEVMAP->GPIOs[GPIOB].REGs.ODR |=  (1 << 11); // PB11 = 1
    }

    // Limpio el flag de EXTI0 para permitir próximas interrupciones
    DEVMAP->EXTI.REGs.PR = (1 << 0);
}

// ------------------------------------------------------------
// VECTOR DE INTERRUPCIONES (Asignamos que función maneja cada interrupción)
// ------------------------------------------------------------
#define SRAM_SIZE ((uint32_t)0x00005000)  // 20 KB de SRAM
#define SRAM_BASE ((uint32_t)0x20000000)
#define STACKINIT ((interrupt_t)(SRAM_BASE+SRAM_SIZE))

typedef void(*interrupt_t)(void);
extern void handler_exti0(void);

const interrupt_t vector_table[256] __attribute__ ((section(".vtab"))) = {
    STACKINIT,                 // 0x0000_0000 Stack Pointer inicial
    (interrupt_t) main,        // 0x0000_0004 Reset Handler
    0,                         // NMI Handler
    0,                         // HardFault Handler
    0,                         // MemManage Handler
    0,                         // BusFault Handler
    0,                         // UsageFault Handler
    0,0,0,0,                   // Reservados
    0,                         // SVCall
    0,                         // Debug Monitor
    0,                         // Reservado
    0,                         // PendSV
    0,                         // SysTick
    0,                         // WWDG
    0,                         // PVD
    0,                         // TAMPER
    0,                         // RTC
    0,                         // FLASH
    0,                         // RCC
    (interrupt_t)handler_exti0 // EXTI0 IRQ (posición 22 en vector table) - ACTIVAMOS EXTI0 ASOCIADO AL PIN PA0
    // El resto quedan en 0 
};
