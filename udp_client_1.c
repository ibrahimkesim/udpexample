
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

struct message {
    long message_id;
    char city [100];
    char temperature [16];   // degrees Celcius 
};
char mcc[3];
char mnc[];
int mncindex;
int sockfd;
struct sockaddr_in addr;
char buffer[1024];
socklen_t addr_size;

void *client_send (void *arg);
void *client_receive (void *arg);
int gtpv2_decode_plmnn_id (char *data);
int gtpv2_encode_plmnn_id (char* val1, char* val2);

void error (char *msg);

pthread_t client_send_thread, client_receive_thread;
int sock_fd;
struct addrinfo *rptr;

int main(int argc, char **argv){

  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(0);
  }

  //char *ip = "192.168.0.15";
  char *ip = "127.0.0.1";
  int port = atoi(argv[1]);



  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

      // create threads
    if (pthread_create (&client_receive_thread, NULL, client_receive, NULL) != 0)
        error ("pthread_create");

    // wait for threads
    if (pthread_join (client_receive_thread, NULL) != 0)
        error ("pthread_join");

    close (sock_fd);



  return 0;
}




void *client_receive (void *arg)
{
    
  struct message message;
  ssize_t numbytes;

  bzero(buffer, 1024);
  strcpy(buffer, "Hello, World!");
  sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, sizeof(addr));
  printf("[+]Data send: %s\n", buffer);

    gtpv2_decode_plmnn_id (buffer);
  // bzero(buffer, 1024);
  // addr_size = sizeof(addr);
  // recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, &addr_size);
  // printf("[+]Data recv: %s\n", buffer);
         
}

int gtpv2_decode_plmnn_id (char *data)
{
  int bufindex = 0;

  bzero(buffer, 1024);
  data = buffer;
  addr_size = sizeof(addr);
  recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, &addr_size);
  printf("[+]Data recv: %s\n", buffer);
  //printf(" sizeofbuffer: %ld\n", sizeof(buffer));

     for(int i = 0 ; data[i]!= '\0'; i++){
        bufindex++;
        }
    
     if( data[0] != '[' )
         return -1;
     if( data[bufindex-1] != ']' )
         return -1;

    if(strstr(data, "[PLMN:") != NULL)
    {

      mcc[0] = data[7];
      mcc[1] = data[6];
      mcc[2] = data[9];

      if( data[8] == 'f' ){
        printf("MNC has two digits\n");
        mncindex = 2;
        mnc[0] = data[11];
        mnc[1] = data[10];   
      }
      else{
        printf("MNC has three digits\n");
        mncindex = 3;
        mnc[0] = data[8];
        mnc[1] = data[11];  
        mnc[2] = data[10]; 
      }


      printf("mcc %s\n", mcc);
      printf("mnc %s\n", mnc);

      gtpv2_encode_plmnn_id ( mcc, mnc);

    }

return 0;

}

int gtpv2_encode_plmnn_id (char* val1, char* val2)
{
char PLMN[6] = {0,0,0,0,0,0};
PLMN[0] = val1[1];
PLMN[1] = val1[0];
PLMN[3] = val1[2];

if(mncindex == 2){
    
  PLMN[2] = 'f';
  PLMN[4] = val2[1];
  PLMN[5] = val2[0];

}
else
{

  PLMN[2] = val2[0];;
  PLMN[4] = val2[2];
  PLMN[5] = val2[1];

}

printf("[PLMN:%s]\n", PLMN);

}

void error (char *msg)
{
    perror (msg);
    exit (1);
}