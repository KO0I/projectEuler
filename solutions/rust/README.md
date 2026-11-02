# Rust problem programs

Each `src/bin/pN.rs` contains that problem's algorithm and private helpers.
`src/lib.rs` exposes `extra_helpers`, which provides reusable mathematical and
input operations. There is no library dispatcher for problem numbers.

From the repository root:

```sh
cargo run --release --manifest-path solutions/rust/Cargo.toml --bin p7
make unit-rust
make rust-release
python3 scripts/bench.py --mode test --timeout 60 --languages rust
```

Problem data is embedded at compile time with `include_str!`. Build dependencies
are pinned in `Cargo.lock`. Tests for problem-specific routines live in their
binary files; tests for shared arithmetic live in the library.
