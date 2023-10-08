#include "utils.hpp"

void rndrboi::utils::print_string_list( std::string prefix, char** list, int count, std::string suffix )
{
    for( int i = 0; i < count; i++ )
    {
	std::cout << prefix << A_RESET << list[i] << suffix << "\n";
    }

}
