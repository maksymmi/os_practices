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

// Таск 1
std::atomic<int> task1_call_count(0);
std::mutex task1_mutex;
std::condition_variable task1_cv;

bool at_least_one_finished = false;
std::mutex file_log_mutex;

void Task1ThreadFunction(int id) {
    task1_call_count++; 

#ifdef DEBUG
    {
        std::lock_guard<std::mutex> lock(file_log_mutex);
        std::ofstream logFile("debug_log.txt", std::ios::app);

        if (logFile.is_open()) {
            logFile << "Потік " << id << " розпочато.\n";
        }
    }
#endif

    // Цикл імітації роботи
    volatile long dummy_trigger = 0;
    for (long i = 0; i < 300000000; ++i) {
        dummy_trigger = i; 
    }

#ifdef DEBUG
    {
        std::lock_guard<std::mutex> lock(file_log_mutex);
        std::ofstream logFile("debug_log.txt", std::ios::app);
        if (logFile.is_open()) {
            logFile << "Потік " << id << " завершено.\n";
        }
    }
#endif
    {
        std::lock_guard<std::mutex> lock(task1_mutex);
        at_least_one_finished = true;

        task1_cv.notify_one();
    }
}

void ExecuteTask1() {
    std::cout << "\n! Задачa 1 !\n";

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
    std::cout << "Сигнал -> мінімум один потік завершив роботу.\n";
    std::cout << "Поточну функцію викликано -> " << task1_call_count.load() << "\n";

    // Очищення потоків 
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
    std::cout << "10 потоків завершено.\n";
}

// Таск 2
std::atomic<int> task2_call_count(0);

void Task2ThreadFunction() {
    task2_call_count++;
}

void ExecuteTask2() {
    std::cout << "\n! Задачa 2 !\n";
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
            std::cout << "ОС заблокувала створення нових потоків на -> " << num_threads << "\n";

            creation_failed = true;
        }

        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }

        std::cout << "Запущено потоків -> " << num_threads 
                  << " ~ Успішно виконано функцій -> " << task2_call_count.load() << "\n";

        // Умови виходу за ліміти
        if (creation_failed || task2_call_count.load() != num_threads) {
            std::cout << "Стабільна максимальна кількість -> " << num_threads / 2 << "\n";
            break;
        }

        num_threads *= 2; 
        
        // Запобіжник для часу виконання 
        if (num_threads > 2000) {
            std::cout << "Досягнуто ліміту тестування.\n";

            break;
        }
    }
}

// Таск 3
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
        
        // Cтворення віртуального файлу
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
            std::cout << "Споживач " << id << " Оброблено файл -> " << filepath << " -> Зміст -> \"" << content << "\"\n";

            f.close();
            // Видаляємо тимчасовий файл 
            std::remove(filepath.c_str()); 
        }
    }
}

void ExecuteTask3() {
    std::cout << "\n! Задачa 3 !\n";
    
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    // Створюємо 3 виробників та 2 споживачів
    for (int i = 0; i < 3; ++i) producers.emplace_back(ProducerThread, i + 1, 3);
    for (int i = 0; i < 2; ++i) consumers.emplace_back(ConsumerThread, i + 1);

    for (auto& p : producers) p.join();
    file_queue.SetFinished(); 
    for (auto& c : consumers) c.join();
    
    std::cout << "Процеси завершені.\n";
}

// Таск 5 
const int PHILOSOPHERS_COUNT = 5;
std::mutex forks[PHILOSOPHERS_COUNT];

void PhilosopherThread(int id, int meals_count) {
    int left_fork = id;
    int right_fork = (id + 1) % PHILOSOPHERS_COUNT;

    for (int i = 0; i < meals_count; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

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
        // Виделки автоматично звільняються при виході з області 
    }
}

void ExecuteTask5() {
    std::cout << "\n! Задачa 5\n";
    std::vector<std::thread> philosophers;

    // Запускаємо 5 філософів
    for (int i = 0; i < PHILOSOPHERS_COUNT; ++i) {
        philosophers.emplace_back(PhilosopherThread, i, 3);
    }

    for (auto& p : philosophers) p.join();
    std::cout << "Перевірка завершена -> блокувань не виявлено.\n";
}

int main() {
    std::cout << "~ ПОЧАТОК ~ \n";

    ExecuteTask1();
    ExecuteTask2();
    ExecuteTask3();
    ExecuteTask5();

    std::cout << " Усі обрані завдання виконані. Програма завершена. \n";

    return 0;
}