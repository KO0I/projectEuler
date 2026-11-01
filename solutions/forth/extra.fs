\ Shared general-purpose helpers for the additional Gforth solutions.
\ Included by solvers.fs after the existing 37 solutions; 64-bit cells required.
: decimal-digits { n } ( n -- digits ) 1 begin n 10 >= while n 10 / to n 1+ repeat ;
: pow10 { exponent } ( n -- 10^n ) 1 exponent 0 ?do 10 * loop ;
: append-number { a b } ( a b -- ab ) a b decimal-digits pow10 * b + ;
: palindrome-base? 0 0 { n base reversed x } ( n base -- flag )
  n to x 0 to reversed
  begin reversed base * x base mod + to reversed x base / to x x 0= until
  reversed n = ;
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
: p15 ( -- n )
  21 cells allocate throw { ways }
  21 0 do 1 ways i cells + ! loop
  20 0 do 21 1 do ways i 1- cells + @ ways i cells + +! loop loop
  20 ways prefix@ ways free throw ;
: p18 ( -- n ) s" tests/data/p018_triangle.txt" 15 triangle-path-max ;
: p23 ( -- n )
  28124 allocate throw 28124 cells allocate throw 0 0 0
  { marked abundant count answer sum }
  marked 28124 erase
  28124 1 do i proper-divisor-sum i > if
    i abundant count cells + ! count 1+ to count
  then loop
  count 0 do
    count i do
      i abundant prefix@ j abundant prefix@ + to sum
      sum 28123 > if leave then
      1 marked sum + c!
    loop
  loop
  28124 1 do marked i + c@ 0= if answer i + to answer then loop
  marked free throw abundant free throw answer ;
: p24 ( -- n )
  10 cells allocate throw 999999 0 0 0 { digits rank result index remaining }
  10 0 do i digits i cells + ! loop
  10 0 do
    10 i - to remaining
    rank remaining 1- factorial / to index
    rank remaining 1- factorial mod to rank
    result 10 * index digits prefix@ + to result
    remaining 1- index ?do
      i 1+ digits prefix@ i digits prefix!
    loop
  loop digits free throw result ;
: p26 ( -- n )
  1000 cells allocate throw 0 0 0 0 0 { seen best answer remainder position cycle }
  1000 2 do
    i 0 do -1 i seen prefix! loop
    1 i mod to remainder 0 to position
    begin remainder 0<> if remainder seen prefix@ -1 = else false then while
      position remainder seen prefix!
      position 1+ to position
      remainder 10 * i mod to remainder
    repeat
    remainder 0= if 0 else position remainder seen prefix@ - then to cycle
    cycle best > if cycle to best i to answer then
  loop seen free throw answer ;
create p31-coins 1 , 2 , 5 , 10 , 20 , 50 , 100 , 200 ,
: p31 ( -- n )
  201 cells allocate throw 0 0 { ways coin result }
  ways 201 cells erase 1 ways !
  8 0 do i p31-coins prefix@ to coin
    201 coin do i coin - ways prefix@ i ways cells + +! loop
  loop
  200 ways prefix@ to result ways free throw result ;
: digit-cancel? 0 0 0 0 0 { numerator denominator nt no dt du valid } ( n d -- flag )
  numerator 10 / to nt numerator 10 mod to no
  denominator 10 / to dt denominator 10 mod to du
  false to valid
  nt dt = nt 0<> and no denominator * numerator du * = and if true to valid then
  nt du = nt 0<> and no denominator * numerator dt * = and if true to valid then
  no dt = no 0<> and nt denominator * numerator du * = and if true to valid then
  no du = no 0<> and nt denominator * numerator dt * = and if true to valid then
  valid ;
: p33 ( -- n ) 1 1 { product-n product-d }
  100 10 do
    100 i 1+ do
      j i digit-cancel? if product-n j * to product-n product-d i * to product-d then
    loop
  loop product-n product-d integer-gcd product-d swap / ;
: p36 ( -- n ) 0 { total }
  1000000 1 do i 10 palindrome-base? i 2 palindrome-base? and if total i + to total then 2 +loop total ;
: right-truncatable? { n } ( n -- flag )
  begin n 0> while n is-prime? 0= if false exit then n 10 / to n repeat true ;
: left-truncatable? 0 { n power } ( n -- flag )
  1 to power begin power n 10 / <= while power 10 * to power repeat
  begin power 1 > while
    n power mod to n n is-prime? 0= if false exit then
    power 10 / to power
  repeat true ;
: p37 ( -- n ) 0 0 { total count }
  1000000 11 do
    i right-truncatable? i left-truncatable? and if total i + to total count 1+ to count then
    count 11 = if total unloop exit then
  2 +loop abort" Not all truncatable primes found" ;
: p38 ( -- n ) 0 0 0 0 { best joined multiplier base product }
  10000 1 do
    0 to joined 1 to multiplier
    begin joined decimal-digits 9 < while
      i multiplier * to product
      joined product append-number to joined
      multiplier 1+ to multiplier
    repeat
    multiplier 2 > joined 100000000 >= and joined 999999999 <= and if
      joined pandigital-nine? if best joined max to best then
    then
  loop best ;
: p42 ( -- n )
  s" tests/data/p042_words.txt" 100000 read-fixture
  0 0 0 0 { raw length score count ch root }
  0 to score 0 to count
  length 0 do raw i + c@ to ch
    ch [char] A >= ch [char] Z <= and if
      score ch [char] A - 1+ + to score
    else ch 34 = score 0> and if
      score 8 * 1+ integer-sqrt to root
      root root * score 8 * 1+ = if count 1+ to count then
      0 to score
    then then
  loop raw free throw count ;
: p45 ( -- n ) 144 { index }
  begin index index 2* 1- * dup pentagonal? if exit then drop index 1+ to index again ;
: p49 ( -- n )
  9999 sieve-build 0 0 0 0 { primes b c answer sig }
  10000 1000 do primes i + c@ if
    i digit-signature to sig
    10000 i 1+ ?do
      j 2* i - to c
      c 10000 >= if leave then
      primes j + c@ primes c + c@ and if
        i 1487 = j 4817 = and 0= if
          sig j digit-signature = sig c digit-signature = and if
            i 100000000 * j 10000 * + c + to answer
            primes free throw answer unloop unloop exit
          then
        then
      then
    loop
  then loop primes free throw answer ;
: p52 ( -- n ) 1 9 0 0 { lower upper signature candidate }
  begin upper 6 / 1+ lower ?do
      i digit-signature to signature
      true 7 2 do signature i j * digit-signature = and loop
      if i unloop exit then
    loop upper 1+ to lower upper 10 * 9 + to upper
  again ;
: p53 ( -- n )
  101 cells allocate throw 0 0 { row count value }
  row 101 cells erase 1 row !
  101 1 do
    i 1+ 1 do
      i j - 1+ to value
      value row prefix@ value 1- row prefix@ + 1000001 min
      value row prefix!
    loop
    i 1+ 0 do i row prefix@ 1000000 > if count 1+ to count then loop
  loop row free throw count ;
: p58 ( -- n ) 0 0 0 { prime-count square step }
  3 begin
    dup dup * to square dup 1- to step
    4 1 do square i step * - is-prime? if prime-count 1+ to prime-count then loop
    prime-count 10 * over 2* 1- < if exit then
    2 +
  again ;
: p63 ( -- n )
  4 bnew 0 0 { value total exponent }
  10 1 do 1 value bset-u
    25 1 do j value bmul-small!
      value bdecimal-length i = if total 1+ to total then
      value bdecimal-length i < if leave then
    loop
  loop value bfree total ;
: p64 ( -- n ) 0 0 0 0 0 { total root m d a period }
  10001 2 do i integer-sqrt to root
    root root * i <> if
      0 to m 1 to d root to a 0 to period
      begin d a * m - to m i m m * - d / to d
        root m + d / to a period 1+ to period
        a root 2* = until
      period 2 mod if total 1+ to total then
    then
  loop total ;
: p67 ( -- n ) s" tests/data/p067_triangle.txt" 100 triangle-path-max ;
: p71 ( -- n ) 0 1 0 { best-n best-d numerator }
  1000001 2 do
    i 3 * 1- 7 / to numerator
    numerator best-d * best-n i * > if numerator to best-n i to best-d then
  loop best-n ;
: p72 ( -- n )
  1000001 cells allocate throw 0 { phi result }
  1000001 0 do i phi i prefix! loop
  1000001 2 do
    i phi prefix@ i = if
      1000001 i do
        i phi prefix@ dup j / - i phi prefix!
      j +loop
    then
  loop
  1000001 2 do result i phi prefix@ + to result loop
  phi free throw result ;
: p73-count { a b c d } ( a/b c/d -- count )
  b d + 12000 > if 0 exit then
  a b a c + b d + recurse
  a c + b d + c d recurse + 1+ ;
: p73 ( -- n ) 1 3 1 2 p73-count ;
: p75 ( -- n )
  1500001 allocate throw 0 0 0 0 { counts answer perimeter m n }
  counts 1500001 erase
  867 2 do i to m
    m 1 do i to n
      m n - 2 mod 0<> m n integer-gcd 1 = and if
        m m n + * 2* to perimeter
        perimeter 1500000 <= if
          1500001 perimeter do counts i + dup c@ 1+ 2 min swap c! perimeter +loop
        then
      then
    loop
  loop
  1500001 1 do counts i + c@ 1 = if answer 1+ to answer then loop
  counts free throw answer ;
: p76 ( -- n )
  101 cells allocate throw 0 { ways answer }
  ways 101 cells erase 1 ways !
  100 1 do
    101 i do i j - ways prefix@ i ways cells + +! loop
  loop
  100 ways prefix@ to answer ways free throw answer ;
: p77 ( -- n ) 128 cells allocate throw 2 { ways n }
  2 to n begin ways 128 cells erase 1 ways !
    n 1+ 2 do i is-prime? if
      n 1+ i do i j - ways prefix@ i ways cells + +! loop
    then loop
    n ways prefix@ 5000 > if ways free throw n exit then
    n 1+ to n
  again ;
: p78 ( -- n )
  100001 cells allocate throw 0 0 0 0 0 { ways value k first second sign }
  ways 100001 cells erase 1 ways !
  100000 1 do
    0 to value 1 to k
    begin k 3 k * 1- * 2 / to first
      first i <= while
      first k + to second
      k 2 mod if 1 else -1 then to sign
      i first - ways prefix@ sign * value + to value
      second i <= if i second - ways prefix@ sign * value + to value then
      k 1+ to k
    repeat
    value 1000000 mod dup 0< if 1000000 + then dup i ways prefix! 0= if
      ways free throw i unloop exit
    then
  loop ways free throw -1 ;
: p85 ( -- n ) 2000000 0 0 0 0 { closest best width height rectangles diff }
  2001 1 do i to width
    width 1+ 1 do i to height
      width width 1+ * height height 1+ * * 4 / to rectangles
      rectangles 2000000 - abs to diff
      diff closest < if diff to closest width height * to best then
      rectangles 2100000 > if leave then
    loop
  loop best ;
: p86 ( -- n ) 0 0 0 0 0 { total largest squared root low high }
  1 to largest
  begin largest 2* 1+ 2 do
      largest largest * i i * + to squared
      squared integer-sqrt to root
      root root * squared = if
        i largest > if i largest - else 1 then to low
        i 2 / largest min to high
        high low >= if total high low - 1+ + to total then
      then
    loop
    total 1000000 > if largest exit then
    largest 1+ to largest
  again ;
: p90-valid? 0 0 0 { a b x y squares } ( mask mask -- flag )
  1 6 lshift 1 9 lshift or to squares
  a squares and if a squares or to a then
  b squares and if b squares or to b then
  10 1 do
    i i * to x
    1 x 10 / lshift to y
    1 x 10 mod lshift to x
    a x and 0<> b y and 0<> and
    a y and 0<> b x and 0<> and or 0= if false unloop exit then
  loop true ;
: bit-count { value } ( n -- population )
  0 begin value 0> while value 1- value and to value 1+ repeat ;
: p90 ( -- n )
  210 cells allocate throw 0 0 { masks count answer }
  1024 0 do i bit-count 6 = if i masks count prefix! count 1+ to count then loop
  count 0 do count i do
    i masks prefix@ j masks prefix@ p90-valid? if answer 1+ to answer then
  loop loop masks free throw answer ;
: p91 ( -- n ) 0 0 0 0 0 0 0 0 { answer x1 y1 x2 y2 dot0 dot1 dot2 }
  51 0 do i to x1
    51 0 do i to y1
      x1 0<> y1 0<> or if
        51 0 do i to x2
          51 0 do i to y2
            x2 0<> y2 0<> or x1 x2 <> y1 y2 <> or and if
              x1 x2 * y1 y2 * + to dot0
              x1 x1 x2 - * y1 y1 y2 - * + to dot1
              x2 x2 x1 - * y2 y2 y1 - * + to dot2
              dot0 0= dot1 0= or dot2 0= or if answer 1+ to answer then
            then
          loop
        loop
      then
    loop
  loop answer 2 / ;
: p97 ( -- n ) 2 7830457 10000000000 power-mod 28433 * 1+ 10000000000 mod ;
: p100 ( -- n ) 15 21 0 0 { blue total next-blue next-total }
  begin total 1000000000000 <= while
    3 blue * 2 total * + 2 - to next-blue
    4 blue * 3 total * + 3 - to next-total
    next-blue to blue next-total to total
  repeat blue ;
: p106 ( -- n ) 0 0 0 { total middle catalan }
  7 2 do
    i 2* i choose to middle
    middle i 1+ / to catalan
    12 i 2* choose middle 2 / catalan - * total + to total
  loop total ;
: p112-bouncy? 0 0 0 0 { n last increasing decreasing digit }
  true to increasing true to decreasing
  n 10 mod to last n 10 / to n
  begin n 0> while
    n 10 mod to digit
    digit last > if decreasing false to decreasing then
    digit last < if increasing false to increasing then
    digit to last n 10 / to n
  repeat increasing 0= decreasing 0= and ;
: p112 ( -- n ) 0 100 { count n }
  begin n p112-bouncy? if count 1+ to count then
    count 100 * n 99 * = if n exit then
    n 1+ to n
  again ;
: p113 ( -- n ) 110 10 choose 109 9 choose + 1002 - ;
: p114 ( -- n )
  51 cells allocate throw 0 { ways result }
  ways 51 cells erase 1 ways !
  51 1 do
    i 1- ways prefix@ i ways prefix!
    i 3 >= if i 1+ 3 do
      i j = if 1 else i j - 1- ways prefix@ then
      j ways cells + +!
    loop then
  loop
  50 ways prefix@ to result ways free throw result ;
: p115 ( -- n )
  256 cells allocate throw 0 0 { ways result index }
  ways 256 cells erase 1 ways !
  256 1 do
    i 1- ways prefix@ to result
    i 50 >= if i 1+ 50 do
      i j = if 1 else i j - 1- ways prefix@ then
      result + to result
    loop then
    result i ways prefix!
    result 1000000 > if ways free throw i unloop exit then
  loop ways free throw -1 ;
: p116 ( -- n )
  51 cells allocate throw 0 0 0 { ways total length result }
  5 2 do
    i to length ways 51 cells erase 1 ways !
    51 1 do
      i 1- ways prefix@ to result
      i length >= if i length - ways prefix@ result + to result then
      result i ways prefix!
    loop
    50 ways prefix@ 1- total + to total
  loop ways free throw total ;
: p117 ( -- n )
  51 cells allocate throw 0 { ways result }
  ways 51 cells erase 1 ways !
  51 1 do
    5 1 do j i >= if j i - ways prefix@ j ways cells + +! then loop
  loop
  50 ways prefix@ to result ways free throw result ;
: p120 ( -- n ) 0 { total }
  1001 3 do
    i 2 mod if i i 1- * else i i 2 - * then total + to total
  loop total ;

\ Further reusable enumerators and exact integer recurrences.
: p32 ( -- n )
  10000 allocate throw 0 0 0 { seen total product joined }
  seen 10000 erase
  100 1 do
    10000 i / 1+ i 1+ do
      i j * to product
      j i append-number product append-number to joined
      joined 100000000 >= joined 999999999 <= and if
        joined pandigital-nine? seen product + c@ 0= and if
          1 seen product + c! total product + to total
        then
      then
    loop
  loop seen free throw total ;
: pandigital-seven? 0 0 0 { n mask digit bit } ( n -- flag )
  0 to mask 7 0 do
    n 10 mod to digit digit 1 < digit 7 > or if false unloop exit then
    1 digit lshift to bit mask bit and if false unloop exit then
    mask bit or to mask n 10 / to n
  loop n 0= mask 254 = and ;
: p41 ( -- n ) 7654321 { number }
  begin number 1234567 >= while
    number pandigital-seven? if number is-prime? if number exit then then
    number 2 - to number
  repeat abort" No 7-pandigital prime" ;
: p43-search 0 0 { position used value total digit } ( position used number -- sum )
  position 10 = if value exit then
  10 0 do
    i to digit
    position 0<> digit 0<> or
    1 digit lshift used and 0= and if
      value 10 * digit +
      position 3 >= if
        dup 1000 mod
        position 3 - case
          0 of 2 endof 1 of 3 endof 2 of 5 endof
          3 of 7 endof 4 of 11 endof 5 of 13 endof 6 of 17 endof
        endcase mod 0<>
      else false then
      if drop else
        position 1+ used 1 digit lshift or swap recurse total + to total
      then
    then
  loop total ;
: p43 ( -- n ) 0 0 0 p43-search ;
: p56 ( -- n )
  8 bnew 0 { value best }
  100 1 do
    1 value bset-u
    100 1 do
      j value bmul-small!
      value bdigitsum best max to best
    loop
  loop value bfree best ;
: p57 ( -- n )
  8 bnew 8 bnew 8 bnew 8 bnew 0 { top bottom next temp total }
  3 top bset-u 2 bottom bset-u
  1000 0 do
    top bdecimal-length bottom bdecimal-length > if total 1+ to total then
    bottom temp bcopy 2 temp bmul-small!
    top temp next badd
    top bottom temp badd
    temp bottom bcopy next top bcopy
  loop
  top bfree bottom bfree next bfree temp bfree total ;
: p65 ( -- n )
  8 bnew 8 bnew 8 bnew 8 bnew 0 0 { numerator previous next temp a k }
  1 numerator bset-u 0 previous bset-u
  100 0 do
    99 i - to k
    k 0= if 2 to a else
      k 3 mod 2 = if k 3 / 1+ 2* to a else 1 to a then
    then
    numerator temp bcopy a temp bmul-small!
    temp previous next badd
    numerator previous bcopy next numerator bcopy
  loop
  numerator bdigitsum to a
  numerator bfree previous bfree next bfree temp bfree a ;
: p79 ( -- n )
  s" tests/data/p079_keylog.txt" 150 fixture-integers
  100 allocate throw 10 allocate throw 10 allocate throw
  0 0 0 0 0 0 0 0 { triples count edges present emitted total next answer triple first second third available }
  edges 100 erase present 10 erase emitted 10 erase
  count 0 do
    i triples prefix@ to triple
    triple 100 / to first
    triple 10 / 10 mod to second
    triple 10 mod to third
    1 present first + c! 1 present second + c! 1 present third + c!
    1 edges first 10 * second + + c!
    1 edges second 10 * third + + c!
  loop
  10 0 do present i + c@ if total 1+ to total then loop
  total 0 do
    -1 to next
    10 0 do
      present i + c@ emitted i + c@ 0= and if
        true to available
        10 0 do
          edges i 10 * j + + c@ emitted i + c@ 0= and if false to available then
        loop
        available if i to next leave then
      then
    loop
    next 0< abort" Cyclic passcode constraints"
    answer 10 * next + to answer
    1 emitted next + c!
  loop
  triples free throw edges free throw present free throw emitted free throw answer ;
: p81 ( -- n )
  s" tests/data/p081_matrix.txt" 6400 fixture-integers
  80 cells allocate throw 0 0 0 { matrix count costs row col answer }
  count 6400 <> abort" Wrong matrix size"
  costs 80 cells erase
  80 0 do i to row
    80 0 do i to col
      row 0= col 0> and if col 1- costs prefix@ col costs prefix! then
      row 0> col 0> and if
        col costs prefix@ col 1- costs prefix@ min col costs prefix!
      then
      row 80 * col + matrix prefix@ col costs cells + +!
    loop
  loop
  79 costs prefix@ to answer
  matrix free throw costs free throw answer ;
: p82 ( -- n )
  s" tests/data/p082_matrix.txt" 6400 fixture-integers
  80 cells allocate throw 0 0 0 { matrix count costs column row answer }
  count 6400 <> abort" Wrong matrix size"
  80 0 do i 80 * matrix prefix@ i costs prefix! loop
  80 1 do i to column
    80 0 do
      i 80 * column + matrix prefix@ i costs cells + +!
    loop
    80 1 do
      i 1- costs prefix@ i 80 * column + matrix prefix@ +
      i costs prefix@ min i costs prefix!
    loop
    79 0 do
      78 i - to row
      row 1+ costs prefix@ row 80 * column + matrix prefix@ +
      row costs prefix@ min row costs prefix!
    loop
  loop
  0 costs prefix@ to answer
  80 1 do answer i costs prefix@ min to answer loop
  matrix free throw costs free throw answer ;
: p94 ( -- n ) 2 0 0 0 0 0 0 { x y nx ny perimeter a b sum }
  begin x 1000000002 <= while
    x 3 mod 2 = if x 1+ 3 / to a a 1+ to b x 2 + to perimeter
    else x 1- 3 / to a a 1- to b x 2 - to perimeter then
    a 1 > b 0> and perimeter 1000000000 <= and if
      b y * 4 mod 0= if sum perimeter + to sum then
    then
    x 2* y 3 * + to nx
    x y 2* + to ny
    nx to x ny to y
  repeat sum ;
: p101-sequence { n } ( n -- polynomial )
  1 10 0 do n * 1 swap - loop ;
: p101-fit 0 0 0 { differences terms at sum coefficient } ( ptr terms at -- value )
  0 to sum 1 to coefficient
  terms 0 do
    i differences prefix@ coefficient * sum + to sum
    i 1+ terms < if coefficient at 1- i - * i 1+ / to coefficient then
  loop sum ;
: p101 ( -- n )
  10 cells allocate throw 10 cells allocate throw
  0 0 0 { differences first total k at }
  11 1 do i to k
    k 0 do i 1+ p101-sequence i differences prefix! loop
    k 0 do
      0 differences prefix@ i first prefix!
      k i - 1- 0 ?do
        j 1+ differences prefix@ j differences prefix@ - j differences prefix!
      loop
    loop
    k 1+ to at
    begin first k at p101-fit at p101-sequence = while at 1+ to at repeat
    first k at p101-fit total + to total
  loop
  differences free throw first free throw total ;
create p108-primes 2 , 3 , 5 , 7 , 11 , 13 , 17 , 19 , 23 , 29 , 31 , 37 , 41 , 43 , 47 ,
variable p108-best
variable p108-target
: p108-search 0 0 { index maxexp value divisor-product next factor } ( index maxexp n divisors -- )
  divisor-product p108-target @ > if
    value p108-best @ min p108-best ! exit
  then
  index 15 >= if exit then
  value to next
  maxexp 1+ 1 do
    p108-best @ 1- index p108-primes prefix@ / next < if leave then
    next index p108-primes prefix@ * to next
    2 i * 1+ to factor
    divisor-product p108-target @ factor / > if
      index 1+ i next p108-target @ 1+ recurse
    else
      index 1+ i next divisor-product factor * recurse
    then
  loop ;
: p108-bound 0 { wanted divisors } ( solutions -- minimum n )
  wanted 2* p108-target ! 1 p108-best !
  1 to divisors
  15 0 do
    divisors p108-target @ > if leave then
    p108-best @ i p108-primes prefix@ * p108-best !
    divisors 3 * to divisors
  loop
  0 63 1 1 p108-search p108-best @ ;
: p108 ( -- n ) 1000 p108-bound ;
: p110 ( -- n ) 4000000 p108-bound ;
: p119-digit-sum { number } ( n -- sum )
  0 begin number 0> while number 10 mod + number 10 / to number repeat ;
: p119-sort 0 0 { values count value position } ( array n -- )
  count 1 do
    i values prefix@ to value
    i to position
    begin position 0> if position 1- values prefix@ value > else false then while
      position 1- values prefix@ position values prefix!
      position 1- to position
    repeat
    value position values prefix!
  loop ;
: p119 ( -- n )
  12000 cells allocate throw 0 100 0 0 0 0
  { values count limit base value result digit-count }
  16 2 do
    i to digit-count
    0 to count
    digit-count 9 * 1+ 2 do
      i to base base base * to value
      begin value limit < while
        value 10 >= if value p119-digit-sum base = if
          count 12000 >= abort" Digit-power candidates capacity exceeded"
          value values count prefix! count 1+ to count
        then then
        value limit 1- base / > if limit to value
        else value base * to value then
      repeat
    loop
    count 30 >= if
      values count p119-sort
      0 to result
      count 0 do
        i 0= if true else
          i values prefix@ i 1- values prefix@ <> then
        if result 1+ to result then
        result 30 = if i values prefix@ values free throw unloop unloop exit then
      loop
    then
    limit 10 * to limit
  loop values free throw abort" Missing digit-power term" ;
