#!/usr/bin/env python3

import json
import sys

def plot_results(filename):
    with open(filename, 'r') as f:
        data = json.load(f)
    
    print("Monte Carlo Pricing Results")
    print("===========================")
    print(f"Mean Price: {data['mean']}")
    print(f"Std Dev:    {data['std_dev']}")
    print(f"Min:        {data['min']}")
    print(f"Max:        {data['max']}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: plot_results.py <json_file>")
        sys.exit(1)
    plot_results(sys.argv[1])
