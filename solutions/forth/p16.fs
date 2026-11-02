\ Project Euler 16: Power Digit Sum.
require math.fs

: p16 ( -- answer )
  64 bnew { n } 1 n bset-u
  1000 0 do 2 n bmul-small! loop n bdigitsum n bfree ;

[undefined] euler-testing [if] p16 . cr bye [then]
