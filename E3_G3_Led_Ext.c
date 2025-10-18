// --- Definición de tipos ---
typedef int            int32_t;
typedef short          int16_t;
typedef char           int8_t;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

// Tipos
typedef unsigned int uint32_t;
typedef void (*interrupt_t)(void);

// Direcciones base
#define RCC_BASE    0x40021000
#define GPIOC_BASE  0x40011000
#define GPIOB_BASE  0x40010C00

// Registros RCC
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x18))

// Registros GPIOC
#define GPIOC_CRH   (*(volatile uint32_t*)(GPIOC_BASE + 0x04))
#define GPIOC_ODR   (*(volatile uint32_t*)(GPIOC_BASE + 0x0C))

// Registros GPIOB
#define GPIOB_CRH   (*(volatile uint32_t*)(GPIOB_BASE + 0x04))
#define GPIOB_ODR   (*(volatile uint32_t*)(GPIOB_BASE + 0x0C))

// Stack inicial
#define SRAM_SIZE   ((uint32_t)0x5000)
#define SRAM_BASE   ((uint32_t)0x20000000)
#define STACKINIT   ((interrupt_t)(SRAM_BASE+SRAM_SIZE))

int main(void)
{
    // 1. Habilitar reloj GPIOC y GPIOB
    RCC_APB2ENR |= (1 << 4);  // GPIOC
    RCC_APB2ENR |= (1 << 3);  // GPIOB

    // 2. Configurar PC13 como salida push-pull 2 MHz
    GPIOC_CRH &= ~(0xF << 20);
    GPIOC_CRH |=  (0x2 << 20);

    // 3. Configurar PB11 como salida push-pull 2 MHz
    GPIOB_CRH &= ~(0xF << 12); // limpiar bits 12–15
    GPIOB_CRH |=  (0x2 << 12); // MODE11=10 (2MHz)
    GPIOB_CRH |=  (0x0 << 14); // CNF11=00 push-pull

    // 4. Patrón de parpadeo
    while(1)
    {
        // Patrón 1: LED interno encendido, LED externo apagado
        GPIOC_ODR &= ~(1 << 13);  // LED interno encendido (activo-bajo) "&= ~ encendido"
        GPIOB_ODR |=  (1 << 11);  // LED externo apagado "|= apagado"

        for(volatile uint32_t i = 0; i < 200000; i++); //De alguna manera esto es lo que provoca el parpadeo
        
        GPIOB_ODR &= ~(1 << 11);  // LED externo encendido (activo-bajo) "&= ~ apagado"

        for(volatile uint32_t i = 0; i < 200000; i++);

        GPIOC_ODR |=  (1 << 13);  // LED interno apagado (activo-bajo) "|= encendido"
        GPIOB_ODR |=  (1 << 11);  // LED externo apagado (activo-bajo) "&= ~ apagado"

        for(volatile uint32_t i = 0; i < 200000; i++);

        // Patrón 2: LED interno apagado, LED externo encendido

        GPIOB_ODR &= ~(1 << 11);  // LED externo encendido (activo-bajo) "&= ~ apagado" 

        for(volatile uint32_t i = 0; i < 500000; i++);
    }

    return 0;
}

// Vector de interrupciones minimalista
typedef void(*interrupt_t)(void);
const interrupt_t vector_table[256] __attribute__ ((section(".vtab"))) = {
    STACKINIT,
    (interrupt_t)main
};
