\ Further shared Euler solvers (do not duplicate number-theory or fixture utilities).

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

: p62-test 0 { signatures values count copies } ( addresses length -- minimum or zero )
  count 0 do
    0 to copies
    count 0 do
      i signatures prefix@ j signatures prefix@ = if copies 1+ to copies then
    loop
    copies 5 = if i values prefix@ exit then
  loop 0 ;
: p62 ( -- n )
  15000 cells allocate throw 15000 cells allocate throw
  0 10 1 0 0 0 { signatures values count bucket n cube answer }
  begin
    n n * n * to cube
    cube bucket >= if
      signatures values count p62-test to answer
      answer 0<> if signatures free throw values free throw answer exit then
      bucket 10 * to bucket 0 to count
    then
    cube digit-signature count signatures prefix!
    cube count values prefix! count 1+ to count
    n 1+ to n
    count 15000 >= abort" Cubic permutation bucket capacity"
  again ;

: p70 ( -- n )
  10000000 cells allocate throw 0 1 0 { phi best best-phi value }
  10000000 0 do i i phi prefix! loop
  10000000 2 do
    i phi prefix@ i = if
      10000000 i do
        i phi prefix@ dup j / - i phi prefix!
      j +loop
    then
  loop
  10000000 2 do
    i phi prefix@ to value
    i digit-signature value digit-signature = if
      best 0= if i to best value to best-phi else
        i best-phi * best value * < if i to best value to best-phi then
      then
    then
  loop phi free throw best ;

create p74-factorials 1 , 1 , 2 , 6 , 24 , 120 , 720 , 5040 , 40320 , 362880 ,
: p74-next { number } ( n -- sum )
  0 begin number 0> while
    number 10 mod p74-factorials swap cells + @ +
    number 10 / to number
  repeat ;
: p74 ( -- n )
  2540161 cells allocate throw 2540161 cells allocate throw
  2540161 cells allocate throw 256 cells allocate throw
  0 0 0 0 0 0 0 { length visited positions path answer start number count first cycle tail }
  length 2540161 cells erase visited 2540161 cells erase
  1000000 1 do i to start
    start length prefix@ 0= if
      start to number 0 to count
      begin
        number length prefix@ 0=
        number visited prefix@ start <> and
      while
        start number visited prefix!
        count number positions prefix!
        number count path prefix!
        count 1+ to count
        count 256 >= abort" Unexpected digit-chain depth"
        number p74-next to number
      repeat
      number length prefix@ if
        number length prefix@ to tail
      else
        number positions prefix@ to first
        count first - to cycle
        count first do cycle i path prefix@ length prefix! loop
        first to count cycle to tail
      then
      begin count 0> while
        count 1- to count tail 1+ to tail
        tail count path prefix@ length prefix!
      repeat
    then
    start length prefix@ 60 = if answer 1+ to answer then
  loop
  length free throw visited free throw positions free throw path free throw answer ;

: p87 ( -- n )
  7072 sieve-build
  1000 cells allocate throw
  6250000 allocate throw
  0 0 0 0 0 0 0 { primes list seen count answer x y z value mask }
  seen 6250000 erase
  7072 2 do primes i + c@ if i list count prefix! count 1+ to count then loop
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

: p88-recurse { product sum count first minimum } ( product sum count first array -- )
  product sum - count + 12000 > if exit then
  product sum - count + dup 2 >= if
    dup minimum prefix@ product > if product swap minimum prefix! else drop then
  else drop then
  24000 product / 1+ first ?do
    product i * sum i + count 1+ i minimum recurse
  loop ;
: p88 ( -- n )
  12001 cells allocate throw 24001 allocate throw
  0 { minima seen answer }
  12001 0 do 24000 i minima prefix! loop
  seen 24001 erase
  1 0 0 2 minima p88-recurse
  12001 2 do i minima prefix@ dup seen + c@ 0= if
    dup seen + 1 swap c! answer + to answer
  else drop then loop
  minima free throw seen free throw answer ;

: p89-symbol ( ascii -- numeral )
  case
    [char] I of 1 endof [char] V of 5 endof
    [char] X of 10 endof [char] L of 50 endof
    [char] C of 100 endof [char] D of 500 endof
    [char] M of 1000 endof
    abort" Invalid roman glyph"
  endcase ;
create p89-values 1000 , 900 , 500 , 400 , 100 , 90 , 50 , 40 , 10 , 9 , 5 , 4 , 1 ,
create p89-lengths 1 , 2 , 1 , 2 , 1 , 2 , 1 , 2 , 1 , 2 , 1 , 2 , 1 ,
: p89-minimum { value } ( numeral -- minimal count )
  0 13 0 do
    value i p89-values prefix@ / i p89-lengths prefix@ * +
    value i p89-values prefix@ mod to value
  loop ;
: p89 ( -- n )
  s" tests/data/p089_roman.txt" 20000 read-fixture
  0 0 0 0 0 0 { raw length saved before value last current count }
  length 0 do
    i raw + c@ dup 32 > if
      p89-symbol to current
      current last > if value last 2* - to value then
      value current + to value
      current to last before 1+ to before
    else drop
      before 0> if
        before value p89-minimum - saved + to saved
        0 to before 0 to value 0 to last count 1+ to count
      then
    then
  loop
  before 0> if before value p89-minimum - saved + to saved count 1+ to count then
  count 1000 <> abort" Expected 1000 roman entries"
  raw free throw saved ;

: p95 ( -- n )
  1000001 cells allocate throw 1000001 cells allocate throw 1000001 cells allocate throw
  0 0 0 0 0 0 0 0 { successors visited depth best-length best-member start number step minimum first cycle-start }
  successors 1000001 cells erase visited 1000001 cells erase
  500001 1 do
    1000001 i 2* do
      j i successors prefix@ + i successors prefix!
    j +loop
  loop
  1000001 2 do i to start
    start visited prefix@ 0= if
      start to number 0 to step
      begin number 0> number 1000000 <= and if number visited prefix@ 0= else false then while
        start number visited prefix!
        step number depth prefix!
        step 1+ to step
        number successors prefix@ to number
      repeat
      number 0> number 1000000 <= and if number visited prefix@ start = else false then if
        step number depth prefix@ - to first
        number to minimum number to cycle-start
        number successors prefix@ to number
        begin number cycle-start <> while
          number minimum min to minimum
          number successors prefix@ to number
        repeat
        first best-length > if first to best-length minimum to best-member else
          first best-length = minimum best-member < and if minimum to best-member then
        then
      then
    then
  loop
  successors free throw visited free throw depth free throw best-member ;

: signed-fixture 0 0 0 0 0 0 0 { addr u cap raw length numbers count value negative active } ( c-addr u limit -- array count )
  addr u cap 16 * read-fixture to length to raw
  cap cells allocate throw to numbers
  length 0 do
    raw i + c@ dup [char] - = if
      negative 0= active 0= and if -1 to negative then
      drop
    else dup [char] 0 [char] 9 1+ within if
      [char] 0 - value 10 * + to value true to active
    else drop
      active if
        count cap >= abort" Signed fixture too long"
        value negative 0< if negate then count numbers prefix!
        count 1+ to count
        0 to value 0 to negative false to active
      else 0 to negative then
    then then
  loop
  active if value negative 0< if negate then count numbers prefix! count 1+ to count then
  raw free throw numbers count ;
: p102 ( -- n )
  s" tests/data/p102_triangles.txt" 6000 signed-fixture
  0 0 0 0 0 0 0 0 0 0 { coords count answer a b c d e f cross1 cross2 cross3 }
  count 6000 <> abort" Invalid triangle fixture size"
  6000 0 do i 6 mod 0= if
    i coords prefix@ to a i 1+ coords prefix@ to b
    i 2 + coords prefix@ to c i 3 + coords prefix@ to d
    i 4 + coords prefix@ to e i 5 + coords prefix@ to f
    a d * b c * - to cross1
    c f * d e * - to cross2
    e b * f a * - to cross3
    cross1 0> cross2 0> and cross3 0> and
    cross1 0< cross2 0< and cross3 0< and or if
      answer 1+ to answer
    then
  then loop
  coords free throw answer ;

: p105-special? 0 0 0 0 0 0 { values count sums k small large size candidate }
  count 1 do
    i values prefix@ to candidate
    i to size
    begin size 0> if size 1- values prefix@ candidate > else false then while
      size 1- values prefix@ size values prefix!
      size 1- to size
    repeat
    candidate size values prefix!
  loop
  count 1 do
    i 2* 1+ count <= if
      0 to small 0 to large
      i 1+ 0 do i values prefix@ small + to small loop
      i 0 do count 1- i - values prefix@ large + to large loop
      small large <= if false unloop exit then
    then
  loop
  4096 cells allocate throw to sums sums 4096 cells erase
  1 to size
  count 0 do
    size 0 do
      i sums prefix@ j values prefix@ + size i + sums prefix!
    loop
    size 2* to size
  loop
  size 1 do
    i sums prefix@ to candidate
    i to k
    begin k 0> if k 1- sums prefix@ candidate > else false then while
      k 1- sums prefix@ k sums prefix!
      k 1- to k
    repeat
    candidate k sums prefix!
  loop
  size 1 do
    i sums prefix@ i 1- sums prefix@ = if sums free throw false unloop exit then
  loop sums free throw true ;
: p105 ( -- n )
  s" tests/data/p105_sets.txt" 30000 read-fixture
  12 cells allocate throw 0 0 0 0 0 0 { raw length values total count value active element line-sum }
  0 to count 0 to value false to active 0 to total
  length 0 do
    raw i + c@ dup [char] 0 [char] 9 1+ within if
      [char] 0 - value 10 * + to value true to active
    else
      dup to element drop
      active if
        count 12 >= abort" Too many set elements"
        value count values prefix! count 1+ to count
        value line-sum + to line-sum
        0 to value false to active
      then
      element 10 = if
        count 0> if
          values count p105-special? if line-sum total + to total then
          0 to count 0 to line-sum
        then
      then
    then
  loop
  count 0> if values count p105-special? if total line-sum + to total then then
  raw free throw values free throw total ;

: p109 ( -- n )
  62 cells allocate throw 0 0 0 0 0 { scores count total finish last value }
  4 1 do
    21 1 do i j * count scores prefix! count 1+ to count loop
  loop
  25 count scores prefix! 50 count 1+ scores prefix! 62 to count
  22 1 do
    i 21 = if 50 else i 2* then to last
    total 1+ to total
    count 0 do
      i scores prefix@ last + 100 < if total 1+ to total then
      count i do
        i scores prefix@ j scores prefix@ + last + 100 < if total 1+ to total then
      loop
    loop
  loop
  scores free throw total ;

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
