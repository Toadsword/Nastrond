#include <utility/file_utility.h>

#ifdef __APPLE__

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

namespace sfge
{
bool FileExists(std::string & filename)
{
#ifndef __APPLE__
	fs::path p = filename;
	return fs::exists(p);
#else
    std::ifstream infile(filename);
    return infile.good();
#endif
}
bool IsRegularFile(std::string& filename)
{
    fs::path p = filename;
    return fs::is_regular_file(p);

}
bool IsDirectory(std::string & filename)
{
	fs::path p = filename;
	return fs::is_directory(p);
}
void IterateDirectory(std::string & dirname, std::function<void(std::string)> func)
{
	
	if (IsDirectory(dirname))
	{
		for (auto& p : fs::directory_iterator(dirname))
		{
			func(p.path().generic_string());
		}
	}
}

}
