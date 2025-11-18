# User Flow & Demo Script

## Quick Reference: What to Run and When

### 🎯 Option 1: Quick Demo (5 minutes)

```bash
# 1. Show the project structure
ls -lh

# 2. Run a small test
./gen_graphs 50 20 0.05 42 demo.txt
./solver demo.txt

# 3. Show results
cat TEST_RESULTS_CHART.md | head -30
```

**Narration:**
1. "Here's the codebase - generator, algorithms, and test suite"
2. "Let me generate and solve a 50-point graph..."
3. "Approximation found 12, optimal is 10 - that's 1.2x, excellent!"
4. "Here are results from 15 comprehensive tests averaging 1.34x optimal"

---

### 🎯 Option 2: Complete Demo (10 minutes)

```bash
# STEP 1: Show tiny example (manually verifiable)
echo "=== STEP 1: Tiny Example ==="
./solver tiny_test.txt

# STEP 2: Medium test with timing
echo "=== STEP 2: Medium Test ==="
time ./paper1algo demo.txt
time ./solver demo.txt

# STEP 3: Large test (approximation only)
echo "=== STEP 3: Large Scale Test ==="
./gen_graphs 500 200 0.02 99999 large.txt
time ./paper1algo large.txt
echo "ILP would timeout here - let's not wait!"

# STEP 4: Show comprehensive results
echo "=== STEP 4: Full Experiment Results ==="
cat TEST_RESULTS_CHART.md
```

---

### 🎯 Option 3: Full Presentation Flow (20 minutes)

#### **Phase 1: Introduction (3 min)**
- Whiteboard: Draw simple hitting set example
- Explain problem in plain English
- Give 2 real-world applications

#### **Phase 2: The Challenge (3 min)**
Show complexity:
```bash
# Open terminal
python3 -c "print('n=50:', 2**50, 'subsets')"
```
> "This is why brute force fails - exponential explosion!"

#### **Phase 3: Solution Overview (4 min)**
Show code structure:
```bash
# Show main files
ls -lh *.cpp

# Quick peek at algorithm
head -50 paper1algocopy.cpp | grep -A 20 "class HittingSetSolver"
```
> "Here's the approximation algorithm class..."

#### **Phase 4: Live Demo (5 min)**

**Part A - Small test**
```bash
echo "Small graph demonstration:"
./gen_graphs 30 15 0.1 123 small.txt
echo "Running approximation..."
./paper1algo small.txt | tail -5
echo "Running optimal ILP..."
./solver small.txt | grep -A 5 "Comparison"
```

**Part B - Scalability test**
```bash
echo "Now let's scale up:"
./gen_graphs 200 100 0.03 456 medium.txt
echo "Approximation (fast):"
time ./paper1algo medium.txt | tail -3

echo "Trying ILP (will be slow)..."
timeout 10s ./solver medium.txt || echo "TIMEOUT - too slow!"
```

**Part C - Show full results**
```bash
echo "Comprehensive testing across 15 test cases:"
cat TEST_RESULTS_CHART.md | head -40
```

#### **Phase 5: Results Analysis (3 min)**
Point to specific results:
```bash
# Show best case
echo "Best approximation ratio:"
grep "Best Ratio" TEST_RESULTS_CHART.md

# Show average
echo "Average performance:"
grep "Average" TEST_RESULTS_CHART.md

# Show scalability
echo "Maximum graph sizes:"
grep "Max graph size" TEST_RESULTS_CHART.md
```

#### **Phase 6: Wrap-up (2 min)**
- Summarize key numbers
- Take questions

---

## 🎬 Demo Script with Exact Commands

### Setup (Run before presenting):
```bash
cd "/mnt/c/everything i use/PROJECTS/aniketsir"
# Make sure everything is compiled
ls gen_graphs paper1algo solver
# Have tiny_test.txt ready
ls tiny_test.txt
```

---

### Scene 1: "The Problem"
**Show on screen:**
```bash
cat tiny_test.txt
```
**Say:** "This is a small hitting set instance - 6 points, 5 ranges"

**Show solution:**
```bash
./solver tiny_test.txt
```
**Say:** "Approximation found 4, optimal is 3 - not bad for this tiny case"

---

### Scene 2: "Scalability Test"
**Generate and solve:**
```bash
./gen_graphs 100 50 0.05 777 test100.txt
echo "Approximation algorithm:"
time ./paper1algo test100.txt | grep "Size of hitting set"
echo "ILP solver:"
time ./solver test100.txt | grep "Optimal Size:"
```
**Say:** "Both complete quickly at 100 points - approximation slightly faster"

---

### Scene 3: "Where ILP Fails"
**Large graph:**
```bash
./gen_graphs 300 150 0.02 888 test300.txt
echo "Approximation (should be instant):"
time ./paper1algo test300.txt | tail -5

echo "ILP (this will take forever):"
echo "I won't make you wait - trust me, it doesn't finish!"
# Don't actually run ILP here
```
**Say:** "Approximation: 0.1 seconds. ILP: would take hours or days"

---

### Scene 4: "Comprehensive Results"
**Show the data:**
```bash
cat TEST_RESULTS_CHART.md
```
**Point out key lines:**
- Line 8-22: Full results table
- Line 26-35: Statistics summary
- Line 70-75: Quality breakdown

**Say:** "15 tests, average 1.34x optimal, all under 0.5 seconds"

---

### Scene 5: "Proof of Correctness"
**Show verification:**
```bash
echo "Every solution is verified - let's trace one:"
./paper1algo tiny_test.txt | grep -E "(Iteration|Success)"
```
**Say:** "Watch it iterate, adjust weights, find solution. Each solution is checked against all ranges."

---

## 🎨 Visual Flow Diagram

```
┌─────────────────────────────────────────────────────┐
│                  START PRESENTATION                  │
└────────────────────┬────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────┐
│  1. PROBLEM INTRO (Whiteboard)                       │
│     - Draw simple example                            │
│     - Explain "hit every range with min points"     │
│     - Give 2 real-world applications                 │
└────────────────────┬────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────┐
│  2. COMPLEXITY CHALLENGE                             │
│     - Show 2^n explosion                             │
│     - Define NP-hard                                 │
│     - Explain approximation rationale                │
└────────────────────┬────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────┐
│  3. SOLUTION OVERVIEW                                │
│     - Flowchart of algorithm                         │
│     - Weight-doubling intuition                      │
│     - ILP as validation                              │
└────────────────────┬────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────┐
│  4. LIVE DEMO                                        │
│                                                       │
│  ┌────────────────────────────────────────────────┐ │
│  │  4a. Tiny Test (n=6)                           │ │
│  │      ./solver tiny_test.txt                    │ │
│  │      → Show both algorithms work               │ │
│  └────────────────┬───────────────────────────────┘ │
│                   │                                   │
│  ┌────────────────▼───────────────────────────────┐ │
│  │  4b. Medium Test (n=100)                       │ │
│  │      Compare timing (both fast)                │ │
│  └────────────────┬───────────────────────────────┘ │
│                   │                                   │
│  ┌────────────────▼───────────────────────────────┐ │
│  │  4c. Large Test (n=300)                        │ │
│  │      Show ILP fails, approximation succeeds    │ │
│  └────────────────┬───────────────────────────────┘ │
│                   │                                   │
│  ┌────────────────▼───────────────────────────────┐ │
│  │  4d. Results Summary                           │ │
│  │      cat TEST_RESULTS_CHART.md                 │ │
│  └────────────────────────────────────────────────┘ │
└────────────────────┬────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────┐
│  5. CODE DEEP DIVE (if time/technical audience)     │
│     - Show key functions                             │
│     - Explain ILP formulation                        │
│     - Walk through iteration example                 │
└────────────────────┬────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────┐
│  6. RESULTS ANALYSIS                                 │
│     - Quality metrics (1.34x avg)                    │
│     - Scalability comparison                         │
│     - Best/worst case examples                       │
└────────────────────┬────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────┐
│  7. CONCLUSIONS & Q&A                                │
│     - Recap key points                               │
│     - Emphasize practical value                      │
│     - Open floor for questions                       │
└─────────────────────────────────────────────────────┘
```

---

## 📱 Backup Plans

### If Demo Fails:
- Have screenshots of successful runs ready
- Show pre-generated TEST_RESULTS_CHART.md
- Walk through experiment_results.txt

### If Compilation Issues:
- Already have binaries compiled
- Test all executables before presentation
- Have backup VM/environment ready

### If Time is Short:
Skip to:
1. Problem intro (1 min)
2. Quick demo of tiny test (2 min)
3. Show TEST_RESULTS_CHART.md (2 min)
4. Q&A

### If Time is Long:
Add:
- Code walkthrough in VS Code
- Whiteboard trace of algorithm on example
- Discussion of alternative approaches
- Future work and extensions

---

## 🎯 Key Messages to Repeat

1. **"Approximation algorithms trade perfection for practicality"**
   - Say this early and often

2. **"1.34x optimal on average - excellent in practice"**
   - Your best number - highlight it

3. **"Scales to 1000 points while ILP maxes out at 150"**
   - Demonstrates practical value

4. **"Every solution is verified - no point in hitting fast if wrong"**
   - Shows rigor and correctness

---

## ✅ Pre-Presentation Checklist

- [ ] All code compiled (`gen_graphs`, `paper1algo`, `solver`)
- [ ] Test files exist (`tiny_test.txt`)
- [ ] Run `./run_experiments.sh` to ensure fresh results
- [ ] Terminal font size readable from back of room
- [ ] Terminal colors visible on projector
- [ ] Practice demo 2-3 times
- [ ] Have backup screenshots
- [ ] Slides/whiteboard material ready
- [ ] Water bottle nearby
- [ ] Confident smile ready! 😊

---

*Good luck with your presentation!*
