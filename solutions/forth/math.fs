\ Reusable mathematics and input; no problem dispatch.
require big.fs

: require-64bit ( -- ) 1 cells 8 <> abort" These solvers require 64-bit Gforth" ;
require-64bit

: is-prime? 0 { n divisor } ( n -- flag )
  n 2 < if false exit then
  n 2 mod 0= if n 2 = exit then
  3 to divisor
  begin divisor n divisor / <= while
    n divisor mod 0= if false exit then
    divisor 2 + to divisor
  repeat true ;

: integer-gcd 0 { a b temp } ( a b -- gcd )
  begin b 0<> while
    a b mod to temp b to a temp to b
  repeat a ;

: divisor-count 0 0 0 0 { n value result divisor exponent } ( n -- count )
  n to value 1 to result 2 to divisor
  begin divisor value divisor / <= while
    value divisor mod 0= if
      1 to exponent
      begin value divisor mod 0= while
        value divisor / to value exponent 1+ to exponent
      repeat
      result exponent * to result
    then
    divisor 2 = if 3 else divisor 2 + then to divisor
  repeat
  value 1 > if result 2 * to result then
  result ;

: proper-divisor-sum 0 0 { n result divisor } ( n -- sum )
  n 2 < if 0 exit then
  1 to result 2 to divisor
  begin divisor n divisor / <= while
    n divisor mod 0= if
      result divisor + to result
      n divisor / divisor <> if result n divisor / + to result then
    then
    divisor 1+ to divisor
  repeat result ;

: factorial 0 { n acc } ( n -- n! )
  1 to acc n 2 < if 1 exit then
  n 1+ 2 do acc i * to acc loop acc ;

: digit-pow-sum 0 0 0 { n exponent result x digit } ( n power -- sum )
  n to x 0 to result
  begin
    x 10 mod to digit
    1 exponent 0 ?do digit * loop
    result + to result
    x 10 / to x
    x 0= if result exit then
  again ;

: read-fixture 0 0 0 { addr u capacity f buf count } ( caddr u capacity -- buf n )
  addr u r/o open-file throw to f
  capacity allocate throw to buf
  buf capacity f read-file throw to count
  f close-file throw buf count ;

: sieve-build 0 0 { limit p j } ( limit -- ptr )
  limit 1+ allocate throw to p
  p limit 1+ 1 fill
  0 p c! 0 p 1+ c!
  limit 1+ 2 do
    i limit i / > if leave then
    p i + c@ 0<> if
      i i * to j
      begin j limit <= while
        0 p j + c! j i + to j
      repeat
    then
  loop p ;

: integer-sqrt 0 0 0 { n low high midpoint } ( n -- root )
  0 to low n to high
  begin low high < while
    low high low - 2 / + 1+ to midpoint
    midpoint n midpoint / <= if midpoint to low else midpoint 1- to high then
  repeat low ;

: pentagonal? 0 0 { n t root } ( n -- flag )
  n 1 < if false exit then
  n 24 * 1+ to t t integer-sqrt to root
  root root * t = root 1+ 6 mod 0= and ;

: multiply-mod { a b modulo } ( a b modulo -- remainder )
  a b um* modulo um/mod drop ;

: power-mod 0 { base exponent modulo result } ( b exp m -- residue )
  1 to result
  begin exponent 0> while
    exponent 1 and if result base modulo multiply-mod to result then
    base base modulo multiply-mod to base exponent 2 / to exponent
  repeat result ;

: prefix@ ( index address -- value ) swap cells + @ ;

: prefix! ( value index address -- ) swap cells + ! ;

: decimal-digits { n } ( n -- digits ) 1 begin n 10 >= while n 10 / to n 1+ repeat ;

: pow10 { exponent } ( n -- 10^n ) 1 exponent 0 ?do 10 * loop ;

: append-number { a b } ( a b -- ab ) a b decimal-digits pow10 * b + ;

: digit-signature 0 { n bits } ( n -- signature )
  0 to bits begin 1 n 10 mod 5 * lshift bits + to bits
    n 10 / to n n 0= until bits ;

: pandigital-nine? 0 0 0 { n mask digit bit } ( n -- flag )
  0 to mask 9 0 do
    n 10 mod to digit digit 0= if false unloop exit then
    1 digit lshift to bit mask bit and if false unloop exit then
    mask bit or to mask n 10 / to n
  loop n 0= mask 1022 = and ;

: choose { n k } ( n k -- binomial )
  k n k - min to k
  1 k 1+ 1 ?do n k - i + * i / loop ;

: triangle-offset { row column } ( r c -- index )
  row row 1+ * 2 / column + ;

: fixture-integers 0 0 0 0 0 0 { addr u cap raw length values count value active } ( c-addr u capacity -- values count )
  addr u cap 32 * read-fixture to length to raw
  cap cells allocate throw to values
  0 to count 0 to value false to active
  length 0 do raw i + c@ dup [char] 0 [char] 9 1+ within if
    [char] 0 - value 10 * + to value true to active
  else drop active if
    count cap >= abort" Too many fixture numbers"
    value values count cells + ! count 1+ to count
    false to active 0 to value
  then then loop
  active if count cap >= abort" Too many fixture numbers" value values count cells + ! count 1+ to count then
  raw free throw values count ;

: triangle-path-max 0 0 0 0 0 { addr u rows values count answer child row } ( c-addr u rows -- maximum )
  rows rows 1+ * 2 / to count
  addr u count fixture-integers count <> abort" Unexpected triangular input size"
  to values
  rows 1- 0 ?do
    rows 2 - i - to row
    row 1+ 0 do
      row 1+ i triangle-offset values swap cells + @
      row 1+ i 1+ triangle-offset values swap cells + @ max to child
      row i triangle-offset values swap cells + dup @ child + swap !
    loop
  loop
  values @ to answer values free throw answer ;

: bit-count { value } ( n -- population )
  0 begin value 0> while value 1- value and to value 1+ repeat ;

create reciprocal-primes 2 , 3 , 5 , 7 , 11 , 13 , 17 , 19 , 23 , 29 , 31 , 37 , 41 , 43 , 47 ,

variable reciprocal-best

variable reciprocal-target

: reciprocal-search 0 0 { index maxexp value divisor-product next factor } ( index maxexp n divisors -- )
  divisor-product reciprocal-target @ > if
    value reciprocal-best @ min reciprocal-best ! exit
  then
  index 15 >= if exit then
  value to next
  maxexp 1+ 1 do
    reciprocal-best @ 1- index reciprocal-primes prefix@ / next < if leave then
    next index reciprocal-primes prefix@ * to next
    2 i * 1+ to factor
    divisor-product reciprocal-target @ factor / > if
      index 1+ i next reciprocal-target @ 1+ recurse
    else
      index 1+ i next divisor-product factor * recurse
    then
  loop ;

: reciprocal-bound 0 { wanted divisors } ( solutions -- minimum n )
  wanted 2* reciprocal-target ! 1 reciprocal-best !
  1 to divisors
  15 0 do
    divisors reciprocal-target @ > if leave then
    reciprocal-best @ i reciprocal-primes prefix@ * reciprocal-best !
    divisors 3 * to divisors
  loop
  0 63 1 1 reciprocal-search reciprocal-best @ ;
