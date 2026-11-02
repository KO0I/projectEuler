\ Project Euler 99: Largest Exponential.
require math.fs
fvariable p99-best-score

: p99 ( -- n )
  s" tests/data/p099_base_exp.txt" 2000 fixture-integers
  0 0 0 { values count best-index index exponent }
  count 2000 <> abort" Invalid exponential-comparison data"
  -1e300 p99-best-score f!
  1000 0 do
    i 2* values prefix@ s>f fln
    i 2* 1+ values prefix@ s>f f*
    fdup p99-best-score f@ f> if
      i 1+ to best-index
      p99-best-score f!
    else fdrop then
  loop values free throw best-index ;

[undefined] euler-testing [if] p99 . cr bye [then]
