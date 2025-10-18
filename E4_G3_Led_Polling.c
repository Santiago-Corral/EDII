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
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x18)) // Registro de habilitacion de reloj (bit 4 = GPIOC).

// Registros GPIOC
#define GPIOC_CRH   (*(volatile uint32_t*)(GPIOC_BASE + 0x04)) // Configuracion de los pines C8–C15 (PC13 esta ahi).
#define GPIOC_ODR   (*(volatile uint32_t*)(GPIOC_BASE + 0x0C)) // Registro de datos de salida de GPIOC

// Registros GPIOB
#define GPIOB_CRH   (*(volatile uint32_t*)(GPIOB_BASE + 0x04))
#define GPIOB_ODR   (*(volatile uint32_t*)(GPIOB_BASE + 0x0C))

#define GPIOB_CRL   (*(volatile uint32_t*)(GPIOB_BASE)) // Configuración pines B 8–15
#define GPIOB_IDR   (*(volatile uint32_t*)(GPIOB_BASE + 0x08)) // Entrada GPIOB

int main(void)
{
    // Habilitar clock de GPIOB y GPIOC
    RCC_APB2ENR |= (1 << 4); // Bit 4 → habilita GPIOC
    RCC_APB2ENR |= (1 << 3); // Bit 2 → habilita GPIOB

    // Configurar PC13 como salida push-pull, 2 MHz
    GPIOC_CRH &= ~(0xF << 20); // Limpia la configuracion anterior del pin 13
    GPIOC_CRH |=  (0x2 << 20); // Pone como salida push-pull, velocidad 2 MHz

    // Configurar PB11 como entrada con pull-up
    GPIOB_CRH &= ~(0xF << 12);  
    GPIOB_CRH |=  (0x8 << 12);  // 1000 = entrada con pull-up/pull-down
    GPIOB_ODR |=  (1 << 11);     // Activo el pull-up (default HIGH)

    // Bucle infinito: LED
    //Led de placa se pone entre PC13 y 3,3V, por eso poner PC13 en 1 anula corriente y lo apaga
    for(;;){
    if(GPIOB_IDR & (1 << 11)) { // Lee la entrada
        // Si PB11 está en alto → LED OFF
        GPIOC_ODR |=  (1 << 13);
    } else {
        // Si PB11 está en bajo → LED ON
        GPIOC_ODR &= ~(1 << 13);
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