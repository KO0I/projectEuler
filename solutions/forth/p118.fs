\ Project Euler 118: Pandigital Prime Sets.
require math.fs

create p118-prime-count 512 cells allot

create p118-memo 512 cells allot

create p118-known 512 allot

: p118-enumerate { mask value sum } ( mask value digit-sum -- )
  mask 0<> if
    value 2 = value 3 = or value 5 = or value 7 = or
    sum 3 mod 0<> value 2 mod 0<> and value 5 mod 0<> and value is-prime? and or if
      mask p118-prime-count prefix@ 1+ mask p118-prime-count prefix!
    then
  then
  10 1 do
    1 i 1- lshift mask and 0= if
      mask 1 i 1- lshift or value 10 * i + sum i + recurse
    then
  loop ;

: p118-partition 0 0 0 { mask count lowbit subset } ( mask -- ways )
  mask 0= if 1 exit then
  p118-known mask + c@ if mask p118-memo prefix@ exit then
  mask negate mask and to lowbit
  mask to subset
  begin subset 0<> while
    subset lowbit and subset p118-prime-count prefix@ 0<> and if
      subset p118-prime-count prefix@ mask subset xor recurse * count + to count
    then
    subset 1- mask and to subset
  repeat
  1 p118-known mask + c!
  count mask p118-memo prefix! count ;

: p118 ( -- n )
  p118-prime-count 512 cells erase
  p118-memo 512 cells erase
  p118-known 512 erase
  0 0 0 p118-enumerate
  511 p118-partition ;

[undefined] euler-testing [if] p118 . cr bye [then]
