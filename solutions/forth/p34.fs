\ Project Euler 34: Digit Factorials.
require math.fs

: digit-factorial-sum 0 0 { n value result } ( n -- sum )
  n to value 0 to result
  begin value 10 mod factorial result + to result value 10 / to value
    value 0= if result exit then
  again ;

: p34 ( -- answer )
  0 { sum }
  2540161 3 do i digit-factorial-sum i = if sum i + to sum then loop sum ;

[undefined] euler-testing [if] p34 . cr bye [then]
