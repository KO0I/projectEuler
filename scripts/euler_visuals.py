#!/usr/bin/env python3
"""Reproducible Euler mathematics: complete answers only for listed, verified cases.

Toy examples are explicitly labelled, and never masquerade as full problem inputs.
This complements math_plots.py and does not time or run solution binaries.
"""
from __future__ import annotations
from collections import Counter
from functools import lru_cache
from heapq import heappop, heappush
from itertools import combinations_with_replacement
from math import comb, gcd, isqrt
from pathlib import Path
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "build/math_plots"

def save(fig, name):
    OUT.mkdir(parents=True, exist_ok=True)
    for ext in ("svg", "png", "pdf"):
        fig.savefig(OUT / (name + "." + ext), dpi=180, bbox_inches="tight")
    plt.close(fig)

def triangle_perimeters():
    P = 1500
    counts = np.zeros(P + 1, dtype=int)
    for m in range(2, isqrt(P // 2) + 2):
        for n in range(1, m):
            p = 2 * m * (m + n)
            if p > P or (m - n) % 2 == 0 or gcd(m, n) != 1:
                continue
            counts[p::p] += 1
    fig, (a, b) = plt.subplots(1, 2, figsize=(13, 4.5))
    x = np.arange(12, 1001)
    a.plot(x, counts[12:1001], ".", markersize=2, color="0.25")
    a.axvline(840, color="0.1", ls="--", lw=1)
    a.annotate("p = 840 (Euler 39)", (840, counts[840]), xytext=(560, 12),
               arrowprops={"arrowstyle": "->"})
    a.axvline(1000, color="0.45", ls=":")
    a.set(xlabel="Perimeter p [units]", ylabel="Number of integer right triangles",
          title="Euler 9 / 39: Pythagorean perimeters")
    a.grid(ls=":", alpha=.3)
    b.plot(np.arange(P + 1), np.cumsum(counts == 1), color="0.2")
    b.set(xlabel="Maximum perimeter [units]",
          ylabel="Perimeters with exactly one triangle",
          title="Euler 75: singular right-triangle perimeters (sample <= 1500)")
    b.grid(ls=":", alpha=.3)
    fig.suptitle("Euclid primitive triples; integer multiples counted once")
    save(fig, "pythagorean_perimeters")

def grid_products():
    data = np.fromstring((ROOT / "tests/data/p011.txt").read_text(),
                         dtype=int, sep=" ")
    assert data.size == 400
    grid = data.reshape(20, 20)
    best = (-1, None)
    for row in range(20):
        for col in range(20):
            for dx, dy in ((1, 0), (0, 1), (1, 1), (-1, 1)):
                cells = [(row + k * dy, col + k * dx) for k in range(4)]
                if not all(0 <= y < 20 and 0 <= x < 20 for y, x in cells):
                    continue
                prod = int(np.prod([int(grid[y, x]) for y, x in cells]))
                if prod > best[0]:
                    best = (prod, cells)
    assert best[0] == 70600674
    fig, ax = plt.subplots(figsize=(9.2, 8.5))
    ax.imshow(grid, cmap="Greys", vmin=0, vmax=99)
    for y in range(20):
        for x in range(20):
            ax.text(x, y, str(grid[y, x]), ha="center", va="center", fontsize=6,
                    color="white" if grid[y, x] > 52 else "black")
    cells = best[1]
    ax.plot([x for y, x in cells], [y for y, x in cells],
            color="#be3030", lw=3.2)
    ax.scatter([x for y, x in cells], [y for y, x in cells],
               s=170, facecolors="none", edgecolors="#be3030", linewidth=2)
    ax.set(xticks=range(20), yticks=range(20),
           title=f"Euler 11: four directions; maximum product {best[0]:,}",
           xlabel="Column index", ylabel="Row index")
    save(fig, "grid_largest_product")

def lattice_paths():
    N = 6
    values = np.array([[comb(x + y, x) for x in range(N + 1)]
                       for y in range(N + 1)])
    fig, ax = plt.subplots(figsize=(7.1, 6.5))
    ax.imshow(values, cmap="Greys")
    for y in range(N + 1):
        for x in range(N + 1):
            ax.text(x, y, str(values[y, x]), ha="center", va="center",
                    color="white" if values[y, x] > 220 else "black")
    ax.plot([0, N, N], [0, 0, N], color="#be3030", lw=2, marker="o", ms=3)
    ax.set(xticks=range(N + 1), yticks=range(N + 1),
           xlabel="Rightward moves", ylabel="Downward moves",
           title=f"Euler 15: {N}x{N} lattice teaching example; C(2n,n) = {comb(2*N,N)}")
    save(fig, "lattice_paths")

def maximum_path_triangle():
    tri = [[3], [7, 4], [2, 4, 6], [8, 5, 9, 3]]
    dp = tri[-1][:]
    choices = []
    for row in tri[-2::-1]:
        ch = [int(dp[i + 1] > dp[i]) for i in range(len(row))]
        choices.insert(0, ch)
        dp = [v + dp[i + ch[i]] for i, v in enumerate(row)]
    path = [0]
    index = 0
    for choice in choices:
        index += choice[index]
        path.append(index)
    assert dp[0] == 23
    fig, ax = plt.subplots(figsize=(7, 5.5))
    for r, row in enumerate(tri):
        for c, val in enumerate(row):
            x = c - r / 2
            yes = (c == path[r])
            ax.scatter([x], [r], s=750, color="0.2" if yes else "0.82", zorder=2)
            ax.text(x, r, str(val), ha="center", va="center",
                    color="white" if yes else "black", fontsize=13, zorder=3)
            if r < 3:
                for child in (c, c + 1):
                    ax.plot([x, child - (r + 1)/2], [r, r + 1],
                            color="0.7", lw=1, zorder=0)
    ax.plot([c - r/2 for r, c in enumerate(path)], range(4),
            color="#be3030", lw=2.5, zorder=1)
    ax.invert_yaxis()
    ax.axis("off")
    ax.set_title("Euler 18 / 67: bottom-up maximum path sum\n"
                 "Four-row teaching triangle (sum = 23, not full input)")
    save(fig, "maximum_path_sum_demo")

def is_prime(n):
    if n < 2:
        return False
    if n % 2 == 0:
        return n == 2
    for d in range(3, isqrt(n) + 1, 2):
        if n % d == 0:
            return False
    return True

def spiral_prime_ratio():
    prime_count, diagonal_count = 0, 1
    sides, ratios = [], []
    for layer in range(1, 1251):
        side = 2 * layer + 1
        corner, step = side*side, side - 1
        prime_count += sum(is_prime(corner - k*step) for k in (1, 2, 3))
        diagonal_count += 4
        sides.append(side)
        ratios.append(prime_count / diagonal_count)
    fig, ax = plt.subplots(figsize=(8, 5))
    ax.plot(sides, ratios, color="0.25")
    ax.axhline(.10, color="0.3", ls="--", label="10% threshold")
    ax.set(xlabel="Spiral side length [cells]", ylabel="Fraction prime",
           title="Euler 58: diagonal prime density (sample through side 2501)")
    ax.legend()
    ax.grid(ls=":", alpha=.4)
    save(fig, "spiral_prime_ratio")

def magic_5gon():
    outer = [6, 10, 9, 8, 7]
    inner = [5, 3, 1, 4, 2]
    assert all(outer[k] + inner[k] + inner[(k+1)%5] == 14 for k in range(5))
    theta = np.pi/2 - np.arange(5)*2*np.pi/5
    p_out = np.array([(2*np.cos(t), -2*np.sin(t)) for t in theta])
    p_in = np.array([(np.cos(t), -np.sin(t)) for t in theta])
    fig, ax = plt.subplots(figsize=(7, 6))
    for i in range(5):
        pts = [p_out[i], p_in[i], p_in[(i+1)%5]]
        ax.plot([v[0] for v in pts], [v[1] for v in pts],
                color="0.4", ls=("-", "--", ":", "-.", "-")[i], lw=2)
    for points, nums, color in ((p_out, outer, "0.25"), (p_in, inner, "0.65")):
        ax.scatter(points[:,0], points[:,1], s=800, color=color, zorder=2)
        for (x, y), v in zip(points, nums):
            ax.text(x, y, str(v), ha="center", va="center", zorder=3,
                    color="white" if color == "0.25" else "black", fontsize=12)
    ax.set(aspect="equal",
           title="Euler 68: magic 5-gon; every outer–inner–next line sums to 14")
    ax.axis("off")
    save(fig, "magic_5gon")

MATRIX = np.array([
    [131,673,234,103,18],
    [201,96,342,965,150],
    [630,803,746,422,111],
    [537,699,497,121,956],
    [805,732,524,37,331]])

def shortest_grid(mat, starts, targets, directions):
    h,w = mat.shape
    dist, pred, queue = {}, {}, []
    for y,x in starts:
        dist[(y,x)] = int(mat[y,x])
        heappush(queue, (int(mat[y,x]),y,x))
    targets = set(targets)
    while queue:
        cost,y,x = heappop(queue)
        if cost != dist[(y,x)]:
            continue
        if (y,x) in targets:
            path = []
            at = (y,x)
            while at in pred:
                path.append(at)
                at = pred[at]
            return cost, [at]+path[::-1]
        for dy,dx in directions:
            yy,xx = y+dy,x+dx
            if 0 <= yy < h and 0 <= xx < w:
                proposed = cost + int(mat[yy,xx])
                if proposed < dist.get((yy,xx), 10**50):
                    dist[(yy,xx)] = proposed
                    pred[(yy,xx)] = (y,x)
                    heappush(queue, (proposed,yy,xx))
    raise RuntimeError("No path")

def path_sum_directions():
    m = MATRIX
    n = len(m)
    cases = [
        ("81: right and down", [(0,0)], [(n-1,n-1)], ((0,1),(1,0))),
        ("82: right, up, down", [(r,0) for r in range(n)],
         [(r,n-1) for r in range(n)], ((0,1),(-1,0),(1,0))),
        ("83: all four directions", [(0,0)], [(n-1,n-1)],
         ((0,1),(0,-1),(1,0),(-1,0))) ]
    fig, axes = plt.subplots(1, 3, figsize=(15, 5))
    for ax, (title, start, targets, dirs) in zip(axes, cases):
        cost, path = shortest_grid(m,start,targets,dirs)
        ax.imshow(m, cmap="Greys", vmin=0,vmax=965)
        for y in range(n):
            for x in range(n):
                ax.text(x,y,str(m[y,x]),ha="center",va="center",fontsize=8,
                        color="white" if m[y,x]>520 else "black")
        ax.plot([x for y,x in path], [y for y,x in path],
                lw=3,color="#b93636",marker="o",ms=4)
        ax.set(title=f"Euler {title}\npath sum = {cost}",
               xticks=range(n),yticks=range(n))
    assert shortest_grid(m,[(0,0)],[(4,4)],((0,1),(1,0)))[0] == 2427
    assert shortest_grid(m,[(r,0) for r in range(n)],
                         [(r,4) for r in range(n)],((0,1),(-1,0),(1,0)))[0] == 994
    assert shortest_grid(m,[(0,0)],[(4,4)],((0,1),(0,-1),(1,0),(-1,0)))[0] == 2297
    fig.suptitle("Same 5x5 example matrix; moves differ (not the 80x80 challenges)")
    save(fig, "path_sum_directions")

def monopoly_dice():
    dice = Counter(a+b for a in range(1,5) for b in range(1,5))
    fig,ax = plt.subplots(figsize=(8,4))
    ax.bar(sorted(dice), [dice[n]/16 for n in sorted(dice)],
           edgecolor="black",color="0.65",hatch="///")
    ax.set(xlabel="Sum of two 4-sided dice",ylabel="Probability",
           title="Euler 84: only the 2d4 movement kernel\n"
                 "Cards, jail and doubles rules not simulated")
    ax.set_xticks(range(2,9))
    ax.grid(axis="y",ls=":",alpha=.35)
    save(fig,"monopoly_dice_kernel")

def rectangle_counts():
    sizes = np.arange(1,51)
    width,height = np.meshgrid(sizes,sizes)
    counts = width*(width+1)*height*(height+1)//4
    fig,ax = plt.subplots(figsize=(8,6))
    im = ax.imshow(np.log10(counts),origin="lower",
                   extent=[.5,50.5,.5,50.5],cmap="Greys",aspect="auto")
    contour = ax.contour(width,height,counts,levels=[2000000],
                         colors=["#a33"],linewidths=2)
    ax.clabel(contour,fmt="2,000,000")
    ax.set(xlabel="Grid width [cells]",ylabel="Grid height [cells]",
           title="Euler 85: m(m+1)n(n+1)/4 subrectangles\n"
                 "2 million target contour, grids ≤50x50")
    fig.colorbar(im,ax=ax,label="log10(subrectangle count)")
    save(fig,"counting_rectangles")

def cuboid_routes():
    M=25
    a,b=np.meshgrid(np.arange(1,M+1),np.arange(1,M+1))
    squared=(a+b)**2+M*M
    valid=np.array([[isqrt(int(v))**2==v for v in row] for row in squared])
    fig,ax=plt.subplots(figsize=(7,5.5))
    ax.imshow(valid,origin="lower",cmap="Greys",vmin=0,vmax=1,
              extent=[.5,M+.5,.5,M+.5])
    ys,xs=np.where(valid)
    ax.scatter(xs+1,ys+1,s=25,facecolors="none",edgecolors="#a33")
    ax.set(xlabel="Second side [units]",ylabel="Third side [units]",
           title=f"Euler 86: integer shortest unfolded paths\n"
                 f"Fixed longest side {M}; teaching sample only")
    save(fig,"cuboid_routes")

def cube_digit_pairs():
    cubes=({0,5,6,7,8,9},{1,2,3,4,8,9})
    def avail(ds): return ds | ({6,9} if 6 in ds or 9 in ds else set())
    squares=("01","04","09","16","25","36","49","64","81")
    for pair in squares:
        a,b=map(int,pair)
        assert (a in avail(cubes[0]) and b in avail(cubes[1])) or (
                b in avail(cubes[0]) and a in avail(cubes[1]))
    fig,ax=plt.subplots(figsize=(11,3))
    ax.set_xlim(0,11);ax.set_ylim(0,3)
    for j,digits in enumerate(cubes):
        x0=j*5+1.4
        ax.add_patch(plt.Rectangle((x0,.8),3.5,1.6,
                                   facecolor="0.85" if j==0 else "0.65",
                                   edgecolor="black"))
        for k,val in enumerate(sorted(digits)):
            ax.text(x0+.57+k%3*1.15,1.85-k//3*.75,str(val),
                    fontsize=18,ha="center",va="center")
    ax.axis("off")
    ax.set_title("Euler 90: a valid cube pair for 01, 04, ..., 81\n"
                 "Rotational 6/9 equivalence; cube order is interchangeable")
    save(fig,"cube_digit_pairs")

def coordinate_right_triangles():
    fig,ax=plt.subplots(figsize=(6.7,5.5))
    ax.scatter(*zip(*[(x,y) for x in range(6) for y in range(6)]),
               s=12,c="0.7")
    points=np.array([[0,0],[4,0],[4,3],[0,0]])
    ax.plot(points[:,0],points[:,1],lw=2.5,color="0.15")
    for (x,y),label in zip(points[:3],("O","P","Q")):
        ax.text(x+.1,y+.1,label,fontsize=11)
    ax.add_patch(plt.Rectangle((3.7,0),.3,.3,fill=False,edgecolor="0.15"))
    ax.set(xlim=(-.5,5.5),ylim=(-.5,5.5),xticks=range(6),yticks=range(6),
           aspect="equal",xlabel="x [lattice units]",ylabel="y [lattice units]",
           title="Euler 91: a right triangle on a 5x5 lattice sample")
    ax.grid(ls=":",alpha=.5)
    save(fig,"coordinate_right_triangles")

def almost_equilateral():
    fig,ax=plt.subplots(figsize=(7,5))
    ax.plot([-3,3,0,-3],[0,0,4,0],color="0.15",lw=2.5)
    ax.plot([0,0],[0,4],ls="--",color="#a33")
    for (x,y),label in (((-1.7,2.4),"5"),((1.7,2.4),"5"),
                        ((0,-.25),"6"),((.2,2),"4")):
        ax.text(x,y,label,fontsize=12)
    ax.set(aspect="equal",xlim=(-4,4),ylim=(-1,5),
           title="Euler 94: almost-equilateral 5-5-6 triangle\n"
                 "Area 12 [square units], perimeter 16 [units]")
    ax.axis("off")
    save(fig,"almost_equilateral")

def sudoku():
    puzzle=("003020600","900305001","001806400",
            "008102900","700000008","006708200",
            "002609500","800203009","005010300")
    grid=[[int(c) for c in row] for row in puzzle]
    given=np.array(grid)>0
    def candidates(row,col):
        used=set(grid[row])|{grid[i][col] for i in range(9)}|{
            grid[y][x] for y in range(row//3*3,row//3*3+3)
            for x in range(col//3*3,col//3*3+3)}
        return [d for d in range(1,10) if d not in used]
    def solve():
        spaces=[(r,c,candidates(r,c)) for r in range(9)
                for c in range(9) if grid[r][c]==0]
        if not spaces: return True
        r,c,opts=min(spaces,key=lambda item:len(item[2]))
        for value in opts:
            grid[r][c]=value
            if solve(): return True
        grid[r][c]=0
        return False
    assert solve()
    fig,ax=plt.subplots(figsize=(6.8,6.6))
    for r in range(9):
        for c in range(9):
            ax.text(c,r,str(grid[r][c]),ha="center",va="center",
                    fontsize=14,fontweight="bold" if given[r,c] else "normal",
                    color="black" if given[r,c] else "0.5")
    for v in np.arange(-.5,9.5):
        ax.axvline(v,lw=2 if int(v+.5)%3==0 else .5,color="0.2")
        ax.axhline(v,lw=2 if int(v+.5)%3==0 else .5,color="0.2")
    ax.set(xlim=(-.5,8.5),ylim=(8.5,-.5),aspect="equal",
           title="Euler 96: Sudoku teaching example\n"
                 "Bold given digits; gray digits filled by backtracking")
    ax.axis("off")
    save(fig,"sudoku_demo")

def origin_in_triangle(vertices):
    cross=lambda a,b:a[0]*b[1]-a[1]*b[0]
    signs=[cross(vertices[(i+1)%3]-vertices[i],-vertices[i])
           for i in range(3)]
    return all(x>=0 for x in signs) or all(x<=0 for x in signs)

def triangle_containment():
    triangles=[np.array([[-3.,-2.],[4.,-1.],[1.,4.]]),
               np.array([[1.,2.],[4.,1.],[2.,5.]])]
    fig,axes=plt.subplots(1,2,figsize=(10,4.5))
    for ax,pts in zip(axes,triangles):
        inside=origin_in_triangle(pts)
        ax.fill(pts[:,0],pts[:,1],edgecolor="black",facecolor="0.85",
                hatch="///" if inside else "xx",lw=1.5)
        ax.scatter([0],[0],s=70,color="#a33",marker="+",lw=2.5)
        ax.axhline(0,color="0.6",ls=":")
        ax.axvline(0,color="0.6",ls=":")
        ax.set(xlim=(-4,5),ylim=(-3,6),aspect="equal",
               title="Origin inside" if inside else "Origin outside",
               xlabel="x [units]",ylabel="y [units]")
    fig.suptitle("Euler 102: oriented edge cross products test containment")
    save(fig,"triangle_containment")

def minimal_network():
    nodes=np.array([[0,0],[2,0],[4,0],[1,1.6],[3,1.5],[5,1.6],[1.1,3],[4.4,3]])
    edges=[(0,1,4),(1,2,5),(0,3,5),(1,3,2),(1,4,7),(2,4,3),
           (2,5,8),(3,4,6),(3,6,4),(4,5,1),(4,6,3),(4,7,9),
           (5,7,4),(6,7,7)]
    parent=list(range(len(nodes)))
    def root(x):
        while parent[x]!=x:x=parent[x]
        return x
    mst=set()
    for u,v,w in sorted(edges,key=lambda e:e[2]):
        a,b=root(u),root(v)
        if a!=b:
            parent[a]=b
            mst.add((u,v))
    assert len(mst)==len(nodes)-1
    fig,axes=plt.subplots(1,2,figsize=(12,5))
    for ax,tree_only in zip(axes,(False,True)):
        for u,v,weight in edges:
            if tree_only and (u,v) not in mst: continue
            x1,y1=nodes[u]
            x2,y2=nodes[v]
            selected=(u,v) in mst
            ax.plot([x1,x2],[y1,y2],
                    color="0.25" if selected else "0.76",
                    lw=2.4 if selected else 1,
                    ls="-" if selected else ":")
            ax.text((x1+x2)/2,(y1+y2)/2,str(weight),fontsize=8,
                    backgroundcolor="white")
        ax.scatter(nodes[:,0],nodes[:,1],s=160,facecolor="white",
                   edgecolor="black",zorder=5)
        for n,(x,y) in enumerate(nodes):
            ax.text(x,y,str(n),ha="center",va="center",zorder=6,fontsize=9)
        ax.set(aspect="equal",xlim=(-.5,5.5),ylim=(-.5,3.6),
               title="Weighted graph" if not tree_only else "Minimum spanning tree")
        ax.axis("off")
    fig.suptitle("Euler 107: Kruskal MST on a synthetic eight-node graph")
    save(fig,"minimal_network")

def darts():
    throws=[(f"S{i}",i) for i in range(1,21)]+[
        (f"D{i}",i*2) for i in range(1,21)]+[
        (f"T{i}",i*3) for i in range(1,21)]+[
        ("S25",25),("D25",50)]
    doubles=[v for name,v in throws if name.startswith("D")]
    scores=Counter()
    for last in doubles:
        if last<80:scores[last]+=1
        for _,first in throws:
            if first+last<80:scores[first+last]+=1
        for (_,a),(_,b) in combinations_with_replacement(throws,2):
            if a+b+last<80:scores[a+b+last]+=1
    fig,ax=plt.subplots(figsize=(10,4))
    values=np.arange(2,80)
    ax.bar(values,[scores[v] for v in values],width=.9,
           color="0.7",edgecolor="0.3",linewidth=.2)
    ax.set(xlabel="Checkout score [points]",ylabel="Combinations",
           title="Euler 109: one–three darts, final dart double\n"
                 "Earlier throws unordered; illustrative totals below 80")
    save(fig,"darts_checkout_counts")

@lru_cache(None)
def red_separated(n,minlen):
    if n<=0: return 1
    ways=red_separated(n-1,minlen)
    for block in range(minlen,n+1):
        ways+=1 if block==n else red_separated(n-block-1,minlen)
    return ways

def tilings():
    @lru_cache(None)
    def mono(n,k):
        return 1 if n==0 else mono(n-1,k)+(mono(n-k,k) if n>=k else 0)
    @lru_cache(None)
    def multi(n):
        return 1 if n==0 else sum(multi(n-k) for k in (1,2,3,4) if n>=k)
    lengths=np.arange(1,21)
    series=[
        (114,[red_separated(int(n),3) for n in lengths],"red min 3; separation","-"),
        (115,[red_separated(int(n),4) for n in lengths],"red min 4; scaled example","--"),
        (116,[sum(mono(int(n),k)-1 for k in (2,3,4)) for n in lengths],"one color per row",":"),
        (117,[multi(int(n)) for n in lengths],"2-,3-,4-cell tiles freely mixed","-.")]
    fig,ax=plt.subplots(figsize=(9,5))
    for number,values,label,style in series:
        ax.plot(lengths,values,ls=style,lw=2,marker="o",markersize=3,
                label=f"#{number}: {label}")
    ax.set(xlabel="Row length [cells]",ylabel="Valid arrangements (log scale)",
           yscale="log",title="Euler 114–117: exact dynamic-programming recurrences\n"
                 "Illustrative row lengths ≤20")
    ax.legend(fontsize=8)
    ax.grid(ls=":",alpha=.35)
    save(fig,"block_tilings")

def make_all():
    figures=[triangle_perimeters,grid_products,lattice_paths,maximum_path_triangle,
             spiral_prime_ratio,magic_5gon,path_sum_directions,monopoly_dice,
             rectangle_counts,cuboid_routes,cube_digit_pairs,coordinate_right_triangles,
             almost_equilateral,sudoku,triangle_containment,minimal_network,
             darts,tilings]
    for fn in figures:fn()
    print(f"Created {len(figures)} additional figures in {OUT}")

if __name__=="__main__":make_all()
