//! Project Euler 73: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(73).expect("implemented Euler problem");
    println!("{answer}");
}
