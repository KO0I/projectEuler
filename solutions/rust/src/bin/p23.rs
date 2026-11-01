//! Project Euler 23: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(23).expect("implemented Euler problem");
    println!("{answer}");
}
