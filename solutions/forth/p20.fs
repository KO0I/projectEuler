\ Project Euler 20: Factorial Digit Sum.
require math.fs

: p20 ( -- answer )
  64 bnew { n } 1 n bset-u
  101 2 do i n bmul-small! loop n bdigitsum n bfree ;

[undefined] euler-testing [if] p20 . cr bye [then]
