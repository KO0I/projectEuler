\ Project Euler 56: Powerful Digit Sum.
require math.fs

: p56 ( -- n )
  8 bnew 0 { value best }
  100 1 do
    1 value bset-u
    100 1 do
      j value bmul-small!
      value bdigitsum best max to best
    loop
  loop value bfree best ;

[undefined] euler-testing [if] p56 . cr bye [then]
