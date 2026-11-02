\ Project Euler 108: Diophantine Reciprocals I.
require math.fs

: p108 ( -- n ) 1000 reciprocal-bound ;

[undefined] euler-testing [if] p108 . cr bye [then]
