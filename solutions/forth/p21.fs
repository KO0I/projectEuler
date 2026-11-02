\ Project Euler 21: Amicable Numbers.
require math.fs

: p21 ( -- answer )
  0 0 { sum pair }
  10000 2 do
    i proper-divisor-sum to pair
    pair i <> pair 0> and if
      pair proper-divisor-sum i = if sum i + to sum then
    then
  loop sum ;

[undefined] euler-testing [if] p21 . cr bye [then]
