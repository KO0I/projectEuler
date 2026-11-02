//! Project Euler 61: Cyclical Figurate Numbers.

fn solve() -> i64 {
    fn search(path: &mut Vec<u64>, used: u8, values: &[Vec<u64>]) -> Option<i64> {
        if path.len() == 6 {
            return (path[5] % 100 == path[0] / 100).then_some(path.iter().sum::<u64>() as i64);
        }
        let prefix = path.last().unwrap() % 100;
        for kind in 0..6 {
            if used & (1 << kind) != 0 {
                continue;
            }
            for &v in &values[kind] {
                if v / 100 == prefix && !path.contains(&v) {
                    path.push(v);
                    if let Some(result) = search(path, used | (1 << kind), values) {
                        return Some(result);
                    }
                    path.pop();
                }
            }
        }
        None
    }
    let mut values = vec![Vec::new(); 6];
    for (kind, out) in values.iter_mut().enumerate() {
        let sides = kind as i64 + 3;
        for n in 1i64.. {
            let v = ((sides - 2) * n * n - (sides - 4) * n) / 2;
            if v >= 10000 {
                break;
            }
            if v >= 1000 && v % 100 >= 10 {
                out.push(v as u64);
            }
        }
    }
    for &v in &values[5] {
        let mut path = vec![v];
        if let Some(answer) = search(&mut path, 1 << 5, &values) {
            return answer;
        }
    }
    panic!("No polygonal cycle")
}

fn main() {
    println!("{}", solve());
}
