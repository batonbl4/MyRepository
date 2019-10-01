// CMakeProject1.cpp: определяет точку входа для приложения.
//

#include "CMakeProject1.h"
#include <string>

using namespace std;



int main(int argc, char* argv[])
{

	int sensors = 0;
	string s;
	DataControl* MyClass = nullptr;

	if (argc > 2)
	{
		cout << "Too many arguments. Programm closes...";
		exit(0);
	}
	else if (argc == 1) //если аргумент командной строки не задан, задается случайное кол-во датчиков
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(2, 9);
		sensors = dist(gen);
	}
	else
	{
		s = argv[1];
		try
		{
			sensors = stoi(s);
		}
		catch (...)
		{
			cout << "Invalid input argument. Programm closes...";
			exit(0);
		}
	}
	
	cout<< "[" << sensors << "] sensors are configured" << endl << "Data collection..." << endl;


	MyClass = new DataControl(sensors);
	MyClass->CreateSensorsThread();
	
	return 0;
}

void DataControl::CreateSensorsThread() // метод создания потоков 
{
	for (int i = 0; i < sensors; i++)
	{
		thread T([=] {DataThread(sensorsData, i);}); // создание потока опроса датчиков
		T.detach();
	}
	this_thread::sleep_for(std::chrono::milliseconds(1500));
	thread T([=] {ViewErrorData(sensorsData, sensors); });// создание события вывода сообщений о критических показаниях
	T.join();
}

void DataControl::DataThread(int* const dataArray, int id)// функция опроса датчиков
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(1, 100);
	int data = 0;
	while (true)
	{
		this_thread::sleep_for(std::chrono::milliseconds(1000));
		data = dist(gen);
		lock_guard<mutex> gMutex(myMutex);
		dataArray[id] = data;
		if (data <= 5)
			myVar.notify_one();
			
	}
}

void DataControl::ViewErrorData(int* const dataArray, int size) //обработчик события вывода сообщений
{
	while (true)
	{
		unique_lock<mutex> uMutex(myMutex);
		myVar.wait(uMutex, [=] {
			for (int i = 0; i < sensors; i++)
			{
				if (sensorsData[i] <= 5)
					cout << "Critical readings of the sensor ID: " << i << endl;
			}
			return false;
			});
	}
}