\ Project Euler 13: Large Sum.
require math.fs

: bto-u 0 { b result } ( b -- u )
  0 to result
  b blen@ 0 ?do
    result big-base * b blen@ 1- i - b blimb@ + to result
  loop result ;

: p13 ( -- answer )
  s" lsum" r/o open-file throw
  0 0 0 0 0 { f total next line count more }
  16 bnew to total 16 bnew to next
  128 allocate throw to line 0 to count
  begin line 128 f read-line throw to more more while
    line over next bfrom-dec total next baddabs!
    count 1+ to count drop
  repeat drop
  f close-file throw line free throw
  count 100 <> abort" p013 must contain 100 numbers"
  begin total bdecimal-length 10 > while 10 total bdiv-small! drop repeat
  total bto-u total bfree next bfree ;

[undefined] euler-testing [if] p13 . cr bye [then]
