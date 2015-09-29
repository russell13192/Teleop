
/*
 * $teleop_server.c$
 *
 * K Lawson
 * 28 Sep 2015
 * 
 * 
 * Teleop controller 
 * -----------------
 * This program allows an XBOX 360 controller to controll a 1/10 RC car.
 *  
 * The RC car is controlled over wifi using a wireless controller plugged
 * into a laptop.  This then uses WIFI to communicate with raspberry PI on
 * the car.  
 * 
 * A periodic UDP message is used to send commands to the car and receive
 * status information back from the car.  Accelerometer info fromt he car is then 
 * used to control the dual shock on the hand controller.
 * 
 * The laptop is client and the car is the server.
 * 
 * An XBOX webcam on the car can be accessed on the laptop via a web browser.
 * 
 * The aim is to evaluate the use of XBOX tech for Teleop robotics.
 * 
 * This module is the server; it waits for a connection with the client, goes live then
 * feeds back accelerometer data to the OCU.  This module works in parallel with the 
 * webcam daemon Monitor although they are loosely coupled.
 * 
 * usage
 * -----
 * sudo ./teleop_server
 *
 */

#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

#define T 5 
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

#define GPIO_PORT 18
#define GPIO_FORE 17
#define GPIO_RED_LED 24
#define GPIO_GN_LED 25
//GPIO0/2, GPIO1/3, GPIO4, GPIO7 and GPIO8.

// Define UDP msg type
struct TMsg {
	int	Fore;
	int	Port;
	int	Accel;
	int Wdog;
	int spare[10];
	char endmsg[10];	
}Msg;	

void die(char *s);
void main(void);
void sigalrm_handler(int);



// Main server program.  This is polled by the client so the client controls the 
// update speed.
// 
int main(void)
{
    struct sockaddr_in si_me, si_other;
     
    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];
     
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1){
        die("bind");
    }
    
    
     
    //keep listening for data
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)         {
            die("recvfrom()");
        }
         
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);
         
        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
    }
 
    close(s);
    return 0;
}// End main


// This function sets the GIO on, off or to a PWM value
// 1=on 0=off 0.nn=PWM
//
void GPIO_Set(int channel, float value)
{
	string s = channel + "=" + value + "\n";

	
	printf("%s\n",s);

    write.Write(s);
    write.Flush();
}//END Set

///@@@@@@@@@@@@@@@@@@@@@@@
PWM
namespace PrototypeAP
{
static class PWM
{

    static string fifoName = "/dev/pi-blaster";

    static FileStream file;
    static StreamWriter write;

    static PWM()
    {
        file = new FileInfo(fifoName).OpenWrite();

        write = new StreamWriter(file, Encoding.ASCII);
    }

   
}
}


//@@@@@@@@@@@'SIG ALARM EXAMPLE


int flag = T;



int  main(void)
{
    signal(SIGALRM, sigalrm_handler);   
    alarm(1);                         
    while (1);  
}



// sigalrm_handler, called if we don't get UDP comms
//
void sigalrm_handler(int sig)
{
    GPIO_Disable();
    //alarm(1);
}//END sigalrm_handler




// GPIO_disable
//
void GPIO_disable(int sig)
{
	PiBlast(GPIO_PORT,0);
	PiBlast(GPIO_FORE,0);
	PiBlast(GPIO_RED_LED,0);
	PiBlast(GPIO_GN_LED,1);

}//END sigalrm_handler



// Does what it says...
//   
void die(char *s)
{
    perror(s);
    exit(1);
    
}//END die
