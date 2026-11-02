// Project Euler 186: connectedness of a telephone network.
// Generate the lagged-Fibonacci call stream and join components with a
// path-compressed, size-weighted disjoint-set forest.
fn find(parent: &mut [usize], mut x: usize) -> usize {
    let mut root = x;
    while parent[root] != root {
        root = parent[root];
    }
    while parent[x] != x {
        let next = parent[x];
        parent[x] = root;
        x = next;
    }
    root
}
fn solve() -> u64 {
    const N: usize = 1_000_000;
    const PM: usize = 524_287;
    let mut parent: Vec<_> = (0..N).collect();
    let mut size = vec![1usize; N];
    let mut seq = [0usize; 55];
    let mut index = 1u64;
    let mut calls = 0u64;
    while {
        let root = find(&mut parent, PM);
        size[root] < 990_000
    } {
        let mut next = || {
            let k = index;
            index += 1;
            let v = if k <= 55 {
                let kk = k as i64;
                (100003 - 200003 * kk + 300007 * kk * kk * kk).rem_euclid(N as i64) as usize
            } else {
                (seq[((k - 25) % 55) as usize] + seq[((k - 56) % 55) as usize]) % N
            };
            seq[((k - 1) % 55) as usize] = v;
            v
        };
        let a0 = next();
        let b0 = next();
        if a0 == b0 {
            continue;
        }
        calls += 1;
        let (mut a, mut b) = (find(&mut parent, a0), find(&mut parent, b0));
        if a == b {
            continue;
        }
        if size[a] < size[b] {
            (a, b) = (b, a);
        }
        parent[b] = a;
        size[a] += size[b];
    }
    calls
}
fn main() {
    println!("{}", solve());
}
