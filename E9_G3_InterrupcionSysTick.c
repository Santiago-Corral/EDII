// Definiciones manuales de enteros con tamanos fijos.
typedef int            int32_t;
typedef short          int16_t;
typedef char           int8_t;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;
typedef void (*interrupt_t)(void);

// Direcciones base
#define RCC_BASE    0x40021000
#define GPIOC_BASE  0x40011000
#define GPIOB_BASE  0x40010C00

// Registros RCC
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x18))

// Registros GPIOC (LED interno PC13)
#define GPIOC_CRH   (*(volatile uint32_t*)(GPIOC_BASE + 0x04))
#define GPIOC_ODR   (*(volatile uint32_t*)(GPIOC_BASE + 0x0C))

// Registros GPIOB (LED externo PB11)
#define GPIOB_CRH   (*(volatile uint32_t*)(GPIOB_BASE + 0x04))
#define GPIOB_ODR   (*(volatile uint32_t*)(GPIOB_BASE + 0x0C))

// SysTick
#define SYST_CSR    (*(volatile uint32_t*)0xE000E010) // Control/Status
#define SYST_RVR    (*(volatile uint32_t*)0xE000E014) // Reload value
#define SYST_CVR    (*(volatile uint32_t*)0xE000E018) // Current value
#define SYST_CALIB  (*(volatile uint32_t*)0xE000E01C) // (no usado)

// Stack inicial
#define SRAM_SIZE   ((uint32_t)0x5000)
#define SRAM_BASE   ((uint32_t)0x20000000)
#define STACKINIT   ((interrupt_t)(SRAM_BASE+SRAM_SIZE))

// =====================
// Handler de SysTick
// =====================
void SysTick_Handler(void)
{
    // Toggle ambos LEDs:
    // - PC13 (interno, activo en bajo) → toggle igual funciona
    // - PB11 (externo, típico activo en alto) → toggle igual funciona
    GPIOC_ODR ^= (1 << 13);
    GPIOB_ODR ^= (1 << 11);
}

int main(void)
{
    // Habilitar clock de GPIOB y GPIOC (APB2)
    RCC_APB2ENR |= (1 << 3); // GPIOB
    RCC_APB2ENR |= (1 << 4); // GPIOC

    // Configurar PC13 como salida push-pull 2 MHz
    GPIOC_CRH &= ~(0xF << 20); // bits 23:20
    GPIOC_CRH |=  (0x2 << 20); // MODE13=10 (2 MHz), CNF13=00 (PP)
    // LED interno activo en bajo: arrancar apagado → PC13=1
    GPIOC_ODR |= (1 << 13);

    // Configurar PB11 como salida push-pull 2 MHz
    GPIOB_CRH &= ~(0xF << 12); // bits 15:12
    GPIOB_CRH |=  (0x2 << 12); // MODE11=10 (2 MHz), CNF11=00 (PP)
    // LED externo típico activo en alto: arrancar apagado → PB11=0
    GPIOB_ODR &= ~(1 << 11);

    // Configurar SysTick para ~1 s si el core corre a 72 MHz
    SYST_RVR = 720000 - 1; // período = (RVR+1)/fclk - cantidad de ciclos de CPU que SysTick debe contar antes de lanzar la interrupción.
    SYST_CVR = 0;             // limpiar contador
    // ENABLE(0)=1 | TICKINT(1)=1 | CLKSOURCE(2)=1 → 0b111 = 0x7
    SYST_CSR = 0x7;
    //TICKINT = 1 → quiero interrupción cuando llegue a 0.

    while (1) {
        // Todo ocurre en la interrupción
    }

    return 0;
}

// =====================
// Vector de interrupciones
// =====================
const interrupt_t vector_table[256] __attribute__ ((section(".vtab"))) = {
    STACKINIT,                // [0]  Stack Pointer
    (interrupt_t) main,       // [1]  Reset
    0,                        // [2]  NMI
    0,                        // [3]  HardFault
    0,                        // [4]  MemManage
    0,                        // [5]  BusFault
    0,                        // [6]  UsageFault
    0,0,0,0,                  // [7..10] Reservados
    0,                        // [11] SVCall
    0,                        // [12] DebugMonitor
    0,                        // [13] Reservado
    0,                        // [14] PendSV
    [15] = SysTick_Handler    // [15] SysTick IRQ
    // resto en 0
};

//El NVIC no se programa explícitamente, porque SysTick ya está “cableado” dentro del núcleo.
//Entonces Countflag = 1 cuando el contador llega a cero, si tickint = 1, se genera la interrupción automáticamente.