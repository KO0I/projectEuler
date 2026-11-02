\ Project Euler 30: Digit Fifth Powers.
require math.fs

: p30 ( -- answer )
  0 { sum }
  354295 2 do i 5 digit-pow-sum i = if sum i + to sum then loop sum ;

[undefined] euler-testing [if] p30 . cr bye [then]
