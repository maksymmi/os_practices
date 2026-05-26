#include <iostream>
#include <vector>
#include <queue>
#include <string>

struct Process {

    std::string name;

    int remainingTime; 
    int currentQueue;  

    Process(std::string n, int t) : name(n), remainingTime(t), currentQueue(0) {}
};

// Параметри черг
const int NUM_QUEUES = 4;
const int QUANTA[NUM_QUEUES] = {2, 4, 8, 1000000}; 

int main() {
    
    std::vector<Process> allProcesses = {
        Process("P1", 15),
        Process("P2", 3),
        Process("P3", 25),
        Process("P4", 5)
    };

    // Масив черг
    std::queue<Process*> queues[NUM_QUEUES];

   
    for (auto& p : allProcesses) {
        queues[0].push(&p);
    }

    std::cout << "Запуск багаторівневої черги планування...\n";
    std::cout << "-------------------------------------------\n";

    bool tasksRemaining = true;
    while (tasksRemaining) {
        tasksRemaining = false;
        
        for (int i = 0; i < NUM_QUEUES; ++i) {
            if (!queues[i].empty()) {

                tasksRemaining = true;

                Process* p = queues[i].front();
                queues[i].pop();

                int timeSlice = QUANTA[i];
                int actualWork = std::min(p->remainingTime, timeSlice);

                std::cout << "Черга Q" << (i + 1) << " | Виконується: " << p->name 
                          << " (залишилось: " << p->remainingTime << ", квант: " << timeSlice << ")\n";

                p->remainingTime -= actualWork;

                if (p->remainingTime > 0) {
                    int nextQ = std::min(i + 1, NUM_QUEUES - 1);

                    p->currentQueue = nextQ;
                    queues[nextQ].push(p);

                    std::cout << "  -> Процес " << p->name << " переміщено в Q" << (nextQ + 1) << "\n";

                } else {
                    std::cout << "  ** Процес " << p->name << " ЗАВЕРШЕНО **\n";
                }

                break; 
            }
        }
    }

    std::cout << "-------------------------------------------\n";
    std::cout << "Всі процеси виконані.\n";

    return 0;
}