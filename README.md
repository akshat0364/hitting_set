# paper1algocopy — Hitting Set solver and test generator

This small project contains:

- `paper1algocopy.cpp` — the hitting-set solver implementation you provided (now accepts graph input from a text file).
- `gen_graphs.cpp` — a simple generator that creates random bipartite set-systems in the input format used by the solver.
- `sample_graph.txt` — example graph file (can be overwritten by the generator).

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

## Generator (`gen_graphs.cpp`) usage

The generator writes `n m e` followed by `e` pairs of `range point` (0-based) to a file.

Usage:

```powershell
# compile
g++ -std=c++17 -O2 "d:\\CODEFORCES\\gen_graphs.cpp" -o "d:\\CODEFORCES\\gen_graphs.exe"

# run: gen_graphs.exe n m p seed [out_file]
& "d:\\CODEFORCES\\gen_graphs.exe" 200 100 0.05 42 "d:\\CODEFORCES\\sample_graph.txt"
```

Arguments:

- `n` number of points
- `m` number of ranges
- `p` probability in [0,1] for each (range,point) edge
- `seed` RNG seed (0 uses nondeterministic seed)
- `out_file` optional output path (defaults to `d:\\CODEFORCES\\sample_graph.txt`)

Notes:

- The generator ensures every range has at least one point (so no empty range lines).
- Output is 0-based by default to match the solver's format.

## Running the solver (`paper1algocopy.exe`)

Compile (if you changed code):

```powershell
g++ -std=c++17 -O2 "d:\\CODEFORCES\\paper1algocopy.cpp" -o "d:\\CODEFORCES\\paper1algocopy.exe"
```

Run:

```powershell
& "d:\\CODEFORCES\\paper1algocopy.exe" "d:\\CODEFORCES\\sample_graph.txt"
```

This will print progress and the final hitting set (list of point indices, 0-based).
