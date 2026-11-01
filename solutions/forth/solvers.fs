\ Project Euler solvers for Gforth; answer is one signed cell (64-bit host).
\ All source files are pure Forth and use the shared big.fs for large integers.
include big.fs

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
    a b mod to temp  b to a  temp to b
  repeat a ;
: decimal-palindrome? 0 0 { n x reversed } ( n -- flag )
  n to x 0 to reversed
  begin x 0> while
    reversed 10 * x 10 mod + to reversed
    x 10 / to x
  repeat reversed n = ;
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
  1 to result
  2 to divisor
  begin divisor n divisor / <= while
    n divisor mod 0= if
      result divisor + to result
      n divisor / divisor <> if result n divisor / + to result then
    then
    divisor 1+ to divisor
  repeat result ;
: factorial 0 { n acc } ( n -- n! )
  1 to acc
  n 2 < if 1 exit then
  n 1+ 2 do acc i * to acc loop acc ;
: digit-pow-sum 0 0 0 { n exponent result x digit } ( n power -- sum )
  n to x 0 to result
  begin
    x 10 mod to digit
    1 exponent 0 ?do digit * loop
    result + to result
    x 10 / to x
    x 0= if exit then
  again ;
: read-fixture 0 0 0 { addr u capacity f buf count } ( caddr u capacity -- buf n )
  addr u r/o open-file throw to f
  capacity allocate throw to buf
  buf capacity f read-file throw to count
  f close-file throw
  buf count ;
: sieve-build 0 0 { limit p j } ( limit -- ptr )
  limit 1+ allocate throw to p
  p limit 1+ 1 fill
  0 p c! 0 p 1+ c!
  limit 1+ 2 do
    i limit i / > if leave then
    p i + c@ 0<> if
      i i * to j
      begin j limit <= while
        0 p j + c!
        j i + to j
      repeat
    then
  loop p ;

: p1 ( -- answer )
  0 1000 0 do
    i 3 mod 0= i 5 mod 0= or if i + then
  loop ;
: p2 ( -- answer )
  1 2 0 0 { a b sum next }
  begin b 4000000 <= while
    b 2 mod 0= if sum b + to sum then
    a b + to next
    b to a  next to b
  repeat sum ;
: p3 ( -- answer )
  600851475143 2 1 { n d last }
  begin d n d / <= while
    n d mod 0= if d to last n d / to n
    else d 2 = if 3 else d 2 + then to d then
  repeat
  n 1 > if n else last then ;
: p4 ( -- answer )
  0 { best }
  1000 100 do
    1000 i do
      i j * dup best > if
        dup decimal-palindrome? if to best else drop then
      else drop then
    loop
  loop best ;
: p5 ( -- answer )
  1 { result }
  21 2 do
    result result i integer-gcd / i * to result
  loop result ;
: p6 ( -- answer )
  100 101 * 2 / dup *
  100 101 * 201 * 6 / - ;
: p7 ( -- answer )
  0 1 { count value }
  begin count 10001 < while
    value 1+ to value
    value is-prime? if count 1+ to count then
  repeat value ;
: p8 ( -- answer )
  s" tests/data/p008.txt" 1200 read-fixture 0 0 0 0 { buffer length digits count best product }
  1000 allocate throw to digits
  0 to count 0 to best
  length 0 do
    buffer i + c@ dup [char] 0 >= over [char] 9 <= and if
      [char] 0 - digits count + c! count 1+ to count
    else drop then
  loop
  count 1000 <> abort" p008 digit fixture must have 1000 digits"
  988 0 do
    1 to product
    13 0 do product digits j i + + c@ * to product loop
    product best > if product to best then
  loop
  buffer free throw digits free throw best ;
: pythagorean-product 0 { a b c } ( a b -- result-or-zero )
  1000 a - b - to c
  c b > a a * b b * + c c * = and if
    a b * c *
  else 0 then ;
: p9 ( -- answer )
  334 1 do
    500 i 1+ do
      j i pythagorean-product dup 0<> if
        unloop unloop exit
      else drop then
    loop
  loop -1 ;
: p10 ( -- answer )
  2000000 sieve-build 0 { bytes answer }
  0 to answer
  2000000 2 do bytes i + c@ 0<> if answer i + to answer then loop
  bytes free throw answer ;
: grid-product 0 0 0 { grid x y dx dy xx yy product } ( grid x y dx dy -- product )
  x dx 3 * + to xx  y dy 3 * + to yy
  xx 0< xx 20 >= or yy 0< or yy 20 >= or if 0 exit then
  1 to product
  4 0 do
    grid y i dy * + 20 * x i dx * + + cells + @ product * to product
  loop product ;
: p11 ( -- answer )
  s" tests/data/p011.txt" 4096 read-fixture
  0 0 0 0 0 { buf length grid count value active best }
  400 cells allocate throw to grid
  0 to count 0 to value false to active
  length 0 do
    buf i + c@ dup [char] 0 >= over [char] 9 <= and if
      [char] 0 - value 10 * + to value  true to active
    else
      drop
      active if value grid count cells + !  count 1+ to count
        0 to value false to active
      then
    then
  loop
  active if value grid count cells + ! count 1+ to count then
  count 400 <> abort" p011 grid must contain 400 numbers"
  0 to best
  20 0 do 20 0 do
    grid i j 1 0 grid-product best max to best
    grid i j 0 1 grid-product best max to best
    grid i j 1 1 grid-product best max to best
    grid i j -1 1 grid-product best max to best
  loop loop
  grid free throw buf free throw best ;
: p12 ( -- answer )
  1 0 { n tri }
  begin
    n n 1+ * 2 / to tri
    tri divisor-count 500 > if tri exit then
    n 1+ to n
  again ;
: bto-u 0 { b result } ( b -- u )
  0 to result
  b blen@ 0 ?do
    result big-base * b blen@ 1- i - b blimb@ + to result
  loop result ;
: p13 ( -- answer )
  s" lsum" r/o open-file throw
  0 0 0 0 0 { f total next line count more }
  16 bnew to total  16 bnew to next
  128 allocate throw to line
  0 to count
  begin
    line 128 f read-line throw to more
    more
  while
    line over next bfrom-dec
    total next baddabs!
    count 1+ to count
    drop
  repeat drop
  f close-file throw line free throw
  count 100 <> abort" p013 must contain 100 numbers"
  begin total bdecimal-length 10 > while
    10 total bdiv-small! drop
  repeat
  total bto-u
  total bfree next bfree ;
: p14 ( -- answer )
  1 0 0 0 { best-start best-length n length }
  1000000 1 do
    i to n  1 to length
    begin n 1 <> while
      n 2 mod 0= if n 2 / to n else n 3 * 1+ to n then
      length 1+ to length
    repeat
    length best-length > if length to best-length i to best-start then
  loop best-start ;
: p16 ( -- answer )
  64 bnew { n }
  1 n bset-u
  1000 0 do 2 n bmul-small! loop
  n bdigitsum  n bfree ;
create letter-small
  0 , 3 , 3 , 5 , 4 , 4 , 3 , 5 , 5 , 4 ,
  3 , 6 , 6 , 8 , 8 , 7 , 7 , 9 , 8 , 8 ,
create letter-tens 0 , 0 , 6 , 6 , 5 , 5 , 5 , 7 , 6 , 6 ,
: small-letters ( n -- count ) cells letter-small + @ ;
: tens-letters ( n -- count ) cells letter-tens + @ ;
: count-letters 0 { n answer } ( n -- count )
  n 1000 = if 11 exit then
  0 to answer
  n 100 >= if
    n 100 / small-letters 7 + answer + to answer
    n 100 mod to n
    n 0<> if answer 3 + to answer then
  then
  n 20 >= if
    n 10 / tens-letters answer + to answer
    n 10 mod to n
  then
  n 0> if n small-letters answer + to answer then
  answer ;
: p17 ( -- answer )
  0 1001 1 do i count-letters + loop ;
: leap? { year } ( year -- flag )
  year 4 mod 0= year 100 mod 0<> year 400 mod 0= or and ;
create month-days 31 , 28 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 ,
: p19 ( -- answer )
  1 0 0 { weekday answer days }
  2001 1900 do
    12 0 do
      j 1901 >= weekday 0= and if answer 1+ to answer then
      month-days i cells + @ to days
      i 1 = j leap? and if days 1+ to days then
      weekday days + 7 mod to weekday
    loop
  loop answer ;
: p20 ( -- answer )
  64 bnew { n }
  1 n bset-u
  101 2 do i n bmul-small! loop
  n bdigitsum n bfree ;
: p21 ( -- answer )
  0 0 { sum pair }
  10000 2 do
    i proper-divisor-sum to pair
    pair i <> pair 0> and if
      pair proper-divisor-sum i = if sum i + to sum then
    then
  loop sum ;
: p25 ( -- answer )
  1 bnew 1 bnew 1 bnew
  0 0 { a b temp old index }
  1 a bset-u 1 b bset-u
  2 to index
  begin b bdecimal-length 1000 < while
    a b temp badd
    a to old
    b to a
    temp to b
    old to temp
    index 1+ to index
  repeat
  a bfree b bfree temp bfree index ;
\ Names scores: in-memory sort of quoted, comma-separated records.
create names-address 5200 cells allot
create names-length 5200 cells allot
: name-addr ( index -- addr ) cells names-address + @ ;
: name-len ( index -- len ) cells names-length + @ ;
: name-compare { a b } ( i j -- -1|0|1 )
  a name-addr a name-len b name-addr b name-len compare ;
: name-swap 0 0 { a b addr len } ( index index -- )
  a name-addr to addr a name-len to len
  b name-addr a cells names-address + !
  b name-len a cells names-length + !
  addr b cells names-address + !
  len b cells names-length + ! ;
: name-sort 0 0 0 0 { lo hi left right pivot-addr pivot-len } ( lo hi -- )
  lo hi >= if exit then
  lo to left  hi to right
  lo hi + 2 / name-addr to pivot-addr
  lo hi + 2 / name-len to pivot-len
  begin left right <= while
    begin
      left hi <= if
        left name-addr left name-len pivot-addr pivot-len compare 0<
      else false then
    while left 1+ to left repeat
    begin
      right lo >= if
        right name-addr right name-len pivot-addr pivot-len compare 0>
      else false then
    while right 1- to right repeat
    left right <= if
      left right name-swap
      left 1+ to left right 1- to right
    then
  repeat
  lo right < if lo right recurse then
  left hi < if left hi recurse then ;
: name-value 0 { addr count sum } ( addr u -- score )
  0 to sum
  count 0 ?do
    addr i + c@ [char] A - 1+ sum + to sum
  loop sum ;
: p22 ( -- answer )
  s" names.txt" 60000 read-fixture
  0 0 0 0 { buf count start inside found sum }
  0 to found false to inside 0 to sum
  count 0 do
    buf i + c@ [char] " = if
      inside if
        buf start + names-address found cells + !
        i start - names-length found cells + !
        found 1+ to found false to inside
      else
        i 1+ to start true to inside
      then
    then
  loop
  found 5163 <> abort" p022 expected 5163 names"
  0 found 1- name-sort
  found 0 do
    i name-addr i name-len name-value i 1+ * sum + to sum
  loop
  buf free throw sum ;

: quadratic-run 0 0 { a b n value } ( a b -- count )
  0 to n
  begin
    n n * a n * + b + to value
    value is-prime?
  while n 1+ to n repeat n ;
: p27 ( -- answer )
  0 0 0 { best product score }
  1000 2 do
    i is-prime? if
      1000 -999 do
        i j quadratic-run to score
        score best > if score to best i j * to product then
      loop
    then
  loop product ;
: p28 ( -- answer )
  1 { result }
  1002 3 do
    i 2 mod 1 = if
      i i * 4 * i 1- 6 * - result + to result
    then
  loop result ;
: primitive-root 0 0 0 0 { n base power exp current-exp } ( n -- root exp )
  n to base 1 to exp
  n 1+ 2 ?do
    i to power
    1 to current-exp
    begin power n < while
      power i * to power current-exp 1+ to current-exp
    repeat
    power n = if i to base current-exp to exp leave then
  loop base exp ;
create power-seen 61000 allot
: p29 ( -- answer )
  power-seen 61000 erase
  0 0 0 0 { count root exp key }
  101 2 do
    i primitive-root to exp to root
    101 2 do
      root 601 * exp i * + to key
      power-seen key + c@ 0= if
        1 power-seen key + c!
        count 1+ to count
      then
    loop
  loop count ;
: p30 ( -- answer )
  0 { sum }
  354295 2 do
    i 5 digit-pow-sum i = if sum i + to sum then
  loop sum ;
: digit-factorial-sum 0 0 { n value result } ( n -- sum )
  n to value 0 to result
  begin
    value 10 mod factorial result + to result
    value 10 / to value
    value 0= if result exit then
  again ;
: p34 ( -- answer )
  0 { sum }
  2540161 3 do
    i digit-factorial-sum i = if sum i + to sum then
  loop sum ;
: p35 ( -- answer )
  1000000 sieve-build 0 0 0 0 { primes count rotated power good }
  0 to count
  1000000 2 do
    primes i + c@ 0<> if
      1 to power
      begin power i 10 / <= while power 10 * to power repeat
      i to rotated true to good
      begin
        rotated 10 / rotated 10 mod power * + to rotated
        primes rotated + c@ 0= if false to good then
        rotated i = good 0= or
      until
      good if count 1+ to count then
    then
  loop
  primes free throw count ;
: p39 ( -- answer )
  0 0 0 0 0 0 0 { best-perimeter best-count count numerator denominator b c }
  1001 12 do
    0 to count
    i 3 / 1 do
      j j i 2 * - * to numerator
      2 j i - * to denominator
      numerator 0> numerator denominator mod 0= and if
        numerator denominator / to b
        j i - b - to c
        i b < b c < and if count 1+ to count then
      then
    loop
    count best-count > if count to best-count i to best-perimeter then
  loop best-perimeter ;
create champernowne-targets 1 , 10 , 100 , 1000 , 10000 , 100000 , 1000000 ,
: p40 ( -- answer )
  0 0 1 1 0 0 { at position product n divisor digit }
  begin at 7 < while
    1 to divisor
    begin divisor n 10 / <= while divisor 10 * to divisor repeat
    begin divisor 0> while
      n divisor / 10 mod to digit
      position 1+ to position
      position champernowne-targets at cells + @ = if
        product digit * to product
        at 1+ to at
        at 7 = if product exit then
      then
      divisor 10 / to divisor
    repeat
    n 1+ to n
  repeat product ;
: integer-sqrt 0 0 { n x y } ( nonnegative n -- floor(sqrt(n)) )
  n 0= if 0 exit then
  n to x
  begin
    x n x / + 2 / to y
    y x >= if x exit then
    y to x
  again ;
: pentagonal? 0 0 { n t root } ( n -- flag )
  n 1 < if false exit then
  n 24 * 1+ to t
  t integer-sqrt to root
  root root * t = root 1+ 6 mod 0= and ;
: p44 ( -- answer )
  0 0 0 0 { best pj pk difference }
  5000 2 do
    i i 3 * 1- * 2 / to pj
    0 i 1- do
      i i 3 * 1- * 2 / to pk
      pj pk - to difference
      best 0<> difference best >= and if leave then
      difference pentagonal? if
        pj pk + pentagonal? if difference to best then
      then
    -1 +loop
  loop
  best ;
: p46 ( -- answer )
  9 0 0 0 { n representation half root }
  begin
    n is-prime? 0= if
      false to representation
      n 2 do
        i is-prime? if
          n i - 2 mod 0= if
            n i - 2 / to half
            half integer-sqrt to root
            root root * half = if true to representation leave then
          then
        then
      loop
      representation 0= if n exit then
    then
    n 2 + to n
  again ;
: distinct-prime-factors 0 0 0 { n value divisor count } ( n -- distinct count )
  n to value 2 to divisor 0 to count
  begin divisor value divisor / <= while
    value divisor mod 0= if
      count 1+ to count
      begin value divisor mod 0= while value divisor / to value repeat
    then
    divisor 1+ to divisor
  repeat
  value 1> if count 1+ to count then
  count ;
: p47 ( -- answer )
  0 { streak }
  1000000 2 do
    i distinct-prime-factors 4 = if
      streak 1+ to streak
      streak 4 = if i 3 - unloop exit then
    else 0 to streak then
  loop -1 ;
: multiply-mod { a b modulo } ( a b modulo -- remainder )
  a b um* modulo um/mod drop ;
: power-mod 0 { base exponent modulo result } ( b exp m -- residue )
  1 to result
  begin exponent 0> while
    exponent 1 and if
      result base modulo multiply-mod to result
    then
    base base modulo multiply-mod to base
    exponent 2 / to exponent
  repeat result ;
: p48 ( -- answer )
  0 { sum }
  1001 1 do
    i i 10000000000 power-mod sum + 10000000000 mod to sum
  loop sum ;
create prime-prefix 80001 cells allot
: p50 ( -- answer )
  1000000 sieve-build 0 0 0 0 0 0 { primes count best-length best-prime start-index end-index value }
  0 to count 0 to best-length 0 to best-prime
  0 prime-prefix !
  1000000 2 do
    primes i + c@ 0<> if
      count prime-prefix cells + @ i + count 1+ prime-prefix cells + !
      count 1+ to count
    then
  loop
  0 to start-index
  begin start-index count < while
    start-index best-length + 1+ to end-index
    begin end-index count <= while
      end-index prime-prefix cells + @
      start-index prime-prefix cells + @ - to value
      value 1000000 >= if leave then
      primes value + c@ 0<> if
        end-index start-index - to best-length
        value to best-prime
      then
      end-index 1+ to end-index
    repeat
    start-index 1+ to start-index
  repeat
  primes free throw best-prime ;
: big-reverse 0 0 { original reversed temp digit } ( big big -- )
  8 bnew to temp
  original temp bcopy
  reversed bclear
  begin temp blen@ 0> while
    10 temp bdiv-small! to digit
    10 reversed bmul-small!
    digit reversed badd-small!
  repeat
  temp bfree ;
: p55 ( -- answer )
  8 bnew 8 bnew 8 bnew
  0 0 0 { value reversed temp old reached count }
  0 to count
  10000 1 do
    i value bset-u false to reached
    50 0 do
      value reversed big-reverse
      value reversed temp badd
      value to old
      temp to value
      old to temp
      value reversed big-reverse
      value reversed babscompare 0= if
        true to reached leave
      then
    loop
    reached 0= if count 1+ to count then
  loop
  value bfree reversed bfree temp bfree count ;
: p69 ( -- answer )
  1 2 { result candidate }
  begin
    candidate is-prime? if
      result 1000000 candidate / > if result exit then
      result candidate * to result
    then
    candidate 1+ to candidate
  again ;
create squared-digits 0 , 1 , 4 , 9 , 16 , 25 , 36 , 49 , 64 , 81 ,
: digit-squares 0 { n result } ( n -- sum )
  0 to result
  begin
    n 10 mod cells squared-digits + @ result + to result
    n 10 / to n
    n 0= if result exit then
  again ;
: chain-end { n } ( n -- 1|89 )
  begin n 1 <> n 89 <> and while
    n digit-squares to n
  repeat n ;
create ends-at-89 568 allot
: p92 ( -- answer )
  ends-at-89 568 erase
  568 1 do i chain-end 89 = if 1 ends-at-89 i + c! then loop
  0 { count }
  10000000 1 do
    i digit-squares ends-at-89 + c@ 0<> if count 1+ to count then
  loop count ;
: euler-solve ( number -- answer )
  case
    1 of p1 endof 2 of p2 endof 3 of p3 endof 4 of p4 endof
    5 of p5 endof 6 of p6 endof 7 of p7 endof 8 of p8 endof
    9 of p9 endof 10 of p10 endof 11 of p11 endof 12 of p12 endof
    13 of p13 endof 14 of p14 endof 16 of p16 endof 17 of p17 endof
    19 of p19 endof 20 of p20 endof 21 of p21 endof 22 of p22 endof
    25 of p25 endof 27 of p27 endof 28 of p28 endof 29 of p29 endof
    30 of p30 endof 34 of p34 endof 35 of p35 endof 39 of p39 endof
    40 of p40 endof 44 of p44 endof 46 of p46 endof 47 of p47 endof
    48 of p48 endof 50 of p50 endof 55 of p55 endof 69 of p69 endof
    92 of p92 endof
    abort" unsupported Project Euler problem"
  endcase ;
