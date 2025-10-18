// Definiciones manuales de enteros con tamanos fijos.
typedef int            int32_t;
typedef short          int16_t;
typedef char           int8_t;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

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

#define SYST_CSR (*(volatile uint32_t*)0xE000E010)
#define SYST_RVR (*(volatile uint32_t*)0xE000E014)
#define SYST_CVR (*(volatile uint32_t*)0xE000E018)
#define SYST_CALIB (*(volatile uint32_t*)0xE000E01C)

int main(void)
{
    // Habilitar clock de GPIOB
    RCC_APB2ENR |= (1 << 3); // Bit 3 → habilita GPIOB

    // Configurar PB11 como salida push-pull 2 MHz
    GPIOB_CRH &= ~(0xF << 12); // limpiar bits 12–15
    GPIOB_CRH |=  (0x2 << 12); // MODE11=10 (2MHz)
    GPIOB_CRH |=  (0x0 << 14); // CNF11=00 push-pull

    // Configurar SysTick
    // Tiempo = (ValorRecarga + 1) / Frecuencia del CPU
    SYST_RVR = 72000000 - 1;    // recarga para contar 72M de ciclos → ~1 s si el core corre a 72 MHz
    // 7200000 → 0,1 segundos.
    // 720000 → 0,01 segundos.

    SYST_CVR = 0;              // limpia valor actual
    SYST_CSR = 0x5;            // habilita SysTick con clock interno, sin interrupción (ENABLE + CLKSOURCE)

    while(1) {
        // Esperar a que COUNTFLAG se ponga en 1 (polling)
        while((SYST_CSR & (1 << 16)) == 0); // espera a que el bit 16 (COUNTFLAG) se setee

        // Cambiar estado del LED (PB11 activo bajo → usar XOR)
        GPIOB_ODR ^= (1 << 11);
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
