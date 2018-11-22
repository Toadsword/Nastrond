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
bool FileExists(const std::string & filename)
{
	fs::path p = filename;
	return fs::exists(p);
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

std::ifstream::pos_type CalculateFileSize(const std::string& filename)
{
	std::ifstream in(filename, std::ifstream::binary | std::ifstream::ate);
	return in.tellg();
}
bool CreateDirectory(const std::string& dirname)
{
	return fs::create_directory(dirname);
}
bool RemoveDirectory(const std::string &dirname, bool removeAll)
{
	if(removeAll)
	{
		return fs::remove_all(dirname);
	} else
	{
		return fs::remove(dirname);
	}
}

}
