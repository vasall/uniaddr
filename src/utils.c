#include "utils.h"

#include <stdlib.h>
#include <string.h>

UNIADDR_API int uniaddr_f_in4(struct uniaddr *addr, char *ptr, int len)
{
	if(!addr || !ptr || len < 0) {
		ALARM(ALARM_WARN, "addr or ptr undefined or len invalid");
		return -1;
	}
	
	if(inet_pton(AF_INET, (char *)ptr, addr->data) != 1)
		goto err_return;

	addr->type = UNITYPE_IN4;
	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to convert in4");
	return -1;
}


UNIADDR_API int uniaddr_f_in6(struct uniaddr *addr, char *ptr, int len)
{
	if(!addr || !ptr || len < 0) {
		ALARM(ALARM_WARN, "addr or ptr undefined or len invalid");
		return -1;
	}
	
	if(inet_pton(AF_INET6, (char *)ptr, addr->data) != 1)
		goto err_return;

	addr->type = UNITYPE_IN6;
	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to convert in6");
	return -1;
}


UNIADDR_API int uniaddr_f_sa4(struct uniaddr *addr, char *ptr, int len)
{
	short port_num;
	int i;

	int brk;
	char str_ip4[64];
	char str_port[8];

	if(!addr || !ptr || len < 0) {
		ALARM(ALARM_WARN, "addr or ptr undefined or len invalid");
		return -1;
	}


	brk = -1;
	for(i = len - 1; i >= 0; i--) {
		if(((char *)ptr)[i] == ':') {
			brk = i;
			break;
		}
	}

	if(brk < 0) {
		ALARM(ALARM_ERR, "Could not seperate port and IPv4");
		goto err_return;
	}

	memcpy(str_ip4, ptr, i);
	str_ip4[i] = 0;

	memcpy(str_port, ptr + i + 1, len - i + 1);
	str_port[len - i + 1] = 0;

	/* Write address to buffer */
	if(inet_pton(AF_INET, str_ip4, addr->data) != 1) {
		ALARM(ALARM_ERR, "Could not convert IPv4 using inet_pton");
		goto err_return;
	}

	/* Write port to buffer */
	port_num = htons(atoi(str_port));
	memcpy(addr->data + 4, &port_num, 2);

	addr->type = UNITYPE_SA4;
	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to convert sa4");
	return -1;
}


UNIADDR_API int uniaddr_f_sa6(struct uniaddr *addr, char *ptr, int len)
{
	short port_num;
	int i;

	int brk;
	char str_ip6[64];
	char str_port[8];

	if(!addr || !ptr || len < 0) {
		ALARM(ALARM_WARN, "addr or ptr undefined or len invalid");
		return -1;
	}


	brk = -1;
	for(i = len - 1; i >= 0; i--) {
		if(((char *)ptr)[i] == ':') {
			brk = i;
			break;
		}
	}

	if(brk < 0) {
		ALARM(ALARM_ERR, "Could not seperate port and IPv6");
		goto err_return;
	}

	memcpy(str_ip6, ptr, i);
	str_ip6[i] = 0;

	memcpy(str_port, ptr + i + 1, len - i + 1);
	str_port[len - i + 1] = 0;

	/* Write address to buffer */
	if(inet_pton(AF_INET6, str_ip6, addr->data) != 1) {
		ALARM(ALARM_ERR, "Could not convert IPv6 using inet_pton");
		goto err_return;
	}

	/* Write port to buffer */
	port_num = htons(atoi(str_port));
	memcpy(addr->data + 16, &port_num, 2);

	addr->type = UNITYPE_SA6;
	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to convert sa6");
	return -1;
}


UNIADDR_API int uniaddr_str_in4(struct uniaddr *addr, char *str, int lim)
{
	char swpbuf[128];
	int swplen;	

	if(!addr || addr->type != UNITYPE_IN4 || !str || lim < 0) {
		ALARM(ALARM_WARN, "addr or str undefined or wrong address type "
				"or lim invalid");
		return -1;
	}

	if(inet_ntop(AF_INET, addr->data, swpbuf, 128) == NULL)
		goto err_return;
	swplen = strlen(swpbuf) + 1;

	if(swplen > lim)
		goto err_return;

	strcpy(str, swpbuf);
	return swplen;

err_return:
	ALARM(ALARM_ERR, "Failed to convert IN4 uniaddr to string");
	return -1;
}


UNIADDR_API int uniaddr_str_in6(struct uniaddr *addr, char *str, int lim)
{
	char swpbuf[128];
	int swplen;

	if(!addr || addr->type != UNITYPE_IN6 || !str || lim < 0) {
		ALARM(ALARM_WARN, "addr or str undefined or wrong address type "
				"or lim invalid");
		return -1;
	}

	if(inet_ntop(AF_INET6, addr->data, swpbuf, 128) == NULL)
		goto err_return;
	swplen = strlen(swpbuf) + 1;

	if(swplen > lim)
		goto err_return;

	strcpy(str, swpbuf);
	return swplen;

err_return:
	ALARM(ALARM_ERR, "Failed to convert IN6 uniaddr to string");
	return -1;
}


UNIADDR_API int uniaddr_str_sa4(struct uniaddr *addr, char *str, int lim)
{
	unsigned char mdl_in4[4];
	short mdl_port;

	char swp_in4[128];
	int swp_in4_len;
	char swp_port[8];
	int swp_port_len;

	if(!addr || addr->type != UNITYPE_SA4 || !str || lim < 0) {
		ALARM(ALARM_WARN, "addr or str undefined or wrong address type "
				"or lim invalid");
		return -1;
	}

	memcpy(mdl_in4, addr->data, 4);
	memcpy(&mdl_port, addr->data + 4, 2);

	if(inet_ntop(AF_INET, mdl_in4, swp_in4, 64) == NULL)
		goto err_return;
	swp_in4_len = strlen(swp_in4);

	mdl_port = htons(mdl_port);
	if((swp_port_len = sprintf(swp_port, "%d", mdl_port)) < 0)
		goto err_return;

	if(swp_in4_len + swp_port_len + 1 > lim)
		goto err_return;

	memcpy(str, swp_in4, swp_in4_len);
	str[swp_in4_len] = ':';
	memcpy(str + swp_in4_len + 1, swp_port, swp_port_len);
	str[swp_in4_len + swp_port_len + 1] = 0;

	return swp_in4_len + swp_port_len + 2;

err_return:
	ALARM(ALARM_ERR, "Failed to convert SA4 uniaddr to string");
	return -1;
}


UNIADDR_API int uniaddr_str_sa6(struct uniaddr *addr, char *str, int lim)
{
	unsigned char mdl_in6[16];
	short mdl_port;

	char swp_in6[128];
	int swp_in6_len;
	char swp_port[8];
	int swp_port_len;

	if(!addr || addr->type != UNITYPE_SA6 || !str || lim < 0) {
		ALARM(ALARM_WARN, "addr or str undefined or wrong address type "
				"or lim invalid");
		return -1;
	}

	memcpy(mdl_in6, addr->data, 16);
	memcpy(&mdl_port, addr->data + 16, 2);

	if(inet_ntop(AF_INET6, mdl_in6, swp_in6, 64) == NULL)
		goto err_return;
	swp_in6_len = strlen(swp_in6);

	mdl_port = htons(mdl_port);
	if((swp_port_len = sprintf(swp_port, "%d", mdl_port)) < 0)
		goto err_return;

	if(swp_in6_len + swp_port_len + 2 > lim)
		goto err_return;

	memcpy(str, swp_in6, swp_in6_len);
	str[swp_in6_len] = ':';
	memcpy(str + swp_in6_len + 1, swp_port, swp_port_len);
	str[swp_in6_len + swp_port_len + 1] = 0;

	return swp_in6_len + swp_port_len + 2;

err_return:
	ALARM(ALARM_ERR, "Failed to convert SA6 uniaddr to string");
	return -1;
}
