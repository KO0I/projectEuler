\ Project Euler 6: Sum Square Difference.
require math.fs

: p6 ( -- answer )
  100 101 * 2 / dup * 100 101 * 201 * 6 / - ;

[undefined] euler-testing [if] p6 . cr bye [then]
