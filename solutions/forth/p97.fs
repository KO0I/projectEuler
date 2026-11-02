\ Project Euler 97: Large Non-Mersenne Prime.
require math.fs

: p97 ( -- n ) 2 7830457 10000000000 power-mod 28433 * 1+ 10000000000 mod ;

[undefined] euler-testing [if] p97 . cr bye [then]
