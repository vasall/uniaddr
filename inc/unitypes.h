#ifndef _UNIADDR_UNITYPES_H
#define _UNIADDR_UNITYPES_H

#include "imports.h"

typedef u8 unitype_t;

/*
 * Note that the numbering of the different types will probably change in the
 * future as more types are added.
 */

enum uniaddr_type {
	/* Internet-Address */
	UNITYPE_IN4 = 0x01,
	UNITYPE_IN6 = 0x02,

	/* Socket-Address */
	UNITYPE_SA4 = 0x03,
	UNITYPE_SA6 = 0x04
};

#endif /* _UNIADDR_UNITYPES_H */
