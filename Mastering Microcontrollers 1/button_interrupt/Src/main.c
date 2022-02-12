
#include <stdint.h>

#define GPIO_MOD_REG_OFFSET	0x00UL
#define GPIO_ODR_REG_OFFSET	0x14UL
#define GPIOE_BASE_ADDR		0x48001000UL
#define GPIOE_MOD_REG_ADDR	((GPIOE_BASE_ADDR) + (GPIO_MOD_REG_OFFSET))
#define GPIOE_ODR_REG_ADDR	((GPIOE_BASE_ADDR) + (GPIO_ODR_REG_OFFSET))

#define RCC_AHB_BASE_ADDR	0x40021000UL
#define RCC_AHBENR_OFFSET	0x14
#define RCC_AHBENR_REG_ADDR	((RCC_AHB_BASE_ADDR)+(RCC_AHBENR_OFFSET))

#define IOPEEN_BIT			21


const int gpioPort = 15; // led connected to GPIOE15

int main(void)
{
	uint32_t *pRccAhbEnrReg   = (uint32_t*)RCC_AHBENR_REG_ADDR;
	*pRccAhbEnrReg |= ( 1 << IOPEEN_BIT);

	uint32_t* pGpioEModReg = (uint32_t*)GPIOE_MOD_REG_ADDR;
	// Set 01 to 31|30 bits
	*pGpioEModReg |= (0x01<<(2*gpioPort));

	uint32_t *pGpioEOdrReg = (uint32_t*)GPIOE_ODR_REG_ADDR;
	*pGpioEOdrReg |= ( 1 << gpioPort);

	/* Loop forever */
	for(;;);
}
