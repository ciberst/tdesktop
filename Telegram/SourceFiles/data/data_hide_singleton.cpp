#include "data_hide_singleton.h"
#include <fstream>
#include <charconv>
std::unique_ptr<DataHideSingleton> DataHideSingleton::_inst;

DataHideSingleton& DataHideSingleton::getInst()
{
	if (_inst) return *_inst;
	_inst.reset(new DataHideSingleton());
	return *_inst;
}

const std::vector<uint64>& DataHideSingleton::getIds() const
{
	std::unique_lock<std::mutex> lock(_mutex);
	return _hide_ids;
}

void DataHideSingleton::addId(uint64 data)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_hide_ids.push_back(data);
	std::ofstream file("hide_ids.txt", std::ios::app);
	if (file.is_open())
	{
		file << std::to_string(data).append("\n");
		file.close();
	}
}


DataHideSingleton::DataHideSingleton()
{
	std::ifstream file("hide_ids.txt");
	if (file.is_open())
	{
		std::string line;
		while(std::getline(file, line))
		{
			if (line.empty()) continue;
			int32 value;
			if (auto [p, ec] = std::from_chars(line.data(), line.data() + line.size(), value); ec == std::errc())
			{
				_hide_ids.push_back(value);
			}
		}

	}
	else
	{
		std::ofstream file_test("hide_ids.txt");
		if (file.is_open()) file_test.close();
	}
}