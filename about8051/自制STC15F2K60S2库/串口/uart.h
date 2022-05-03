#ifndef __UART_H
#define __UART_H
#define BAUD 9600             // baud rate  ������
#define SYSTEMCLOCK 11059200L // system clock frequency ����Ƶ��

typedef void (*uart_interrupt_cb_t)(void);

/**
 * @brief ���ڳ�ʼ������.
 */
void uart_init();

/**
 * @brief   �������ݷ��ͺ���, �򴮿�д��һ���ֽ�����.
 *
 * @param string    ��Ҫ���͵�����.
 */
void uart_send_data(unsigned char *string);

/**
 * @brief   ���������жϺ�������ֻ�ô��ڽ��м򵥵ķ��ͣ�����Ҫ��
 */
void uart_interrupt_enable();

/**
 *      ע���ж��¼��Ĵ���������ֻ��Ҫ����ֻע�ᷢ�ͺ���������Ҫ��������������ʹ��
 *      �·���Ĭ�����ú�������ռλ��
 *
 * @param on_send ��������������Ҫִ�еĺ�����
 * @param on_recv �����յ�������Ҫִ�еĺ�����
 * @param default Ĭ��ִ�еĺ�����
 */
void uart_interrupt_handler_register(uart_interrupt_cb_t on_send,
                                     uart_interrupt_cb_t on_recv,
                                     uart_interrupt_cb_t _default);

/**
 * @brief   ����ռλ�����ú������ڵ����ж�ע�ắ��ʱռλ�á�
 *
 */
void on_uart_send_complete_default(void);
void on_uart_recv_complete_default(void);
void on_uart_interrupt_default(void);

#endif