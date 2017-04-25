///******************************************************************************
// * Copyright 2013-2014 Espressif Systems (Wuxi)
// *
// * FileName: i2c_master.c
// *
// * Description: i2c master API
// *
// * Modification history:
// *     2014/3/12, v1.0 create this file.
//*******************************************************************************/
//#include "c_types.h"
//#include "esp8266/ets_sys.h"
//#include "gpio.h"
//
//#include "i2c_master.h"
//
//LOCAL uint8 m_nLastSDA;
//LOCAL uint8 m_nLastSCL;
//
///******************************************************************************
// * FunctionName : i2c_master_setDC
// * Description  : Internal used function -
// *                    set i2c SDA and SCL bit value for half clk cycle
// * Parameters   : uint8 SDA
// *                uint8 SCL
// * Returns      : NONE
//*******************************************************************************/
//LOCAL void ICACHE_FLASH_ATTR
//i2c_master_setDC(uint8 SDA, uint8 SCL)
//{
//    SDA	&= 0x01;
//    SCL	&= 0x01;
//    m_nLastSDA = SDA;
//    m_nLastSCL = SCL;
//    ETS_INTR_LOCK();
//    if ((0 == SDA) && (0 == SCL))
//    {
//        I2C_MASTER_SDA_LOW_SCL_LOW();
//    } else if ((0 == SDA) && (1 == SCL))
//    {
//        I2C_MASTER_SDA_LOW_SCL_HIGH();
//    } else if ((1 == SDA) && (0 == SCL))
//    {
//        I2C_MASTER_SDA_HIGH_SCL_LOW();
//    } else
//    {
//        I2C_MASTER_SDA_HIGH_SCL_HIGH();
//    }
//    ETS_INTR_UNLOCK();
//}
//
///******************************************************************************
// * FunctionName : i2c_master_getDC
// * Description  : Internal used function -
// *                    get i2c SDA bit value
// * Parameters   : NONE
// * Returns      : uint8 - SDA bit value
//*******************************************************************************/
//LOCAL uint8 ICACHE_FLASH_ATTR
//i2c_master_getDC(void)
//{
//    uint8 sda_out;
//    ETS_INTR_LOCK();
//    sda_out = GPIO_INPUT_GET(GPIO_ID_PIN(I2C_MASTER_SDA_GPIO));
//    ETS_INTR_UNLOCK();
//    return sda_out;
//}
//
///******************************************************************************
// * FunctionName : i2c_master_init
// * Description  : initilize I2C bus to enable i2c operations
// * Parameters   : NONE
// * Returns      : NONE
//*******************************************************************************/
//void ICACHE_FLASH_ATTR
//i2c_master_init(void)
//{
//    uint8 i;
//
//    i2c_master_setDC(1, 0);
//    i2c_master_wait(5);
//
//    // when SCL = 0, toggle SDA to clear up
//    i2c_master_setDC(0, 0) ;
//    i2c_master_wait(5);
//    i2c_master_setDC(1, 0) ;
//    i2c_master_wait(5);
//
//    // set data_cnt to max value
//    for (i = 0; i < 28; i++) {
//        i2c_master_setDC(1, 0);
//        i2c_master_wait(5);	// sda 1, scl 0
//        i2c_master_setDC(1, 1);
//        i2c_master_wait(5);	// sda 1, scl 1
//    }
//
//    // reset all
//    i2c_master_stop();
//    return;
//}
//
///******************************************************************************
// * FunctionName : i2c_master_gpio_init
// * Description  : config SDA and SCL gpio to open-drain output mode,
// *                mux and gpio num defined in i2c_master.h
// * Parameters   : NONE
// * Returns      : NONE
//*******************************************************************************/
//void ICACHE_FLASH_ATTR
//i2c_master_gpio_init(void)
//{
////    ETS_GPIO_INTR_DISABLE() ;
//    ETS_INTR_LOCK();
//
//    PIN_FUNC_SELECT(I2C_MASTER_SDA_MUX, I2C_MASTER_SDA_FUNC);
//    PIN_FUNC_SELECT(I2C_MASTER_SCL_MUX, I2C_MASTER_SCL_FUNC);
//
//    GPIO_REG_WRITE(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_MASTER_SDA_GPIO)), GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_MASTER_SDA_GPIO))) | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)); //open drain;
//    GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << I2C_MASTER_SDA_GPIO));
//    GPIO_REG_WRITE(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_MASTER_SCL_GPIO)), GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_MASTER_SCL_GPIO))) | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)); //open drain;
//    GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << I2C_MASTER_SCL_GPIO));
//
//    I2C_MASTER_SDA_HIGH_SCL_HIGH();
//
////    ETS_GPIO_INTR_ENABLE() ;
//    ETS_INTR_UNLOCK();
//
//    i2c_master_init();
//}
//
///******************************************************************************
// * FunctionName : i2c_master_start
// * Description  : set i2c to send state
// * Parameters   : NONE
// * Returns      : NONE
//*******************************************************************************/
//void ICACHE_FLASH_ATTR
//i2c_master_start(void)
//{
//    i2c_master_setDC(1, m_nLastSCL);
//    i2c_master_wait(5);
//    i2c_master_setDC(1, 1);
//    i2c_master_wait(5);	// sda 1, scl 1
//    i2c_master_setDC(0, 1);
//    i2c_master_wait(5);	// sda 0, scl 1
//}
//
///******************************************************************************
// * FunctionName : i2c_master_stop
// * Description  : set i2c to stop sending state
// * Parameters   : NONE
// * Returns      : NONE
//*******************************************************************************/
//void ICACHE_FLASH_ATTR
//i2c_master_stop(void)
//{
//    i2c_master_wait(5);
//
//    i2c_master_setDC(0, m_nLastSCL);
//    i2c_master_wait(5);	// sda 0
//    i2c_master_setDC(0, 1);
//    i2c_master_wait(5);	// sda 0, scl 1
//    i2c_master_setDC(1, 1);
//    i2c_master_wait(5);	// sda 1, scl 1
//}
//
///******************************************************************************
// * FunctionName : i2c_master_setAck
// * Description  : set ack to i2c bus as level value
// * Parameters   : uint8 level - 0 or 1
// * Returns      : NONE
//*******************************************************************************/
//void ICACHE_FLASH_ATTR
//i2c_master_setAck(uint8 level)
//{
//    i2c_master_setDC(m_nLastSDA, 0);
//    i2c_master_wait(5);
//    i2c_master_setDC(level, 0);
//    i2c_master_wait(5);	// sda level, scl 0
//    i2c_master_setDC(level, 1);
//    i2c_master_wait(8);	// sda level, scl 1
//    i2c_master_setDC(level, 0);
//    i2c_master_wait(5);	// sda level, scl 0
//    i2c_master_setDC(1, 0);
//    i2c_master_wait(5);
//}
//
///******************************************************************************
// * FunctionName : i2c_master_getAck
// * Description  : confirm if peer send ack
// * Parameters   : NONE
// * Returns      : uint8 - ack value, 0 or 1
//*******************************************************************************/
//uint8 ICACHE_FLASH_ATTR
//i2c_master_getAck(void)
//{
//    uint8 retVal;
//    i2c_master_setDC(m_nLastSDA, 0);
//    i2c_master_wait(5);
//    i2c_master_setDC(1, 0);
//    i2c_master_wait(5);
//    i2c_master_setDC(1, 1);
//    i2c_master_wait(5);
//
//    retVal = i2c_master_getDC();
//    i2c_master_wait(5);
//    i2c_master_setDC(1, 0);
//    i2c_master_wait(5);
//
//    return retVal;
//}
//
///******************************************************************************
//* FunctionName : i2c_master_checkAck
//* Description  : get dev response
//* Parameters   : NONE
//* Returns      : true : get ack ; false : get nack
//*******************************************************************************/
//bool ICACHE_FLASH_ATTR
//i2c_master_checkAck(void)
//{
//    if(i2c_master_getAck()){
//        return FALSE;
//    }else{
//        return TRUE;
//    }
//}
//
///******************************************************************************
//* FunctionName : i2c_master_send_ack
//* Description  : response ack
//* Parameters   : NONE
//* Returns      : NONE
//*******************************************************************************/
//void ICACHE_FLASH_ATTR
//i2c_master_send_ack(void)
//{
//    i2c_master_setAck(0x0);
//}
///******************************************************************************
//* FunctionName : i2c_master_send_nack
//* Description  : response nack
//* Parameters   : NONE
//* Returns      : NONE
//*******************************************************************************/
//void ICACHE_FLASH_ATTR
//i2c_master_send_nack(void)
//{
//    i2c_master_setAck(0x1);
//}
//
///******************************************************************************
// * FunctionName : i2c_master_readByte
// * Description  : read Byte from i2c bus
// * Parameters   : NONE
// * Returns      : uint8 - readed value
//*******************************************************************************/
//uint8 ICACHE_FLASH_ATTR
//i2c_master_readByte(void)
//{
//    uint8 retVal = 0;
//    uint8 k, i;
//
//    i2c_master_wait(5);
//    i2c_master_setDC(m_nLastSDA, 0);
//    i2c_master_wait(5);	// sda 1, scl 0
//
//    for (i = 0; i < 8; i++) {
//        i2c_master_wait(5);
//        i2c_master_setDC(1, 0);
//        i2c_master_wait(5);	// sda 1, scl 0
//        i2c_master_setDC(1, 1);
//        i2c_master_wait(5);	// sda 1, scl 1
//
//        k = i2c_master_getDC();
//        i2c_master_wait(5);
//
//        if (i == 7) {
//            i2c_master_wait(3);   ////
//        }
//
//        k <<= (7 - i);
//        retVal |= k;
//    }
//
//    i2c_master_setDC(1, 0);
//    i2c_master_wait(5);	// sda 1, scl 0
//
//    return retVal;
//}
//
///******************************************************************************
// * FunctionName : i2c_master_writeByte
// * Description  : write wrdata value(one byte) into i2c
// * Parameters   : uint8 wrdata - write value
// * Returns      : NONE
//*******************************************************************************/
//void ICACHE_FLASH_ATTR
//i2c_master_writeByte(uint8 wrdata)  // Delays are 10us here???
//{
//    uint8 dat;
//    sint8 i;
//
//    i2c_master_wait(5);
//
//    i2c_master_setDC(m_nLastSDA, 0);
//    i2c_master_wait(5);
//
//    for (i = 7; i >= 0; i--)
//    {
//        dat = wrdata >> i;
//        i2c_master_setDC(dat, 0);
//        i2c_master_wait(5);
//        i2c_master_setDC(dat, 1);
//        i2c_master_wait(5);
//
//        if (i == 0) {
//            i2c_master_wait(3);   //// SB: I think this is to allow
//        }
//
//        i2c_master_setDC(dat, 0);
//        i2c_master_wait(5);
//    }
//}

#include "c_types.h"
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "i2c_master.h"

/**
 * Set SDA to state
 */
LOCAL void ICACHE_FLASH_ATTR i2c_sda(uint8 state)
{
    state &= 0x01;
    //Set SDA line to state
    if (state)
        gpio_output_set(1 << I2C_SDA_PIN, 0, 1 << I2C_SDA_PIN, 0);
    else
        gpio_output_set(0, 1 << I2C_SDA_PIN, 1 << I2C_SDA_PIN, 0);
}

/**
 * Set SCK to state
 */
LOCAL void ICACHE_FLASH_ATTR
i2c_sck(uint8 state)
{
    //Set SCK line to state
    if (state)
        gpio_output_set(1 << I2C_SCK_PIN, 0, 1 << I2C_SCK_PIN, 0);
    else
        gpio_output_set(0, 1 << I2C_SCK_PIN, 1 << I2C_SCK_PIN, 0);
}

/**
 * I2C init function
 * This sets up the GPIO io
 */
void ICACHE_FLASH_ATTR
i2c_init(void)
{
    //Disable interrupts
	ETS_GPIO_INTR_DISABLE();

    //Set pin functions
    PIN_FUNC_SELECT(I2C_SDA_MUX, I2C_SDA_FUNC);
    PIN_FUNC_SELECT(I2C_SCK_MUX, I2C_SCK_FUNC);

    //Set SDA as open drain
    GPIO_REG_WRITE(
        GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SDA_PIN)),
        GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SDA_PIN))) |
        GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)
    );

    GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << I2C_SDA_PIN));

    //Set SCK as open drain
    GPIO_REG_WRITE(
        GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SCK_PIN)),
        GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SCK_PIN))) |
        GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)
    );

    GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << I2C_SCK_PIN));

    //Turn interrupt back on
    ETS_GPIO_INTR_ENABLE();

    i2c_sda(1);
    i2c_sck(1);
    return;
}

/**
 * I2C Start signal
 */
void ICACHE_FLASH_ATTR
i2c_start(void)
{
    i2c_sda(1);
    i2c_sck(1);
    os_delay_us(I2C_SLEEP_TIME);
    i2c_sda(0);
    os_delay_us(I2C_SLEEP_TIME);
    i2c_sck(0);
    os_delay_us(I2C_SLEEP_TIME);
}

/**
 * I2C Stop signal
 */
void ICACHE_FLASH_ATTR
i2c_stop(void)
{
    os_delay_us(I2C_SLEEP_TIME);
    i2c_sck(1);
    os_delay_us(I2C_SLEEP_TIME);
    i2c_sda(1);
    os_delay_us(I2C_SLEEP_TIME);
}

/**
 * Send I2C ACK to the bus
 * uint8 state 1 or 0
 *  1 for ACK
 *  0 for NACK
 */
void ICACHE_FLASH_ATTR
i2c_send_ack(uint8 state)
{
    i2c_sck(0);
    os_delay_us(I2C_SLEEP_TIME);
    //Set SDA
    //  HIGH for NACK
    //  LOW  for ACK
    i2c_sda((state?0:1));

    //Pulse the SCK
    i2c_sck(0);
    os_delay_us(I2C_SLEEP_TIME);
    i2c_sck(1);
    os_delay_us(I2C_SLEEP_TIME);
    i2c_sck(0);
    os_delay_us(I2C_SLEEP_TIME);

    i2c_sda(1);
    os_delay_us(I2C_SLEEP_TIME);
}

/**
 * Receive I2C ACK from the bus
 * returns 1 or 0
 *  1 for ACK
 *  0 for NACK
 */
uint8 ICACHE_FLASH_ATTR
i2c_check_ack(void)
{
    uint8 ack;
    i2c_sda(1);
    os_delay_us(I2C_SLEEP_TIME);
    i2c_sck(0);
    os_delay_us(I2C_SLEEP_TIME);
    i2c_sck(1);
    os_delay_us(I2C_SLEEP_TIME);

    //Get SDA pin status
    ack = i2c_read();

    os_delay_us(I2C_SLEEP_TIME);
    i2c_sck(0);
    os_delay_us(I2C_SLEEP_TIME);
    i2c_sda(0);
    os_delay_us(I2C_SLEEP_TIME);

    return (ack?0:1);
}

/**
 * Receive byte from the I2C bus
 * returns the byte
 */
uint8 ICACHE_FLASH_ATTR
i2c_readByte(void)
{
//    uint8 data = 0;
//    uint8 data_bit;
//    uint8 i;
//
//    i2c_sda(1);
//
//    for (i = 0; i < 8; i++)
//    {
//        os_delay_us(I2C_SLEEP_TIME);
//        i2c_sck(0);
//        os_delay_us(I2C_SLEEP_TIME);
//
//        i2c_sck(1);
//        os_delay_us(I2C_SLEEP_TIME);
//
//        data_bit = i2c_read();
//        os_delay_us(I2C_SLEEP_TIME);
//
//        data_bit <<= (7 - i);
//        data |= data_bit;
//    }
//    i2c_sck(0);
//    os_delay_us(I2C_SLEEP_TIME);
//
//    return data;


	//////////!!!!!!!!!!!!!!!!!!!!!!!!!! READ BYTE IS BROKEN. Try to understand why clocking bits dont work...
	//// I think this might be due to the task being rescheduled?
	// TODO: Clock out SCL lines until pre-empted? See if we can get something consistent?
    uint8 data = 0;
    uint8 data_bit;
    uint8 i;

    i2c_sda(0);

    for (i = 0; i < 8; i++)
    {
        i2c_sck(1);
        os_delay_us(I2C_SLEEP_TIME/2);

        data_bit = i2c_read();

        os_delay_us(I2C_SLEEP_TIME/2);
        i2c_sck(0);

        os_delay_us(I2C_SLEEP_TIME);

        data_bit <<= (7 - i);
        data |= data_bit;
    }

    i2c_sck(0);
    os_delay_us(I2C_SLEEP_TIME);

    // Check ACK on 9th bit
    i2c_sck(1);
    os_delay_us(I2C_SLEEP_TIME / 2);
    uint8 ack = i2c_read(); // Sample SDA state in the 'middle' of the clock pulse
    os_delay_us(I2C_SLEEP_TIME / 2);
    i2c_sck(0); // Stop clocking
    i2c_sda(1); // Leave SDA high for next read/write operation

    return data;
}

/**
 * Write byte to I2C bus
 * uint8 data: to byte to be writen
 */
bool ICACHE_FLASH_ATTR
i2c_writeByte(uint8 data)
{
    uint8 data_bit;
    sint8 i;

    os_delay_us(I2C_SLEEP_TIME);

    for (i = 7; i >= 0; i--)
    {
        data_bit = data >> i;
        i2c_sda(data_bit);
        os_delay_us(I2C_SLEEP_TIME);
        i2c_sck(1);
        os_delay_us(I2C_SLEEP_TIME);
        i2c_sck(0);
    }

    // Check ACK on 9th bit
    os_delay_us(I2C_SLEEP_TIME);
    i2c_sck(1);
    os_delay_us(I2C_SLEEP_TIME / 2);
    uint8 ack = i2c_read(); // Sample SDA state in the 'middle' of the clock pulse
    os_delay_us(I2C_SLEEP_TIME / 2);
    i2c_sck(0); // Stop clocking
    i2c_sda(1); // Leave SDA high for next read/write operation

    return (ack?0:1);
}


void ICACHE_FLASH_ATTR
i2c_clockSCK(void)
{
	int i = 0;
	for(i = 0; i < 9; i++)
	{
		os_delay_us(I2C_SLEEP_TIME);
		i2c_sck(1);
		os_delay_us(I2C_SLEEP_TIME);
		i2c_sck(0);
	}
}
