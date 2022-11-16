#include<stdio.h>

#include<Winsock2.h>

#include<ws2tcpip.h>

#include<stdlib.h>

#include<malloc.h>

#include<string.h>

#include <windows.h>

#pragma comment(lib , "Ws2_32.lib")

#define ICMP_ECHO_REQUEST 8 //���������������

#define DEF_ICMP_DATA_SIZE 20 //���巢�����ݳ���

#define DEF_ICMP_PACK_SIZE 32 //�������ݰ�����

#define MAX_ICMP_PACKET_SIZE 1024 //����������ݰ�����

#define DEF_ICMP_TIMEOUT 3000  //���峬ʱΪ3��

#define ICMP_TIMEOUT 11 //ICMP��ʱ����

#define ICMP_ECHO_REPLY 0 //�������Ӧ������

/*

 *IP��ͷ�ṹ

 */

typedef struct

{

    byte h_len_ver ; //IP�汾��

    byte tos ; // ��������

    unsigned short total_len ; //IP���ܳ���

    unsigned short ident ; // ��ʶ

    unsigned short frag_and_flags ; //��־λ

    byte ttl ; //����ʱ��

    byte proto ; //Э��

    unsigned short cksum ; //IP�ײ�У���

    unsigned long sourceIP ; //ԴIP��ַ

    unsigned long destIP ; //Ŀ��IP��ַ

} IP_HEADER ;

/*

 *����ICMP��������

 */

typedef struct _ICMP_HEADER

{

    byte type ; //����-----8

    byte code ; //����-----8

    unsigned short cksum ; //У���------16

    unsigned short id ; //��ʶ��-------16

    unsigned short seq ; //���к�------16

    unsigned int choose ; //ѡ��-------32

} ICMP_HEADER ;

typedef struct

{

    int usSeqNo ; //��¼���к�

    DWORD dwRoundTripTime ; //��¼��ǰʱ��

    byte ttl ; //����ʱ��

    struct in_addr dwIPaddr ; //ԴIP��ַ

} DECODE_RESULT ;

/*

 *��������У���

 */

unsigned short GenerateChecksum(unsigned short *pBuf , int iSize)

{

    unsigned long cksum = 0 ; //��ʼʱ������У��ͳ�ʼ��Ϊ0

    while(iSize > 1)

    {

        cksum += *pBuf++ ; //����У�������ÿ16λ��λ��ӱ�����cksum��

        iSize -= sizeof(unsigned short) ; //ÿ16λ�����򽫴�У����������ȥ16

    }

    //�����У�������Ϊ��������ѭ����֮���轫���һ���ֽڵ�������֮ǰ������

    if(iSize)

    {

        cksum += *(unsigned char*)pBuf ;

    }

        //֮ǰ�Ľ�������˽�λ����Ҫ�ѽ�λҲ�������Ľ����

    cksum = (cksum >> 16) + (cksum & 0xffff) ;

    cksum += (cksum >> 16) ;

    return (unsigned short)(~ cksum) ;

}

/*

 *��pingӦ����Ϣ���н���

 */

boolean DecodeIcmpResponse_Ping(char *pBuf , int iPacketSize , DECODE_RESULT *stDecodeResult)

{

    IP_HEADER *pIpHrd = (IP_HEADER*)pBuf ;

    int iIphedLen = 20 ;

    if(iPacketSize < (int)(iIphedLen + sizeof(ICMP_HEADER)))

    {

        printf("size error! \n") ;

        return 0 ;

    }

    //ָ��ָ��ICMP���ĵ��׵�ַ

    ICMP_HEADER *pIcmpHrd = (ICMP_HEADER*)(pBuf + iIphedLen) ;

    unsigned short usID , usSeqNo ;

    //��õ����ݰ���type�ֶ�ΪICMP_ECHO_REPLY�����յ�һ������Ӧ��ICMP����

    if(pIcmpHrd->type == ICMP_ECHO_REPLY)

    {

        usID = pIcmpHrd->id ;

        //���յ����������ֽ�˳���seq�ֶ���Ϣ �� ��ת��Ϊ�����ֽ�˳��

        usSeqNo = ntohs(pIcmpHrd->seq) ;

    }

    if(usID != GetCurrentProcessId() || usSeqNo != stDecodeResult->usSeqNo)

    {

        printf("usID error!\n") ;

        return 0 ;

    }

    //��¼�Է�������IP��ַ�Լ�����������ʱ��RTT

    if(pIcmpHrd->type == ICMP_ECHO_REPLY)

    {

        stDecodeResult->dwIPaddr.s_addr = pIpHrd->sourceIP ;

        stDecodeResult->ttl = pIpHrd->ttl ;

        stDecodeResult->dwRoundTripTime = GetTickCount() - stDecodeResult->dwRoundTripTime ;

        return 1 ;

    }

    return 0 ;

}

void Ping(char *IP)

{

   unsigned long ulDestIP = inet_addr(IP) ; //��IP��ַת��Ϊ������

   if(ulDestIP == INADDR_NONE)

   {

       //ת�����ɹ�ʱ����������

       HOSTENT *pHostent = gethostbyname(IP) ;

       if(pHostent)

       {

           ulDestIP = (*(IN_ADDR*)pHostent->h_addr).s_addr ; //��HOSTENTת��Ϊ������

       }

       else

       {

           printf("TIMEOUT\n") ;

           return ;

       }

   }

   //���Ŀ��Socket��ַ

   SOCKADDR_IN destSockAddr ; //����Ŀ�ĵ�ַ

   ZeroMemory(&destSockAddr , sizeof(SOCKADDR_IN)) ; //��Ŀ�ĵ�ַ���

   destSockAddr.sin_family = AF_INET ;

   destSockAddr.sin_addr.s_addr = ulDestIP ;

   destSockAddr.sin_port = htons(0);

    //��ʼ��WinSock

    WORD wVersionRequested = MAKEWORD(2,2);

    WSADATA wsaData;

    if(WSAStartup(wVersionRequested,&wsaData) != 0)

    {

        printf("��ʼ��WinSockʧ�ܣ�\n") ;

        return ;

    }

   //ʹ��ICMPЭ�鴴��Raw Socket

   SOCKET sockRaw = WSASocket(AF_INET , SOCK_RAW , IPPROTO_ICMP , NULL , 0 , WSA_FLAG_OVERLAPPED) ;

   if(sockRaw == INVALID_SOCKET)

   {

       printf("����Socketʧ�� !\n") ;

       return ;

   }

   //���ö˿�����

   int iTimeout = DEF_ICMP_TIMEOUT ;

   if(setsockopt(sockRaw , SOL_SOCKET , SO_RCVTIMEO , (char*)&iTimeout , sizeof(iTimeout)) == SOCKET_ERROR)

   {

         printf("���ò���ʧ�ܣ�\n") ;

         return ;

   }

   if(setsockopt(sockRaw , SOL_SOCKET , SO_SNDTIMEO , (char*)&iTimeout , sizeof(iTimeout)) == SOCKET_ERROR)

   {

         printf("���ò���ʧ�ܣ�\n") ;

         return ;

   }

   //���巢�͵����ݶ�

   char IcmpSendBuf[DEF_ICMP_PACK_SIZE] ;

   //���ICMP���ݰ������ֶ�

   ICMP_HEADER *pIcmpHeader  = (ICMP_HEADER*)IcmpSendBuf;

   pIcmpHeader->type = ICMP_ECHO_REQUEST ;

   pIcmpHeader->code = 0 ;

   pIcmpHeader->id = (unsigned short)GetCurrentProcessId() ;

   memset(IcmpSendBuf + sizeof(ICMP_HEADER) , 'E' , DEF_ICMP_DATA_SIZE) ;

   //ѭ�������ĸ��������icmp���ݰ�

   int usSeqNo = 0 ;

   DECODE_RESULT stDecodeResult ;

   while(usSeqNo <= 3)

   {

     pIcmpHeader->seq = htons(usSeqNo) ;

     pIcmpHeader->cksum = 0 ;

     pIcmpHeader->cksum = GenerateChecksum((unsigned short*)IcmpSendBuf , DEF_ICMP_PACK_SIZE) ; //����У��λ

     //��¼���кź͵�ǰʱ��

     stDecodeResult.usSeqNo = usSeqNo ;

     stDecodeResult.dwRoundTripTime = GetTickCount() ;

     //����ICMP��EchoRequest���ݰ�

     if(sendto(sockRaw , IcmpSendBuf , DEF_ICMP_PACK_SIZE , 0 , (SOCKADDR*)&destSockAddr , sizeof(destSockAddr)) == SOCKET_ERROR)

     {

        //���Ŀ���������ɴ���ֱ���˳�

        if(WSAGetLastError() == WSAEHOSTUNREACH)

        {

            printf("Ŀ���������ɴ\n") ;

            exit(0) ;

        }

     }

     SOCKADDR_IN from ;

     int iFromLen = sizeof(from) ;

     int iReadLen ;

     //������յ����ݰ�

     char IcmpRecvBuf[MAX_ICMP_PACKET_SIZE] ;

     while(1)

     {

         iReadLen = recvfrom(sockRaw , IcmpRecvBuf , MAX_ICMP_PACKET_SIZE , 0 , (SOCKADDR*)&from , &iFromLen) ;

         if(iReadLen != SOCKET_ERROR)

         {

             if(DecodeIcmpResponse_Ping(IcmpRecvBuf , sizeof(IcmpRecvBuf) , &stDecodeResult))

             {

                printf("���� %s �Ļظ�: �ֽ� = %d ʱ�� = %dms TTL = %d\n" , inet_ntoa(stDecodeResult.dwIPaddr) ,

                         iReadLen - 20,stDecodeResult.dwRoundTripTime ,stDecodeResult.ttl) ;

             }

             break ;

         }

         else if(WSAGetLastError() == WSAETIMEDOUT)

         {

             printf("time out !  *****\n") ;

             break ;

         }

         else

         {

             printf("����δ֪����\n") ;

             break ;

         }

     }

     usSeqNo++ ;

   }

   //�����Ļ��Ϣ

   printf("Ping complete...\n") ;

   closesocket(sockRaw) ;

   WSACleanup() ;

}

int main()

{

   char  com[10] , IP[20] ;

   while(1){

   printf("command>>") ;

   scanf("%s %s" , com , IP) ;

   if(strcmp(com , "ping") == 0)

   {

       Ping(IP) ;

   }

   else

   {

       printf("������� ! \n") ;

   }

   }

   return 0 ;

}