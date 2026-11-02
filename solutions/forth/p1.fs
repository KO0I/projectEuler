\ Project Euler 1: Multiples of 3 or 5.
require math.fs

: p1 ( -- answer )
  0 1000 0 do i 3 mod 0= i 5 mod 0= or if i + then loop ;

[undefined] euler-testing [if] p1 . cr bye [then]
