// Project Euler 212: union volume of 50,000 cuboids.
// Sweep unit x slabs while linked endpoint lists maintain active boxes.  A
// y-event sweep plus z segment tree computes their union area in each slab.
const BOXES: usize = 50_000;
const MAX: usize = 10_400;
#[derive(Clone, Copy, Default)]
struct Box3 {
    x0: usize,
    x1: usize,
    y0: usize,
    y1: usize,
    z0: usize,
    z1: usize,
    next_start: isize,
    next_end: isize,
    pos: isize,
}
#[derive(Clone, Copy)]
struct Event {
    y: usize,
    z0: usize,
    z1: usize,
    delta: i32,
}
fn update(
    node: usize,
    left: usize,
    right: usize,
    ql: usize,
    qr: usize,
    delta: i32,
    cover: &mut [i32],
    length: &mut [usize],
) {
    if ql >= right || qr <= left {
        return;
    }
    if ql <= left && right <= qr {
        cover[node] += delta;
    } else {
        let mid = (left + right) / 2;
        update(node * 2, left, mid, ql, qr, delta, cover, length);
        update(node * 2 + 1, mid, right, ql, qr, delta, cover, length);
    }
    length[node] = if cover[node] > 0 {
        right - left
    } else if right - left == 1 {
        0
    } else {
        length[node * 2] + length[node * 2 + 1]
    };
}
fn area(
    boxes: &[Box3],
    active: &[usize],
    events: &mut Vec<Event>,
    cover: &mut [i32],
    length: &mut [usize],
) -> u64 {
    events.clear();
    for &i in active {
        let b = boxes[i];
        events.push(Event {
            y: b.y0,
            z0: b.z0,
            z1: b.z1,
            delta: 1,
        });
        events.push(Event {
            y: b.y1,
            z0: b.z0,
            z1: b.z1,
            delta: -1,
        });
    }
    events.sort_unstable_by_key(|e| e.y);
    let mut result = 0;
    let mut i = 0;
    let mut prev = events.first().map_or(0, |e| e.y);
    while i < events.len() {
        let y = events[i].y;
        result += length[1] as u64 * (y - prev) as u64;
        while i < events.len() && events[i].y == y {
            let e = events[i];
            update(1, 0, MAX, e.z0, e.z1, e.delta, cover, length);
            i += 1;
        }
        prev = y;
    }
    result
}
fn solve() -> u64 {
    let mut s = vec![0usize; 6 * BOXES + 1];
    for k in 1..=55 {
        let k1 = k as i64;
        s[k] = (100003 - 200003 * k1 + 300007 * k1 * k1 * k1).rem_euclid(1_000_000) as usize;
    }
    for k in 56..s.len() {
        s[k] = (s[k - 24] + s[k - 55]) % 1_000_000;
    }
    let (mut starts, mut ends) = (vec![-1isize; MAX], vec![-1isize; MAX]);
    let mut boxes = vec![Box3::default(); BOXES];
    for i in 0..BOXES {
        let k = 6 * i + 1;
        let mut b = Box3 {
            x0: s[k] % 10000,
            y0: s[k + 1] % 10000,
            z0: s[k + 2] % 10000,
            ..Default::default()
        };
        b.x1 = b.x0 + 1 + s[k + 3] % 399;
        b.y1 = b.y0 + 1 + s[k + 4] % 399;
        b.z1 = b.z0 + 1 + s[k + 5] % 399;
        b.next_start = starts[b.x0];
        starts[b.x0] = i as isize;
        b.next_end = ends[b.x1];
        ends[b.x1] = i as isize;
        b.pos = -1;
        boxes[i] = b;
    }
    let mut active: Vec<usize> = Vec::with_capacity(BOXES);
    let mut events = Vec::with_capacity(2 * BOXES);
    let (mut cover, mut length) = (vec![0i32; 4 * MAX], vec![0usize; 4 * MAX]);
    let mut volume = 0;
    // Retaining the live list is a little more work than position-based O(1)
    // deletion, but it is robust when many boxes share an end coordinate and
    // remains small because a box spans at most 399 of the 10,400 x slabs.
    for x in 0..MAX - 1 {
        active.retain(|&i| boxes[i].x1 != x);
        let mut cur = starts[x];
        while cur >= 0 {
            let i = cur as usize;
            active.push(i);
            cur = boxes[i].next_start;
        }
        volume += area(&boxes, &active, &mut events, &mut cover, &mut length);
    }
    volume
}
fn main() {
    println!("{}", solve());
}
