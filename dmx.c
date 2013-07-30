#include "libdynamixel.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc < 3) return 1;
	FILE *port = fopen(argv[1], "a+");
	if(port == NULL)
	{
		printf("Cannot open %s\n", argv[1]);
		return -1;
	}
	initlib(port, TRUE);
	if(!strcmp(argv[2], "ping"))
	{
		if(argc > 3)
		{
			int id = atoi(argv[3]);
			if(!ping(id))
			{
				printf("OK\n");
			}
		}
		else
		{
			printf("Enter servo ID\n");
			return 1;
		}
	}
	else if(!strcmp(argv[2], "readb"))
	{
		if(argc > 4)
		{
			int id = atoi(argv[3]);
			int addr = atoi(argv[4]);
			uint8_t result;
			if(!readb(id, addr, &result))
			{
				printf("%02X\n", result);
			}
		}
		else
		{
			printf("Enter servo ID\n");
			return 1;
		}
	}
	else if(!strcmp(argv[2], "readw"))
	{
		if(argc > 4)
		{
			int id = atoi(argv[3]);
			int addr = atoi(argv[4]);
			uint16_t result;
			if(!readw(id, addr, &result))
			{
				printf("%04X\n", result);
			}
		}
		else
		{
			printf("Enter servo ID\n");
			return 1;
		}
	}
	else if(!strcmp(argv[2], "writeb"))
	{
		if(argc > 5)
		{
			int id = atoi(argv[3]);
			int addr = atoi(argv[4]);
			int value = atoi(argv[5]);
			if(!writeb(id, addr, value))
			{
				printf("OK\n");
			}
		}
		else
		{
			printf("Enter servo ID\n");
			return 1;
		}
	}
	else if(!strcmp(argv[2], "writew"))
	{
		if(argc > 4)
		{
			int id = atoi(argv[3]);
			int addr = atoi(argv[4]);
			int value = atoi(argv[5]);
			if(!writew(id, addr, value))
			{
				printf("OK\n");
			}
		}
		else
		{
			printf("Enter servo ID\n");
			return 1;
		}
	}
	else if(!strcmp(argv[2], "reset"))
	{
		if(argc > 3)
		{
			int id = atoi(argv[3]);
		}
		else
		{
			printf("Enter servo ID\n");
			return 1;
		}
	}
	return 0;
}
