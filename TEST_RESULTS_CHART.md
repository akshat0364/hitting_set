# Quick Reference: Test Results at a Glance

**Generated:** 2025-11-18 15:02:48

## All Test Cases - Approximation Ratios

| # | Test Name | n | m | Edges | Approx | Optimal | Ratio | Time(s) |
|---|-----------|---|---|-------|--------|---------|-------|---------|
| 1 | Tiny Dense | 10 | 5 | 23 | 3 | 2 | 1.5000 | .013960110 |
| 2 | Tiny Medium Density | 15 | 8 | 35 | 4 | 3 | 1.3333 | .020397847 |
| 3 | Tiny Sparse | 20 | 10 | 46 | 6 | 4 | 1.5000 | .021858359 |
| 4 | Small Dense | 50 | 20 | 100 | 12 | 8 | 1.5000 | .029900444 |
| 5 | Small Medium Density | 50 | 25 | 67 | 15 | 14 | **1.0714** ⭐ | .038557806 |
| 6 | Small Sparse | 60 | 30 | 59 | 19 | 18 | **1.0555** ⭐ | .042614484 |
| 7 | Medium Dense | 100 | 50 | 256 | 25 | 15 | 1.6666 | .049900320 |
| 8 | Medium Medium Density | 120 | 60 | 210 | 25 | 23 | **1.0869** ⭐ | .064293203 |
| 9 | Medium Sparse | 150 | 75 | 235 | 43 | 35 | 1.2285 | .074002838 |
| 10 | Large Sparse | 300 | 150 | 916 | 57 | N/A | N/A | .100786194 |
| 11 | Very Large Sparse | 500 | 200 | 1503 | 69 | N/A | N/A | .175571195 |
| 12 | Extra Large Sparse | 1000 | 400 | 4108 | 139 | N/A | N/A | .446214587 |
| 13 | Many Points, Few Ranges | 100 | 20 | 187 | 8 | 6 | 1.3333 | .033354835 |
| 14 | Few Points, Many Ranges | 50 | 100 | 498 | 22 | 16 | 1.3750 | .037597690 |
| 15 | Balanced Graph | 80 | 80 | 356 | 29 | 20 | 1.4500 | .049477247 |

⭐ = Near-optimal (ratio < 1.10)

## Statistics Summary

```
Total Tests:                    15
Tests with ILP comparison:      12
Tests approximation only:       3

Average Approximation Ratio:    1.3417x
Best Ratio:                     1.0555x  (#6 - Small Sparse)
Worst Ratio:                    1.6666x  (#7 - Medium Dense)

Fastest Test:                   .013960110s (#1 - Tiny Dense)
Slowest Test:                   .446214587s (#12 - Extra Large Sparse)

Max graph size solved by ILP:   150 points
Max graph size by approx:       1000 points
```

## Performance Trends

### Approximation Time vs Graph Size
```
n=10     → .013960110s
n=15     → .020397847s
n=20     → .021858359s
n=50     → .029900444s
n=50     → .038557806s
n=60     → .042614484s
n=100     → .049900320s
n=120     → .064293203s
n=150     → .074002838s
n=300     → .100786194s
n=500     → .175571195s
n=1000     → .446214587s
n=100     → .033354835s
n=50     → .037597690s
n=80     → .049477247s

Growth: Roughly linear (polynomial time)
```

## Quality Breakdown

### Excellent (Ratio < 1.3): 4 tests (33.3%)
### Good (1.3 ≤ Ratio < 1.5): 4 tests (33.3%)
### Acceptable (1.5 ≤ Ratio < 2.0): 4 tests (33.3%)
### Outside Bound (Ratio ≥ 2.0): 0 tests (0%)

All results well within theoretical guarantee!

## Key Insights

1. **Sparse graphs** tend to produce better approximation ratios
2. **Graph structure** matters more than absolute size
3. **All ratios** far better than O(log n) worst-case guarantee
4. **Approximation algorithm** scales to large graphs where ILP fails

---

*Auto-generated from `run_experiments.sh`*  
*See `experiment_results.txt` for detailed raw data*
