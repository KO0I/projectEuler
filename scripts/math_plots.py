#!/usr/bin/env python3
"""Reproducible mathematical visualizations with explicitly labelled sampling."""
from __future__ import annotations
import math
from pathlib import Path
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "build" / "math_plots"


def save(fig, stem):
    OUT.mkdir(parents=True, exist_ok=True)
    for ext in ("svg", "png", "pdf"):
        fig.savefig(OUT / f"{stem}.{ext}", dpi=200, bbox_inches="tight")
    plt.close(fig)


def sieve(limit):
    prime = bytearray(b"\x01") * (limit + 1)
    prime[0:2] = b"\0\0"
    for p in range(2, math.isqrt(limit) + 1):
        if prime[p]:
            prime[p*p:limit+1:p] = b"\0" * ((limit-p*p)//p+1)
    return prime


def ulam(side=101):
    assert side % 2
    grid = np.zeros((side, side), dtype=int)
    row = col = side // 2
    grid[row, col] = 1
    n = 1
    # Spiral extends right, down, left, up in Cartesian image coordinates.
    drdc = ((0,1),(1,0),(0,-1),(-1,0))
    step = 1
    direction = 0
    while n < side*side:
        for _ in range(2):
            dr,dc = drdc[direction % 4]
            for _ in range(step):
                row, col = row+dr, col+dc
                n += 1
                if 0 <= row < side and 0 <= col < side:
                    grid[row,col] = n
            direction += 1
        step += 1
    prime = sieve(side*side)
    mask = np.zeros_like(grid)
    for r in range(side):
        for c in range(side):
            if grid[r,c] and prime[int(grid[r,c])]:
                mask[r,c] = 1
    fig, ax = plt.subplots(figsize=(7,7))
    ax.imshow(mask, cmap="Greys", interpolation="nearest", vmin=0, vmax=1)
    ax.plot([0,side-1],[0,side-1], color="#b73535", linestyle="--", linewidth=.7)
    ax.plot([0,side-1],[side-1,0], color="#b73535", linestyle=":", linewidth=.8)
    ax.set_title(f"Ulam spiral: primes within {side} x {side} grid\n"
                 "Diagonals highlighted; sampling only (Euler 28 uses 1001 x 1001)")
    ax.set_xticks([]); ax.set_yticks([])
    save(fig, "ulam_spiral")


def spiral_diagonals(side=1001):
    sizes = np.arange(1,side+1,2,dtype=np.int64)
    # Sum of four corners of ring of side s >1: 4*s^2 - 6*(s-1)
    corners = 4*sizes[1:]**2 - 6*(sizes[1:]-1)
    cumulative = np.concatenate(([1],1 + np.cumsum(corners)))
    fig,ax=plt.subplots(figsize=(8,5))
    ax.plot(sizes,cumulative,linestyle="-",linewidth=1.6,color="#294b6d")
    ax.scatter([side],[cumulative[-1]],marker="s",color="#222",s=35)
    ax.set_xlabel("Spiral side length [cells]")
    ax.set_ylabel("Cumulative diagonal sum")
    ax.set_title(f"Euler 28 diagonal sums; {side} x {side}: {int(cumulative[-1])}")
    ax.grid(linestyle=":",alpha=.5)
    save(fig,"spiral_diagonal_sum")


def collatz(limit=10000):
    cache={1:1}
    def length(n):
        trail=[]
        while n not in cache:
            trail.append(n)
            n = n//2 if n%2==0 else 3*n+1
        length_so_far=cache[n]
        for k in reversed(trail):
            length_so_far += 1
            cache[k]=length_so_far
        return length_so_far
    lengths = np.array([length(n) for n in range(1,limit+1)])
    fig,ax=plt.subplots(figsize=(11,5))
    ax.scatter(np.arange(1,limit+1),lengths,s=1,color="#3a5675",alpha=.65)
    ax.set_xlabel("Starting value")
    ax.set_ylabel("Chain length [terms]")
    ax.set_title(f"Euler 14: Collatz chain lengths for starts 1..{limit} (sample)")
    ax.grid(linestyle=":",alpha=.3)
    save(fig,"collatz_lengths")


def prime_distribution(limit=50000):
    p=sieve(limit)
    x=np.arange(limit+1)
    c=np.cumsum(np.frombuffer(p,dtype=np.uint8),dtype=np.int64)
    s=np.cumsum(x*np.frombuffer(p,dtype=np.uint8),dtype=np.int64)
    fig,ax=plt.subplots(figsize=(10,5))
    ax.plot(x,c,linestyle="-",color="#174b69",label="Prime count π(x)")
    ax.set_xlabel("Upper bound x")
    ax.set_ylabel("Prime count",color="#174b69")
    bx=ax.twinx()
    bx.plot(x,s,linestyle="--",color="#865431",label="Cumulative prime sum")
    bx.set_ylabel("Sum of primes ≤ x",color="#865431")
    ax.set_title(f"Euler 10: prime count and prime sum, sample up to {limit}")
    ax.grid(linestyle=":",alpha=.3)
    save(fig,"prime_distribution")


def quadratic_primes(bound=100):
    prime=sieve(200000)
    arr=np.zeros((2*bound+1,2*bound+1),dtype=np.int16)
    for ai,a in enumerate(range(-bound,bound+1)):
        for bi,b in enumerate(range(-bound,bound+1)):
            n=0
            while True:
                value=n*n+a*n+b
                if value<2 or value>=len(prime) or not prime[value]:
                    break
                n+=1
            arr[bi,ai]=n
    fig,ax=plt.subplots(figsize=(8,7))
    im=ax.imshow(arr,origin="lower",extent=(-bound,bound,-bound,bound),
                 cmap="Greys",interpolation="nearest",aspect="auto")
    fig.colorbar(im,ax=ax,label="Consecutive primes from n=0 [terms]")
    ax.set_xlabel("Coefficient a")
    ax.set_ylabel("Coefficient b")
    ax.set_title(f"Euler 27: quadratic prime run lengths (|a|,|b| ≤ {bound} sample)")
    save(fig,"quadratic_prime_heatmap")


if __name__=="__main__":
    ulam()
    spiral_diagonals()
    collatz()
    prime_distribution()
    quadratic_primes()
    print(f"Saved mathematical figures to {OUT}")
