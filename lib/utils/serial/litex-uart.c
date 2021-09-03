/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2021
 *
 * Authors:
 *   fgssfgss
 */

#include <sbi/riscv_io.h>
#include <sbi/sbi_console.h>
#include <sbi_utils/serial/litex-uart.h>

/* clang-format off */

#define UART_REG_RXTX       0
#define UART_REG_TXFULL     1
#define UART_REG_RXEMPTY    2
#define UART_REG_EV_STATUS  3
#define UART_REG_EV_PENDING 4
#define UART_REG_EV_ENABLE  5

/* clang-format on */

static volatile void *uart_base;

static u32 get_reg(u32 num)
{
	return readl(uart_base + (num * 0x4));
}

static void set_reg(u32 num, u32 val)
{
	writel(val, uart_base + (num * 0x4));
}

static void litex_uart_putc(char ch)
{
	while (get_reg(UART_REG_TXFULL) & 0x01);
	set_reg(UART_REG_RXTX, ch);
}

static int litex_uart_getc(void)
{
	u32 c = -1;
	if (!(get_reg(UART_REG_RXEMPTY) & 0x01)) {
		c = get_reg(UART_REG_RXTX);
		set_reg(UART_REG_EV_PENDING, 0x02);
	}
	return c;
}

static struct sbi_console_device litex_console = {
	.name	      = "litex_uart",
	.console_putc = litex_uart_putc,
	.console_getc = litex_uart_getc
};

int litex_uart_init(unsigned long base)
{
	uart_base = (volatile void *)base;

	sbi_console_set_device(&litex_console);

	return 0;
}

