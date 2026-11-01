USING: io kernel locals math math.functions math.order math.parser ranges sequences math.primes ;
IN: scratchpad

! Factor's sieve-backed prime sequence; upper bound is inclusive.
: p10 ( -- n ) 1999999 primes-upto sum ;
p10 number>string print
