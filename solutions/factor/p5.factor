USING: io kernel locals math math.functions math.order math.parser ranges sequences ;
IN: scratchpad

! LCM fold avoids enumerating candidates.
: p5 ( -- n ) 20 [1..b] 1 [ lcm ] reduce ;
p5 number>string print
