#include <stdio.h>
#include <cstring>
#include <chrono>

static std::chrono::steady_clock::time_point StartTime;
static std::chrono::steady_clock::time_point EndTime;

void StartTimer()
{
	StartTime = std::chrono::steady_clock::now();
}

void EndTimer()
{
	EndTime = std::chrono::steady_clock::now();
}

double GetElapsedTimeInMilliseconds()
{
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);
	return static_cast<double>(duration.count());
}

int main(int argc, char** argv)
{
	if (argc > 1 && strcmp(argv[1], "start") == 0)
	{
		StartTimer();
		FILE* file = fopen("timer_log.txt", "w");
		if (file)
		{
			long long start_time_ns = StartTime.time_since_epoch().count();
			fprintf(file, "%lld\n", start_time_ns);
			fclose(file);
		}
	}
	else if (argc > 1 && strcmp(argv[1], "end") == 0)
	{
		EndTimer();
		FILE* file = fopen("timer_log.txt", "r");
		long long start_time_ns = 0;
		if (file)
		{
			fscanf(file, "%lld", &start_time_ns);
			fclose(file);
		}
		long long end_time_ns = EndTime.time_since_epoch().count();
		long long elapsed_ns = end_time_ns - start_time_ns;
		double result = static_cast<double>(elapsed_ns) / 1e9;
		printf("Elapsed Time: %f s\n", result);
	}



	return (0);
}
