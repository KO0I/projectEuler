\ Project Euler 47: Distinct Primes Factors.
require math.fs

: distinct-prime-factors 0 0 0 { n value divisor count } ( n -- distinct count )
  n to value 2 to divisor 0 to count
  begin divisor value divisor / <= while
    value divisor mod 0= if count 1+ to count
      begin value divisor mod 0= while value divisor / to value repeat
    then divisor 1+ to divisor
  repeat value 1 > if count 1+ to count then count ;

: p47 ( -- answer )
  0 { streak }
  1000000 2 do i distinct-prime-factors 4 = if
    streak 1+ to streak streak 4 = if i 3 - unloop exit then
  else 0 to streak then loop -1 ;

[undefined] euler-testing [if] p47 . cr bye [then]
