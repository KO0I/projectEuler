# Gforth solutions and arbitrary precision

Run from the repository root, for example `gforth solutions/forth/p25.fs`. Each `pN.fs` includes `solvers.fs` relative to the file and prints one integer. The solution collection requires a **64-bit Gforth** host; it fails explicitly with other cell widths. Gforth 0.7.3 is the validated interpreter. Brace locals are a documented Gforth extension rather than an assertion of universal Forth portability.

## Big integers (`big.fs`)

A heap-allocated number stores sign, used-limb count, capacity and a separately allocated limb array. Limbs are little-endian in base 10000; capacity grows with `resize`. Zero is normalized to length=0 and sign=0.

- `bnew` / `bfree`: allocation and ownership; every returned object must be released.
- `bset-u`, `bfrom-dec`, `bcopy`: construct from an unsigned machine integer, unsigned decimal text, or another big integer. Decimal parsing accepts digits only; signed values are represented through `bsign!`.
- `badd`: signed addition into a distinct destination; `bsubabs!` subtracts magnitudes in place, requiring the left magnitude to be at least the right.
- `bmul`: signed multiplication into a distinct destination. `bmul-small!`, `badd-small!`, `bdiv-small!` serve positive-magnitude arithmetic and return the small-division remainder.
- `babscompare`, `bdecimal-length`, `bdigitsum`, `bprint`: comparison, inspection and decimal output.

The destination of `badd` or `bmul` must not alias either input. Small factors/divisors must keep intermediate cell arithmetic in range. This is an initial project-specific bigint library, not a general replacement for GMP; full big-divisor division and a public signed text parser are not implemented. Problem 48 uses bounded double-cell modular arithmetic through `multiply-mod` and `power-mod` in `solvers.fs`.

Run `make unit-forth` for bigint tests and the standard `T{ ... -> ... }T` stack-test convention for numerical helpers. Bigint regressions cover 1201-digit dynamic growth, zero normalization, carries, borrows, signs, multiplication, small division and decimal conversion. Run `python3 scripts/bench.py --mode test --timeout 60 --languages forth` to check all 120 entry points with a one-minute limit per solver, or `make profile-forth PROFILE_TIMEOUT=60` to record one timed invocation of each and generate graphs. The current result is 115 passes and five timeouts, with no wrong answers or runtime errors; see the [validation record](../../docs/VALIDATION_FORTH_1_120.md).
