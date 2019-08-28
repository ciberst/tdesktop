#pragma once
#include "base/basic_types.h"
#include <memory>
#include <vector>
#include <mutex>
class DataHideSingleton {
public:
	static DataHideSingleton& getInst();
	const std::vector<uint64>& getIds() const;
	void addId(uint64 data);

private:
	DataHideSingleton();
	std::vector<uint64> _hide_ids;
	mutable std::mutex _mutex;
	
	static std::unique_ptr<DataHideSingleton> _inst;
};





