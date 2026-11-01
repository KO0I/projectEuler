\ Additional standalone searches; share prime, bigint and fixture facilities.
\ Problem 60: concatenation-prime graph with increasing-clique search.
create p60-primes 4000 cells allot
create p60-adj 16000000 allot
create p60-candidates 4000 cells allot
create p60-scratch 20000 cells allot
variable p60-count
variable p60-best
: p60-concat 0 { a b scale } ( a b -- ab )
  10 to scale begin scale b <= while scale 10 * to scale repeat
  a scale * b + ;
: p60-adj@ { i j } ( i j -- flag )
  i p60-count @ * j + p60-adj + c@ 0<> ;
: p60-prime@ ( index -- prime ) cells p60-primes + @ ;
: p60-build 0 0 0 0 0 0 { limit sieve count a b n j } ( bound -- )
  limit sieve-build to sieve 0 to count
  limit 1+ 3 do
    i 2 <> i 5 <> and sieve i + c@ 0<> and if
      i count cells p60-primes + ! count 1+ to count
    then
  loop
  count p60-count !
  p60-adj count count * erase
  count 0 do
    i p60-prime@ to a
    i 1+ to j
    begin j count < while
      j p60-prime@ to b
      a b + p60-best @ < a b + 3 mod 0<> and if
        a b p60-concat is-prime? if
          b a p60-concat is-prime? if
            1 p60-adj i count * j + + c!
            1 p60-adj j count * i + + c!
          then
        then
      then
      j 1+ to j
    repeat
  loop
  sieve free throw ;
: p60-search 0 0 0 0 0 0
  { depth total candidates size needed lower index next next-count vertex } ( depth sum candidates count -- )
  5 depth - to needed
  size needed < if exit then
  total to lower
  needed 0 do candidates i cells + @ p60-prime@ lower + to lower loop
  lower p60-best @ >= if exit then
  needed 1 = if lower p60-best ! exit then
  0 to index
  begin index needed + size <= while
    total to lower
    needed 0 do candidates index i + cells + @ p60-prime@ lower + to lower loop
    lower p60-best @ >= if exit then
    depth 4000 * cells p60-scratch + to next
    0 to next-count
    candidates index cells + @ to vertex
    size index 1+ do
      vertex candidates i cells + @ p60-adj@ if
        candidates i cells + @ next-count cells next + !
        next-count 1+ to next-count
      then
    loop
    depth 1+ total vertex p60-prime@ + next next-count recurse
    index 1+ to index
  repeat ;
: p60 ( -- sum )
  9223372036854775807 p60-best !
  10000 p60-build
  p60-count @ 0 do i i cells p60-candidates + ! loop
  0 0 p60-candidates p60-count @ p60-search
  p60-best @ 9223372036854775807 = abort" Prime-pair discovery bound failed"
  p60-best @ 1- p60-build
  p60-count @ 0 do i i cells p60-candidates + ! loop
  0 0 p60-candidates p60-count @ p60-search
  p60-best @ ;

\ Problem 93: evaluate every binary-expression tree using reduced rationals.
defer p93-expression
create p93-num 16 cells allot
create p93-den 16 cells allot
create p93-reached 10000 allot
: p93-n@ { depth index } ( depth index -- n ) depth 4 * index + cells p93-num + @ ;
: p93-d@ { depth index } ( depth index -- d ) depth 4 * index + cells p93-den + @ ;
: p93-set { n d depth index } ( n d depth index -- )
  n depth 4 * index + cells p93-num + !
  d depth 4 * index + cells p93-den + ! ;
: p93-rational 0 { n d factor } ( n d -- reduced-n reduced-d )
  d 0< if n negate to n d negate to d then
  n abs d integer-gcd to factor
  n factor / d factor / ;
: p93-test { n d depth size } ( n d depth count -- )
  n d p93-rational to d to n
  n d depth size 2 - p93-set
  depth size 1- p93-expression ;
:noname 0 0 0 0 0 0 0 0 0 0
  { depth size i j a b c d next other one two } ( depth count -- )
  size 1 = if
    depth 0 p93-n@ to a depth 0 p93-d@ to b
    b 1 = a 0> and a 10000 < and if 1 p93-reached a + c! then
    exit
  then
  size 0 do
    i to one
    i 1+ size do
      i to two
      depth one p93-n@ to a depth one p93-d@ to b
      depth two p93-n@ to c depth two p93-d@ to d
      0 to next
      size 0 do
        i one <> i two <> and if
          depth i p93-n@ depth i p93-d@ depth 1+ next p93-set
          next 1+ to next
        then
      loop
      a d * c b * + b d * depth 1+ size p93-test
      a d * c b * - b d * depth 1+ size p93-test
      c b * a d * - b d * depth 1+ size p93-test
      a c * b d * depth 1+ size p93-test
      c 0<> if a d * b c * depth 1+ size p93-test then
      a 0<> if c b * d a * depth 1+ size p93-test then
    loop
  loop ; is p93-expression
: p93-run 0 0 { a b c d position answer } ( a b c d -- consecutive )
  p93-reached 10000 erase
  a 1 0 0 p93-set b 1 0 1 p93-set
  c 1 0 2 p93-set d 1 0 3 p93-set
  0 4 p93-expression
  1 to position
  begin position 10000 < if p93-reached position + c@ 0<> else false then while
    position 1+ to position
  repeat position 1- ;
: p93 0 0 0 0 0 0 { a b c d best answer } ( -- digits )
  0 to best 0 to answer
  7 0 do i to a
    8 a 1+ do i to b
      9 b 1+ do i to c
        10 c 1+ do i to d
          a b c d p93-run best > if
            a b c d p93-run to best
            a 1000 * b 100 * + c 10 * + d + to answer
          then
        loop
      loop
    loop
  loop answer ;

\ Problem 103: prove the constructed seven-element bound by exhaustive
\ increasing-set search with subset-collision and cardinality pruning.
create p103-current 7 cells allot
create p103-best 7 cells allot
create p103-sums 1024 cells allot
create p103-seen 4096 allot
variable p103-bound
: p103-value@ { index } index cells p103-current + @ ;
: p103-cardinal? 0 0 0 0 0 { chosen k low high index last } ( chosen -- flag )
  1 to k
  begin 2 k * 1+ 7 <= while
    chosen k 1+ >= if
      0 to low 0 to high
      k 1+ 0 do i p103-value@ low + to low loop
      7 k - to index
      begin index 7 < while
        index chosen < if index p103-value@
        else chosen 1- p103-value@ index chosen - + 1+ then
        high + to high
        index 1+ to index
      repeat
      low high <= if false exit then
    then
    k 1+ to k
  repeat true ;
: p103-search 0 0 0 0 0 0 0 0 0 0
  { depth total left value minsum source newseen newlist oldlist size check index } ( depth sum -- )
  depth 7 = if
    total p103-bound !
    p103-current p103-best 7 cells move
    exit
  then
  7 depth - to left
  depth 0= if 1 else depth 1- p103-value@ 1+ then to value
  begin
    total left value * + left left 1- * 2 / + to minsum
    minsum p103-bound @ <
  while
    value depth cells p103-current + !
    depth 1+ p103-cardinal? if
      depth 512 * p103-seen + to oldlist
      depth 128 * cells p103-sums + to source
      1 depth lshift to size
      true to check
      size 0 do
        source i cells + @ value + oldlist + c@ 0<> if
          false to check leave
        then
      loop
      check if
        depth 1+ 512 * p103-seen + to newseen
        oldlist newseen 512 move
        depth 1+ 128 * cells p103-sums + to newlist
        source newlist size cells move
        size 0 do
          source i cells + @ value + to index
          1 newseen index + c!
          index newlist size i + cells + !
        loop
        depth 1+ total value + recurse
      then
    then
    value 1+ to value
  repeat ;
: p103 ( -- concatenation )
  20 0 cells p103-best + !
  11 18 19 20 22 25 0 0 0 0 { a b c d e f value index power result }
  0 p103-bound !
  a 20 + 1 cells p103-best + !
  b 20 + 2 cells p103-best + !
  c 20 + 3 cells p103-best + !
  d 20 + 4 cells p103-best + !
  e 20 + 5 cells p103-best + !
  f 20 + 6 cells p103-best + !
  7 0 do i cells p103-best + @ p103-bound @ + p103-bound ! loop
  p103-seen 4096 erase
  1 p103-seen c!
  0 0 cells p103-sums + !
  0 0 p103-search
  0 to result
  7 0 do
    i cells p103-best + @ to value
    10 to power
    begin power value <= while power 10 * to power repeat
    result power * value + to result
  loop result ;

\ Problem 84: 4-sided dice and the exact independent-draw Markov model.
\ Unlike the fixed cyclic-deck model, Chance/Chest cards are uniformly
\ reshuffled after drawing. Both reproduce the requested 3-square modal code.
create p84-current 120 floats allot
create p84-next 120 floats allot
create p84-used 40 allot
fvariable p84-maximum
: p84-store { square doubles } ( F: probability -- )
  p84-next square 3 * doubles + floats +
  dup f@ f+ f! ;
: p84-cc { square doubles } ( F: probability -- )
  fdup 16e f/ 0 doubles p84-store
  fdup 16e f/ 10 0 p84-store
  14e 16e f/ square doubles p84-store ;
: p84-ch { square doubles } ( F: probability -- )
  fdup 16e f/ 0 doubles p84-store
  fdup 16e f/ 10 0 p84-store
  fdup 16e f/ 11 doubles p84-store
  fdup 16e f/ 24 doubles p84-store
  fdup 16e f/ 39 doubles p84-store
  fdup 16e f/ 5 doubles p84-store
  fdup 8e f/ square 7 = if 15 else square 22 = if 25 else 5 then then doubles p84-store
  fdup 16e f/ square 22 = if 28 else 12 then doubles p84-store
  fdup 16e f/
    square 36 = if 33 doubles p84-cc else square 3 - doubles p84-store then
  6e 16e f/ square doubles p84-store ;
: p84-step 0 0 { square doubles roll pair next-square next-doubles } ( square doubles dice-sum same? -- ) ( F: prob -- )
  pair doubles 2 = and if 10 0 p84-store exit then
  pair if doubles 1+ else 0 then to next-doubles
  square roll + 40 mod to next-square
  next-square 30 = if 10 0 p84-store exit then
  next-square 7 = next-square 22 = or next-square 36 = or if
    next-square next-doubles p84-ch exit
  then
  next-square 2 = next-square 17 = or next-square 33 = or if
    next-square next-doubles p84-cc exit
  then
  next-square next-doubles p84-store ;
: p84-square-prob { square } ( F: -- p )
  0e
  3 0 do p84-current square 3 * i + floats + f@ f+ loop ;
: p84 0 0 0 0 0 0 { state square doubles chosen result probability } ( -- 6digit code )
  p84-used 40 erase
  120 0 do 1e 120e f/ p84-current i floats + f! loop
  300 0 do
    p84-next 120 floats erase
    120 0 do
      i to state
      state 3 / to square state 3 mod to doubles
      5 1 do 5 1 do
        p84-current state floats + f@ 16e f/
        square doubles i j + i j = p84-step
      loop loop
    loop
    p84-next p84-current 120 floats move
  loop
  0 to result
  3 0 do
    -1e p84-maximum f!
    -1 to chosen
    40 0 do
      p84-used i + c@ 0= if
        i p84-square-prob fdup p84-maximum f@ f> if
          p84-maximum f! i to chosen
        else fdrop then
      then
    loop
    chosen 0< abort" Failed to rank Monopoly squares"
    1 p84-used chosen + c!
    result 100 * chosen + to result
  loop result ;

\ Problem 98: square mappings are checked as bijections in both directions.
create p98-words 1800 cells allot
create p98-lengths 1800 cells allot
create p98-signatures 46800 allot
create p98-letters 26 cells allot
create p98-used 10 cells allot
variable p98-count
: p98-word@ ( index -- address ) cells p98-words + @ ;
: p98-length@ ( index -- n ) cells p98-lengths + @ ;
: p98-signature ( index -- address ) 26 * p98-signatures + ;
: p98-power 0 { exponent value } ( k -- 10^k )
  1 to value exponent 0 do value 10 * to value loop value ;
: p98-load 0 0 0 0 0 { raw length count position word-start letter signature } ( ptr size -- )
  0 to count 0 to position
  p98-signatures 46800 erase
  begin position length < while
    raw position + c@ [char] " <> abort" Invalid quoted word"
    position 1+ to position
    raw position + count cells p98-words + !
    position to word-start
    count p98-signature to signature
    begin position length < if raw position + c@ [char] " <> else false then while
      raw position + c@ [char] A - to letter
      letter 0< letter 25 > or abort" Invalid anagram letter"
      signature letter + c@ 1+ signature letter + c!
      position 1+ to position
    repeat
    position word-start - count cells p98-lengths + !
    count 1+ to count
    position 1+ to position
    position length < if
      raw position + c@ [char] , <> abort" Invalid anagram separator"
      position 1+ to position
    then
  repeat
  count 1786 <> abort" Incomplete anagram word fixture"
  count p98-count ! ;
: p98-pair 0 0 0 0 0 0 0 0 0 0 0
  { first second length lower upper root square letter digit power other consistent best }
  ( word-index word-index -- largest square )
  first p98-length@ to length
  length 1- p98-power integer-sqrt to lower
  lower lower * length 1- p98-power < if lower 1+ to lower then
  length p98-power 1- integer-sqrt to upper
  0 to best
  lower to root
  begin root upper <= while
    root root * to square
    p98-letters 26 cells 255 fill
    p98-used 10 cells 255 fill
    true to consistent
    length 0 do
      first p98-word@ i + c@ [char] A - to letter
      length i - 1- p98-power to power
      square power / 10 mod to digit
      letter cells p98-letters + @ -1 <> if
        letter cells p98-letters + @ digit <> if false to consistent leave then
      else
        digit cells p98-used + @ -1 <> if false to consistent leave then
        letter digit cells p98-used + !
        digit letter cells p98-letters + !
      then
    loop
    consistent if
      second p98-word@ c@ [char] A - cells p98-letters + @ 0<> if
        0 to other
        length 0 do
          second p98-word@ i + c@ [char] A - cells p98-letters + @ to digit
          other 10 * digit + to other
        loop
        other integer-sqrt dup * other = if
          square other max best max to best
        then
      then
    then
    root 1+ to root
  repeat best ;
: p98 ( -- largest-square )
  s" tests/data/p098_words.txt" 18000 read-fixture
  0 0 0 { raw length answer count i }
  raw length p98-load
  0 to answer
  p98-count @ 0 do
    i 1+ p98-count @ do
      i p98-length@ j p98-length@ = if
        i p98-signature 26 j p98-signature 26 compare 0= if
          j i p98-pair answer max to answer
        then
      then
    loop
  loop
  raw free throw answer ;
