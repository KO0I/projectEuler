#!/usr/bin/env python3
import os
import subprocess
import argparse
import json
import re

def run_test(file_path, solutions_path):
    filename = os.path.basename(file_path)
    
    # Extract number from filename (e.g., 'multiples_of_3_and_5_1.c' -> '1')
    # Regex looks for the last underscore followed by digits and .c
    match = re.search(r'_(\d+)\.c$', filename)
    if not match:
        return f"Could_not_parse_number_from_{filename}", None
    problem_id = match.group(1)

    # Compile
    obj_path = file_path.replace('.c', '.out')
    try:
        # Use a temporary object file to ensure a fresh build
        subprocess.run(["gcc", "-O3", file_path, "-o", obj_path], check=True, capture_output=True, text=True)
    except Exception as e:
        return f"Compilation_failed_{e}", None

    # Execute
    try:
        result = subprocess.run([obj_path], check=True, capture_output=True, text=True)
        output = result.stdout.strip()
    except Exception as e:
        return f"Execution_failed_{e}", None
    finally:
        if os.path.exists(obj_path):
            os.remove(obj_path)

    # Validate
    try:
        with open(solutions_path, 'r') as f:
            solutions = json.load(f)
        
        # We look for the key in the JSON (which will be the problem number as string)
        expected = str(solutions.get(problem_id))
        
        if not expected or expected == "None":
            return f"No_expected_value_for_{problem_id}", output
            
        if str(expected) == str(output):
            return "PASS", output
        else:
            return f"FAIL_Expected_{expected}_Got_{output}", output
    except Exception as e:
        return f"Validation_error_{e}", None

def main():
    parser = argparse.ArgumentParser(description='Project Euler Validator')
    parser.add_argument('file', help='The .c file to test')
    parser.add_argument('--solutions', default='solutions.json', help='Path to solutions JSON')
    args = parser.parse_args()

    if not os.path.exists(args.file):
        print(f"Error: {args.file} not found.")
        return

    status, output = run_test(args.file, args.solutions)
    
    if status == "PASS":
        print(f"✅ {os.path.basename(args.file)} -> {status}!")
    else:
        print(f"❌ {os.path.basename(args.file)} -> {status}")

if __name__ == "__main__":
    main()