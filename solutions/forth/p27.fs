\ Project Euler 27: Quadratic Primes.
require math.fs

: quadratic-run 0 0 { a b n value } ( a b -- count )
  0 to n begin n n * a n * + b + to value value is-prime?
  while n 1+ to n repeat n ;

: p27 ( -- answer )
  0 0 0 { best product score }
  1000 2 do i is-prime? if
    1000 -999 do
      i j quadratic-run to score score best > if score to best i j * to product then
    loop
  then loop product ;

[undefined] euler-testing [if] p27 . cr bye [then]
