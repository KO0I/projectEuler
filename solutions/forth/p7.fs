\ Project Euler 7: 10 001st Prime.
require math.fs

: p7 ( -- answer )
  0 1 { count value }
  begin count 10001 < while
    value 1+ to value
    value is-prime? if count 1+ to count then
  repeat value ;

[undefined] euler-testing [if] p7 . cr bye [then]
