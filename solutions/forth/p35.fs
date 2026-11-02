\ Project Euler 35: Circular Primes.
require math.fs

: p35 ( -- answer )
  1000000 sieve-build 0 0 0 0 { primes count rotated power good }
  0 to count
  1000000 2 do primes i + c@ 0<> if
    1 to power begin power i 10 / <= while power 10 * to power repeat
    i to rotated true to good
    begin rotated 10 / rotated 10 mod power * + to rotated
      primes rotated + c@ 0= if false to good then
      rotated i = good 0= or
    until
    good if count 1+ to count then
  then loop primes free throw count ;

[undefined] euler-testing [if] p35 . cr bye [then]
