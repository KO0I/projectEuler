//! Project Euler 104: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(104).expect("implemented Euler problem");
    println!("{answer}");
}
