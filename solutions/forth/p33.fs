\ Project Euler 33: Digit Cancelling Fractions.
require math.fs

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

[undefined] euler-testing [if] p33 . cr bye [then]
