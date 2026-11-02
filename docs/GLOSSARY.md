# Project Euler glossary

**Historical Attempt:** The source code, mathematical strategy, intermediate files, and
unsuccessful experiments initially committed to the repository. Git history and the
existing variants directories preserve the original implementation.

**Corrected Solution:** Source that implements the intended problem algorithm
after addressing a known defect. This status alone is not evidence of correctness.

**Validated Solution:** A buildable program that prints exactly one signed or
canonical one-line answer string and whose exit status and answer match the versioned
fixture in \`tests/answers.json\`. Edge tests and toolchain availability must also
be reported; passing one reference answer does not establish a proof.

**Solution Scaffold:** An explicitly incomplete language-specific starter file
containing official problem metadata and an entrypoint shape. It is excluded from
implementation coverage, builds, answer validation, and profiling until replaced
by a solution.

**Language Port:** An independently implemented solution in C, Rust, or Forth
with the same problem semantics and reference answer. It need not duplicate
control flow or data representation from another language.

**Profile Run:** One invocation of a solution through an external
\`time.perf_counter_ns()\` timer, including startup, excluding compilation.
One observation is not a distribution: no uncertainty bars or confidence
intervals are derived from it.

**Suite Wall Time:** Elapsed time for a runner invocation, including Python
overhead, scheduling, I/O and all invoked child processes. This is distinct
from the sum of per-problem child runtimes.

**Mathematical Figure:** A deterministic plot of a problem-specific mathematical
construction, not a performance benchmark; any sample bounds must be labelled.
