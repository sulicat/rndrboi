#include "utils.hpp"

void rndrboi::utils::print_string_list( std::string prefix, char** list, int count, std::string suffix )
{
    for( int i = 0; i < count; i++ )
    {
        std::cout << prefix << A_RESET << list[i] << suffix << "\n";
    }
}


std::vector<char> rndrboi::utils::read_file(const std::string& filename)
{
    std::ifstream file( filename,
                        std::ios::ate | std::ios::binary );

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t file_size = (size_t) file.tellg();
    std::vector<char> buffer(file_size);
    file.seekg(0);
    file.read(buffer.data(), file_size);
    file.close();

    return buffer;
}
