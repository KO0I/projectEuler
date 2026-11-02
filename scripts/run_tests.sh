#!/usr/bin/env bash
# Hermetic test entry point for ProjectEuler-multilang.
set -euo pipefail

repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$repo_root"

# Cargo's package cache is build input, not application state.  Remember it
# before replacing HOME, then force offline use so this nominally offline suite
# can neither refresh the registry nor depend on network availability.
caller_home=${HOME-}
caller_cargo_home=${CARGO_HOME:-${caller_home}/.cargo}

# Give libraries a writable, disposable home/cache instead of reading developer
# configuration or writing under the caller's real HOME.  The trap's target is
# the explicit directory returned by mktemp, never a broad or unresolved path.
test_home=$(mktemp -d)
trap 'rm -rf -- "$test_home"' EXIT
export HOME="$test_home"
export XDG_CACHE_HOME="$test_home/.cache"
export MPLCONFIGDIR="$test_home/.cache/matplotlib"
mkdir -p "$XDG_CACHE_HOME" "$MPLCONFIGDIR"

# Keep output and subprocess behavior reproducible across developer machines and CI.
export TZ=UTC
export LANG=C.UTF-8
export LC_ALL=C.UTF-8
export PYTHONHASHSEED=0
if [[ -d $caller_cargo_home ]]; then
    export CARGO_HOME=$caller_cargo_home
    export CARGO_NET_OFFLINE=true
fi

# This suite is offline. Proxy variables can accidentally make a missing local input
# look recoverable on one machine while failing elsewhere.
unset HTTP_PROXY HTTPS_PROXY ALL_PROXY http_proxy https_proxy all_proxy
unset OPENAI_API_KEY ANTHROPIC_API_KEY GEMINI_API_KEY GOOGLE_API_KEY
unset AWS_ACCESS_KEY_ID AWS_SECRET_ACCESS_KEY AWS_SESSION_TOKEN

run_language_range() {
    local language=$1 first=$2 last=$3
    case "$language" in
        c) make c-release check-c-coverage check-inputs ;;
        rust) make rust-release ;;
        forth) make forth-check ;;
        factor) make factor-check ;;
        *) echo "unsupported language: $language" >&2; return 2 ;;
    esac
    # bench.py consults explicit manifest coverage, so a sparse Factor range
    # executes implemented entries only and never mistakes a scaffold for a test.
    python3 scripts/bench.py --mode test --timeout 60 --languages "$language" \
        --problem-range "$first" "$last"
}

if [[ ${1-} == --c-range ]]; then
    if (($# != 3)); then
        echo "usage: scripts/run_tests.sh --c-range FIRST LAST" >&2
        exit 2
    fi
    run_language_range c "$2" "$3"
    exit $?
fi

if [[ ${1-} == --language-range ]]; then
    if (($# != 4)); then
        echo "usage: scripts/run_tests.sh --language-range LANGUAGE FIRST LAST" >&2
        exit 2
    fi
    run_language_range "$2" "$3" "$4"
    exit $?
fi

if [[ ${1-} == --extended-range ]]; then
    if (($# != 1)); then
        echo "usage: scripts/run_tests.sh --extended-range" >&2
        exit 2
    fi
    # Rust is complete in 121..255.  Factor is intentionally sparse and its
    # exact implemented set is declared by tests/answers.json.
    make rust-release factor-check
    python3 scripts/bench.py --mode test --timeout 60 --languages rust factor \
        --problem-range 121 255
    exit $?
fi

if (($#)); then
    # unittest accepts file paths, module names, and fully-qualified test names.
    python3 -m unittest "$@"
    exit $?
fi

make check-scaffolds
make check-inputs
make unit-c
make unit-python
make test-c
