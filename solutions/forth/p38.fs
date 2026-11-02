\ Project Euler 38: Pandigital Multiples.
require math.fs

: p38 ( -- n ) 0 0 0 0 0 { best joined multiplier base product }
  10000 1 do
    0 to joined 1 to multiplier
    begin joined decimal-digits 9 < while
      i multiplier * to product
      joined product append-number to joined
      multiplier 1+ to multiplier
    repeat
    multiplier 2 > joined 100000000 >= and joined 999999999 <= and if
      joined pandigital-nine? if best joined max to best then
    then
  loop best ;

[undefined] euler-testing [if] p38 . cr bye [then]
