\ Project Euler 67: Maximum Path Sum II.
require math.fs

: p67 ( -- n ) s" tests/data/p067_triangle.txt" 100 triangle-path-max ;

[undefined] euler-testing [if] p67 . cr bye [then]
