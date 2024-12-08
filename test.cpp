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
#include <cstdlib>
#include <ctime>
#include <cmath>

#define max_node 100
#define max_cars 100
#define inf 999999

using namespace std;

const int HASH_SIZE = 1000;

class Signal
{
public:
    char node;
    int green_light_time;
    int red_light_time;
    time_t last_update; // Time of the last state change
    bool is_green;

    Signal(char node = '\0', int green_light_time = 10, int red_light_time = 5)
    {
        this->node = node;
        this->green_light_time = green_light_time;
        this->red_light_time = red_light_time;
        this->last_update = std::time(nullptr); // Initialize to the current time
        this->is_green = true;
    }

    void update_signal()
    {
        time_t now = std::time(nullptr);                                  // Get the current time
        int duration = static_cast<int>(std::difftime(now, last_update)); // Calculate elapsed time in seconds

        if (is_green && duration >= green_light_time)
        {
            is_green = false;
            last_update = now;
        }
        else if (!is_green && duration >= red_light_time)
        {
            is_green = true;
            last_update = now;
        }
    }

    bool get_signal_state() const
    {
        return is_green;
    }
};

class Signals
{
public:
    static const int max_signals = 26; // Define the maximum number of signals
    Signal signals[max_signals];
    int current_size = 0;

    Signals()
    {
        load_data();
    }

    Signal &operator[](int index)
    {
        static Signal dummy; // Static dummy object for out-of-bounds access
        if (index >= 0 && index < current_size)
        {
            return signals[index];
        }
        else
        {
            cout << "Error: Index " << index << " out of range.\n";
            return dummy;
        }
    }

    void add_signal(const Signal &temp)
    {
        if (current_size < max_signals)
        {
            signals[current_size] = temp;
            current_size++;
        }
        else
        {
            cout << "Error: Cannot add more signals. Maximum limit reached.\n";
        }
    }

    void load_data()
    {
        ifstream file("traffic_signals.csv");
        if (!file.is_open())
        {
            cout << "Error: File not found or could not be opened.\n";
            return;
        }

        string line;
        getline(file, line); // Skip the header line if there's one

        while (getline(file, line) && current_size < max_signals)
        {
            stringstream ss(line);
            string signal, green_time_str;

            if (getline(ss, signal, ',') && getline(ss, green_time_str, ','))
            {
                try
                {
                    int green_time = stoi(green_time_str);
                    add_signal(Signal(signal[0], green_time, 5)); // Assuming default red light time is 5
                }
                catch (const invalid_argument &)
                {
                    cout << "Warning: Invalid data format in line \"" << line << "\". Skipping.\n";
                }
            }
            else
            {
                cout << "Warning: Invalid line format \"" << line << "\". Skipping.\n";
            }
        }

        file.close();
    }

    void update_signals()
    {
        for (int i = 0; i < current_size; i++)
        {
            signals[i].update_signal();
        }
    }

    bool is_signal_open(char node) const
    {
        for (int i = 0; i < current_size; i++)
        {
            if (signals[i].node == node)
            {
                return signals[i].get_signal_state();
            }
        }
        return true; // Default to open if the signal is not found
    }

    void display() const
    {
        cout << "----------- TRAFFIC SIGNALS --------------" << endl;
        for (int i = 0; i < current_size; i++)
        {
            cout << "Intersection " << signals[i].node

                 << ", State: " << (signals[i].is_green ? "Green" : "Red") << endl;
        }
    }
};

// aa
class Hash
{
public:
    int operator()(string &key)
    {
        int hash_value = 0;
        for (size_t i = 0; i < key.length(); ++i)
        {
            char c = key[i];
            hash_value = (hash_value * 31 + c) % HASH_SIZE;
            // here mode 31 ki jaga use 39
            /// ABGH ko hash table ma use ker raha
        }
        return hash_value;
    }
};

// aa
class Entry
{
public:
    string road;        // Road as a string (e.g., "A-B")
    int no_of_vehicles; // Number of vehicles on that road
    Entry *next;

    Entry(string str, int n)
    {
        road = str;
        no_of_vehicles = n;
        next = nullptr;
    }
};

// aa
class hash_map
{
private:
    static const int TABLE_SIZE = 100;
    Entry *table[TABLE_SIZE];

    // Hash function to calculate index for the road
    int hashFunction(const string &road)
    {
        int hash = 0;
        for (size_t i = 0; i < road.length(); i++)
        {
            hash = (hash * 31 + road[i]) % TABLE_SIZE;
        }
        return hash;
    }

public:
    hash_map()
    {
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = nullptr;
    }

    // addding cars for a givenn road
    void put(const string &road, int count)
    {
        int index = hashFunction(road);
        Entry *entry = table[index];

        // Search for the road in the hash table
        while (entry != nullptr)
        {
            if (entry->road == road)
            {
                entry->no_of_vehicles = count; // Update the vehicle count
                return;
            }
            entry = entry->next;
        }

        // If the road is not found, create a new entry
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
            {
                int x = entry->no_of_vehicles;
                return x;
            }
            entry = entry->next;
        }
        return 0; // Return 0 if the road is not found
    }
};

class Queue
{
private:
    static const int MAX_SIZE = 100;
    char data[MAX_SIZE];
    int front, rear, size;

public:
    Queue() : front(0), rear(0), size(0) {}

    bool isEmpty() { return size == 0; }

    bool isFull() { return size == MAX_SIZE; }

    void enqueue(char value)
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

    char dequeue()
    {
        if (isEmpty())
        {
            cout << "Queue Underflow" << endl;
            return '\0';
        }
        char value = data[front];
        front = (front + 1) % MAX_SIZE;
        size--;
        return value;
    }
};
// all 3
class Graph
{
private:
    int adjacency_matrix[max_node][max_node];
    char node_name[max_node];
    int node_count;
    Hash hashing;
    Signals signals;

public:
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
        load_road_closures();
    }

    bool is_valid_road(int s, int e)
    {
        return adjacency_matrix[s][e] != inf;
    }

    void update_signals()
    {
        signals.update_signals();
    }

    bool is_signal_open(char node)
    {
        return signals.is_signal_open(node);
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

    int get_travel_time(char node1, char node2)
    {
        int index1 = get_node_index(node1);
        int index2 = get_node_index(node2);

        if (index1 != -1 && index2 != -1)
            return adjacency_matrix[index1][index2];
        else
            return inf; // if road hi nahi exist kerti to travel time inf return
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
    }

    const Hash &get_hash_function() const
    {
        return hashing;
    } /// bhai ya kia kerta hay abbb??
    // hash function return kerta hay

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
        // first = true;
        while (getline(file, line))
        {
            // if (first)
            // {
            //     first = false;
            //     continue;
            // }
            stringstream ss(line);
            string start, end, traveltime;
            getline(ss, start, ',');
            getline(ss, end, ',');
            getline(ss, traveltime, ',');
            // cout << "Loading road: " << start << " to " << end << " with travel time: " << traveltime << endl;
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
                if (adjacency_matrix[i][j] != inf && adjacency_matrix[i][j] != inf - 1 && adjacency_matrix[i][j] != 0) //  connected nodes bass dikhain gay
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

    int visited[26] = {0};
    char path[26];
    int path_index = 0;
    int weight = 0;
    void print_all_paths(char start, char end, int node_count = 26)
    {
        visited[get_node_index(start)] = 1;
        path[path_index] = start;
        path_index++;

        if (start == end)
        {
            for (int i = 0; i < path_index; i++)
            {
                cout << path[i];
                if (i != path_index - 1)
                    cout << " ";
                else
                    cout << " | Total Weight = " << weight << endl;
            }
        }
        else
        {
            for (int i = 0; i < node_count; i++)
            {
                int edge_weight = adjacency_matrix[get_node_index(start)][i];
                if (edge_weight != inf - 1 && edge_weight != inf && !visited[i]) // if edge exists and not visited
                {
                    weight += edge_weight;
                    print_all_paths(node_name[i], end);
                    weight -= edge_weight; // backtracking
                }
            }
        }

        visited[get_node_index(start)] = 0; // Backtrack
        path_index--;
    }

    bool bfs(char start, char end, char *path, int &pathLength)
    {
        bool visited[256] = {false};
        char parent[256];

        Queue queue;
        queue.enqueue(start);
        visited[start] = true;
        parent[start] = '\0';

        int start_index = get_node_index(start);
        int end_index = get_node_index(end);

        while (!queue.isEmpty())
        {
            char node = queue.dequeue();

            if (node == end)
            {
                int length = 0;
                for (char cur = end; cur != '\0'; cur = parent[cur])
                {
                    path[length++] = cur;
                }
                path[length] = '\0';
                pathLength = length;
                return true;
            }

            int node_index = get_node_index(node);
            for (int i = 0; i < node_count; i++)
            {
                if (adjacency_matrix[node_index][i] != inf && !visited[i])
                {
                    visited[i] = true;
                    parent[i] = node;
                    queue.enqueue(node_name[i]);
                }
            }
        }

        return false;
    }
};

// aa

// aa
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

// aa
class CongestionManager
{
private:
    hash_map vehicleCounts;
    hash_map congestedRoads;

    // Helper function to create a road identifier string (e.g., "A-B")
    string createRoadKey(char from, char to)
    {
        if (from < to)
            return string(1, from) + "-" + string(1, to);
        else
            return string(1, to) + "-" + string(1, from);
    }

public:
    void monitorTraffic(Graph &g, char from, char to)
    {

        string roadKey = createRoadKey(from, to);

        int currentCount = vehicleCounts.get(roadKey);

        cout << "Before update: Road " << roadKey << " has " << currentCount << " vehicles." << endl;

        vehicleCounts.put(roadKey, currentCount + 1);

        cout << "After update: Road " << roadKey << " now has " << currentCount + 1 << " vehicles." << endl;

        int fromIndex = g.get_node_index(from);
        int toIndex = g.get_node_index(to);
        int roadWeight = g.get_adjacency_matrix()[fromIndex][toIndex];

        // Check if the road is congested
        if (currentCount + 1 >= roadWeight)
        {
            // agar congested to hash main put
            congestedRoads.put(roadKey, 1);
            cout << "Road " << roadKey << " is now [CONGESTED]" << endl;
        }
    }

    bool isPathCongested(Graph &g, char from, char to)
    {

        string roadKey = createRoadKey(from, to);

        monitorTraffic(g, from, to);

        if (congestedRoads.get(roadKey) == 1)
        {
            cout << "Path " << roadKey << " is congested." << endl;
            return true;
        }

        cout << "Path " << roadKey << " is not congested." << endl;
        return false;
    }

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

                         << endl;
                }
            }
        }
    }

    void Min_congested(Graph &g)
    {
        MinHeap minHeap;

        for (int i = 0; i < g.get_node_count(); i++)
        {
            for (int j = i + 1; j < g.get_node_count(); j++)
            {
                char from = g.get_node_name()[i];
                char to = g.get_node_name()[j];
                string roadKey = createRoadKey(from, to);

                int count = vehicleCounts.get(roadKey);
                int roadWeight = g.get_adjacency_matrix()[i][j];

                if (roadWeight != inf)
                {

                    minHeap.insert(i, j, count);
                }
            }
        }

        // extraction yahan pay ho rahi hay
        if (!minHeap.isEmpty())
        {
            MinHeap::Road leastCongested = minHeap.extractMin();
            char from = g.get_node_name()[leastCongested.startIndex];
            char to = g.get_node_name()[leastCongested.endIndex];
            int vehicleCount = leastCongested.vehicleCount;

            cout << "Most congested path: " << from << " to " << to
                 << " with " << vehicleCount << " vehicles." << endl;
        }
        else
        {
            cout << "No roads found!" << endl;
        }
    }

    bool AlternativePathing(Graph g, char start, char end)
    {
        if (isPathCongested(g, start, end))
        {
            cout << "Path between " << start << " and " << end << " is congested. Rerouting...\n";

            // Use BFS to find a new path
            char path[256];
            int pathLength = 0;

            if (g.bfs(start, end, path, pathLength))
            {
                cout << "New path found: ";
                for (int i = 0; i < pathLength; ++i)
                {
                    cout << path[i] << " ";
                }
                cout << "\n";
                return true;
            }
            else
            {
                cout << "No alternative path found.\n";
                return false;
            }
        }
        else
        {
            cout << "Path between " << start << " and " << end << " is clear.\n";
            return false;
        }
    }

    void alter(Graph &g)
    {
    }
};

// for movement to determine next pos
// zt
class snode
{
public:
    char data;
    snode *next;

    snode(char data)
    {
        this->data = data;
        next = NULL;
    }
};

// zt
class Stack
{
private:
    snode *top;

public:
    Stack()
    {
        top = NULL;
    }

    char peek()
    {
        return top->data;
    }

    void push(char value)
    {
        snode *nn = new snode(value);
        nn->next = top;
        top = nn;
    }

    void pop()
    {
        if (top == NULL)
        {
            cout << "empty!" << endl;
            return;
        }
        snode *temp = top;
        top = top->next;
        delete temp;
    }

    bool empty()
    {
        return top == NULL;
    }

    void display()
    {
        if (empty())
        {
            cout << "Stack is empty." << endl;
            return;
        }
        snode *temp = top;
        while (temp != NULL)
        {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    ~Stack()
    {
        while (!empty())
        {
            pop();
        }
    }
};

// az
class vehicle
{
public:
    string id;
    char start;
    char end;
    char current; // Stores the vehicle's current position
    string priority;
    int totaltime;

    vehicle() {}

    vehicle(string id, char start, char end, string priority = "low", int time = 0)
    {
        this->id = id;
        this->start = start;
        this->end = end;
        this->current = start;
        this->priority = priority;
        this->totaltime = time;
    }

    void move(Graph &g, CongestionManager &cm)
    {
        cout << "Vehicle " << id << " is moving from " << start << " to " << end << endl;

        char *path = g.dijkstra(current, end);

        if (path == nullptr)
        {
            cout << "No path found!" << endl;
            return;
        }

        int pathIndex = 0;
        while (path[pathIndex] != '\0' && path[pathIndex] != current)
        {
            pathIndex++;
        }

        // Check if we can move to the next node
        if (path[pathIndex + 1] != '\0')
        {
            char nextNode = path[pathIndex + 1];

            // Check if the path to the next node is congested
            if (cm.isPathCongested(g, current, nextNode))
            {
                cout << "Path from " << current << " to " << nextNode << " jalsa ho raha hay. Attempting to reroute..." << endl;

                // Attempt to find an alternative path
                if (cm.AlternativePathing(g, current, end))
                {
                    // Recalculate the path after rerouting
                    path = g.dijkstra(current, end);
                    if (path == nullptr)
                    {
                        cout << "No alternative roads found " << endl;
                        return;
                    }

                    nextNode = path[pathIndex + 1];
                }
                else
                {
                    cout << "Rerouting failed. Vehicle cannot move further." << endl;
                    return;
                }
            }

            cm.monitorTraffic(g, current, nextNode);

            current = nextNode;
            cout << "Vehicle " << id << " moved to " << current << endl;
        }
        else
        {
            cout << "Vehicle " << id << " has reached its destination " << end << endl;
        }
    }

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

        ifstream file2("emergency_vehicles.csv");
        if (!file2.is_open())
        {
            cout << "File not found" << endl;
            return;
        }

        string line2;
        getline(file2, line2); // Skip the header

        while (getline(file2, line2))
        {
            stringstream ss(line2);
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
        file2.close();

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

// az
class emergency_vehicle
{
public:
    string id;
    char start;
    char end;
    string priority;
    int time;

    emergency_vehicle()
    {
        Graph g;
        load_data(g);
    }

    emergency_vehicle(string id, char start, char end, string priority, int t = 0)
    {
        this->id = id;
        this->start = start;
        this->end = end;
        this->priority = priority;
        this->time = t;
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

// az--max heap
class priority_queue : public vehicle, emergency_vehicle // for traffic managment,max heap
{
public:
    vehicle heap[max_cars];
    int size;

    int priority_level(string &str)
    {
        if (str == "High")
            return 3;
        else if (str == "Medium")
            return 2;
        else
            return 1; // in case of low
    }

    void insert(int index)
    {
        while (index > 0)
        {
            int parent = (index - 1) / 2; // to find parent
            if (priority_level(heap[index].priority) > priority_level(heap[parent].priority))
            {
                swap(heap[index], heap[parent]);
                index = parent;
            }
            else
            {
                break;
            }
        }
    }
    void delete_vehicle(int index)
    {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        if (left < size && priority_level(heap[left].priority) > priority_level(heap[largest].priority))
            largest = left;
        if (right < size && priority_level(heap[right].priority) > priority_level(heap[largest].priority))
            largest = right;
        if (largest != index)
        {
            swap(heap[index], heap[largest]);
            delete_vehicle(largest);
        }
    }

public:
    priority_queue()
    { // insertings cars into heap, max heap
        ifstream file("vehicles.csv");
        string line;
        getline(file, line);
        while (getline(file, line))
        {
            stringstream ss(line);
            string start, end, id;
            getline(ss, id, ',');
            getline(ss, start, ',');
            getline(ss, end, ',');
            vehicle v(id, start[0], end[0]);
            insert_vehicle(v);
        }
        file.close();
        ifstream file2("emergency_vehicles.csv");
        string line2;
        getline(file2, line2);
        while (getline(file2, line2))
        {
            stringstream ss(line2);
            string start, end, id, p;
            getline(ss, id, ',');
            getline(ss, start, ',');
            getline(ss, end, ',');
            getline(ss, p, ',');
            vehicle v(id, start[0], end[0], p);
            insert_vehicle(v);
        }
        file2.close();
    }

    void insert_vehicle(vehicle &v)
    {
        if (size == max_cars)
        {
            cout << "Queue is full" << endl;
            return;
        }
        heap[size] = v; // value assign
        insert(size);   // arrange the value
        size++;
    }

    vehicle get_vehicle()
    {
        if (size == 0)
        {
            cout << "Queue is empty" << endl;
            return vehicle();
        }
        vehicle temp = heap[0];
        heap[0] = heap[size - 1]; // root ki jaga last element ko dal dia
        size--;
        delete_vehicle(0);
        return temp;
    }

    bool is_empty()
    {
        return size == 0;
    }
};

void simulate_signals(Graph &graph, int signal_count)
{
    priority_queue q;
    Signals s;
    // no.of vihicles = 30+18 = 48
    // signal_count = no.of nodes = 26
    while (!q.is_empty())
    {
        graph.update_signals();
        vehicle car = q.get_vehicle();
        std::cout << "Processing Vehicle: " << car.id << " (Priority: " << car.priority << ")\n";
        int travel_time = graph.get_travel_time(car.start, car.end);
        // particular nodes kay between ka travel time mil gaya
        if (travel_time == inf)
        {
            std::cout << "Invalid route\n";
            continue;
        }
        bool signal_state = true; // now checking the signal state,startu=ing of node
        for (int i = 0; i < signal_count; i++)
        {
            if (s.signals[i].node == car.start)
            {
                signal_state = s.signals[i].get_signal_state();
                if (!signal_state)
                {
                    std::cout << "Signal is red. Waiting for signal to turn green\n";
                    signal_state = false;
                    break;
                }
            }
        }
        if (!signal_state)
        {
            car.totaltime += 5; // delay add ho gya
        }
        car.totaltime += travel_time;
        std::cout << "Vehicle " << car.id << " has reached " << car.end << " in " << car.totaltime << " mins\n";
    }
}

// congestion implementation
// zt
class hnode
{
public:
    string key; // -> start, end which is basically the road
    int num;    // -> num of cars
    hnode *next;

    hnode()
    {
        num = 0;
        key = "";
        next = NULL;
    }

    hnode(int num, string key)
    {
        this->key = key;
        this->num = num;
        next = NULL;
    }
};

// zt
class list
{
public:
    hnode *head;

    list()
    {
        head = NULL;
    }

    void insert(string key, int num)
    {
        hnode *nn = new hnode(num, key);

        if (head == NULL)
        {
            head = nn;
        }
        else
        {
            hnode *temp = head;
            while (temp->next != NULL)
            {
                if (temp->key == key)
                {
                    temp->num += num;
                    return;
                }
                temp = temp->next;
            }
            if (temp->key == key)
            {
                temp->num += num;
            }
            else
            {
                temp->next = nn;
            }
        }
    }

    // Search function
    string find(string key)
    {
        hnode *temp = head;
        while (temp != NULL)
        {
            if (temp->key == key)
            {
                return "Road " + key + " has " + to_string(temp->num) + " vehicles.";
            }
            temp = temp->next;
        }
        return "";
    }

    // modified deletion code, we basiclly rmv car from road i.e. dcrement num by 1
    void del(string key)
    {
        hnode *temp = head;
        while (temp != NULL)
        {
            if (temp->key == key)
            {
                if (temp->num > 0)
                {
                    temp->num--;
                    return;
                }
                return;
            }
            temp = temp->next;
        }
    }
};

int main()
{
    Graph g;
    vehicle v;
    // hash_table cmap;
    CongestionManager cm;
    VehicleManager vm;
    Signals s;
    vm.load_data(g, cm);
    int choice;
    time_t old_time = time(0);

    while (true)
    {
        time_t current_time = time(0);
        long int elapsed_time = current_time - old_time;
        if (elapsed_time >= 15)
        {
            do
            {

                cout << "------ Simulation Dashboard ------" << endl;
                cout << "1. Display City Traffic Network" << endl;
                cout << "2. Display Traffic Signal Status" << endl;
                cout << "3. Display Congestion Status" << endl;
                cout << "4. Display Blocked Roads" << endl;
                cout << "5. Handle Emergency Vehicle Routing" << endl;
                cout << "6. Block Road due to Accident" << endl;
                cout << "7. Simulate Vehicle Routing" << endl;
                cout << "8. Exit Simulation" << endl;
                cout << "----------------------------------" << endl;
                cout << "Enter your choice: ";
                cin >> choice;
                // Handle menu options
                switch (choice)
                {
                case 1:
                    g.print_graph();
                    break;
                case 2:
                    s.display();
                    s.update_signals();
                    s.display();
                    break;
                case 3:
                    cm.displayCongestionLevels(g);
                    cm.Min_congested(g);
                    // cmap.del("S to V");
                    // cout << "\nCongestion levels after car deletion:" << endl;
                    // cmap.print(); //

                    break;
                case 4:
                    g.display_blocked_roads();
                    break;
                case 5:

                    break;
                case 6:
                    char start, end;
                    cout << "Enter the road to block (start, end): ";
                    cin >> start >> end;
                    g.block_road(start, end);
                    g.blockedroad_to_csv(start, end);
                    cout << "UPDATED:" << endl;
                    g.display_blocked_roads();
                    break;
                case 7:
                    cout << "--------ALL POSSIBLE PATHS--------" << endl;
                    g.print_all_paths('A', 'F');
                    vm.move_vehicles(g, cm);
                    break;
                case 8:
                    cout << "\nbyebye :p" << endl;
                    break;
                default:
                    cout << "\nInvalid choice. Please select a valid option (1-8)." << endl;
                }

            } while (choice != 8);
            old_time = current_time;
        }
    }

    return 0;
}