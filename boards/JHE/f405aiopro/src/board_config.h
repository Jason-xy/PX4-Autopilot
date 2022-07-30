/****************************************************************************
 *
 *   Copyright (c) 2018, 2014 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file board_config.h
 *
 * JHEF405Pro internal definitions
 */

#pragma once

/****************************************************************************************************
 * Included Files
 ****************************************************************************************************/

#include <px4_platform_common/px4_config.h>
#include <nuttx/compiler.h>
#include <stdint.h>

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/
/* Configuration ************************************************************************************/

/* JHF F105AIO GPIOs ***********************************************************************************/
/* LEDs */
// power - green
// LED1 - PB5 - blue
#define GPIO_LED1       (GPIO_OUTPUT|GPIO_OPENDRAIN|GPIO_SPEED_50MHz|GPIO_OUTPUT_SET|GPIO_PORTC|GPIO_PIN14)
#define GPIO_LED_BLUE   GPIO_LED1

#define BOARD_HAS_CONTROL_STATUS_LEDS      1
#define BOARD_ARMED_STATE_LED  LED_BLUE

#define  FLASH_BASED_PARAMS

#define ADC1_CH(n)                  (n)
#define ADC1_GPIO(n)                GPIO_ADC1_IN##n

#define PX4_ADC_GPIO  \
	/* PC0 */  ADC1_GPIO(10),  \
	/* PC3 */  ADC1_GPIO(12),  \
	/* PC2 */  ADC1_GPIO(13)

/* Define Channel numbers must match above GPIO pin IN(n)*/
#define ADC_RSSI_IN_CHANNEL                 ADC1_CH(10) //C00
#define ADC_BATTERY_VOLTAGE_CHANNEL         ADC1_CH(13) //C03
#define ADC_BATTERY_CURRENT_CHANNEL         ADC1_CH(12) //C02

#define ADC_CHANNELS \
	((1 << ADC_BATTERY_VOLTAGE_CHANNEL)       | \
	 (1 << ADC_BATTERY_CURRENT_CHANNEL)       | \
	 (1 << ADC_RSSI_IN_CHANNEL))

/* Define Battery 1 Voltage Divider and A per V
 */
#define BOARD_BATTERY1_V_DIV         (10.9f)
#define BOARD_BATTERY1_A_PER_V       (17.f)

#define BOARD_NUMBER_BRICKS             1
#define BOARD_ADC_BRICK_VALID   (1)

#define GPIO_TONE_ALARM_IDLE    /* PD15 */ (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_2MHz|GPIO_OUTPUT_CLEAR|GPIO_PORTC|GPIO_PIN13)
#define GPIO_TONE_ALARM_GPIO    /* PD15 */ (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_2MHz|GPIO_OUTPUT_SET|GPIO_PORTC|GPIO_PIN13)

/* User GPIOs
 *
 * GPIO0-5 are the PWM servo outputs.
 * GPIO_TIM3_CH3OUT        GPIO_TIM3_CH3OUT_1 //PB0 S1_OUT D1_ST7
 * GPIO_TIM3_CH4OUT        GPIO_TIM3_CH4OUT_1 //PB1 S2_OUT D1_ST2
 * GPIO_TIM2_CH4OUT        GPIO_TIM2_CH4OUT_1 //PA3 S3_OUT D1_ST6
 * GPIO_TIM2_CH3OUT        GPIO_TIM2_CH3OUT_1 //PA2 S4_OUT D1_ST1
 * GPIO_TIM5_CH2OUT        GPIO_TIM5_CH2OUT_1 //PA1 S5_OUT
 * GPIO_TIM1_CH1OUT        GPIO_TIM1_CH1OUT_1 //PA8 S6_OUT
 */

#define _MK_GPIO_INPUT(def) (((def) & (GPIO_PORT_MASK | GPIO_PIN_MASK)) | (GPIO_INPUT|GPIO_PULLUP))

#define GPIO_GPIO0_INPUT             _MK_GPIO_INPUT(GPIO_TIM3_CH3IN)
#define GPIO_GPIO1_INPUT             _MK_GPIO_INPUT(GPIO_TIM3_CH4IN)
#define GPIO_GPIO2_INPUT             _MK_GPIO_INPUT(GPIO_TIM2_CH4IN)
#define GPIO_GPIO3_INPUT             _MK_GPIO_INPUT(GPIO_TIM2_CH3IN)
//#define GPIO_GPIO4_INPUT             _MK_GPIO_INPUT(GPIO_TIM5_CH2IN)
//#define GPIO_GPIO5_INPUT             _MK_GPIO_INPUT(GPIO_TIM1_CH1IN)

#define _MK_GPIO_OUTPUT(def) (((def) & (GPIO_PORT_MASK | GPIO_PIN_MASK)) | (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_2MHz|GPIO_OUTPUT_CLEAR))

//This is the CORRECT MAPPING
// #define GPIO_GPIO0_OUTPUT            _MK_GPIO_OUTPUT(GPIO_TIM3_CH3OUT)
// #define GPIO_GPIO1_OUTPUT            _MK_GPIO_OUTPUT(GPIO_TIM3_CH4OUT)
// #define GPIO_GPIO2_OUTPUT            _MK_GPIO_OUTPUT(GPIO_TIM2_CH4OUT)
// #define GPIO_GPIO3_OUTPUT            _MK_GPIO_OUTPUT(GPIO_TIM2_CH3OUT)

//This IS FOR CL35
#define GPIO_GPIO0_OUTPUT            _MK_GPIO_OUTPUT(GPIO_TIM2_CH3OUT)
#define GPIO_GPIO1_OUTPUT            _MK_GPIO_OUTPUT(GPIO_TIM3_CH3OUT)
#define GPIO_GPIO2_OUTPUT            _MK_GPIO_OUTPUT(GPIO_TIM3_CH4OUT)
#define GPIO_GPIO3_OUTPUT            _MK_GPIO_OUTPUT(GPIO_TIM2_CH4OUT)
//#define GPIO_GPIO4_OUTPUT            _MK_GPIO_OUTPUT(GPIO_TIM5_CH2OUT)
//#define GPIO_GPIO5_OUTPUT            _MK_GPIO_OUTPUT(GPIO_TIM1_CH1OUT)

/* USB OTG FS
 *
 * PA9  OTG_FS_VBUS VBUS sensing
 */
#define GPIO_OTGFS_VBUS		(GPIO_INPUT|GPIO_FLOAT|GPIO_SPEED_100MHz|GPIO_OPENDRAIN|GPIO_PORTC|GPIO_PIN5)

/* PWM
 *
 * Alternatively CH3/CH4 could be assigned to UART6_TX/RX
 */
#define DIRECT_PWM_OUTPUT_CHANNELS      4
#define DIRECT_INPUT_TIMER_CHANNELS  4

// Has pwm outputs
#define BOARD_HAS_PWM    DIRECT_PWM_OUTPUT_CHANNELS

/* High-resolution timer */
#define HRT_TIMER                    4 // T4C1
#define HRT_TIMER_CHANNEL            1 // use capture/compare channel 1

#define HRT_PPM_CHANNEL              3 // capture/compare channel 3
#define GPIO_PPM_IN                  (GPIO_ALT|GPIO_AF2|GPIO_PULLUP|GPIO_PORTB|GPIO_PIN8)

#define RC_SERIAL_PORT               "/dev/ttyS3"
#define BOARD_SUPPORTS_RC_SERIAL_PORT_OUTPUT
#define GPIO_RSSI_IN                      (GPIO_INPUT|GPIO_PULLUP|GPIO_PORTC|GPIO_PIN0)

/*
 * One RC_IN
 *
 * GPIO PPM_IN on PB8 T4CH3
 * SPEKTRUM_RX (it's TX or RX in Bind) on PA10 UART1
 * The FMU can drive GPIO PPM_IN as an output
 */
// TODO?
//#define GPIO_PPM_IN_AS_OUT            (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_2MHz|GPIO_OUTPUT_SET|GPIO_PORTC|GPIO_PIN6)
//#define SPEKTRUM_RX_AS_GPIO_OUTPUT()  px4_arch_configgpio(GPIO_PPM_IN_AS_OUT)
//#define SPEKTRUM_RX_AS_UART()         px4_arch_configgpio(GPIO_USART1_RX)
//#define SPEKTRUM_OUT(_one_true)       px4_arch_gpiowrite(GPIO_PPM_IN_AS_OUT, (_one_true))

#define BOARD_HAS_PWM    DIRECT_PWM_OUTPUT_CHANNELS

/* This board provides a DMA pool and APIs */
#define BOARD_DMA_ALLOC_POOL_SIZE 5120

#define BOARD_HAS_ON_RESET 1

#define BOARD_ENABLE_CONSOLE_BUFFER
#define BOARD_CONSOLE_BUFFER_SIZE (1024*3)

#define BOARD_DSHOT_MOTOR_ASSIGNMENT {0, 2, 3, 1};

#define PX4_GPIO_INIT_LIST { \
		PX4_ADC_GPIO,                     \
		GPIO_TONE_ALARM_IDLE,             \
		GPIO_RSSI_IN,                \
	}

__BEGIN_DECLS

/****************************************************************************************************
 * Public Types
 ****************************************************************************************************/

/****************************************************************************************************
 * Public data
 ****************************************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************************************
 * Public Functions
 ****************************************************************************************************/

/****************************************************************************************************
 * Name: stm32_spiinitialize
 *
 * Description:
 *   Called to configure SPI chip select GPIO pins for the PX4FMU board.
 *
 ****************************************************************************************************/

extern void stm32_spiinitialize(void);


/****************************************************************************************************
 * Name: stm32_usbinitialize
 *
 * Description:
 *   Called to configure USB IO.
 *
 ****************************************************************************************************/

extern void stm32_usbinitialize(void);

extern void board_peripheral_reset(int ms);

#include <px4_platform_common/board_common.h>

#endif /* __ASSEMBLY__ */

__END_DECLS
