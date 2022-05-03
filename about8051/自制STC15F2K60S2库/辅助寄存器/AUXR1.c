#include "AUXR1.h"

void uart_location_set(uart_setting_t loc) {
	AUXR1 |= loc;
}
void spi_location_set(spi_setting_t loc) {
	AUXR1 |= loc;
}
void ccp_location_set(ccp_setting_t loc) {
	AUXR1 |= loc;
}