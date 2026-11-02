\ Project Euler 112: Bouncy Numbers.
require math.fs

: p112-bouncy? 0 0 0 0 { n last increasing decreasing digit }
  true to increasing true to decreasing
  n 10 mod to last n 10 / to n
  begin n 0> while
    n 10 mod to digit
    digit last > if false to decreasing then
    digit last < if false to increasing then
    digit to last n 10 / to n
  repeat increasing 0= decreasing 0= and ;

: p112 ( -- n ) 0 100 { count n }
  begin n p112-bouncy? if count 1+ to count then
    count 100 * n 99 * = if n exit then
    n 1+ to n
  again ;

[undefined] euler-testing [if] p112 . cr bye [then]
