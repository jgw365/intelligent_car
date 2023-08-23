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
* �ļ�����          zf_device_imu963ra
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
/*********************************************************************************************************************
* ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   // Ӳ�� SPI ����
*                   SCL/SPC             �鿴 zf_device_imu963ra.h �� IMU963RA_SPC_PIN �궨��
*                   SDA/DSI             �鿴 zf_device_imu963ra.h �� IMU963RA_SDI_PIN �궨��
*                   SA0/SDO             �鿴 zf_device_imu963ra.h �� IMU963RA_SDO_PIN �궨��
*                   CS                  �鿴 zf_device_imu963ra.h �� IMU963RA_CS_PIN  �궨��
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ������������

*                   // ��� IIC ����
*                   SCL/SPC             �鿴 zf_device_imu963ra.h �� IMU963RA_SCL_PIN �궨��
*                   SDA/DSI             �鿴 zf_device_imu963ra.h �� IMU963RA_SDA_PIN �궨��
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ������������
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_imu963ra_h
#define _zf_device_imu963ra_h

#include "zf_common_typedef.h"

#define IMU963RA_USE_SOFT_IIC                       (0)                         // Ĭ��ʹ��Ӳ�� SPI ��ʽ����
#if IMU963RA_USE_SOFT_IIC                                                       // ������ ��ɫ�����Ĳ�����ȷ�� ��ɫ�ҵľ���û���õ�
//====================================================��� IIC ����====================================================
#define IMU963RA_SOFT_IIC_DELAY                     (100)                       // ��� IIC ��ʱ����ʱ���� ��ֵԽС IIC ͨ������Խ��
#define IMU963RA_SCL_PIN                            (C23)                       // ��� IIC SCL ���� ���� IMU963RA �� SCL ����
#define IMU963RA_SDA_PIN                            (C22)                       // ��� IIC SDA ���� ���� IMU963RA �� SDA ����
//====================================================��� IIC ����====================================================
#else
//====================================================Ӳ�� SPI ����====================================================
#define IMU963RA_SPI_SPEED                          (10 * 1000 * 1000)          // Ӳ�� SPI ����
#define IMU963RA_SPI                                (SPI_4        )             // Ӳ�� SPI ��
#define IMU963RA_SPC_PIN                            (SPI4_SCK_C23 )             // Ӳ�� SPI SCK ����
#define IMU963RA_SDI_PIN                            (SPI4_MOSI_C22)             // Ӳ�� SPI MOSI ����
#define IMU963RA_SDO_PIN                            (SPI4_MISO_C21)             // Ӳ�� SPI MISO ����
//====================================================Ӳ�� SPI ����====================================================
#endif
#define IMU963RA_CS_PIN                             C20                         // CS Ƭѡ����
#define IMU963RA_CS(x)                              (x? (gpio_high(IMU963RA_CS_PIN)): (gpio_low(IMU963RA_CS_PIN)))

#define IMU963RA_TIMEOUT_COUNT                      (0x00FF)                    // IMU963RA ��ʱ����

//================================================���� IMU963RA �ڲ���ַ================================================
#define IMU963RA_DEV_ADDR                           (0x6B)                      // SA0�ӵأ�0x6A SA0������0x6B ģ��Ĭ������
#define IMU963RA_SPI_W                              (0x00)
#define IMU963RA_SPI_R                              (0x80)

#define IMU963RA_FUNC_CFG_ACCESS                    (0x01)
#define IMU963RA_PIN_CTRL                           (0x02)
#define IMU963RA_S4S_TPH_L                          (0x04)
#define IMU963RA_S4S_TPH_H                          (0x05)
#define IMU963RA_S4S_RR                             (0x06)
#define IMU963RA_FIFO_CTRL1                         (0x07)
#define IMU963RA_FIFO_CTRL2                         (0x08)
#define IMU963RA_FIFO_CTRL3                         (0x09)
#define IMU963RA_FIFO_CTRL4                         (0x0A)
#define IMU963RA_COUNTER_BDR_REG1                   (0x0B)
#define IMU963RA_COUNTER_BDR_REG2                   (0x0C)
#define IMU963RA_INT1_CTRL                          (0x0D)
#define IMU963RA_INT2_CTRL                          (0x0E)
#define IMU963RA_WHO_AM_I                           (0x0F)
#define IMU963RA_CTRL1_XL                           (0x10)
#define IMU963RA_CTRL2_G                            (0x11)
#define IMU963RA_CTRL3_C                            (0x12)
#define IMU963RA_CTRL4_C                            (0x13)
#define IMU963RA_CTRL5_C                            (0x14)
#define IMU963RA_CTRL6_C                            (0x15)
#define IMU963RA_CTRL7_G                            (0x16)
#define IMU963RA_CTRL8_XL                           (0x17)
#define IMU963RA_CTRL9_XL                           (0x18)
#define IMU963RA_CTRL10_C                           (0x19)
#define IMU963RA_ALL_INT_SRC                        (0x1A)
#define IMU963RA_WAKE_UP_SRC                        (0x1B)
#define IMU963RA_TAP_SRC                            (0x1C)
#define IMU963RA_D6D_SRC                            (0x1D)
#define IMU963RA_STATUS_REG                         (0x1E)
#define IMU963RA_OUT_TEMP_L                         (0x20)
#define IMU963RA_OUT_TEMP_H                         (0x21)
#define IMU963RA_OUTX_L_G                           (0x22)
#define IMU963RA_OUTX_H_G                           (0x23)
#define IMU963RA_OUTY_L_G                           (0x24)
#define IMU963RA_OUTY_H_G                           (0x25)
#define IMU963RA_OUTZ_L_G                           (0x26)
#define IMU963RA_OUTZ_H_G                           (0x27)
#define IMU963RA_OUTX_L_A                           (0x28)
#define IMU963RA_OUTX_H_A                           (0x29)
#define IMU963RA_OUTY_L_A                           (0x2A)
#define IMU963RA_OUTY_H_A                           (0x2B)
#define IMU963RA_OUTZ_L_A                           (0x2C)
#define IMU963RA_OUTZ_H_A                           (0x2D)
#define IMU963RA_EMB_FUNC_STATUS_MAINPAGE           (0x35)
#define IMU963RA_FSM_STATUS_A_MAINPAGE              (0x36)
#define IMU963RA_FSM_STATUS_B_MAINPAGE              (0x37)
#define IMU963RA_STATUS_MASTER_MAINPAGE             (0x39)
#define IMU963RA_FIFO_STATUS1                       (0x3A)
#define IMU963RA_FIFO_STATUS2                       (0x3B)
#define IMU963RA_TIMESTAMP0                         (0x40)
#define IMU963RA_TIMESTAMP1                         (0x41)
#define IMU963RA_TIMESTAMP2                         (0x42)
#define IMU963RA_TIMESTAMP3                         (0x43)
#define IMU963RA_TAP_CFG0                           (0x56)
#define IMU963RA_TAP_CFG1                           (0x57)
#define IMU963RA_TAP_CFG2                           (0x58)
#define IMU963RA_TAP_THS_6D                         (0x59)
#define IMU963RA_INT_DUR2                           (0x5A)
#define IMU963RA_WAKE_UP_THS                        (0x5B)
#define IMU963RA_WAKE_UP_DUR                        (0x5C)
#define IMU963RA_FREE_FALL                          (0x5D)
#define IMU963RA_MD1_CFG                            (0x5E)
#define IMU963RA_MD2_CFG                            (0x5F)
#define IMU963RA_S4S_ST_CMD_CODE                    (0x60)
#define IMU963RA_S4S_DT_REG                         (0x61)
#define IMU963RA_I3C_BUS_AVB                        (0x62)
#define IMU963RA_INTERNAL_FREQ_FINE                 (0x63)
#define IMU963RA_INT_OIS                            (0x6F)
#define IMU963RA_CTRL1_OIS                          (0x70)
#define IMU963RA_CTRL2_OIS                          (0x71)
#define IMU963RA_CTRL3_OIS                          (0x72)
#define IMU963RA_X_OFS_USR                          (0x73)
#define IMU963RA_Y_OFS_USR                          (0x74)
#define IMU963RA_Z_OFS_USR                          (0x75)
#define IMU963RA_FIFO_DATA_OUT_TAG                  (0x78)
#define IMU963RA_FIFO_DATA_OUT_X_L                  (0x79)
#define IMU963RA_FIFO_DATA_OUT_X_H                  (0x7A)
#define IMU963RA_FIFO_DATA_OUT_Y_L                  (0x7B)
#define IMU963RA_FIFO_DATA_OUT_Y_H                  (0x7C)
#define IMU963RA_FIFO_DATA_OUT_Z_L                  (0x7D)
#define IMU963RA_FIFO_DATA_OUT_Z_H                  (0x7E)

//������������ؼĴ��� ��Ҫ��FUNC_CFG_ACCESS��SHUB_REG_ACCESSλ����Ϊ1������ȷ����
#define IMU963RA_SENSOR_HUB_1                       (0x02) 
#define IMU963RA_SENSOR_HUB_2                       (0x03) 
#define IMU963RA_SENSOR_HUB_3                       (0x04) 
#define IMU963RA_SENSOR_HUB_4                       (0x05) 
#define IMU963RA_SENSOR_HUB_5                       (0x06) 
#define IMU963RA_SENSOR_HUB_6                       (0x07) 
#define IMU963RA_SENSOR_HUB_7                       (0x08) 
#define IMU963RA_SENSOR_HUB_8                       (0x09) 
#define IMU963RA_SENSOR_HUB_9                       (0x0A) 
#define IMU963RA_SENSOR_HUB_10                      (0x0B) 
#define IMU963RA_SENSOR_HUB_11                      (0x0C) 
#define IMU963RA_SENSOR_HUB_12                      (0x0D) 
#define IMU963RA_SENSOR_HUB_13                      (0x0E) 
#define IMU963RA_SENSOR_HUB_14                      (0x0F) 
#define IMU963RA_SENSOR_HUB_15                      (0x10) 
#define IMU963RA_SENSOR_HUB_16                      (0x11) 
#define IMU963RA_SENSOR_HUB_17                      (0x12) 
#define IMU963RA_SENSOR_HUB_18                      (0x13) 
#define IMU963RA_MASTER_CONFIG                      (0x14) 
#define IMU963RA_SLV0_ADD                           (0x15) 
#define IMU963RA_SLV0_SUBADD                        (0x16) 
#define IMU963RA_SLV0_CONFIG                        (0x17) 
#define IMU963RA_SLV1_ADD                           (0x18) 
#define IMU963RA_SLV1_SUBADD                        (0x19) 
#define IMU963RA_SLV1_CONFIG                        (0x1A) 
#define IMU963RA_SLV2_ADD                           (0x1B) 
#define IMU963RA_SLV2_SUBADD                        (0x1C) 
#define IMU963RA_SLV2_CONFIG                        (0x1D) 
#define IMU963RA_SLV3_ADD                           (0x1E) 
#define IMU963RA_SLV3_SUBADD                        (0x1F) 
#define IMU963RA_SLV3_CONFIG                        (0x20) 
#define IMU963RA_DATAWRITE_SLV0                     (0x21) 
#define IMU963RA_STATUS_MASTER                      (0x22)

#define IMU963RA_MAG_ADDR                           (0x0D)                      // 7λIIC��ַ
#define IMU963RA_MAG_OUTX_L                         (0x00)
#define IMU963RA_MAG_CONTROL1                       (0x09)
#define IMU963RA_MAG_CONTROL2                       (0x0A)
#define IMU963RA_MAG_FBR                            (0x0B)
#define IMU963RA_MAG_CHIP_ID                        (0x0D)

#define IMU963RA_ACC_SAMPLE                         (0x3C)                      // ���ٶȼ�����
// ����Ϊ:0x30 ���ٶ�����Ϊ:��2G      ��ȡ���ļ��ٶȼ����� ����16393������ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
// ����Ϊ:0x38 ���ٶ�����Ϊ:��4G      ��ȡ���ļ��ٶȼ����� ����8197�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
// ����Ϊ:0x3C ���ٶ�����Ϊ:��8G      ��ȡ���ļ��ٶȼ����� ����4098�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
// ����Ϊ:0x34 ���ٶ�����Ϊ:��16G     ��ȡ���ļ��ٶȼ����� ����2049�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)

#define IMU963RA_GYR_SAMPLE                         (0x5C)                      // ����������
// ����Ϊ:0x52 ����������Ϊ:��125dps  ��ȡ�������������ݳ���228.6��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
// ����Ϊ:0x50 ����������Ϊ:��250dps  ��ȡ�������������ݳ���114.3��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
// ����Ϊ:0x54 ����������Ϊ:��500dps  ��ȡ�������������ݳ���57.1��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
// ����Ϊ:0x58 ����������Ϊ:��1000dps ��ȡ�������������ݳ���28.6��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
// ����Ϊ:0x5C ����������Ϊ:��2000dps ��ȡ�������������ݳ���14.3��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
// ����Ϊ:0x51 ����������Ϊ:��4000dps ��ȡ�������������ݳ���7.1��     ����ת��Ϊ������λ�����ݣ���λΪ����/s

#define IMU963RA_MAG_SAMPLE                         (0x19)                      // �شż�����
// ����Ϊ:0x19 ����������Ϊ:8G     ��ȡ���ļ��ٶȼ����� ����3000�� ����ת��Ϊ������λ�����ݣ���λ��G(��˹)
// ����Ϊ:0x09 ����������Ϊ:2G     ��ȡ���ļ��ٶȼ����� ����12000������ת��Ϊ������λ�����ݣ���λ��G(��˹)

//================================================���� IMU963RA �ڲ���ַ================================================

extern int16 imu963ra_acc_x,  imu963ra_acc_y,  imu963ra_acc_z;
extern int16 imu963ra_gyro_x, imu963ra_gyro_y, imu963ra_gyro_z;
extern int16 imu963ra_mag_x,  imu963ra_mag_y,  imu963ra_mag_z;

void    imu963ra_get_acc            (void);
void    imu963ra_get_gyro           (void);
void    imu963ra_get_mag            (void);
float   imu963ra_acc_transition     (int16 acc_value);                          // �� IMU963RA ���ٶȼ�����ת��Ϊʵ����������
float   imu963ra_gyro_transition    (int16 gyro_value);                         // �� IMU963RA ����������ת��Ϊʵ����������
float   imu963ra_mag_transition     (int16 mag_value);                          // �� IMU963RA �شż�����ת��Ϊʵ����������

uint8   imu963ra_init               (void);

#endif
