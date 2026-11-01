//! Project Euler 52: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(52).expect("implemented Euler problem");
    println!("{answer}");
}
