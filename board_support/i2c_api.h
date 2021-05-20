#ifndef I2C_API_H_INCLUDED
#define I2C_API_H_INCLUDED

#include "stm32l1xx.h"

/**
    @ingroup interface_drivers
    @defgroup i2c_api ��� ��������� ������ ����� I2C
    @{
*/

 /**
    @file i2c_api.h
    @author RedBees, LLC
    @copyright This file is distributed under the terms of the MIT license.
 */

///������������ ���������� ������, ������� ����� ������������ �������� ������� ������ I2C �� ������� (��������, ��������� ������� START).
///���������� ��� ���������� ��������� ����������� � ������ ��������������� ��������� ��������� �������� ������.
#define I2C_SAFETY_TIMEOUT      2000000UL

#ifndef NULL
#define NULL                    ((void *)0)
#endif

typedef enum { I2C_ERROR_NONE,I2C_ERROR_START_FAIL,I2C_ERROR_ADDR_FAIL,I2C_ERROR_READ_FAIL,I2C_ERROR_WRITE_FAIL } i2c_error_t;

/**
    @brief ��������� ��������� ������ I2C ��� ������ �� ����������� ������� SDA, ������ 100 ���.

    @warning ������� �� �������� ������������ ������ � �� ������������� ������������ �����. ��� �������� ������ ���� ������� ��������, �� �� ������.

    @param [in] i2c_inst ��������� �� ������ I2C.
    @param [in] sys_clk �������� �������� ������� ������ (��������, ��� I2C1 - ������������ ���� APB1) � ������ (��������, 16 ��� = 16000000 ��).

    @warning ��� ���������� ������ ������ I2C ��� �������� ������� ������ ���� �� ����� 2 � �� ����� 50 ���. � ����������� ��
    �������� ������������ ����������� ��� ����� �������� � ������������� ��������������� �������� ������������ ��� ��� ����, � ������� ���������
    ���������� ������ I2C, ������������� � ������� ���� �������.
*/
void init_i2c(I2C_TypeDef* i2c_inst,uint32_t sys_clk);

/**
    @brief ������� ��������� ������ �������� ���������� I2C �� ��������, ����� ��� ������ I2C-���������.

    ������ I2C-���������� (� ���������, ����������� TMP75 � ������������ LIS3DH) ������������� ������ � ����� ���������� ��������� ���������
    ��������: ����� ��������� ��������� START ������� �������� ���������� � ������ ������ �������� ��������� I2C � �������� ����-������ ��������,
    ������� ��������� ���������. ����� ����� ������� ���������� ��������� ���������� ������, �������� ���������� � ������ ������ � �����
    ��������� ������ ����� ������������� ��������. ����� ������ ������ ����������� ���������� STOP.
    ������� ��������� ��������� �������� �������.

    @param [in] i2c_inst ��������� �� ���� I2C
    @param [in] i2c_addr ����� ���������� �� ���� I2C (���� ���, ��� ���� ������/������)
    @param [in] pointer_byte ������ �������� ����������, �� �������� ����� ����������� ������
    @param [in] read_length ���������� ����, ������� ��������� ��������
    @param [out] read_buffer ��������� �� ������, � ������� ����� ����������� ������ ����������� ������

    @return ��� ������; ��� �������� ���������� - I2C_ERROR_NONE
*/
i2c_error_t i2c_read_with_pointer(I2C_TypeDef* i2c_inst,uint8_t i2c_addr,uint8_t pointer_byte,uint8_t read_length,uint8_t* read_buffer);
/**
    @brief ������� ��������� ������ �������� ���������� I2C �� ��������, ����� ��� ������ I2C-���������.

    ������ I2C-���������� (� ���������, ����������� TMP75 � ������������ LIS3DH) ������������� ����������� ������ � ���� ��������
    ��������� ��������: ����� ��������� ��������� START ������� �������� ���������� � ������ ������ �������� ��������� I2C, ����� ����
    �������� ����-������ ��������, � ������� ��������� ��������� ������. ����� ����� ������� �������� ������ � ��������� ������ ���������� STOP.
    ������� ��������� ��������� �������� �������.

    @param [in] i2c_inst ��������� �� ���� I2C
    @param [in] i2c_addr ����� ���������� �� ���� I2C (���� ���, ��� ���� ������/������)
    @param [in] pointer_byte ����-������ ��������, � ������� ����� ����������� ������
    @param [in] write_length ���������� ����, ������� ��������� ��������
    @param [in] write_buffer ������, ������� ��������� ��������

    @return ��� ������; ��� �������� ���������� - I2C_ERROR_NONE
*/
i2c_error_t i2c_write_with_pointer(I2C_TypeDef* i2c_inst,uint8_t i2c_addr,uint8_t pointer_byte,uint16_t write_length,uint8_t* write_buffer);

/**
    @}
*/

#endif /* I2C_API_H_INCLUDED */
