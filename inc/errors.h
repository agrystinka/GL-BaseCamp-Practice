/**
 * libsk errors - provides definitions for all error codes in libsk
 *
 * User may check corresponding error codes against this definitions.
 * It is not requried to include this file in user code as it is already included
 * everywhere these error definitions are used (unless you want to use same codes for own routines)
 */
 #ifndef ERRORS_H
 #define ERRORS_H
enum sk_err {
	/** No error, everything fine */
	SK_EOK = 0,
	/** Unknown error. Something went wrong but don't boter detecting what */
	SK_EUNKNOWN = -1,
	/** Some feature is not implemented yet */
	SK_ENOTIMPLEMENTED = -2,
	/** Wrong argument passed */
	SK_EWRONGARG = -3
};


// alias type for more concise definitions
typedef enum sk_err sk_err;
#endif
