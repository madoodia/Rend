/* (C) 2026 madoodia.com */

#include "utils.h"

// class TimeStamp
// {
// public:
// 	TimeStamp(char* msg)
// 	{
// 		m_start = ReadTimeStampCounter();
// 		m_cpu_hz = GetCPUFrequencyHz();
// 		m_msg = msg;
// 	}
// 	~TimeStamp()
// 	{
// 		m_end = ReadTimeStampCounter();

// 		double elapsed_ms = (double)(m_end - m_start) / m_cpu_hz * 1000.0;
// 		printf("%s >>> Elapsed Time: %.2f ms\n", m_msg, elapsed_ms);
// 	}

// private:
// 	int m_start;
// 	int m_end;
// 	int m_cpu_hz;
// 	char* m_msg;
// };

int main(int, char**)
{

	{
		TimeStamp Timer("Summation 1");
		int sum = 0;
		for (int i = 0; i < 1000000; i++)
		{
			sum += i;
		}
	}
	{
		TimeStamp Timer("Summation 2");
		int sum = 0;
		for (int i = 0; i < 1000000; i++)
		{
			sum += i;
		}
		printf("Sum: %d\n", sum);
	}

	return 0;
}
