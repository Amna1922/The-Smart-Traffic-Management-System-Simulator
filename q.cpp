
// Amna Zubair  23i-2556   DS-C
// Zara Tanveer 23i-2507   DS-C
// Ali Asjad    23i-2648   DS-C

// git clone https://github.com/Amna1922/DS-Final-project.git
// cd
// git add .
// git commit -m "msg"
// git push

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#define max_node 100
#define inf 999999

using namespace std;

class Graph
{
private:
    int adjacency_matrix[max_node][max_node];
    char node_name[max_node];
    int node_count;

    int get_node_index(char node)
    {
        for (int i = 0; i < node_count; i++)
        {
            if (node_name[i] == node)
            {
                return i;
            }
        }
        return -1;
    }

public:
    Graph()
    {
        node_count = 0;
        for (int i = 0; i < max_node; i++)
        {
            for (int j = 0; j < max_node; j++)
            {
                if (i == j)
                    adjacency_matrix[i][j] = 0;
                else
                    adjacency_matrix[i][j] = inf;
            }
        }
        load_data();
    }

    void make_marix(int n) {}

    void add_node(char node)
    {
        node_name[node_count] = node;
        node_count++;
    }

    void add_edge(char node1, char node2, int traveltime)
    {
        int node1_index = get_node_index(node1);
        if (node1_index == -1)
        {
            node_name[node_count] = node1;
            node1_index = node_count;
            node_count++;
            // if node not found then increment the node count
        }

        int node2_index = get_node_index(node2);
        if (node2_index == -1)
        {
            node_name[node_count] = node2;
            node2_index = node_count;
            node_count++;
        }

        adjacency_matrix[node1_index][node2_index] = traveltime;
        adjacency_matrix[node2_index][node1_index] = traveltime;
        // because this is an undirected graph
    }

    void load_data()
    {
        ifstream file("road_network.csv");
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }
        string line;
        getline(file, line);
        bool first = true;
        while (getline(file, line))
        {
            if (first)
            {
                first = false;
                continue;
            }
            stringstream ss(line);
            string start, end, traveltime;
            getline(ss, start, ',');
            getline(ss, end, ',');
            getline(ss, traveltime, ',');
            add_edge(start[0], end[0], stoi(traveltime));
        }
        file.close();
    }
    void print_graph()
    {
        for (int i = 0; i < node_count; i++)
        {
            for (int j = 0; j < node_count; j++)
            {
                if (adjacency_matrix[i][j] == inf)
                    cout << "-" << " ";
                else
                    cout << adjacency_matrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    void dijkstra(char start, char end)
    {
        int *distance = new int[node_count];
        bool *visited = new bool[node_count];
        int *parent = new int[node_count];
        int start_index = get_node_index(start);
        int end_index = get_node_index(end);

        if (start_index == -1 || end_index == -1)
        {
            cout << "Invalid node" << endl;
            return;
        }

        for (int i = 0; i < node_count; i++)
        {
            distance[i] = inf;
            visited[i] = false;
            parent[i] = -1;
        }

        distance[start_index] = 0;
        for (int i = 0; i < node_count; i++)
        {
            int min_distance = inf;
            int min_index = -1;

            for (int j = 0; j < node_count; j++)
            {
                if (!visited[j] && distance[j] < min_distance)
                {
                    min_distance = distance[j];
                    min_index = j;
                }
            }
            if (min_index == -1)
                break; // all nodes r visited

            visited[min_index] = true;

            for (int j = 0; j < node_count; j++)
            {
                if (!visited[j] && adjacency_matrix[min_index][j] != inf && distance[min_index] + adjacency_matrix[min_index][j] < distance[j])
                {
                    distance[j] = distance[min_index] + adjacency_matrix[min_index][j];
                    parent[j] = min_index;
                }
            }
        }
        cout << "Shortest path from " << start << " to " << end << " is: ";
        int current = end_index;
        string str = "";
        while (current != start_index)
        {
            cout << node_name[current] << " <- ";
            str = node_name[current] + str;
            current = parent[current];
        }
        cout << node_name[start_index] << endl;
        cout << "Total distance: " << distance[end_index] << endl;
    }
};

class vehicle
{
public:
    string id;
    char start;
    char end;

    vehicle()
    {
        Graph g;
        load_data(g);
    }

    vehicle(string id, char start, char end)
    {
        this->id = id;
        this->start = start;
        this->end = end;
    }

    void move(Graph &g)
    {
        cout << "Vehicle " << id << " is moving from " << start << " to " << end << endl;
        g.dijkstra(start, end);
    }

    void load_data(Graph &g)
    {
        ifstream file("vehicles.csv");
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }
        string line;
        getline(file, line);
        // bool first = true;
        while (getline(file, line))
        {
            // if (first)
            // {
            //     first = false;
            //     continue;
            // }
            stringstream ss(line);
            string start, end, id;
            getline(ss, id, ',');
            getline(ss, start, ',');
            getline(ss, end, ',');
            vehicle v(id, start[0], end[0]);
            v.move(g);
        }
        file.close();
    }
};

int main()
{
    // Graph g;
    // g.print_graph();

    vehicle v;

    return 0;
}