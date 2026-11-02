// Project Euler 247: squares under a hyperbola.
// A max-heap reproduces global largest-square order.  Child indices increment
// the number of squares to the left or below; stop after all <=(3,3) regions.
use std::cmp::Ordering;
use std::collections::BinaryHeap;
#[derive(Clone)]
struct S {
    x: f64,
    y: f64,
    side: f64,
    left: u32,
    below: u32,
}
impl Eq for S {}
impl PartialEq for S {
    fn eq(&self, o: &Self) -> bool {
        self.side == o.side
    }
}
impl Ord for S {
    fn cmp(&self, o: &Self) -> Ordering {
        self.side.total_cmp(&o.side)
    }
}
impl PartialOrd for S {
    fn partial_cmp(&self, o: &Self) -> Option<Ordering> {
        Some(self.cmp(o))
    }
}
fn side(x: f64, y: f64) -> f64 {
    (((x - y) * (x - y) + 4.0).sqrt() - x - y) / 2.0
}
fn solve() -> u64 {
    let mut heap = BinaryHeap::new();
    heap.push(S {
        x: 1.0,
        y: 0.0,
        side: side(1.0, 0.0),
        left: 0,
        below: 0,
    });
    let (mut serial, mut last, mut relevant) = (0, 0, 1i64);
    while relevant > 0 {
        let s = heap.pop().unwrap();
        serial += 1;
        if s.left <= 3 && s.below <= 3 {
            relevant -= 1;
        }
        if s.left == 3 && s.below == 3 {
            last = serial;
        }
        let r = S {
            x: s.x + s.side,
            y: s.y,
            side: side(s.x + s.side, s.y),
            left: s.left + 1,
            below: s.below,
        };
        if r.left <= 3 && r.below <= 3 {
            relevant += 1;
        }
        heap.push(r);
        let a = S {
            x: s.x,
            y: s.y + s.side,
            side: side(s.x, s.y + s.side),
            left: s.left,
            below: s.below + 1,
        };
        if a.left <= 3 && a.below <= 3 {
            relevant += 1;
        }
        heap.push(a);
    }
    last
}
fn main() {
    println!("{}", solve());
}
