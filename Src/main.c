#include <stdint.h>
#include "stm32F446re.h"
#include "stm32F446re_gpio.h"

// ==========================================
// Pin Mapping:
// EnA (En1) -> D11 --> PA_7  (Left motor enable)
// IN1       -> D8  --> PA_9  (Left motor direction 1)
// IN2       -> D9  --> PC_7  (Left motor direction 2)
// IN3       -> D6  --> PB_10 (Right motor direction 1)
// IN4       -> D7  --> PA_8  (Right motor direction 2)
// EnB       -> D10 --> PB_6  (Right motor enable)
// ==========================================

#define PWM_STEP 500

/**************************************************************************************
 * @fn          - move_forward
 * @brief       - Sets directional inputs to drive robot forward
 *************************************************************************************/
void move_forward(void)
{
	// Left motor forward
	GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_9, DISABLE);  // IN1 (PA9) = LOW
	GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_NO_7, ENABLE);   // IN2 (PC7) = HIGH

	// Right motor forward
	GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_10, ENABLE);  // IN3 (PB10) = HIGH
	GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_8, DISABLE);  // IN4 (PA8)  = LOW
}

/**************************************************************************************
 * @fn          - move_backword
 * @brief       - Reverses directional inputs to drive robot backward
 *************************************************************************************/
void move_backword(void)
{
	// Left motor backward
	GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_9, ENABLE);   // IN1 (PA9) = HIGH
	GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_NO_7, DISABLE);  // IN2 (PC7) = LOW

	// Right motor backward
	GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_10, DISABLE); // IN3 (PB10) = LOW
	GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_8, ENABLE);   // IN4 (PA8)  = HIGH
}

/**************************************************************************************
 * @fn          - move_brake
 * @brief       - Performs active electrical brake by pulling all motor inputs HIGH
 *************************************************************************************/
void move_brake(void)
{
	GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_9, ENABLE);   // IN1
	GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_NO_7, ENABLE);   // IN2
	GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_10, ENABLE);  // IN3
	GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_8, ENABLE);   // IN4
}

/**************************************************************************************
 * @fn          - turn_right
 * @brief       - In-place right turn (Left forward, Right backward)
 *************************************************************************************/
void turn_right(void)
{
	GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_9, DISABLE);  // IN1 = LOW
	GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_NO_7, ENABLE);   // IN2 = HIGH
	GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_10, DISABLE); // IN3 = LOW
	GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_8, ENABLE);   // IN4 = HIGH
}

/**************************************************************************************
 * @fn          - turn_left
 * @brief       - In-place left turn (Left backward, Right forward)
 *************************************************************************************/
void turn_left(void)
{
	GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_9, ENABLE);   // IN1 = HIGH
	GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_NO_7, DISABLE);  // IN2 = LOW
	GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_10, ENABLE);  // IN3 = HIGH
	GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_8, DISABLE);  // IN4 = LOW
}

/**************************************************************************************
 * @fn          - delay
 * @brief       - Busy-wait delay (~250 ms at 16 MHz)
 *************************************************************************************/
void delay(void)
{
	for (volatile uint32_t i = 0; i < 1000000; i++);
}

/**************************************************************************************
 * @fn          - move_forward_speed
 * @brief       - Moves forward at specified speed (0-10) using software PWM
 *************************************************************************************/
void move_forward_speed(uint8_t speed, uint32_t duration_ms)
{
	if (speed > 10) speed = 10;

	move_forward();

	uint32_t cycles = (duration_ms * 800) / 1000;

	for (uint32_t c = 0; c < cycles; c++)
	{
		// PWM ON phase
		if (speed > 0)
		{
			GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_7, ENABLE);   // EnA (PA7) ON
			GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_6, ENABLE);   // EnB (PB6) ON
			for (volatile uint32_t i = 0; i < speed * PWM_STEP; i++);
		}

		// PWM OFF phase
		if (speed < 10)
		{
			GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_7, DISABLE);  // EnA (PA7) OFF
			GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_6, DISABLE);  // EnB (PB6) OFF
			for (volatile uint32_t i = 0; i < (10 - speed) * PWM_STEP; i++);
		}
	}
}

/**************************************************************************************
 * @fn          - move_backword_speed
 * @brief       - Moves backward at specified speed (0-10) using software PWM
 *************************************************************************************/
void move_backword_speed(uint8_t speed, uint32_t duration_ms)
{
	if (speed > 10) speed = 10;

	move_backword();

	uint32_t cycles = (duration_ms * 800) / 1000;

	for (uint32_t c = 0; c < cycles; c++)
	{
		// PWM ON phase
		if (speed > 0)
		{
			GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_7, ENABLE);   // EnA (PA7) ON
			GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_6, ENABLE);   // EnB (PB6) ON
			for (volatile uint32_t i = 0; i < speed * PWM_STEP; i++);
		}

		// PWM OFF phase
		if (speed < 10)
		{
			GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_7, DISABLE);  // EnA (PA7) OFF
			GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_6, DISABLE);  // EnB (PB6) OFF
			for (volatile uint32_t i = 0; i < (10 - speed) * PWM_STEP; i++);
		}
	}
}

int main(void)
{
	// Enable peripheral clocks for GPIO ports A, B, and C
	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_PeriClockControl(GPIOB, ENABLE);
	GPIO_PeriClockControl(GPIOC, ENABLE);

	// IN1 -> D8 (PA9)
	GPIO_Handle_t GPIO_HBridge_In1;
	GPIO_HBridge_In1.pGPIOx = GPIOA;
	GPIO_HBridge_In1.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	GPIO_HBridge_In1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIO_HBridge_In1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_HBridge_In1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIO_HBridge_In1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	GPIO_Init(&GPIO_HBridge_In1);

	// IN2 -> D9 (PC7)
	GPIO_Handle_t GPIO_HBridge_In2;
	GPIO_HBridge_In2.pGPIOx = GPIOC;
	GPIO_HBridge_In2.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_HBridge_In2.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIO_HBridge_In2.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIO_HBridge_In2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_HBridge_In2.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	GPIO_Init(&GPIO_HBridge_In2);

	// IN3 -> D6 (PB10)
	GPIO_Handle_t GPIO_HBridge_In3;
	GPIO_HBridge_In3.pGPIOx = GPIOB;
	GPIO_HBridge_In3.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GPIO_HBridge_In3.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIO_HBridge_In3.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIO_HBridge_In3.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_HBridge_In3.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	GPIO_Init(&GPIO_HBridge_In3);

	// IN4 -> D7 (PA8)
	GPIO_Handle_t GPIO_HBridge_In4;
	GPIO_HBridge_In4.pGPIOx = GPIOA;
	GPIO_HBridge_In4.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
	GPIO_HBridge_In4.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIO_HBridge_In4.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIO_HBridge_In4.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_HBridge_In4.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	GPIO_Init(&GPIO_HBridge_In4);

	// EnA (En1) -> D11 (PA7)
	GPIO_Handle_t GPIO_HBridge_EnA;
	GPIO_HBridge_EnA.pGPIOx = GPIOA;
	GPIO_HBridge_EnA.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_HBridge_EnA.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIO_HBridge_EnA.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIO_HBridge_EnA.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_HBridge_EnA.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	GPIO_Init(&GPIO_HBridge_EnA);

	// EnB -> D10 (PB6)
	GPIO_Handle_t GPIO_HBridge_EnB;
	GPIO_HBridge_EnB.pGPIOx = GPIOB;
	GPIO_HBridge_EnB.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_HBridge_EnB.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIO_HBridge_EnB.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIO_HBridge_EnB.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_HBridge_EnB.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	GPIO_Init(&GPIO_HBridge_EnB);

	// Initialize Enables HIGH
	GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_7, ENABLE);   // EnA
	GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_6, ENABLE);   // EnB

	while (1)
	{
	    GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_7, ENABLE);
	    GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_6, ENABLE);

	    move_forward();

	    delay();
	}
}
