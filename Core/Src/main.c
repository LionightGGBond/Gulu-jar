/*
 *
 *
+ *  USER CODE BEGIN Header *
 *  /
/**20261012
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BLE.h"
#include "Conductivity sensor.h"
#include <stdio.h>
#include <string.h>
#include <AHT21_ENS160.h>
#include "ph_sensor.h"
#include "led.h"
#include "kk_ui.h"
#include "kk_ui_app.h"
#include "kk_oled.h"
#include "kk_oled_driver.h"
#include "ui_input.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t ble_rx_buf[128];
float current_ph_value = 0.0f;
// 声明外部变量
extern float T_message;
extern uint16_t E_message;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// 1. 蓝牙使用的空闲中断 + DMA 回调
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {


    //解析小程序下发的三色灯控制指令 ↓↓↓↓↓↓

    // 假设蓝牙接收的数据存放在全局数组 rx_data 中
        for (uint16_t i = 0; i < Size; i++)
        {
            // 确保剩余长度至少还有3个字节，并且匹配到了帧头(0x5A)和帧尾(0xA5)
            if ((i + 2 < Size) && (ble_rx_buf[i] == 0x5A) && (ble_rx_buf[i + 2] == 0xA5))
            {
                uint8_t cmd = ble_rx_buf[i + 1]; // 提取中间的指令字节

                if (cmd == 0x01) {
                    LED_SetStatus(SYS_STATUS_SAFE);    // 绿灯
                } else if (cmd == 0x02) {
                    LED_SetStatus(SYS_STATUS_WARNING); // 黄灯
                } else if (cmd == 0x03) {
                    LED_SetStatus(SYS_STATUS_DANGER);  // 红灯
                }

                // 成功解析一条指令后，游标向后跳过已处理的字节，提高效率
                i += 2;
            }
        }
        BLE_RxCallback(Size);
    }
}

// 2. 传感器使用的普通中断接收完成回调
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        // 只有当状态机处于等待接收状态时，才处理这批数据
        if (Cond_State == COND_STATE_BUSY_RX)
        {
            // 收到完整指令应答，状态推进，通知主循环开始解析
            Cond_State = COND_STATE_DATA_READY;
        }
    }
}

// 3. KK_OLED 的 I2C2 异步刷新回调：把 HAL 完成/错误事件转发给 OLED 驱动
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C2)
    {
        OLED_DriverHandleMemTxComplete();
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C2)
    {
        OLED_DriverHandleError();
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  BLE_Init();            // 蓝牙初始化 (DMA空闲中断接收)
  Conductivity_Init();   // 电导率传感器初始化
  EnvSensor_Init(&hi2c1);  //初始化 AHT21 和 ENS160 传感器
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  LED_SetStatus(SYS_STATUS_OFF); // 上电默认关闭所有指示灯

  // OLED + KK_UI 初始化（屏幕未接好时不影响传感器与蓝牙功能）
  UI_InputInit();
  if (OLED_Init() == OLED_OK)
  {
      KK_UI_AppReset(HAL_GetTick());
      (void)KK_UI_Init(KK_UI_AppGet());
  }

  char dbg_msg[60];
  sprintf(dbg_msg, "Gulu Altar System Starting...\r\n");
  BLE_SendData((uint8_t *)dbg_msg, strlen(dbg_msg));
  HAL_Delay(500);

  uint32_t last_tick = HAL_GetTick(); // 记录上一次的时间戳

  // 启动前先触发一次测量，让传感器开始准备第一帧数据
  Conductivity_StartMeasure_IT();

  //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    // 定义风扇运行状态标志位 (static 保证每次循环不丢失状态)
    static uint8_t fan_is_running = 0;

    while (1)
    {
    	  current_ph_value = PH_Get_Final_Value();
          // 1. 电导率状态机处理 (轮询)
          Conductivity_Process();

          // 2. OLED 菜单 UI：采样按键 + 运行 KK_UI（每轮调用，保证输入采样频率）
          {
              uint32_t ui_now = HAL_GetTick();
              KK_UI_Input ui_input = UI_InputRead();
              KK_UI_Status ui_status;

              KK_UI_AppUpdate(ui_now);
              ui_status = KK_UI_Update(ui_now, ui_input);
              KK_UI_AppProcessEvents();
              // I2C 传输失败或超时后允许重画重发，避免界面永久卡死
              if (ui_status == KK_UI_DISPLAY_ERROR ||
                  ui_status == KK_UI_DISPLAY_TIMEOUT)
              {
                  (void)KK_UI_RecoverDisplay();
              }
          }

          // 3. 定时 1000ms 执行一次数据综合上报与温度控制
          if (HAL_GetTick() - last_tick >= 1000)
          {
              last_tick = HAL_GetTick();

              // 读取传感器数据
              EnvSensor_Read_Data(&hi2c1, &EnvData);
              char message[100];

              // 判断电导率数据有效性，并按标准格式融合 AHT21 和 ENS160 数据
              if (Cond_State == COND_STATE_ERROR) {
                  // 电导率报错时的输出格式 (保留湿度，将 Con 标记为 Error)
                  sprintf(message, "Tem:%.2f,Hum:%.1f,Co2:%u,Con:Error\r\n",
                          EnvData.Temperature, EnvData.Humidity, EnvData.eCO2);
              } else {
                  // 正常状态下的标准输出格式
                  sprintf(message, "Tem:%.2f,Hum:%.1f,Co2:%u,Con:%u，pH:%.2f\r\n",
                          EnvData.Temperature, EnvData.Humidity, EnvData.eCO2, E_message,current_ph_value);
              }
              BLE_SendData((uint8_t *)message, strlen(message)); // 发送数据
              Conductivity_StartMeasure_IT(); // 触发下一次电导率测量

              // ==========================================
              // 4. 咕噜坛智能散热系统：风扇温度曲线控制
              // ==========================================
              float current_temp = EnvData.Temperature;
              uint32_t pwm_duty = 0;

              // 启动与停止判定 (带 2℃ 的迟滞，防止临界点风扇抽搐)
              if (!fan_is_running && current_temp > 26.0f) {
                  fan_is_running = 1; // 温度超过25度，起转
              }
              else if (fan_is_running && current_temp < 23.0f) {
                  fan_is_running = 0; // 温度降到23度以下，彻底停转
              }

              // 计算占空比 (假设 CubeMX 中 TIM2 的 ARR = 999，最大占空比为 1000)
              if (fan_is_running) {
                  if (current_temp >= 40.0f) {
                      pwm_duty = 1000; // 40度及以上，全速运转
                  } else {
                      // 线性映射：起步给予 20% (200) 的占空比防止电机异响，剩下的 800 动态分配给 26~40度 区间
                      pwm_duty = 200 + (uint32_t)(((current_temp - 26.0f) / (40.0f - 26.0f)) * 800.0f);
                  }
              } else {
                  pwm_duty = 0; // 停转
              }

              // 更新 TIM2 通道 1 的 PWM 占空比
              __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm_duty);
          }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

    if (huart->Instance == USART3)
    {

        __HAL_UART_CLEAR_OREFLAG(huart);
        __HAL_UART_CLEAR_FEFLAG(huart);


       // JW01_Init(huart);
    }

    else if (huart->Instance == USART2)
    {
        __HAL_UART_CLEAR_OREFLAG(huart);
        __HAL_UART_CLEAR_FEFLAG(huart);

        extern volatile Cond_State_t Cond_State;
        Cond_State = COND_STATE_IDLE;
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
