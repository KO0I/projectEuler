//! Project Euler 82: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(82).expect("implemented Euler problem");
    println!("{answer}");
}
