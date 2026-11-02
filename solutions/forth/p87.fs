\ Project Euler 87: Prime Power Triples.
require math.fs

: p87 ( -- n )
  7072 sieve-build
  1000 cells allocate throw
  6250000 allocate throw
  0 0 0 0 0 0 0 { primes list seen count answer x y z value mask }
  seen 6250000 erase
  7072 2 do primes i + c@ if i count list prefix! count 1+ to count then loop
  count 0 do
    i list prefix@ dup * dup * to x
    x 50000000 >= if leave then
    count 0 do
      i list prefix@ dup dup * * to y
      x y + 50000000 >= if leave then
      count 0 do
        i list prefix@ dup * x y + + to z
        z 50000000 >= if leave then
        1 z 7 and lshift to mask
        z 8 / seen + dup c@ mask and 0= if
          dup c@ mask or swap c! answer 1+ to answer
        else drop then
      loop
    loop
  loop
  primes free throw list free throw seen free throw answer ;

[undefined] euler-testing [if] p87 . cr bye [then]
