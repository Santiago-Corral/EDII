// --- Definición de tipos ---
typedef int            int32_t;
typedef short          int16_t;
typedef char           int8_t;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

// --- Direcciones base ---
#define RCC_BASE       0x40021000
#define GPIOA_BASE     0x40010800
#define GPIOC_BASE     0x40011000

// --- Registros RCC ---
#define RCC_APB2ENR    (*(volatile uint32_t*)(RCC_BASE + 0x18))

// --- Registros GPIOA (entrada botón) ---
#define GPIOA_CRL      (*(volatile uint32_t*)(GPIOA_BASE + 0x00)) //Primeros 8 pines del A
#define GPIOA_IDR      (*(volatile uint32_t*)(GPIOA_BASE + 0x08)) //Entrada de datos

// --- Registros GPIOC (LED) ---
#define GPIOC_CRH      (*(volatile uint32_t*)(GPIOC_BASE + 0x04))
#define GPIOC_ODR      (*(volatile uint32_t*)(GPIOC_BASE + 0x0C))

int main(void)
{
    // 1. Habilitar reloj GPIOA y GPIOC
    RCC_APB2ENR |= (1 << 2); // BIT 2 habilitado ("1") - GPIOA
    RCC_APB2ENR |= (1 << 4); // BIT 4 habilitado ("1") - GPIOC

    // 2. Configurar PA0 como entrada flotante (botón)
    GPIOA_CRL &= ~(0xF << 0);   // limpiar config de PA0
    GPIOA_CRL |=  (0x4 << 0);   // entrada flotante

    // 3. Configurar PC13 como salida push-pull 2 MHz (LED)
    GPIOC_CRH &= ~(0xF << 20);  
    GPIOC_CRH |=  (0x2 << 20);  

    // 4. Loop infinito
    while (1)
    {
        if (GPIOA_IDR & (1 << 0)) {
            // Botón NO presionado (entrada en 1) -> apagar LED
            GPIOC_ODR |= (1 << 13);
        } else {
            // Botón presionado (entrada en 0) -> encender LED
            GPIOC_ODR &= ~(1 << 13);
        }
    }

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
