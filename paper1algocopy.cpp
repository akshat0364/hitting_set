#include <bits/stdc++.h>
using namespace std;

struct Graph
{
    int num_points;
    int num_ranges;
    // vector of point indices in range i.
    // The index of the outer vector corresponds to a range's ID.
    // The inner vector at that index stores the IDs of all the points contained within that specific range.
    vector<vector<int>>
        ranges_to_points;

    Graph(int n, int m) : num_points(n), num_ranges(m), ranges_to_points(m)
    {
    }

    void add_edge(int range_idx, int point_idx)
    {
        if (range_idx < num_ranges && point_idx < num_points)
        {
            ranges_to_points[range_idx].push_back(point_idx);
        }
    }
};

class HittingSetSolver
{
public:
    HittingSetSolver(const Graph &graph) : graph_(graph),
                                           num_points_(graph.num_points),
                                           num_ranges_(graph.num_ranges) {}

    optional<unordered_set<int>> find_hitting_set()
    {
        if (num_points_ == 0)
            return unordered_set<int>();

        for (int c_prime = 1; c_prime <= num_points_; c_prime *= 2)
        {
            cout << "Attempting to find hitting set with optimal size c' = " << c_prime << " xxx\n";
            auto result = find_hitting_set_for_c(c_prime);
            if (result)
            {
                cout << "\n>>> Success! Found a hitting set.\n";
                return result;
            }
            cout << "Failed to find a hitting set assuming c' = " << c_prime
                 << ". Doubling c'...\n\n";
        }
        return nullopt; // Should not be reached if a hitting set exists.
    }

private:
    optional<unordered_set<int>> find_hitting_set_for_c(int c_prime)
    {
        // Initialize weights of all points to 1.
        vector<double> weights(num_points_, 1.0);

        // Lemma 3.4. If there is a hitting set of size c, the doubling process cannot iterate more than 4c log(n / c) times, and the total weight will not exceed n4 / c 3 .

        double max_iterations_double = 4.0 * c_prime * log((double)num_points_ / c_prime);
        
        int max_iterations = static_cast<int>(ceil(max_iterations_double));

        if (c_prime >= num_points_)
            max_iterations = 1;
        // Avoid log(x) where x<=1

        cout << "Max iterations for c'=" << c_prime << " is " << max_iterations << endl;

        for (int i = 0; i < max_iterations; ++i)
        {
            cout << "Iteration " << i + 1 << ":" << endl;

            // 1. Select a (1/2c')-net.

            double epsilon = 1.0 / (2.0 * c_prime);
            unordered_set<int> net = find_weighted_epsilon_net(epsilon, weights);

            cout << "  - Computed a (1/" << 2 * c_prime << ")-net of size " << net.size() << ".";

            // 2. Verify if the net is a hitting set.
            optional<int> missed_range_idx = verify_hitting_set(net);

            if (!missed_range_idx)
            {
                // Verification succeeded: the net is a hitting set.
                cout << " Net is a valid hitting set.\n";
                return net;
            }

            // Verification failed: the net missed at least one range.
            int missed_idx = *missed_range_idx;
            cout << " Net is not a hitting set. Missed range #" << missed_idx << ".\n";

            // 3. Double the weights of all points in the missed range.
            cout << "  - Doubling weights for points in missed range: { ";
            for (int point_idx : graph_.ranges_to_points[missed_idx])
            {
                weights[point_idx] *= 2.0;
                cout << point_idx << " ";
            }
            cout << "}\n";
        }

        return nullopt;
    }

    // A "Net Finder" implementation.

    unordered_set<int> find_weighted_epsilon_net(double epsilon, const vector<double> &weights)
    {
        unordered_set<int> net;
        double total_weight = accumulate(weights.begin(), weights.end(), 0.0);
        double weight_threshold = epsilon * total_weight;

        vector<bool> is_range_hit(num_ranges_, false);
        bool found_heavy_unhit_range = true;

        while (found_heavy_unhit_range)
        {
            found_heavy_unhit_range = false;
            int best_range_to_hit = -1;
            double max_weight = -1.0;

            // heaviest un-hit range
            for (int i = 0; i < num_ranges_; ++i)
            {
                if (!is_range_hit[i])
                {
                    double current_range_weight = 0.0;
                    for (int point_idx : graph_.ranges_to_points[i])
                    {
                        current_range_weight += weights[point_idx];
                    }
                    if (current_range_weight > weight_threshold && current_range_weight > max_weight)
                    {
                        max_weight = current_range_weight;
                        best_range_to_hit = i;
                        found_heavy_unhit_range = true;
                    }
                }
            }

            if (found_heavy_unhit_range)
            {
                // Add the heaviest point from this heaviest range to the net
                // because  it makes the most sense to add teh point with the most point in the net out of the heaviest range
                // this parrt of the implementation has been left open to do whatever in the paper
                const auto &points_in_range = graph_.ranges_to_points[best_range_to_hit];
                int point_to_add = *max_element(points_in_range.begin(), points_in_range.end(),
                                                [&](int a, int b)
                                                { return weights[a] < weights[b]; });

                net.insert(point_to_add);

                // Mark all ranges hit by this new point
                // all the ranges that the new point that we added are hit by it we need to mark them all
                for (int i = 0; i < num_ranges_; ++i)
                {
                    if (!is_range_hit[i])
                    {
                        for (int p_idx : graph_.ranges_to_points[i])
                        {
                            if (p_idx == point_to_add)
                            {
                                is_range_hit[i] = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        return net;
    }

    // null if all hit
    optional<int> verify_hitting_set(const unordered_set<int> &hitting_set) const
    {
        for (int i = 0; i < num_ranges_; ++i)
        {
            bool is_hit = false;
            for (int point_idx : graph_.ranges_to_points[i])
            {
                if (hitting_set.count(point_idx))
                {
                    is_hit = true;
                    break;
                }
            }
            if (!is_hit)
            {
                return i; // This range was missed.
            }
        }
        return nullopt; // All ranges were hit.
    }

    const Graph &graph_;
    int num_points_;
    int num_ranges_;
};

void print_hitting_set(const optional<unordered_set<int>> &result)
{
    if (result)
    {
        cout << "\nFinal Hitting Set (point indices): { ";
        for (int point_idx : *result)
        {
            cout << point_idx << " ";
        }
        cout << "}\n";
        cout << "Size of hitting set: " << result->size() << endl;
    }
    else
    {
        cout << "Could not find a hitting set." << endl;
    }
}

int main(int argc, char **argv)
{
    // This main now supports reading a bipartite graph from a text file.
    // Usage (recommended):
    //   paper1algocopy.exe <path-to-graph-file>
    // Or set environment variable INPUT_GRAPH_FILE to the path before running.
    // If no file is provided, the program falls back to the built-in example.

    Graph graph(0, 0);
    bool loaded_from_file = false;

    // Helper: load graph from a file. Format supported:
    //   First non-empty line: "n m" or "n m e"
    //   Following lines: pairs of integers (range_idx point_idx) representing edges.
    //   Comments starting with '#' are ignored. Both 0-based and 1-based indices are accepted.
    auto load_graph_from_file = [&](const string &path) -> bool
    {
        ifstream in(path);
        if (!in)
        {
            cerr << "Could not open file: " << path << "\n";
            return false;
        }

        string line;
        vector<vector<int>> lines_ints;
        while (getline(in, line))
        {
            auto pos = line.find('#');
            if (pos != string::npos)
                line = line.substr(0, pos);
            istringstream ss(line);
            int x;
            vector<int> ints;
            while (ss >> x)
                ints.push_back(x);
            if (!ints.empty())
                lines_ints.push_back(move(ints));
        }

        if (lines_ints.empty())
        {
            cerr << "No numeric input found in file: " << path << "\n";
            return false;
        }

        if (lines_ints[0].size() < 2)
        {
            cerr << "First line must contain at least two integers: n m\n";
            return false;
        }

        int n = lines_ints[0][0];
        int m = lines_ints[0][1];
        int e = -1;
        if (lines_ints[0].size() >= 3)
            e = lines_ints[0][2];

        vector<int> remaining_ints;
        for (size_t j = 0; j < lines_ints[0].size(); ++j)
            if (j >= 3)
                remaining_ints.push_back(lines_ints[0][j]);
        for (size_t i = 1; i < lines_ints.size(); ++i)
            for (int v : lines_ints[i])
                remaining_ints.push_back(v);

        vector<pair<int, int>> edges;
        if (e >= 0)
        {
            for (size_t k = 0; k + 1 < remaining_ints.size() && (int)edges.size() < e; k += 2)
                edges.emplace_back(remaining_ints[k], remaining_ints[k + 1]);
        }
        else
        {
            for (size_t k = 0; k + 1 < remaining_ints.size(); k += 2)
                edges.emplace_back(remaining_ints[k], remaining_ints[k + 1]);
        }

        bool need_subtract = false;
        for (auto &pr : edges)
        {
            int r = pr.first;
            int p = pr.second;
            if (r >= m || p >= n)
            {
                if (r - 1 >= 0 && r - 1 < m && p - 1 >= 0 && p - 1 < n)
                {
                    need_subtract = true;
                    break;
                }
            }
        }

        if (need_subtract)
        {
            for (auto &pr : edges)
            {
                pr.first -= 1;
                pr.second -= 1;
            }
        }

        graph = Graph(n, m);
        for (auto &pr : edges)
        {
            if (pr.first >= 0 && pr.first < m && pr.second >= 0 && pr.second < n)
                graph.add_edge(pr.first, pr.second);
            else
                cerr << "Warning: ignoring out-of-range edge (" << pr.first << ", " << pr.second << ")\n";
        }

        return true;
    };

    // Prefer command-line argument if provided
    if (argc > 1)
    {
        string path = argv[1];
        loaded_from_file = load_graph_from_file(path);
    }
    else
    {
        const char *envp = getenv("INPUT_GRAPH_FILE");
        if (envp != nullptr)
            loaded_from_file = load_graph_from_file(string(envp));
    }

    if (!loaded_from_file)
    {
        // Fallback: built-in example
        graph = Graph(6, 5);
        graph.add_edge(0, 0);
        graph.add_edge(0, 1);
        graph.add_edge(0, 4);
        graph.add_edge(1, 0);
        graph.add_edge(1, 2);
        graph.add_edge(1, 5);
        graph.add_edge(2, 1);
        graph.add_edge(2, 3);
        graph.add_edge(3, 2);
        graph.add_edge(3, 3);
        graph.add_edge(4, 4);
        graph.add_edge(4, 5);
    }

    // tc 2
    // Points X = {0, 1, 2, 3, 4, 5, 6, 7} (n=8)
    // Ranges R =
    // {
    //   R0: {0, 7},
    //   R1: {1, 7},
    //   R2: {2, 3},
    //   R3: {4, 5},
    //   R4: {3, 4},
    //   R5: {6}
    // } (m=6)
    // Optimal hitting set is size 4, e.g., {3, 4, 6, 7}

    // uncomment from here for tc 2
    // Graph graph(8, 6);
    // graph.add_edge(0, 0);
    // graph.add_edge(0, 7);
    // graph.add_edge(1, 1);
    // graph.add_edge(1, 7);
    // graph.add_edge(2, 2);
    // graph.add_edge(2, 3);
    // graph.add_edge(3, 4);
    // graph.add_edge(3, 5);
    // graph.add_edge(4, 3);
    // graph.add_edge(4, 4);
    // graph.add_edge(5, 6);

    cout << "Finding hitting set for the graph...\n"
         << endl;

    HittingSetSolver solver(graph);
    auto result = solver.find_hitting_set();

    print_hitting_set(result);

    return 0;
}