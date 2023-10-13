#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>

#define A_RESET "\033[0m"
#define A_FG(R,G,B) "\033[38;2;" #R ";" #G ";" #B "m"
#define A_BG(R,G,B) "\033[48;2;" #R ";" #G ";" #B "m"
#define A_UL "\e[4m"
#define A_STRIKE "\e[9m"
#define A_BOLD "\e[8m"
#define A_BLINK "\e[5m"

#define A_RED A_FG(255,0,0)
#define A_GREEN A_FG(0,255,0)
#define A_BLUE A_FG(0,0,255)
#define A_YELLOW A_FG(255,255,0)

namespace rndrboi
{
    namespace utils
    {

	// print a c list
	void print_string_list( std::string prefix, char** list, int count, std::string suffix );

	// convert a list to a vector
	template <typename T>
	std::vector<T> list_to_vec( T* list, int count )
	{
	    std::vector<T> out;
	    for( int i = 0; i < count; i++ )
	    {
		out.push_back( list[i] );
	    }
	    return out;
	}


	template <typename IN, typename OUT, typename NAME, typename T>
	std::vector<OUT> member_vec( std::vector<T> input )
	{
	    std::vector<OUT> out( input.size() );
	    for( auto x : input )
		out.push_back( x.NAME );

	    return out;
	}

	template <typename T>
	void print_vec( std::vector<T> in, std::string prefix = "", std::string suffix = "" )
	{
	    for( auto x : in )
		std::cout << prefix << x << suffix << "\n";
	}


	template <typename T>
	void print_iter( T in, std::string prefix = "", std::string suffix = "" )
	{
	    std::cout << prefix << "{ ";
	    for( auto x : in )
	    {
		std::cout << x << " ";
	    }

	    std::cout << "}\n" << suffix;
	}

	std::vector<char> read_file(const std::string& filename);

    }
}

#endif
