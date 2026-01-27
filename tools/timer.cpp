#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <stdint.h>

uint64_t GetCPUFrequencyHz()
{
	DWORD freq_mhz = 0;
	DWORD size = sizeof(freq_mhz);
	LONG result = RegGetValueA(HKEY_LOCAL_MACHINE,
							   "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
							   "~MHz", RRF_RT_REG_DWORD, NULL, &freq_mhz, &size);

	if (result == ERROR_SUCCESS)
	{
		return (uint64_t)freq_mhz * 1000000; // MHz to Hz
	}
	return 0;
}

int main(int argc, char** argv)
{
	char filename[512] = "D:\\madoodia\\dev\\Rend\\build\\tsc_start.txt";

	if (argc > 1 && (argv[1][0] == 's' || argv[1][0] == 'S'))
	{
		remove(filename);
		FILE* f = fopen(filename, "w");
		if (f)
		{
			uint64_t start = ReadTimeStampCounter();
			fprintf(f, "%llu\n", start);
			fclose(f);
		}
	}
	else
	{

		FILE* f = fopen(filename, "r");
		uint64_t start_count = 0;
		if (f)
		{
			fscanf(f, "%llu", &start_count);
			fclose(f);

			uint64_t cpu_hz = GetCPUFrequencyHz();
			if (cpu_hz > 0)
			{
				uint64_t end = ReadTimeStampCounter();
				double elapsed_ms = (double)(end - start_count) / cpu_hz * 1000.0;
				printf("%.2f ms\n", elapsed_ms);
			}
			else
			{
				printf("0.00 ms\n"); // Error getting CPU freq
			}
		}
	}
	return 0;
}
