//! Project Euler 80: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(80).expect("implemented Euler problem");
    println!("{answer}");
}
