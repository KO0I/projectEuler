\ Project Euler 89: Roman Numerals.
require math.fs

: p89-symbol ( ascii -- numeral )
  case
    [char] I of 1 endof [char] V of 5 endof
    [char] X of 10 endof [char] L of 50 endof
    [char] C of 100 endof [char] D of 500 endof
    [char] M of 1000 endof
    abort" Invalid roman glyph"
  endcase ;

create p89-values 1000 , 900 , 500 , 400 , 100 , 90 , 50 , 40 , 10 , 9 , 5 , 4 , 1 ,

create p89-lengths 1 , 2 , 1 , 2 , 1 , 2 , 1 , 2 , 1 , 2 , 1 , 2 , 1 ,

: p89-minimum { value } ( numeral -- minimal count )
  0 13 0 do
    value i p89-values prefix@ / i p89-lengths prefix@ * +
    value i p89-values prefix@ mod to value
  loop ;

: p89 ( -- n )
  s" tests/data/p089_roman.txt" 20000 read-fixture
  0 0 0 0 0 0 { raw length saved before value last current count }
  length 0 do
    i raw + c@ dup 32 > if
      p89-symbol to current
      current last > if value last 2* - to value then
      value current + to value
      current to last before 1+ to before
    else drop
      before 0> if
        before value p89-minimum - saved + to saved
        0 to before 0 to value 0 to last count 1+ to count
      then
    then
  loop
  before 0> if before value p89-minimum - saved + to saved count 1+ to count then
  count 1000 <> abort" Expected 1000 roman entries"
  raw free throw saved ;

[undefined] euler-testing [if] p89 . cr bye [then]
