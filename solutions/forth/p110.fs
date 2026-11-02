\ Project Euler 110: Diophantine Reciprocals II.
require math.fs

: p110 ( -- n ) 4000000 reciprocal-bound ;

[undefined] euler-testing [if] p110 . cr bye [then]
