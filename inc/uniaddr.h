#ifndef _UNIADDR_UNIADDR_H
#define _UNIADDR_UNIADDR_H

struct uniaddr;

#include "define.h"
#include "utils.h"

#include "../../alarm/inc/alarm.h"

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define UNIADDR_LEN 32

typedef uint8_t unitype_t;

enum uniaddr_type {
	/* Internet-Address */
	UNITYPE_IN4,
	UNITYPE_IN6,

	/* Socket-Address */
	UNITYPE_SA4,
	UNITYPE_SA6
};

struct uniaddr {
	/*
	 * The type of the address.
	 */
	unitype_t          type;

	/*
	 * The buffer containing the address.
	 */
	uint8_t                data[UNIADDR_LEN];
};


struct unimask {
	short                  off;
	short                  len;

	uint8_t               data[UNIADDR_LEN];
};

/*
 * Clear a uniaddr struct.
 * 
 * @addr: Pointer to the uniaddr struct
 */
UNIADDR_API void uniaddr_clear(struct uniaddr *addr);


/*
 * Copy the content of one uniaddr struct to another.
 *
 * @out: Pointer to the uniaddr to copy to
 * @in: Pointer to the uniaddr to copy from
 *
 * Returns: 0 on success or -1 if an error occurred
 */
UNIADDR_API int uniaddr_copy(struct uniaddr *out, struct uniaddr *in);


/*
 * Parse a string containing an address in the uniaddress struct in binary form.
 *
 * @addr: A pointer to the address to parse the data to
 * @type: The type of the address
 * @ptr: A pointer to the string containing the address
 * @len: The length of the address string in bytes
 *
 * Returns: 0 on success or -1 if an error occurred
 */
UNIADDR_API int uniaddr_parse(struct uniaddr *addr, unitype_t type,
		void *ptr, int len);


/*
 * Convert a uniaddr struct to a nullterminated string.
 *
 * @addr: Pointer to the address to convert to a string
 * @str: A pointer to a char buffer to write the address string to
 * @lim: The length limit of the given buffer
 *
 * Returns: Either the number of bytes written to the buffer or -1 if an error
 *          occurred
 */
UNIADDR_API int uniaddr_str(struct uniaddr *addr, char *str, int lim);


/*
 * Convert a socket address to a uniaddr.
 *
 * @addr: The uniaddr to copy the address to
 * @sa: The socket address struct to copy the address from
 *
 * Returns: 0 on success or -1 if an error occurred
 */
UNIADDR_API int uniaddr_fsa(struct uniaddr *addr, struct sockaddr *sa);


/*
 * Clear and reset a uniaddr mask.
 *
 * @mask: Pointer to the mask to clear
 */
UNIADDR_API void unimask_clear(struct unimask *mask);


/*
 * Set an address mask which can be used to filter an address list.
 *
 * @mask: Pointer to the mask struct to write the filter mask to
 * @off: The offset of the important part in bytes
 * @len: The length of the active filter data in bytes
 * @ptr: The active filter data
 *
 * Returns: 0 on success or -1 if an error occurred
 */
UNIADDR_API int unimask_set(struct unimask *mask, short off, int len,
		unsigned char *ptr); 

/*
 * Create a mask from an address.
 *
 * @mask: Pointer to the mask to set
 * @addr: Pointer to the address to create the mask from
 *
 * Returns: 0 on success or -1 if an error occurred
 */
UNIADDR_API int unimask_faddr(struct unimask *mask,
		struct uniaddr *addr);


/*
 * Apply a mask to the address.
 *
 * @addr: The address to apply the mask on
 * @mask: The mask to apply
 *
 * Returns: 1 if the mask fits, 0 if not and -1 if an error occurred
 */
UNIADDR_API int unimask_use(struct uniaddr *addr, struct unimask *mask);

#endif /* _UNIADDR_UNIADDR_H */
