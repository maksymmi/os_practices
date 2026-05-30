extern "C" const char* get_string(int id) {
    switch(id) {
        case 101: return "Surname -> Stupak";
        case 102: return "Faculty -> Computer Science";
        case 103: return "Group -> SE-24-6";
        case 104: return "Discipline > Operating Systems";
        default: return "String not found";
    }
}