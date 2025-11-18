# Hitting Set Approximation Algorithm

A comprehensive implementation of an approximation algorithm for the **Hitting Set Problem** with ILP-based validation and extensive experimental testing.

## 📋 Overview

This project contains:

- **Approximation Algorithm** (`paper1algocopy.cpp`) — Weighted ε-net based algorithm achieving O(log n) approximation
- **Optimal ILP Solver** (`ilp.cpp`) — Integer Linear Programming solver using GLPK for ground truth validation
- **Graph Generator** (`gen_graphs.cpp`) — Random bipartite graph generator for testing
- **Automated Test Suite** (`run_experiments.sh`) — Comprehensive testing across 15 different graph sizes
- **Documentation** — Complete presentation guides and results analysis

## 🎯 Key Results

- **Average Approximation Ratio:** 1.34x optimal (excellent!)
- **Best Performance:** 1.06x (near-optimal)
- **Scalability:** Handles 1000+ points in <0.5s (ILP fails at ~200 points)
- **Consistency:** All results within 2x optimal, far better than O(log n) theoretical bound

## Input file format (accepted by `paper1algocopy.cpp`)

- First non-empty, non-comment line: `n m [e]`
  - `n` = number of points (indices 0..n-1)
  - `m` = number of ranges (indices 0..m-1)
  - `e` = optional total number of edges (pairs) that follow
- Each subsequent line contains a pair of integers: `range_index point_index`
  - Each pair indicates that the given range contains that point.
  - Lines beginning with `#` are treated as comments and ignored.
  - Both 0-based and 1-based indices are tolerated: if every index is in [1..n] or [1..m], the loader will try to convert to 0-based automatically.

Example (the built-in example used in the code):

```
6 5
0 0
0 1
0 4
1 0
1 2
1 5
2 1
2 3
3 2
3 3
4 4
4 5
```

---

## 🚀 Quick Start

### Prerequisites

- **C++ compiler** (g++ with C++17 support)
- **GLPK library** (for ILP solver)
  - Ubuntu/WSL: `sudo apt-get install libglpk-dev`
  - macOS: `brew install glpk`

### Compilation (WSL/Linux)

```bash
# Compile all components
g++ gen_graphs.cpp -o gen_graphs -O2
g++ paper1algocopy.cpp -o paper1algo -O2
g++ ilp.cpp -o solver -lglpk -O2
```

### Basic Usage

**Generate a test graph:**

```bash
./gen_graphs 100 50 0.05 42 test.txt
# Creates graph with 100 points, 50 ranges, 5% edge probability
```

**Run approximation algorithm:**

```bash
./paper1algo test.txt
# Fast polynomial-time solution
```

**Run ILP solver (for comparison):**

```bash
./solver test.txt
# Finds optimal solution (slow for large graphs)
```

**Run comprehensive test suite:**

```bash
./run_experiments.sh
# Runs 15 tests and generates complete results
```

---

## 📊 Understanding `run_experiments.sh`

### What Happens When You Run the Script

The automated test suite performs a comprehensive evaluation of both algorithms:

#### **Step-by-Step Process:**

1. **Initialization**

   - Creates timestamp for this experiment run
   - Initializes counters and result storage arrays
   - Prepares output files (`experiment_results.txt`, `TEST_RESULTS_CHART.md`)

2. **Test Execution** (15 test cases)

   For each test:

   **a) Graph Generation**

   ```bash
   ./gen_graphs [n] [m] [p] [seed] test_X.txt
   ```

   - Generates random bipartite graph with specified parameters
   - Ensures no empty ranges (each range has ≥1 point)

   **b) Approximation Algorithm**

   ```bash
   timeout 30s ./paper1algo test_X.txt
   ```

   - Runs weighted ε-net approximation algorithm
   - Times execution (typically <0.5s)
   - Extracts solution size
   - Verifies solution validity (all ranges hit)

   **c) ILP Solver (if n ≤ 200)**

   ```bash
   timeout 120s ./solver test_X.txt
   ```

   - Runs optimal ILP solver (if graph small enough)
   - Times execution
   - Extracts optimal solution size
   - Computes approximation ratio = approx_size / optimal_size
   - For large graphs (n > 200): Skips (would timeout)

   **d) Data Collection**

   - Stores: test name, n, m, edges, approx size, optimal size, ratio, time
   - Writes detailed results to `experiment_results.txt`
   - Cleans up temporary test files

3. **Result Analysis & Chart Generation**

   After all tests complete:

   **a) Statistics Computation**

   - Average approximation ratio
   - Best/worst ratios and which tests achieved them
   - Fastest/slowest execution times
   - Success rates for ILP solver

   **b) Auto-Generate `TEST_RESULTS_CHART.md`**

   - Creates markdown table with all 15 test results
   - Marks near-optimal solutions (ratio < 1.10) with ⭐
   - Includes statistics summary
   - Shows performance trends (time vs. graph size)
   - Breaks down quality distribution (excellent/good/acceptable)
   - Adds timestamp showing when generated

   **c) Console Summary**

   - Prints total tests run
   - Shows ILP success count
   - Displays average approximation ratio

4. **Output Files**
   - `experiment_results.txt` — Detailed raw output (overwritten each run)
   - `TEST_RESULTS_CHART.md` — Quick reference table (auto-generated each run)

#### **Test Case Categories:**

```
TINY (n=10-20)        → Baseline correctness verification
SMALL (n=50-60)       → Quality assessment
MEDIUM (n=100-150)    → ILP feasibility limit
LARGE (n=300-1000)    → Approximation-only (ILP infeasible)
SPECIAL STRUCTURES    → Edge cases (few ranges, many ranges, balanced)
```

#### **Example Output:**

```
Running Test #5: Small Medium Density (n=50, m=25, p=0.05, seed=2002)...
  Running approximation algorithm...
  Running ILP solver...
  ILP completed: optimal=14, ratio=1.0714

=========================================
Experiment complete!
Results written to: experiment_results.txt
Generating TEST_RESULTS_CHART.md...
=========================================

Summary:
  Total tests: 15
  ILP successful: 12
  Average approximation ratio: 1.3417
```

---

## 📁 Project Structure

```
├── gen_graphs.cpp           # Random graph generator
├── paper1algocopy.cpp       # Approximation algorithm implementation
├── ilp.cpp                  # ILP optimal solver implementation
├── run_experiments.sh       # Automated test suite
├── tiny_test.txt            # Small validation test case
├── experiment_results.txt   # Detailed test output (auto-generated)
├── TEST_RESULTS_CHART.md    # Quick reference table (auto-generated)
├── EXPERIMENT_SUMMARY.md    # Executive summary and analysis
└── README.md                # This file
```

---

## 🔧 Component Details

### Graph Generator (`gen_graphs`)

**Usage:**

```bash
./gen_graphs <n> <m> <p> <seed> <output_file>
```

**Arguments:**

- `n` — Number of points (0 to n-1)
- `m` — Number of ranges (0 to m-1)
- `p` — Edge probability [0.0, 1.0] for each (range, point) pair
- `seed` — RNG seed (use 0 for random)
- `output_file` — Path to output file

**Example:**

```bash
./gen_graphs 200 100 0.05 42 my_graph.txt
# Creates graph with 200 points, 100 ranges, ~5% density
```

### Approximation Algorithm (`paper1algo`)

**Features:**

- Weighted ε-net based approach
- O(mn log n) time complexity
- O(log n) approximation guarantee
- Handles 1000+ points easily

**Usage:**

```bash
./paper1algo <graph_file>
```

**Output:**

```
Attempting to find hitting set with optimal size c' = 1
...
>>> Success! Found a hitting set.
Final Hitting Set (point indices): { 3 7 12 15 ... }
Size of hitting set: 15
```

### ILP Solver (`solver`)

**Features:**

- Runs both approximation AND optimal ILP
- Uses GLPK library for ILP solving
- Compares results directly
- Practical for graphs with n < 200

**Usage:**

```bash
./solver <graph_file>
```

**Output:**

```
--- Approximation Algorithm Result ---
Hitting Set: { 3 7 12 15 ... }
Total Size: 15

--- [ILP Solver] Solving for optimal solution... ---
--- [ILP Solver] Optimal solution size found: 12 ---

--- Optimal ILP Solver Result ---
Hitting Set: { 3 8 11 14 ... }
Total Size: 12

--- Comparison ---
Approximation Size: 15
Optimal Size:       12
```

---

## 📖 Documentation Files

- **`PRESENTATION_GUIDE.md`** — Complete 20-25 minute presentation guide

  - Problem introduction and real-world applications
  - Algorithm explanation (technical and non-technical versions)
  - Live demo walkthrough
  - Results analysis
  - Q&A preparation

- **`DEMO_SCRIPT.md`** — Practical demo execution guide

  - Exact terminal commands for 5/10/20 minute demos
  - Visual flow diagrams
  - Backup plans and checklist

- **`EXPERIMENT_SUMMARY.md`** — Executive summary

  - High-level results analysis
  - Performance comparisons
  - Practical recommendations

- **`TEST_RESULTS_CHART.md`** — Quick reference table
  - All 15 test results in table format
  - Auto-generated by `run_experiments.sh`
  - Updated with each test run

---

## 🎓 Algorithm Details

### Approximation Algorithm

**Core Idea:** Weighted ε-net with iterative refinement

1. Start with guess c' = 1 (optimal size guess)
2. Initialize all point weights to 1.0
3. Repeat until solution found:
   - Find ranges with heavy weight (≥ ε × total weight)
   - Add heaviest point from that range to solution
   - Double weights for all points in that range
   - Verify if all ranges are hit → success!
4. If fail, double c' and restart

**Key Innovation:** Weight doubling creates "penalty" for hard-to-cover ranges, ensuring balanced coverage while maintaining small solution size.

### ILP Formulation

**Variables:** Binary p_i ∈ {0,1} for each point (1 = selected)

**Objective:** Minimize Σ p_i (minimize total selected points)

**Constraints:** Σ(p_i : i ∈ R_j) ≥ 1 for each range R_j (at least one point per range)

**Solver:** GLPK branch-and-bound algorithm with preprocessing

---

## 📈 Performance Characteristics

| Metric                 | Approximation  | ILP Optimal          |
| ---------------------- | -------------- | -------------------- |
| **Time Complexity**    | O(mn log n)    | Exponential (2^n)    |
| **Max Practical Size** | 1000+ points   | ~150-200 points      |
| **Solution Quality**   | ~1.34x optimal | Provably optimal     |
| **Speed (n=100)**      | ~0.05s         | ~0.05s               |
| **Speed (n=500)**      | ~0.18s         | Infeasible (timeout) |

---

## 🔬 Testing & Validation

### Correctness Verification

1. **Verification Function** — Every solution checked against all ranges
2. **ILP Comparison** — Approximation always ≥ optimal (12 successful comparisons)
3. **Manual Testing** — Hand-verified on tiny examples

### Test Coverage

- **15 test cases** across different sizes and densities
- **12 ILP-verified** tests (graphs with n ≤ 150)
- **3 large-scale** tests (approximation only, n = 300-1000)
- **100% success rate** — No failures or invalid solutions

---

## 💡 Use Cases

### When to Use Approximation Algorithm:

✅ Graph has > 200 points/ranges  
✅ Need results in seconds  
✅ Near-optimal solution acceptable (1-2x optimal)  
✅ Production/real-time systems

### When to Use ILP Solver:

✅ Graph has < 150 points/ranges  
✅ Need provably optimal solution  
✅ Time not critical (willing to wait minutes)  
✅ Benchmarking or research purposes

---

## 🎯 Real-World Applications

- **Network Monitoring** — Minimum monitoring stations to cover all segments
- **Resource Allocation** — Minimum servers to handle all jobs
- **Database Optimization** — Minimum indices to speed up all queries
- **Sensor Placement** — Minimum sensors for area coverage
- **Security** — Minimum cameras to monitor all zones

---

## 🤝 Contributing

This project is for academic/research purposes. Feel free to:

- Test on different graph structures
- Implement alternative approximation algorithms
- Add visualization tools
- Extend to weighted hitting set variants

---

## 📄 License

Academic/Research use. See project documentation for details.

---

## 📧 Contact

For questions about the implementation, algorithms, or results, refer to the comprehensive documentation files included in this repository.
