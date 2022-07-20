#ifndef _UNIADDR_UTILS_H
#define _UNIADDR_UTILS_H

#include "define.h"
#include "imports.h"
#include "uniaddr.h"

#include <arpa/inet.h>

/*
 * Convert an IPv4 address from the string format to binary format.
 *
 * @addr: The uniaddr to write the address to
 * @ptr: Pointer to the string containing the IPv4 address
 * @len: The length of the string containing the address
 *
 * Returns: 0 on success or -1 if an error occurred
 */
UNIADDR_API s8 uniaddr_f_in4(struct uniaddr *addr, s8 *ptr, s32 len);


/*
 * Convert an IPv6 address from the string format to binary format.
 *
 * @addr: The uniaddr to write the address to
 * @ptr: Pointer to the string containing the IPv6 address
 * @len: The length of the string containing the address
 *
 * Returns: 0 on success or -1 if an error occurred
 */
UNIADDR_API s8 uniaddr_f_in6(struct uniaddr *addr, s8 *ptr, s32 len);


/*
 * Convert an IPv4 socket address from the string format to binary format.
 *
 * @addr: The uniaddr to write the address to
 * @ptr: Pointer to the string containing the IPv4 socket address
 * @len: The length of the string containing the address
 *
 * Returns: 0 on success or -1 if an error occurred
 */
UNIADDR_API s8 uniaddr_f_sa4(struct uniaddr *addr, s8 *ptr, s32 len);


/*
 * Convert an IPv6 socket address from the string format to binary format.
 *
 * @addr: The uniaddr to write the address to
 * @ptr: Pointer to the string containing the IPv6 socket address
 * @len: The length of the string containing the address
 *
 * Returns: 0 on success or -1 if an error occurred
 */
UNIADDR_API s8 uniaddr_f_sa6(struct uniaddr *addr, s8 *ptr, s32 len);



/*
 * Convert a uniaddr containing an IPv4 address from the binary format to the
 * string format.
 *
 * @addr: The uniaddr containing the address to convert
 * @str: A pointer to write the stringified address to
 * @lim: The limit of the buffer
 *
 * Returns: Either the number of bytes written to the string pointer including
 *          the nullterminator or -1 if an error occurred
 */
UNIADDR_API s32 uniaddr_str_in4(struct uniaddr *addr, s8 *str, s32 lim);


/*
 * Convert a uniaddr containing an IPv6 address from the binary format to the
 * string format.
 *
 * @addr: The uniaddr containing the address to convert
 * @str: A pointer to write the stringified address to
 * @lim: The limit of the buffer
 *
 * Returns: Either the number of bytes written to the string pointer including
 *          the nullterminator or -1 if an error occurred
 */
UNIADDR_API s32 uniaddr_str_in6(struct uniaddr *addr, s8 *str, s32 lim);


/*
 * Convert a uniaddr containing an IPv4 socket address from the binary form to
 * string form.
 *
 * @addr: The uniaddr containing the address to convert
 * @str: A pointer to write the stringified address to
 * @lim: The limit of the buffer
 *
 * Returns: Either the number of bytes written to the string pointer including
 *          the nullterminator or -1 if an error occurred
 */
UNIADDR_API s32 uniaddr_str_sa4(struct uniaddr *addr, s8 *str, s32 lim);


/*
 * Convert a uniaddr containing an IPv6 socket address from the binary form to
 * string form.
 *
 * @addr: The uniaddr containing the address to convert
 * @str: A pointer to write the stringified address to
 * @lim: The limit of the buffer
 *
 * Returns: Either the number of bytes written to the string pointer including
 *          the nullterminator or -1 if an error occurred
 */
UNIADDR_API s32 uniaddr_str_sa6(struct uniaddr *addr, s8 *str, s32 lim);


#endif /* _UNIADDR_UTILS_H */
