\ Project Euler 59: XOR Decryption.
require math.fs

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

[undefined] euler-testing [if] p59 . cr bye [then]
