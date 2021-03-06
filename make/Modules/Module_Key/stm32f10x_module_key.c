#include "stm32f10x_driver_eeprom.h"
#include "stm32f10x_module_key.h"
#include "stm32f10x_module_nrf24l01.h"
#include "stm32f10x_algorithm_control.h"

extern int  Pitch_Offest;
extern int  Roll_Offest;

/********************************************
              Key初始化函数
功能：
1.配置Key接口IO输入方向

描述：
Key接口：
Key1-->PB2
Key2-->PB1
Key3-->PB3
********************************************/
void KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    /* config the extiline(PB1,PB3,PA8) clock and AFIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);

    /* config the NVIC(Mode-->PB1) */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* config the NVIC(“＋”-->PB3) */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* config the NVIC(“－”-->PA8) */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure Mode-->PB1 as output push-pull */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure “＋”-->PB3 as output push-pull */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure “－”-->PA8 as output push-pull */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    //Mode-->PB1
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;            //设定外部中断1
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  //设定中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //设定下降沿触发模式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    //“＋”-->PB3
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;            //设定外部中断1
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  //设定中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //设定下降沿触发模式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    //“－”-->PA8
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);
    EXTI_InitStructure.EXTI_Line = EXTI_Line8;              //设定外部中断1
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //设定中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //设定下降沿触发模式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

extern char IMUcalibratflag;

void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET) //确保是否产生了EXTI Line中断
    {
        ClibraFlag = FAIL;
#if UART_DEBUG
        // printf("key mode press...\r\n");
#endif
        NRF24L01_SetTxAddr();
        EXTI_ClearITPendingBit(EXTI_Line1);     //清除中断标志位
    }
}

extern char Lockflag;

//“+”按键，控制电机是否待机转动
void EXTI3_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line3) != RESET) //确保是否产生了EXTI Line中断
    {
        Lockflag = 1;
#if UART_DEBUG
        // printf("key add press...\r\n");
#endif
        EXTI_ClearITPendingBit(EXTI_Line3);     //清除中断标志位
    }
}

//“-”按键，控制IMU校准
void EXTI9_5_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line8) != RESET) //确保是否产生了EXTI Line中断
    {
        IMUcalibratflag = !IMUcalibratflag;
#if UART_DEBUG
        // printf("key sub press...\r\n");
#endif
        EXTI_ClearITPendingBit(EXTI_Line8);     //清除中断标志位
    }
}
