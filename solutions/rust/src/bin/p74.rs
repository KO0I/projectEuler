//! Project Euler 74: Digit Factorial Chains.

fn p74_next(mut n: usize) -> usize {
    const FAC: [usize; 10] = [1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880];
    let mut sum = 0;
    loop {
        sum += FAC[n % 10];
        n /= 10;
        if n == 0 {
            return sum;
        }
    }
}

fn solve() -> i64 {
    let len = 7 * 362880 + 1;
    let mut length = vec![0u16; len];
    let mut seen = vec![0usize; len];
    let mut position = vec![0usize; len];
    let mut answer = 0;
    for start in 1..1_000_000 {
        if length[start] == 0 {
            let mut v = start;
            let mut path = Vec::new();
            while length[v] == 0 && seen[v] != start {
                seen[v] = start;
                position[v] = path.len();
                path.push(v);
                v = p74_next(v);
            }
            let mut end = path.len();
            let mut tail;
            if length[v] > 0 {
                tail = length[v];
            } else {
                let first = position[v];
                tail = (path.len() - first) as u16;
                for &entry in &path[first..] {
                    length[entry] = tail;
                }
                end = first;
            }
            for &entry in path[..end].iter().rev() {
                tail += 1;
                length[entry] = tail;
            }
        }
        if length[start] == 60 {
            answer += 1;
        }
    }
    answer
}

fn main() {
    println!("{}", solve());
}
