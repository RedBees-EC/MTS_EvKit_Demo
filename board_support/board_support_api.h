#ifndef BOARD_DRIVERS_H_INCLUDED
#define BOARD_DRIVERS_H_INCLUDED

/**
    @ingroup fw_code
    @defgroup board_support ��� ��������� ���������
    @{
*/

 /**
    @file board_support_api.h
    @author RedBees, LLC
    @copyright This file is distributed under the terms of the MIT license.
 */

#include "stm32l1xx.h"
#include "board_pins.h"
#include "port_macros.h"
#include "uart_api.h"
#include "i2c_api.h"
#include "stdio.h"
#include "tmp75_driver.h"
#include "lis3dh_driver.h"
#include "at_cmd_support.h"
#include "awu_api.h"
#include "coap_essentials.h"
#include "gnss_support.h"
#include "string.h"
#include "stdlib.h"
#include "ds1721_driver.h"

#define FIRMWARE_VERSION            "2.6 alpha, 23.07.2021"

#define MTS_COAP_SERVER_IP          "195.34.49.22"
#define MTS_COAP_SERVER_PORT        6683

///�������� �������� �������, �� ������� �������� ����������
#define DEFAULT_CLOCK_FREQ      16000000UL
///�������� �������� ������ ��� ������� (����� printf() ����� USART1 � ���������� � ����� USB-UART ���������)
#define CONSOLE_BAUDRATE        115200UL
///�� ��������� ������ EVA M8 ������ ������ �� �������� 9600 ���/�
#define GNSS_BAUDRATE           9600UL
/**
�������� 1 �������� �������� ������� �������� ��������; �������� ������� �������� �������� ������� �������� ��������� ����� � ����� ������� �������� (������� ��������)
��� ���������� ������� �������� ����������� ������
*/
#define ECHO_INPUT_CHARS            0
///�������� ������ � ������������
#define SARA_R410M_DEFAULT_BAUDRATE 115200UL

///����� ������������ TMP75, �������������� �� �����
#define LOCAL_TSENSOR_I2C_ADDR      0x48
///����� ������������� LIS3DH, �������������� �� �����
#define LOCAL_ACCEL_I2C_ADDR        0x18

/**
    ����� ����� ���� �������������� ������������� TMP75 ���� DS1721. ��������� ������ � ���� ������, �� �� ���������� �� 100%.
    ���� ������ ����� �������� 1, �������� ����� �������������� � ���������� DS1721; ����� ����� ����������� ������� ��� TMP75.
*/
#define USE_DS1721              1

#define MCU_UNIQUE_IDENTIFIER_ADDRESS       ((uint32_t *)0x1FF800CC)

//���������� �����: ��������� ����� ������� ������������ �� PA8 (D7 �� Arduino-����������� �������).
//#define CONFIGURE_CLOCK_OUT

//�������� �� PA8 ������ �� ���������� LSE
//#define OBSERVE_LSE
//�������� �� PA8 ������� �������� �������� ������ �����������
//#define OBSERVE_SYSCLK

///������������ ���������� ������ �������� ������� �������� �����������. ���� ������ ������ ������ �������, ����� ������� ������� � ���, ���
///��������� �������.
#define OSC_STARTUP_CYCLES_MAX      2000000UL

/**
    ������������ ��� �������, ������������������� ��� ������� ������� ������ ������� � ������ ������.
    �������� ��-��������� ������������� ������������ ������� �� ������� 1 ��� ��� ��������� ������� 16 ��� (HSI).
*/
#define TMR_PRESC                   15
///���������� �������� ������� �� 1 ��.
#define TMR_CNT_1MSEC               1000
///�������� ������ ������, ���������� � �������� �������.
#define BTN_POLL_PERIOD             50000

///��. �������� @link device_setup_data_t @endlink
#define LOGGER_MODE_VALUE           15

///��. �������� ���� reserved ��������� @link device_setup_data_t @endlink
#define SETTINGS_SIGNATURE_0        0xA5
///��. �������� ���� reserved ��������� @link device_setup_data_t @endlink
#define SETTINGS_SIGNATURE_1        0x55
///��. �������� ���� reserved ��������� @link device_setup_data_t @endlink
#define SETTINGS_SIGNATURE_2        0xAA

/** @brief ���������, �������� ��������� ������ ��������� ����������������� ����������
    @warning ��������� ����������� � ������� EEPROM. �� ������� ������������ ������ � ��� �������, ������ ��������� ������ ���� ������ ������� ������.
*/
typedef struct {
    ///IP/IPv6-����� ������� ��� �������� ���������� ����� IP/IPv6
    uint8_t target_server_IP[48];
    ///URL �� ������� ��� �������� ���������� ����� IP
    uint8_t target_URL[128];
    ///��� APN ��� NIDD
    uint8_t NIDD_APN_name[64];
    //60 32-bit words up to here
    ///�������� �������� ���������� � �������������
    uint32_t telemetry_interval_ms;
    ///����� ����� �� ������� ��� �������� ���������� ����� IP
    uint16_t target_server_port;
    ///���� ������������� ���������� NIDD. ���� �������� �������, �������� ����� ����������� ����� NIDD, ����� ����� IP
    uint8_t use_NIDD;
    /**
        ���� ���������� ����� �������� @link LOGGER_MODE_VALUE @endlink, �� ����� �������� ����� ����� ���������� � ����� ������������� �������� ����������. ��� ����� ������ ��������
        ����� ����� ����������� � ������ ���� ��������.

        @note ���������� �������� ���������� ������� ��� ����, ����� � ������, ����� ��������� � EEPROM �� ���������������� ��� ����������, �����
        � ������������ ������������ ���������� � ������ ����.
    */
    uint8_t logger_mode_on_startup;
    ///���� ���������� ������� �� ����, � ����� ���������� �� ����� ���������� �������� ������, �������� � GNSS-������ (��������� �� ����������� �������� ����� ������ � �������).
    uint8_t gnss_privacy_mode;
    /**
        ������������ ��������� �� ������� �����, � ����� ��������, ��������������� � ������� ���������� ������ � EEPROM. ��� ��������� �������������
        � ���� ������ ������������ �������� SETTINGS_SIGNATURE_[0..2]. ���� �������� ����� ���� �� ����� ������������������ ��������� ��������,
        ����������� �������������� ���� � ��������� ��������� ���������.
    */
    uint8_t reserved[3];
} device_setup_data_t;

///� ������, ���� ��������� LSE ��������� �� �������, �������� ���� ���������� ����� ���������.
extern volatile uint8_t LSE_failed;
///������������� ��������: ���������� �������� ������� ����� �� 1 ������� ��� ������� �������� �������. ���������� �������� ����������� ��� ������ �����.
extern volatile uint32_t CYCLES_PER_1SEC;

/**
    @brief ������� ��������� ��������� ������������ � ������������� ���������, ������������ ���������������� �����������.

    USART2 ������������ ��� ���������� ������� � ���������� printf (��������� XP4 � XP5 ������ ������ � ��������� 1-2)
    I2C1 ������������ ��� ������ ������� ����������� � �������������
    ����� PA5 ��������� � ����������������� ����������, �������������� �� ����� (��������� XP8 ������ ������ � ��������� 2-3)
    USART3 ������������ ��� ������������ � NB-IoT �������.

*/
void init_board(void);

/**
    @brief ���������� ����� � ������� ������ ������ ��������

    @return ����� � ������� ������ ������ ��������, � �������������.
*/
uint32_t get_uptime_ms(void);

///���������������� ���������� ��������� �� �������� �����. �������� �������� �������� � �������������.
void delay_ms(uint32_t N);

/**
    @brief ���������� ����� ��������� ������ ������� � ������� ����������� ������ �������.

    ��� ����� ������ �������� ���������� ����� ������ �� ������� � ���������� BTN_POLL_PERIOD. ���� ������ ������, ������ ��������
    ������ ���������� ��������� ��������. ����� ������ ���������, �������� �������� �����������. ����� �������, ����� ����������, ���� �� ������
    ������, � ��������� �����.

    @warning ����� ����, ��� ������ ���� ��������, � �� ������ get_button_press(), ������� �� ��������� �� ������� ������.

    @return �����, ������� ������ ���� ������, � �������� BTN_POLL_PERIOD. ���� ������ �� ���� ������ � ������� ����������� ������ �������, ������������ ����.
*/
uint32_t get_button_press(void);

/**
    @brief �������� ��� ��������� ��������� �� �����.

    @param [in] state 1 - �������� ���������, 0 - ��������� ���������.
*/
void switch_LED(uint8_t state);

///���������� ������� ������� ������ ������������������ �������� 500 �� �� ����� PWR_ON �����������.
void SARA_R410_PWR_ON_pulse(void);

#define EEPROM_START_ADDRESS            ((uint32_t *)0x08080000)

/**
    @brief ������ � EEPROM

    @param [in] eeprom_ptr ��������� �� ����� � EEPROM, � �������� ������� ������ ������
    @param [in] data_ptr ��������� �� ������ ������ � ���� ������������������ ����
    @param [in] data_size_words ������ ������� ������ � ������������������ ������

    @warning ������ ������ ���� ��������� �� ������� �����.
*/
void FLASH_WriteEEPROM(volatile uint32_t *eeprom_ptr,uint32_t *data_ptr,uint16_t data_size_words);

/**
    @brief ���������� �������� ���������� � ������� EEPROM

    @param [in] data - ��������� �� ��������� ���� @link device_setup_data_t @endlink, ������ �� ������� ����� �������� � EEPROM
*/
void store_device_settings(device_setup_data_t *data);

/**
    @brief �������� �������� ���������� �� ������� EEPROM

    @param [in] data - ��������� �� ���������, � ������� ����� ����������� ������ �� EERPOM
*/
void recall_device_settings(device_setup_data_t *data);

/**
    @brief ������������ ������ � ������ �������� backspace

    ���������� scanf() � newlib �� ������������ ������� backspace. �������� ������� �������� ������ ������������ � ������
    ������ � ����������� ���������; apply_backspace() ��������� ������ �������� ��������� �������� ��������.

    @param [in] str ������, �������� ���������� ���������������; ������ ����������� �� �����
    @param [in] max_length ������������ ����� ������ (������ ������)
*/
void apply_backspace(uint8_t *str,uint16_t max_length);

/**
    @}
*/

#endif /* BOARD_DRIVERS_H_INCLUDED */
