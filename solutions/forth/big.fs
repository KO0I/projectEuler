\ Base-10000 dynamically allocated signed big integers for Gforth.
\ Layout: [sign] [length] [capacity] [address of cell limbs].
\ Limbs are little-endian. Zero is represented by sign=0, length=0.
\ This is a reusable Forth implementation, not a GMP/FFI wrapper.
\ Gforth and Forth-2012 locals syntax { ... } is used.

10000 constant big-base
: bsign@ ( b -- n ) @ ;
: bsign! ( n b -- ) ! ;
: blen@ ( b -- u ) cell+ @ ;
: blen! ( u b -- ) cell+ ! ;
: bcap@ ( b -- u ) 2 cells + @ ;
: bcap! ( u b -- ) 2 cells + ! ;
: bptr@ ( b -- a ) 3 cells + @ ;
: bptr! ( a b -- ) 3 cells + ! ;

: bnew 0 0 { requested b p } ( u -- b )
  4 cells allocate throw to b
  requested 1 max cells allocate throw to p
  0 b bsign!  0 b blen!
  requested 1 max b bcap!  p b bptr!
  b ;
: bfree { b } ( b -- ) b bptr@ free throw b free throw ;
: bclear { b } ( b -- ) 0 b bsign! 0 b blen! ;
: blimb@ { index b } ( i b -- u )
  index b blen@ < if b bptr@ index cells + @ else 0 then ;
: blimb! { value index b } ( u i b -- )
  value b bptr@ index cells + ! ;
: bensure 0 0 { needed b newcap p } ( u b -- )
  b bcap@ to newcap
  begin newcap needed < while newcap 2* to newcap repeat
  newcap b bcap@ > if
    b bptr@ newcap cells resize throw to p
    p b bptr!  newcap b bcap!
  then ;
: bnormalize { b } ( b -- )
  begin
    b blen@ 0> if b blen@ 1- b blimb@ 0= else false then
  while b blen@ 1- b blen! repeat
  b blen@ 0= if 0 b bsign! then ;
: bset-u { n b } ( u b -- )
  b bclear
  begin n 0> while
    b blen@ 1+ b bensure
    n big-base mod b blen@ b blimb!
    b blen@ 1+ b blen!
    n big-base / to n
  repeat
  b blen@ 0> if 1 b bsign! then ;
: bcopy { a b } ( a b -- )
  a blen@ b bensure
  a blen@ 0 ?do i a blimb@ i b blimb! loop
  a blen@ b blen! a bsign@ b bsign! ;
: babscompare 0 0 { a b v w } ( a b -- -1|0|1 )
  a blen@ b blen@ < if -1 exit then
  a blen@ b blen@ > if 1 exit then
  a blen@ 0 ?do
    a blen@ 1- i - a blimb@ to v
    b blen@ 1- i - b blimb@ to w
    v w < if -1 exit then
    v w > if 1 exit then
  loop 0 ;
: baddabs! 0 0 0 { a b carry value size } ( a b -- )
  a blen@ b blen@ max to size
  size 1+ a bensure
  0 to carry
  size 0 ?do
    i a blimb@ i b blimb@ + carry + to value
    value big-base mod i a blimb!
    value big-base / to carry
  loop
  carry 0> if carry size a blimb! size 1+ a blen!
  else size a blen! then
  a blen@ 0> if 1 a bsign! else 0 a bsign! then ;
: bsubabs! 0 0 { a b borrow value } ( a b -- )
  \ Preconditions: |a| >= |b|; this subtracts unsigned magnitudes.
  a b babscompare 0< abort" bsubabs: negative magnitude"
  0 to borrow
  a blen@ 0 ?do
    i a blimb@ i b blimb@ - borrow - to value
    value 0< if value big-base + to value 1 to borrow
    else 0 to borrow then
    value i a blimb!
  loop
  borrow 0<> abort" bsubabs: borrow"
  a bnormalize ;
: badd 0 { a b out relation } ( a b out -- )
  \ Output must not alias the two inputs.
  a bsign@ b bsign@ = if
    a out bcopy out b baddabs!
    out blen@ 0> if a bsign@ out bsign! then
  else
    a b babscompare to relation
    relation 0= if out bclear exit then
    relation 0> if a out bcopy out b bsubabs!
    else b out bcopy out a bsubabs! then
  then ;
: bmul-small! 0 0 { factor b carry value } ( u b -- )
  factor 0= if b bclear exit then
  0 to carry
  b blen@ 0 ?do
    i b blimb@ factor * carry + to value
    value big-base mod i b blimb!
    value big-base / to carry
  loop
  begin carry 0> while
    b blen@ 1+ b bensure
    carry big-base mod b blen@ b blimb!
    carry big-base / to carry
    b blen@ 1+ b blen!
  repeat ;
: bmul 0 0 0 0 { a b out carry value index limit } ( a b out -- )
  \ Output must not alias the two inputs.
  a blen@ b blen@ + to limit
  limit 1+ out bensure
  limit 1+ 0 ?do 0 i out blimb! loop
  limit out blen!
  a blen@ 0 ?do
    0 to carry
    b blen@ 0 ?do
      j i + to index
      j a blimb@ i b blimb@ * index out blimb@ + carry + to value
      value big-base mod index out blimb!
      value big-base / to carry
    loop
    carry i b blen@ + out blimb!
  loop
  a bsign@ b bsign@ * 0< if -1 else 1 then out bsign!
  out bnormalize ;
: bdiv-small! 0 0 0 { divisor b remainder value index } ( u b -- r )
  divisor 0= abort" division by zero"
  0 to remainder
  b blen@ 0 ?do
    b blen@ 1- i - to index
    remainder big-base * index b blimb@ + to value
    value divisor / index b blimb!
    value divisor mod to remainder
  loop
  b bnormalize
  remainder ;
: badd-small! 0 { small b temp } ( u b -- )
  small 0= if exit then
  1 bnew to temp
  small temp bset-u
  b temp baddabs!
  temp bfree ;
: bfrom-dec { addr count b } ( c-addr u b -- )
  b bclear
  count 0 ?do
    addr i + c@ dup [char] 0 < over [char] 9 > or
      abort" nondecimal input"
    [char] 0 -
    10 b bmul-small! b badd-small!
  loop ;
: bdecimal-length 0 0 { b n digits } ( b -- u )
  b blen@ 0= if 1 exit then
  b blen@ 1- b blimb@ to n
  0 to digits
  begin n 0> while n 10 / to n digits 1+ to digits repeat
  b blen@ 1- 4 * digits + ;
: bdigitsum 0 0 { b result n } ( b -- u )
  0 to result
  b blen@ 0 ?do
    i b blimb@ to n
    4 0 do n 10 mod result + to result n 10 / to n loop
  loop result ;
: .padded4 { n } ( 0..9999 -- )
  n 1000 < if [char] 0 emit then
  n 100  < if [char] 0 emit then
  n 10   < if [char] 0 emit then
  n 0 .r ;
: bprint { b } ( b -- )
  b blen@ 0= if [char] 0 emit exit then
  b bsign@ 0< if [char] - emit then
  b blen@ 1- b blimb@ 0 .r
  b blen@ 1- 0 ?do
    b blen@ 2 - i - b blimb@ .padded4
  loop ;
