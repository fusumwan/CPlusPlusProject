#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "emulator.h"
#include "sr.h"

/*

GoBackN Reliable Transmitter

Description: This program implements the Go-Back-N ARQ protocol, ensuring reliable data transfer over networks with packet ordering, corruption handling, and loss recovery capabilities. It optimizes network communication efficiency.
*/


/* ******************************************************************
 Go Back N protocol.  Adapted from
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose
 
 Network properties:
 - one way network delay averages five time units (longer if there
 are other messages in the channel for GBN), but can be larger
 - packets can be corrupted (either the header or the data portion)
 or lost, according to user-defined probabilities
 - packets will be delivered in the order in which they were sent
 (although some can be lost).
 
 Modifications (6/6/2008 - CLP):
 - removed bidirectional GBN code and other code not used by prac.
 - fixed C style to adhere to current programming style
 (24/3/2013 - CLP)
 - added GBN implementation
 **********************************************************************/

#define RTT  15.0       /* round trip time.  MUST BE SET TO 15.0 when submitting assignment */
#define WINDOWSIZE 6    /* Maximum number of buffered unacked packet */
#define SEQSPACE 7      /* min sequence space for GBN must be at least windowsize + 1 */
#define SEQMAX 10
#define NOTINUSE (-1)   /* used to fill header fields that are not being used */








struct node_struct
{
    struct pkt data;
    struct node_struct *next;
};


typedef struct node_struct *int_stack;

static size_t tot_mem = 0;


void ptk_stack_init(int_stack *s)
{
    *s = NULL;
}


int ptk_stack_push(int_stack *s, struct pkt val)
{
    struct node_struct *newnode =
    (struct node_struct *)malloc(sizeof(struct node_struct));
    if(newnode == NULL) return 0;
    tot_mem += sizeof(struct node_struct);
    newnode->data = val;
    newnode->next = *s;
    *s = newnode;
    
    /* Worked. */
    return 1;
}

int ptk_stack_pop(int_stack *s, struct pkt *val)
{
    struct node_struct *node;
    if(*s == NULL) return 0;
    node = *s;
    *s = (*s)->next;
    *val = node->data;
    free(node);
    tot_mem -= sizeof(struct node_struct);
    return 1;
}

struct pkt ptk_stack_top(const int_stack *s)
{
    struct pkt tmp;
    tmp.acknum=-1;
    tmp.seqnum=-1;
    tmp.checksum=-1;
    if(*s == NULL)
        return tmp;
    else
        return (*s)->data;
}


int ptk_stack_empty(const int_stack *s)
{
    return *s == NULL;
}

int ptk_stack_search(int_stack *Head,struct pkt item){
    
    int pos=0;
    struct node_struct *zombie,             /* Node about to be freed. */
    *scan;               /* Scan list of nodes. */
    
    /* Get rid of all the nodes, and count 'em. */
    scan = *Head;
    while(scan != NULL)
    {
        /* Remember where we are before we split. */
        zombie = scan;
        if(scan->data.acknum==item.acknum){
            return pos;
        }
        scan = scan->next;
        
        pos+=1;
    }
    return -1;
}


void ptk_stack_tini(int_stack *s)
{
    struct node_struct *node,
    *scan;
    
    scan = *s;
    while(scan != NULL)
    {
        node = scan;
        scan = scan->next;
        free(node);
        tot_mem -= sizeof(struct node_struct);
    }
    
    *s = NULL;
}
size_t int_stack_mem_usage(void)
{
    return tot_mem;
}

/* generic procedure to compute the checksum of a packet.  Used by both sender and receiver
 the simulator will overwrite part of your packet with 'z's.  It will not overwrite your
 original checksum.  This procedure must generate a different checksum to the original if
 the packet is corrupted.
 */
int ComputeChecksum(struct pkt packet)
{
    int checksum = 0;
    int i;
    
    checksum = packet.seqnum;
    checksum += packet.acknum;
    for ( i=0; i<20; i++ )
        checksum += (int)(packet.payload[i]);
    
    return checksum;
}

bool IsCorrupted(struct pkt packet)
{
    if (packet.checksum == ComputeChecksum(packet))
        return (false);
    else
        return (true);
}


/********* Sender (A) variables and functions ************/

static struct pkt buffer[WINDOWSIZE];  /* array for storing packets waiting for ACK */
static int windowfirst, windowlast;    /* array indexes of the first/last packet awaiting ACK */
static int totalsend;

static int windowcount;                /* the number of packets currently awaiting an ACK */
static int A_expectedseqnum; /* the sequence number expected next by the sender */

static int A_nextseqnum;               /* the next sequence number to be used by the sender */
static int_stack lost_ack_list;
static int cmdnum;
/* called from layer 5 (application layer), passed the message to be sent to other side */
void A_output(struct msg message)
{

    struct pkt sendpkt;
    int i;

    /* if not blocked waiting on ACK */
    if ( (windowcount+ALostAckWaiting()+packets_resent) < WINDOWSIZE) {
        
        
        if (TRACE > 1)
            printf("----A: New message arrives, send window is not full, send new messge to layer3!\n");
        /* create packet */
        sendpkt.seqnum = A_nextseqnum;
        sendpkt.acknum = NOTINUSE;
        for ( i=0; i<20 ; i++ )
            sendpkt.payload[i] = message.data[i];
        sendpkt.checksum = ComputeChecksum(sendpkt);
        
        /* put packet in window buffer */
        /* windowlast will always be 0 for alternating bit; but not for GoBackN */
        windowlast = (windowlast + 1) % WINDOWSIZE;
        buffer[windowlast] = sendpkt;
        windowcount+=1;
        
        /* send out packet */
        if (TRACE > 0)
            printf("Sending packet %d to layer 3\n", sendpkt.seqnum);
        tolayer3 (A, sendpkt);
        totalsend+=1;
        
        /* start timer if first packet in window */
        if (windowcount == 1)
            starttimer(A,RTT);
        
        /* get next sequence number, wrap back to 0 */
        A_nextseqnum = (A_nextseqnum + 1) % SEQMAX;
    }
    /* if blocked,  window is full */
    else {
        if (TRACE > 0)
            printf("----A: New message arrives, send window is full\n");
        window_full++;
    }
}


/* called from layer 3, when a packet arrives for layer 4
 In this practical this will always be an ACK as B never sends data.
 */
void A_input(struct pkt packet)
{
    int i;
    int pktindex=-1;
    /* if received ACK is not corrupted */
    if (!IsCorrupted(packet)) {
        if (TRACE > 0)
            printf("----A: uncorrupted ACK %d is received\n",packet.acknum);
        
        total_ACKs_received++;
        
        /* check if new ACK or duplicate */
        
        if (windowcount != 0) {
            int seqfirst = buffer[windowfirst].seqnum % WINDOWSIZE;
            int seqlast = buffer[windowlast].seqnum % WINDOWSIZE;
            int acknum = packet.acknum % WINDOWSIZE;
            /* check case when seqnum has and hasn't wrapped */
            if (((seqfirst <= seqlast) && (acknum >= seqfirst && acknum <= seqlast)) ||
                ((seqfirst > seqlast) && (acknum >= seqfirst || acknum<= seqlast)) || AFindPacket(packet.acknum,-2)>=0) {
                windowcount-=1;
                /* packet is a new ACK */
                /*
                if (TRACE > 0)
                    printf("----A: ACK %d is not a duplicate\n",packet.acknum);
                */
                
                int stock_len=(int)int_stack_mem_usage();
                if (TRACE > 0)
                    printf("----A: ACK %d is not a duplicate\n",packet.acknum);
                
                new_ACKs+=1;
                
                
                
                
                /* start timer again if there are still more unacked packets in window */
                if(packet.acknum>WINDOWSIZE && (totalsend-new_ACKs-ALostAckWaiting())==ALostAckWaiting() && ALostAckWaiting()>=1){
                    if(windowcount<0)
                        windowcount=0;
                }
                else if(packet.acknum>WINDOWSIZE && (totalsend-new_ACKs-ALostAckWaiting())==ALostAckWaiting() && ALostAckWaiting()==0){
                    
                    if(ALostAckWaiting()==0){
                        stoptimer(A);
                    }

                }else if(packet.acknum==A_expectedseqnum ||
                   AFindPacket(packet.acknum,-2)>=0 ||
                   (
                    packet.acknum!=A_expectedseqnum &&
                    AFindPacket(packet.acknum,-2)<0 &&
                    ALostAckWaiting()==0 &&
                    windowcount>0 &&
                    (totalsend-packets_resent-new_ACKs)<=0
                    )){
                    if(stock_len<=0 || AFindPacketBySeq(packet.acknum)>=0){
                        /* slide window by the number of packets ACKed */
                        windowfirst = (windowfirst + 1) % WINDOWSIZE;
                        if(
                           (ALostAckWaiting()==0 && packet.acknum==A_expectedseqnum) || AFindPacket(packet.acknum,-2)>=0 ||
                           (
                            packet.acknum!=A_expectedseqnum &&
                            AFindPacket(packet.acknum,-2)<0 &&
                            ALostAckWaiting()==0 &&
                            windowcount>0 &&
                            (totalsend-packets_resent-new_ACKs)<=0
                            )
                           ){
                            stoptimer(A);
                            if (windowcount > 0)
                                starttimer(A, RTT);
                            
                        }
                        
                        pktindex=AFindPacket(packet.acknum,-2);
                        if(pktindex>=0)
                            buffer[pktindex].acknum=-1;
                    }

                }
                else{
                    if(AFindPacket(A_expectedseqnum,-2)<0){
                        struct pkt ackpkt;
                        pktindex=AFindPacket(A_expectedseqnum,-1);
                        ackpkt.seqnum = A_expectedseqnum;
                        ackpkt.acknum = -1;
                        buffer[pktindex].acknum=-2;
                        for ( i=0; i<20 ; i++ )
                            ackpkt.payload[i] = buffer[pktindex].payload[i];
                        ackpkt.checksum = ComputeChecksum(ackpkt);
                        ptk_stack_push(&lost_ack_list, ackpkt);
                        
                    }
                    A_expectedseqnum=packet.acknum;
                }
                
                pktindex=AFindPacketBySeq(packet.acknum);
                if(pktindex>=0){
                    buffer[pktindex].acknum=-3;
                }
                
                

                A_expectedseqnum = (A_expectedseqnum + 1) % SEQMAX;
            }
            else{
                if(totalsend-total_ACKs_received-packets_resent<=0){
                    if (TRACE > 0)
                        printf ("----A: duplicate ACK received, do nothing!\n");
                }
            }
        }
        else
            if (TRACE > 0)
                printf ("----A: duplicate ACK received, do nothing!\n");
    }
    else
        if (TRACE > 0)
            printf ("----A: corrupted ACK is received, do nothing!\n");
}

/* called when A's timer goes off */
void A_timerinterrupt(void)
{

    struct pkt ackpkt;
    ackpkt.acknum=-1;
    if(!ptk_stack_empty(&lost_ack_list)){
        ptk_stack_pop(&lost_ack_list, &ackpkt);
        windowfirst=AFindPacketBySeq(ackpkt.seqnum);
        
        if (TRACE > 0)
            printf("----A: time out,resend packets!\n");
        
        if (TRACE > 0)
            printf ("---A: resending packet %d\n", ackpkt.seqnum);
        ackpkt.checksum=ComputeChecksum(ackpkt);
        tolayer3(A,ackpkt);
        packets_resent++;
        starttimer(A,RTT);
        
    }
    else if(windowcount>0){
        if(buffer[windowfirst].acknum!=-3){
            if (TRACE > 0)
                printf("----A: time out,resend packets!\n");
            
            if (TRACE > 0)
                printf ("---A: resending packet %d\n", (buffer[windowfirst]).seqnum);
            buffer[windowfirst].acknum=-1;
            buffer[windowfirst].checksum=ComputeChecksum(buffer[windowfirst]);
            tolayer3(A,buffer[windowfirst]);
            windowfirst=AFindPacketBySeq(buffer[windowfirst].seqnum);
            packets_resent++;
            starttimer(A,RTT);
        }
        else{
            int pktindex=AFindPacketByACK(-1);
            if(pktindex>=0){
                if (TRACE > 0)
                    printf("----A: time out,resend packets!\n");
                if (TRACE > 0){
                    printf ("---A: resending packet %d\n", (buffer[pktindex]).seqnum);
                }
                
                buffer[pktindex].acknum=-1;
                buffer[pktindex].checksum=ComputeChecksum(buffer[pktindex]);
                tolayer3(A,buffer[pktindex]);
                packets_resent++;
                starttimer(A,RTT);
            }
        }
    }
}

int ALostAckWaiting(){
    int i=0;
    int count=0;
    for(i=0;i<WINDOWSIZE;i++){
        if(buffer[i].acknum==-2)
        {
            count+=1;
        }
    }
    return count;
}
int AFindPacket(int seqnum,int acknum){
    int i=0;
    for(i=0;i<WINDOWSIZE;i++){
        if(buffer[i].seqnum==seqnum && buffer[i].acknum==acknum)
        {
            return i;
        }
    }
    return -1;
}

int AFindPacketBySeq(int seqnum){
    int i=0;
    for(i=0;i<WINDOWSIZE;i++){
        if(buffer[i].seqnum==seqnum)
        {
            return i;
        }
    }
    return -1;
}

int AFindPacketByACK(int acknum){
    int i=0;
    for(i=0;i<WINDOWSIZE;i++){
        if(buffer[i].acknum==acknum)
        {
            return i;
        }
    }
    return -1;
}
int APrintPacketByACK(){
    printf("ACK LIST [");
    int i=WINDOWSIZE;
    for(i=WINDOWSIZE-1;i>=0;i--){
        printf("%d:%d,",i,buffer[i].acknum);
    }
    printf("] ");
    return -1;
}



/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init(void)
{
    /* initialise A's window, buffer and sequence number */
    A_expectedseqnum = 0;
    A_nextseqnum = 0;  /* A starts with seq num 0, do not change this */
    windowfirst = 0;
    windowlast = -1;   /* windowlast is where the last packet sent is stored.
                        new packets are placed in winlast + 1
                        so initially this is set to -1
                        */
    ptk_stack_init(&lost_ack_list);
    windowcount = 0;
    cmdnum=0;
    totalsend=0;
}



/********* Receiver (B)  variables and procedures ************/

static int B_expectedseqnum; /* the sequence number expected next by the receiver */
static int B_nextseqnum;   /* the sequence number for the next packets sent by B */
static struct pkt B_buffer[WINDOWSIZE];  /* array for storing packets waiting for ACK */
static int B_windowlast;/* array indexes of the first/last packet awaiting ACK */


int FindPacket(struct pkt packet){
    int i=0;
    for(i=0;i<WINDOWSIZE;i++){
        if(
           B_buffer[i].acknum==packet.acknum &&
           B_buffer[i].seqnum==packet.seqnum &&
           B_buffer[i].checksum==packet.checksum
           )
        {
            return i;
        }
    }
    return -1;
}

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
    struct pkt sendpkt;
    int i;
    int findpdk=-1;
    bool sendack=true;
    /* if not corrupted and received packet is in order */
    if  (!IsCorrupted(packet)) {
        if (TRACE > 0)
            printf("----B: packet %d is correctly received, send ACK!\n",packet.seqnum);
        packets_received++;
        findpdk=FindPacket(packet);
        if(findpdk==-1){
            /* deliver to receiving application */
            tolayer5(B, packet.payload);
            B_buffer[B_windowlast] = packet;
            B_windowlast = (B_windowlast + 1) % WINDOWSIZE;
        }
        
        if(packet.seqnum == B_expectedseqnum && findpdk==-1){
            /* send an ACK for the received packet */
            sendpkt.acknum = B_expectedseqnum;
            /* update state variables */
            B_expectedseqnum = (B_expectedseqnum + 1) % SEQMAX;
        }else if(packet.seqnum == B_expectedseqnum && findpdk>-1){
            sendpkt.seqnum=B_buffer[findpdk].acknum;
            sendpkt.acknum=B_buffer[findpdk].seqnum;
        }
        else{
            sendpkt.seqnum = packet.acknum;
            sendpkt.acknum = packet.seqnum;
            
        }
    }
    else {
        sendack=false;
        findpdk=FindPacket(packet);
    }
    if(findpdk==-1){
        /* create packet */
        sendpkt.seqnum = B_nextseqnum;
        B_nextseqnum = (B_nextseqnum + 1) % 2;
    }
    if(sendack){
        /* we don't have any data to send.  fill payload with 0's */
        for ( i=0; i<20 ; i++ )
            sendpkt.payload[i] = '0';
        
        /* computer checksum */
        sendpkt.checksum = ComputeChecksum(sendpkt);
        
        /* send out packet */
        tolayer3 (B, sendpkt);
    }
    
}

/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init(void)
{
    B_expectedseqnum = 0;
    B_nextseqnum = 1;
    windowfirst = 0;
    windowlast = -1;   /* windowlast is where the last packet sent is stored.
                        new packets are placed in winlast + 1
                        so initially this is set to -1
                        */
}

/******************************************************************************
 * The following functions need be completed only for bi-directional messages *
 *****************************************************************************/

/* Note that with simplex transfer from a-to-B, there is no B_output() */
void B_output(struct msg message)
{
    
}

/* called when B's timer goes off */
void B_timerinterrupt(void)
{
}
