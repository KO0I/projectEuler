//! Project Euler 62: Cubic Permutations.
use project_euler::extra_helpers::decimal_signature;
use std::collections::HashMap;

fn solve() -> i64 {
    {
        let mut bucket = Vec::<(u64, u64)>::new();
        let mut magnitude = 10u64;
        let mut answer = 0;
        for n in 1.. {
            let cube = n * n * n;
            if cube >= magnitude {
                let mut groups: HashMap<u64, Vec<u64>> = HashMap::new();
                for &(signature, value) in &bucket {
                    groups.entry(signature).or_default().push(value);
                }
                if let Some(best) = groups
                    .values()
                    .filter(|v| v.len() == 5)
                    .flat_map(|v| v.iter())
                    .min()
                {
                    answer = *best;
                    break;
                }
                bucket.clear();
                magnitude *= 10;
            }
            bucket.push((decimal_signature(cube), cube));
        }
        answer as i64
    }
}

fn main() {
    println!("{}", solve());
}
