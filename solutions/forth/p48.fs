\ Project Euler 48: Self Powers.
require math.fs

: p48 ( -- answer )
  0 { sum }
  1001 1 do i i 10000000000 power-mod sum + 10000000000 mod to sum loop sum ;

[undefined] euler-testing [if] p48 . cr bye [then]
