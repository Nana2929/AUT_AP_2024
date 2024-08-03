#include "Utils.h"

bool Utils::write_to_file(const std::string &file_name, const std::string &data)

{
    std::ofstream file;
    file.open(file_name);
    if (!file)
    {
        return false;
    }
    file << data;
    file.close();
    return true;
}
