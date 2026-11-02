\ Project Euler 10: Summation of Primes.
require math.fs

: p10 ( -- answer )
  2000000 sieve-build 0 { bytes answer }
  0 to answer
  2000000 2 do bytes i + c@ 0<> if answer i + to answer then loop
  bytes free throw answer ;

[undefined] euler-testing [if] p10 . cr bye [then]
