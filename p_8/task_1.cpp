#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct Process {

    std::string name;

    int basePriority;
    int currentPriority;
    int burstTime; 

    Process(std::string n, int p, int t) 
        : name(n), basePriority(p), currentPriority(p), burstTime(t) {}
};

void printProcesses(const std::vector<Process>& processes) {

    std::cout << "Процес\tБаз. пріор.\tПоточ. пріор.\tЧас\n";

    for (const auto& p : processes) {

        std::cout << p.name << "\t" << p.basePriority << "\t\t" 
                  << p.currentPriority << "\t\t" << p.burstTime << "\n";
    }
    std::cout << "-------------------------------------------\n";
}

int main() {

    std::vector<Process> processes = {

        Process("P1", 1, 10),
        Process("P2", 2, 5),
        Process("P3", 3, 7)
    };

    std::cout << "Початковий стан:\n";
    printProcesses(processes);

    for (int cycle = 1; cycle <= 3; ++cycle) {

        std::cout << "Цикл планування №" << cycle << "\n";

        auto it = std::max_element(processes.begin(), processes.end(), 

            [](const Process& a, const Process& b) {
                return a.currentPriority < b.currentPriority;
            });

        std::cout << "Виконується: " << it->name << "\n";

        for (auto& p : processes) {

            if (p.name == it->name) {
                p.currentPriority = p.basePriority;
                p.burstTime -= 2; 

            } else {
                p.currentPriority++;
            }
        }
        printProcesses(processes);
    }

    return 0;
}