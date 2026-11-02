# One executable per problem and language; one observation per profile run.
PYTHON ?= python3
CC ?= gcc
CFLAGS ?= -O2 -std=c11 -Wall -Wextra
LDLIBS ?= -lgmp -lm
export CC CFLAGS
C_FILES := $(wildcard solutions/c/*.c)
C_TARGETS := $(patsubst solutions/c/%.c,build/c/%,$(C_FILES))
RUST_MANIFEST := solutions/rust/Cargo.toml
FACTOR_VM ?= $(CURDIR)/.toolchains/factor/factor
export FACTOR_VM

.PHONY: all c-release rust-release forth-check factor-check unit unit-c unit-rust unit-forth unit-python \
        test test-c test-rust test-forth test-factor profile profile-c profile-rust profile-forth profile-factor \
        plots plots-math docs clean
all: c-release
c-release: $(C_TARGETS)
build/c/euler_common.o: support/c/euler_common.c support/c/euler_common.h
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -Isupport/c -c "$<" -o "$@"
build/c/%: solutions/c/%.c build/c/euler_common.o support/c/euler_common.h
	$(CC) $(CFLAGS) -Isupport/c "$<" build/c/euler_common.o -o "$@" $(LDFLAGS) $(LDLIBS)
rust-release:
	cargo build --release --bins --manifest-path $(RUST_MANIFEST)
forth-check:
	@command -v gforth >/dev/null || { echo "Install 64-bit Gforth to run Forth solutions"; exit 1; }
factor-check:
	@test -x "$(FACTOR_VM)" || { echo "Factor language VM missing: set FACTOR_VM to the executable, NOT GNU factor"; exit 1; }

build/tests/c_helpers: tests/c_helpers.c support/c/euler_common.c support/c/euler_common.h
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -UNDEBUG -Isupport/c "$<" -o "$@" $(LDFLAGS) $(LDLIBS)
unit-c: build/tests/c_helpers
	./build/tests/c_helpers
unit-rust:
	cargo test --release --lib --manifest-path $(RUST_MANIFEST)
unit-forth: forth-check
	gforth solutions/forth/test-big.fs
	gforth solutions/forth/test-solvers.fs
unit-python:
	$(PYTHON) -m unittest discover -s tests -p 'test_*.py'
unit: unit-c unit-rust unit-forth unit-python

test: c-release rust-release forth-check factor-check
	$(PYTHON) scripts/bench.py --mode test --languages c rust forth factor
test-c: c-release
	$(PYTHON) scripts/bench.py --mode test --languages c
test-rust: rust-release
	$(PYTHON) scripts/bench.py --mode test --languages rust
test-forth: forth-check
	$(PYTHON) scripts/bench.py --mode test --languages forth
test-factor: factor-check
	$(PYTHON) scripts/bench.py --mode test --languages factor

# Preserve BOTH validation and plotting errors. Shell dollars require $$ in Make.
define profile_command
	@status=0; $(PYTHON) scripts/bench.py --mode profile --languages $(1) || status=$$?; \
	plot_status=0; $(MAKE) plots || plot_status=$$?; \
	if [ $$status -ne 0 ]; then exit $$status; else exit $$plot_status; fi
endef
profile: c-release rust-release forth-check factor-check
	$(call profile_command,c rust forth factor)
profile-c: c-release
	$(call profile_command,c)
profile-rust: rust-release
	$(call profile_command,rust)
profile-forth: forth-check
	$(call profile_command,forth)
profile-factor: factor-check
	$(call profile_command,factor)
plots:
	$(PYTHON) scripts/plots.py
plots-math:
	$(PYTHON) scripts/math_plots.py
docs:
	$(MAKE) -C explanations/latex
clean:
	rm -rf build
	-$(MAKE) -C explanations/latex clean
	-cargo clean --manifest-path $(RUST_MANIFEST)
