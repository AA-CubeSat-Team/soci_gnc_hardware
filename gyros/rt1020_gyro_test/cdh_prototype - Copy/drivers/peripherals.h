/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "fsl_clock.h"

#include "fsl_lpuart.h"
#include "fsl_lpuart_freertos.h"

#include "fsl_lpi2c.h"
#include "fsl_lpi2c_freertos.h"

#include "fsl_lpspi.h"
#include "fsl_lpspi_freertos.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

///***********************************************************************************************************************
// * Definitions
// **********************************************************************************************************************/
///* Definitions for BOARD_InitPeripherals functional group */
///* Definition of peripheral ID */
//#define LPUART1_PERIPHERAL LPUART1
///* Definition of the clock source frequency */
//#define LPUART1_CLOCK_SOURCE 80000000UL
///* Definition of peripheral ID */
//#define LPUART3_PERIPHERAL LPUART3
///* Definition of the clock source frequency */
//#define LPUART3_CLOCK_SOURCE 80000000UL
///* Definition of peripheral ID */
//#define LPUART4_PERIPHERAL LPUART4
///* Definition of the clock source frequency */
//#define LPUART4_CLOCK_SOURCE 80000000UL
///* BOARD_InitPeripherals defines for LPI2C2 */
///* Definition of peripheral ID */
//#define LPI2C2_PERIPHERAL LPI2C2
///* Definition of clock source */
//#define LPI2C2_CLOCK_FREQ 60000000UL
///* Transfer buffer size */
//#define LPI2C2_MASTER_BUFFER_SIZE 1
///* BOARD_InitPeripherals defines for LPI2C1 */
///* Definition of peripheral ID */
//#define LPI2C1_PERIPHERAL LPI2C1
///* Definition of clock source */
//#define LPI2C1_CLOCK_FREQ 60000000UL
///* Transfer buffer size */
//#define LPI2C1_MASTER_BUFFER_SIZE 1
///* BOARD_InitPeripherals defines for LPSPI1 */
///* Definition of peripheral ID */
//#define LPSPI1_PERIPHERAL LPSPI1
///* Definition of clock source */
//#define LPSPI1_CLOCK_FREQ 105600000UL
///* BOARD_InitPeripherals defines for LPI2C3 */
///* Definition of peripheral ID */
//#define LPI2C3_PERIPHERAL LPI2C3
///* Definition of clock source */
//#define LPI2C3_CLOCK_FREQ 60000000UL
///* Transfer buffer size */
//#define LPI2C3_MASTER_BUFFER_SIZE 1

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

extern lpuart_rtos_handle_t uart1_handle;
extern lpuart_rtos_handle_t uart3_handle;
extern lpuart_rtos_handle_t uart4_handle;

extern lpspi_rtos_handle_t spi_m_rwa1_handle;
extern lpspi_rtos_handle_t spi_m_rwa2_handle;
extern lpspi_rtos_handle_t spi_m_rwa3_handle;

extern lpi2c_rtos_handle_t i2c1_m_rtos_handle;
extern lpi2c_rtos_handle_t i2c2_m_rtos_handle;
extern lpi2c_rtos_handle_t i2c3_m_rtos_handle;


void SPI_transfer(lpspi_rtos_handle_t * handler, uint8_t * txBuffer, uint8_t * rxBuffer, size_t transferSize);

void I2C_send(lpi2c_rtos_handle_t * handle, uint16_t slave_address, uint8_t subaddress, uint8_t * tx_buffer, size_t tx_size);
void I2C_request(lpi2c_rtos_handle_t * handle, uint16_t slave_address, uint8_t subaddress, uint8_t * rx_buffer, size_t rx_size);


/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals(void);



#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */
