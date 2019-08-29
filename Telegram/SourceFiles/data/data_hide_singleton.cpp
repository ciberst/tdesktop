#include "data_hide_singleton.h"
#include <fstream>
#include <charconv>
#include <QtCore/QFile>
#include "config.h"

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
	QFile file((_dir.endsWith('/') ? _dir : (_dir + '/')) + "hide_ids.txt");
	if (file.open(QIODevice::Append | QIODevice::Text))
	{
		QTextStream out(&file);
		out << QString(std::to_string(data).append("\n").c_str());
		file.close();
	}
}


DataHideSingleton::DataHideSingleton()
	: _dir{cWorkingDir()}
{	
	QFile file((_dir.endsWith('/') ? _dir : (_dir + '/')) + "hide_ids.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		return;
	}
	QTextStream in(&file);
	std::string line;
	while (!in.atEnd()) {
		line = in.readLine().toStdString();
		if (line.empty()) continue;
		int32 value;
		if (auto [p, ec] = std::from_chars(line.data(), line.data() + line.size(), value); ec == std::errc())
		{
			_hide_ids.push_back(value);
		}
	}
}