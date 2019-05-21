/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/queue.h>

#include <rte_memory.h>
#include <rte_launch.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_lcore.h>
#include <rte_debug.h>

static int
lcore_hello(void *arg)
{
	unsigned lcore_id;
	char *ptr = arg;
	lcore_id = rte_lcore_id();
	printf("hello from core %u\n", lcore_id);
	printf("hello from %s\n", ptr);
	return 0;
}

int
main(int argc, char **argv)
{
	int ret;
	unsigned lcore_id;
	char caller[16];

	ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_panic("Cannot init EAL\n");

	memset(caller, 0, sizeof(caller));
	/* call lcore_hello() on every slave lcore */
	RTE_LCORE_FOREACH_SLAVE(lcore_id) {
		snprintf(caller, sizeof(caller), "slave-id %d", lcore_id);
		rte_eal_remote_launch(lcore_hello, caller, lcore_id);
	}

	/* call it on master lcore too */
	lcore_hello("Master");

	rte_eal_mp_wait_lcore();
	return 0;
}
