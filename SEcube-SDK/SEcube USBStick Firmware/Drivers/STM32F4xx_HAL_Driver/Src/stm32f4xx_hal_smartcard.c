/**
  ******************************************************************************
  * @file    stm32f4xx_hal_smartcard.c
  * @author  MCD Application Team
  * @version V1.4.4
  * @date    22-January-2016
  * @brief   SMARTCARD HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the SMARTCARD peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral State and Errors functions
  *
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
      The SMARTCARD HAL driver can be used as follows:

    (#) Declare a SMARTCARD_HandleTypeDef handle structure.
    (#) Initialize the SMARTCARD low level resources by implementing the HAL_SMARTCARD_MspInit() API:
        (##) Enable the USARTx interface clock.
        (##) SMARTCARD pins configuration:
            (+++) Enable the clock for the SMARTCARD GPIOs.
            (+++) Configure these SMARTCARD pins as alternate function pull-up.
        (##) NVIC configuration if you need to use interrupt process (HAL_SMARTCARD_Transmit_IT()
             and HAL_SMARTCARD_Receive_IT() APIs):
            (+++) Configure the USARTx interrupt priority.
            (+++) Enable the NVIC USART IRQ handle.
        (##) DMA Configuration if you need to use DMA process (HAL_SMARTCARD_Transmit_DMA()
             and HAL_SMARTCARD_Receive_DMA() APIs):
            (+++) Declare a DMA handle structure for the Tx/Rx stream.
            (+++) Enable the DMAx interface clock.
            (+++) Configure the declared DMA handle structure with the required Tx/Rx parameters.
            (+++) Configure the DMA Tx/Rx Stream.
            (+++) Associate the initialized DMA handle to the SMARTCARD DMA Tx/Rx handle.
            (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on the DMA Tx/Rx Stream.

    (#) Program the Baud Rate, Word Length , Stop Bit, Parity, Hardware
        flow control and Mode(Receiver/Transmitter) in the SMARTCARD Init structure.

    (#) Initialize the SMARTCARD registers by calling the HAL_SMARTCARD_Init() API:
        (++) These APIs configure also the low level Hardware GPIO, CLOCK, CORTEX...etc)
             by calling the customized HAL_SMARTCARD_MspInit() API.
    [..]
    (@) The specific SMARTCARD interrupts (Transmission complete interrupt,
        RXNE interrupt and Error Interrupts) will be managed using the macros
        __HAL_SMARTCARD_ENABLE_IT() and __HAL_SMARTCARD_DISABLE_IT() inside the transmit and receive process.

    [..]
    Three operation modes are available within this driver :

    *** Polling mode IO operation ***
    =================================
    [..]
      (+) Send an amount of data in blocking mode using HAL_SMARTCARD_Transmit()
      (+) Receive an amount of data in blocking mode using HAL_SMARTCARD_Receive()

    *** Interrupt mode IO operation ***
    ===================================
    [..]
      (+) Send an amount of data in non blocking mode using HAL_SMARTCARD_Transmit_IT()
      (+) At transmission end of transfer HAL_SMARTCARD_TxCpltCallback is executed and user can
          add his own code by customization of function pointer HAL_SMARTCARD_TxCpltCallback
      (+) Receive an amount of data in non blocking mode using HAL_SMARTCARD_Receive_IT()
      (+) At reception end of transfer HAL_SMARTCARD_RxCpltCallback is executed and user can
          add his own code by customization of function pointer HAL_SMARTCARD_RxCpltCallback
      (+) In case of transfer Error, HAL_SMARTCARD_ErrorCallback() function is executed and user can
          add his own code by customization of function pointer HAL_SMARTCARD_ErrorCallback

    *** DMA mode IO operation ***
    ==============================
    [..]
      (+) Send an amount of data in non blocking mode (DMA) using HAL_SMARTCARD_Transmit_DMA()
      (+) At transmission end of transfer HAL_SMARTCARD_TxCpltCallback is executed and user can
          add his own code by customization of function pointer HAL_SMARTCARD_TxCpltCallback
      (+) Receive an amount of data in non blocking mode (DMA) using HAL_SMARTCARD_Receive_DMA()
      (+) At reception end of transfer HAL_SMARTCARD_RxCpltCallback is executed and user can
          add his own code by customization of function pointer HAL_SMARTCARD_RxCpltCallback
      (+) In case of transfer Error, HAL_SMARTCARD_ErrorCallback() function is executed and user can
          add his own code by customization of function pointer HAL_SMARTCARD_ErrorCallback

    *** SMARTCARD HAL driver macros list ***
    =============================================
    [..]
      Below the list of most used macros in SMARTCARD HAL driver.

      (+) __HAL_SMARTCARD_ENABLE: Enable the SMARTCARD peripheral
      (+) __HAL_SMARTCARD_DISABLE: Disable the SMARTCARD peripheral
      (+) __HAL_SMARTCARD_GET_FLAG : Check whether the specified SMARTCARD flag is set or not
      (+) __HAL_SMARTCARD_CLEAR_FLAG : Clear the specified SMARTCARD pending flag
      (+) __HAL_SMARTCARD_ENABLE_IT: Enable the specified SMARTCARD interrupt
      (+) __HAL_SMARTCARD_DISABLE_IT: Disable the specified SMARTCARD interrupt

    [..]
      (@) You can refer to the SMARTCARD HAL driver header file for more useful macros

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup SMARTCARD SMARTCARD
  * @brief HAL USART SMARTCARD module driver
  * @{
  */
#ifdef HAL_SMARTCARD_MODULE_ENABLED
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup SMARTCARD_Private_Constants
  * @{
  */
/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup SMARTCARD_Private_Functions
  * @{
  */
static void SMARTCARD_SetConfig (SMARTCARD_HandleTypeDef *hsc);
static HAL_StatusTypeDef SMARTCARD_Transmit_IT(SMARTCARD_HandleTypeDef *hsc);
static HAL_StatusTypeDef SMARTCARD_EndTransmit_IT(SMARTCARD_HandleTypeDef *hsmartcard);
static HAL_StatusTypeDef SMARTCARD_Receive_IT(SMARTCARD_HandleTypeDef *hsc);
static void SMARTCARD_DMATransmitCplt(DMA_HandleTypeDef *hdma);
static void SMARTCARD_DMAReceiveCplt(DMA_HandleTypeDef *hdma);
static void SMARTCARD_DMAError(DMA_HandleTypeDef *hdma);
static HAL_StatusTypeDef SMARTCARD_WaitOnFlagUntilTimeout(SMARTCARD_HandleTypeDef *hsc, uint32_t Flag, FlagStatus Status, uint32_t Timeout);
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @defgroup SMARTCARD_Exported_Functions SMARTCARD Exported Functions
  * @{
  */

/** @defgroup SMARTCARD_Exported_Functions_Group1 SmartCard Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
  ==============================================================================
              ##### Initialization and Configuration functions #####
  ==============================================================================
  [..]
  This subsection provides a set of functions allowing to initialize the USART
  in Smartcard mode.
  [..]
  The Smartcard interface is designed to support asynchronous protocol Smartcards as
  defined in the ISO 7816-3 standard.
  [..]
  The USART can provide a clock to the smartcard through the SCLK output.
  In smartcard mode, SCLK is not associated to the communication but is simply derived
  from the internal peripheral input clock through a 5-bit prescaler.
  [..]
  (+) For the Smartcard mode only these parameters can be configured:
      (++) Baud Rate
      (++) Word Length => Should be 9 bits (8 bits + parity)
      (++) Stop Bit
      (++) Parity: => Should be enabled

      (+++) +-------------------------------------------------------------+
      (+++) |   M bit |  PCE bit  |        SMARTCARD frame                |
      (+++) |---------------------|---------------------------------------|
      (+++) |    1    |    1      |    | SB | 8 bit data | PB | STB |     |
      (+++) +-------------------------------------------------------------+

      (++) USART polarity
      (++) USART phase
      (++) USART LastBit
      (++) Receiver/transmitter modes
      (++) Prescaler
      (++) GuardTime
      (++) NACKState: The Smartcard NACK state

     (+) Recommended SmartCard interface configuration to get the Answer to Reset from the Card:
        (++) Word Length = 9 Bits
        (++) 1.5 Stop Bit
        (++) Even parity
        (++) BaudRate = 12096 baud
        (++) Tx and Rx enabled
  [..]
  Please refer to the ISO 7816-3 specification for more details.

    -@- It is also possible to choose 0.5 stop bit for receiving but it is recommended
        to use 1.5 stop bits for both transmitting and receiving to avoid switching
        between the two configurations.
  [..]
    The HAL_SMARTCARD_Init() function follows the USART  SmartCard configuration
    procedure (details for the procedure are available in reference manual (RM0329)).

@endverbatim
  * @{
  */

/**
  * @brief Initializes the SmartCard mode according to the specified
  *         parameters in the SMARTCARD_InitTypeDef and create the associated handle .
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Init(SMARTCARD_HandleTypeDef *hsc)
{
  /* Check the SMARTCARD handle allocation */
  if(hsc == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_SMARTCARD_INSTANCE(hsc->Instance));
  assert_param(IS_SMARTCARD_NACK_STATE(hsc->Init.NACKState));

  if(hsc->gState == HAL_SMARTCARD_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    hsc->Lock = HAL_UNLOCKED;
    /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
    HAL_SMARTCARD_MspInit(hsc);
  }

  hsc->gState = HAL_SMARTCARD_STATE_BUSY;

  /* Set the Prescaler */
  MODIFY_REG(hsc->Instance->GTPR, USART_GTPR_PSC, hsc->Init.Prescaler);

  /* Set the Guard Time */
  MODIFY_REG(hsc->Instance->GTPR, USART_GTPR_GT, ((hsc->Init.GuardTime)<<8));

  /* Set the Smartcard Communication parameters */
  SMARTCARD_SetConfig(hsc);

  /* In SmartCard mode, the following bits must be kept cleared:
  - LINEN bit in the USART_CR2 register
  - HDSEL and IREN bits in the USART_CR3 register.*/
  hsc->Instance->CR2 &= ~USART_CR2_LINEN;
  hsc->Instance->CR3 &= ~(USART_CR3_IREN | USART_CR3_HDSEL);

  /* Enable the SMARTCARD Parity Error Interrupt */
  __HAL_SMARTCARD_ENABLE_IT(hsc, SMARTCARD_IT_PE);

  /* Enable the SMARTCARD Framing Error Interrupt */
  __HAL_SMARTCARD_ENABLE_IT(hsc, SMARTCARD_IT_ERR);

  /* Enable the Peripheral */
  __HAL_SMARTCARD_ENABLE(hsc);

  /* Configure the Smartcard NACK state */
  MODIFY_REG(hsc->Instance->CR3, USART_CR3_NACK, hsc->Init.NACKState);

  /* Enable the SC mode by setting the SCEN bit in the CR3 register */
  hsc->Instance->CR3 |= (USART_CR3_SCEN);

  /* Initialize the SMARTCARD state*/
  hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;
  hsc->gState= HAL_SMARTCARD_STATE_READY;
  hsc->RxState= HAL_SMARTCARD_STATE_READY;

  return HAL_OK;
}

/**
  * @brief DeInitializes the USART SmartCard peripheral
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_DeInit(SMARTCARD_HandleTypeDef *hsc)
{
  /* Check the SMARTCARD handle allocation */
  if(hsc == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_SMARTCARD_INSTANCE(hsc->Instance));

  hsc->gState = HAL_SMARTCARD_STATE_BUSY;

  /* Disable the Peripheral */
  __HAL_SMARTCARD_DISABLE(hsc);

  /* DeInit the low level hardware */
  HAL_SMARTCARD_MspDeInit(hsc);

  hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;
  hsc->gState = HAL_SMARTCARD_STATE_RESET;
  hsc->RxState = HAL_SMARTCARD_STATE_RESET;

  /* Release Lock */
  __HAL_UNLOCK(hsc);

  return HAL_OK;
}

/**
  * @brief SMARTCARD MSP Init
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
 __weak void HAL_SMARTCARD_MspInit(SMARTCARD_HandleTypeDef *hsc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SMARTCARD_MspInit could be implemented in the user file
   */
}

/**
  * @brief SMARTCARD MSP DeInit
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
 __weak void HAL_SMARTCARD_MspDeInit(SMARTCARD_HandleTypeDef *hsc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SMARTCARD_MspDeInit could be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup SMARTCARD_Exported_Functions_Group2 IO operation functions
  *  @brief   SMARTCARD Transmit and Receive functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    This subsection provides a set of functions allowing to manage the SMARTCARD data transfers.
    [..]
    Smartcard is a single wire half duplex communication protocol.
    The Smartcard interface is designed to support asynchronous protocol Smartcards as
    defined in the ISO 7816-3 standard. The USART should be configured as:
    (+) 8 bits plus parity: where M=1 and PCE=1 in the USART_CR1 register
    (+) 1.5 stop bits when transmitting and receiving: where STOP=11 in the USART_CR2 register.

    (#) There are two modes of transfer:
       (++) Blocking mode: The communication is performed in polling mode.
            The HAL status of all data processing is returned by the same function
            after finishing transfer.
       (++) Non Blocking mode: The communication is performed using Interrupts
           or DMA, These APIs return the HAL status.
           The end of the data processing will be indicated through the
           dedicated SMARTCARD IRQ when using Interrupt mode or the DMA IRQ when
           using DMA mode.
           The HAL_SMARTCARD_TxCpltCallback(), HAL_SMARTCARD_RxCpltCallback() user callbacks
           will be executed respectively at the end of the Transmit or Receive process
           The HAL_SMARTCARD_ErrorCallback() user callback will be executed when a communication error is detected

    (#) Blocking mode APIs are :
        (++) HAL_SMARTCARD_Transmit()
        (++) HAL_SMARTCARD_Receive()

    (#) Non Blocking mode APIs with Interrupt are :
        (++) HAL_SMARTCARD_Transmit_IT()
        (++) HAL_SMARTCARD_Receive_IT()
        (++) HAL_SMARTCARD_IRQHandler()

    (#) Non Blocking mode functions with DMA are :
        (++) HAL_SMARTCARD_Transmit_DMA()
        (++) HAL_SMARTCARD_Receive_DMA()

    (#) A set of Transfer Complete Callbacks are provided in non Blocking mode:
        (++) HAL_SMARTCARD_TxCpltCallback()
        (++) HAL_SMARTCARD_RxCpltCallback()
        (++) HAL_SMARTCARD_ErrorCallback()

@endverbatim
  * @{
  */

/**
  * @brief Send an amount of data in blocking mode
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @param pData: pointer to data buffer
  * @param Size: amount of data to be sent
  * @param Timeout: Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Transmit(SMARTCARD_HandleTypeDef *hsc, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  if(hsc->gState == HAL_SMARTCARD_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(hsc);

    hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;
    hsc->gState = HAL_SMARTCARD_STATE_BUSY_TX;

    hsc->TxXferSize = Size;
    hsc->TxXferCount = Size;
    while(hsc->TxXferCount > 0U)
    {
      hsc->TxXferCount--;
      if(SMARTCARD_WaitOnFlagUntilTimeout(hsc, SMARTCARD_FLAG_TXE, RESET, Timeout) != HAL_OK)
      {
        return HAL_TIMEOUT;
      }
      uint16_t *tmp = (uint16_t*) pData;
      hsc->Instance->DR = (*tmp & (uint16_t)0x01FFU);
      pData +=1U;
    }

    if(SMARTCARD_WaitOnFlagUntilTimeout(hsc, SMARTCARD_FLAG_TC, RESET, Timeout) != HAL_OK)
    {
      return HAL_TIMEOUT;
    }

	/* At end of Tx process, restore hsc->gState to Ready */
    hsc->gState = HAL_SMARTCARD_STATE_READY;

    /* Process Unlocked */
    __HAL_UNLOCK(hsc);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief Receive an amount of data in blocking mode
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @param pData: pointer to data buffer
  * @param Size: amount of data to be received
  * @param Timeout: Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Receive(SMARTCARD_HandleTypeDef *hsc, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  if(hsc->RxState == HAL_SMARTCARD_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(hsc);

    hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;
    hsc->RxState = HAL_SMARTCARD_STATE_BUSY_RX;

    hsc->RxXferSize = Size;
    hsc->RxXferCount = Size;

    /* Check the remain data to be received */
    while(hsc->RxXferCount > 0U)
    {
      hsc->RxXferCount--;
      if(SMARTCARD_WaitOnFlagUntilTimeout(hsc, SMARTCARD_FLAG_RXNE, RESET, Timeout) != HAL_OK)
      {
        return HAL_TIMEOUT;
      }
      uint16_t *tmp = (uint16_t*) pData;
      *tmp = (uint16_t)(hsc->Instance->DR & (uint16_t)0x00FFU);
      pData +=1U;
    }

	/* At end of Rx process, restore hsc->RxState to Ready */
    hsc->RxState = HAL_SMARTCARD_STATE_READY;

    /* Process Unlocked */
    __HAL_UNLOCK(hsc);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief Send an amount of data in non blocking mode
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @param pData: pointer to data buffer
  * @param Size: amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Transmit_IT(SMARTCARD_HandleTypeDef *hsc, uint8_t *pData, uint16_t Size)
{
  /* Check that a Tx process is not already ongoing */
  if(hsc->gState == HAL_SMARTCARD_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(hsc);

    hsc->pTxBuffPtr = pData;
    hsc->TxXferSize = Size;
    hsc->TxXferCount = Size;

    hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;
    hsc->gState = HAL_SMARTCARD_STATE_BUSY_TX;

    /* Process Unlocked */
    __HAL_UNLOCK(hsc);

    /* Enable the SMARTCARD Parity Error Interrupt */
    __HAL_SMARTCARD_ENABLE_IT(hsc, SMARTCARD_IT_PE);

    /* Disable the SMARTCARD Error Interrupt: (Frame error, noise error, overrun error) */
    __HAL_SMARTCARD_DISABLE_IT(hsc, SMARTCARD_IT_ERR);

    /* Enable the SMARTCARD Transmit data register empty Interrupt */
    __HAL_SMARTCARD_ENABLE_IT(hsc, SMARTCARD_IT_TXE);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief Receive an amount of data in non blocking mode
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @param pData: pointer to data buffer
  * @param Size: amount of data to be received
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Receive_IT(SMARTCARD_HandleTypeDef *hsc, uint8_t *pData, uint16_t Size)
{
  /* Check that a Rx process is not already ongoing */
  if(hsc->RxState == HAL_SMARTCARD_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(hsc);

    hsc->pRxBuffPtr = pData;
    hsc->RxXferSize = Size;
    hsc->RxXferCount = Size;

    hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;
    hsc->RxState = HAL_SMARTCARD_STATE_BUSY_RX;

    /* Process Unlocked */
    __HAL_UNLOCK(hsc);

    /* Enable the SMARTCARD Data Register not empty Interrupt */
    __HAL_SMARTCARD_ENABLE_IT(hsc, SMARTCARD_IT_RXNE);

    /* Enable the SMARTCARD Parity Error Interrupt */
    __HAL_SMARTCARD_ENABLE_IT(hsc, SMARTCARD_IT_PE);

    /* Enable the SMARTCARD Error Interrupt: (Frame error, noise error, overrun error) */
    __HAL_SMARTCARD_ENABLE_IT(hsc, SMARTCARD_IT_ERR);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief Send an amount of data in non blocking mode
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @param pData: pointer to data buffer
  * @param Size: amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Transmit_DMA(SMARTCARD_HandleTypeDef *hsc, uint8_t *pData, uint16_t Size)
{
  /* Check that a Tx process is not already ongoing */
  if(hsc->gState == HAL_SMARTCARD_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(hsc);

    hsc->pTxBuffPtr = pData;
    hsc->TxXferSize = Size;
    hsc->TxXferCount = Size;

    hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;
    hsc->gState = HAL_SMARTCARD_STATE_BUSY_TX;

    /* Set the SMARTCARD DMA transfer complete callback */
    hsc->hdmatx->XferCpltCallback = SMARTCARD_DMATransmitCplt;

    /* Set the DMA error callback */
    hsc->hdmatx->XferErrorCallback = SMARTCARD_DMAError;

    /* Enable the SMARTCARD transmit DMA Stream */
    uint32_t *tmp = (uint32_t*)&pData;
    HAL_DMA_Start_IT(hsc->hdmatx, *(uint32_t*)tmp, (uint32_t)&hsc->Instance->DR, Size);

     /* Clear the TC flag in the SR register by writing 0 to it */
    __HAL_SMARTCARD_CLEAR_FLAG(hsc, SMARTCARD_FLAG_TC);

    /* Enable the DMA transfer for transmit request by setting the DMAT bit
    in the SMARTCARD CR3 register */
    hsc->Instance->CR3 |= USART_CR3_DMAT;

    /* Process Unlocked */
    __HAL_UNLOCK(hsc);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief Receive an amount of data in non blocking mode
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @param pData: pointer to data buffer
  * @param Size: amount of data to be received
  * @note   When the SMARTCARD parity is enabled (PCE = 1) the data received contain the parity bit.s
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SMARTCARD_Receive_DMA(SMARTCARD_HandleTypeDef *hsc, uint8_t *pData, uint16_t Size)
{
  /* Check that a Rx process is not already ongoing */
  if(hsc->RxState == HAL_SMARTCARD_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(hsc);

    hsc->pRxBuffPtr = pData;
    hsc->RxXferSize = Size;

    hsc->ErrorCode = HAL_SMARTCARD_ERROR_NONE;
    hsc->RxState = HAL_SMARTCARD_STATE_BUSY_RX;

    /* Set the SMARTCARD DMA transfer complete callback */
    hsc->hdmarx->XferCpltCallback = SMARTCARD_DMAReceiveCplt;

    /* Set the DMA error callback */
    hsc->hdmarx->XferErrorCallback = SMARTCARD_DMAError;

    /* Enable the DMA Stream */
    uint32_t *tmp = (uint32_t*)&pData;
    HAL_DMA_Start_IT(hsc->hdmarx, (uint32_t)&hsc->Instance->DR, *(uint32_t*)tmp, Size);

    /* Enable the DMA transfer for the receiver request by setting the DMAR bit
    in the SMARTCARD CR3 register */
    hsc->Instance->CR3 |= USART_CR3_DMAR;

    /* Process Unlocked */
    __HAL_UNLOCK(hsc);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief This function handles SMARTCARD interrupt request.
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
void HAL_SMARTCARD_IRQHandler(SMARTCARD_HandleTypeDef *hsc)
{
  uint32_t tmp1 = 0U, tmp2 = 0U;

  tmp1 = hsc->Instance->SR;
  tmp2 = __HAL_SMARTCARD_GET_IT_SOURCE(hsc, SMARTCARD_IT_PE);

  /* SMARTCARD parity error interrupt occurred --------------------------------*/
  if(((tmp1 & SMARTCARD_FLAG_PE) != RESET) && (tmp2 != RESET))
  {
    __HAL_SMARTCARD_CLEAR_PEFLAG(hsc);
    hsc->ErrorCode |= HAL_SMARTCARD_ERROR_PE;
  }

  tmp2 = __HAL_SMARTCARD_GET_IT_SOURCE(hsc, SMARTCARD_IT_ERR);
  /* SMARTCARD frame error interrupt occurred ---------------------------------*/
  if(((tmp1 & SMARTCARD_FLAG_FE) != RESET) && (tmp2 != RESET))
  {
    __HAL_SMARTCARD_CLEAR_FEFLAG(hsc);
    hsc->ErrorCode |= HAL_SMARTCARD_ERROR_FE;
  }

  tmp2 = __HAL_SMARTCARD_GET_IT_SOURCE(hsc, SMARTCARD_IT_ERR);
  /* SMARTCARD noise error interrupt occurred ---------------------------------*/
  if(((tmp1 & SMARTCARD_FLAG_NE) != RESET) && (tmp2 != RESET))
  {
    __HAL_SMARTCARD_CLEAR_NEFLAG(hsc);
    hsc->ErrorCode |= HAL_SMARTCARD_ERROR_NE;
  }

  tmp2 = __HAL_SMARTCARD_GET_IT_SOURCE(hsc, SMARTCARD_IT_ERR);
  /* SMARTCARD Over-Run interrupt occurred ------------------------------------*/
  if(((tmp1 & SMARTCARD_FLAG_ORE) != RESET) && (tmp2 != RESET))
  {
    __HAL_SMARTCARD_CLEAR_OREFLAG(hsc);
    hsc->ErrorCode |= HAL_SMARTCARD_ERROR_ORE;
  }

  tmp2 = __HAL_SMARTCARD_GET_IT_SOURCE(hsc, SMARTCARD_IT_RXNE);
  /* SMARTCARD in mode Receiver ----------------------------------------------*/
  if(((tmp1 & SMARTCARD_FLAG_RXNE) != RESET) && (tmp2 != RESET))
  {
    SMARTCARD_Receive_IT(hsc);
  }

  tmp2 = __HAL_SMARTCARD_GET_IT_SOURCE(hsc, SMARTCARD_IT_TXE);
  /* SMARTCARD in mode Transmitter -------------------------------------------*/
  if(((tmp1 & SMARTCARD_FLAG_TXE) != RESET) && (tmp2 != RESET))
  {
    SMARTCARD_Transmit_IT(hsc);
  }

  tmp2 = __HAL_SMARTCARD_GET_IT_SOURCE(hsc, SMARTCARD_IT_TC);
  /* SMARTCARD in mode Transmitter (transmission end) ------------------------*/
  if(((tmp1 & SMARTCARD_FLAG_TC) != RESET) && (tmp2 != RESET))
  {
    SMARTCARD_EndTransmit_IT(hsc);
  }

  /* Call the Error call Back in case of Errors */
  if(hsc->ErrorCode != HAL_SMARTCARD_ERROR_NONE)
  {
    /* Set the SMARTCARD state ready to be able to start again the process */
    hsc->gState= HAL_SMARTCARD_STATE_READY;
    hsc->RxState= HAL_SMARTCARD_STATE_READY;
    HAL_SMARTCARD_ErrorCallback(hsc);
  }
}

/**
  * @brief Tx Transfer completed callbacks
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
 __weak void HAL_SMARTCARD_TxCpltCallback(SMARTCARD_HandleTypeDef *hsc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SMARTCARD_TxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief Rx Transfer completed callbacks
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
__weak void HAL_SMARTCARD_RxCpltCallback(SMARTCARD_HandleTypeDef *hsc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SMARTCARD_TxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief SMARTCARD error callbacks
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
 __weak void HAL_SMARTCARD_ErrorCallback(SMARTCARD_HandleTypeDef *hsc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SMARTCARD_ErrorCallback could be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup SMARTCARD_Exported_Functions_Group3 Peripheral State and Errors functions
  *  @brief   SMARTCARD State and Errors functions
  *
@verbatim
 ===============================================================================
                ##### Peripheral State and Errors functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the SmartCard.
     (+) HAL_SMARTCARD_GetState() API can be helpful to check in run-time the state of the SmartCard peripheral.
     (+) HAL_SMARTCARD_GetError() check in run-time errors that could be occurred during communication.
@endverbatim
  * @{
  */

/**
  * @brief return the SMARTCARD state
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval HAL state
  */
HAL_SMARTCARD_StateTypeDef HAL_SMARTCARD_GetState(const SMARTCARD_HandleTypeDef *hsc)
{
  uint32_t temp1= 0x00U, temp2 = 0x00U;
  temp1 = hsc->gState;
  temp2 = hsc->RxState;

  return (HAL_SMARTCARD_StateTypeDef)(temp1 | temp2);
}

/**
  * @brief  Return the SMARTCARD error code
  * @param  hsc : pointer to a SMARTCARD_HandleTypeDef structure that contains
  *              the configuration information for the specified SMARTCARD.
  * @retval SMARTCARD Error Code
  */
uint32_t HAL_SMARTCARD_GetError(const SMARTCARD_HandleTypeDef *hsc)
{
  return hsc->ErrorCode;
}

/**
  * @}
  */

/**
  * @brief DMA SMARTCARD transmit process complete callback
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void SMARTCARD_DMATransmitCplt(DMA_HandleTypeDef *hdma)
{
  SMARTCARD_HandleTypeDef* hsc = ( SMARTCARD_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

  hsc->TxXferCount = 0U;

  /* Disable the DMA transfer for transmit request by setting the DMAT bit
  in the USART CR3 register */
  hsc->Instance->CR3 &= (uint32_t)~((uint32_t)USART_CR3_DMAT);

  /* Enable the SMARTCARD Transmit Complete Interrupt */
  __HAL_SMARTCARD_ENABLE_IT(hsc, SMARTCARD_IT_TC);
}

/**
  * @brief DMA SMARTCARD receive process complete callback
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void SMARTCARD_DMAReceiveCplt(DMA_HandleTypeDef *hdma)
{
  SMARTCARD_HandleTypeDef* hsc = ( SMARTCARD_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

  hsc->RxXferCount = 0U;

  /* Disable the DMA transfer for the receiver request by setting the DMAR bit
  in the USART CR3 register */
  hsc->Instance->CR3 &= (uint32_t)~((uint32_t)USART_CR3_DMAR);

  /* At end of Rx process, restore hsc->RxState to Ready */
  hsc->RxState = HAL_SMARTCARD_STATE_READY;

  HAL_SMARTCARD_RxCpltCallback(hsc);
}

/**
  * @brief DMA SMARTCARD communication error callback
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void SMARTCARD_DMAError(DMA_HandleTypeDef *hdma)
{
  SMARTCARD_HandleTypeDef* hsc = ( SMARTCARD_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

  hsc->RxXferCount = 0U;
  hsc->TxXferCount = 0U;
  hsc->ErrorCode = HAL_SMARTCARD_ERROR_DMA;
  hsc->gState= HAL_SMARTCARD_STATE_READY;
  hsc->RxState= HAL_SMARTCARD_STATE_READY;

  HAL_SMARTCARD_ErrorCallback(hsc);
}

/**
  * @brief  This function handles SMARTCARD Communication Timeout.
  * @param  hsc: SMARTCARD handle
  * @param  Flag: specifies the SMARTCARD flag to check.
  * @param  Status: The new Flag status (SET or RESET).
  * @param  Timeout: Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef SMARTCARD_WaitOnFlagUntilTimeout(SMARTCARD_HandleTypeDef *hsc, uint32_t Flag, FlagStatus Status, uint32_t Timeout)
{
  uint32_t tickstart = 0U;

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait until flag is set */
  if(Status == RESET)
  {
    while(__HAL_SMARTCARD_GET_FLAG(hsc, Flag) == RESET)
    {
      /* Check for the Timeout */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0U)||((HAL_GetTick() - tickstart ) > Timeout))
        {
          /* Disable TXE and RXNE interrupts for the interrupt process */
          __HAL_SMARTCARD_DISABLE_IT(hsc, SMARTCARD_IT_TXE);
          __HAL_SMARTCARD_DISABLE_IT(hsc, SMARTCARD_IT_RXNE);

          hsc->gState= HAL_SMARTCARD_STATE_READY;
		  hsc->RxState= HAL_SMARTCARD_STATE_READY;

          /* Process Unlocked */
          __HAL_UNLOCK(hsc);

          return HAL_TIMEOUT;
        }
      }
    }
  }
  else
  {
    while(__HAL_SMARTCARD_GET_FLAG(hsc, Flag) != RESET)
    {
      /* Check for the Timeout */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0U)||((HAL_GetTick() - tickstart ) > Timeout))
        {
          /* Disable TXE and RXNE interrupts for the interrupt process */
          __HAL_SMARTCARD_DISABLE_IT(hsc, SMARTCARD_IT_TXE);
          __HAL_SMARTCARD_DISABLE_IT(hsc, SMARTCARD_IT_RXNE);

          hsc->gState= HAL_SMARTCARD_STATE_READY;
		  hsc->RxState= HAL_SMARTCARD_STATE_READY;

          /* Process Unlocked */
          __HAL_UNLOCK(hsc);

          return HAL_TIMEOUT;
        }
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief Send an amount of data in non blocking mode
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval HAL status
  */
static HAL_StatusTypeDef SMARTCARD_Transmit_IT(SMARTCARD_HandleTypeDef *hsc)
{
  /* Check that a Tx process is ongoing */
  if(hsc->gState == HAL_SMARTCARD_STATE_BUSY_TX)
  {
    uint16_t *tmp = (uint16_t*) hsc->pTxBuffPtr;
    hsc->Instance->DR = (uint16_t)(*tmp & (uint16_t)0x01FFU);
    hsc->pTxBuffPtr += 1U;

    if(--hsc->TxXferCount == 0U)
    {
      /* Disable the SMARTCARD Transmit data register empty Interrupt */
      __HAL_SMARTCARD_DISABLE_IT(hsc, SMARTCARD_IT_TXE);

      /* Enable the SMARTCARD Transmit Complete Interrupt */
      __HAL_SMARTCARD_ENABLE_IT(hsc, SMARTCARD_IT_TC);
    }

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Wraps up transmission in non blocking mode.
  * @param  hsmartcard: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for the specified SMARTCARD module.
  * @retval HAL status
  */
static HAL_StatusTypeDef SMARTCARD_EndTransmit_IT(SMARTCARD_HandleTypeDef *hsmartcard)
{
  /* Disable the SMARTCARD Transmit Complete Interrupt */
  __HAL_SMARTCARD_DISABLE_IT(hsmartcard, SMARTCARD_IT_TC);

  /* Disable the SMARTCARD Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_SMARTCARD_DISABLE_IT(hsmartcard, SMARTCARD_IT_ERR);

  /* Tx process is ended, restore hsmartcard->gState to Ready */
  hsmartcard->gState = HAL_SMARTCARD_STATE_READY;

  HAL_SMARTCARD_TxCpltCallback(hsmartcard);

  return HAL_OK;
}

/**
  * @brief Receive an amount of data in non blocking mode
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval HAL status
  */
static HAL_StatusTypeDef SMARTCARD_Receive_IT(SMARTCARD_HandleTypeDef *hsc)
{
  /* Check that a Rx process is ongoing */
  if(hsc->RxState == HAL_SMARTCARD_STATE_BUSY_RX)
  {
    uint16_t *tmp = (uint16_t*) hsc->pRxBuffPtr;
    *tmp = (uint16_t)(hsc->Instance->DR & (uint16_t)0x00FFU);
    hsc->pRxBuffPtr += 1U;

    if(--hsc->RxXferCount == 0U)
    {
      __HAL_SMARTCARD_DISABLE_IT(hsc, SMARTCARD_IT_RXNE);

      /* Disable the SMARTCARD Parity Error Interrupt */
      __HAL_SMARTCARD_DISABLE_IT(hsc, SMARTCARD_IT_PE);

      /* Disable the SMARTCARD Error Interrupt: (Frame error, noise error, overrun error) */
      __HAL_SMARTCARD_DISABLE_IT(hsc, SMARTCARD_IT_ERR);

	  /* Rx process is completed, restore hsc->RxState to Ready */
      hsc->RxState = HAL_SMARTCARD_STATE_READY;

      HAL_SMARTCARD_RxCpltCallback(hsc);

      return HAL_OK;
    }
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief Configure the SMARTCARD peripheral
  * @param  hsc: pointer to a SMARTCARD_HandleTypeDef structure that contains
  *                the configuration information for SMARTCARD module.
  * @retval None
  */
static void SMARTCARD_SetConfig(SMARTCARD_HandleTypeDef *hsc)
{
  uint32_t tmpreg = 0x00U;

  /* Check the parameters */
  assert_param(IS_SMARTCARD_INSTANCE(hsc->Instance));
  assert_param(IS_SMARTCARD_POLARITY(hsc->Init.CLKPolarity));
  assert_param(IS_SMARTCARD_PHASE(hsc->Init.CLKPhase));
  assert_param(IS_SMARTCARD_LASTBIT(hsc->Init.CLKLastBit));
  assert_param(IS_SMARTCARD_BAUDRATE(hsc->Init.BaudRate));
  assert_param(IS_SMARTCARD_WORD_LENGTH(hsc->Init.WordLength));
  assert_param(IS_SMARTCARD_STOPBITS(hsc->Init.StopBits));
  assert_param(IS_SMARTCARD_PARITY(hsc->Init.Parity));
  assert_param(IS_SMARTCARD_MODE(hsc->Init.Mode));
  assert_param(IS_SMARTCARD_NACK_STATE(hsc->Init.NACKState));

  /* The LBCL, CPOL and CPHA bits have to be selected when both the transmitter and the
     receiver are disabled (TE=RE=0) to ensure that the clock pulses function correctly. */
  hsc->Instance->CR1 &= (uint32_t)~((uint32_t)(USART_CR1_TE | USART_CR1_RE));

  /*---------------------------- USART CR2 Configuration ---------------------*/
  tmpreg = hsc->Instance->CR2;
  /* Clear CLKEN, CPOL, CPHA and LBCL bits */
  tmpreg &= (uint32_t)~((uint32_t)(USART_CR2_CPHA | USART_CR2_CPOL | USART_CR2_CLKEN | USART_CR2_LBCL));
  /* Configure the SMARTCARD Clock, CPOL, CPHA and LastBit -----------------------*/
  /* Set CPOL bit according to hsc->Init.CLKPolarity value */
  /* Set CPHA bit according to hsc->Init.CLKPhase value */
  /* Set LBCL bit according to hsc->Init.CLKLastBit value */
  /* Set Stop Bits: Set STOP[13:12] bits according to hsc->Init.StopBits value */
  tmpreg |= (uint32_t)(USART_CR2_CLKEN | hsc->Init.CLKPolarity |
                      hsc->Init.CLKPhase| hsc->Init.CLKLastBit | hsc->Init.StopBits);
  /* Write to USART CR2 */
  hsc->Instance->CR2 = (uint32_t)tmpreg;

  tmpreg = hsc->Instance->CR2;

  /* Clear STOP[13:12] bits */
  tmpreg &= (uint32_t)~((uint32_t)USART_CR2_STOP);

  /* Set Stop Bits: Set STOP[13:12] bits according to hsc->Init.StopBits value */
  tmpreg |= (uint32_t)(hsc->Init.StopBits);

  /* Write to USART CR2 */
  hsc->Instance->CR2 = (uint32_t)tmpreg;

  /*-------------------------- USART CR1 Configuration -----------------------*/
  tmpreg = hsc->Instance->CR1;

  /* Clear M, PCE, PS, TE and RE bits */
  tmpreg &= (uint32_t)~((uint32_t)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | \
                                   USART_CR1_RE));

  /* Configure the SMARTCARD Word Length, Parity and mode:
     Set the M bits according to hsc->Init.WordLength value
     Set PCE and PS bits according to hsc->Init.Parity value
     Set TE and RE bits according to hsc->Init.Mode value */
  tmpreg |= (uint32_t)hsc->Init.WordLength | hsc->Init.Parity | hsc->Init.Mode;

  /* Write to USART CR1 */
  hsc->Instance->CR1 = (uint32_t)tmpreg;

  /*-------------------------- USART CR3 Configuration -----------------------*/
  /* Clear CTSE and RTSE bits */
  hsc->Instance->CR3 &= (uint32_t)~((uint32_t)(USART_CR3_RTSE | USART_CR3_CTSE));

  /*-------------------------- USART BRR Configuration -----------------------*/
  if((hsc->Instance == USART1) || (hsc->Instance == USART6))
  {
    hsc->Instance->BRR = SMARTCARD_BRR(HAL_RCC_GetPCLK2Freq(), hsc->Init.BaudRate);
  }
  else
  {
    hsc->Instance->BRR = SMARTCARD_BRR(HAL_RCC_GetPCLK1Freq(), hsc->Init.BaudRate);
  }
}

/**
  * @}
  */

#endif /* HAL_SMARTCARD_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
