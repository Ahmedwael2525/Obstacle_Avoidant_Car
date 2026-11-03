/*
 * stm32F446re_gpio.c
 *
 *  Created on: Oct 16, 2026
 *      Author: Asus
 */

#include "stm32F446re_gpio.h"

/**************************************************************************************
 * @fn          - GPIO_PeriClockControl
 *
 * @brief       - Enables or disables peripheral clock for the given GPIO port
 *
 * @param[1]    - Base address of the GPIO port (typecast to GPIO_RegDef_t*)
 * @param[2]    - ENABLE or DISABLE macros
 *
 * @return      - None
 *
 * @Note        - Must be called before accessing any port registers
 **************************************************************************************/
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint32_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{

			GPIOA_PCLK_EN();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}
		else if(pGPIOx == GPIOC)
				{
					GPIOC_PCLK_EN();
				}
		else if(pGPIOx == GPIOD)
				{
					GPIOD_PCLK_EN();
				}
		else if(pGPIOx == GPIOE)
				{
					GPIOE_PCLK_EN();
				}
		else if(pGPIOx == GPIOF)
				{
					GPIOF_PCLK_EN();
				}
		else if(pGPIOx == GPIOG)
				{
					GPIOG_PCLK_EN();
				}
		else if(pGPIOx == GPIOH)
				{
					GPIOH_PCLK_EN();
				}
	}
	else
	{
		if(pGPIOx == GPIOA)
				{

					GPIOA_PCLK_DI();
				}
				else if(pGPIOx == GPIOB)
				{
					GPIOB_PCLK_DI();
				}
				else if(pGPIOx == GPIOC)
						{
							GPIOC_PCLK_DI();
						}
				else if(pGPIOx == GPIOD)
						{
							GPIOD_PCLK_DI();
						}
				else if(pGPIOx == GPIOE)
						{
							GPIOE_PCLK_DI();
						}
				else if(pGPIOx == GPIOF)
						{
							GPIOF_PCLK_DI();
						}
				else if(pGPIOx == GPIOG)
						{
							GPIOG_PCLK_DI();
						}
				else if(pGPIOx == GPIOH)
						{
							GPIOH_PCLK_DI();
						}

	}

}

/**************************************************************************************
 * @fn          - GPIO_Init
 *
 * @brief       - Initializes a GPIO pin according to the settings in GPIO_Handle_t. You will configure all
 * 				  parameters of the pin [Mode,Speed....].
 * 				  Simply it's about setting the things on the hardware
 *
 * @param[1]    - Pointer to the GPIO_Handle_t structure containing port and pin settings
 *
 * @return      - None
 *
 * @Note        - Writes to MODER, OTYPER, OSPEEDR, PUPDR, and AFR registers
 **************************************************************************************/
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	//1. Configure The mode of the Pin
	uint32_t temp = 0;

	if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		/* Since the mode takes two bits to configure [for ex: 01,11] so we have to shift the mode two bits
		 * toward the correct pin number
		 */
		temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		pGPIOHandle->pGPIOx->MODER &= ~ (0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); 	// Clearning the  bits from any values in it
		pGPIOHandle->pGPIOx->MODER |= temp;			// We are putting shift OR to keep any other register and only change this exact pin speed
		temp = 0;
	}
	else
	{

	}
	//2. Configure The speed
	temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OSPEEDR &= ~ (0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); 	// Clearning the bits from any values in it
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;
	temp = 0;

	//3. Configure the pull up / pull down options
	temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->PUPDR &= ~ (0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); 	// Clearning the bits from any values in it
	pGPIOHandle->pGPIOx->PUPDR |= temp;
	temp = 0;

	//4. Configure the output mode OpType

	temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
	pGPIOHandle->pGPIOx->OTYPER &= (0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); 	// Clearning the bits from any values in it
	pGPIOHandle->pGPIOx->OTYPER |= temp;
	temp = 0;

	//5. Configure the alternate functionality

	uint8_t temp1;		// temp 1 to know if it's low or high register
	uint8_t temp2;		// temp 2 to know what order of the pin to know how many bits should we shift

	temp1= pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
	temp2= pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
	pGPIOHandle->pGPIOx->AFR[temp1] &= ~ (0xF << 4 * temp2); 	// Clearning the bits from any values in it


	pGPIOHandle->pGPIOx->AFR[temp1]= pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode <<(4 * temp2);









}

/**************************************************************************************
 * @fn          - GPIO_DeInit
 *
 * @brief       - Resets all registers of the given GPIO port back to factory reset state
 *
 * @param[1]    - Base address of the GPIO port (typecast to GPIO_RegDef_t*)
 *
 * @return      - None
 *
 * @Note        - Uses the RCC_AHB1RSTR register macros to pulse a reset
 **************************************************************************************/
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
    if(pGPIOx == GPIOA)
    {
        GPIOA_REG_RESET();
    }
    else if(pGPIOx == GPIOB)
    {
        GPIOB_REG_RESET();
    }
    else if(pGPIOx == GPIOC)
    {
        GPIOC_REG_RESET();
    }
    else if(pGPIOx == GPIOD)
    {
        GPIOD_REG_RESET();
    }
    else if(pGPIOx == GPIOE)
    {
        GPIOE_REG_RESET();
    }
    else if(pGPIOx == GPIOF)
    {
        GPIOF_REG_RESET();
    }
    else if(pGPIOx == GPIOG)
    {
        GPIOG_REG_RESET();
    }
    else if(pGPIOx == GPIOH)
    {
        GPIOH_REG_RESET();
    }
}
/**************************************************************************************
 * @fn          - GPIO_ReadFromInputPin
 *
 * @brief       - Reads the logic level (0 or 1) of a specific input pin
 *
 * @param[1]    - Base address of the GPIO port
 * @param[2]    - Pin number to read from (0 to 15)
 *
 * @return      - 0 or 1 (value read from the pin)
 *
 * @Note        - Reads from the IDR (Input Data Register)
 **************************************************************************************/
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint32_t PinNumber)
{
	uint8_t value;
	value = (uint8_t) (pGPIOx->IDR >> PinNumber) & 0x00000001  ;
	return value;
}

/**************************************************************************************
 * @fn          - GPIO_ReadFromInputPort
 *
 * @brief       - Reads the entire 16-bit status of an input port
 *
 * @param[1]    - Base address of the GPIO port
 *
 * @return      - 16-bit integer reflecting the states of all 16 pins
 *
 * @Note        - Reads all bits of the IDR (Input Data Register)
 **************************************************************************************/
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = ((uint16_t) (pGPIOx->IDR) );
	return value;
}

/**************************************************************************************
 * @fn          - GPIO_WriteToOutputPin
 *
 * @brief       - Writes logic HIGH (1) or LOW (0) to a specific output pin
 *
 * @param[1]    - Base address of the GPIO port
 * @param[2]    - Pin number to write to (0 to 15)
 * @param[3]    - Output value (GPIO_PIN_SET or GPIO_PIN_RESET)
 *
 * @return      - None
 *
 * @Note        - Modifies the corresponding bit in the ODR (Output Data Register)
 **************************************************************************************/
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint32_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		pGPIOx->ODR |= 1 << PinNumber;
	}
	else
	{
		pGPIOx->ODR &= ~ (1 << PinNumber);
	}

}
/**************************************************************************************
 * @fn          - GPIO_WriteToOutputPort
 *
 * @brief       - Writes a 16-bit pattern across all 16 pins of a port simultaneously
 *
 * @param[1]    - Base address of the GPIO port
 * @param[2]    - 16-bit value to write to the port
 *
 * @return      - None
 *
 * @Note        - Overwrites the entire ODR (Output Data Register)
 **************************************************************************************/
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}

/**************************************************************************************
 * @fn          - GPIO_ToggleOutputPin
 *
 * @brief       - Inverts the current state of a specific output pin
 *
 * @param[1]    - Base address of the GPIO port
 * @param[2]    - Pin number to toggle (0 to 15)
 *
 * @return      - None
 *
 * @Note        - Bitwise XORs the target pin bit in the ODR
 **************************************************************************************/
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint32_t PinNumber)
{
	pGPIOx->ODR = pGPIOx->ODR ^= (1<<PinNumber);
}

/**************************************************************************************
 * @fn          - GPIO_IRQInterruptConfig
 *
 * @brief       - Enables/disables an interrupt line in the ARM Cortex-M4 NVIC
 *
 * @param[1]    - IRQ position number in the vector table
 * @param[2]    - ENABLE or DISABLE macro
 *
 * @return      - None
 *
 * @Note        - Configures processor-core NVIC ISER and ICER registers
 **************************************************************************************/
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

}

/**************************************************************************************
 * @fn          - GPIO_IRQPriorityConfig
 *
 * @brief       - Configures the priority level for a specific IRQ line
 *
 * @param[1]    - IRQ position number in the vector table
 * @param[2]    - Priority level to assign
 *
 * @return      - None
 *
 * @Note        - Configures processor-core NVIC IPR registers
 **************************************************************************************/
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{

}

/**************************************************************************************
 * @fn          - GPIO_IRQHandling
 *
 * @brief       - Clears the pending interrupt flag for a specific pin
 *
 * @param[1]    - Pin number corresponding to the EXTI line triggered
 *
 * @return      - None
 *
 * @Note        - Clears the pending bit in EXTI_PR by writing a 1 to it
 **************************************************************************************/
void GPIO_IRQHandling(uint32_t PinNumber)
{

}
