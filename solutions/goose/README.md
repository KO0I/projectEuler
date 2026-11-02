# Run and measure Goose solutions

Goose covers problems 1–100. Each `<problem_title>_<number>.goose` file owns its
algorithm and prints one exact answer. `support/goose/euler.goose` contains
reused math, decimal big integers, and input readers. These programs do not
call the C or Rust solvers.

Build the pinned compiler and check the answers from the repository root:

```sh
make setup-goose
make unit-goose test-goose
make profile-goose
```

On Arch Linux, `./setup.sh` also installs the compiler build dependencies and
builds Goose. `make run_profile_all` includes Goose alongside the existing
languages. To use a compiler you already built, set `GOOSE=/absolute/path/to/goose`.

The pinned compiler commit is
`eadca409f68210893f366e60d0c2f0dc29a339be` from
[aardappel/goose](https://github.com/aardappel/goose). Building it requires Git,
a C++20 compiler, and CMake 3.27 or later. Optional graphics packages are excluded.

To measure a complete rebuild separately:

```sh
python3 scripts/build_goose.py --force
```

`build/goose/compilation.json` records each build invocation, the compiler,
the C backend, and hashes of the build inputs. Native builds use GCC `-O2`.
An unchanged build preserves the last compilation report. The cached runtime
object may already exist, so these are not cold toolchain installation timings.

The profiler launches each standalone executable once, checks its exact output,
and records total child-process wall time in `[ms]`. Compilation is excluded.
A separate no-op executable estimates the minimal process startup baseline.
The residual includes input loading, computation, and shutdown. With one
observation per solution, the report does not claim statistical error bars.
Goose's JIT runner is not used for performance measurements.

Reports are under `build/profile/<run_id>/`. The saved
[Goose measurement record](../../docs/GOOSE_1_100.md) contains the measured run
and its qualifications. Algorithms differ between languages, so a runtime ratio
does not isolate the compiler's performance.

Problem 84 uses independent uniform card draws, rather than persistent shuffled
decks. Problem 93 enumerates expression trees with `f64` arithmetic and a
`1e-7` integer tolerance. Big integers have capacity for 1100 decimal digits;
operations abort if that capacity is exceeded.
