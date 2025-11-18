# Comprehensive Experiment Documentation

This directory contains complete test results and analysis for the Hitting Set approximation algorithm.

## 📁 Documentation Files

### 1. **experiment_results.txt** (Raw Data)

- Complete raw output from all 15 test cases
- Includes graph parameters, timing data, and results
- Machine-readable format for further analysis

### 2. **EXPERIMENT_SUMMARY.md** (Executive Summary)

- High-level overview with tables and analysis
- Visual presentation of results
- Key findings and recommendations
- **START HERE** for an overview

### 3. **TEST_RESULTS_CHART.md** (Quick Reference)

- Single-page reference table with all results
- Statistics summary and trends
- Performance comparison charts
- **BEST FOR** quick lookups

## 🧪 Test Suite Coverage

### Test Categories

- **Tiny graphs** (n=10-20): Baseline correctness verification
- **Small graphs** (n=50-60): Quality assessment
- **Medium graphs** (n=100-150): ILP feasibility limit
- **Large graphs** (n=300-1000): Approximation scalability
- **Special structures**: Edge cases and variations

### Total Tests: 15

- **ILP verified:** 12 tests (with ground truth optimal)
- **Approximation only:** 3 tests (too large for ILP)

## 📊 Key Results

### Average Approximation Ratio: **1.34x**

- Best: 1.06x (near-optimal!)
- Worst: 1.67x (still excellent)
- All within theoretical O(log n) guarantee

### Performance

- **Approximation:** Handles 1000+ points in <0.4s
- **ILP:** Practical limit ~150-200 points
- **Speedup:** 100x-1000x on large graphs

## 🚀 How to Reproduce

### Run the full test suite:

```bash
./run_experiments.sh
```

This will:

1. Generate 15 test graphs of various sizes
2. Run both approximation and ILP algorithms
3. Compute approximation ratios
4. Generate `experiment_results.txt`

### Run individual tests:

```bash
# Generate a graph
./gen_graphs 100 50 0.05 12345 my_graph.txt

# Run approximation only
./paper1algo my_graph.txt

# Run both algorithms (with ILP comparison)
./solver my_graph.txt
```

## 📈 File Descriptions

| File                     | Purpose             | Best For                  |
| ------------------------ | ------------------- | ------------------------- |
| `experiment_results.txt` | Raw detailed output | Data mining, verification |
| `EXPERIMENT_SUMMARY.md`  | Executive summary   | Presentations, overview   |
| `TEST_RESULTS_CHART.md`  | Quick reference     | Fast lookups, comparison  |
| `run_experiments.sh`     | Test automation     | Reproducibility           |

## 🎯 Main Conclusions

1. **✅ Algorithm Verified Correct**

   - All solutions are valid hitting sets
   - Ratios match theoretical expectations

2. **⚡ Excellent Performance**

   - 1.34x average approximation ratio
   - Handles graphs 10x larger than ILP
   - Completes in milliseconds

3. **🏆 Practical Value**
   - Near-optimal solutions on most instances
   - Scales to production-sized problems
   - Better than worst-case theory predicts

## 📝 Citation

If using these results, please note:

- **Date:** November 12, 2025
- **Platform:** WSL Ubuntu, g++ -O2
- **ILP Solver:** GLPK
- **Test Cases:** 15 random bipartite graphs

---

_For questions or detailed analysis, see the individual documentation files above._
