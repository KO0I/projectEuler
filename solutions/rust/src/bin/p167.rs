//! Detect the finite membership-state cycle of each U(2,v) sequence.
fn kth(v: usize, rank: u64) -> u64 {
    let gap = v + 1;
    let even = 2 * gap;
    let mut member = [false; 128];
    let mut reps = [0u8; 256];
    let mut terms = vec![2, v];
    member[2] = true;
    member[v] = true;
    reps[2 + v] += 1;
    let mut last = v;
    while last < even {
        let mut next = last + 1;
        while reps[next] != 1 {
            next += 1
        }
        for &t in &terms {
            reps[next + t] = (reps[next + t] + 1).min(2)
        }
        terms.push(next);
        member[next] = true;
        last = next
    }
    let (mut state, mut seed) = (0usize, 0u64);
    for i in 0..gap {
        let bit = member[2 * i + 1] as usize;
        state = (state << 1) | bit;
        seed += bit as u64
    }
    let even_rank = 2 + seed;
    if rank == 1 {
        return 2;
    }
    if rank == even_rank {
        return even as u64;
    }
    let odd_rank = if rank < even_rank { rank - 1 } else { rank - 2 };
    let mut seen = vec![usize::MAX; 1 << gap];
    let mut prefix = vec![0u64; (1 << gap) + gap + 1];
    for i in 0..gap {
        prefix[i + 1] = prefix[i] + member[2 * i + 1] as u64
    }
    let mut generated = gap;
    let mask = (1 << gap) - 1;
    seen[state] = generated;
    let (start, end) = loop {
        let next = ((state >> (gap - 1)) ^ state) & 1;
        state = ((state << 1) & mask) | next;
        prefix[generated + 1] = prefix[generated] + next as u64;
        generated += 1;
        if seen[state] != usize::MAX {
            break (seen[state], generated);
        }
        seen[state] = generated
    };
    let index = if odd_rank <= prefix[start] {
        let mut i = 0;
        while prefix[i + 1] < odd_rank {
            i += 1
        }
        i as u64
    } else {
        let ones = prefix[end] - prefix[start];
        let rem = odd_rank - prefix[start] - 1;
        let cycles = rem / ones;
        let within = rem % ones + 1;
        let mut local = start;
        while prefix[local + 1] - prefix[start] < within {
            local += 1
        }
        local as u64 + cycles * (end - start) as u64
    };
    2 * index + 1
}
fn solve() -> u64 {
    (2..=10).map(|n| kth(2 * n + 1, 100_000_000_000)).sum()
}
fn main() {
    println!("{}", solve());
}
