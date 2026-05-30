#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem> 

namespace fs = std::filesystem;

class Mailbox {
private:
    // Загальнa кількість скриньок 
    static int total_mailboxes;
    
    std::string filepath;

    // Структура заголовка скриньки
    struct MailboxHeader {
        uint32_t message_count = 0;
        uint32_t total_msg_size = 0; 
        uint32_t max_mailbox_size = 0;
    } header;

public:
    // Створюємо вскриньку у каталозі
    Mailbox(const std::string& directory, const std::string& filename, uint32_t max_size) {

        total_mailboxes++;

        if (!fs::exists(directory)) {
            fs::create_directory(directory);
        }

        filepath = directory + "/" + filename;

        std::ifstream check_file(filepath, std::ios::binary);

        if (!check_file) {
            std::ofstream out_file(filepath, std::ios::binary);

            header.message_count = 0;
            header.total_msg_size = 0;
            header.max_mailbox_size = max_size;

            out_file.write(reinterpret_cast<const char*>(&header), sizeof(MailboxHeader));

            out_file.close();

            addMessage("Тестове повідомлення 1: Never gonna give you up.");
            addMessage("Тестове повідомлення 2: Never gonna let you down.");
            addMessage("Тестове повідомлення 3: Never gonna run around and desert you.");
            std::cout << "~~~ Створено скриньку та записано 3 тестові листи ~~~\n";

        } else {
            // Якщо файл уже існує — просто зчитуємо його поточний заголовок
            check_file.read(reinterpret_cast<char*>(&header), sizeof(MailboxHeader));
            check_file.close();
            std::cout << "[Система] Успішно відкрито існуючу поштову скриньку.\n";
        }
    }

    // Зменшуємо лічильник об'єктів при видаленні скриньки
    ~Mailbox() {
        total_mailboxes--;
    }

    static int getTotalMailboxes() {
        return total_mailboxes;
    }

    uint32_t getMessageCount() const {
        return header.message_count;
    }

    // Додавання листа
    bool addMessage(const std::string& body) {
        uint32_t msg_len = body.size();
        uint32_t required_space = sizeof(uint32_t) + msg_len;

        // Перевірка ліміту скриньки 
        if (sizeof(MailboxHeader) + header.total_msg_size + required_space > header.max_mailbox_size) {
            std::cout << "Помилка! Перевищено максимальний розмір поштової скриньки (" 
                      << header.max_mailbox_size << " байт).\n";

            return false;
        }

        std::fstream file(filepath, std::ios::binary | std::ios::in | std::ios::out);
        if (!file) return false;

        // Переміщуємо покажчик у кінець файлу 
        file.seekp(0, std::ios::end);
        file.write(reinterpret_cast<const char*>(&msg_len), sizeof(msg_len));
        file.write(body.c_str(), msg_len);

        // Оновлення внутрішнього заголовкa об'єкта
        header.message_count++;
        header.total_msg_size += required_space;

        // Переміщуємо покажчик на початок файлу 
        file.seekp(0, std::ios::beg);
        file.write(reinterpret_cast<const char*>(&header), sizeof(MailboxHeader));
        file.close();

        return true;
    }

    // Читання листа з видаленням або без
    void readMessage(uint32_t index, bool delete_after) {
        if (index == 0 || index > header.message_count) {
            std::cout << "Помилка! -> Листа з індексом " << index << " не існує\n";

            return;
        }

        std::ifstream file(filepath, std::ios::binary);
        if (!file) return;

        // Зміщуємо покажчик 
        file.seekg(sizeof(MailboxHeader), std::ios::beg);

        uint32_t current_len = 0;
        std::string text = "";

        for (uint32_t i = 1; i <= header.message_count; ++i) {
            file.read(reinterpret_cast<char*>(&current_len), sizeof(current_len));

            std::vector<char> buffer(current_len);
            file.read(buffer.data(), current_len);

            if (i == index) {
                text = std::string(buffer.begin(), buffer.end());

                break;
            }
        }

        file.close();

        std::cout << "\n[Зміст листа №" << index << " (" << current_len << " байт)]: " << text << "\n";

        if (delete_after) {
            deleteMessage(index);
        }
    }

    // Видалення листа за індексом
    void deleteMessage(uint32_t index) {
        if (index == 0 || index > header.message_count) {
            std::cout << "Помилка: Некоректний індекс для видалення.\n";

            return;
        }

        std::ifstream file(filepath, std::ios::binary);
        if (!file) return;

        file.seekg(sizeof(MailboxHeader), std::ios::beg);

        // Структура для тимчасового збереження даних у пам'яті
        struct TempLetter {
            uint32_t len;
            std::string body;
        };
        std::vector<TempLetter> remaining_letters;

        // Зчитуємо всі листи
        for (uint32_t i = 1; i <= header.message_count; ++i) {
            uint32_t current_len = 0;
            file.read(reinterpret_cast<char*>(&current_len), sizeof(current_len));

            std::vector<char> buffer(current_len);
            file.read(buffer.data(), current_len);

            if (i != index) {
                remaining_letters.push_back({current_len, std::string(buffer.begin(), buffer.end())});
            }
        }

        file.close();

        // Oчищуємо файл 
        std::ofstream out(filepath, std::ios::binary | std::ios::trunc);
        header.message_count = remaining_letters.size();
        header.total_msg_size = 0;

        for (const auto& letter : remaining_letters) {
            header.total_msg_size += sizeof(uint32_t) + letter.len;
        }

        out.write(reinterpret_cast<const char*>(&header), sizeof(MailboxHeader));

        // Записуємо листи що залишилися
        for (const auto& letter : remaining_letters) {
            out.write(reinterpret_cast<const char*>(&letter.len), sizeof(letter.len));
            out.write(letter.body.c_str(), letter.len);
        }
        out.close();

        std::cout << "Лист #" << index << " видалено зі скриньки!\n";
    }

    // Видалення усіх листів 
    void deleteAllMessages() {
        std::ofstream out(filepath, std::ios::binary | std::ios::trunc);
          
        header.message_count = 0;
        header.total_msg_size = 0;

        out.write(reinterpret_cast<const char*>(&header), sizeof(MailboxHeader));
        out.close();

        std::cout << "Усі листи видалено! Скринька порожня ~\n";
    }
};

int Mailbox::total_mailboxes = 0;

int main() {
    Mailbox myMailbox("mailbox_dir", "file.dat", 5000);

    while (true) {
        std::cout << "\n~~~ МЕНЮ ~~~\n";
        std::cout << "1 -> Додати новий лист\n";
        std::cout << "2 -> Читати лист (без видалення)\n";
        std::cout << "3 -> Читати лист (з видаленням)\n";
        std::cout << "4 -> Видалити конкретний лист за номером\n";
        std::cout << "5 -> Видалити всі листи зі скриньки\n";
        std::cout << "6 -> Дізнатися кількість листів у скриньці\n";
        std::cout << "7 -> Дізнатися загальну кількість об'єктів-скриньок у програмі\n";
        std::cout << "0 -> Вийти з програми\n";
        std::cout << "Що робимо? -> ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');

            continue;
        }
        std::cin.ignore(); 

        if (choice == 0) break;

        switch (choice) {
            case 1: {
                std::cout << "Введіть текст листа -> ";
                std::string text;
                std::getline(std::cin, text);

                if (myMailbox.addMessage(text)) {
                    std::cout << "Додано!\n";
                }

                break;
            }
            case 2: {
                std::cout << "Введіть номер листа -> ";
                uint32_t idx;
                std::cin >> idx;

                myMailbox.readMessage(idx, false);

                break;
            }
            case 3: {
                std::cout << "Введіть номер листа -> ";
                uint32_t idx;
                std::cin >> idx;

                myMailbox.readMessage(idx, true);

                break;
            }
            case 4: {
                std::cout << "Введіть номер листа -> ";
                uint32_t idx;
                std::cin >> idx;

                myMailbox.deleteMessage(idx);

                break;
            }
            case 5:
                myMailbox.deleteAllMessages();

                break;
            case 6:
                std::cout << "Кількість листів у файлі -> " << myMailbox.getMessageCount() << "\n";

                break;
            case 7:
                std::cout << "Загальна кількість об'єктів у пам'яті -> " << Mailbox::getTotalMailboxes() << "\n";

                break;
            default:
                std::cout << "Спробуйте ще раз :/\n";
        }
    }

    return 0;
}