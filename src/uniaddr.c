#include "uniaddr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

UNIADDR_API void uniaddr_clear(struct uniaddr *addr)
{
	if(!addr) {
		ALARM(ALARM_WARN, "addr undefined");
		return;
	}

	memset(addr, 0, sizeof(struct uniaddr));
}


UNIADDR_API s32 uniaddr_copy(struct uniaddr *out, struct uniaddr *in)
{
	if(!out || !in) {
		ALARM(ALARM_WARN, "out or in undefined");
		return -1;
	}

	memcpy(out, in, sizeof(struct uniaddr));
	return 0;
}


UNIADDR_API s32 uniaddr_parse(struct uniaddr *addr, unitype_t type,
		void *ptr, s32 len)
{
	if(!addr || !ptr || len < 0) {
		ALARM(ALARM_WARN, "addr or ptr undefined or len invalid");
		return -1;
	}

	uniaddr_clear(addr);

	switch(type) {
		case UNITYPE_IN4:
			if(uniaddr_f_in4(addr, ptr, len) < 0)
				goto err_return;
			break;

		case UNITYPE_IN6:
			if(uniaddr_f_in6(addr, ptr, len) < 0)
				goto err_return;
			break;

		case UNITYPE_SA4:
			if(uniaddr_f_sa4(addr, ptr, len) < 0)
				goto err_return;
			break;

		case UNITYPE_SA6:
			if(uniaddr_f_sa6(addr, ptr, len) < 0)
				goto err_return;
			break;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Unable to parse address");
	return -1;
}


UNIADDR_API s32 uniaddr_str(struct uniaddr *addr, s8 *str, s32 lim)
{
	static s8 swp[128];
	s32 len;

	if(!addr || !str || lim < 0) {
		ALARM(ALARM_WARN, "addr or str undefined or lim invalid");
		return -1;
	}

	switch(addr->type) {
		case UNITYPE_IN4:
			if((len = uniaddr_str_in4(addr, swp, 128)) < 0)
				goto err_return;
			break;

		case UNITYPE_IN6:
			if((len = uniaddr_str_in6(addr, swp, 128)) < 0)
				goto err_return;
			break;

		case UNITYPE_SA4:
			if((len = uniaddr_str_sa4(addr, swp, 128)) < 0)
				goto err_return;
			break;

		case UNITYPE_SA6:
			if((len = uniaddr_str_sa6(addr, swp, 128)) < 0)
				goto err_return;
			break;
	}

	if(len > lim)
		goto err_return;

	strcpy((char *)str, (char *)swp);

	return len;

err_return:
	ALARM(ALARM_ERR, "Failed to convert the uniaddress to a string");
	return -1;	
}


UNIADDR_API s32 uniaddr_fsa(struct uniaddr *addr, struct sockaddr *sa)
{
	if(!addr || !sa) {
		ALARM(ALARM_WARN, "addr or sa undefined");
		return -1;
	}

	/*
	 * Clear the uniaddr.
	 */
	uniaddr_clear(addr);


	switch(sa->sa_family) {
		case AF_INET:
			addr->type = UNITYPE_SA4;
			memcpy(addr->data, &((struct sockaddr_in *)sa)->sin_addr, 4);
			memcpy(addr->data + 4, &((struct sockaddr_in *)sa)->sin_port, 2);
			break;

		case AF_INET6:
			addr->type = UNITYPE_SA6;
			memcpy(addr->data, &((struct sockaddr_in6 *)sa)->sin6_addr, 16);
			memcpy(addr->data + 16, &((struct sockaddr_in6 *)sa)->sin6_port, 2);
			break;

		default:
			ALARM(ALARM_WARN, "Unknown address family");
			goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to convert sockaddr to uniaddr");
	return -1;
}


UNIADDR_API void unimask_clear(struct unimask *mask)
{
	if(!mask) {
		ALARM(ALARM_WARN, "mask undefined");
		return;
	}

	memset(mask, 0, sizeof(struct unimask));
}



UNIADDR_API s32 unimask_set(struct unimask *mask, s16 off, s32 len,
		u8 *ptr)
{
	if(!mask) {
		ALARM(ALARM_WARN, "mask undefined");
		return -1;
	}

	unimask_clear(mask);

	mask->off = off;
	mask->len = len;
	memcpy(mask->data + off, ptr, len);

	return 0;
}


UNIADDR_API s32 unimask_faddr(struct unimask *mask,
		struct uniaddr *addr)
{
	if(!mask || !addr) {
		ALARM(ALARM_WARN, "mask or addr undefined");
		return -1;
	}


	unimask_clear(mask);

	mask->off = 0;
	mask->len = UNIADDR_LEN;
	memcpy(mask->data, addr->data, UNIADDR_LEN);

	return 0;
}


UNIADDR_API s32 unimask_use(struct uniaddr *addr, struct unimask *mask)
{
	s32 i;

	if(!addr || !mask) {
		ALARM(ALARM_WARN, "addr or mask undefined");
		return -1;
	}

	for(i = 0; i < mask->len; i++) {
		if(addr->data[mask->off + i] != mask->data[i])
			return 0;
	}

	return 1;
}


#define UNIADDR_DUMP_LINE   16


UNIADDR_API s32 uniaddr_dump(struct uniaddr *addr)
{
	u8 buf[17];
	u8 *pc = (u8 *)addr;
	s32 i;

	if(!addr) {
		ALARM(ALARM_WARN, "addr undefined");
		return -1;
	}

	for(i = 0; i < UNIADDR_LEN; i++) {
		if ((i % UNIADDR_DUMP_LINE) == 0) {
			if (i != 0)
				printf("  %s\n", buf);

			printf("  %04x ", i);
		}

		printf(" %02x", pc[i]);

		if ((pc[i] < 0x20) || (pc[i] > 0x7e))
			buf[i % 16] = '.';
		else
			buf[i % 16] = pc[i];

		buf[(i % 16) + 1] = '\0';
	}

	while((i % 16) != 0) {
		printf("   ");
		i++;
	}

	printf("  %s\n", buf);

	return 0;
}
