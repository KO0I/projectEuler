\ Project Euler 101: Optimum Polynomial.
require math.fs

: p101-sequence { n } ( n -- polynomial )
  1 10 0 do n * 1 swap - loop ;

: p101-fit 0 0 { differences terms at sum coefficient } ( ptr terms at -- value )
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
        i 1+ differences prefix@ i differences prefix@ - i differences prefix!
      loop
    loop
    k 1+ to at
    begin first k at p101-fit at p101-sequence = while at 1+ to at repeat
    first k at p101-fit total + to total
  loop
  differences free throw first free throw total ;

[undefined] euler-testing [if] p101 . cr bye [then]
