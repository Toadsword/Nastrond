/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
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
bool IsDirectory(const std::string & filename)
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

bool CopyFile(const std::string& source, const std::string& destination)
{
	return fs::copy_file(source, destination);
}
}
