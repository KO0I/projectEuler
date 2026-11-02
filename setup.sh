#!/usr/bin/env bash
# Arch Linux native benchmark dependencies for C, Rust, Gforth, and Factor.
# Factor is a standalone language distribution, not GNU coreutils 'factor'.
set -euo pipefail

ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT"

usage() {
  cat <<'HELP'
Usage: ./setup.sh [--check] [--no-pacman]
  (default)    Install Arch dependencies and Factor 0.102 on x86_64.
  --check      Verify local prerequisites without installing anything.
  --no-pacman  Skip pacman; set up Factor and check the existing tools.

Optional environment:
  FACTOR_VM=/path/to/factor  Use an existing Factor language runtime.
  FACTOR_VERSION=0.102      Official Factor release to download (default).
HELP
}

CHECK_ONLY=0
NO_PACMAN=0
for arg in "$@"; do
  case "$arg" in
    --check) CHECK_ONLY=1 ;;
    --no-pacman) NO_PACMAN=1 ;;
    -h|--help) usage; exit 0 ;;
    *) printf 'Unknown option: %s\n' "$arg" >&2; usage >&2; exit 2 ;;
  esac
done

if [[ "$(uname -s)" != Linux ]]; then
  echo 'This installer targets Arch Linux. Use the existing Makefile for other systems.' >&2
  exit 1
fi
if [[ "$CHECK_ONLY" == 0 ]] && [[ "$NO_PACMAN" == 0 ]]; then
  if ! command -v pacman >/dev/null 2>&1; then
    echo 'pacman is unavailable. This script supports Arch Linux (or --no-pacman).' >&2
    exit 1
  fi
  packages=(base-devel gmp gforth rust python python-numpy python-matplotlib curl tar)
  echo "Installing Arch packages: ${packages[*]}"
  if [[ "${EUID}" -eq 0 ]]; then
    pacman -S --needed --noconfirm "${packages[@]}"
  else
    command -v sudo >/dev/null || { echo 'sudo is required to install Arch packages.' >&2; exit 1; }
    sudo pacman -S --needed --noconfirm "${packages[@]}"
  fi
fi

factor_dir="$ROOT/.toolchains/factor"
factor_vm="${FACTOR_VM:-$factor_dir/factor}"
if [[ "$CHECK_ONLY" == 0 ]] && [[ ! -x "$factor_vm" ]]; then
  if [[ -n "${FACTOR_VM:-}" ]]; then
    echo "FACTOR_VM was supplied but is not executable: $factor_vm" >&2
    exit 1
  fi
  if [[ "$(uname -m)" != x86_64 ]]; then
    echo 'Official prebuilt Factor 0.102 for Linux requires x86_64. Supply FACTOR_VM for your architecture.' >&2
    exit 1
  fi
  command -v curl >/dev/null || { echo 'curl is required to download Factor.' >&2; exit 1; }
  version="${FACTOR_VERSION:-0.102}"
  if [[ ! "$version" =~ ^[0-9]+\.[0-9]+$ ]]; then
    echo 'FACTOR_VERSION must be a numeric release, e.g. 0.102.' >&2
    exit 1
  fi
  url="https://downloads.factorcode.org/releases/${version}/factor-linux-x86-64-${version}.tar.gz"
  mkdir -p "$ROOT/.toolchains"
  tempdir="$(mktemp -d "$ROOT/.toolchains/.factor-download.XXXXXX")"
  trap 'rm -rf -- "$tempdir"' EXIT
  echo "Downloading Factor programming language ${version} from ${url}"
  curl --fail --location --retry 3 --show-error --silent "$url" -o "$tempdir/factor.tar.gz"
  mkdir "$tempdir/extracted"
  tar -xzf "$tempdir/factor.tar.gz" -C "$tempdir/extracted"
  candidate="$(find "$tempdir/extracted" -type f -name factor -perm -u=x -print -quit)"
  if [[ -z "$candidate" ]]; then
    echo 'Factor executable missing from archive; installation aborted.' >&2
    exit 1
  fi
  source_dir="$(dirname "$candidate")"
  if [[ ! -f "$source_dir/factor.image" ]]; then
    echo 'Factor image missing alongside executable; installation aborted.' >&2
    exit 1
  fi
  if [[ -e "$factor_dir" ]]; then
    echo "Installation destination already exists: $factor_dir" >&2
    echo 'Set FACTOR_VM to your existing VM, or remove the old installation yourself.' >&2
    exit 1
  fi
  mv -- "$source_dir" "$factor_dir"
  factor_vm="$factor_dir/factor"
  echo "Installed Factor to $factor_dir"
fi

failed=0
for cmd in gcc make cargo rustc gforth python3; do
  if command -v "$cmd" >/dev/null 2>&1; then
    printf '  %-12s %s\n' "$cmd" "$(command -v "$cmd")"
  else
    echo "  MISSING: $cmd" >&2
    failed=1
  fi
done
if command -v python3 >/dev/null 2>&1; then
  if python3 -c 'import numpy, matplotlib' >/dev/null 2>&1; then
    echo '  numpy/matplotlib: available'
  else
    echo '  MISSING: Python numpy and/or matplotlib' >&2
    failed=1
  fi
fi
if [[ -x "$factor_vm" ]]; then
  printf '  %-12s %s\n' 'Factor VM' "$factor_vm"
  if ! "$factor_vm" -help >/dev/null 2>&1; then
    echo 'Factor executable exists but could not start; check shared libraries with ldd.' >&2
    failed=1
  fi
else
  echo "  MISSING: Factor language VM ($factor_vm)" >&2
  failed=1
fi
if [[ "$failed" != 0 ]]; then
  echo 'Some prerequisites are unavailable; run ./setup.sh (or supply FACTOR_VM).' >&2
  exit 1
fi
printf '\nReady: make run_profile_all\n'
