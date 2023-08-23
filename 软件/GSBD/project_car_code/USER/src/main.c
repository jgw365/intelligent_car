/*********************************************************************************************************************
* RT1064DVL6A Opensourec Library ����RT1064DVL6A ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
* 
* ���ļ��� RT1064DVL6A ��Դ���һ����
* 
* RT1064DVL6A ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
* 
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
* 
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
* 
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
* 
* �ļ�����          main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 8.32.4 or MDK 5.33
* ����ƽ̨          RT1064DVL6A
* ��������          https://seekfree.taobao.com/
* 
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-21        SeekFree            first version
********************************************************************************************************************/
// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������
// �������ǿ�Դ����ֲ�ÿչ���


#include "zf_common_headfile.h"

//��ʼ���ϼ�
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
	clock_init(SYSTEM_CLOCK_600M);  // ����ɾ��
	debug_init();                   // ���Զ˿ڳ�ʼ��
	timer_init(GPT_TIM_1, TIMER_US);//��ʱ����ʼ�� ��������������ʱ
	//time start
	gpio_init(BEEP_PIN,GPO,0,GPIO_PIN_CONFIG);
	encoder_init();//��������ʼ��
	//
	//wireless_uart_init();//���ߴ��ڳ�ʼ��
	motor_init();//���pwm�����ų�ʼ��
	tft180_init();//��Ļ��ʼ��
	imu660ra_init();
	//icm20602_init(); 
    imu_init();	
	//imu_init_20602();//��Ư�ͱ�����ֵ
	key_sw_init();
	uart_init(UART_4, 9600, UART4_TX_C16, UART4_RX_C17); 
	uart_init(UART_1, 9600, UART1_TX_B12, UART1_RX_B13); 
    //uart_init(UART_5, 9600, UART5_TX_C28, UART5_RX_C29); 
    uart_init(UART_3, 9600, UART3_TX_C8, UART3_RX_C9); 	
	pit_ms_init(PIT_CH0, 5);//��ʱ���жϳ�ʼ��
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
	//pid��dǰ���Լӵ�ͨ�˲��� Ҳ���Լ�ǰ��   �������� �������ڼ��޻�   ��������ԽС  imuԽ��  ��ͨ�˲� �������Ż�
}   //����ջ��� û��Ŀ��ֵ ��ͨ���ǶȻ�����������  �ٵ�  ���ԼӲ���  ��������Щ��ͨд�Ĳ�����ȷ�ĵ�ͨ

int main(void)
{  key_sw_init();
	config_all();
  //test_pid_debug();//�ܵ�ʱ��ע�͵�
	state_control(0);
	uart_write_byte((UART_3),9);
	//work_state=0;
	while(1)
    {   tft180_show_int(0,9*lcd_wide,work_state,8);
		switch(work_state)
		{	
			case 0://ʶ���ͼ
				state_0();
				break;
			case 1://����λ��
				state_1();
				break;
			case 2://ʶ��ͼƬ
				state_2();
				break;
			//�ȴ�ͼƬʶ����
			case 3://��ʼ��
				state_3();
				break;
			//����
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


