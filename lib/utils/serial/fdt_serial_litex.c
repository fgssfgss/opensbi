/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2021
 *
 * Authors:
 *   fgssfgss
 */

#include <sbi_utils/fdt/fdt_helper.h>
#include <sbi_utils/serial/fdt_serial.h>
#include <sbi_utils/serial/litex-uart.h>

static int serial_litex_init(void *fdt, int nodeoff,
			       const struct fdt_match *match)
{
	int rc;
	struct platform_uart_data uart;

	rc = fdt_parse_litex_uart_node(fdt, nodeoff, &uart);
	if (rc)
		return rc;

	return litex_uart_init(uart.addr);
}

static const struct fdt_match serial_litex_match[] = {
	{ .compatible = "litex.uart0" },
	{ .compatible = "litex.liteuart" },
	{},
};

struct fdt_serial fdt_serial_litex = {
	.match_table = serial_litex_match,
	.init = serial_litex_init
};
 
