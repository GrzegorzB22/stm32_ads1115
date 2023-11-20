/*
 * ads1115.c
 *
 *  Created on: Oct 29, 2023
 *      Author: Admin
 */

#include "ads1115.h"
#include "main.h"
#include "i2c.h"

extern I2C_HandleTypeDef hi2c1;

static uint8_t ADS_REGISTERS[4] = {ADS_CONV_REGISTER, ADS_CONF_REGISTER, ADS_LoTH_REGISTER, ADS_HiTH_REGISTER};

ads_config ADS1115 = {
		0, ADS_ADDRESS_GND, ADS_AIN0_AIN1, ADS_FSR_2_048_V, ADS_SINGLE_MODE, ADS_DR_128_SPS, ADS_TRADITIONAL_COMP, ADS_COMP_ACTIVE_LOW, ADS_NON_LATCHING_COMP, ADS_COMP_DISABLE
};

static void ADS_init(void)
{
	if (ADS1115.mode != ADS_INTERRUPT_MODE) {
		uint8_t ads_init_array[] = {
				ADS_REGISTERS[1],
				ADS1115.os | ADS1115.input_mux | ADS1115.fsr | ADS1115.mode,
				ADS1115.data_rate | ADS1115.comp_mode | ADS1115.comp_pol | ADS1115.comp_lat | ADS1115.comp_que
		};
		HAL_I2C_Master_Transmit(&hi2c1, ADS1115.address << 1, ads_init_array, sizeof(ads_init_array)/sizeof(ads_init_array[0]), HAL_MAX_DELAY);
	}
	else {
		uint8_t ads_init_array[3][3] = {
				{ADS_REGISTERS[3], 0x80, 0x00},
				{ADS_REGISTERS[2], 0x00, 0x00},
				{
						ADS_REGISTERS[1],
						ADS1115.os | ADS1115.input_mux | ADS1115.fsr,
						ADS1115.data_rate | ADS1115.comp_mode | ADS1115.comp_pol | ADS1115.comp_lat | ADS1115.comp_que
				}
		};
		for (uint8_t i = 0; i < 3; i++)
			HAL_I2C_Master_Transmit(&hi2c1, ADS1115.address << 1, ads_init_array[i], sizeof(ads_init_array[i])/sizeof(ads_init_array[i][0]), HAL_MAX_DELAY);
	}
}

void ADS_startInterrupt(void)
{
	ADS1115.mode = ADS_INTERRUPT_MODE;
	ADS1115.os = ADS_START;
	ADS1115.comp_que = ADS_COMP_ASSERT_ONE_CONV;
	ADS_init();
}

void ADS_startSingle(void)
{
	ADS1115.mode = ADS_SINGLE_MODE;
	ADS1115.os = ADS_START;
	ADS_init();
}

void ADS_startContinuous(void)
{
	ADS1115.mode = ADS_CONTINUOUS_MODE;
	ADS1115.os = ADS_START;
	ADS_init();
}

int16_t ADS_read(void)
{
	uint8_t ads_read_array[2];

	HAL_I2C_Master_Transmit(&hi2c1, ADS1115.address << 1, &ADS_REGISTERS[0], 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, ADS1115.address << 1, ads_read_array, 2, HAL_MAX_DELAY);

	return ((ads_read_array[0] << 8) | ads_read_array[1]);
}

bool ADS_busy(void)
{
	uint8_t ads_busy_array[2];

	HAL_I2C_Master_Transmit(&hi2c1, ADS1115.address << 1, &ADS_REGISTERS[1], 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, ADS1115.address << 1, ads_busy_array, 2, HAL_MAX_DELAY);

	if (ads_busy_array[0] & ADS_BUSY)
		return false;
	else
		return true;
}

float ADS_getLSB(void)
{
	float x;

	switch (ADS1115.fsr) {
	case ADS_FSR_6_144_V:
		x = 6.144*2;
		break;
	case ADS_FSR_4_096_V:
		x = 4.096*2;
		break;
	case ADS_FSR_1_024_V:
		x = 1.024*2;
		break;
	case ADS_FSR_0_512_V:
		x = 0.512*2;
		break;
	case ADS_FSR_0_256_V:
		x = 0.256*2;
		break;
	default:
		x = 2.048*2;
		break;
	}

	return (x / ADC_RESOLUTION);
}
