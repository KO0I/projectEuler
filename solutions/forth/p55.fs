\ Project Euler 55: Lychrel Numbers.
require math.fs

: big-reverse 0 0 { original reversed temp digit } ( big big -- )
  8 bnew to temp original temp bcopy reversed bclear
  begin temp blen@ 0> while
    10 temp bdiv-small! to digit 10 reversed bmul-small! digit reversed badd-small!
  repeat temp bfree ;

: p55 ( -- answer )
  8 bnew 8 bnew 8 bnew 0 0 0 { value reversed temp old reached count }
  0 to count
  10000 1 do i value bset-u false to reached
    50 0 do
      value reversed big-reverse value reversed temp badd
      value to old temp to value old to temp
      value reversed big-reverse
      value reversed babscompare 0= if true to reached leave then
    loop reached 0= if count 1+ to count then
  loop value bfree reversed bfree temp bfree count ;

[undefined] euler-testing [if] p55 . cr bye [then]
