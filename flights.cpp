#include <iostream>
#include <string>
#include <fstream>   //read from files
#include <vector>    //vector class
#include <algorithm> //tolower
#include <queue>
#include <list>
using namespace std;

void printPath(int parents[], int size, int startv, int endv, vector<string> lookup)
{
    if (endv != startv)
    {
        printPath(parents, size, startv, parents[endv], lookup);
        cout << "--> ";
    }
    cout << lookup[endv] << " ";
}
void bfs(vector<int> alists[], int size, int start, int target, vector<string> lookup)
{
    int *parents = new int[size];
    for (int i = 0; i < size; i++)
        parents[i] = -1;
    parents[start] = start;
    queue<int> q;
    q.push(start);
    bool found = false;
    while (!q.empty() && !found)
    {
        int v = q.front();
        q.pop();
        if (v == target)
            found = true;

        else
            for (int i = 0; i < alists[v].size(); i++)
            {
                int w = alists[v][i];
                if (parents[w] == -1)
                {
                    parents[w] = v;
                    q.push(w);
                }
            }
    }
    if (found)
        printPath(parents, size, start, target, lookup);
    else
        cout << "Not found";
    cout << endl;
    delete[] parents;
}

int main()
{
    string input;
    ifstream myFile;
    cout << "Input file name: ";
    getline(cin, input);
    myFile.open(input);
    while (myFile.fail())
    {
        cout << "Invalid file name. " << endl;
        cout << "Input file name: ";
        getline(cin, input);
        myFile.open(input);
    }
    cout << "File opened successfully. " << endl << endl;



    ofstream outFile;
    vector<string> lookupTable;
    int numCities = 0;
    string line;

    outFile.open("cities.txt");
    while (!myFile.eof()) // creates lookup table with no dupes
    {
        bool dupe = false;
        getline(myFile, line);
        if (line.length() <= 7)
            break;
        string city = line.substr(7);

        for (int i = 0; i < lookupTable.size(); i++)
        {
            if (city == lookupTable.at(i))
            {
                dupe = true;
                break;
            }
        }

        if (!dupe)
        {
            lookupTable.push_back(city);
            outFile << city << endl;
            numCities++;
        }
    }
    outFile.close();
    myFile.close();
    myFile.open(input);
    // generate alists array
    vector<int> alists[numCities];
    int indexFromCity;
    int indexToCity = 0;
    while (!myFile.eof()) // connections.txt
    {
        for (int i = 0; getline(myFile, line);)
        {
            if (line.substr(0, 4) == "From")
            {
                string fromCity = line.substr(7);            // Oakland
                for (int j = 0; j < lookupTable.size(); j++) // Set the index of fromCity
                {
                    if (lookupTable.at(j) == fromCity)
                    {
                        indexFromCity = j;
                        break;
                    }
                }

            }
            else //To or " "
            {
                string toCity = line.substr(7);
                for (int j = 0; j < lookupTable.size(); j++)
                {
                    if (lookupTable.at(j) == toCity)
                    {
                        indexToCity = j;
                        break;
                    }
                }
                alists[indexFromCity].push_back(indexToCity);
            }
        }

        myFile.close();

        bool finished = false;
        while (!finished)
        {
            cout << "Please enter a departing city name or \"quit\" to quit: ";
            getline(cin, line);
            if (line == "quit")
                return 1;
            while (line.length() < 2)
            {
                cout << "City has to be 2 characters or more. " << endl;
                cout << "Please enter a departing city name or \"quit\" to quit: ";
                getline(cin, line);
                if (line == "quit")
                    return 1;
            }

            transform(line.begin(), line.end(), line.begin(), ::tolower);

            // Enter in SA: display list
            vector<int> departingCities;
            for (int i = 0; i < lookupTable.size(); i++)
            {
                string currentCity = lookupTable.at(i);
                transform(currentCity.begin(), currentCity.end(), currentCity.begin(), ::tolower);
                int found = currentCity.find(line);
                if (found != string::npos)
                    departingCities.push_back(i);
            }

            int departingIndex;

            if (departingCities.size() == 1)
            {
                cout << "Selected departure: " << departingCities.at(0) << ": " << lookupTable.at(departingCities.at(0)) << endl;
                departingIndex = departingCities.at(0);
            }
            else
            {
                int count = 0;
                for (int i = 0; i < departingCities.size(); i++)
                {
                    cout << count + 1 << ": " << lookupTable.at(departingCities.at(i)) << endl;
                    count++;
                }
                cout << "\n"
                     << "Select a departing city by entering a number from the list above: ";
                int departure;
                cin >> departure;
                departingIndex = departingCities.at(departure - 1);
                cout << "\n"
                     << "Selected departure: " << departingIndex << ": " << lookupTable.at(departingCities.at(departure - 1)) << endl;
                cin.ignore();
            }

            int destinationIndex = -1;
            bool exists = false;

            cout << "Enter a destination city or \"quit\" to exit: ";
            getline(cin, line);
            if (line == "quit")
                return 1;
            while (!exists)
            {
                if (line == "quit")
                    return 1;
                transform(line.begin(), line.end(), line.begin(), ::tolower);
                for (int i = 0; i < lookupTable.size(); i++)
                {
                    string currentCity = lookupTable.at(i);
                    transform(currentCity.begin(), currentCity.end(), currentCity.begin(), ::tolower);
                    int found = currentCity.find(line);
                    if (found != string::npos) // it found a match
                    {
                        destinationIndex = i;
                        exists = true;
                        break;
                    }
                }
                if (destinationIndex == -1)
                {
                    cout << "City doesn't exist. " << endl;
                    cout << "Enter a destination city or \"quit\" to exit: ";
                    getline(cin, line);
                }
            }

            cout << destinationIndex << endl;

            cout << "Shortest route: ";
            bfs(alists, numCities, departingIndex, destinationIndex, lookupTable);

            cout << "Would you like to go again? (\"yes\" or \"no\"): ";
            getline(cin, line);
            if (line == "no")
                finished = true;
        }
    }
}