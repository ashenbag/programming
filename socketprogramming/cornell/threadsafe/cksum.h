/* checksum routine for the Internet Protocol family headers
taken from Unix Network Programming - Stevens p454
*/


#include <sys/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* This function computes the check sum.
 */

unsigned short in_cksum( const void* ptr, size_t nbytes );

#ifdef __cplusplus
}
#endif
