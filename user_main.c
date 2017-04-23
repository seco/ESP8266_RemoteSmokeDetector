#include "osapi.h"
#include "user_interface.h"
#include "pwm.h"
#include "uart.h"
#include "mem.h"
#include "espmissingincludes.h"
#include "gpio.h"

#include "i2c_master.h"

// 7 bit address of the iAQ-Core I2C sensor
#define SMOKE_DETECTOR_ADDRESS ((0x5A << 1) | 0x01) // Address is 0x5A but with read bit its 0x5B.
#define SMOKE_DETECT_RESPONSE_LENGTH 9

// For other i2c peripheral testing
#define ACCELEROMETER_ADDRESS (0x1F << 1)
#define ACCELEROMETER_WRITE_COMMAND (0x80)

typedef struct
{
	uint16 co2PPM; // Co2 concentration
	uint8 status;
	uint32 resistance;
	uint16 tvocPPB; // Total Volatile Organic Compound
}SmokeDetectorResponse;

static volatile os_timer_t application_timer;
int count = 0;

uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 8;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void application_function(void *pData)
{

	count++;
	os_printf("count - %d\n", count);

//	// Accelerometer stuff
//	i2c_master_start();
//	i2c_master_writeByte(ACCELEROMETER_ADDRESS);
//	i2c_master_checkAck();
//	i2c_master_writeByte(ACCELEROMETER_WRITE_COMMAND);
//	i2c_master_checkAck();
//	i2c_master_stop();
//	i2c_master_start();
//	int i = 0;
//	uint8 receivedByte = 0;
//	for(i = 0; i < 14; i++)
//	{
//		receivedByte = i2c_master_readByte();
//		os_printf("%d ", receivedByte);
//	}
//
//	i2c_master_stop();

//-----------------------------------------------------------

//// SB WORKS
//	// Send address byte
//	i2c_master_start();
//	i2c_master_writeByte(SMOKE_DETECTOR_ADDRESS);
//	i2c_master_checkAck();
//	uint8 receivedByte = i2c_master_readByte();
//	i2c_master_send_ack();
//	os_printf("%x", receivedByte);
//// SB END WORKS


	SmokeDetectorResponse lastMeasurement;

	i2c_master_start();
	i2c_master_writeByte(SMOKE_DETECTOR_ADDRESS);
	i2c_master_checkAck(); // TODO: Find out why this doesnt return true all the time

	// Read bytes
	int i;
	for (i = 0; i < SMOKE_DETECT_RESPONSE_LENGTH; i++)
	{
		uint8 receivedByte = i2c_master_readByte();
		if(i == SMOKE_DETECT_RESPONSE_LENGTH -1)
		{
			i2c_master_send_nack();
		}
		else
		{
			i2c_master_send_ack();
		}

		((uint8 *) &lastMeasurement)[i] = receivedByte;
	}

	i2c_master_stop();

	// Print parsed response
	os_printf("co2PPM: %d \r\n", lastMeasurement.co2PPM);
	os_printf("status: %d \r\n", lastMeasurement.status);
	os_printf("resistance: %d \r\n", lastMeasurement.resistance);
	os_printf("tvocPPB: %d \r\n", lastMeasurement.tvocPPB);

	// TODO: Check wifi connection status

	// TODO: Send email if CO2PPM > threshold

	////////////////////////////////////////////////////
	if(count % 2)
	{
		//GPIO_OUTPUT_SET(5, 1);
		//I2C_MASTER_SDA_HIGH_SCL_LOW();
	}
	else
	{
		//I2C_MASTER_SDA_LOW_SCL_HIGH();
		//GPIO_OUTPUT_SET(5, 0);
	}
}

void user_init(void)
{
	uart_init(115200, 115200);
	os_printf("SB SDK version:%s\n", system_get_sdk_version());
	os_printf("Free heap size: %d\n", system_get_free_heap_size());
	os_printf("user_init running ...\n");

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
	GPIO_OUTPUT_SET(5, 1);

	//I2C master Init
	i2c_master_gpio_init(13,4,100000);

	// Start timer that will call our application function repeatedly
	os_timer_setfn(&application_timer, (os_timer_func_t *)application_function, NULL);
	os_timer_arm(&application_timer, 1000, 1);
}

