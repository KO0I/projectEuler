\ Project Euler 50: Consecutive Prime Sum.
require math.fs

: p50 ( -- answer )
  1000000 sieve-build 0 0 0 0 0 0 0 { primes count best-length best-prime start-index end-index value prefix }
  80001 cells allocate throw to prefix
  0 to count 0 to best-length 0 to best-prime 0 0 prefix prefix!
  1000000 2 do primes i + c@ 0<> if
    count prefix prefix@ i + count 1+ prefix prefix! count 1+ to count
  then loop
  0 to start-index
  begin start-index count < while
    start-index best-length + 1+ to end-index
    begin end-index count <= while
      end-index prefix prefix@ start-index prefix prefix@ - to value
      value 1000000 >= if
        \ LEAVE is for counted loops, not BEGIN loops.
        count 1+ to end-index
      else
        primes value + c@ 0<> if end-index start-index - to best-length value to best-prime then
        end-index 1+ to end-index
      then
    repeat start-index 1+ to start-index
  repeat primes free throw prefix free throw best-prime ;

[undefined] euler-testing [if] p50 . cr bye [then]
