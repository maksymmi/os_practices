extern "C" const char* get_string(int id) {
    switch(id) {
        case 101: return "Прізвище ->Ступак";
        case 102: return "Факультет -> Комп'ютерних наук";
        case 103: return "Група -> ПЗПІ-24-6"; 
        case 104: return "Дисципліна -> Операційні системи";
        default: return "Рядок не знайдено";
    }
}