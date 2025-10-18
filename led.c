// --- Definición de tipos ---
typedef int            int32_t;
typedef short          int16_t;
typedef char           int8_t;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

// --- Direcciones base ---
#define RCC_BASE       0x40021000
#define GPIOC_BASE     0x40011000

// --- Registros RCC ---
#define RCC_APB2ENR    (*(volatile uint32_t*)(RCC_BASE + 0x18))

// --- Registros GPIOC ---
#define GPIOC_CRH      (*(volatile uint32_t*)(GPIOC_BASE + 0x04))
#define GPIOC_ODR      (*(volatile uint32_t*)(GPIOC_BASE + 0x0C))

int main(void)
{
    // 1. Habilitar reloj GPIOC
    RCC_APB2ENR |= (1 << 4);

    // 2. Configurar PC13 como salida push-pull 2 MHz
    GPIOC_CRH &= ~(0xF << 20);  // limpiar bits
    GPIOC_CRH |=  (0x2 << 20);  // salida 2 MHz push-pull

    // 3. Bucle infinito parpadeando LED
    for(;;)
    {
        GPIOC_ODR ^= -1;

        for(volatile uint32_t i = 0; i < 100000; i++);
    }

    //Si remplazamos por while(1) en vez de for(;;) queda prendido el led

    return 0;
}

// Vector de interrupciones minimalista
#define SRAM_SIZE ((uint32_t) 0x00005000)
#define SRAM_BASE ((uint32_t) 0x20000000)
#define STACKINIT ((interrupt_t)(SRAM_BASE+SRAM_SIZE))

typedef void(*interrupt_t)(void);
const interrupt_t vector_table[256] __attribute__ ((section(".vtab"))) = {
    STACKINIT,                // 0x0000_0000 Stack Pointer
    (interrupt_t) main,       // 0x0000_0004 Reset
};