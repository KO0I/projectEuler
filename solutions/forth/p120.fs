\ Project Euler 120: Square Remainders.
require math.fs

: p120 ( -- n ) 0 { total }
  1001 3 do
    i 2 mod if i i 1- * else i i 2 - * then total + to total
  loop total ;

\ Further reusable enumerators and exact integer recurrences.

[undefined] euler-testing [if] p120 . cr bye [then]
