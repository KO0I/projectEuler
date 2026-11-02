\ Project Euler 71: Ordered Fractions.
require math.fs

: p71 ( -- n ) 0 1 0 { best-n best-d numerator }
  1000001 2 do
    i 3 * 1- 7 / to numerator
    numerator best-d * best-n i * > if numerator to best-n i to best-d then
  loop best-n ;

[undefined] euler-testing [if] p71 . cr bye [then]
