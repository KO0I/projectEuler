//! Project Euler 66: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(66).expect("implemented Euler problem");
    println!("{answer}");
}
