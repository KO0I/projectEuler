//! Project Euler 79: Passcode Derivation.

fn solve() -> i64 {
    let mut edges = [[false; 10]; 10];
    let mut present = [false; 10];
    for line in include_str!("../../../../tests/data/p079_keylog.txt").lines() {
        let d: Vec<usize> = line.bytes().map(|b| (b - b'0') as usize).collect();
        assert_eq!(d.len(), 3);
        for &v in &d {
            present[v] = true;
        }
        edges[d[0]][d[1]] = true;
        edges[d[1]][d[2]] = true;
    }
    let mut emitted = [false; 10];
    let mut result = 0;
    for _ in 0..present.iter().filter(|&&p| p).count() {
        let next = (0..10)
            .find(|&v| {
                present[v] && !emitted[v] && (0..10).all(|prior| !edges[prior][v] || emitted[prior])
            })
            .expect("acyclic passcode");
        result = result * 10 + next as i64;
        emitted[next] = true;
    }
    result
}

fn main() {
    println!("{}", solve());
}
