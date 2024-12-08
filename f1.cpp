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
#include <ctime>
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
		load_road_closures();
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
		// bool first = true;
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

	string dijkstra(char start, char end)
	{
		int *distance = new int[node_count];
		bool *visited = new bool[node_count];
		int *parent = new int[node_count];		 // to store prev
		int start_index = get_node_index(start); // to store 1st
		int cur_node;							 // to store curr -> to make path
		int end_index = get_node_index(end);
		string path = "";

		// check if valid node
		if (start_index == -1 || end_index == -1)
		{
			cout << "Invalid node" << endl;
			delete[] distance;
			delete[] visited;
			delete[] parent;
			return "";
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
				// inf -1 => to check blocked roads, inf => to check no road
				if (!visited[j] && adjacency_matrix[min_index][j] != inf && adjacency_matrix[min_index][j] != inf - 1 && distance[min_index] + adjacency_matrix[min_index][j] < distance[j])
				{
					distance[j] = distance[min_index] + adjacency_matrix[min_index][j];
					parent[j] = min_index;
				}
			}

			if (min_index == end_index)
			{

				for (cur_node = end_index; cur_node != start_index; cur_node = parent[cur_node])
				{
					path = node_name[cur_node] + path;
				}

				path = node_name[start_index] + path;
				break;
			}
		}

		if (distance[end_index] == inf)
		{
			cout << "No path exists from " << start << " to " << end << endl;
			delete[] distance;
			delete[] visited;
			delete[] parent;
			return "";
		}

		cout << "Shortest path from " << start << " to " << end << " is: " << path << endl;
		cout << "Total distance: " << distance[end_index] << endl;

		delete[] distance;
		delete[] visited;
		delete[] parent;

		return path;
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
};

// for movement to determine next pos
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
class vehicle
{
public:
	string id;
	char start;
	char end;
	string priority;
	Graph g;
	Stack stk;
	int count;
	vehicle()
	{
		count = 0;
		load_data(g);
	}

	vehicle(string id, char start, char end, string priority = "low")
	{
		this->id = id;
		this->start = start;
		this->end = end;
		this->priority = priority;
		cout << "Vehicle " << id << " is moving from " << start << " to " << end << endl;
		string str = g.dijkstra(start, end);
		cout << endl;
		for (int i = str.length() - 1; i >= 0; i--)
		{
			stk.push(str[i]);
		}
	}

	void move(Graph &g)
	{
		if (start == end)
		{
			cout << "Vehicle " << id << " has already reached its destination." << endl;
			return;
		}

		if (!stk.empty())
		{
			start = stk.peek();
			cout << "Vehicle " << id << " is moving from " << start << " to " << end << endl;
			stk.pop();
		}
		else
		{
			cout << "Vehicle " << id << " has already reached its destination." << endl;
		}

		if (start == end)
		{
			cout << "Vehicle " << id << " has reached its destination. enjoy :) " << endl;
		}
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
			count++;
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

// congestion implementation
class hnode
{
public:
	string key; // -> start, end which is basically the road
	int num;	// -> num of cars
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

// Hash table implementation
class hash_table
{
private:
	static const int hashGroups = 42;
	list table[hashGroups];

public:
	hash_table()
	{
		load_vehicles_inhash("vehicles.csv");
	}
	// Hash function
	int hash_func(string key)
	{
		int hash = 0;
		for (int i = 0; i < key.length(); i++)
		{
			hash = (hash + key[i]) % hashGroups;
		}
		return hash;
	}

	// Insert function
	void insert(int num, string key)
	{
		int index = hash_func(key);
		table[index].insert(key, num);
	}

	// Search function
	string search(string key)
	{
		int index = hash_func(key);
		return table[index].find(key);
	}

	// decrement car count
	void del(string key)
	{
		int index = hash_func(key);
		table[index].del(key);
	}

	void print()
	{
		cout << "----------------------CONGESTION STATUS-------------------------" << endl;
		for (int i = 0; i < hashGroups; i++)
		{
			hnode *temp = table[i].head;
			if (temp != NULL)
			{
				while (temp != NULL)
				{
					cout << temp->key << " has " << temp->num << " vehicles." << endl;
					temp = temp->next;
				}
			}
		}
	}

	void load_vehicles_inhash(const string &fileName)
	{
		ifstream file(fileName);
		if (!file.is_open())
		{
			cout << "Error opening file!" << endl;
			return;
		}
		string line;
		getline(file, line); // Skip header

		while (getline(file, line))
		{
			stringstream ss(line);
			string vehicleID, start, end;
			getline(ss, vehicleID, ',');
			getline(ss, start, ',');
			getline(ss, end, ',');

			string str = start + " to " + end;
			insert(1, str);
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
	Graph g;
	vehicle v;
	hash_table cmap;
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

					break;
				case 3:
					cmap.print();
					cmap.del("S to V");
					cout << "\nCongestion levels after car deletion:" << endl;
					cmap.print();
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