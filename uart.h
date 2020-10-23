#ifndef __UART_H
#define	__UART_H

#include "sys.h"

void uart_send(char *str);
void config_uart(unsigned int baud);

#endif