\ Graph, text, combinatoric and high-precision solvers shared by the last Forth entrypoints.
: p54-rank { char } ( ascii -- rank )
  char [char] 2 >= char [char] 9 <= and if char [char] 0 - exit then
  char case [char] T of 10 endof [char] J of 11 endof
    [char] Q of 12 endof [char] K of 13 endof [char] A of 14 endof
    abort" Invalid card rank" endcase ;
: p54-value 0 0 0 0 0 0 0 0 0 0 0 0 0
  { text counts ordered pairs keys suit flush unique trip four pair-count straight category rank } ( ptr -- rank value )
  15 cells allocate throw to counts
  5 cells allocate throw to ordered
  2 cells allocate throw to pairs
  5 cells allocate throw to keys
  counts 15 cells erase keys 5 cells erase
  true to flush 0 to unique 0 to trip 0 to four 0 to pair-count 0 to straight
  text 1+ c@ to suit
  5 0 do
    text i 3 * + c@ p54-rank to rank
    rank counts prefix@ 1+ rank counts prefix!
    text i 3 * 1+ + c@ suit <> if false to flush then
  loop
  1 14 do
    i counts prefix@ 0> if
      i unique ordered prefix! unique 1+ to unique
      i counts prefix@ 2 = if i pair-count pairs prefix! pair-count 1+ to pair-count then
      i counts prefix@ 3 = if i to trip then
      i counts prefix@ 4 = if i to four then
    then
  -1 +loop
  unique 5 = if
    0 ordered prefix@ 4 ordered prefix@ - 4 = if 0 ordered prefix@ to straight else
      14 counts prefix@ 2 counts prefix@ and 3 counts prefix@ and
      4 counts prefix@ and 5 counts prefix@ and if 5 to straight then
    then
  then
  0 to category
  straight 0> flush and if 8 to category straight 0 keys prefix! else
    four 0> if 7 to category four 0 keys prefix!
      15 2 do i counts prefix@ 1 = if i 1 keys prefix! then loop
    else trip 0> pair-count 1 = and if
      6 to category trip 0 keys prefix! 0 pairs prefix@ 1 keys prefix!
    else flush if
      5 to category 5 0 do i ordered prefix@ i keys prefix! loop
    else straight 0> if 4 to category straight 0 keys prefix!
    else trip 0> if
      3 to category trip 0 keys prefix! 1 to unique
      1 14 do i counts prefix@ 1 = if i unique keys prefix! unique 1+ to unique then -1 +loop
    else pair-count 2 = if
      2 to category 0 pairs prefix@ 0 keys prefix! 1 pairs prefix@ 1 keys prefix!
      15 2 do i counts prefix@ 1 = if i 2 keys prefix! then loop
    else pair-count 1 = if
      1 to category 0 pairs prefix@ 0 keys prefix! 1 to unique
      1 14 do i counts prefix@ 1 = if i unique keys prefix! unique 1+ to unique then -1 +loop
    else 5 0 do i ordered prefix@ i keys prefix! loop
    then then then then then then then
  then
  5 0 do category 15 * i keys prefix@ + to category loop
  counts free throw ordered free throw pairs free throw keys free throw category ;
: p54 ( -- n )
  s" tests/data/p054_poker.txt" 30000 read-fixture
  0 { text length answer }
  length 30000 <> abort" Invalid poker dataset size"
  1000 0 do
    text i 30 * + p54-value
    text i 30 * 15 + + p54-value
    > if answer 1+ to answer then
  loop text free throw answer ;

create p59-letter-weights 8 , 2 , 3 , 4 , 13 , 2 , 2 , 6 , 7 , 1 , 1 , 4 , 2 , 7 , 8 , 2 , 1 , 6 , 6 , 9 , 3 , 1 , 2 , 1 , 2 , 1 ,
: p59-weight { ch } ( ascii -- weight )
  ch 32 = if 15 exit then
  ch [char] A >= ch [char] Z <= and if ch [char] A - p59-letter-weights swap cells + @ exit then
  ch [char] a >= ch [char] z <= and if ch [char] a - p59-letter-weights swap cells + @ exit then
  ch [char] 0 >= ch [char] 9 <= and if -3 exit then
  ch case [char] . of -1 endof [char] , of -1 endof [char] ; of -1 endof
    [char] : of -1 endof [char] ' of -1 endof 34 of -1 endof
    [char] ! of -1 endof [char] ? of -1 endof [char] ( of -1 endof
    [char] ) of -1 endof [char] - of -1 endof [char] [ of -1 endof
    [char] ] of -1 endof -12 swap endcase ;
: p59-match 0 { text length word size hits } ( ptr n word n -- count )
  length size >= if
    length size - 1+ 0 do
      text i + size word size compare 0= if hits 1+ to hits then
    loop
  then hits ;
: p59-words 0 0 { text length score matches } ( ptr count -- score matches )
  text length s"  the " p59-match matches + to matches
  text length s"  and " p59-match matches + to matches
  text length s"  of " p59-match matches + to matches
  text length s"  to " p59-match matches + to matches
  text length s"  in " p59-match matches + to matches
  text length s"  that " p59-match matches + to matches
  text length s"  is " p59-match matches + to matches
  text length s"  a " p59-match matches + to matches
  text length s"  for " p59-match matches + to matches
  text length s"  with " p59-match matches + to matches
  matches 100 * matches ;
: p59-keyscore 0 0 0 0 0 0
  { cipher length k0 k1 k2 text score total ch valid index matches } ( cipher n key0 key1 key2 text -- score sum )
  true to valid
  length 0 do
    i 3 mod case 0 of k0 endof 1 of k1 endof 2 of k2 endof endcase to ch
    i cipher prefix@ ch xor to ch
    ch 32 < ch 126 > or if false to valid then
    ch text i + c!
    ch p59-weight score + to score
    total ch + to total
  loop
  valid 0= if -1000000000000 0 exit then
  text length p59-words to matches score + to score
  matches 3 < if -1000000000000 0 exit then
  score total ;
: p59 ( -- n )
  s" tests/data/p059_cipher.txt" 1455 fixture-integers
  1455 allocate throw
  -1000000000000 0 0 0 0 0 { cipher length text best answer score checksum tied key }
  length 1455 <> abort" Invalid ciphertext length"
  123 97 do 123 97 do 123 97 do
    cipher length k j i text p59-keyscore to checksum to score
    score best > if score to best checksum to answer false to tied else
      score best = if true to tied then
    then
  loop loop loop
  cipher free throw text free throw
  tied abort" Multiple equally ranked English XOR keys"
  answer ;

: p83-relax 0 { vertex costs distance current candidate } ( index costs dist value -- )
  current vertex costs prefix@ + to candidate
  candidate vertex distance prefix@ < if candidate vertex distance prefix! then ;


: p83 ( -- n )
  s" tests/data/p083_matrix.txt" 6400 fixture-integers
  6400 cells allocate throw 6400 allocate throw
  0 0 0 0 0 { costs count distance used best vertex row col candidate }
  count 6400 <> abort" Matrix requires 6400 values"
  used 6400 erase
  6400 0 do 100000000 i distance prefix! loop
  0 costs prefix@ 0 distance prefix!
  6400 0 do
    100000000 to best -1 to vertex
    6400 0 do
      used i + c@ 0= if
        i distance prefix@ best < if
          i to vertex i distance prefix@ to best
        then
      then
    loop
    vertex 0< abort" Disconnected matrix graph"
    1 used vertex + c!
    vertex 6399 = if
      costs free throw distance free throw used free throw best unloop exit
    then
    vertex 80 / to row vertex 80 mod to col
    col 0> if vertex 1- costs distance best p83-relax then
    col 79 < if vertex 1+ costs distance best p83-relax then
    row 0> if vertex 80 - costs distance best p83-relax then
    row 79 < if vertex 80 + costs distance best p83-relax then
  loop -1 ;
fvariable p99-best-score
: p99 ( -- n )
  s" tests/data/p099_base_exp.txt" 2000 fixture-integers
  0 0 0 { values count best-index index exponent }
  count 2000 <> abort" Invalid exponential-comparison data"
  -1e300 p99-best-score f!
  1000 0 do
    i 2* values prefix@ s>f fln
    i 2* 1+ values prefix@ s>f f*
    fdup p99-best-score f@ f> if
      i 1+ to best-index
      p99-best-score f!
    else fdrop then
  loop values free throw best-index ;

create p61-values 900 cells allot
create p61-count 6 cells allot
create p61-path 6 cells allot
: p61-prepare 0 0 0 0 { sides value tally offset }
  p61-count 6 cells erase
  6 0 do
    i 3 + to sides
    200 1 do
      sides 2 - i i * * sides 4 - i * - 2 / to value
      value 1000 >= value 10000 < and value 100 mod 10 >= and if
        j p61-count prefix@ to tally
        tally 150 >= abort" Polygonal fixture exceeded capacity"
        j 150 * tally + to offset
        value offset p61-values prefix!
        tally 1+ j p61-count prefix!
      then
    loop
  loop ;
: p61-recurse 0 0 0 0 { depth used leading candidate answer repeated } ( depth used -- result )
  depth 6 = if
    5 p61-path prefix@ 100 mod 0 p61-path prefix@ 100 / = if
      0 6 0 do i p61-path prefix@ + loop exit
    then 0 exit
  then
  depth 1- p61-path prefix@ 100 mod to leading
  6 0 do
    used 1 i lshift and 0= if
      i p61-count prefix@ 0 do
        j 150 * i + p61-values prefix@ to candidate
        candidate 100 / leading = if
          false to repeated
          depth 0 do i p61-path prefix@ candidate = if true to repeated then loop
          repeated 0= if
            candidate depth p61-path prefix!
            depth 1+ used 1 j lshift or recurse to answer
            answer 0<> if answer unloop unloop exit then
          then
        then
      loop
    then
  loop 0 ;
: p61 ( -- n ) 0 { answer }
  p61-prepare
  5 p61-count prefix@ 0 do
    5 150 * i + p61-values prefix@ 0 p61-path prefix!
    1 1 5 lshift p61-recurse to answer
    answer 0<> if answer unloop exit then
  loop -1 ;

create p68-inner 5 cells allot
create p68-outer 5 cells allot
variable p68-best
: p68-search 0 0 0 0 0 0 { depth used first line all candidate representation valid } ( depth used -- )
  depth 5 < if
    10 1 do
      used 1 i lshift and 0= if
        i depth p68-inner prefix!
        depth 1+ used 1 i lshift or recurse
      then
    loop exit
  then
  11 1 do
    used 1 i lshift and 0= if
      i to first
      first 0 p68-inner prefix@ + 1 p68-inner prefix@ + to line
      used to all true to valid
      5 0 do
        line i p68-inner prefix@ - i 1+ 5 mod p68-inner prefix@ - to candidate
        candidate first < candidate 10 > or if false to valid leave then
        all 1 candidate lshift and if false to valid leave then
        candidate i p68-outer prefix!
        all 1 candidate lshift or to all
      loop
      valid if
        0 to representation
        5 0 do
          i p68-outer prefix@ to candidate
          candidate 10 = if representation 100 * 10 + to representation else
            representation 10 * candidate + to representation then
          representation 10 * i p68-inner prefix@ + to representation
          representation 10 * i 1+ 5 mod p68-inner prefix@ + to representation
        loop
        representation p68-best @ > if representation p68-best ! then
      then
    then
  loop ;
: p68 ( -- n ) 0 p68-best ! 0 0 p68-search p68-best @ ;

: p107 ( -- n )
  s" tests/data/p107_network.txt" 8000 read-fixture
  1600 cells allocate throw 40 cells allocate throw 40 allocate throw
  0 0 0 0 0 0 0 0 0
  { raw length edges distance chosen count number active char total cost best vertex candidate }
  0 to count 0 to number false to active
  length 0 do
    raw i + c@ to char
    char [char] 0 [char] 9 1+ within if
      char [char] 0 - number 10 * + to number true to active
    else char [char] - = if
      true to active 0 to number
    else active if
      count 1600 >= abort" Network too large"
      number count edges prefix! count 1+ to count
      false to active 0 to number
    then then then
  loop
  active if number count edges prefix! count 1+ to count then
  count 1600 <> abort" Network requires 40x40 values"
  40 0 do
    40 i 1+ ?do
      j 40 * i + edges prefix@ total + to total
    loop
  loop
  chosen 40 erase
  40 0 do 1000000 i distance prefix! loop
  0 0 distance prefix!
  40 0 do
    1000000 to best -1 to vertex
    40 0 do
      chosen i + c@ 0= if i distance prefix@ best < if
        i distance prefix@ to best i to vertex
      then then
    loop
    vertex 0< abort" Graph disconnected"
    best cost + to cost
    1 chosen vertex + c!
    40 0 do
      vertex 40 * i + edges prefix@ to candidate
      candidate 0> chosen i + c@ 0= and if
        candidate i distance prefix@ < if candidate i distance prefix! then
      then
    loop
  loop
  raw free throw edges free throw distance free throw chosen free throw
  total cost - ;

: p66-period 0 0 0 0 0 { n root m denominator a period } ( D -- period )
  n integer-sqrt to root
  0 to m 1 to denominator root to a 0 to period
  begin
    denominator a * m - to m
    n m m * - denominator / to denominator
    root m + denominator / to a
    period 1+ to period
    a root 2* =
  until period ;
: p66-numerator 0 0 0 0 0 0 0 0 0 0
  { n answer p0 p1 next old root m denominator a period steps } ( D big-result -- )
  8 bnew to p0 8 bnew to p1 8 bnew to next
  0 p0 bset-u 1 p1 bset-u
  n integer-sqrt to root
  n p66-period to period
  period 2 mod if period 2* else period then to steps
  0 to m 1 to denominator root to a
  steps 0 do
    i 0> if
      denominator a * m - to m
      n m m * - denominator / to denominator
      root m + denominator / to a
    then
    p1 next bcopy
    a next bmul-small!
    next p0 baddabs!
    p0 to old p1 to p0 next to p1 old to next
  loop
  p1 answer bcopy p0 bfree p1 bfree next bfree ;
: p66 ( -- n )
  8 bnew 8 bnew 0 { best value answer }
  1001 2 do
    i integer-sqrt dup * i <> if
      i value p66-numerator
      value best babscompare 0> if
        value best bcopy i to answer
      then
    then
  loop best bfree value bfree answer ;

: p80-root-digits 0 0 0 0 0 0 { number remainder root trial answer digit found } ( n -- digit-sum )
  12 bnew to remainder 12 bnew to root 12 bnew to trial
  0 remainder bset-u 0 root bset-u
  100 0 do
    100 remainder bmul-small!
    i 0= if number remainder badd-small! then
    false to found
    10 0 do
      9 i - to digit
      root trial bcopy
      20 trial bmul-small!
      digit trial badd-small!
      digit trial bmul-small!
      remainder trial babscompare 0>= if
        remainder trial bsubabs!
        10 root bmul-small!
        digit root badd-small!
        answer digit + to answer
        true to found leave
      then
    loop
    found 0= abort" Square-root digit extraction failed"
  loop
  remainder bfree root bfree trial bfree answer ;
: p80 ( -- n ) 0 { total }
  101 2 do
    i integer-sqrt dup * i <> if i p80-root-digits total + to total then
  loop total ;

: p96-search 0 0 0 0 0 0 0 0 0 0 0
  { board rows cols boxes choice candidates few row col box digit bit mask count old } ( board rows cols boxes -- solved? )
  -1 to choice 10 to few
  81 0 do
    i board prefix@ 0= if
      i 9 / to row i 9 mod to col
      row 3 / 3 * col 3 / + to box
      row rows prefix@ col cols prefix@ or box boxes prefix@ or invert 1022 and to mask
      mask bit-count to count
      count 0= if false unloop exit then
      count few < if count to few i to choice mask to candidates then
    then
  loop
  choice -1 = if true exit then
  choice 9 / to row choice 9 mod to col
  row 3 / 3 * col 3 / + to box
  10 1 do
    1 i lshift to bit
    candidates bit and if
      i choice board prefix!
      row rows prefix@ bit or row rows prefix!
      col cols prefix@ bit or col cols prefix!
      box boxes prefix@ bit or box boxes prefix!
      board rows cols boxes recurse if true unloop exit then
      0 choice board prefix!
      row rows prefix@ bit xor row rows prefix!
      col cols prefix@ bit xor col cols prefix!
      box boxes prefix@ bit xor box boxes prefix!
    then
  loop false ;
: p96 ( -- n )
  s" tests/data/p096_sudoku.txt" 5000 read-fixture
  81 cells allocate throw 9 cells allocate throw 9 cells allocate throw 9 cells allocate throw
  0 0 0 0 0 0 0
  { raw length board rows cols boxes answer offset ch row col square bit }
  length 4899 <> length 4900 <> and abort" Sudoku fixture has unexpected length"
  50 0 do
    rows 9 cells erase cols 9 cells erase boxes 9 cells erase
    i 98 * 8 + to offset
    9 0 do i to row
      9 0 do i to col
        raw offset + row 10 * + col + c@ [char] 0 - to ch
        ch row 9 * col + board prefix!
        ch 0> if
          1 ch lshift to bit
          row rows prefix@ bit or row rows prefix!
          col cols prefix@ bit or col cols prefix!
          row 3 / 3 * col 3 / + to square
          square boxes prefix@ bit or square boxes prefix!
        then
      loop
    loop
    board rows cols boxes p96-search 0= abort" Sudoku board is unsolvable"
    0 board prefix@ 100 * 1 board prefix@ 10 * + 2 board prefix@ + answer + to answer
  loop
  raw free throw board free throw rows free throw cols free throw boxes free throw answer ;

: bceil-div10! { big } ( bigint -- )
  10 big bdiv-small! 0<> if 1 big badd-small! then ;
: bsmall-u { big } ( bigint <=9 digits -- n )
  0 big blen@ 0 do
    i big blimb@ i 4 * pow10 * +
  loop ;
: bleading-nine 0 { big temp } ( bignum -- 9-digit integer )
  12 bnew to temp big temp bcopy
  begin temp bdecimal-length 9 > while 10 temp bdiv-small! drop repeat
  temp bsmall-u temp bfree ;
: p104 ( -- n )
  12 bnew 12 bnew 12 bnew 12 bnew 12 bnew 12 bnew 12 bnew
  1 1 2 0 0 0 0
  { low-a low-b low-next high-a high-b high-next temp tail-a tail-b index result old low-head high-head }
  1 low-a bset-u 1 low-b bset-u
  1 high-a bset-u 1 high-b bset-u
  begin
    tail-a tail-b + 1000000000 mod to result
    tail-b to tail-a result to tail-b
    index 1+ to index
    low-a low-b low-next badd
    high-a high-b high-next badd
    low-a to old low-b to low-a low-next to low-b old to low-next
    high-a to old high-b to high-a high-next to high-b old to high-next
    low-b bdecimal-length 40 > if
      10 low-a bdiv-small! drop 10 low-b bdiv-small! drop
      high-a bceil-div10! high-b bceil-div10!
    then
    tail-b pandigital-nine? if
      low-b bleading-nine to low-head
      high-b bleading-nine to high-head
      low-head high-head = low-head pandigital-nine? and if
        low-a bfree low-b bfree low-next bfree
        high-a bfree high-b bfree high-next bfree temp bfree
        index exit
      then
    then
    index 1000000 > abort" Pandigital Fibonacci solution not found"
  again ;
