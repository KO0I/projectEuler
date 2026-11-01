//! Project Euler 85: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(85).expect("implemented Euler problem");
    println!("{answer}");
}
