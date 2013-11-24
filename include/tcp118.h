# ifndef TCP118_H
# define TCP118_H

#include <stdio.h>
#include <sys/types.h>   // definitions of a number of data types used in socket.h and netinet/in.h
#include <sys/socket.h>  // definitions of structures needed for sockets, e.g. sockaddr
#include <netinet/in.h>  // constants and structures needed for internet domain addresses, e.g. sockaddr_in
#include <stdlib.h>
#include <strings.h>
#include <sys/wait.h>	/* for the waitpid() system call */
#include <signal.h>	/* signal name macros, and the kill() prototype */
#include <string.h>
#include <assert.h>
#include <time.h>

#define PACKET_SIZE 1024
#define HEADER_SIZE 12
#define MAX_BODY_SIZE 1012
#define TIME_OUT 60
#define C_WND 4096

#define SEQ_NUM_OFFSET 	0
#define ACK_NUM_OFFSET 	32
#define ACK_OFFSET 		64
#define LAST_OFFSET		65
#define SIZE_OFFSET		70
#define CHECKSUM_OFFSET	80
#define BODY_OFFSET     96

typedef struct {
	uint32_t seq_num;
	uint32_t ack_num;
	uint16_t bits_and_size;
	uint16_t checksum;
	char *	 str;
} packet_t;

typedef struct {
	uint32_t c_wnd;
	uint32_t size;
	uint32_t next_seq;
	uint32_t next_mss;
	uint8_t *acks;
	packet_t * packets;
} cwnd_t;

uint16_t checksum(const uint8_t * addr, uint32_t count);

uint32_t getSeqNum(const char * pkt);
void setSeqNum(char * pkt, uint32_t seqNum);
uint32_t getACKNum(const char * pkt);
void setACKNum(char * pkt, uint32_t ACKNum);
uint16_t getACK(const char * pkt);
void setACK(char * pkt, uint16_t ACK);
uint16_t getLast(const char * pkt);
void setLast(char * pkt, uint16_t last);
char * getData(const char * pkt);
int setData(char * pkt, char * buff, size_t count);

packet_t * generatePacket(uint32_t seq_num, 
					   uint32_t ack_num, 
					   uint8_t ack, 
					   uint8_t last,
					   const char * buff,
					   size_t count);

int writeTCP(int * file_p, const char * buf, size_t nbytes);
char** strToPackets(const char * file_s);
#endif //TCP118_H