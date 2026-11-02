\ Project Euler 44: Pentagon Numbers.
require math.fs

: p44 ( -- answer )
  0 0 0 0 { best pj pk difference }
  5000 2 do
    i i 3 * 1- * 2 / to pj
    \ A descending +LOOP includes its limit. Use 1, never the invalid P_0.
    1 i 1- do
      i i 3 * 1- * 2 / to pk pj pk - to difference
      best 0<> difference best >= and if leave then
      difference pentagonal? if
        pj pk + pentagonal? if difference to best then
      then
    -1 +loop
  loop best ;

[undefined] euler-testing [if] p44 . cr bye [then]
