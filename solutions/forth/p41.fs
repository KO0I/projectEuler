\ Project Euler 41: Pandigital Prime.
require math.fs

: pandigital-seven? 0 0 0 { n mask digit bit } ( n -- flag )
  0 to mask 7 0 do
    n 10 mod to digit digit 1 < digit 7 > or if false unloop exit then
    1 digit lshift to bit mask bit and if false unloop exit then
    mask bit or to mask n 10 / to n
  loop n 0= mask 254 = and ;

: p41 ( -- n ) 7654321 { number }
  begin number 1234567 >= while
    number pandigital-seven? if number is-prime? if number exit then then
    number 2 - to number
  repeat abort" No 7-pandigital prime" ;

[undefined] euler-testing [if] p41 . cr bye [then]
