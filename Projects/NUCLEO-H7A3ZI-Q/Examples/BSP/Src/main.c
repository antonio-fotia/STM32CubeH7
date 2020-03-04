/**
  ******************************************************************************
  * @file    BSP/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a set of tests of:
  *			 - Leds and button
  *          - LCD, SD and joystick through Adafruit Shield
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stlogo.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  SHIELD_NOT_DETECTED = 0,
  SHIELD_DETECTED
}ShieldStatus;

BSP_DemoTypedef  BSP_examples[]=
{
  {LCD_demo, "LCD", 0},
  {Joystick_demo, "JOYSTICK", 0},
  {SD_demo, "SD", 0},
};

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t DemoIndex = 0;
__IO uint32_t ButtonState = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static ShieldStatus TFT_ShieldDetect(void);
static void Display_DemoDescription(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  static char dataOut[64];
  COM_InitTypeDef COM_Init;

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* Initialize User_Button on STM32H7xx-Nucleo ------------------*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);


  /* Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
     board. This is done by reading the state of IO PB.01 pin (mapped to
     JoyStick available on adafruit 1.8" TFT shield). If the state of PB.01
     is high then the adafruit 1.8" TFT shield is available. */
  if(TFT_ShieldDetect() == SHIELD_DETECTED)
  {
    /* Initialize the LCD */
    ADAFRUIT_802_LCD_Init(0, LCD_ORIENTATION_PORTRAIT);
    GUI_SetFuncDriver(&LCD_Driver);

    Display_DemoDescription();

    while (1)
    {
      if(BSP_PB_GetState(BUTTON_USER) != RESET)
      {
        HAL_Delay(10);
        while (BSP_PB_GetState(BUTTON_USER) != RESET);

        BSP_examples[DemoIndex++].DemoFunc();

        if(DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
        {
          DemoIndex = 0;
        }
        Display_DemoDescription();
      }
    }
  }
  else /* there is no AdaFruit shield 802 connected */
  {
    /* Initialize Led1 on STM32H7xx-Nucleo    ------------------*/
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* Initialize COM port on STM32H7xx-Nucleo ------------------*/
    COM_Init.BaudRate   = 115200;
    COM_Init.WordLength = COM_WORDLENGTH_8B;
    COM_Init.StopBits   = COM_STOPBITS_1;
    COM_Init.Parity     = COM_PARITY_NONE;
    COM_Init.HwFlowCtl  = COM_HWCONTROL_NONE;
    BSP_COM_Init(COM1, &COM_Init);

    sprintf(dataOut, "\r\n LED2 OFF\r\n Press Key button to put it ON\r\n ");
    printf("%s", dataOut);

    /* Wait for User button to be pressed ---------------------------------*/
      while(CheckForUserInput() == 0)
      {}
      /* Toggle LED2 */
      BSP_LED_Toggle(LED2);

    /* Infinite loop */
    while (1)
    {
      while(CheckForUserInput() == 0)
      {}
        /* LED1 and LED3 On */
        BSP_LED_On(LED1);
        BSP_LED_On(LED3);

        sprintf(dataOut, "\r\n LED1 and LED3 ON\r\n Press Key button to put them OFF\r\n ");
        printf("%s", dataOut);
        ButtonState = 0;

      while(CheckForUserInput() == 0)
      {}
        /* LED1 and LED3 Off */
        BSP_LED_Off(LED1);
        BSP_LED_Off(LED3);

        sprintf(dataOut, "\r\n LED1 and LED3 OFF\r\n Press Key button to put them ON\r\n ");
        printf("%s", dataOut);
        ButtonState = 0;
    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 280000000 (CPU Clock)
  *            HCLK(Hz)                       = 280000000 (Bus matrix and AHBs Clock)
  *            AHB Prescaler                  = 1
  *            CD APB3 Prescaler              = 2 (APB3 Clock  140MHz)
  *            CD APB1 Prescaler              = 2 (APB1 Clock  140MHz)
  *            CD APB2 Prescaler              = 2 (APB2 Clock  140MHz)
  *            SRD APB4 Prescaler             = 2 (APB4 Clock  140MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 280
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 6
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device is
  clocked below the maximum system frequency, to update the voltage scaling value
  regarding system frequency refer to product datasheet.
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 280;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
    RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/**
  * @brief  Display main demo messages.
  * @retval None
  */
static void Display_DemoDescription(void)
{
  uint8_t desc[100];
  uint32_t x_size;
  uint32_t y_size;

  ADAFRUIT_802_LCD_GetXSize(0, &x_size);
  ADAFRUIT_802_LCD_GetYSize(0, &y_size);

  GUI_SetFont(&Font8);

  /* Clear the LCD */
  GUI_SetBackColor(GUI_COLOR_WHITE);
  GUI_Clear(GUI_COLOR_WHITE);

  /* Set the LCD Text Color */
  GUI_SetTextColor(GUI_COLOR_BLUE);

  /* Display LCD messages */
  GUI_DisplayStringAt(0, 10, (uint8_t *)"STM32H7A3ZIQ BSP", CENTER_MODE);
  GUI_DisplayStringAt(0, 25, (uint8_t *)"Drivers examples", CENTER_MODE);

  /* Draw Bitmap */
  GUI_DrawBitmap((x_size - 80)/2, 35, (uint8_t *)stlogo);

  GUI_DisplayStringAt(0, y_size- 15, (uint8_t *)"Copyright (c) STM 2019", CENTER_MODE);

  GUI_SetTextColor(GUI_COLOR_BLUE);
  GUI_FillRect(0, y_size/2 + 15, x_size, 40, GUI_COLOR_BLUE);
  GUI_SetTextColor(GUI_COLOR_WHITE);
  GUI_SetBackColor(GUI_COLOR_BLUE);
  GUI_DisplayStringAt(0, y_size/2 + 25, (uint8_t *)"Use User Button to start", CENTER_MODE);
  sprintf((char *)desc,"%s example", BSP_examples[DemoIndex].DemoName);
  GUI_DisplayStringAt(0, y_size/2 + 40, (uint8_t *)desc, CENTER_MODE);
}


/**
  * @brief  Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
  *         board. This is done by reading the state of IO PB.01 pin (mapped to
  *         JoyStick available on adafruit 1.8" TFT shield). If the state of PB.01
  *         is high then the adafruit 1.8" TFT shield is available.
  * @retval SHIELD_DETECTED: 1.8" TFT shield is available
  *         SHIELD_NOT_DETECTED: 1.8" TFT shield is not available
  */
static ShieldStatus TFT_ShieldDetect(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable GPIO clock */
  ADAFRUIT_802_ADCx_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin  = ADAFRUIT_802_ADCx_GPIO_PIN ;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(ADAFRUIT_802_ADCx_GPIO_PORT , &GPIO_InitStruct);

  HAL_Delay(1);

  if(HAL_GPIO_ReadPin(ADAFRUIT_802_ADCx_GPIO_PORT , ADAFRUIT_802_ADCx_GPIO_PIN ) != 0)
  {
    return SHIELD_DETECTED;
  }
  else
  {
    return SHIELD_NOT_DETECTED;
  }
}

/**
  * @brief  Check for user input
  * @param  None
* @retval Input state (1 : active / 0 : Inactive)
  */
uint8_t CheckForUserInput(void)
{
  return ButtonState;
}

/**
  * @brief  Button Callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  if(Button == BUTTON_USER)
  {
    ButtonState = 1;
  }
}


/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
