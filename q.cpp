

// Amna Zubair  23i-2556   DS-C
// Zara Tanveer 23i-2507   DS-C
// Ali Asjad    23i-2648   DS-C

// git clone https://github.com/Amna1922/DS-Final-project.git
// cd
// git add .
// git commit -m "msg"
// git push
// #include <vector>
// #include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#define max_node 100
#define inf 999999

using namespace std;

const int HASH_SIZE = 1000;

class Hash
{
public:
    // Hash function to convert a string key into an integer index
    int operator()(const string &key) const
    {
        int hashValue = 0;
        for (char c : key)
        {
            hashValue = (hashValue * 31 + c) % HASH_SIZE;
        }
        return hashValue;
    }
};

// Define a struct to store key-value pairs
class HashMap
{
private:
    static const int TABLE_SIZE = 100;
    struct Entry
    {
        string road;      // Road as a string (e.g., "A-B")
        int vehicleCount; // Number of vehicles on that road
        Entry *next;

        Entry(string road, int vehicleCount)
            : road(road), vehicleCount(vehicleCount), next(nullptr) {}
    };

    Entry *table[TABLE_SIZE];

    // Hash function to calculate index for the road
    int hashFunction(const string &road)
    {
        int hash = 0;
        for (char c : road)
        {
            hash = (hash * 31 + c) % TABLE_SIZE;
        }
        return hash;
    }

public:
    HashMap()
    {
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = nullptr;
    }

    // Add or update the vehicle count for a given road
    void put(const string &road, int count)
    {
        int index = hashFunction(road);
        Entry *entry = table[index];

        while (entry != nullptr)
        {
            if (entry->road == road)
            {
                cout << "Updating road " << road << " with count " << count << endl;
                entry->vehicleCount = count;
                return;
            }
            entry = entry->next;
        }

        cout << "Adding new road " << road << " with count " << count << endl;
        entry = new Entry(road, count);
        entry->next = table[index];
        table[index] = entry;
    }

    // Get the vehicle count for a given road
    int get(const string &road)
    {
        int index = hashFunction(road);
        Entry *entry = table[index];

        // Search for the road
        while (entry != nullptr)
        {
            if (entry->road == road)
                return entry->vehicleCount;
            entry = entry->next;
        }

        return 0; // Return 0 if the road is not found
    }
};

class Graph
{
private:
    int adjacency_matrix[max_node][max_node];
    char node_name[max_node];
    int node_count;
    Hash hasher;

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

    bool is_valid_road(int from_index, int to_index)
    {
        return adjacency_matrix[from_index][to_index] != inf;
    }

    void add_node(char node)
    {
        node_name[node_count] = node;
        node_count++;
    }

    int (*get_adjacency_matrix())[max_node]
    {
        return adjacency_matrix;
    }

    char *get_node_name()
    {
        return node_name;
    }

    int get_node_count()
    {
        return node_count;
    }

    void add_edge(char node1, char node2, int traveltime)
    {
        // Check if nodes already exist in the graph
        int node1_index = get_node_index(node1);
        int node2_index = get_node_index(node2);

        cout << "Adding edge between " << node1 << " and " << node2 << " with travel time " << traveltime << endl;

        // If node1 doesn't exist, add it
        if (node1_index == -1)
        {
            node_name[node_count] = node1;
            node1_index = node_count;
            node_count++;
        }

        // If node2 doesn't exist, add it
        if (node2_index == -1)
        {
            node_name[node_count] = node2;
            node2_index = node_count;
            node_count++;
        }

        // Update the adjacency matrix with the travel time
        adjacency_matrix[node1_index][node2_index] = traveltime;
        adjacency_matrix[node2_index][node1_index] = traveltime; // Since roads are bidirectional
    }
    const Hash &get_hash_function() const
    {
        return hasher;
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
        getline(file, line); // Skip header
        while (getline(file, line))
        {
            stringstream ss(line);
            string start, end, traveltime;
            getline(ss, start, ',');
            getline(ss, end, ',');
            getline(ss, traveltime, ',');

            cout << "Loading road: " << start << " to " << end << " with travel time: " << traveltime << endl;
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
                if (adjacency_matrix[i][j] != inf && adjacency_matrix[i][j] != 0)
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

    void block_road(char node1, char node2)
    {
        int index1 = get_node_index(node1);
        int index2 = get_node_index(node2);

        if (index1 == -1 || index2 == -1)
        {
            cout << "Road not found." << endl;
            return;
        }

        // Block the road (set adjacency to 0)
        if (adjacency_matrix[index1][index2] != 0)
        {
            adjacency_matrix[index1][index2] = 0;
            adjacency_matrix[index2][index1] = 0;
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
                if (adjacency_matrix[i][j] == 0)
                {
                    cout << node_name[i] << " to " << node_name[j] << " is blocked " << endl;
                }
            }
        }
    }
    // Assuming maximum nodes is 100
#define MAX_NODESs 100

    char *dijkstra(char start, char end)
    {
        int *distance = new int[node_count];
        bool *visited = new bool[node_count];
        int *parent = new int[node_count];
        int start_index = get_node_index(start);
        int end_index = get_node_index(end);

        static char path[MAX_NODESs]; // Static array to store the path
        int pathIndex = 0;            // To track the current index in the path

        if (start_index == -1 || end_index == -1)
        {
            cout << "Invalid node" << endl;
            return nullptr; // Return nullptr if no path
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
                break; // All nodes are visited

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

        // Reconstruct the path from end to start using the parent array
        int current = end_index;
        while (current != -1)
        {
            path[pathIndex++] = node_name[current]; // Store the current node in path
            current = parent[current];
        }

        // Reverse the path to get it from start to end
        for (int i = 0; i < pathIndex / 2; i++)
        {
            char temp = path[i];
            path[i] = path[pathIndex - i - 1];
            path[pathIndex - i - 1] = temp;
        }

        return path; // Return the path array
    }
};

class Queue
{
private:
    static const int MAX_SIZE = 100;
    int data[MAX_SIZE];
    int front, rear, size;

public:
    Queue() : front(0), rear(0), size(0) {}

    bool isEmpty() { return size == 0; }

    bool isFull() { return size == MAX_SIZE; }

    void enqueue(int value)
    {
        if (isFull())
        {
            cout << "Queue Overflow" << endl;
            return;
        }
        data[rear] = value;
        rear = (rear + 1) % MAX_SIZE;
        size++;
    }

    int dequeue()
    {
        if (isEmpty())
        {
            cout << "Queue Underflow" << endl;
            return -1;
        }
        int value = data[front];
        front = (front + 1) % MAX_SIZE;
        size--;
        return value;
    }
};

class MinHeap
{
public:
    struct Road
    {
        int startIndex;
        int endIndex;
        int vehicleCount;

        bool operator<(const Road &other) const
        {
            return vehicleCount > other.vehicleCount;
        }
    };

    Road heap[1000];
    int size;

    void heapifyUp(int index)
    {
        while (index > 0)
        {
            int parent = (index - 1) / 2;
            if (heap[parent] < heap[index])
                break;
            swap(heap[parent], heap[index]);
            index = parent;
        }
    }

    void heapifyDown(int index)
    {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < size && heap[left] < heap[smallest])
        {
            smallest = left;
        }

        if (right < size && heap[right] < heap[smallest])
        {
            smallest = right;
        }

        if (smallest != index)
        {
            swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    MinHeap() : size(0) {}

    void insert(int start, int end, int vehicleCount)
    {
        if (size >= 1000)
        {
            cout << "Heap Overflow" << endl;
            return;
        }
        heap[size] = {start, end, vehicleCount};
        heapifyUp(size);
        size++;
    }

    Road extractMin()
    {
        if (size == 0)
        {
            cout << "Heap Underflow" << endl;
            return {-1, -1, -1};
        }
        Road min = heap[0];
        heap[0] = heap[size - 1];
        size--;
        heapifyDown(0);
        return min;
    }

    bool isEmpty()
    {
        return size == 0;
    }
};

class CongestionManager
{
private:
    HashMap vehicleCounts;  // Tracks vehicles on each road using HashMap
    HashMap congestedRoads; // Tracks congested roads using HashMap

    // Helper function to create a road identifier string (e.g., "A-B")
    string createRoadKey(char from, char to)
    {
        if (from < to)
            return string(1, from) + "-" + string(1, to);
        else
            return string(1, to) + "-" + string(1, from);
    }

public:
    // Monitor traffic and update congestion
    void monitorTraffic(Graph &g, char from, char to)
    {
        string roadKey = createRoadKey(from, to); // Generate the key for the road segment
        int currentCount = vehicleCounts.get(roadKey);

        cout << "Before update: Road " << roadKey << " has " << currentCount << " vehicles." << endl;

        // Increment the vehicle count for this road
        vehicleCounts.put(roadKey, currentCount + 1);

        cout << "After update: Road " << roadKey << " now has " << currentCount + 1 << " vehicles." << endl;

        // Get the road weight (travel time) from the graph
        int fromIndex = g.get_node_index(from);
        int toIndex = g.get_node_index(to);
        int roadWeight = g.get_adjacency_matrix()[fromIndex][toIndex];

        if (currentCount + 1 >= roadWeight)
        {
            congestedRoads.put(roadKey, 1);
            cout << "Road " << roadKey << " is now [CONGESTED]" << endl;
        }
    }

    // Display congestion levels
    void displayCongestionLevels(Graph &g)
    {
        cout << "-----------Congestion Status--------------" << endl;
        for (int i = 0; i < g.get_node_count(); i++)
        {
            for (int j = i + 1; j < g.get_node_count(); j++)
            {
                char from = g.get_node_name()[i];
                char to = g.get_node_name()[j];
                string roadKey = createRoadKey(from, to);

                int count = vehicleCounts.get(roadKey);
                int roadWeight = g.get_adjacency_matrix()[i][j];

                if (count > 0 && roadWeight != inf)
                {
                    cout << from << " to " << to
                         << " -> vehicles: " << count
                         << " (Road weight: " << roadWeight << ")"
                         << (congestedRoads.get(roadKey) ? " [CONGESTED]" : "") << endl;
                }
            }
        }
    }
};

class vehicle
{
public:
    string id;
    char start;
    char end;
    char current; // Stores the vehicle's current position
    string priority;

    vehicle() {}

    vehicle(string id, char start, char end, string priority = "low")
    {
        this->id = id;
        this->start = start;
        this->end = end;
        this->current = start; // Initialize current position to the starting position
        this->priority = priority;
    }

    // The move method will update the congestion for each road in the path
    void move(Graph &g, CongestionManager &cm)
    {
        cout << "Vehicle " << id << " is moving from " << start << " to " << end << endl;

        // Get the shortest path from start to end using Dijkstra
        char *path = g.dijkstra(start, end);

        if (path == nullptr)
        {
            cout << "No path found!" << endl;
            return;
        }

        // Find the current position in the path
        int pathIndex = 0;
        while (path[pathIndex] != '\0' && path[pathIndex] != current)
        {
            pathIndex++;
        }

        // Check if we can move to the next node
        if (path[pathIndex + 1] != '\0')
        {
            char nextNode = path[pathIndex + 1];

            // Update congestion for the road segment
            cm.monitorTraffic(g, current, nextNode);

            // Move to the next node
            current = nextNode;

            cout << "Vehicle " << id << " moved to " << current << endl;
        }
        else
        {
            cout << "Vehicle " << id << " has reached its destination " << end << endl;
        }
    }

    // Load data and move vehicles
    void load_data(Graph &g, CongestionManager &cm)
    {
        ifstream file("vehicles.csv");
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }

        string line;
        getline(file, line); // Skip the header
        while (getline(file, line))
        {
            stringstream ss(line);
            string id, start, end, priority;
            getline(ss, id, ',');
            getline(ss, start, ',');
            getline(ss, end, ',');
            getline(ss, priority, ',');

            // Create and move vehicle
            vehicle v(id, start[0], end[0], priority);
            v.move(g, cm); // Move the vehicle and update congestion
        }
        file.close();
    }
};

class VehicleManager
{
private:
    static const int MAX_VEHICLES = 100;
    vehicle vehicles[MAX_VEHICLES];
    int vehicleCount;

public:
    VehicleManager() : vehicleCount(0) {}

    void load_data(Graph &g, CongestionManager &cm)
    {
        ifstream file("vehicles.csv");
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }

        string line;
        getline(file, line); // Skip the header

        while (getline(file, line))
        {
            stringstream ss(line);
            string id, start, end, priority;
            getline(ss, id, ',');
            getline(ss, start, ',');
            getline(ss, end, ',');
            getline(ss, priority, ',');

            if (vehicleCount < MAX_VEHICLES)
            {
                vehicles[vehicleCount++] = vehicle(id, start[0], end[0], priority);
            }
        }
        file.close();
    }

    void move_vehicles(Graph &g, CongestionManager &cm)
    {
        for (int i = 0; i < vehicleCount; i++)
        {
            vehicles[i].move(g, cm);
        }
    }

    void displayVehiclePositions()
    {
        cout << "\nVehicle Positions:" << endl;
        for (int i = 0; i < vehicleCount; i++)
        {
            cout << "Vehicle ID: " << vehicles[i].id
                 << " at node " << vehicles[i].current
                 << " heading to " << vehicles[i].end
                 << " (Priority: " << vehicles[i].priority << ")" << endl;
        }
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
    // Graph g;
    // g.print_graph();
    // g.dijkstra('P', 'K');
    // CongestionManager cm;
    // VehicleManager vm;
    Graph g;
    g.print_graph(); // Display the initial city traffic network

    // Create a congestion manager to track congestion levels on roads
    CongestionManager cm;

    // Create a vehicle manager and load vehicle data
    VehicleManager vm;

    // Load vehicle data and simulate the movement of vehicles
    vm.load_data(g, cm);
    vm.displayVehiclePositions();
    // vm.displayVehiclePositions();
    // Display vehicle positions after movement
    // vm.displayVehiclePositions();
    vm.move_vehicles(g, cm);
    // Display congestion levels for each road
    cm.displayCongestionLevels(g);
    vm.move_vehicles(g, cm);
    // Simulate more vehicle movements and update congestion
    // cout << "\nSimulating additional vehicle movements...\n";
    // vm.load_data(g, cm); // Load the vehicle data again to simulate another batch of vehicle movements

    // // Display updated congestion levels
    cm.displayCongestionLevels(g);
    // Load vehicle data and update congestion
    // vm.load_data(g, cm);
    // vm.displayVehiclePositions();
    // Display vehicle positions
    // vm.displayVehiclePositions();

    // Display congestion levels
    // cm.displayCongestionLevels(g);

    // Find and display the most congested road
    // cm.findMostCongestedRoad(g);

    // g.load_road_closures();
    // g.display_blocked_roads();
    // char start, end;
    // cout << "Enter the road to block (start, end): ";
    // cin >> start >> end;
    // g.block_road(start, end);
    // g.blockedroad_to_csv(start, end);
    // cout << "UPDATED:" << endl;
    // g.display_blocked_roads();
    // g.dijkstra(start, end);

    return 0;
}