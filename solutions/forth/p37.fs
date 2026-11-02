\ Project Euler 37: Truncatable Primes.
require math.fs

: right-truncatable? { n } ( n -- flag )
  begin n 0> while n is-prime? 0= if false exit then n 10 / to n repeat true ;

: left-truncatable? 0 { n power } ( n -- flag )
  1 to power begin power n 10 / <= while power 10 * to power repeat
  begin power 1 > while
    n power mod to n n is-prime? 0= if false exit then
    power 10 / to power
  repeat true ;

: p37 ( -- n ) 0 0 { total count }
  1000000 11 do
    i right-truncatable? i left-truncatable? and if total i + to total count 1+ to count then
    count 11 = if total unloop exit then
  2 +loop abort" Not all truncatable primes found" ;

[undefined] euler-testing [if] p37 . cr bye [then]
