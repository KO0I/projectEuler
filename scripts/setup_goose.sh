#!/usr/bin/env bash
# Reproducible minimal compiler: no optional graphics/audio dependencies.
set -euo pipefail
ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
PIN=eadca409f68210893f366e60d0c2f0dc29a339be
DEST="$ROOT/.toolchains/goose"
if [[ -n "${GOOSE:-}" && "$GOOSE" != "$DEST/build/goose" ]]; then
  "$GOOSE" version
  exit 0
fi
if [[ ! -d "$DEST/.git" ]]; then
  git clone https://github.com/aardappel/goose.git "$DEST"
fi
if [[ -n "$(git -C "$DEST" status --porcelain --untracked-files=no)" ]]; then
  echo 'Goose checkout has local edits; refusing to replace them.' >&2
  exit 1
fi
git -C "$DEST" fetch origin "$PIN"
git -C "$DEST" checkout --detach "$PIN"
cmake -S "$DEST" -B "$DEST/build" -DCMAKE_BUILD_TYPE=Release
cmake --build "$DEST/build" --parallel "${GOOSE_BUILD_JOBS:-2}"
"$DEST/build/goose" version
