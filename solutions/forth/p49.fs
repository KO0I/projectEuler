\ Project Euler 49: Prime Permutations.
require math.fs

: p49 ( -- n )
  9999 sieve-build 0 0 0 0 { primes b c answer sig }
  10000 1000 do primes i + c@ if
    i digit-signature to sig
    10000 i 1+ ?do
      j 2* i - to c
      c 10000 >= if leave then
      primes j + c@ primes c + c@ and if
        i 1487 = j 4817 = and 0= if
          sig j digit-signature = sig c digit-signature = and if
            i 100000000 * j 10000 * + c + to answer
            primes free throw answer unloop unloop exit
          then
        then
      then
    loop
  then loop primes free throw answer ;

[undefined] euler-testing [if] p49 . cr bye [then]
