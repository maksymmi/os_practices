#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>

#define LAUNCH_IMMEDIATE(cmd, args, priority, pid) { \
    pid = fork(); \
    if (pid == 0) { \
        setpriority(PRIO_PROCESS, 0, priority); \
        execvp(cmd, args); \
        _exit(1); \
    } \
}

// імітація CREATE_SUSPENDED через SIGSTOP
#define LAUNCH_DEFERRED(cmd, args, priority, pid) { \
    pid = fork(); \
    if (pid == 0) { \
        setpriority(PRIO_PROCESS, 0, priority); \
        raise(SIGSTOP); \
        execvp(cmd, args); \
        _exit(1); \
    } \
}

int main() {
    std::cout << "Програма 3 -> Диспетчер Процесів\n\n";

    std::string folderPath = "/tmp/Lab7Folder";
    
    setenv("LAB7_FOLDER", folderPath.c_str(), 1);

    char* prog1_args[] = { (char*)"./editor", (char*)folderPath.c_str(), NULL };
    char* prog2_args[] = { (char*)"./prog2_analyzer", (char*)folderPath.c_str(), NULL };

    pid_t pid1, pid2;

    // Негайний запуск 
    std::cout << "Запуск Програми 1\n";
    LAUNCH_IMMEDIATE("./editor", prog1_args, 0, pid1);

    if (pid1 > 0) {
        std::cout << "Очікування завершення роботи Програми 1\n";
        waitpid(pid1, NULL, 0); // Очікуємо закриття
        std::cout << "Програма 1 завершила роботу.\n\n";
    }

    // Відкладений запуск Програми 2 
    std::cout << "Відкладений запуск Програми 2\n";
    LAUNCH_DEFERRED("./analyzer", prog2_args, 10, pid2);

    if (pid2 > 0) {
        std::cout << "Процес переведено в стан зупинки. Чекаємо 2 секунди ~\n";
        sleep(2); 

        std::cout << "Надсилаємо SIGCONT для активації Програми 2 ~\n";
        kill(pid2, SIGCONT); // Аналог ResumeThread

        // Очікуємо результат аналізу
        waitpid(pid2, NULL, 0);
        std::cout << "\nПрограма 2 завершила роботу.\n";
    }

    return 0;
}