// Project Euler 220: Heighway dragon.
// Precompute displacement, heading, and step count summaries for A_n/B_n.
// Walking a prefix skips every complete recursive block in O(1).
#[derive(Clone, Copy, Default)]
struct State {
    x: i64,
    y: i64,
    dir: i32,
    steps: u64,
}
fn rotate(x: i64, y: i64, d: i32) -> (i64, i64) {
    match d & 3 {
        0 => (x, y),
        1 => (y, -x),
        2 => (-x, -y),
        _ => (-y, x),
    }
}
fn apply(s: &mut State, b: State) {
    let (dx, dy) = rotate(b.x, b.y, s.dir);
    s.x += dx;
    s.y += dy;
    s.dir = (s.dir + b.dir) & 3;
    s.steps += b.steps;
}
fn turn(s: &mut State, q: i32) {
    s.dir = (s.dir + q) & 3;
}
fn forward(s: &mut State) {
    let dirs = [(0, 1), (1, 0), (0, -1), (-1, 0)];
    let d = dirs[s.dir as usize];
    s.x += d.0;
    s.y += d.1;
    s.steps += 1;
}
fn walk(a: bool, depth: usize, remaining: &mut u64, s: &mut State, aa: &[State], bb: &[State]) {
    if *remaining == 0 || depth == 0 {
        return;
    }
    let whole = if a { aa[depth] } else { bb[depth] };
    if whole.steps <= *remaining {
        apply(s, whole);
        *remaining -= whole.steps;
        return;
    }
    if a {
        walk(true, depth - 1, remaining, s, aa, bb);
        if *remaining == 0 {
            return;
        }
        turn(s, 1);
        walk(false, depth - 1, remaining, s, aa, bb);
        if *remaining == 0 {
            return;
        }
        forward(s);
        *remaining -= 1;
        if *remaining > 0 {
            turn(s, 1);
        }
    } else {
        turn(s, -1);
        forward(s);
        *remaining -= 1;
        if *remaining == 0 {
            return;
        }
        walk(true, depth - 1, remaining, s, aa, bb);
        if *remaining == 0 {
            return;
        }
        turn(s, -1);
        walk(false, depth - 1, remaining, s, aa, bb);
    }
}
fn solve() -> String {
    let (mut aa, mut bb) = (vec![State::default(); 51], vec![State::default(); 51]);
    for d in 1..=50 {
        let mut a = State::default();
        apply(&mut a, aa[d - 1]);
        turn(&mut a, 1);
        apply(&mut a, bb[d - 1]);
        forward(&mut a);
        turn(&mut a, 1);
        aa[d] = a;
        let mut b = State::default();
        turn(&mut b, -1);
        forward(&mut b);
        apply(&mut b, aa[d - 1]);
        turn(&mut b, -1);
        apply(&mut b, bb[d - 1]);
        bb[d] = b;
    }
    let mut s = State::default();
    forward(&mut s);
    let mut remaining = 999_999_999_999;
    walk(true, 50, &mut remaining, &mut s, &aa, &bb);
    format!("{},{}", s.x, s.y)
}
fn main() {
    println!("{}", solve());
}
