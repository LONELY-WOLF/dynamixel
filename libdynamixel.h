#ifndef _LIB_DYNAMIXEL_H
#define _LIB_DYNAMIXEL_H

#include <stdint.h>
#include <stdio.h>

#define DMX_PING		0x01
#define DMX_READ		0x02
#define DMX_WRITE		0x03
#define DMX_REG_WRITE	0x04
#define DMX_ACTION		0x05
#define DMX_RESET		0x06

#define DMX_ERR_VOLTAGE		0x01
#define DMX_ERR_ANGLE		0x02
#define DMX_ERR_OVERHEAT	0x04
#define DMX_ERR_RANGE		0x08
#define DMX_ERR_CHECKSUM	0x10
#define DMX_ERR_OVERLOAD	0x20
#define DMX_ERR_INSTRUCTION	0x40
#define DMX_ERR_RESERVED	0x80

#define TRUE	1
#define FALSE	0

struct DMXPacket
{
	uint8_t id;
	uint8_t length;
	union
	{
		uint8_t instruction;
		uint8_t error;
	};
	uint8_t params[32];
	uint8_t checksum;
};

//extern FILE *port;
//extern uint8_t debug;

void initlib(FILE *p_port, uint8_t p_debug);
uint8_t get_checksum(struct DMXPacket *data);
//void get_raw(int max);
//uint8_t get_char();
void print_packet(struct DMXPacket *data);
void get_response(struct DMXPacket *data);
void send_packet(struct DMXPacket *data);
int ping(int id);
int readb(int id, int addr, uint8_t *result);
int readw(int id, int addr, uint16_t *result);
int writeb(int id, int addr, uint8_t value);
int writew(int id, int addr, uint16_t value);
int reset(int id);
#endif
