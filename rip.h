#ifndef __MYRIP_H
#define __MYRIP_H
#include <stdio.h>  
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <pthread.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

#define RIP_VERSION    	2
#define RIP_REQUEST    	1
#define RIP_RESPONSE   	2
#define RIP_INFINITY  	16
#define RIP_PORT		520
#define RIP_PACKET_HEAD	4
#define RIP_MAX_PACKET  504
#define RIP_MAX_ENTRY   25
#define ROUTE_MAX_ENTRY 256
#define RIP_GROUP		"224.0.0.9"

#define RIP_CHECK_OK    1
#define RIP_CHECK_FAIL  0
#define AddRoute        24
#define DelRoute        25

typedef struct RipEntry		//RIP报文表项的结构体
{
	unsigned short usFamily; //地址族标识
	unsigned short usTag; //路由标记
	struct in_addr stAddr;	//ip地址
	struct in_addr stPrefixLen; //子网掩码
	struct in_addr stNexthop;	//下一跳地址
	unsigned int uiMetric;	//度量值metric
}TRipEntry;

typedef struct  RipPacket	//RIP报文的头部
{
	unsigned char ucCommand;
	unsigned char ucVersion;
	unsigned short usZero; 
	TRipEntry RipEntries[RIP_MAX_ENTRY];
}TRipPkt;


typedef struct RouteEntry	//自己构建RIP路由表
{
	struct RouteEntry *pstNext;
	struct in_addr stIpPrefix; //ip地址
	unsigned int  uiPrefixLen;	//掩码
	struct in_addr stNexthop;	//下一跳
	unsigned int   uiMetric;
	char  *pcIfname;
}TRtEntry;

typedef struct SockRoute	//发送给转发引擎结构体
{
	unsigned int uiPrefixLen;
	struct in_addr stIpPrefix;
	unsigned int uiIfindex;
	struct in_addr stNexthop;
	unsigned int uiCmd;
}TSockRoute;

void route_SendForward(unsigned int uiCmd,TRtEntry *pstRtEntry);
void requestpkt_Encapsulate();
void rippacket_Receive();
void rippacket_Send(struct in_addr stSourceIp);
void rippacket_Multicast(char *pcLocalAddr);
void request_Handle(struct in_addr stSourceIp);
void response_Handle(struct in_addr stSourceIp);
void rippacket_Update();
void routentry_Insert();
void localinterf_GetInfo();
void ripdaemon_Start();

#endif

