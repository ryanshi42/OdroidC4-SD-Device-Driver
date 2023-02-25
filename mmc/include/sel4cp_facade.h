#pragma once

#if MMC_RPI3B_UNIT_TEST

#include <stdlib.h>
#include <stdint.h>

/**
 * The following is originally defined in:
 * build/libsel4/include/sel4/shared_types_gen.h
 */
typedef int seL4_MessageInfo_t;

/**
 * The following is defined in: kernel/libsel4/include/sel4/errors.h
 */
typedef enum {
    seL4_NoError = 0,
    seL4_InvalidArgument,
    seL4_InvalidCapability,
    seL4_IllegalOperation,
    seL4_RangeError,
    seL4_AlignmentError,
    seL4_FailedLookup,
    seL4_TruncatedMessage,
    seL4_DeleteFirst,
    seL4_RevokeFirst,
    seL4_NotEnoughMemory,

    /* This should always be the last item in the list
     * so it gives a count of the number of errors in the
     * enum.
     */
    seL4_NumErrors
} seL4_Error;

typedef size_t seL4_Uint64;
typedef seL4_Uint64 seL4_Word;
typedef seL4_Word seL4_CPtr;

typedef seL4_CPtr seL4_ARM_VSpace;

typedef unsigned int sel4cp_channel;
typedef seL4_MessageInfo_t sel4cp_msginfo;

void sel4cp_notify(sel4cp_channel ch);

void sel4cp_dbg_puts(const char *s);

seL4_Error seL4_ARM_VSpace_Clean_Data(seL4_ARM_VSpace _service, seL4_Word start, seL4_Word end);

sel4cp_msginfo sel4cp_msginfo_new(uint64_t label, uint16_t count);

void sel4cp_irq_ack(sel4cp_channel ch);

#else

#include <sel4cp.h>

#endif
