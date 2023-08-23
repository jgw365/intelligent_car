/*********************************************************************************************************************
* RT1064DVL6A Opensourec Library 即（RT1064DVL6A 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 RT1064DVL6A 开源库的一部分
* 
* RT1064DVL6A 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 8.32.4 or MDK 5.33
* 适用平台          RT1064DVL6A
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-09-21        SeekFree            first version
********************************************************************************************************************/
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完
// 本例程是开源库移植用空工程


#include "zf_common_headfile.h"

//初始化合计
void key_sw_init()
{
	gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);
	gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);
	gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);
	gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP);
	gpio_init(SW1, GPI, GPIO_HIGH, GPI_PULL_UP);
	gpio_init(SW2, GPI, GPIO_HIGH, GPI_PULL_UP);
}
void config_all()
{	
	DisableGlobalIRQ();
	clock_init(SYSTEM_CLOCK_600M);  // 不可删除
	debug_init();                   // 调试端口初始化
	timer_init(GPT_TIM_1, TIMER_US);//定时器初始化 用来给编码器计时
	//time start
	gpio_init(BEEP_PIN,GPO,0,GPIO_PIN_CONFIG);
	encoder_init();//编码器初始化
	//
	//wireless_uart_init();//无线串口初始化
	motor_init();//电机pwm和引脚初始化
	tft180_init();//屏幕初始化
	imu660ra_init();
	//icm20602_init(); 
    imu_init();	
	//imu_init_20602();//零漂和变量赋值
	key_sw_init();
	uart_init(UART_4, 9600, UART4_TX_C16, UART4_RX_C17); 
	uart_init(UART_1, 9600, UART1_TX_B12, UART1_RX_B13); 
    //uart_init(UART_5, 9600, UART5_TX_C28, UART5_RX_C29); 
    uart_init(UART_3, 9600, UART3_TX_C8, UART3_RX_C9); 	
	pit_ms_init(PIT_CH0, 5);//定时器中断初始化
	pit_ms_init(PIT_CH1,10);
	//pit_ms_init(PIT_CH2,100);
	interrupt_set_priority((PIT_IRQn), 0);  
	interrupt_set_priority(LPUART8_IRQn , 1);
	interrupt_set_priority(LPUART4_IRQn , 1);
	interrupt_set_priority(LPUART1_IRQn , 1);
	interrupt_set_priority(LPUART3_IRQn , 1);
	//interrupt_set_priority(LPUART5_IRQn , 1);
	uart_rx_interrupt(UART_4, 1);
  //   uart_rx_interrupt(UART_5, 1);	
	uart_rx_interrupt(UART_1, 1);   
	uart_rx_interrupt(UART_3, 0);   
	EnableGlobalIRQ(0);
	servo_init();
	wheel_control(0,0,0);
	key_index_enum key_index_array[KEY_NUMBER] = {KEY_1,KEY_2,KEY_3,KEY_4};
	//pid中d前可以加低通滤波器 也可以加前馈   控制周期 采样周期极限化   采样周期越小  imu越好  低通滤波 参数待优化
}   //电机闭环软 没到目标值 但通过角度环纠正过来了  再调  可以加差速  程序里有些低通写的不是正确的低通

int main(void)
{  key_sw_init();
	config_all();
  //test_pid_debug();//跑的时候注释掉
	state_control(0);
	uart_write_byte((UART_3),9);
	//work_state=0;
	while(1)
    {   tft180_show_int(0,9*lcd_wide,work_state,8);
		switch(work_state)
		{	
			case 0://识别地图
				state_0();
				break;
			case 1://纠正位置
				state_1();
				break;
			case 2://识别图片
				state_2();
				break;
			//等待图片识别结果
			case 3://初始化
				state_3();
				break;
			//完赛
			case 4:
				state_4();
				break;
			case 5:
				state_5();
				break;
			case 9:
				state_9(0,0);
				break;
			case 10:
				state_10();
				break;
		}
		work_state_p = work_state;
		

	}
}


