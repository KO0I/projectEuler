\ Project Euler 92: Square Digit Chains.
require math.fs

create squared-digits 0 , 1 , 4 , 9 , 16 , 25 , 36 , 49 , 64 , 81 ,

: digit-squares 0 { n result } ( n -- sum )
  0 to result
  begin n 10 mod cells squared-digits + @ result + to result n 10 / to n
    n 0= if result exit then
  again ;

: chain-end { n } ( n -- 1|89 )
  begin n 1 <> n 89 <> and while n digit-squares to n repeat n ;

create ends-at-89 568 allot

: p92 ( -- answer )
  ends-at-89 568 erase
  568 1 do i chain-end 89 = if 1 ends-at-89 i + c! then loop
  0 { count }
  10000000 1 do i digit-squares ends-at-89 + c@ 0<> if count 1+ to count then loop count ;

[undefined] euler-testing [if] p92 . cr bye [then]
