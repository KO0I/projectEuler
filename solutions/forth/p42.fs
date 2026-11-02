\ Project Euler 42: Coded Triangle Numbers.
require math.fs

: p42 ( -- n )
  s" tests/data/p042_words.txt" 100000 read-fixture
  0 0 0 0 { raw length score count ch root }
  0 to score 0 to count
  length 0 do raw i + c@ to ch
    ch [char] A >= ch [char] Z <= and if
      score ch [char] A - 1+ + to score
    else ch 34 = score 0> and if
      score 8 * 1+ integer-sqrt to root
      root root * score 8 * 1+ = if count 1+ to count then
      0 to score
    then then
  loop raw free throw count ;

[undefined] euler-testing [if] p42 . cr bye [then]
