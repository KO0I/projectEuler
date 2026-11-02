\ Project Euler 18: Maximum Path Sum I.
require math.fs

: p18 ( -- n ) s" tests/data/p018_triangle.txt" 15 triangle-path-max ;

[undefined] euler-testing [if] p18 . cr bye [then]
