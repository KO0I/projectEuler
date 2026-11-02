\ Project Euler 51: Prime Digit Replacements.
require math.fs

: p51-family? 0 0 0 0 0 0 0 { number mask primes selected coefficient digit base count first candidate } ( n mask sieve -- flag )
  10 to selected 0 to coefficient
  number decimal-digits 0 do
    mask 1 i lshift and if
      number i pow10 / 10 mod to digit
      selected 10 = if digit to selected else
        selected digit <> if false unloop exit then
      then
      coefficient i pow10 + to coefficient
    then
  loop
  number selected coefficient * - to base
  0 to count 0 to first
  10 0 do
    i 0= mask 1 number decimal-digits 1- lshift and 0<> and 0= if
      base i coefficient * + to candidate
      primes candidate + c@ if
        count 0= if candidate to first then
        count 1+ to count
      then
    then
  loop count 8 = first number = and ;

: p51 ( -- n ) 10 99 0 { lower upper primes }
  10 2 do
    upper sieve-build to primes
    upper 1+ lower do
      primes j + c@ if
        1 j decimal-digits lshift 1 do
          j i primes p51-family? if
            primes free throw j unloop unloop unloop exit
          then
        loop
      then
    loop
    primes free throw
    upper 1+ to lower upper 10 * 9 + to upper
  loop -1 ;

[undefined] euler-testing [if] p51 . cr bye [then]
