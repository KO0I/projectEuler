//! Project Euler 59: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(59).expect("implemented Euler problem");
    println!("{answer}");
}
