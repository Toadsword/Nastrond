
#include <utility/json_utility.h>
#include <utility/log.h>

#include <fstream>
#include <string>

namespace sfge
{

bool IsJsonValueNumeric(const json::value_type & jsonValue)
{
	return jsonValue.type() == json::value_t::number_float || 
		   jsonValue.type() == json::value_t::number_integer || 
		   jsonValue.type() == json::value_t::number_unsigned;
}

bool CheckJsonExists(const json & jsonObject, std::string parameterName)
{
	return jsonObject.find(parameterName) != jsonObject.end();
}

bool CheckJsonParameter(const json& jsonObject, std::string parameterName, json::value_t expectedType)
{
	return CheckJsonExists(jsonObject, parameterName) && jsonObject[parameterName].type() == expectedType;
}

bool CheckJsonNumber(const json& jsonObject, std::string parameterName)
{
	return CheckJsonParameter(jsonObject, parameterName, json::value_t::number_float) or
		   CheckJsonParameter(jsonObject, parameterName, json::value_t::number_integer) or
		   CheckJsonParameter(jsonObject, parameterName, json::value_t::number_unsigned);
}

sf::Vector2f GetVectorFromJson(const json & jsonObject, std::string parameterName)
{
	sf::Vector2f vector = sf::Vector2f();
	if (CheckJsonParameter(jsonObject, parameterName, json::value_t::array))
	{
		if (jsonObject[parameterName].size() == 2)
		{
			auto vectorJson = jsonObject[parameterName];
			if (IsJsonValueNumeric(vectorJson[0]))
			{
				vector.x = vectorJson[0];
			}
			if (IsJsonValueNumeric(vectorJson[1]))
			{
				vector.y = vectorJson[1];
			}
		}
	}
	else if (CheckJsonParameter(jsonObject, parameterName, json::value_t::object))
	{
		auto vectorJson = jsonObject[parameterName];
		if (IsJsonValueNumeric(vectorJson["x"]))
		{
			vector.x = vectorJson["x"];
		}
		if (IsJsonValueNumeric(vectorJson["y"]))
		{
			vector.y = vectorJson["y"];
		}
	}
	return vector;
}

std::unique_ptr<json> LoadJson(std::string jsonPath)
{
	std::ifstream jsonFile(jsonPath.c_str());
	if (jsonFile.peek() == std::ifstream::traits_type::eof())
	{
		{
			std::ostringstream oss;
			oss << "[JSON ERROR] EMPTY JSON FILE at: " << jsonPath;
			Log::GetInstance()->Error(oss.str());
		}
		return nullptr;
	}
	std::unique_ptr<json> jsonContent = std::make_unique<json>();
	try
	{
		jsonFile >> *jsonContent;
	}
	catch (json::parse_error& e)
	{
		{
			std::ostringstream oss;
			oss << "THE FILE: " << jsonPath << " IS NOT JSON\n" << e.what();
			Log::GetInstance()->Error(oss.str());
		}
		return nullptr;
	}
	return jsonContent;
}
}
