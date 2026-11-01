<# Build and test Project Euler under Windows (MSYS2/UCRT64 suggested).
Requires: Python 3, GCC + GMP headers and libraries, Rust + Cargo, Gforth.
Use: ./scripts/test.ps1 -Action test|profile|plots|plots-math
#>
param(
  [ValidateSet('test','test-c','test-rust','test-forth','profile','profile-c','profile-rust','profile-forth','plots','plots-math','clean')]
  [string]$Action = 'test'
)
$ErrorActionPreference = 'Stop'
$Root = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
Set-Location $Root

function Invoke-Checked([string]$Tool, [string[]]$Arguments) {
    & $Tool @Arguments
    if ($LASTEXITCODE -ne 0) { throw "$Tool failed with exit code $LASTEXITCODE" }
}
function Build-C {
    New-Item -ItemType Directory -Force -Path 'build/c' | Out-Null
    foreach ($source in (Get-ChildItem 'solutions/c' -Filter '*.c')) {
        $stem = [System.IO.Path]::GetFileNameWithoutExtension($source.Name)
        Invoke-Checked 'gcc' @('-O2','-std=c11','-Wall','-Wextra','-Isupport/c', $source.FullName,
            'support/c/euler_common.c','-o',"build/c/$stem.exe",'-lgmp','-lm')
    }
}
function Build-Rust {
    Invoke-Checked 'cargo' @('build','--release','--bins','--manifest-path','solutions/rust/Cargo.toml')
}
function Check-Forth {
    if (-not (Get-Command 'gforth' -ErrorAction SilentlyContinue)) { throw 'Gforth is missing' }
}

switch ($Action) {
    'test' {
        Build-C; Build-Rust; Check-Forth
        Invoke-Checked 'python' (@('scripts/bench.py','--mode','test','--languages','c','rust','forth'))
    }
    'test-c' { Build-C; Invoke-Checked 'python' @('scripts/bench.py','--mode','test','--languages','c') }
    'test-rust' { Build-Rust; Invoke-Checked 'python' @('scripts/bench.py','--mode','test','--languages','rust') }
    'test-forth' { Check-Forth; Invoke-Checked 'python' @('scripts/bench.py','--mode','test','--languages','forth') }
    'profile' {
        Build-C; Build-Rust; Check-Forth
        try { Invoke-Checked 'python' @('scripts/bench.py','--mode','profile','--languages','c','rust','forth') }
        finally { Invoke-Checked 'python' @('scripts/plots.py') }
    }
    'profile-c' { Build-C; try {Invoke-Checked 'python' @('scripts/bench.py','--mode','profile','--languages','c')} finally {Invoke-Checked 'python' @('scripts/plots.py')} }
    'profile-rust' { Build-Rust; try {Invoke-Checked 'python' @('scripts/bench.py','--mode','profile','--languages','rust')} finally {Invoke-Checked 'python' @('scripts/plots.py')} }
    'profile-forth' { Check-Forth; try {Invoke-Checked 'python' @('scripts/bench.py','--mode','profile','--languages','forth')} finally {Invoke-Checked 'python' @('scripts/plots.py')} }
    'plots' { Invoke-Checked 'python' @('scripts/plots.py') }
    'plots-math' { Invoke-Checked 'python' @('scripts/math_plots.py') }
    'clean' { Remove-Item -Force -Recurse 'build' -ErrorAction SilentlyContinue; Invoke-Checked 'cargo' @('clean','--manifest-path','solutions/rust/Cargo.toml') }
}
