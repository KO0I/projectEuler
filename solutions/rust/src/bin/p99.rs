//! Project Euler 99: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(99).expect("implemented Euler problem");
    println!("{answer}");
}
