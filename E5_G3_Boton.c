// --- Definición de tipos ---
typedef int            int32_t;
typedef short          int16_t;
typedef char           int8_t;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

// Direcciones base
#define RCC_BASE       0x40021000
#define GPIOC_BASE     0x40011000
#define GPIOA_BASE     0x40010800

// Registros RCC
#define RCC_APB2ENR    (*(volatile uint32_t*)(RCC_BASE + 0x18))

// Registros GPIOC (LED PC13)
#define GPIOC_CRH      (*(volatile uint32_t*)(GPIOC_BASE + 0x04))
#define GPIOC_ODR      (*(volatile uint32_t*)(GPIOC_BASE + 0x0C))

// Registros GPIOA (Entrada PA0)
#define GPIOA_CRL      (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_IDR      (*(volatile uint32_t*)(GPIOA_BASE + 0x08))
#define GPIOA_ODR      (*(volatile uint32_t*)(GPIOA_BASE + 0x0C))

int main(void)
{
    // 1. Habilitar reloj para GPIOA y GPIOC
    RCC_APB2ENR |= (1 << 2);  // GPIOA
    RCC_APB2ENR |= (1 << 4);  // GPIOC

    // 2. Configurar PC13 como salida push-pull 2 MHz
    GPIOC_CRH &= ~(0xF << 20); //Limpiamos pin 13
    GPIOC_CRH |=  (0x2 << 20); //Salida push-pull, 2Mhz

    // 3. Configurar PA0 como entrada con pull-up/pull-down
    GPIOA_CRL &= ~(0xF << 0);
    GPIOA_CRL |=  (0x8 << 0);   // 1000: entrada con pull-up/pull-down

    // 4. Seleccionar pull-down → escribir 0 en ODR0
    GPIOA_ODR &= ~(1 << 0);

    // 5. Bucle principal
    for(;;){
    if (GPIOA_IDR & (1 << 0)) {   // PA0 = 1 (3.3V) -> LED ON
        GPIOC_ODR &= ~(1 << 13);
    } else {                      // PA0 = 0 (pull-down) -> LED OFF
        GPIOC_ODR |=  (1 << 13);
    }
}

    return 0;
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