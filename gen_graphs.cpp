#include <bits/stdc++.h>
using namespace std;

int main(int argc, char **argv)
{
    // Usage:
    // gen_graphs.exe n m p seed out_file
    //   n = number of points (left side)
    //   m = number of ranges (right side)
    //   p = edge probability in [0,1] (for each (range,point) independently)
    //   seed = integer seed for RNG (use 0 for nondeterministic)
    //   out_file = (optional) path to write the graph (default "sample_graph.txt")
    // Output format matches the loader used by paper1algocopy.cpp:
    // First non-empty line: n m e  (e = total number of edges)
    // Following lines: pairs "range_index point_index" (0-based)

    if (argc < 5)
    {
        cerr << "Usage: " << argv[0] << " n m p seed [out_file]\n";
        cerr << "Example: " << argv[0] << " 100 50 0.02 42 d:\\\\CODEFORCES\\\\sample_graph.txt\n";
        return 1;
    }

    int n = stoi(argv[1]);
    int m = stoi(argv[2]);
    double p = stod(argv[3]);
    unsigned seed = static_cast<unsigned>(stoul(argv[4]));
    string out_path = (argc >= 6) ? string(argv[5]) : string("d:\\CODEFORCES\\sample_graph.txt");

    if (n <= 0 || m <= 0)
    {
        cerr << "n and m must be positive integers.\n";
        return 1;
    }
    if (!(p >= 0.0 && p <= 1.0))
    {
        cerr << "p must be in [0,1].\n";
        return 1;
    }

    mt19937 rng(seed == 0 ? random_device{}() : seed);
    uniform_real_distribution<double> ud(0.0, 1.0);

    vector<pair<int, int>> edges;
    edges.reserve(static_cast<size_t>(min<long long>((long long)n * m, 10000000LL)));

    for (int r = 0; r < m; ++r)
    {
        bool any_in_range = false;
        for (int v = 0; v < n; ++v)
        {
            if (ud(rng) < p)
            {
                edges.emplace_back(r, v);
                any_in_range = true;
            }
        }
        // Ensure no empty ranges: if a range has zero points, add one random point
        if (!any_in_range)
        {
            int v = rng() % n;
            edges.emplace_back(r, v);
        }
    }

    // Optionally we could remove duplicates (shouldn't exist here), but keep safe
    sort(edges.begin(), edges.end());
    edges.erase(unique(edges.begin(), edges.end()), edges.end());

    ofstream out(out_path);
    if (!out)
    {
        cerr << "Could not open output file: " << out_path << "\n";
        return 1;
    }

    out << n << " " << m << " " << edges.size() << "\n";
    for (auto &e : edges)
    {
        out << e.first << " " << e.second << "\n";
    }
    out.close();

    cout << "Wrote graph to " << out_path << " (n=" << n << ", m=" << m << ", e=" << edges.size() << ")\n";
    return 0;
}
