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
        cout << "-----------CITY TRAFFIC NETWORK--------------" << endl;
        for (int i = 0; i < node_count; i++)
        {
            cout << node_name[i] << " -> ";
            bool first = true;
            for (int j = 0; j < node_count; j++)
            {
                if (adjacency_matrix[i][j] != inf && adjacency_matrix[i][j] != 0) //  connected nodes bass dikhain gay
                {
                    if (!first)
                    {
                        cout << " ";
                    }
                    cout << "(" << node_name[j] << "," << adjacency_matrix[i][j] << ")";
                    first = false;
                }
            }
            cout << endl;
        }
    }

    //* REMEMBER THAT BLOCKED ROADS ARE MARKED WITH inf-1
    void block_road(char node1, char node2)
    {
        int index1 = get_node_index(node1);
        int index2 = get_node_index(node2);

        if (index1 == -1 || index2 == -1)
        {
            cout << "road not found." << endl;
            return;
        }

        // if not alr blocked
        if (adjacency_matrix[index1][index2] != 0)
        {
            adjacency_matrix[index1][index2] = inf - 1;
            adjacency_matrix[index2][index1] = inf - 1;
        }
    }

    void load_road_closures()
    {
        ifstream file("road_closures.csv");
        if (!file.is_open())
        {
            cout << "error opening file." << endl;
            return;
        }

        string line;
        getline(file, line); // Skip header
        while (getline(file, line))
        {
            stringstream ss(line);
            string Intersection1, Intersection2, Status;
            getline(ss, Intersection1, ',');
            getline(ss, Intersection2, ',');
            getline(ss, Status, ',');

            if (Status == "Blocked")
            {
                block_road(Intersection1[0], Intersection2[0]);
            }
        }
        file.close();
    }

    void blockedroad_to_csv(char node1, char node2)
    {
        // app=>append
        ofstream file("road_closures.csv", ios::app);
        if (!file.is_open())
        {
            cout << "error opening file." << endl;
            return;
        }

        file << node1 << "," << node2 << ",Blocked\n";
        file.close();
    }

    void display_blocked_roads()
    {
        // we print only upper triangle of the adjacency matrix -> inorder to avoid repitition
        cout << "-----------BLOCKED ROADS--------------" << endl;
        for (int i = 0; i < node_count; i++)
        {
            for (int j = i + 1; j < node_count; j++)
            {
                if (adjacency_matrix[i][j] == inf - 1)
                {
                    cout << node_name[i] << " to " << node_name[j] << " is blocked " << endl;
                }
            }
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
    string priority;

    vehicle()
    {
        Graph g;
        load_data(g);
    }

    vehicle(string id, char start, char end, string priority = "low")
    {
        this->id = id;
        this->start = start;
        this->end = end;
        this->priority = priority;
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

class emergency_vehicle
{
public:
    string id;
    char start;
    char end;
    string priority;

    emergency_vehicle()
    {
        Graph g;
        load_data(g);
    }

    emergency_vehicle(string id, char start, char end, string priority)
    {
        this->id = id;
        this->start = start;
        this->end = end;
        this->priority = priority;
    }

    void move(Graph &g)
    {
        cout << "emergency_vehicle " << id << " is moving from " << start << " to " << end << endl;
        g.dijkstra(start, end);
    }

    void load_data(Graph &g)
    {
        ifstream file("emergency_vehicles.csv");
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }
        string line;
        getline(file, line);

        while (getline(file, line))
        {
            stringstream ss(line);
            string start, end, id, priority;
            getline(ss, id, ',');
            getline(ss, start, ',');
            getline(ss, end, ',');
            getline(ss, priority, ',');
            emergency_vehicle v(id, start[0], end[0], priority);
            v.move(g);
        }
        file.close();
    }
};

class VehicleManager : public vehicle
{
private:
    static const int MAX_VEHICLES = 100; // iskay sath jo marzi lagao
    vehicle vehicles[MAX_VEHICLES];      // for  vehicle objects
    int vehicleCount;

public:
    VehicleManager()
    {
        vehicleCount = 0;
    }

    void addVehicle(const vehicle &v)
    {
        if (vehicleCount >= MAX_VEHICLES)
        {
            cout << "VehicleManager is full. Cannot add more vehicles." << endl;
            return;
        }
        vehicles[vehicleCount++] = v;
        cout << "Vehicle with ID " << v.id << " added." << endl;
    }

    void removeVehicle(const string &vehicleID)
    {
        bool found = false;
        for (int i = 0; i < vehicleCount; ++i)
        {
            if (vehicles[i].id == vehicleID)
            {
                found = true;

                for (int j = i; j < vehicleCount - 1; ++j)
                {
                    vehicles[j] = vehicles[j + 1];
                }
                vehicleCount--;
                cout << "Vehicle with ID " << vehicleID << " removed." << endl;
                break;
            }
        }
        if (!found)
        {
            cout << "Vehicle with ID " << vehicleID << " not found." << endl;
        }
    }

    void updateRoutes(Graph &g)
    {
        for (int i = 0; i < vehicleCount; ++i)
        {
            cout << "Updating route for vehicle ID " << vehicles[i].id << ":" << endl;
            g.dijkstra(vehicles[i].start, vehicles[i].end);
        }
    }

    void displayVehiclePositions()
    {
        cout << "Vehicle Positions:" << endl;
        for (int i = 0; i < vehicleCount; ++i)
        {
            cout << "Vehicle ID: " << vehicles[i].id << " at node " << vehicles[i].start << " heading to " << vehicles[i].end << endl;
        }
    }
};

class Signal
{
public:
    char ch;
    int green_light_time;

    void add_signal(char signal, int time)
    {
        ch = signal;
        green_light_time = time;
    }
};

class Signals
{
private:
    static const int max_signals = 26; // Defin the maximum number of signals
    Signal signals[max_signals];
    int current_size = 0;

public:
    Signals()
    {
        load_data();
    }
    Signal &operator[](int index)
    {
        if (index >= 0 && index < current_size)
        {
            return signals[index];
        }
        else
        {
            cout << "Index out of range\n";
            return signals[0];
        }
    }

    void load_data()
    {
        ifstream file("traffic_signals.csv");
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }
        string line;
        getline(file, line);
        while (getline(file, line) && current_size < max_signals)
        {
            stringstream ss(line);
            string signal, time;
            getline(ss, signal, ',');
            getline(ss, time, ',');
            signals[current_size].add_signal(signal[0], stoi(time));
            current_size++;
        }
        file.close();
    }
};

int main()
{
    Graph g;
    g.print_graph();
    g.dijkstra('P', 'K');
    vehicle v;

    g.load_road_closures();
    g.display_blocked_roads();
    char start, end;
    cout << "Enter the road to block (start, end): ";
    cin >> start >> end;
    g.block_road(start, end);
    g.blockedroad_to_csv(start, end);
    cout << "UPDATED:" << endl;
    g.display_blocked_roads();
    g.dijkstra(start, end);

    return 0;
}