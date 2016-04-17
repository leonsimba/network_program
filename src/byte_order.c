/*
 * byte_order.c - Get host byte order
 *
 * Copyright (C) 2016 ZhiPing.He. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Author:
 * 	ZhiPing.He <zhiping.hzp@alibaba-inc.com>
 */

#include <stdio.h>

void byte_order(void)
{
	union {
		short s;
		char  c[sizeof(short)];
	} un;

   	un.s = 0x0102;

	if (sizeof(short) == 2) {
		if ((un.c[0] == 1) && (un.c[1] == 2))
			printf("big endian\n");
		else if ((un.c[0] == 2) && (un.c[1] == 1))
			printf("little endian\n");
		else
			printf("unknown ...\n");
	}
}

int main()
{
	byte_order();
	return 0;
}
