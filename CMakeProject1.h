// CMakeProject1.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once

#include <iostream>
#include <thread>
#include <random>
#include <mutex>
#include <condition_variable>

class DataControl
{
public:
	DataControl(int sensorsCount)
	{
		sensors = sensorsCount;
		sensorsData = new int[sensors];
	}
	~DataControl()
	{
		delete[] sensorsData;
	}
	void CreateSensorsThread();
	
	
private:
	std::mutex myMutex;
	std::condition_variable myVar;
	int* sensorsData;
	int sensors;
	void DataThread(int* const dataArray, int id);
	void ViewErrorData(int* const dataArray, int size);
};

// TODO: установите здесь ссылки на дополнительные заголовки, требующиеся для программы.
