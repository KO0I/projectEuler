\ Project Euler 36: Double-base Palindromes.
require math.fs

: palindrome-base? 0 0 { n base reversed x } ( n base -- flag )
  n to x 0 to reversed
  begin reversed base * x base mod + to reversed x base / to x x 0= until
  reversed n = ;

: p36 ( -- n ) 0 { total }
  1000000 1 do i 10 palindrome-base? i 2 palindrome-base? and if total i + to total then 2 +loop total ;

[undefined] euler-testing [if] p36 . cr bye [then]
