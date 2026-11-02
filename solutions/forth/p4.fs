\ Project Euler 4: Largest Palindrome Product.
require math.fs

: decimal-palindrome? 0 0 { n x reversed } ( n -- flag )
  n to x 0 to reversed
  begin x 0> while
    reversed 10 * x 10 mod + to reversed
    x 10 / to x
  repeat reversed n = ;

: p4 ( -- answer )
  0 { best }
  1000 100 do
    1000 i do
      i j * dup best > if
        dup decimal-palindrome? if to best else drop then
      else drop then
    loop
  loop best ;

[undefined] euler-testing [if] p4 . cr bye [then]
