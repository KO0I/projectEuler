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


def trinomial(n, i, j):
    """Return n!/(i!j!k!) for k=n-i-j using exact integer arithmetic."""
    k = n - i - j
    if min(i, j, k) < 0:
        raise ValueError("trinomial indices must be nonnegative and sum to n")
    # Choosing i positions and then j of the remaining positions avoids
    # constructing three factorials.  Python's comb keeps the result exact.
    return math.comb(n, i) * math.comb(n - i, j)


def pascal_pyramid_coefficients(max_layer=14):
    """Return (i,j,k,value) for every entry through one pyramid layer."""
    points = []
    for n in range(max_layer + 1):
        for i in range(n + 1):
            for j in range(n - i + 1):
                k = n - i - j
                points.append((i, j, k, trinomial(n, i, j)))
    return points


def pascal_pyramid_tetrahedron(max_layer=14):
    """Plot all trinomial coefficients as the integer points of a tetrahedron."""
    points = pascal_pyramid_coefficients(max_layer)
    i, j, k, values = (np.asarray(column) for column in zip(*points))
    log_values = np.log10(values.astype(float))

    fig = plt.figure(figsize=(9, 7))
    ax = fig.add_subplot(111, projection="3d")
    scatter = ax.scatter(
        i, j, k, c=log_values, cmap="cividis", s=12 + 8 * log_values,
        alpha=.82, depthshade=False, edgecolors="none")
    fig.colorbar(scatter, ax=ax, shrink=.68, pad=.08,
                 label=r"Coefficient magnitude $\log_{10}{n!/(i!j!k!)}$")

    # The coordinate planes and i+j+k=max_layer bound the plotted tetrahedron.
    vertices = np.array(((0, 0, 0), (max_layer, 0, 0),
                         (0, max_layer, 0), (0, 0, max_layer)))
    for a, b in ((0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)):
        ax.plot(*zip(vertices[a], vertices[b]), color="#303030", linewidth=.8)
    ax.set_xlabel("Exponent i")
    ax.set_ylabel("Exponent j")
    ax.set_zlabel("Exponent k")
    ax.set_title(
        f"Pascal's pyramid through layer {max_layer}\n"
        r"Each point is the coefficient of $x^iy^jz^k$ in $(x+y+z)^{i+j+k}$")
    ax.view_init(elev=24, azim=38)
    ax.set_box_aspect((1, 1, 1))
    save(fig, "pascal_pyramid_tetrahedron")


def pascal_pyramid_layer(layer=24):
    """Plot one triangular layer as a 3D log-coefficient surface."""
    xs, ys, heights = [], [], []
    for i in range(layer + 1):
        for j in range(layer - i + 1):
            k = layer - i - j
            # Equilateral-triangle coordinates preserve the permutation
            # symmetry among i, j, and k on the horizontal plane.
            xs.append(j + .5 * k)
            ys.append(math.sqrt(3) * .5 * k)
            heights.append(math.log10(trinomial(layer, i, j)))

    fig = plt.figure(figsize=(9, 7))
    ax = fig.add_subplot(111, projection="3d")
    surface = ax.plot_trisurf(xs, ys, heights, cmap="cividis",
                              linewidth=.16, edgecolor="#555555", antialiased=True)
    fig.colorbar(surface, ax=ax, shrink=.68, pad=.08,
                 label=r"Coefficient magnitude $\log_{10}{n!/(i!j!k!)}$")
    ax.set_xlabel("Barycentric direction j")
    ax.set_ylabel("Barycentric direction k")
    ax.set_zlabel(r"$\log_{10}$ coefficient")
    ax.set_title(
        f"Pascal's pyramid, layer n={layer}\n"
        "The central coefficients dominate the three symmetric edges")
    ax.view_init(elev=28, azim=-55)
    save(fig, "pascal_pyramid_layer_surface")


def pascal_pyramid_parity(max_layer=31):
    """Plot the odd coefficients, which form a tetrahedral mod-2 fractal."""
    odd = []
    for i, j, k, value in pascal_pyramid_coefficients(max_layer):
        if value & 1:
            odd.append((i, j, k, i + j + k))
    i, j, k, layers = (np.asarray(column) for column in zip(*odd))

    fig = plt.figure(figsize=(9, 7))
    ax = fig.add_subplot(111, projection="3d")
    scatter = ax.scatter(i, j, k, c=layers, cmap="Greys", vmin=0,
                         vmax=max_layer, s=9, alpha=.86, depthshade=False)
    fig.colorbar(scatter, ax=ax, shrink=.68, pad=.08, label="Layer n")
    ax.set_xlabel("Exponent i")
    ax.set_ylabel("Exponent j")
    ax.set_zlabel("Exponent k")
    ax.set_title(
        f"Odd entries of Pascal's pyramid through layer {max_layer}\n"
        "Lucas parity leaves a self-similar tetrahedral pattern")
    ax.view_init(elev=23, azim=40)
    ax.set_box_aspect((1, 1, 1))
    save(fig, "pascal_pyramid_parity")


if __name__=="__main__":
    ulam()
    spiral_diagonals()
    collatz()
    prime_distribution()
    quadratic_primes()
    pascal_pyramid_tetrahedron()
    pascal_pyramid_layer()
    pascal_pyramid_parity()
    from euler_visuals import make_all
    make_all()
    print(f"Saved mathematical figures to {OUT}")
