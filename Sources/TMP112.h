/*
 * TMP112.h
 *
 *  Created on: Jul 5, 2013
 *      Author: RoGeorge
 */

#ifndef TMP112_H_
#define TMP112_H_
 
#include "PE_Types.h"
#include "PE_LDD.h"
 
typedef struct {
  volatile bool dataReceivedFlg; /* set to TRUE by the interrupt if we have received data */
  volatile bool dataTransmittedFlg; /* set to TRUE by the interrupt if we have set data */
  LDD_TDeviceData *handle; /* pointer to the device handle */
} TMP112_TDataState;

/* \brief Run the demo application */
void TMP112_Run(void);

#endif /* TMP112_H_ */
