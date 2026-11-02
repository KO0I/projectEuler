# One executable per problem and language; one observation per profile run.
PYTHON ?= python3
CC ?= gcc
CFLAGS ?= -O2 -std=c11 -Wall -Wextra -Werror
LDLIBS ?= -lgmp -lm
export CC CFLAGS
C_FILES := $(wildcard solutions/c/*.c)
C_TARGETS := $(patsubst solutions/c/%.c,build/c/%,$(C_FILES))
C_EXAMPLE_TESTS := $(patsubst tests/%.c,build/tests/%,$(wildcard tests/c_*_examples.c))
RUST_MANIFEST := solutions/rust/Cargo.toml
FACTOR_VM ?= $(CURDIR)/.toolchains/factor/factor
export FACTOR_VM
EXE := $(if $(filter Windows_NT,$(OS)),.exe,)
STARTUP_C := build/bench/noop-c$(EXE)
STARTUP_RUST := build/bench/noop-rust$(EXE)
PROFILE_TIMEOUT ?= 30
TEST_TIMEOUT ?= 60
STARTUP_SAMPLES ?= 5

.PHONY: all c-release rust-release forth-check factor-check unit unit-c unit-rust unit-forth unit-python \
        test test-c test-rust test-forth test-factor profile profile-c profile-rust profile-forth profile-factor \
        run_profile_all setup-arch check-arch-setup plots plots-math docs clean scaffolds check-scaffolds refresh-scaffolds
all: c-release
c-release: $(C_TARGETS)
build/c/euler_common.o: support/c/euler_common.c support/c/euler_common.h
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -Isupport/c -c "$<" -o "$@"
build/c/%: solutions/c/%.c build/c/euler_common.o support/c/euler_common.h support/c/euler_extra.h
	$(CC) $(CFLAGS) -Isupport/c "$<" build/c/euler_common.o -o "$@" $(LDFLAGS) $(LDLIBS)
rust-release:
	cargo build --release --bins --manifest-path $(RUST_MANIFEST)
$(STARTUP_C): support/bench/noop.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) "$<" -o "$@" $(LDFLAGS)
$(STARTUP_RUST): support/bench/noop.rs
	@mkdir -p "$(@D)"
	rustc --edition=2024 -C opt-level=3 -C lto=thin "$<" -o "$@"
forth-check:
	@command -v gforth >/dev/null || { echo "Install 64-bit Gforth to run Forth solutions"; exit 1; }
factor-check:
	@test -x "$(FACTOR_VM)" || { echo "Factor language VM missing: set FACTOR_VM to the executable, NOT GNU factor"; exit 1; }

build/tests/c_helpers: tests/c_helpers.c support/c/euler_common.c support/c/euler_common.h solutions/c/number_letter_counts_17.c solutions/c/counting_sundays_19.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -UNDEBUG -Isupport/c "$<" support/c/euler_common.c -o "$@" $(LDFLAGS) $(LDLIBS)
build/tests/c_extra_helpers: tests/c_extra_helpers.c support/c/euler_extra.h
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -UNDEBUG -Isupport/c "$<" -o "$@" $(LDFLAGS) $(LDLIBS)
build/tests/c_%_examples: tests/c_%_examples.c support/c/euler_extra.h $(C_FILES)
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -UNDEBUG -Isupport/c "$<" -o "$@" $(LDFLAGS) $(LDLIBS)
unit-c: build/tests/c_helpers build/tests/c_extra_helpers $(C_EXAMPLE_TESTS)
	./build/tests/c_helpers
	./build/tests/c_extra_helpers
	@set -e; for example_test in $(C_EXAMPLE_TESTS); do ./$$example_test; done
unit-rust:
	cargo test --release --lib --bins --manifest-path $(RUST_MANIFEST)
unit-forth: forth-check
	gforth solutions/forth/test-big.fs
	gforth solutions/forth/test-solvers.fs
unit-python:
	$(PYTHON) -m unittest discover -s tests -p 'test_*.py'
unit: unit-c unit-rust unit-forth unit-python

test: c-release rust-release forth-check factor-check check-c-coverage check-inputs
	$(PYTHON) scripts/bench.py --mode test --languages c rust forth factor
test-c: c-release check-c-coverage check-inputs
	$(PYTHON) scripts/bench.py --mode test --timeout $(TEST_TIMEOUT) --languages c
test-rust: rust-release
	$(PYTHON) scripts/bench.py --mode test --languages rust
test-forth: forth-check
	$(PYTHON) scripts/bench.py --mode test --languages forth
test-factor: factor-check
	$(PYTHON) scripts/bench.py --mode test --languages factor

# Preserve BOTH validation and plotting errors. Shell dollars require $$ in Make.
define profile_command
	@status=0; $(PYTHON) scripts/bench.py --mode profile --timeout $(PROFILE_TIMEOUT) --startup-samples $(STARTUP_SAMPLES) --languages $(1) || status=$$?; \
	plot_status=0; $(MAKE) plots || plot_status=$$?; \
	if [ $$status -ne 0 ]; then exit $$status; else exit $$plot_status; fi
endef
# Profile explicit manifest coverage: C/Rust 1–255; Gforth 1–120; Factor 191 entries.
# Uses one shared run ID and generates all cross-language plots; never mixes runs.
run_profile_all: profile
setup-arch:
	./setup.sh
check-arch-setup:
	./setup.sh --check
profile: c-release rust-release $(STARTUP_C) $(STARTUP_RUST) forth-check factor-check check-c-coverage check-inputs
	$(call profile_command,c rust forth factor)
profile-c: c-release $(STARTUP_C) check-c-coverage check-inputs
	$(call profile_command,c)
profile-rust: rust-release $(STARTUP_RUST)
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

.PHONY: check-c-coverage check-inputs fetch-inputs run-c sanitize-c catalog-c audit-c
check-c-coverage:
	$(PYTHON) scripts/check_c_coverage.py
check-inputs:
	$(PYTHON) scripts/euler_inputs.py
fetch-inputs:
	$(PYTHON) scripts/euler_inputs.py --download
run-c:
	$(PYTHON) scripts/run_c.py $(PROBLEM)
sanitize-c: check-c-coverage check-inputs
	$(PYTHON) scripts/sanitize_c.py
catalog-c:
	$(PYTHON) scripts/c_catalog.py
audit-c:
	$(PYTHON) scripts/audit_c.py
scaffolds:
	$(PYTHON) scripts/scaffold_euler.py
check-scaffolds:
	$(PYTHON) scripts/scaffold_euler.py --check
refresh-scaffolds:
	$(PYTHON) scripts/scaffold_euler.py --refresh
