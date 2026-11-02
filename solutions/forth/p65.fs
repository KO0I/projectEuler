\ Project Euler 65: Convergents of e.
require math.fs

: p65 ( -- n )
  8 bnew 8 bnew 8 bnew 8 bnew 0 0 { numerator previous next temp a k }
  1 numerator bset-u 0 previous bset-u
  100 0 do
    99 i - to k
    k 0= if 2 to a else
      k 3 mod 2 = if k 3 / 1+ 2* to a else 1 to a then
    then
    numerator temp bcopy a temp bmul-small!
    temp previous next badd
    numerator previous bcopy next numerator bcopy
  loop
  numerator bdigitsum to a
  numerator bfree previous bfree next bfree temp bfree a ;

[undefined] euler-testing [if] p65 . cr bye [then]
