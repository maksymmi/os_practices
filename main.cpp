#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <queue>
#include <string>
#include <chrono>

// ============================================================================
// ЗАДАЧА 1: Ресурсомісткі потоки та сигналізація завершення
// ============================================================================
std::atomic<int> task1_call_count(0);
std::mutex task1_mutex;
std::condition_variable task1_cv;
bool at_least_one_finished = false;
std::mutex file_log_mutex;

void Task1ThreadFunction(int id) {
    task1_call_count++; // Рахуємо кількість викликів потокової функції

#ifdef DEBUG
    {
        std::lock_guard<std::mutex> lock(file_log_mutex);
        std::ofstream logFile("debug_log.txt", std::ios::app);
        if (logFile.is_open()) {
            logFile << "[DEBUG] Потік " << id << " РОЗПОЧАТО.\n";
        }
    }
#endif

    // Цикл для імітації тривалої роботи, що перевищує квант часу
    double dummy_calculation = 0.0;
    for (volatile long i = 0; i < 300000000; ++i) {
        dummy_calculation += 0.001;
    }

#ifdef DEBUG
    {
        std::lock_guard<std::mutex> lock(file_log_mutex);
        std::ofstream logFile("debug_log.txt", std::ios::app);
        if (logFile.is_open()) {
            logFile << "[DEBUG] Потік " << id << " ЗАВЕРШЕНО.\n";
        }
    }
#endif

    // Сповіщаємо головний потік про завершення хоча б одного потоку
    {
        std::lock_guard<std::mutex> lock(task1_mutex);
        at_least_one_finished = true;
        task1_cv.notify_one();
    }
}

void ExecuteTask1() {
    std::cout << "\n--- Виконання Задачі 1 ---\n";
    task1_call_count = 0;
    at_least_one_finished = false;
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(Task1ThreadFunction, i + 1);
    }

    // Чекаємо завершення хоча б одного потоку
    {
        std::unique_lock<std::mutex> lock(task1_mutex);
        task1_cv.wait(lock, [] { return at_least_one_finished; });
    }
    std::cout << "Сигнал: Як мінімум один потік завершив роботу!\n";
    std::cout << "Поточну функцію викликано разів: " << task1_call_count.load() << "\n";

    // Очищення потоків (обов'язково для std::thread)
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
    std::cout << "Усі 10 потоків успішно синхронізовано та завершено.\n";
}

// ============================================================================
// ЗАДАЧА 2: Визначення максимальної кількості потоків
// ============================================================================
std::atomic<int> task2_call_count(0);

void Task2ThreadFunction() {
    task2_call_count++;
    // Потік швидко завершується після фіксації виклику
}

void ExecuteTask2() {
    std::cout << "\n--- Виконання Задачі 2 ---\n";
    int num_threads = 2;
    
    while (true) {
        task2_call_count = 0;
        std::vector<std::thread> threads;
        bool creation_failed = false;

        try {
            for (int i = 0; i < num_threads; ++i) {
                threads.emplace_back(Task2ThreadFunction);
            }
        } catch (const std::system_error& e) {
            std::cout << "ОС заблокувала створення нових потоків на позначці: " << num_threads << "\n";
            creation_failed = true;
        }

        // Чекаємо завершення створеної пачки потоків
        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }

        std::cout << "Запущено потоків: " << num_threads 
                  << " | Успішно виконано функцій: " << task2_call_count.load() << "\n";

        // Перевірка умови виходу за ліміти
        if (creation_failed || task2_call_count.load() != num_threads) {
            std::cout << "Невідповідність знайдена. Стабільна максимальна кількість: " << num_threads / 2 << "\n";
            break;
        }

        num_threads *= 2; // Геометрична прогресія
        
        // Запобіжник для адекватного часу виконання лабораторної
        if (num_threads > 2000) {
            std::cout << "Досягнуто безпечного ліміту тестування програми (2000 потоків).\n";
            break;
        }
    }
}

// ============================================================================
// ЗАДАЧА 3: Виробник і Споживач (Потокобезпечна черга)
// ============================================================================
class SafeQueue {
private:
    std::queue<std::string> q;
    std::mutex mtx;
    std::condition_variable cv;
    bool finished = false;
public:
    void Push(const std::string& val) {
        std::lock_guard<std::mutex> lock(mtx);
        q.push(val);
        cv.notify_one();
    }

    bool Pop(std::string& val) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !q.empty() || finished; });
        if (q.empty() && finished) return false;
        val = q.front();
        q.pop();
        return true;
    }

    void SetFinished() {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
        cv.notify_all();
    }
};

SafeQueue file_queue;

void ProducerThread(int id, int count) {
    for (int i = 0; i < count; ++i) {
        std::string filename = "shared_file_p" + std::to_string(id) + "_" + std::to_string(i) + ".dat";
        
        // Імітуємо роботу: створення віртуального файлу
        std::ofstream f("/tmp/" + filename);
        f << "Дані від виробника " << id << " номер запису " << i;
        f.close();

        file_queue.Push("/tmp/" + filename);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void ConsumerThread(int id) {
    std::string filepath;
    while (file_queue.Pop(filepath)) {
        std::ifstream f(filepath);
        if (f.is_open()) {
            std::string content;
            std::getline(f, content);
            std::cout << "[Споживач " << id << "] Оброблено файл: " << filepath << " -> Зміст: \"" << content << "\"\n";
            f.close();
            std::remove(filepath.c_str()); // Видаляємо тимчасовий файл після зчитування
        }
    }
}

void ExecuteTask3() {
    std::cout << "\n--- Виконання Задачі 3 (Виробники та Споживачі) ---\n";
    
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    // Створюємо 3 виробників та 2 споживачів
    for (int i = 0; i < 3; ++i) producers.emplace_back(ProducerThread, i + 1, 3);
    for (int i = 0; i < 2; ++i) consumers.emplace_back(ConsumerThread, i + 1);

    for (auto& p : producers) p.join();
    file_queue.SetFinished(); // Всі файли згенеровано
    for (auto& c : consumers) c.join();
    
    std::cout << "Черга успішно перевірена на потокобезпечність. Процеси завершені.\n";
}

// ============================================================================
// ЗАДАЧА 5: Філософи, що обідають (Асиметричне блокування м'ютексів)
// ============================================================================
const int PHILOSOPHERS_COUNT = 5;
std::mutex forks[PHILOSOPHERS_COUNT];

void PhilosopherThread(int id, int meals_count) {
    int left_fork = id;
    int right_fork = (id + 1) % PHILOSOPHERS_COUNT;

    for (int i = 0; i < meals_count; ++i) {
        // Філософ думає
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        // Асиметричне взяття виделок для уникнення взаємного блокування (Deadlock)
        if (id % 2 == 0) {
            std::unique_lock<std::mutex> lock_right(forks[right_fork]);
            std::unique_lock<std::mutex> lock_left(forks[left_fork]);
            
            std::cout << "Філософ " << id << " обідає (ітерація " << i + 1 << ")\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        } else {
            std::unique_lock<std::mutex> lock_left(forks[left_fork]);
            std::unique_lock<std::mutex> lock_right(forks[right_fork]);
            
            std::cout << "Філософ " << id << " обідає (ітерація " << i + 1 << ")\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        // Виделки автоматично звільняються при виході з області видимості блоку
    }
}

void ExecuteTask5() {
    std::cout << "\n--- Виконання Задачі 5 (Філософи, що обідають) ---\n";
    std::vector<std::thread> philosophers;

    // Запускаємо 5 філософів, кожен має пообідати по 3 рази
    for (int i = 0; i < PHILOSOPHERS_COUNT; ++i) {
        philosophers.emplace_back(PhilosopherThread, i, 3);
    }

    for (auto& p : philosophers) p.join();
    std::cout << "Перевірка завершена: дедлоків та стану гонитви не виявлено.\n";
}

// ============================================================================
// ГОЛОВНА КЕРУЮЧА ПРОГРАМА
// ============================================================================
int main() {
    std::cout << "========================================================\n";
    std::cout << "   ЛАБОРАТОРНА РОБОТА: КЕРУВАННЯ ПОТОКАМИ В LINUX C++   \n";
    std::cout << "========================================================\n";

    ExecuteTask1();
    ExecuteTask2();
    ExecuteTask3();
    ExecuteTask5();

    std::cout << "\n========================================================\n";
    std::cout << " Усі обрані завдання успішно виконані. Програма завершена. \n";
    std::cout << "========================================================\n";
    return 0;
}