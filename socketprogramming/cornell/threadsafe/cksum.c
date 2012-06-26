/* checksum routine for the Internet Protocol family headers
taken from Unix Network Programming - Stevens p454
*/

#include "cksum.h"

/* This function computes the check sum.
 */
unsigned short in_cksum( const void* _ptr, size_t nbytes) {
  const unsigned char* ptr = (const unsigned char*)_ptr;
  register long sum; /* assumes long == 32 bits*/
  unsigned short  oddbyte;
  register unsigned short answer = 0; /* assumes u_short == 16 bits*/

  sum = 0;
  while (nbytes > 1) {
    sum += *ptr++;
    nbytes -=2;
  }

  if (nbytes ==1) {
    oddbyte =0;
    *(unsigned char *) &oddbyte = *ptr;
    sum +=oddbyte;
  }

  sum = (sum>>16) + (sum & 0xffff);
  sum += (sum >> 16);
  answer = (unsigned short)~sum;
  return answer;  
}

