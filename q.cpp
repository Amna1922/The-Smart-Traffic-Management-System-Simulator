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

// priority queue node
class pnode
{
public:
    char index;
    int priority;
    pnode *next;
    pnode(char index, int priority)
    {
        this->index = index;
        this->priority = priority;
    }
};

// priority queue for Asar algo for the emergency vehicles
class epqueue
{
private:
    pnode *front;
    pnode *rear;

public:
    epqueue()
    {
        front = NULL;
        rear = NULL;
    }

    void push(char index, int priority)
    {
        pnode *nn = new pnode(index, priority);

        if (empty())
        {
            front = nn;
            rear = nn;
            return;
        }

        if (nn->priority < front->priority)
        {
            nn->next = front;
            front = nn;
        }

        else
        {
            pnode *temp = front;
            while (temp->next && temp->next->priority <= priority)
            {
                temp = temp->next;
            }
            nn->next = temp->next;
            temp->next = nn;

            if (!temp->next)
            {
                rear = nn;
            }
        }
    }

    char pop()
    {
        if (empty())
        {
            return NULL;
        }

        pnode *temp = front;
        char index = front->index;
        front = front->next;

        if (empty())
        {
            rear = NULL;
        }

        delete temp;
        return index;
    }

    bool empty()
    {
        if (front == NULL)
        {
            return true;
        }
        return false;
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

    void astar(char start, char end, bool check_signal)
    {
        static const int num = 26;
        bool visited[num];
        int cost[num];
        char parent[num];
        epqueue pq;
        int heuristic[num];
        int s = get_node_index(start);
        int e = get_node_index(end);
        int hcost;
        int ascore;
        char cur;
        // hearistic is total, ascore is from curr
        for (int i = 0; i < num; i++)
        {
            cost[i] = inf;
            parent[i] = '\0';
            visited[i] = false;

            // acc to signal open or close
            if (check_signal)
            {
                heuristic[i] = 0;
            }
            else
            {
                heuristic[i] = 5;
            }
        }

        cost[s] = 0;
        pq.push(start, heuristic[s]);

        while (!pq.empty())
        {
            cur = pq.pop();
            int i = get_node_index(cur);

            if (cur == end)
            {
                break;
            }

            visited[i] = true;

            for (int n = 0; n < num; n++)
            {
                if (adjacency_matrix[i][n] != 0 && !visited[n])
                {
                    hcost = cost[i] + adjacency_matrix[i][n];
                    if (hcost < cost[n])
                    {
                        cost[n] = hcost;
                        parent[n] = cur;
                        ascore = cost[n] + heuristic[n];
                        pq.push(65 + n, ascore);
                        // as we hv no getnode func so I used ascii val :(
                    }
                }
            }
        }

        if (cost[e] == inf)
        {
            cout << "No path found." << endl;
            return;
        }
        if (cost[e] == inf - 1)
        {
            cout << "Path blocked." << endl;
            return;
        }

        cout << "A* path cost: " << cost[e] << endl;
        cout << "A* path: ";
        char temp = end;
        while (temp != '\0')
        {
            cout << temp << " ";
            temp = parent[temp - 'A'];
        }
        cout << endl;
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
class minheap
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

    void heapify(int index)
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
            heapify(smallest);
        }
    }

public:
    minheap() : size(0) {}

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
        heapify(0);
        return min;
    }

    bool isEmpty()
    {
        return size == 0;
    }
};

// aa
class congestion_manager
{
private:
    hash_map vehicle_counts;
    hash_map congested_roads;

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

        int currentCount = vehicle_counts.get(roadKey);

        cout << "Before update: Road " << roadKey << " has " << currentCount << " vehicles." << endl;

        vehicle_counts.put(roadKey, currentCount + 1);

        cout << "After update: Road " << roadKey << " now has " << currentCount + 1 << " vehicles." << endl;

        int fromIndex = g.get_node_index(from);
        int toIndex = g.get_node_index(to);
        int roadWeight = g.get_adjacency_matrix()[fromIndex][toIndex];

        // Check if the road is congested
        if (currentCount + 1 >= roadWeight)
        {
            // agar congested to hash main put
            congested_roads.put(roadKey, 1);
            cout << "Road " << roadKey << " is now [CONGESTED]" << endl;
        }
    }

    bool isPathCongested(Graph &g, char from, char to)
    {

        string roadKey = createRoadKey(from, to);

        monitorTraffic(g, from, to);

        if (congested_roads.get(roadKey) == 1)
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

                int count = vehicle_counts.get(roadKey);
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
        minheap minHeap;

        for (int i = 0; i < g.get_node_count(); i++)
        {
            for (int j = i + 1; j < g.get_node_count(); j++)
            {
                char from = g.get_node_name()[i];
                char to = g.get_node_name()[j];
                string roadKey = createRoadKey(from, to);

                int count = vehicle_counts.get(roadKey);
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
            minheap::Road leastCongested = minHeap.extractMin();
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
    int totaltime; // the overall time spent on a road by a vehicle

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

    void move(Graph &g, congestion_manager &cm)
    {
        cout << "Vehicle " << id << " is moving from " << start << " to " << end << endl;

        char *path = g.dijkstra(current, end);

        if (path == NULL)
        {
            cout << "no path. vehicle will fall in khayi :(" << endl;
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
            char next = path[pathIndex + 1];
            // Check if the path to the next node is congested
            if (cm.isPathCongested(g, current, next))
            {
                cout << "Path from " << current << " to " << next << " jalsa ho raha hay. Finding new route." << endl;

                // rerouting
                if (cm.AlternativePathing(g, current, end))
                {
                    // Recalculate the path after rerouting
                    path = g.dijkstra(current, end);
                    if (path == nullptr)
                    {
                        cout << "No alternative roads found " << endl;
                        return;
                    }
                    next = path[pathIndex + 1];
                }
                else
                {
                    cout << "Rerouting failed. Vehicle cannot move further." << endl;
                    return;
                }
            }

            cm.monitorTraffic(g, current, next);

            current = next;
            cout << "Vehicle " << id << " moved to " << current << endl;
        }
        else
        {
            cout << "Vehicle " << id << " has reached its destination " << end << endl;
        }
    }

    void load_data(Graph &g, congestion_manager &cm)
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

            vehicle v(id, start[0], end[0], priority);
            v.move(g, cm); // Move the vehicle and update congestion
        }
        file.close();
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
class VehicleManager
{
private:
    static const int MAX_VEHICLES = 100;
    vehicle vehicles[MAX_VEHICLES];
    int vehicleCount;

public:
    VehicleManager() : vehicleCount(0) {}

    void load_data(Graph &g, congestion_manager &cm)
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

    void move_vehicles(Graph &g, congestion_manager &cm)
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

int main()
{
    Graph g;
    vehicle v;
    // hash_table cmap;
    congestion_manager cm;
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
                cout << "8. All possible paths" << endl;
                cout << "9. Exit Simulation" << endl;
                cout << "----------------------------------" << endl;
                cout << "Enter your choice: ";
                cin >> choice;
                if (cin.fail() || choice < 1 || choice > 9)
                {
                    cin.clear();                                         // Clear the error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "Invalid choice. Please enter a number between 1 and 9.\n";
                    continue;
                }
                vm.move_vehicles(g, cm);

                switch (choice)
                {
                case 1:
                    g.print_graph();
                    break;
                case 2:
                    cout << "--------SIGNALS--------" << endl;
                    s.display();
                    s.update_signals();
                    break;
                case 3:
                    cm.displayCongestionLevels(g);
                    cm.Min_congested(g);

                    break;
                case 4:
                    g.display_blocked_roads();
                case 5:
                {
                    char start, end;
                    cout << "Enter the start and end intersections for emergency vehicles (start, end): ";
                    cin >> start >> end;

                    if (g.get_node_index(start) == -1 || g.get_node_index(end) == -1)
                    {
                        cout << "Invalid intersections. Please ensure both start and end are valid nodes.\n";
                        break;
                    }

                    g.astar(start, end, s.is_signal_open(end));
                }
                break;
                case 6:
                {
                    char start, end;
                    cout << "Enter the road to block (start, end): ";
                    cin >> start >> end;

                    if (g.get_node_index(start) == -1 || g.get_node_index(end) == -1)
                    {
                        cout << "Invalid intersections. Please ensure both start and end are valid nodes.\n";
                        break;
                    }

                    g.block_road(start, end);
                    g.blockedroad_to_csv(start, end);
                    cout << "UPDATED:" << endl;
                    g.display_blocked_roads();
                }
                break;
                case 7:
                    cout << "--------VEHICLE MOVEMENT--------" << endl;
                    vm.move_vehicles(g, cm);
                    break;
                case 8:
                    cout << "--------ALL POSSIBLE PATHS--------" << endl;
                    g.print_all_paths('A', 'F');
                    break;
                case 9:
                    cout << "\nbyebye :p" << endl;
                    exit(0);
                    break;
                default:
                    cout << "\nInvalid choice. Please select a valid option (1-8)." << endl;
                }

            } while (choice != 9);
            old_time = current_time;
        }
    }

    return 0;
}