\ Project Euler 58: Spiral Primes.
require math.fs

: p58 ( -- n ) 0 0 0 { prime-count square step }
  3 begin
    dup dup * to square dup 1- to step
    4 1 do square i step * - is-prime? if prime-count 1+ to prime-count then loop
    prime-count 10 * over 2* 1- < if exit then
    2 +
  again ;

[undefined] euler-testing [if] p58 . cr bye [then]
