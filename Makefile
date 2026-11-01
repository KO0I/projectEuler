# Project Euler: one executable per problem and language.
# Profile runs every executable ONCE, including process startup, excluding builds.
PYTHON ?= python3
CC ?= gcc
CFLAGS ?= -O2 -std=c11 -Wall -Wextra
LDLIBS ?= -lgmp -lm
C_FILES := $(wildcard solutions/c/*.c)
C_TARGETS := $(patsubst solutions/c/%.c,build/c/%,$(C_FILES))
FORTH_FILES := $(wildcard solutions/forth/p*.fs)
RUST_MANIFEST := solutions/rust/Cargo.toml

.PHONY: all c-release rust-release forth-check test test-c test-rust test-forth \
	profile profile-c profile-rust profile-forth plots plots-math clean
all: c-release
c-release: $(C_TARGETS)
build/c/%: solutions/c/%.c support/c/euler_common.c support/c/euler_common.h
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -Isupport/c "$<" support/c/euler_common.c -o "$@" $(LDFLAGS) $(LDLIBS)

rust-release:
	cargo build --release --bins --manifest-path $(RUST_MANIFEST)

forth-check:
	@command -v gforth >/dev/null || { echo "Install Gforth to run the Forth solutions"; exit 1; }

test: c-release rust-release forth-check
	$(PYTHON) scripts/bench.py --mode test --languages c rust forth
test-c: c-release
	$(PYTHON) scripts/bench.py --mode test --languages c
test-rust: rust-release
	$(PYTHON) scripts/bench.py --mode test --languages rust
test-forth: forth-check
	$(PYTHON) scripts/bench.py --mode test --languages forth

profile: c-release rust-release forth-check
	$(PYTHON) scripts/bench.py --mode profile --languages c rust forth
	$(MAKE) plots
profile-c: c-release
	$(PYTHON) scripts/bench.py --mode profile --languages c
	$(MAKE) plots
profile-rust: rust-release
	$(PYTHON) scripts/bench.py --mode profile --languages rust
	$(MAKE) plots
profile-forth: forth-check
	$(PYTHON) scripts/bench.py --mode profile --languages forth
	$(MAKE) plots
plots:
	$(PYTHON) scripts/plots.py
plots-math:
	$(PYTHON) scripts/math_plots.py
clean:
	rm -rf build
	-cargo clean --manifest-path $(RUST_MANIFEST)
