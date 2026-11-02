// Project Euler 244: sliders.
// BFS keys a board by red mask and blank position.  For merged shortest paths
// track both path count and rolling-checksum sum algebraically.
fn solve() -> u64 {
    const N: usize = 1 << 20;
    const MOD: u64 = 100_000_007;
    let start_red = (1 << 1) | (1 << 4) | (1 << 5) | (1 << 8) | (1 << 9) | (1 << 12) | (1 << 13);
    let target_red = (1 << 2) | (1 << 5) | (1 << 7) | (1 << 8) | (1 << 10) | (1 << 13) | (1 << 15);
    let start = start_red;
    let target = target_red;
    let mut distance = vec![u8::MAX; N];
    let (mut count, mut checks) = (vec![0u64; N], vec![0u64; N]);
    let mut q = std::collections::VecDeque::new();
    q.push_back(start);
    distance[start] = 0;
    count[start] = 1;
    while let Some(state) = q.pop_front() {
        if state == target {
            break;
        }
        let red = state & 0xffff;
        let blank = state >> 16;
        let (x, y) = (blank & 3, blank >> 2);
        for (dx, dy, letter) in [(1, 0, b'L'), (-1, 0, b'R'), (0, 1, b'U'), (0, -1, b'D')] {
            let (nx, ny) = (x as i32 + dx, y as i32 + dy);
            if !(0..4).contains(&nx) || !(0..4).contains(&ny) {
                continue;
            }
            let neighbor = (ny * 4 + nx) as usize;
            let mut nr = red;
            if red >> neighbor & 1 == 1 {
                nr &= !(1 << neighbor);
                nr |= 1 << blank;
            }
            let next = nr | (neighbor << 16);
            let nd = distance[state] + 1;
            if distance[next] == u8::MAX {
                distance[next] = nd;
                q.push_back(next);
            }
            if distance[next] == nd {
                count[next] += count[state];
                checks[next] =
                    (checks[next] + 243 * checks[state] + letter as u64 * count[state]) % MOD;
            }
        }
    }
    checks[target]
}
fn main() {
    println!("{}", solve());
}
