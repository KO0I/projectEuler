\ Project Euler 40: Champernowne's Constant.
require math.fs

create champernowne-targets 1 , 10 , 100 , 1000 , 10000 , 100000 , 1000000 ,

: p40 ( -- answer )
  0 0 1 1 0 0 { at position product n divisor digit }
  begin at 7 < while
    1 to divisor begin divisor n 10 / <= while divisor 10 * to divisor repeat
    begin divisor 0> while
      n divisor / 10 mod to digit position 1+ to position
      position champernowne-targets at cells + @ = if
        product digit * to product at 1+ to at
        at 7 = if product exit then
      then
      divisor 10 / to divisor
    repeat n 1+ to n
  repeat product ;
\ Integer binary search. Forth stack comments cannot contain nested parentheses.

[undefined] euler-testing [if] p40 . cr bye [then]
