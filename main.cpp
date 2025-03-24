#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <random>

using namespace std;

// Base Animal Class
class Animal {
protected:
    string name;
    int age;
    string species;
    string color;
    string gender;
    int weight;
    string origin;
public:
    Animal(string sp, int a, string c, string g, int w, string o)
        : species(sp), age(a), color(c), gender(g), weight(w), origin(o) {}
    virtual void display() const {
        cout << species << " - " << name << ", " << age << " years, " << gender
             << ", " << color << ", " << weight << " lbs, from " << origin << endl;
    }
    void setName(string n) { name = n; }
    string getSpecies() const { return species; }
    string getName() const { return name; }
    int getAge() const { return age; }
};

// Derived Classes
class Hyena : public Animal {
public:
    Hyena(int a, string c, string g, int w, string o)
        : Animal("Hyena", a, c, g, w, o) {}
};

class Lion : public Animal {
public:
    Lion(int a, string c, string g, int w, string o)
        : Animal("Lion", a, c, g, w, o) {}
};

class Tiger : public Animal {
public:
    Tiger(int a, string c, string g, int w, string o)
        : Animal("Tiger", a, c, g, w, o) {}
};

class Bear : public Animal {
public:
    Bear(int a, string c, string g, int w, string o)
        : Animal("Bear", a, c, g, w, o) {}
};

// Function to randomly assign names from name list
string assignName(vector<string>& names) {
    if (!names.empty()) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distr(0, names.size() - 1);
        int index = distr(gen);
        string chosenName = names[index];
        names.erase(names.begin() + index); // Remove used name
        return chosenName;
    }
    return "Unnamed";
}

int main() {
    ifstream inputFile("/Users/decentbean795/Documents/XCode/CIT-66/Module_4/arrivingAnimals.txt");
    ifstream namesFile("/Users/decentbean795/Documents/XCode/CIT-66/Module_4/animalNames.txt");
    ofstream outputFile("/Users/decentbean795/Documents/XCode/CIT-66/Module_4/newAnimals.txt");
    
    if (!inputFile || !namesFile || !outputFile) {
        cerr << "Error opening files." << endl;
        return 1;
    }
    
    // Load animal names
    map<string, vector<string>> nameLists;
    string line, category;
    while (getline(namesFile, line)) {
        if (line.find("Names:") != string::npos) {
            category = line.substr(0, line.find(" Names:")); // Extract only the species name
            nameLists[category] = {}; // Initialize
        } else if (!line.empty()) {
            nameLists[category].push_back(line);
        }
    }
    
    vector<Animal*> animals;
    map<string, int> speciesCount;
    
    // Read input and create animal objects
    while (getline(inputFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        int age, weight;
        string gender, species, color, origin, junk;

        ss >> age >> junk >> junk >> gender >> species;
        species = species.substr(0, species.find(','));
        getline(ss, junk, ',');
        getline(ss, color, ',');
        ss >> weight;
        getline(ss, junk, ',');
        getline(ss, origin);

        if (age <= 0 || weight <= 0 || species.empty()) {
            cerr << "Invalid data, skipping: " << line << endl;
            continue;
        }

        string speciesKey = species;
        speciesKey[0] = toupper(speciesKey[0]);
        for (size_t i = 1; i < speciesKey.length(); i++) {
            speciesKey[i] = tolower(speciesKey[i]);
        }

        Animal* animal = nullptr;
        if (species == "hyena") animal = new Hyena(age, color, gender, weight, origin);
        else if (species == "lion") animal = new Lion(age, color, gender, weight, origin);
        else if (species == "tiger") animal = new Tiger(age, color, gender, weight, origin);
        else if (species == "bear") animal = new Bear(age, color, gender, weight, origin);
        
        if (animal) {
            if (nameLists.find(speciesKey) != nameLists.end() && !nameLists[speciesKey].empty()) {
                animal->setName(assignName(nameLists[speciesKey]));
            } else {
                animal->setName("Unnamed");
            }
            animals.push_back(animal);
            speciesCount[speciesKey]++;
        }
    }
    
    // Write output
    for (const auto& [speciesKey, count] : speciesCount) {
        outputFile << speciesKey << " (Total: " << count << ")\n";
        for (auto animal : animals) {
            if (animal->getSpecies() == speciesKey) {
                outputFile << "Name: " << animal->getName() << ", Age: " << animal->getAge() << "\n";
            }
        }
        outputFile << "\n";
    }
    
    // Cleanup
    for (auto animal : animals) delete animal;
    
    cout << "Processing complete. Check newAnimals.txt for the report." << endl;
    return 0;
}
