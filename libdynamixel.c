#include "libdynamixel.h"

void get_raw(int max);
uint8_t get_char();

FILE *port;
uint8_t debug;


//Calculations
uint8_t get_checksum(struct DMXPacket *data)
{
	uint8_t sum = 0;
	sum += data->id;
	sum += data->length;
	sum += data->error;
	for(int i = 0; i < data->length - 2; i++)
	{
		sum += data->params[i];
	}
	sum = ~sum;
	return sum;
}

//Low level IO
void get_raw(int max)
{
	for(int i = 0; i < max; i++) printf("%02x\n", fgetc(port));
}

uint8_t get_char()
{
	int c = EOF;
	while(c == EOF) c = fgetc(port);
	return (uint8_t)c;
}

//Print
void print_packet(struct DMXPacket *data)
{
	printf(	"DMX packet:\n"
			"ID = %02x\n"
			"Length = %02x\n"
			"Error / Instruction ID = %02x\n", data->id, data->length, data->error);
	if(data->length > 2)
	{
		printf("Params:");
		for(int i = 0; i < data->length - 2; i++)
		{
			printf(" %02x", data->params[i]);
		}
		printf("\n");
	}
}

void print_errors(uint8_t error)
{
	printf("Errors:\n");
	if(error == 0)
	{
		printf("No errors\n");
	}
	else
	{
		if(error & DMX_ERR_VOLTAGE)
		{
			printf("Input voltage\n");
		}
		if(error & DMX_ERR_ANGLE)
		{
			printf("Angle limit\n");
		}
		if(error & DMX_ERR_OVERHEAT)
		{
			printf("Overheating\n");
		}
		if(error & DMX_ERR_RANGE)
		{
			printf("Range\n");
		}
		if(error & DMX_ERR_CHECKSUM)
		{
			printf("Checksum\n");
		}
		if(error & DMX_ERR_OVERLOAD)
		{
			printf("Overload\n");
		}
		if(error & DMX_ERR_INSTRUCTION)
		{
			printf("Instruction\n");
		}
		if(error & DMX_ERR_RESERVED)
		{
			printf("Unknown/Reserved\n");
		}
	}
}

//Upper level IO
void get_response(struct DMXPacket *data)
{
	if(get_char() == 0xFF)
	{
		if(get_char() == 0xFF)
		{
			data->id = get_char();
			data->length = get_char();
			data->error = get_char();
			for(int i = 0; i < data->length - 2; i++)
			{
				data->params[i] = get_char();
			}
			data->checksum = get_char();
		}
		else if(debug)
		{
			printf("Invalid signature (2nd byte)");
		}
	}
	else if(debug)
	{
		printf("Invalid signature (1st byte)");
	}
	if(debug)
	{
		printf("get_response:\n");
		if(data->checksum != get_checksum(data))
		{
			printf("Checksum error! Following data may be invalid");
		}
		print_packet(data);
		if(data->error)
		{
			printf("\n");
			print_errors(data->error);
		}
		printf("\n\n");
	}
}

void send_packet(struct DMXPacket *data)
{
	if(debug)
	{
		printf("send_packet:\n");
		print_packet(data);
		printf("\n\n");
	}
	fputc(0xFF, port);
	fputc(0xFF, port);
	fputc(data->id, port);
	fputc(data->length, port);
	fputc(data->instruction, port);
	for(int i = 0; i < data->length - 2; i++)
	{
		fputc(data->params[i], port);
	}
	fputc(data->checksum, port);
}

//Commands
int ping(int id)
{
	struct DMXPacket ping;
	ping.id = id;
	ping.length = 2;
	ping.instruction = INSTRUCTION_PING;
	ping.checksum = ~(ping.id + ping.length + ping.instruction);
	if(debug)
	{
		printf("Sending request\n");
	}
	send_packet(&ping);
	if(debug)
	{
		printf("Waiting for response\n");
	}
	get_response(&ping);
	return ping.checksum == get_checksum(&ping);
}


