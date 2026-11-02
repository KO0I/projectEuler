# Factor 121–255 bounded-port exception ledger

Factor has 71 validated implementations in this 135-problem range. The remaining
64 files stay as explicit failing scaffolds and are absent from answer-test and
profile coverage. They were not replaced with answer literals, Rust/C subprocess
wrappers, or silent success stubs.

The bounded porting pass first reused 16 native BSD-licensed Project Euler
vocabularies shipped with the repository's Factor image, then implemented 55 more
algorithms in `euler.extended`. The completed set includes dense dynamic programs
(171, 181, 189, 208, 217, 240), number-theory searches (135, 139, 159, 182, 202,
234, 251), and the four-million-cell directional scan for 149. Their measured
maximum was 17.932 seconds, below the 60-second ceiling.

## Deferred after algorithm review

These problems had a known C/Rust approach, but a direct Factor translation would
require a substantial specialized state/search implementation that was not clear
enough to finish and validate in the bounded pass:

`122, 126, 127, 141, 142, 143, 146, 147, 152, 153, 155, 156, 160, 161, 165,
166, 167, 168, 170, 172, 176, 177, 180, 184, 185, 192, 195, 198, 200, 220,
221, 223, 224, 226, 233, 236, 238, 241, 244, 245, 246, 247, 248, 254`.

The attempted process for this group was to inspect the validated C and Rust
algorithm, identify a Factor-native state representation, and estimate whether
the result could be tested inside the 60-second limit. Work moved on when the
port required new exact-rational geometry, large meet-in-the-middle state,
specialized heap/search machinery, or an unbounded amount of debugging.

## Deferred for projected memory or runtime cost

These validated algorithms depend on large dense sieves/tables, very wide scans,
or high-volume geometric/event processing. Factor's boxed general arrays would
make a literal translation materially larger or slower; no run was claimed for
them because no implementation reached a trustworthy executable state:

`136, 154, 179, 187, 193, 196, 201, 210, 211, 212, 213, 214, 216, 222, 229,
231, 249, 250, 252, 253`.

The manifest and scaffold generator are the source of truth: adding a real
`solutions/factor/pN.factor` entrypoint removes that problem's scaffold and makes
it eligible for declared coverage. The next useful optimization before expanding
coverage is to split `euler.extended` into smaller vocabularies so each Factor
process compiles only the solver family it uses.
