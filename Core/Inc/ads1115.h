/*
 * ads1115.h
 *
 *  Created on: Oct 29, 2023
 *      Author: Admin
 */

#ifndef INC_ADS1115_H_
#define INC_ADS1115_H_

#include <stdint.h>
#include <stdbool.h>

/* ADS1115 ADDRESS POINTER REGISTER */
#define ADS_CONV_REGISTER	0x00
#define ADS_CONF_REGISTER	0x01
#define ADS_LoTH_REGISTER	0x02
#define ADS_HiTH_REGISTER	0x03
/*------------------------------------*/


/* ADS1115 CONFIG_REGISTER */
#define ADS_START				0x80
#define ADS_BUSY				0x80

#define ADS_AIN0_AIN1			0x00	// default
#define ADS_AIN0_AIN3			0x10
#define ADS_AIN1_AIN3			0x20
#define ADS_AIN2_AIN3			0x30
#define ADS_AIN0_GND			0x40
#define ADS_AIN1_GND			0x50
#define ADS_AIN2_GND			0x60
#define ADS_AIN3_GND			0x70

#define ADS_FSR_6_144_V			0x00
#define ADS_FSR_4_096_V			0x02
#define ADS_FSR_2_048_V			0x04	// default
#define ADS_FSR_1_024_V			0x06
#define ADS_FSR_0_512_V			0x08
#define ADS_FSR_0_256_V			0x0A

#define ADS_CONTINUOUS_MODE		0x00
#define ADS_SINGLE_MODE			0x01	// default
#define ADS_INTERRUPT_MODE		0xFF

#define ADS_DR_8_SPS			0x00
#define ADS_DR_16_SPS			0x20
#define ADS_DR_32_SPS			0x40
#define ADS_DR_64_SPS			0x60
#define ADS_DR_128_SPS			0x80	// default
#define ADS_DR_250_SPS			0xA0
#define ADS_DR_475_SPS			0xC0
#define ADS_DR_860_SPS			0xE0

#define ADS_TRADITIONAL_COMP	0x00	// default
#define ADS_WINDOW_COMP			0x10

#define ADS_COMP_ACTIVE_LOW		0x00	// default
#define ADS_COMP_ACTIVE_HIGH	0x08

#define ADS_NON_LATCHING_COMP	0x00	// default
#define ADS_LATCHING_COMP		0x04

#define ADS_COMP_DISABLE			0x03	// default
#define ADS_COMP_ASSERT_ONE_CONV	0x00
#define ADS_COMP_ASSERT_TWO_CONV	0x01
#define ADS_COMP_ASSERT_FOUR_CONV	0x02
/*----------------------------------------------------*/

#define ADS_ADDRESS_GND		0x48
#define ADS_ADDRESS_VDD 	0x49
#define ADS_ADDRESS_SDA		0x4A
#define ADS_ADDRESS_SCL		0x4B

#define ADC_RESOLUTION	65536

typedef struct {
	uint8_t os;
	uint8_t address;
	uint8_t input_mux;
	uint8_t fsr;
	uint8_t mode;
	uint8_t data_rate;
	uint8_t comp_mode;
	uint8_t comp_pol;
	uint8_t comp_lat;
	uint8_t comp_que;
} ads_config;

extern ads_config ADS1115;

#define ADS_SET_INPUT_MUX(x)	(ADS1115.input_mux = (x))
#define ADS_SET_DATA_RATE(x)	(ADS1115.data_rate = (x))
#define ADS_SET_ADDRESS(x)		(ADS1115.address = (x))
#define ADS_SET_MODE(x)			(ADS1115.mode = (x))
#define ADS_SET_COMP_MODE(x)	(ADS1115.comp_mode = (x))
#define ADS_SET_COMP_POL(x)		(ADS1115.comp_pol = (x))
#define ADS_SET_COMP_LAT(x)		(ADS1115.comp_lat = (x))
#define ADS_SET_COMP_QUE(x)		(ADS1115.comp_que = (x))

#define ADS_GET_INPUT_MUX		(ADS1115.input_mux)
#define ADS_GET_DATA_RATE		(ADS1115.data_rate)
#define ADS_GET_ADDRESS			(ADS1115.address)
#define ADS_GET_MODE			(ADS1115.mode)
#define ADS_GET_COMP_MODE		(ADS1115.comp_mode)
#define ADS_GET_COMP_POL		(ADS1115.comp_pol)
#define ADS_GET_COMP_LAT		(ADS1115.comp_lat)
#define ADS_GET_COMP_QUE		(ADS1115.comp_que)

void ADS_startSingle(void);
void ADS_startContinuous(void);
void ADS_startInterrupt(void);
float ADS_getLSB(void);
bool ADS_busy(void);
int16_t ADS_read(void);

#endif /* INC_ADS1115_H_ */
