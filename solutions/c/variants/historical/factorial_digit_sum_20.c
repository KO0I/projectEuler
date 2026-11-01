#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

/*
 * n! means n * (n-1)*...*3*2*1
 *
 * For example, 10! = 10*9...*3*2*1 = 3628800,
 * and the sum of the digits in the number 10! is 3 + 6 + 2 + 8 + 8 + 0 + 0 =
 * 27.
 *
 * Find the sum of the digits in the number 100!
*/

int main(void)
{
  mpz_t factorial;
  mpz_t base;
  mpz_t result;
  mpz_t temp;

  /* a,b are integers used for isolating a specific digit */
  mpz_t a,b;
  int i;
  /* since gmp won't take integers directly, an unsigned long is created, but
   * this will be a constant used only in the primary for loop.
   */
  unsigned long t=10;

  // n represents the nth factorial
  unsigned long n=100;

  // initialize mpz_t's
  mpz_init(factorial);
  mpz_init(base);
  mpz_init(result);
  mpz_init(temp);
  mpz_init(a);
  mpz_init(b);

  // set up appropriate values
  mpz_set_str(base,"2",10);
  mpz_set_str(factorial,"0",10);
  mpz_set_str(temp,"0",10);
  mpz_set_str(a,"10",10);
  mpz_set_str(b,"1",10);

  // this generates the giant number for analysis
  mpz_fac_ui(factorial, n);
  //gmp_printf("\n%Zd\n", factorial);

  //now, go through and sum up each digit in factorial
  for(i=1; i<=305; i++){
    //remove leading digits in giant and place it into temp
    mpz_mod( temp, factorial, a );
    //prepare b to truncate temp
    mpz_div_ui( b, a, t );
    //truncate trailing integers, isolating the digit
    mpz_div( temp, temp, b);
    //for debugging, this prints the iteration number and the temporary digit
    //printf("%i,", i);
    //gmp_printf("%Zd\n", temp);

    //add temp to the final sum
    mpz_add(result, result, temp);

    // get ready for the next iteration:
    mpz_mul_ui( a, a, t );
    // note: if I overshoot i, temp=0 and the final sum is not altered.
  }
  //print the final result
  gmp_printf("The sum of digits in 2^%lu is: %Zd\n", n, result);


  /* free used memory */
  mpz_clear(factorial);
  mpz_clear(base);
  mpz_clear(result);
  mpz_clear(temp);
  mpz_clear(a);
  mpz_clear(b);
  return 0;
}
