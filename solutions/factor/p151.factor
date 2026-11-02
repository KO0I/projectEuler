! Native Factor Project Euler 151 solver; evolves the paper-size distribution.
USING: formatting io math project-euler.151 ; IN: scratchpad
! The vocabulary deliberately returns an exact ratio; the problem requests
! six decimal places, for which Factor's shortest float rendering is exact.
euler151 >float "%.6f" printf nl
