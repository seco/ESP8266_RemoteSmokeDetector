#include "espressif/esp_common.h"
#include "espressif/esp_system.h"
#include "gpio.h"
#include "i2c_master.h"
#include "uart.h"

// 7 bit address of the iAQ-Core I2C sensor
#define SMOKE_DETECTOR_ADDRESS ((0x5A << 1) | 0x01) // Address is 0x5A but with read bit its 0x5B.
#define SMOKE_DETECT_RESPONSE_LENGTH 9

typedef struct
{
	uint16 co2PPM; // Co2 concentration
	uint8 status;
	uint32 resistance;
	uint16 tvocPPB; // Total Volatile Organic Compound
}SmokeDetectorResponse;

uint32 ICACHE_FLASH_ATTR user_rf_cal_sector_set(void) {
	flash_size_map size_map = system_get_flash_size_map();
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

void setupUART1() {
	UART_ConfigTypeDef uart_config;
	uart_config.baud_rate = BIT_RATE_115200;
	uart_config.data_bits = UART_WordLength_8b;
	uart_config.parity = USART_Parity_None;
	uart_config.stop_bits = USART_StopBits_1;
	uart_config.flow_ctrl = USART_HardwareFlowControl_None;
	uart_config.UART_RxFlowThresh = 120;
	uart_config.UART_InverseMask = UART_None_Inverse;
	UART_ParamConfig(UART1, &uart_config);

	UART_SetPrintPort(UART1);
}

void taskConnect(void *pData)
{
	int count = 0;
	printf("Hello from taskConnect!\n");

	SmokeDetectorResponse lastMeasurement;
	while (1)
	{
		count++;
		printf("count - %d\n", count);

		// TODO: Send start to smoke sensor
//		os_printf("--- Examining ---\n");
//		uint8_t i;
//		for (i=1; i<127; i++)
//		{
//			i2c_master_start();
//			i2c_master_writeByte(i << 1);
//			if (i2c_master_checkAck())
//			{
//				os_printf("Found device at: 0x%2x\n", i);
//			}
//			i2c_master_stop();
//		}
//		os_printf("Done!\n");




		// Send address byte
		i2c_master_start();
		i2c_master_writeByte(SMOKE_DETECTOR_ADDRESS);

		GPIO_OUTPUT_SET(5, 1);
		GPIO_OUTPUT_SET(5, 0);
		GPIO_OUTPUT_SET(5, 1);

		// Did it respond?
		if (i2c_master_checkAck())
		{
			printf("Got ACK");

			GPIO_OUTPUT_SET(5, 1);
			GPIO_OUTPUT_SET(5, 0);
			GPIO_OUTPUT_SET(5, 1);

			uint8 receivedByte = i2c_master_readByte();
			printf("Got Byte: %d\r\n", receivedByte);


//			// Read bytes
//			int i;
//
//			// Parse response
//			for (i = 1; i < SMOKE_DETECT_RESPONSE_LENGTH; i++)
//			{
//				uint8 receivedByte = i2c_master_readByte();
//				//i2c_master_send_ack();
//				printf("Got Byte: %d\r\n", receivedByte);
//
//				((uint8 *) &lastMeasurement)[i] = receivedByte;
//			}
//
//			// Print parsed response
//			printf("co2PPM: %d \r\n", lastMeasurement.co2PPM);
//			printf("status: %d \r\n", lastMeasurement.status);
//			printf("resistance: %d \r\n", lastMeasurement.resistance);
//			printf("tvocPPB: %d \r\n", lastMeasurement.tvocPPB);
		}
		i2c_master_stop();

		// Repeat

		vTaskDelay(100);
//		if(count % 2)
//		{
//			GPIO_OUTPUT_SET(5, 1);
//			//I2C_MASTER_SDA_HIGH_SCL_LOW();
//		}
//		else
//		{
//			//I2C_MASTER_SDA_LOW_SCL_HIGH();
//			GPIO_OUTPUT_SET(5, 0);
//		}
	}
}

void user_init(void)
{
	//uart_init_new();
	//setupUART1();
	UART_SetBaudrate(UART0, 115200);
	printf("SB SDK version:%s\n", system_get_sdk_version());
	printf("Free heap size: %d\n", system_get_free_heap_size());
	printf("user_init running ...\n");
	xTaskCreate(taskConnect, "taskConnect", 256, NULL, 2, NULL);

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
	GPIO_OUTPUT_SET(5, 1);

	//I2C master Init
	i2c_master_gpio_init();

//	os_delay_us(5);
//	GPIO_OUTPUT_SET(5, 0);
//	os_delay_us(5);
//	GPIO_OUTPUT_SET(5, 1);
//	os_delay_us(5);
//	GPIO_OUTPUT_SET(5, 0);

}

