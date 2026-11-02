\ Project Euler 111: Primes with Runs.
require math.fs

variable p111-count variable p111-sum

: p111-replace { repeat-digit position remaining value } ( repeat-digit position remaining value -- )
  remaining 10 position - > if exit then
  position 10 = if
    remaining 0= if value is-prime? if
      p111-count @ 1+ p111-count !
      value p111-sum @ + p111-sum !
    then then
    exit
  then
  repeat-digit 0<> position 0<> or if
    repeat-digit position 1+ remaining value 10 * repeat-digit + recurse
  then
  remaining 0> if
    10 0 do
      i repeat-digit <> position 0<> i 0<> or and if
        repeat-digit position 1+ remaining 1- value 10 * i + recurse
      then
    loop
  then ;

: p111 ( -- n ) 0 0 { result digit }
  10 0 do i to digit
    11 0 do
      0 p111-count ! 0 p111-sum !
      digit 0 i 0 p111-replace
      p111-count @ 0<> if result p111-sum @ + to result leave then
    loop
  loop result ;

[undefined] euler-testing [if] p111 . cr bye [then]
