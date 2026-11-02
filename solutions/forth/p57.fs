\ Project Euler 57: Square Root Convergents.
require math.fs

: p57 ( -- n )
  8 bnew 8 bnew 8 bnew 8 bnew 0 { top bottom next temp total }
  3 top bset-u 2 bottom bset-u
  1000 0 do
    top bdecimal-length bottom bdecimal-length > if total 1+ to total then
    bottom temp bcopy 2 temp bmul-small!
    top temp next badd
    top bottom temp badd
    temp bottom bcopy next top bcopy
  loop
  top bfree bottom bfree next bfree temp bfree total ;

[undefined] euler-testing [if] p57 . cr bye [then]
