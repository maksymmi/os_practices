#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>

// Встановлення та перевірка паролю
unsigned long simpleHash(const char* str) {
    unsigned long hash = 5381;

    int c;
    while ((c = *str++)) hash = ((hash << 5) + hash) + c;

    return hash;
}

bool isGoodPassword(const char* pass) {
    std::string p(pass);

    if (p.length() < 8) return false;
    bool hasUpper = false, hasLower = false, hasDigit = false;

    for (char c : p) {
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
        if (isdigit(c)) hasDigit = true;
    }

    return hasUpper && hasLower && hasDigit;
}

void securePasswordSetup() {
    size_t size = 256;
    
    // Виділення пам'яті
    char* pass1 = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    char* pass2 = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    // Захист даних від вивантаження на диск
    mlock(pass1, size);
    mlock(pass2, size);

    std::cout << "Введіть пароль (мін. 8 символів) -> ";
    std::cin.ignore(); // Clear any leftover newline characters
    std::cin.getline(pass1, size);
    std::cout << "Повторіть пароль -> ";
    std::cin.getline(pass2, size);

    if (strcmp(pass1, pass2) == 0 && isGoodPassword(pass1)) {
        unsigned long hash = simpleHash(pass1);

        std::cout << "Пароль було встановлено. Хеш -> " << hash << "\n";
        
        std::cout << "Введіть пароль для перевірки -> ";

        char* testPass = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        mlock(testPass, size);
        std::cin.getline(testPass, size);
        
        if (simpleHash(testPass) == hash) {
            std::cout << "Пароль вірний!\n";
        } else {
            std::cout << "Невірний пароль!\n";
        }
        
        memset(testPass, 0, size);
        munlock(testPass, size);
        munmap(testPass, size);

    } else {
        std::cout << "Помилка! Паролі не співпадають або не відповідають вимогам безпеки.\n";
    }

    memset(pass1, 0, size);
    memset(pass2, 0, size);
    
    // Зняття блокування 
    munlock(pass1, size);
    munlock(pass2, size);
    
    munmap(pass1, size);
    munmap(pass2, size);
}

// Черга на heap 
class CustomHeap {
    void* start;
    size_t size;
    size_t offset;
public:
    CustomHeap(size_t sz) : size(sz), offset(0) {
        start = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }
    void* alloc(size_t bytes) {
        if (offset + bytes > size) return nullptr;
        void* ptr = (char*)start + offset;
        offset += bytes;

        return ptr;
    }

    ~CustomHeap() { munmap(start, size); }
};

struct Node {
    int data;
    Node* next;
};

class HeapStack {
    Node* topNode;
    CustomHeap* heap;
public:
    HeapStack(CustomHeap* h) : topNode(nullptr), heap(h) {}
    
    void push(int value) {
        Node* newNode = (Node*)heap->alloc(sizeof(Node));
        if (newNode) {
            newNode->data = value;
            newNode->next = topNode;
            topNode = newNode;

            std::cout << "Push -> " << value << "\n";
        }
    }
    
    void pop() {
        if (topNode) {
            std::cout << "Pop -> " << topNode->data << "\n";

            topNode = topNode->next; 
        } else {
            std::cout << "Стек порожній!\n";
        }
    }
};

void testHeapStack() {
    std::cout << "\n~~~ Стек на базі купи ~~~\n";
    CustomHeap myHeap(4096); 
    HeapStack stack(&myHeap);
    
    stack.push(10);
    stack.push(20);
    stack.push(30);
    stack.pop();
    stack.pop();
}

// Моделювання кешу 
struct CacheLine {
    std::list<unsigned long> lru_queue;
    int max_blocks = 4;
    
    void access(unsigned long tag) {
        auto it = std::find(lru_queue.begin(), lru_queue.end(), tag);

        if (it != lru_queue.end()) {
            lru_queue.erase(it); 
            std::cout << " Блок " << tag << " знайдено. Оновлено LRU.\n";
        } else {
            if (lru_queue.size() >= (size_t)max_blocks) {
                std::cout << " [Miss] Кеш-лінія заповнена. Витіснено блок: " << lru_queue.back() << "\n";
                lru_queue.pop_back(); 
            } else {
                std::cout << " [Miss] Блок " << tag << " додано до вільного місця.\n";
            }
        }
        lru_queue.push_front(tag);
    }
};

void simulateCacheLRU() {
    std::cout << "\n~~~ Моделювання кешу ~~~\n";
    int lines_count = 128; 

    std::vector<CacheLine> cache(lines_count);
    
    std::vector<unsigned long> addresses = {
        0x1000, 0x1040, 0x1080, 0x10C0, 0x1100, // 5 запитів до однієї лінії
        0x1040 // Повторний запит
    };

    for (unsigned long A : addresses) {
        unsigned long offset = A & 0x3F; 
        unsigned long line = (A >> 6) & 0x7F; 
        unsigned long tag = A >> 13; 

        std::cout << "Запит до адреси 0x" << std::hex << A << std::dec 
                  << " (Line -> " << line << ", Offset -> " << offset << ", Tag -> " << tag << ")";
        
        cache[line].access(tag);
    }
}

int main() {
    while (true) {
        std::cout << "\n~~~ МЕНЮ ~~~\n";
        std::cout << "1 -> Встановлення та перевірка паролю\n";
        std::cout << "2 -> Стек у динамічній пам'яті\n";
        std::cout << "3 -> LRU алгоритм\n";
        std::cout << "0 -> Вихід\n";
        std::cout <<"Що робимо? -> \n";
        
        int choice;
        if (!(std::cin >> choice)) break;

        switch (choice) {
            case 1: securePasswordSetup(); break;
            case 2: testHeapStack(); break;
            case 3: simulateCacheLRU(); break;
            case 0: return 0;
            default: std::cout << "Невірний вибір!\n";
        }
    }
    return 0;
}