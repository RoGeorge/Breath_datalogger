/*
 * TMP112.c
 *
 *  Created on: Jul 5, 2013
 *      Author: RoGeorge
 */

#include "TMP112.h"
#include "I2C_CHANNEL1.h"
/*
#include "LED1.h"
#include "LED2.h"
#include "LED3.h"
*/
 
/* External 3-axis accelerometer control register addresses */
#define TMP112_CTRL_REG_1 0x2A
/* TMP112 3-axis accelerometer control register bit masks */
#define TMP112_ACTIVE_BIT_MASK 0x01
#define TMP112_F_READ_BIT_MASK 0x02
 
/* External 3-axis accelerometer data register addresses */
#define TMP112_OUT_X_MSB 0x01
#define TMP112_OUT_X_LSB 0x02
#define TMP112_OUT_Y_MSB 0x03
#define TMP112_OUT_Y_LSB 0x04
#define TMP112_OUT_Z_MSB 0x05
#define TMP112_OUT_Z_LSB 0x06
 
static TMP112_TDataState deviceData;
 
uint8_t TMP112_ReadReg(uint8_t addr, uint8_t *data, short dataSize) {
  uint8_t res;
 
  /* Send I2C address plus register address to the I2C bus *without* a stop condition */
  res = I2C_CHANNEL1_MasterSendBlock(deviceData.handle, &addr, 1U, LDD_I2C_NO_SEND_STOP);
  if (res!=ERR_OK) {
    return ERR_FAILED;
  }
  while (!deviceData.dataTransmittedFlg) {} /* Wait until data is sent */
  deviceData.dataTransmittedFlg = FALSE;
 
  /* Receive InpData (1 byte) from the I2C bus and generates a stop condition to end transmission */
  res = I2C_CHANNEL1_MasterReceiveBlock(deviceData.handle, data, dataSize, LDD_I2C_SEND_STOP);
  if (res!=ERR_OK) {
    return ERR_FAILED;
  }
  while (!deviceData.dataReceivedFlg) {} /* Wait until data is received received */
  deviceData.dataReceivedFlg = FALSE;
  return ERR_OK;
}
 
uint8_t TMP112_WriteReg(uint8_t addr, uint8_t val) {
  uint8_t buf[2], res;
 
  buf[0] = addr;
  buf[1] = val;
  res = I2C_CHANNEL1_MasterSendBlock(deviceData.handle, &buf, 2U, LDD_I2C_SEND_STOP); /* Send OutData (3 bytes with address) on the I2C bus and generates not a stop condition to end transmission */
  if (res!=ERR_OK) {
    return ERR_FAILED;
  }
  while (!deviceData.dataTransmittedFlg) {}  /* Wait until date is sent */
  deviceData.dataTransmittedFlg = FALSE;
  return ERR_OK;
}



static int8_t xyz[3];
 
void TMP112_Run(void) {
  uint8_t res;
 
/*
  LED1_On();
  LED2_On();
  LED3_On();
*/
  deviceData.handle = I2C_CHANNEL1_Init(&deviceData);
  /* F_READ: Fast read mode, data format limited to single byte (auto increment counter will skip LSB)
  * ACTIVE: Full scale selection
  */
  res = TMP112_WriteReg(TMP112_CTRL_REG_1,  TMP112_F_READ_BIT_MASK|TMP112_ACTIVE_BIT_MASK);
  if (res==ERR_OK) {
    for(;;) {
      res = TMP112_ReadReg(TMP112_OUT_X_MSB, (uint8_t*)&xyz, 3);
/*
      LED1_Put(xyz[0]>50);
      LED2_Put(xyz[1]>50);
      LED3_Put(xyz[2]>50);
*/
    }
  }
  I2C_CHANNEL1_Deinit(deviceData.handle);
/*
  LED1_Off();
  LED2_Off();
  LED3_Off();
*/
}
