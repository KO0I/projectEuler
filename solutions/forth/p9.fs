\ Project Euler 9: Special Pythagorean Triplet.
require math.fs

: pythagorean-product 0 { a b c } ( a b -- result-or-zero )
  1000 a - b - to c
  c b > a a * b b * + c c * = and if a b * c * else 0 then ;

: p9 ( -- answer )
  334 1 do
    500 i 1+ do
      j i pythagorean-product dup 0<> if unloop unloop exit else drop then
    loop
  loop -1 ;

[undefined] euler-testing [if] p9 . cr bye [then]
