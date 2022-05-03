#ifndef __UART_H
#define __UART_H
#define BAUD 9600             // baud rate  波特率
#define SYSTEMCLOCK 11059200L // system clock frequency 晶振频率

typedef void (*uart_interrupt_cb_t)(void);

/**
 * @brief 串口初始化函数.
 */
void uart_init();

/**
 * @brief   串口数据发送函数, 向串口写入一个字节数据.
 *
 * @param string    需要发送的数据.
 */
void uart_send_data(unsigned char *string);

/**
 * @brief   开启串口中断函数，如只用串口进行简单的发送，则不需要。
 */
void uart_interrupt_enable();

/**
 *      注册中断事件的处理函数，如只需要类似只注册发送函数而不需要其他函数，可以使用
 *      下方的默认无用函数进行占位。
 *
 * @param on_send 当发送完数据需要执行的函数。
 * @param on_recv 当接收到数据需要执行的函数。
 * @param default 默认执行的函数。
 */
void uart_interrupt_handler_register(uart_interrupt_cb_t on_send,
                                     uart_interrupt_cb_t on_recv,
                                     uart_interrupt_cb_t _default);

/**
 * @brief   用于占位的无用函数，在调用中断注册函数时占位用。
 *
 */
void on_uart_send_complete_default(void);
void on_uart_recv_complete_default(void);
void on_uart_interrupt_default(void);

#endif