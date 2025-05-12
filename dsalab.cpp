#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <climits>
#include <vector>
using namespace std;
struct Airport {
    string iata;
    double longitude;
    double latitude;
};

Airport findAirportByIataCode(const vector<Airport>& airports, const string& iataCode) {
    for (const auto& airport : airports) {
        if (airport.iata == iataCode) {
            return airport;
        }
    }
    return {"", 0.0, 0.0}; // Return an empty Airport object if not found
}

double safe_stod(const string& str) {
    try {
        return stod(str);
    } catch (const std::invalid_argument&) {
        return 0.0;
    } catch (const std::out_of_range&) {
        return 0.0;
    }
}

class Graph {
private:
    vector<string> airportNames;
    vector<float> latitudes;
    vector<float> longitudes;
    vector<vector<float>> adjMatrix;
    int numVertices;

    float toRadian(float degree) {
        return degree * M_PI / 180.0;
    }

    float calculateDistance(float lon1, float lon2, float lat1, float lat2) {
        const int EARTH_RADIUS = 6371; // Radius of Earth in km
        float deltaLat = toRadian(lat2 - lat1);
        float deltaLon = toRadian(lon2 - lon1);
        lat1 = toRadian(lat1);
        lat2 = toRadian(lat2);

        float a = sin(deltaLat / 2) * sin(deltaLat / 2) +
                  cos(lat1) * cos(lat2) * sin(deltaLon / 2) * sin(deltaLon / 2);
        float c = 2 * atan2(sqrt(a), sqrt(1 - a));
        return EARTH_RADIUS * c;
    }

public:
    Graph(int vertices, const vector<string>& names, const vector<float>& lats, const vector<float>& longs)
        : numVertices(vertices), airportNames(names), latitudes(lats), longitudes(longs) {
        adjMatrix.resize(numVertices, vector<float>(numVertices, -1.0));
    }

    void addEdge(int i, int j) {
        if (i != j) {
            adjMatrix[i][j] = calculateDistance(longitudes[i], longitudes[j], latitudes[i], latitudes[j]);
            adjMatrix[j][i] = adjMatrix[i][j];
        }
    }

    void displayMatrix() {
        cout << "Adjacency Matrix (Distances in km):\n";
        for (const auto& row : adjMatrix) {
            for (float dist : row) {
                if (dist == -1) {
                    cout << "INF\t";
                } else {
                    cout << dist << "\t";
                }
            }
            cout << endl;
        }
    }

    void primMST() {
        vector<float> key(numVertices, INT_MAX);
        vector<bool> mstSet(numVertices, false);
        vector<int> parent(numVertices, -1);
        key[0] = 0;

        for (int count = 0; count < numVertices - 1; ++count) {
            float min = INT_MAX;
            int u = -1;

            for (int v = 0; v < numVertices; ++v) {
                if (!mstSet[v] && key[v] < min) {
                    min = key[v];
                    u = v;
                }
            }

            mstSet[u] = true;

            for (int v = 0; v < numVertices; ++v) {
                if (adjMatrix[u][v] != -1 && !mstSet[v] && adjMatrix[u][v] < key[v]) {
                    parent[v] = u;
                    key[v] = adjMatrix[u][v];
                }
            }
        }

        cout << "Minimum Spanning Tree:\n";
        cout << "Edge\tWeight (km)\n";
        for (int i = 1; i < numVertices; ++i) {
            cout << airportNames[parent[i]] << " - " << airportNames[i] << "\t" << adjMatrix[i][parent[i]] << " km\n";
        }
    }
};
struct Passenger {
    string name;
    int id;
    string seatNumber;
    Passenger* next;

    Passenger(string n, int i, string s) {
    	name = n ;
    	id = i ;
    	seatNumber = s;
    	next = NULL;
	}
};
class PassengerQueue {
private:
    Passenger* front;
    Passenger* rear; 

public:
    PassengerQueue() {
    	front = NULL;
    	rear = NULL;
    	
	}

   
    void enqueue(string name, int id, string seatNumber) {
    	
        Passenger* newPassenger = new Passenger(name, id, seatNumber);

        if (rear == NULL) { 
            front = rear = newPassenger;
        } else { 
            rear->next = newPassenger;
            rear = newPassenger;
        }

        cout << "Passenger " << name << " with ID " << id << " added to the queue.\n";
    }

  
    void dequeue() {
        if (front == NULL) { 
            cout << "No passengers in the queue." << endl;
            return;
        }

        Passenger* temp = front;
        
        
        cout << "Passenger " << front->name << " with ID " << front->id << " boarded." << endl;
        front = front->next;

        if (front == NULL) { 
            rear = NULL;
        }

        delete temp;
    }

  
    void displayQueue() {
        if (front == NULL) {
            cout << "The queue is empty." << endl;
            
            return;
        }

        Passenger* current = front;
        cout << "Passengers in the queue:  " << endl;
        while (current != NULL) {
            cout << "Name: " << current->name
                 << ", ID: " << current->id
                 << ", Seat: " << current->seatNumber << "\n";
            current = current->next;
        }
    }


    ~PassengerQueue() {
        while (front != NULL) {
            Passenger* temp = front;
            front = front->next;
            delete temp;
        }
    }
};
class Flight {
public:
    string flightNumber;
    string source;
    string destination;
    string arrivalTime;
    string departureTime;
    string gate;

    // Constructor
    Flight(string fn, string src, string dest, string arr, string dep, string g)
        : flightNumber(fn), source(src), destination(dest), arrivalTime(arr), departureTime(dep), gate(g) {}

    // Display flight details (marked as const)
    void displayFlight() const {
        cout << "Flight Number: " << flightNumber
             << ", Source: " << source
             << ", Destination: " << destination
             << ", Arrival: " << arrivalTime
             << ", Departure: " << departureTime
             << ", Gate: " << gate << endl;
    }
};

// FlightScheduler class to manage flights
class FlightScheduler {
private:
    vector<Flight> flights;

public:
    // Add a flight to the schedule
    void addFlight(Flight flight) {
        flights.push_back(flight);
        cout << "Flight added successfully.\n";
    }

    // Remove a flight from the schedule
    void removeFlight(const string &flightNumber) {
        for (size_t i = 0; i < flights.size(); ++i) {
            if (flights[i].flightNumber == flightNumber) {
                flights.erase(flights.begin() + i);
                cout << "Flight removed successfully.\n";
                return;
            }
        }
        cout << "Flight not found.\n";
    }

    // Display all flights in the schedule
    void displaySchedule() const {
        if (flights.empty()) {
            cout << "No flights scheduled.\n";
            return;
        }
        cout << "\n--- Flight Schedule ---\n";
        for (size_t i = 0; i < flights.size(); ++i) {
            flights[i].displayFlight();
        }
    }

    // Search for a specific flight by flight number
    void searchFlight(const string &flightNumber) const {
        for (size_t i = 0; i < flights.size(); ++i) {
            if (flights[i].flightNumber == flightNumber) {
                cout << "Flight found:\n";
                flights[i].displayFlight();
                return;
            }
        }
        cout << "Flight not found.\n";
    }
};
int main()
{
	int a;
	cout<<"1. Passenger management system "<<endl;
	cout<<"2. Flight scheduling "<<endl;
	cout<<"3. Airport navigation "<<endl;	
	cout<<"Please enter your choice :";cin>>a;
	if (a==1)
    {
	PassengerQueue queue;
    int choice;

    do {
        cout << "\n--- Airlines Passenger Management System ---\n";
        cout << "1.   Add Passenger\n";
        cout << "2.   Remove Passenger (Boarding)\n";
        cout << "3.   Display Passengers\n";
        cout << "4.   Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string name, seatNumber;
            int id;
            cout << "Enter passenger name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter passenger ID: ";
            cin >> id;
            cout << "Enter seat number: ";
            cin.ignore();
            getline(cin, seatNumber);
            queue.enqueue(name, id, seatNumber);
            break;
        }
        case 2:
            queue.dequeue();
            break;
        case 3:
            queue.displayQueue();
            break;
        case 4:
            cout << "Exiting the system.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
	while (choice != 4);
    }
    if (a==2)
    {
    	FlightScheduler scheduler;
    int choice;
    do {
        cout << "\n--- Flight Scheduling System ---\n";
        cout << "1. Add Flight\n";
        cout << "2. Remove Flight\n";
        cout << "3. Display Schedule\n";
        cout << "4. Search Flight\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string fn, src, dest, arr, dep, gate;
                cout << "Enter Flight Number: ";
                cin >> fn;
                cout << "Enter Source: ";
                cin >> src;
                cout << "Enter Destination: ";
                cin >> dest;
                cout << "Enter Arrival Time: ";
                cin >> arr;
                cout << "Enter Departure Time: ";
                cin >> dep;
                cout << "Enter Gate: ";
                cin >> gate;
                scheduler.addFlight(Flight(fn, src, dest, arr, dep, gate));
                break;
            }
            case 2: {
                string fn;
                cout << "Enter Flight Number to Remove: ";
                cin >> fn;
                scheduler.removeFlight(fn);
                break;
            }
            case 3:
                scheduler.displaySchedule();
                break;
            case 4: {
                string fn;
                cout << "Enter Flight Number to Search: ";
                cin >> fn;
                scheduler.searchFlight(fn);
                break;
            }
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);
	}
	if (a==3)
	{
		vector<Airport> airportList;

    ifstream inputFile("C:\\Users\\Llewlyn\\Desktop\\projects\\dsa-lab\\airports.dat.txt");
    if (!inputFile.is_open()) {
        cout << "Error: Unable to open file!" << endl;
        return 1;
    }
    while (inputFile.good()) {
        string id, name, city, country, iata, icao, latitudeStr, longitudeStr, elevationStr, timezoneOffsetStr, timezoneRegion, type, source;
        getline(inputFile, id, ',');
        getline(inputFile, name, ',');
        getline(inputFile, city, ',');
        getline(inputFile, country, ',');
        getline(inputFile, iata, ',');
        getline(inputFile, icao, ',');
        getline(inputFile, latitudeStr, ',');
        getline(inputFile, longitudeStr, ',');
        getline(inputFile, elevationStr, ',');
        getline(inputFile, timezoneOffsetStr, ',');
        getline(inputFile, timezoneRegion, ',');
        getline(inputFile, type, ',');
        getline(inputFile, source);

        double longitude = safe_stod(longitudeStr);
        double latitude = safe_stod(latitudeStr);

        airportList.push_back({iata, longitude, latitude});
    }

    int numAirports;
    cout << "Enter the number of airports for the MST: ";
    cin >> numAirports;

    vector<string> selectedIATA(numAirports);
    vector<float> latitudes(numAirports), longitudes(numAirports);

    for (int i = 0; i < numAirports; ++i) {
        cout << "Enter IATA code for airport " << i + 1 << ": ";
        cin >> selectedIATA[i];

        Airport airport = findAirportByIataCode(airportList, selectedIATA[i]);
        if (airport.iata.empty()) {
            cout << "Error: IATA code not found: " << selectedIATA[i] << endl;
            return 1;
        }

        latitudes[i] = airport.latitude;
        longitudes[i] = airport.longitude;
    }

    Graph graph(numAirports, selectedIATA, latitudes, longitudes);

    for (int i = 0; i < numAirports; ++i) {
        for (int j = 0; j < numAirports; ++j) {
            graph.addEdge(i, j);
        }
    }
    graph.primMST();
	}
}
