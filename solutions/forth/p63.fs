\ Project Euler 63: Powerful Digit Counts.
require math.fs

: p63 ( -- n )
  4 bnew 0 0 { value total exponent }
  10 1 do 1 value bset-u
    25 1 do j value bmul-small!
      value bdecimal-length i = if total 1+ to total then
      value bdecimal-length i < if leave then
    loop
  loop value bfree total ;

[undefined] euler-testing [if] p63 . cr bye [then]
