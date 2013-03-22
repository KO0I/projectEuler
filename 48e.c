#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

/* 
 *  The series, 11 + 22 + 33 + ... + 1010 = 10405071317.
 *
 *  Find the last ten digits of the series, 11 + 22 + 33 + ... + 10001000.
 */

/*
 * The algorithm:
 * 1. starting from 999^999, calculate number with brute force
 * 2. if greater than 10 billion, truncate everything but the last 10 digits
 * 3. add to a temporary sum
 * 4. repeat until 1^1
 * 5. truncate anything beyond the 10th digit.
 * 6. print final result.
 */
int main(void)
{
  //this number is the result of the exponentiation shenanigans
  mpz_t bignum;

  //this number represents both the base and the exponent, starting with the
  //monster number 999^999.
  unsigned long n=999;
  
  //represents the truncated bignum;
  mpz_t temp;

  //result represents the final 10-digit number
  mpz_t result;

  /* a is the integers used for isolating the last 10 digits*/
  mpz_t a;
  int i;
  /* since gmp won't take integers directly, an unsigned long is created, but
   * this will be a constant used only in the primary for loop.
   */

  //digitspan and exp controls the size of the final sum, here, it is 10 digits.
  mpz_t digitspan;
  unsigned long exp = 10;

  // initialize mpz_t's
  mpz_init(bignum);
  mpz_init(result);
  mpz_init(temp);
  mpz_init(a);
  mpz_init(digitspan);

  // set up appropriate values
  mpz_set_str(temp,"0",10);
  mpz_set_str(result,"0",10);
//  mpz_set_str(a,"10",10);
  mpz_set_str(digitspan,"10",10);
  mpz_pow_ui(digitspan, digitspan, exp);

for(n=999; n>=1; n--){
  /* this generates the giant number */
  mpz_ui_pow_ui(bignum, n, n);
  //gmp_printf("\n%Zd\n", bignum);
  /* Truncate!*/
  mpz_mod(temp, bignum, digitspan);
  //gmp_printf("\n%Zd\n", temp);
  /* Add this truncated value to the result! */
  mpz_add(result,result,temp);
}

/* truncate the final value, for completeness */
mpz_mod(result, result, digitspan);
gmp_printf("\n%Zd\n", result);
  
/* free used memory */
  mpz_clear(bignum);
  mpz_clear(result);
  mpz_clear(temp);
  mpz_clear(digitspan);
  mpz_clear(a);
  return 0;
}
