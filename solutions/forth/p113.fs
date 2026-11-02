\ Project Euler 113: Non-bouncy Numbers.
require math.fs

: p113 ( -- n ) 110 10 choose 109 9 choose + 1002 - ;

[undefined] euler-testing [if] p113 . cr bye [then]
