#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <unordered_set>
#include <optional>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <glpk.h> // <-- Include the GLPK library header

using namespace std;

// (Your existing Graph struct...)
struct Graph
{
    int num_points;
    int num_ranges;
    vector<vector<int>> ranges_to_points;

    Graph(int n, int m) : num_points(n), num_ranges(m), ranges_to_points(m) {}

    void add_edge(int range_idx, int point_idx)
    {
        if (range_idx < num_ranges && point_idx < num_points)
        {
            ranges_to_points[range_idx].push_back(point_idx);
        }
    }
};

// (Your existing HittingSetSolver class...)
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
            auto result = find_hitting_set_for_c(c_prime);
            if (result)
            {
                return result;
            }
        }
        return nullopt;
    }

private:
    optional<unordered_set<int>> find_hitting_set_for_c(int c_prime)
    {
        vector<double> weights(num_points_, 1.0);
        double max_iterations_double = 4.0 * c_prime * log((double)num_points_ / c_prime);
        int max_iterations = static_cast<int>(ceil(max_iterations_double));
        if (c_prime >= num_points_)
            max_iterations = 1;

        for (int i = 0; i < max_iterations; ++i)
        {
            double epsilon = 1.0 / (2.0 * c_prime);
            unordered_set<int> net = find_weighted_epsilon_net(epsilon, weights);
            optional<int> missed_range_idx = verify_hitting_set(net);

            if (!missed_range_idx)
            {
                return net;
            }

            int missed_idx = *missed_range_idx;
            for (int point_idx : graph_.ranges_to_points[missed_idx])
            {
                weights[point_idx] *= 2.0;
            }
        }
        return nullopt;
    }

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

            for (int i = 0; i < num_ranges_; ++i)
            {
                if (!is_range_hit[i])
                {
                    double current_range_weight = 0.0;
                    for (int point_idx : graph_.ranges_to_points[i])
                    {
                        current_range_weight += weights[point_idx];
                    }
                    if (current_range_weight >= weight_threshold && current_range_weight > max_weight)
                    {
                        max_weight = current_range_weight;
                        best_range_to_hit = i;
                        found_heavy_unhit_range = true;
                    }
                }
            }

            if (found_heavy_unhit_range)
            {
                const auto &points_in_range = graph_.ranges_to_points[best_range_to_hit];
                if (points_in_range.empty())
                {
                    // This should not happen if a hitting set exists, but good to check.
                    is_range_hit[best_range_to_hit] = true;
                    continue;
                }
                int point_to_add = *max_element(points_in_range.begin(), points_in_range.end(),
                                                [&](int a, int b)
                                                { return weights[a] < weights[b]; });
                net.insert(point_to_add);
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

    optional<int> verify_hitting_set(const unordered_set<int> &hitting_set) const
    {
        for (int i = 0; i < num_ranges_; ++i)
        {
            if (graph_.ranges_to_points[i].empty())
                continue; // Skip empty ranges

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
                return i;
            }
        }
        return nullopt;
    }

    const Graph &graph_;
    int num_points_;
    int num_ranges_;
};

/**
 * Finds the true optimal hitting set using an
 * Integer Linear Programming (ILP) solver (GLPK).
 */
optional<unordered_set<int>> findOptimalHittingSet_ILP(const Graph &graph)
{
    int n = graph.num_points;
    int m = graph.num_ranges;

    // 1. Create the ILP problem object
    glp_prob *lp = glp_create_prob();
    glp_set_prob_name(lp, "Hitting Set");
    glp_set_obj_dir(lp, GLP_MIN); // We want to MINIMIZE

    // 2. Add variables (columns) - one for each point
    glp_add_cols(lp, n);
    for (int i = 0; i < n; ++i)
    {
        string col_name = "p" + to_string(i);
        glp_set_col_name(lp, i + 1, col_name.c_str());
        glp_set_col_kind(lp, i + 1, GLP_BV); // Binary Variable (0 or 1)
        glp_set_obj_coef(lp, i + 1, 1.0);    // Objective: Minimize sum of p_i
    }

    // 3. Add constraints (rows) - one for each range
    glp_add_rows(lp, m);
    // GLPK requires 1-based indexing for arrays, so we add 1.
    int ind[n + 1];
    double val[n + 1];

    for (int j = 0; j < m; ++j)
    {
        string row_name = "r" + to_string(j);
        glp_set_row_name(lp, j + 1, row_name.c_str());

        // This is the constraint: sum(p_i) >= 1
        glp_set_row_bnds(lp, j + 1, GLP_LO, 1.0, 0.0); // Lower bound of 1.0

        const auto &points_in_range = graph.ranges_to_points[j];
        if (points_in_range.empty())
            continue; // Skip empty ranges

        int k = 0;
        for (int point_idx : points_in_range)
        {
            k++;
            ind[k] = point_idx + 1; // Point index (1-based)
            val[k] = 1.0;           // Coefficient is 1.0
        }
        glp_set_mat_row(lp, j + 1, k, ind, val);
    }

    // 4. Solve the problem
    cout << "--- [ILP Solver] Solving for optimal solution... ---" << endl;
    glp_iocp parm;
    glp_init_iocp(&parm);
    parm.presolve = GLP_ON;     // Use presolver
    parm.msg_lev = GLP_MSG_OFF; // Turn off solver output

    int err = glp_intopt(lp, &parm);
    if (err)
    {
        cout << "--- [ILP Solver] Error: Could not solve ILP. ---" << endl;
        glp_delete_prob(lp);
        return nullopt;
    }

    double z = glp_mip_obj_val(lp); // Get the optimal objective value (min size)
    cout << "--- [ILP Solver] Optimal solution size found: " << z << " ---" << endl;

    // 5. Extract the solution
    unordered_set<int> optimal_set;
    for (int i = 0; i < n; ++i)
    {
        if (glp_mip_col_val(lp, i + 1) == 1.0)
        {
            optimal_set.insert(i); // This point is in the optimal set
        }
    }

    // 6. Clean up
    glp_delete_prob(lp);
    return optimal_set;
}

void print_hitting_set(const string &title, const optional<unordered_set<int>> &result)
{
    cout << "\n--- " << title << " ---" << endl;
    if (result)
    {
        cout << "Hitting Set (point indices): { ";
        for (int point_idx : *result)
        {
            cout << point_idx << " ";
        }
        cout << "}\n";
        cout << "Total Size: " << result->size() << endl;
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

    cout << "Finding hitting set for the graph...\n"
         << endl;

    // 1. Run your approximation algorithm
    HittingSetSolver approx_solver(graph);
    auto approx_result = approx_solver.find_hitting_set();
    print_hitting_set("Approximation Algorithm Result", approx_result);

    // 2. Run the optimal ILP solver
    auto optimal_result = findOptimalHittingSet_ILP(graph);
    print_hitting_set("Optimal ILP Solver Result", optimal_result);

    // 3. Compare
    cout << "\n--- Comparison ---" << endl;
    if (approx_result && optimal_result)
    {
        cout << "Approximation Size: " << approx_result->size() << endl;
        cout << "Optimal Size:       " << optimal_result->size() << endl;
    }

    return 0;
}
