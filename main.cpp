#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include <string>
#include <climits>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/mman.h>

void runPageReplacementAlgorithms() {
    std::vector<int> pages = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int frames = 3;
    
    std::cout << "\n~~~ Алгоритми заміщення сторінок ~~~\n";
    std::list<int> fifo_queue;

    int fifo_faults = 0;

    for (int page : pages) {
        if (std::find(fifo_queue.begin(), fifo_queue.end(), page) == fifo_queue.end()) {
            fifo_faults++;

            if (fifo_queue.size() == frames) fifo_queue.pop_front();

            fifo_queue.push_back(page);
        }
    }

    std::vector<int> lru_memory;

    int lru_faults = 0;

    for (int page : pages) {
        auto it = std::find(lru_memory.begin(), lru_memory.end(), page);

        if (it == lru_memory.end()) {
            lru_faults++;

            if (lru_memory.size() == frames) lru_memory.erase(lru_memory.begin());

        } else {
            lru_memory.erase(it);
        }
        lru_memory.push_back(page);
    }
    std::cout << "FIFO (Page Faults): " << fifo_faults << "\n";
    std::cout << "LRU (Page Faults):  " << lru_faults << "\n";
}

void showSystemInfo() {
    std::cout << "\n~~~ Системна інформація про пам'ять ~~~\n";

    struct sysinfo memInfo;
    sysinfo(&memInfo);

    long pagesize = sysconf(_SC_PAGESIZE);
    
    std::cout << "Розмір сторінки пам'яті -> " << pagesize << " байт\n";
    std::cout << "Загальний обсяг фізичної пам'яті -> " << (memInfo.totalram * memInfo.mem_unit) / (1024 * 1024) << " MB\n";
    std::cout << "Доступний обсяг фізичної пам'яті -> " << (memInfo.freeram * memInfo.mem_unit) / (1024 * 1024) << " MB\n";
    std::cout << "Aдреса процесу -> " << (void*)showSystemInfo << "\n\n";
    
    std::cout << "[Відкрийте інший термінал, запустіть програму]\n";
    std::cout << "~ Натисніть Enter ~ \n";
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

void analyzeVirtualMemory() {
    std::cout << "\n~~~ Аналіз віртуальної пам'яті ~~~\n";
    std::cout << "Карта пам'яті процесу -> \n";

    std::ifstream maps_file("/proc/self/maps");
    std::string line;

    for (int i = 0; i < 3 && std::getline(maps_file, line); ++i) {
        std::cout << line << "\n";

    }

    std::cout << "\nВиділяємо пам'ять. Розмір -> 4096 байт...\n";
    void* allocatedMem = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (allocatedMem != MAP_FAILED) {
        std::cout << "Пам'ять успішно виділена за адресою -> " << allocatedMem << "\n";

        munmap(allocatedMem, 4096);

        std::cout << "Пам'ять звільнено!\n";
    } else {
        std::cout << "Помилка!\n";
    }
}

void simulateBestFit() {
    struct MemBlock { int id; int size; bool isFree; };

    std::cout << "\n~~~ Best Fit ~~~\n";
    
    std::vector<MemBlock> heap = {
        {1, 5000, true}, {2, 1200, true}, {3, 7000, true}, {4, 1500, true}, {5, 3000, true}
    };

    int requestSize = 1300;
    std::cout << "Запит на виділення -> " << requestSize << " байт.\n";

    int bestIdx = -1;
    int minDifference = INT_MAX;

    for (size_t i = 0; i < heap.size(); ++i) {
        if (heap[i].isFree && heap[i].size >= requestSize) {
          
            int diff = heap[i].size - requestSize;

            if (diff < minDifference) {
                minDifference = diff;

                bestIdx = i;
            }
        }
    }

    if (bestIdx != -1) {
        heap[bestIdx].isFree = false;

        std::cout << "-> Вибрано Блок " << heap[bestIdx].id << " (Розмір -> " << heap[bestIdx].size 
                  << " байт). Залишок (фрагментація) -> " << minDifference << " байт.\n";
    } else {
        std::cout << "-> Помилка!\n";
    }
}

int main() {
    while (true) {
        std::cout << "\n~~~ МЕНЮ ~~~\n";
        std::cout << "1 -> Алгоритми заміщення сторінок\n";
        std::cout << "2 -> Системна інформація\n";
        std::cout << "3 -> Аналіз віртуальної пам'яті (mmap)\n";
        std::cout << "4 -> 'Найменший достатній'\n";
        std::cout << "0 -> Вихід\n";
        std::cout << "Що робимо? -> ";
        
        int choice;
        if (!(std::cin >> choice)) break;

        switch (choice) {
            case 1: runPageReplacementAlgorithms(); break;
            case 2: showSystemInfo(); break;
            case 3: analyzeVirtualMemory(); break;
            case 4: simulateBestFit(); break;
            case 0: return 0;
            default: std::cout << "Невірний вибір!\n";
        }
    }

    return 0;
}