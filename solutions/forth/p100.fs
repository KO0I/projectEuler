\ Project Euler 100: Arranged Probability.
require math.fs

: p100 ( -- n ) 15 21 0 0 { blue total next-blue next-total }
  begin total 1000000000000 <= while
    3 blue * 2 total * + 2 - to next-blue
    4 blue * 3 total * + 3 - to next-total
    next-blue to blue next-total to total
  repeat blue ;

[undefined] euler-testing [if] p100 . cr bye [then]
