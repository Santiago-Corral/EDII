/* 8. Implementar el punto 5 pero utilizando interrupciones.*/

// ------------------------------------------------------------
// Definiciones manuales de enteros con tamaños fijos
// ------------------------------------------------------------
typedef int            int32_t;
typedef short          int16_t;
typedef char           int8_t;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;
typedef void (*interrupt_t)(void);

// ------------------------------------------------------------
// Direcciones base
// ------------------------------------------------------------
#define RCC_BASE     0x40021000
#define GPIOA_BASE   0x40010800
#define GPIOC_BASE   0x40011000
#define GPIOB_BASE   0x40010C00
#define AFIO_BASE    0x40010000
#define EXTI_BASE    0x40010400
#define NVIC_BASE    0xE000E100

// ------------------------------------------------------------
// Registros RCC
// ------------------------------------------------------------
#define RCC_APB2ENR  (*(volatile uint32_t*)(RCC_BASE + 0x18))

// ------------------------------------------------------------
// Registros GPIOA
// ------------------------------------------------------------
#define GPIOA_CRL    (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_IDR    (*(volatile uint32_t*)(GPIOA_BASE + 0x08))

// ------------------------------------------------------------
// Registros GPIOC
// ------------------------------------------------------------
#define GPIOC_CRH    (*(volatile uint32_t*)(GPIOC_BASE + 0x04))
#define GPIOC_ODR    (*(volatile uint32_t*)(GPIOC_BASE + 0x0C))

// Registros GPIOB
#define GPIOB_CRH   (*(volatile uint32_t*)(GPIOB_BASE + 0x04))
#define GPIOB_ODR   (*(volatile uint32_t*)(GPIOB_BASE + 0x0C))

// ------------------------------------------------------------
// Registros AFIO
// ------------------------------------------------------------
#define AFIO_EXTICR1 (*(volatile uint32_t*)(AFIO_BASE + 0x08))

// ------------------------------------------------------------
// Registros EXTI 
// ------------------------------------------------------------
#define EXTI_IMR     (*(volatile uint32_t*)(EXTI_BASE + 0x00)) // Interrupt mask register ("Que lineas pueden generar interrupcion")
#define EXTI_RTSR    (*(volatile uint32_t*)(EXTI_BASE + 0x08)) // Rising trigger selection register ("Flanco ascendente")
#define EXTI_FTSR    (*(volatile uint32_t*)(EXTI_BASE + 0x0C)) // Falling trigger selection register ("Flanco descendente")
#define EXTI_PR      (*(volatile uint32_t*)(EXTI_BASE + 0x14)) // Pending register ("El EXTI guarda si ocurrió un evento = bit pending en 1")

// ------------------------------------------------------------
// Registros NVIC
// ------------------------------------------------------------
#define NVIC_ISER0   (*(volatile uint32_t*)(NVIC_BASE + 0x00)) // Nested Vector Interrupt Controller

// ------------------------------------------------------------
// PROGRAMA PRINCIPAL
// ------------------------------------------------------------
int main(void)
{
    // 1) Habilitar reloj para GPIOA, GPIOC y AFIO
    RCC_APB2ENR |= (1 << 2); // habilita GPIOA
    RCC_APB2ENR |= (1 << 4); // habilita GPIOC
    RCC_APB2ENR |= (1 << 3); // habilita GPIOB
    RCC_APB2ENR |= (1 << 0); // habilita AFIO (Alternate Function IO)

    // 2) Configurar PC13 como salida push-pull a 2 MHz
    // Cada pin se configura con 4 bits. PC13 usa bits 20–23 del CRH.
    GPIOC_CRH &= ~(0xF << 20); // borro la configuración anterior
    GPIOC_CRH |=  (0x2 << 20); // salida push-pull, 2 MHz

    // 3) Configurar PA0 como entrada flotante
    // Conectaste un botón con resistencia pull-down externa
    GPIOA_CRL &= ~(0xF << 0);  // borro configuración de PA0
    GPIOA_CRL |=  (0x4 << 0);  // 0100 = entrada flotante

    // Configurar PB11 como salida push-pull 2 MHz
    GPIOB_CRH &= ~(0xF << 12); // limpiar bits 12–15
    GPIOB_CRH |=  (0x2 << 12); // MODE11=10 (2MHz)
    GPIOB_CRH |=  (0x0 << 14); // CNF11=00 push-pull

    // 4) Conectar la línea EXTI0 al pin PA0 en el AFIO
    AFIO_EXTICR1 &= ~(0xF << 0); // PA0 -> EXTI0 (valor por defecto)

    // 5) Configurar interrupción externa en EXTI0
    EXTI_IMR  |= (1 << 0);   // habilito interrupción en línea 0
    EXTI_RTSR |= (1 << 0);   // flanco ascendente
    EXTI_FTSR |= (1 << 0);   // flanco descendente
    // Con esto se dispara al apretar y al soltar el botón

    // 6) Habilitar interrupción EXTI0 en el NVIC (IRQ6)
    NVIC_ISER0 |= (1 << 6); //“cuando EXTI0 pida interrupción, dejala pasar al núcleo”

    // 7) Bucle infinito: el programa se queda acá,
    //    y las acciones ocurren en la ISR (handler_exti0).
    for(;;);

    return 0;
}

// ------------------------------------------------------------
// HANDLER DE INTERRUPCIÓN EXTI0
// ------------------------------------------------------------

void handler_exti0(void)
{
    // Leo PA0: si vale 1 (botón apretado), prendo LED.
    if (GPIOA_IDR & (1 << 0)) {
        GPIOC_ODR &= ~(1 << 13); // LED ON (activo en bajo)
        GPIOB_ODR &= ~(1 << 11);  // LED externo encendido (activo-bajo) "&= ~ apagado"

    } else {
        GPIOC_ODR |=  (1 << 13); // LED OFF
        GPIOB_ODR |=  (1 << 11);  // LED externo apagado "|= apagado"
    }

    // Limpio el flag de EXTI0 para permitir próximas interrupciones
    EXTI_PR |= (1 << 0);
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