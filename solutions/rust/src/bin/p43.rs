//! Project Euler 43: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(43).expect("implemented Euler problem");
    println!("{answer}");
}
