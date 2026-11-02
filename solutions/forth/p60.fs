\ Project Euler 60: Prime Pair Sets.
require math.fs

create p60-primes 4000 cells allot

variable p60-adj

create p60-candidates 4000 cells allot

create p60-scratch 20000 cells allot

variable p60-count

variable p60-best

: p60-free-adj ( -- )
  p60-adj @ ?dup if free throw 0 p60-adj ! then ;

: p60-allocate-adj { count } ( count -- )
  p60-free-adj
  count count * allocate throw p60-adj !
  p60-adj @ count count * erase ;

: p60-concat 0 { a b scale } ( a b -- ab )
  10 to scale begin scale b <= while scale 10 * to scale repeat
  a scale * b + ;

: p60-adj@ { i j } ( i j -- flag )
  i p60-count @ * j + p60-adj @ + c@ 0<> ;

: p60-prime@ ( index -- prime ) cells p60-primes + @ ;

: p60-build 0 0 0 0 0 0 { limit sieve count a b n j } ( bound -- )
  limit sieve-build to sieve 0 to count
  limit 1+ 3 do
    i 2 <> i 5 <> and sieve i + c@ 0<> and if
      i count cells p60-primes + ! count 1+ to count
    then
  loop
  count p60-count !
  count p60-allocate-adj
  count 0 do
    i p60-prime@ to a
    i 1+ to j
    begin j count < while
      j p60-prime@ to b
      a b + p60-best @ < a b + 3 mod 0<> and if
        a b p60-concat is-prime? if
          b a p60-concat is-prime? if
            1 p60-adj @ i count * j + + c!
            1 p60-adj @ j count * i + + c!
          then
        then
      then
      j 1+ to j
    repeat
  loop
  sieve free throw ;

: p60-search 0 0 0 0 0 0
  { depth total candidates size needed lower index next next-count vertex } ( depth sum candidates count -- )
  5 depth - to needed
  size needed < if exit then
  total to lower
  needed 0 do candidates i cells + @ p60-prime@ lower + to lower loop
  lower p60-best @ >= if exit then
  needed 1 = if lower p60-best ! exit then
  0 to index
  begin index needed + size <= while
    total to lower
    needed 0 do candidates index i + cells + @ p60-prime@ lower + to lower loop
    lower p60-best @ >= if exit then
    depth 4000 * cells p60-scratch + to next
    0 to next-count
    candidates index cells + @ to vertex
    size index 1+ do
      vertex candidates i cells + @ p60-adj@ if
        candidates i cells + @ next-count cells next + !
        next-count 1+ to next-count
      then
    loop
    depth 1+ total vertex p60-prime@ + next next-count recurse
    index 1+ to index
  repeat ;

: p60 ( -- sum )
  9223372036854775807 p60-best !
  10000 p60-build
  p60-count @ 0 do i i cells p60-candidates + ! loop
  0 0 p60-candidates p60-count @ p60-search
  p60-best @ 9223372036854775807 = if
    p60-free-adj true abort" Prime-pair discovery bound failed"
  then
  p60-best @ 1- p60-build
  p60-count @ 0 do i i cells p60-candidates + ! loop
  0 0 p60-candidates p60-count @ p60-search
  p60-best @ p60-free-adj ;

\ Problem 93: evaluate every binary-expression tree using reduced rationals.

[undefined] euler-testing [if] p60 . cr bye [then]
