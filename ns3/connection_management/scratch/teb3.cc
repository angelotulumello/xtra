#include <iostream>
#include <fstream>
#include <string>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/traffic-control-helper.h"
#include "ns3/traffic-control-layer.h"

using namespace ns3;


/**********************************/
/*       Various parameters       */
/*                                */
/*  To vary them, move them to    */
/*  the code for cmd-parsing      */
/**********************************/

#define TEB_ACTIVE 1
// only turn SACK on when not doing TEB!
#define DOSACK false

#define SIMDURATION 4.

// 1446 yields packets of 1500 bytes.
// 1448 is the max for default pointtopoint links, yielding packets of size 1502

// !!! NOTE: this number is hardcoded in my seq plotting script
#define TCPMSS 946

//#define TCPMSS 1446
#define PACKETSIZE (TCPMSS+54)
// this is in segments. 3 is a normal default, or 10 for Linux, not 1 like in ns-3
#define TCPINITCWND 3

// in units of TCPMSS !  e.g. 3 = 3 TCP packets
#define APPBLOCKSIZE 30
// number of app blocks to send in total
#define APPNUMBLOCKS 10000
#define APPRATE "1Gbps"

// packets to drop; to drop none, make this empty
// Index from 0; the sender sends SYN, then ACK, then data packets
// So data packets begin at number 2
// E.g. to drop the first and last from IW10: 2, 11
const int PACKETS_TO_DROP[] = {};



// BDP: 800kbps * 120ms = 800000/8 * 0.12 = 12000 byte.
// packets are 1000 byte each, so that's 12 packets.
// if we give the queue a length of 12, cwnd should reach 24 packets (24000 bytes).


// access links have 1Gbps, 1ms, so the total OWD becomes 2ms + BOTTLENECK_DELAY
// ... and the RTT is 4ms + 2*BOTTLENECK_DELAY
//#define BOTTLENECK_CAPACITY "1Mbps"
#define BOTTLENECK_CAPACITY "0.8Mbps"
#define BOTTLENECK_DELAY "58ms"


// #define USE_HELPER
#define QLEN 4
//#define QLEN 12


/**********************************/
/*      MyApp (from fifth.cc)     */
/*                                */
/*  This can be useful to emulate */
/*  app-limited senders (perhaps) */
/**********************************/

class MyApp : public Application
{
    public:
    
    MyApp ();
    virtual ~MyApp();
    
    void Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate);
    
    private:
    virtual void StartApplication (void);
    virtual void StopApplication (void);
    
    void ScheduleTx (void);
    void SendPacket (void);
    
    Ptr<Socket>     m_socket;
    Address         m_peer;
    uint32_t        m_packetSize;
    uint32_t        m_nPackets;
    DataRate        m_dataRate;
    EventId         m_sendEvent;
    bool            m_running;
    uint32_t        m_packetsSent;
};

MyApp::MyApp ()
: m_socket (0),
m_peer (),
m_packetSize (0),
m_nPackets (0),
m_dataRate (0),
m_sendEvent (),
m_running (false),
m_packetsSent (0)
{
}

MyApp::~MyApp()
{
    m_socket = 0;
}

void
MyApp::Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate)
{
    m_socket = socket;
    m_peer = address;
    m_packetSize = packetSize;
    m_nPackets = nPackets;
    m_dataRate = dataRate;
}

void
MyApp::StartApplication (void)
{
    m_running = true;
    m_packetsSent = 0;
    m_socket->Bind ();
    m_socket->Connect (m_peer);
    SendPacket ();
}

void
MyApp::StopApplication (void)
{
    m_running = false;
    
    if (m_sendEvent.IsRunning ())
    {
        Simulator::Cancel (m_sendEvent);
    }
    
    if (m_socket)
    {
        m_socket->Close ();
    }
}

void
MyApp::SendPacket (void)
{
    Ptr<Packet> packet = Create<Packet> (m_packetSize);
    m_socket->Send (packet);
    
    if (++m_packetsSent < m_nPackets)
    {
        ScheduleTx ();
    }
}

void
MyApp::ScheduleTx (void)
{
    if (m_running)
    {
        Time tNext (Seconds (m_packetSize * 8 / static_cast<double> (m_dataRate.GetBitRate ())));
        m_sendEvent = Simulator::Schedule (tNext, &MyApp::SendPacket, this);
    }
}


/**********************************/
/*      CALLBACKS (for tracing    */
/**********************************/

static void
CwndChange (Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
{
    // this writes it to stderr...
    //    NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "\t" << newCwnd);
    *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << newCwnd <<  "\t" << (newCwnd/TCPMSS) << std::endl;
}

/*static void
QlenChange (Ptr<OutputStreamWrapper> stream, uint32_t oldQ, uint32_t newQ)
{
    *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << newQ << std::endl;
}
*/

/****************************************************/
/****************************************************/
/****************************************************/

int main (int argc, char *argv[])
{
    //int tcpSegmentSize = 1000-54;
    //Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpNewReno"));
    //    Config::SetDefault ("ns3::PointToPointNetDevice::Mtu", UintegerValue (1500));
    Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (TCPMSS));
    Config::SetDefault ("ns3::TcpSocket::InitialCwnd", UintegerValue (TCPINITCWND));
    Config::SetDefault ("ns3::TcpSocket::DelAckTimeout", TimeValue (Time("0ms")));
    Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (0));
    Config::SetDefault ("ns3::TcpSocketBase::Sack", BooleanValue (DOSACK));
    //Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpReno"));
    //  Config::SetDefault ("ns3::RttEstimator::MinRTO", TimeValue (MilliSeconds (500)));
    //    Config::SetDefault ("ns3::PfifoFastQueueDisc::Limit", UintegerValue (QLEN) );
    
    // no teb socket for us
    //Config::SetDefault ("ns3::TcpSocketBase::TebSocket", UintegerValue (TEB_ACTIVE));

    uint32_t    nLeaf = 1; // If non-zero, number of both left and right
    // add more parameters here, e.g. replacing the defines above, to vary them
    CommandLine cmd;
    cmd.AddValue ("nLeaf",     "Number of left and right side leaf nodes", nLeaf);
    cmd.Parse (argc,argv);

    
    
    /****************************************************/
    // Create topology
    /****************************************************/
    
    PointToPointHelper pointToPointRouter;
    pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue (BOTTLENECK_CAPACITY));
    pointToPointRouter.SetChannelAttribute ("Delay", StringValue (BOTTLENECK_DELAY));
    pointToPointRouter.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue(QLEN));

    PointToPointHelper pointToPointLeaf;
    //    pointToPointLeaf.SetDeviceAttribute    ("DataRate", StringValue ("1Gbps"));
    pointToPointLeaf.SetDeviceAttribute    ("DataRate", StringValue ("1Gbps"));
    pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("1ms"));
    

    PointToPointDumbbellHelper d (nLeaf, pointToPointLeaf,
                                  nLeaf, pointToPointLeaf,
                                  pointToPointRouter);

    // Install Stack
    InternetStackHelper stack;
    d.InstallStack (stack);
    
    // Assign IP Addresses
    d.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"),
                           Ipv4AddressHelper ("10.2.1.0", "255.255.255.0"),
                           Ipv4AddressHelper ("10.3.1.0", "255.255.255.0"));

    
    // finish off the address assignment + static routing job
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    
    /****************************************************/
    // Initialize TCP sources and sinks
    /****************************************************/
    
    uint16_t sinkPort = 8080;
    Address sinkAddress (InetSocketAddress (d.GetRightIpv4Address (0), sinkPort));
    PacketSinkHelper sinkA ("ns3::TcpSocketFactory", sinkAddress);
    ApplicationContainer sinkAppA = sinkA.Install (d.GetRight (0));
    
    Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (d.GetLeft (0), TcpSocketFactory::GetTypeId ());

    /****************************************************/
    /* TCP XFSM configuration (sender side)             */
    /****************************************************/

    /*~~~~~~~~~~~~~~~~~~~ Conditions ~~~~~~~~~~~~~~~~~~~*/
    // available window > 0
    Condition c1 = Condition(XFSM_AVAILABLE_WIN, XFSM_ZERO, 0, XFSM_GREATER);
    // is dupack?
    Condition c2 = Condition(XFSM_LASTACKEDSEQ, XFSM_CONTEXT_PARAM0, 0, XFSM_EQUAL);

    // timerEntrySeqNo < lastAckedSeq ?
    Condition c3 = Condition(XFSM_CONTEXT_PARAM0, XFSM_LASTACKEDSEQ, 0, XFSM_LESS);
    // currentReTXRound < timerEntryReTXCount ?
    Condition c4 = Condition(XFSM_CUSTOM0, XFSM_CONTEXT_PARAM1, 0, XFSM_LESS);
    // timerEntrySeqNo >= highTXMark ?
    Condition c5 = Condition(XFSM_NEXTTXSEQ, XFSM_HIGHTXMARK, 0, XFSM_GREATER_EQ);
    // ackNo >= HighTxMark
    Condition c6 = Condition(XFSM_CONTEXT_PARAM0, XFSM_HIGHTXMARK, 0, XFSM_GREATER_EQ);
    // four "fake" conditions which will always be true
    Condition c7 = Condition(XFSM_SEGMENTSIZE, XFSM_ZERO, 0, XFSM_GREATER_EQ);
    Condition c8 = Condition(XFSM_SEGMENTSIZE, XFSM_ZERO, 0, XFSM_GREATER_EQ);
    Condition c9 = Condition(XFSM_SEGMENTSIZE, XFSM_ZERO, 0, XFSM_GREATER_EQ);
    Condition c10 = Condition(XFSM_SEGMENTSIZE, XFSM_ZERO, 0, XFSM_GREATER_EQ);

    // The generic timer expired is the one we use to finally close our socket
    Condition c11 = Condition(XFSM_NEXTTXSEQ, XFSM_CONTEXT_PARAM0, 0, XFSM_EQUAL);

    // XFSM_SOCKETISCLOSED is > 0? If so, the socket told us to close after we've sent everything
    Condition c12 = Condition(XFSM_SOCKETISCLOSED, XFSM_ZERO, 0, XFSM_GREATER);
    //Do i have any more data to transmit?
    Condition c13 = Condition(XFSM_BYTESTOTRANSMIT, XFSM_ZERO, 0, XFSM_GREATER);

    Condition c14 = Condition(XFSM_LASTACKEDSEQ, XFSM_HIGHTXMARK, 0, XFSM_GREATER_EQ);


    ConditionList conditionList;
    conditionList.add_element(c1);
    conditionList.add_element(c2);
    conditionList.add_element(c3);
    conditionList.add_element(c4);
    conditionList.add_element(c5);
    conditionList.add_element(c6);
    conditionList.add_element(c7);
    conditionList.add_element(c8);
    conditionList.add_element(c9);
    conditionList.add_element(c10);
    conditionList.add_element(c11);
    conditionList.add_element(c12);
    conditionList.add_element(c13);
    conditionList.add_element(c14);
 

    /*~~~~~~~~~~~~~~~~~~~ Table entries ~~~~~~~~~~~~~~~~*/

    /*********** TABLE ENTRY 1 (generic timeout) ***********/
    /******* Create state table ********/
    std::vector <TableEntry> table;
  
    // expected results for table entry 1
    std::vector <Results_t> results1;
    results1.push_back(XFSM_TRUE);
    results1.push_back(XFSM_DONT_CARE);
    results1.push_back(XFSM_DONT_CARE);
    results1.push_back(XFSM_DONT_CARE);
    results1.push_back(XFSM_DONT_CARE);
    results1.push_back(XFSM_DONT_CARE);
    /*results1.push_back(XFSM_DONT_CARE);
    results1.push_back(XFSM_DONT_CARE);
    results1.push_back(XFSM_DONT_CARE);
    results1.push_back(XFSM_DONT_CARE);*/
    results1.push_back(XFSM_DONT_CARE);
    results1.push_back(XFSM_DONT_CARE);
    results1.push_back(XFSM_TRUE);

    // set the timeout
    XfsmAction setTimer1_1 = XfsmAction(XFSM_SUM, XFSM_CUSTOM5, (uint32_t) 0, XFSM_ARG2);  // timeout = rtt
    XfsmAction setTimer1_2 = XfsmAction(XFSM_MULTIPLY, XFSM_ARG2, 2, XFSM_ARG2);  // timeout = 2*rtt
    // set the sequence number
    XfsmAction setTimer1_3 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0);
    // set the retx count
    XfsmAction setTimer1_4 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 1, XFSM_ARG1);
    XfsmAction setTimer1_5 = XfsmAction(XFSM_SETTIMER);

    /* SendDataPacket */
    XfsmAction send1_1 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0); // sequence
    XfsmAction send1_2 = XfsmAction(XFSM_SUM, XFSM_SEGMENTSIZE, (uint32_t) 0, XFSM_ARG1); // size
    XfsmAction send1_3 = XfsmAction(XFSM_SENDPCKT);

    /* Set "fake" timer to schedule next trasmissions, if any */
    XfsmAction setGenTimer1_1 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 5, XFSM_ARG0); // fake '5us' schedule time
    XfsmAction setGenTimer1_2 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer1_3 = XfsmAction(XFSM_SETGENERICTIMER);

    /* Create the action list */
    std::vector<XfsmAction> actions1;
    actions1.push_back(setTimer1_1);
    actions1.push_back(setTimer1_2);
    actions1.push_back(setTimer1_3);
    actions1.push_back(setTimer1_4);
    actions1.push_back(setTimer1_5);
    actions1.push_back(send1_1);
    actions1.push_back(send1_2);
    actions1.push_back(send1_3);
    actions1.push_back(setGenTimer1_1);
    actions1.push_back(setGenTimer1_2);
    actions1.push_back(setGenTimer1_3);

    /* Construct the table entry */
    TableEntry te1 = TableEntry(results1, 0, 0, actions1, 10, XFSM_GENERIC_TIMEOUT);

    table.push_back(te1);


    /*********** TABLE ENTRY 2 (ACK) ***********/
    // note: m_tcb->m_lastAckedSequence is updated from outside xfsm, no need to do it here

    // expected results for table entry 2
    std::vector <Results_t> results2;
    results2.push_back(XFSM_DONT_CARE); 
    results2.push_back(XFSM_FALSE);  
    results2.push_back(XFSM_DONT_CARE); 
    results2.push_back(XFSM_DONT_CARE); 
    results2.push_back(XFSM_DONT_CARE); 

    /* Increase window! */
    XfsmAction update2_1 = XfsmAction(XFSM_SUM, XFSM_CWND, XFSM_SEGMENTSIZE, XFSM_CWND);

    /* Schedule "fake" timer to schedule next trasmissions, if any */
    XfsmAction setGenTimer2_1 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 5, XFSM_ARG0); // fake '5us' schedule time
    XfsmAction setGenTimer2_2 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer2_3 = XfsmAction(XFSM_SETGENERICTIMER);
    XfsmAction updateRTT = XfsmAction(XFSM_MAX, XFSM_CUSTOM5, XFSM_CONTEXT_PARAM1, XFSM_CUSTOM5);

    std::vector<XfsmAction> actions2;
    actions2.push_back(update2_1);
    actions2.push_back(setGenTimer2_1);
    actions2.push_back(setGenTimer2_2);
    actions2.push_back(setGenTimer2_3);
    actions2.push_back(updateRTT);

    /* Construct the table entry */
    TableEntry te2 = TableEntry(results2, 0, 0, actions2, 0, XFSM_ACK);

    table.push_back(te2);

    /*********** TABLE ENTRY 3 (TEB timeout): state transition to recovery ***********/

    // expected results for table entry 1
    std::vector <Results_t> results3;
    results3.push_back(XFSM_DONT_CARE);
    results3.push_back(XFSM_DONT_CARE);
    results3.push_back(XFSM_FALSE);
    results3.push_back(XFSM_TRUE);
    results3.push_back(XFSM_DONT_CARE);
    results3.push_back(XFSM_DONT_CARE);
    results3.push_back(XFSM_DONT_CARE);
    results3.push_back(XFSM_DONT_CARE);
    results3.push_back(XFSM_DONT_CARE);
    results3.push_back(XFSM_DONT_CARE);
    results3.push_back(XFSM_DONT_CARE);
    results3.push_back(XFSM_DONT_CARE);
    results3.push_back(XFSM_TRUE); 

    /* Actions */
    // decrease window to cwnd/2
    XfsmAction update3_1 = XfsmAction(XFSM_DIVIDE, XFSM_CWND, (uint32_t) 2, XFSM_CWND);

    // set currentReTXRound++
    XfsmAction update3_2 = XfsmAction(XFSM_SUM, XFSM_CUSTOM0, 1, XFSM_CUSTOM0);

    // calculate the pacing delay
    XfsmAction update3_3 = XfsmAction(XFSM_DIVIDE, XFSM_CWND, XFSM_SEGMENTSIZE, XFSM_CUSTOM1); // cwnd/segSize
    XfsmAction update3_4 = XfsmAction(XFSM_DIVIDE, XFSM_CUSTOM5, XFSM_CUSTOM1, XFSM_CUSTOM1); // rtt/ cwnd [pkt]

    // retransmit packet
    XfsmAction send3_1 = XfsmAction(XFSM_SUM, XFSM_CONTEXT_PARAM0, (uint32_t) 0, XFSM_ARG0); // sequence
    XfsmAction send3_2 = XfsmAction(XFSM_SUM, XFSM_SEGMENTSIZE, (uint32_t) 0, XFSM_ARG1); // size
    XfsmAction send3_3 = XfsmAction(XFSM_SENDPCKT);
    
    /* Start scheduling the next transmission at pacing time */
    XfsmAction setGenTimer3_1 = XfsmAction(XFSM_SUM, XFSM_CUSTOM1, 0, XFSM_ARG0); // pacing time
    XfsmAction setGenTimer3_2 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer3_3 = XfsmAction(XFSM_SETGENERICTIMER);

    // TODO: re initialize the teb timeout

    // set global highestSentSeqNo = highTxMark: we will use this variable to know when to end recovery,
    //                                           i.e when we retransmitted everything
    XfsmAction update3_5 = XfsmAction(XFSM_SUM, XFSM_HIGHTXMARK, 0, XFSM_CUSTOM2);

    // populate the action vector
    std::vector<XfsmAction> actions3;
    actions3.push_back(update3_1);
    actions3.push_back(update3_2);
    actions3.push_back(update3_3);
    actions3.push_back(update3_4);
    actions3.push_back(send3_1);
    actions3.push_back(send3_2);
    actions3.push_back(send3_3);
    actions3.push_back(setGenTimer3_1);
    actions3.push_back(setGenTimer3_2);
    actions3.push_back(setGenTimer3_3);
    actions3.push_back(update3_5);

    /* Construct the table entry */
    TableEntry te3 = TableEntry(results3, 0, 1, actions3, 0, XFSM_TIMEOUT);

    table.push_back(te3);


    /*********** TABLE ENTRY 4 (generic timeout): recovery pacing transmission ***********/

    // expected results for table entry 4
    std::vector <Results_t> results4;
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_FALSE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_DONT_CARE);
    results4.push_back(XFSM_TRUE); 

    /* Actions */
    // retransmit packet
    XfsmAction send4_1 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0); // sequence
    XfsmAction send4_2 = XfsmAction(XFSM_SUM, XFSM_SEGMENTSIZE, (uint32_t) 0, XFSM_ARG1); // size
    XfsmAction send4_3 = XfsmAction(XFSM_SENDPCKT);

    /* Schedule the next transmission at pacing time */
    XfsmAction setGenTimer4_1 = XfsmAction(XFSM_SUM, XFSM_CUSTOM1, 0, XFSM_ARG0); // pacing time
    XfsmAction setGenTimer4_2 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer4_3 = XfsmAction(XFSM_SETGENERICTIMER);

    std::vector<XfsmAction> actions4;
    actions4.push_back(send4_1);
    actions4.push_back(send4_2);
    actions4.push_back(send4_3);
    actions4.push_back(setGenTimer4_1);
    actions4.push_back(setGenTimer4_2);
    actions4.push_back(setGenTimer4_3);

    /* Construct the table entry */
    TableEntry te4 = TableEntry(results4, 1, 1, actions4, 0, XFSM_GENERIC_TIMEOUT);

    table.push_back(te4);


    /*********** TABLE ENTRY 5 (generic timeout): state transition to post recovery ***********/

    // expected results for table entry 5
    
    std::vector <Results_t> results5;
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_TRUE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_DONT_CARE);
    results5.push_back(XFSM_TRUE); 

    // Actions 
    // transmit packet
    XfsmAction send5_1 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0); // sequence
    XfsmAction send5_2 = XfsmAction(XFSM_SUM, XFSM_SEGMENTSIZE, (uint32_t) 0, XFSM_ARG1); // size
    XfsmAction send5_3 = XfsmAction(XFSM_SENDPCKT);

    // reset globalReTXRound to 0
    XfsmAction update5_1 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 0, XFSM_CUSTOM0);

    // calculate the pacing delay
    // saving packets to pace in custom 3, so we can add one more at each scheduled tx to reduce delay
    XfsmAction update5_2 = XfsmAction(XFSM_DIVIDE, XFSM_CWND, XFSM_SEGMENTSIZE, XFSM_CUSTOM3); // cwnd/segSize = pktsToPace
    XfsmAction update5_3 = XfsmAction(XFSM_SUM, XFSM_CUSTOM3, (uint32_t) 1, XFSM_CUSTOM3); // pktsToPace++
    XfsmAction update5_4 = XfsmAction(XFSM_DIVIDE, XFSM_CUSTOM5, XFSM_CUSTOM3, XFSM_CUSTOM1); // rtt/pktsToPace

    /* Schedule the next transmission at pacing time */
    XfsmAction setGenTimer5_1 = XfsmAction(XFSM_SUM, XFSM_CUSTOM1, 0, XFSM_ARG0); // pacing time
    XfsmAction setGenTimer5_2 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer5_3 = XfsmAction(XFSM_SETGENERICTIMER);

    // set the timeout
    XfsmAction setTimer5_1 = XfsmAction(XFSM_SUM, XFSM_CUSTOM5, (uint32_t) 0, XFSM_ARG2);  // timeout = rtt
    XfsmAction setTimer5_2 = XfsmAction(XFSM_MULTIPLY, XFSM_ARG2, 2, XFSM_ARG2);  // timeout = 2*rtt
    // set the sequence number
    XfsmAction setTimer5_3 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0);
    // set the retx count
    XfsmAction setTimer5_4 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 1, XFSM_ARG1);
    XfsmAction setTimer5_5 = XfsmAction(XFSM_SETTIMER);

    std::vector<XfsmAction> actions5;
    actions5.push_back(update5_1);
    actions5.push_back(send5_1);
    actions5.push_back(send5_2);
    actions5.push_back(send5_3);
    actions5.push_back(update5_2);
    actions5.push_back(update5_3);
    actions5.push_back(update5_4);
    actions5.push_back(setGenTimer5_1);
    actions5.push_back(setGenTimer5_2);
    actions5.push_back(setGenTimer5_3);
    actions5.push_back(setTimer5_1);
    actions5.push_back(setTimer5_2);
    actions5.push_back(setTimer5_3);
    actions5.push_back(setTimer5_4);
    actions5.push_back(setTimer5_5);

    //Construct the table entry //
    TableEntry te5 = TableEntry(results5, 1, 2, actions5, 0, XFSM_GENERIC_TIMEOUT);

    table.push_back(te5);


    /*********** TABLE ENTRY 5_1 (ACK): new "good" ack recived, end recovery ***********/

    // expected results for table entry 5
    std::vector <Results_t> results5_1;
    results5_1.push_back(XFSM_DONT_CARE);
    results5_1.push_back(XFSM_DONT_CARE);
    results5_1.push_back(XFSM_DONT_CARE);
    results5_1.push_back(XFSM_DONT_CARE);
    results5_1.push_back(XFSM_DONT_CARE); 
    results5_1.push_back(XFSM_TRUE);

    std::vector <XfsmAction> actions5_1;
    actions5_1.push_back(update5_1);
    actions5_1.push_back(send5_1);
    actions5_1.push_back(send5_2);
    actions5_1.push_back(send5_3);
    actions5_1.push_back(update5_2);
    actions5_1.push_back(update5_3);
    actions5_1.push_back(update5_4);
    actions5_1.push_back(setGenTimer5_1);
    actions5_1.push_back(setGenTimer5_2);
    actions5_1.push_back(setGenTimer5_3);
    actions5_1.push_back(setTimer5_1);
    actions5_1.push_back(setTimer5_2);
    actions5_1.push_back(setTimer5_3);
    actions5_1.push_back(setTimer5_4);
    actions5_1.push_back(setTimer5_5);
    actions5_1.push_back(updateRTT);
    TableEntry te5_1 = TableEntry(results5_1, 1, 2, actions5_1, 0, XFSM_ACK);

    table.push_back(te5_1);



    /*********** TABLE ENTRY 6 (generic timeout): post recovery pacing ***********/

    // expected results for table entry 6
    
    std::vector <Results_t> results6;
    results6.push_back(XFSM_TRUE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_DONT_CARE);
    results6.push_back(XFSM_TRUE); 

    // Actions 
    // transmit packet
    XfsmAction send6_1 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0); // sequence
    XfsmAction send6_2 = XfsmAction(XFSM_SUM, XFSM_SEGMENTSIZE, (uint32_t) 0, XFSM_ARG1); // size
    XfsmAction send6_3 = XfsmAction(XFSM_SENDPCKT);

    XfsmAction update6_1 = XfsmAction(XFSM_SUM, XFSM_CUSTOM3, (uint32_t) 1, XFSM_CUSTOM3); // pktsToPace++
    XfsmAction update6_2 = XfsmAction(XFSM_DIVIDE, XFSM_CUSTOM5, XFSM_CUSTOM3, XFSM_CUSTOM1); // rtt/pktsToPace

    /* Schedule the next transmission at pacing time */
    XfsmAction setGenTimer6_1 = XfsmAction(XFSM_SUM, XFSM_CUSTOM1, 0, XFSM_ARG0); // pacing time
    XfsmAction setGenTimer6_2 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer6_3 = XfsmAction(XFSM_SETGENERICTIMER);

    // set the timeout
    XfsmAction setTimer6_1 = XfsmAction(XFSM_SUM, XFSM_CUSTOM5, (uint32_t) 0, XFSM_ARG2);  // timeout = rtt
    XfsmAction setTimer6_2 = XfsmAction(XFSM_MULTIPLY, XFSM_ARG2, 2, XFSM_ARG2);  // timeout = 2*rtt
    // set the sequence number
    XfsmAction setTimer6_3 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG0);
    // set the retx count
    XfsmAction setTimer6_4 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 1, XFSM_ARG1);
    XfsmAction setTimer6_5 = XfsmAction(XFSM_SETTIMER);


    std::vector<XfsmAction> actions6;
    actions6.push_back(send6_1);
    actions6.push_back(send6_2);
    actions6.push_back(send6_3);
    actions6.push_back(update6_1);
    actions6.push_back(update6_2);
    actions6.push_back(setGenTimer6_1);
    actions6.push_back(setGenTimer6_2);
    actions6.push_back(setGenTimer6_3);
    actions6.push_back(setTimer6_1);
    actions6.push_back(setTimer6_2);
    actions6.push_back(setTimer6_3);
    actions6.push_back(setTimer6_4);
    actions6.push_back(setTimer6_5);


    //Construct the table entry //
    TableEntry te6 = TableEntry(results6, 2, 2, actions6, 0, XFSM_GENERIC_TIMEOUT);

    table.push_back(te6);

    /*********** TABLE ENTRY 7 (generic timeout): end post recovery -> goto congestion avoidance ***********/

    // expected results for table entry 7
    
    std::vector <Results_t> results7;
    results7.push_back(XFSM_FALSE);
    results7.push_back(XFSM_DONT_CARE);
    results7.push_back(XFSM_DONT_CARE);
    results7.push_back(XFSM_DONT_CARE);
    results7.push_back(XFSM_DONT_CARE); 

    // empty: only go in state congestion avoidance
    std::vector<XfsmAction> actions7;

    //Construct the table entry //
    TableEntry te7 = TableEntry(results7, 2, 3, actions7, 0, XFSM_GENERIC_TIMEOUT);

    table.push_back(te7);


    /*********** TABLE ENTRY 8 (ACK): new ack received! schedule next transmission ***********/

    // expected results for table entry 8
    std::vector <Results_t> results8;
    results8.push_back(XFSM_DONT_CARE);
    results8.push_back(XFSM_FALSE);
    results8.push_back(XFSM_DONT_CARE);
    results8.push_back(XFSM_DONT_CARE);
    results8.push_back(XFSM_DONT_CARE); 

    // Actions 
    // increase cwnd (AIMD): cwnd += MSS*MSS/cwnd
    XfsmAction update8_1 = XfsmAction(XFSM_MULTIPLY, XFSM_SEGMENTSIZE, XFSM_SEGMENTSIZE, XFSM_CUSTOM4); // MSS*MSS
    XfsmAction update8_2 = XfsmAction(XFSM_DIVIDE, XFSM_CUSTOM4, XFSM_CWND, XFSM_CUSTOM4); // MSS^2/cwnd
    XfsmAction update8_3 = XfsmAction(XFSM_SUM, XFSM_CWND, XFSM_CUSTOM4, XFSM_CWND);

    /* Schedule "fake" timer to schedule next trasmissions, if any */
    XfsmAction setGenTimer8_1 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 5, XFSM_ARG0); // fake '5us' schedule time
    XfsmAction setGenTimer8_2 = XfsmAction(XFSM_SUM, XFSM_NEXTTXSEQ, (uint32_t) 0, XFSM_ARG1); // sequence
    XfsmAction setGenTimer8_3 = XfsmAction(XFSM_SETGENERICTIMER);

    std::vector<XfsmAction> actions8;
    actions8.push_back(update8_1);
    actions8.push_back(update8_2);
    actions8.push_back(update8_3);
    actions8.push_back(setGenTimer8_1);
    actions8.push_back(setGenTimer8_2);
    actions8.push_back(setGenTimer8_3);
    actions8.push_back(updateRTT);

    //Construct the table entry //
    TableEntry te8 = TableEntry(results8, 3, 3, actions8, 0, XFSM_ACK);

    table.push_back(te8);


    /*********** TABLE ENTRY 9 (generic timeout) ***********/

    // expected results for table entry 9
    std::vector <Results_t> results9;
    results9.push_back(XFSM_TRUE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_DONT_CARE);
    results9.push_back(XFSM_TRUE); 

    //Construct the table entry //
    // no need for actions!!! same as the first table entry
    TableEntry te9 = TableEntry(results9, 3, 3, actions1, 0, XFSM_GENERIC_TIMEOUT);

    table.push_back(te9);


    /*********** TABLE ENTRY 10 (teb timeout)  go to recovery***********/

    // expected results for table entry 10
    std::vector <Results_t> results10;
    results10.push_back(XFSM_DONT_CARE);
    results10.push_back(XFSM_DONT_CARE);
    results10.push_back(XFSM_FALSE);
    results10.push_back(XFSM_TRUE);
    results10.push_back(XFSM_DONT_CARE);
    results10.push_back(XFSM_DONT_CARE);
    results10.push_back(XFSM_DONT_CARE);
    results10.push_back(XFSM_DONT_CARE);
    results10.push_back(XFSM_DONT_CARE);
    results10.push_back(XFSM_DONT_CARE);
    results10.push_back(XFSM_DONT_CARE);
    results10.push_back(XFSM_DONT_CARE);
    results10.push_back(XFSM_TRUE);

    //Construct the table entry //
    // no need for actions!!! same as the third table entry
    TableEntry te10 = TableEntry(results10, 3, 1, actions3, 0, XFSM_TIMEOUT);

    table.push_back(te10);


    // ************ BORING TCP CONNECTION ESTABLISHMENT AND CLOSING PART STARTS HERE *****************

    // 0. the socket is closed, we connect
    std::vector <Results_t> results300;
    results300.push_back(XFSM_DONT_CARE);


    std::vector<XfsmAction> actions300;
    XfsmAction clearRegister = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 0, XFSM_TCPFLAGS);
    XfsmAction setSynFlag= XfsmAction(XFSM_OR, XFSM_TCPFLAGS, (uint32_t) XFSM_FLAG_SYN, XFSM_TCPFLAGS);
    XfsmAction setGenTimer300_1 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 500000, XFSM_ARG2); // '0.5s' schedule time
    XfsmAction setGenTimer300_2 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 1, XFSM_ARG0); // sequence
    XfsmAction setGenTimer300_3 = XfsmAction(XFSM_SETTIMER);
    XfsmAction sendEmptyPacket = XfsmAction(XFSM_SENDEMPTYPACKET);

    actions300.push_back(clearRegister);
    actions300.push_back(setSynFlag);
    actions300.push_back(setGenTimer300_1);
    actions300.push_back(setGenTimer300_2);
    actions300.push_back(setGenTimer300_3);
    actions300.push_back(sendEmptyPacket);

    TableEntry te300 = TableEntry(results300, XFSM_CLOSED, XFSM_SYNSENT, actions300, 0, XFSM_CONNECT);
    table.push_back(te300);


    // 1. we are in syn sent, timer for the syn expires

    std::vector <Results_t> results301;
    results301.push_back(XFSM_DONT_CARE);

    std::vector<XfsmAction> actions301;
    //simply re-send the empty packet
    XfsmAction setGenTimer301_1 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 500000, XFSM_ARG2); // '0.5s' schedule time
    XfsmAction setGenTimer301_2 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 1, XFSM_ARG0); // sequence
    XfsmAction setGenTimer301_3 = XfsmAction(XFSM_SETTIMER);

    actions301.push_back(clearRegister);
    actions301.push_back(setSynFlag);
    actions301.push_back(setGenTimer301_1);
    actions301.push_back(setGenTimer301_2);
    actions301.push_back(setGenTimer301_3);
    actions301.push_back(sendEmptyPacket);


    TableEntry te301 = TableEntry(results301, XFSM_SYNSENT, XFSM_SYNSENT, actions301, 0, XFSM_TIMEOUT);
    table.push_back(te301);

    // 2. we are in syn sent, a synack arrives

    std::vector <Results_t> results302;


    results302.push_back(XFSM_DONT_CARE);
    results302.push_back(XFSM_DONT_CARE);
    results302.push_back(XFSM_DONT_CARE);
    results302.push_back(XFSM_DONT_CARE);
    results302.push_back(XFSM_DONT_CARE);
    results302.push_back(XFSM_DONT_CARE);
    results302.push_back(XFSM_DONT_CARE); 
    results302.push_back(XFSM_DONT_CARE); 
    results302.push_back(XFSM_DONT_CARE);
    results302.push_back(XFSM_DONT_CARE);


    std::vector<XfsmAction> actions302;

    //first, remove the previous scheduled timer
    XfsmAction rmTimer1 = XfsmAction(XFSM_SUM, XFSM_ZERO, XFSM_NEXTTXSEQ, XFSM_ARG0);
    XfsmAction rmTimer2 = XfsmAction(XFSM_REMOVETIMER);

    // now, we can start setting flags and sending the new packet
    XfsmAction setAckFlag= XfsmAction(XFSM_OR, XFSM_TCPFLAGS, (uint32_t) XFSM_FLAG_ACK, XFSM_TCPFLAGS);
    XfsmAction setRtt = XfsmAction(XFSM_MULTIPLY, XFSM_RTT, 2, XFSM_ARG2);
    XfsmAction setGenTimer302_2 = XfsmAction(XFSM_SUM, XFSM_ZERO, XFSM_NEXTTXSEQ, XFSM_ARG0); // sequence
    XfsmAction setGenTimer302_3 = XfsmAction(XFSM_SETTIMER);
    XfsmAction sendPendingData = XfsmAction(XFSM_SENDBUFF);

    actions302.push_back(rmTimer1);
    actions302.push_back(rmTimer2);
    actions302.push_back(clearRegister);
    actions302.push_back(setAckFlag);
    actions302.push_back(sendEmptyPacket);
    actions302.push_back(sendPendingData);
    actions302.push_back(setRtt);
    actions302.push_back(setGenTimer302_2);
    actions302.push_back(setGenTimer302_3);

    TableEntry te302= TableEntry(results302, XFSM_SYNSENT, 0, actions302, 0, XFSM_SYNACK);
    table.push_back(te302);

    // 3. we are in syn sent, a syn arrives. We reply with a synack and we mode to SYN_RCVD

    
    std::vector <Results_t> results303;

    // The conditions we want to satisfy are: the header contains SYN+ACK

    results303.push_back(XFSM_DONT_CARE);
    results303.push_back(XFSM_DONT_CARE);
    results303.push_back(XFSM_DONT_CARE);
    results303.push_back(XFSM_DONT_CARE);
    results303.push_back(XFSM_DONT_CARE);
    results303.push_back(XFSM_DONT_CARE);
    results303.push_back(XFSM_DONT_CARE); 
    results303.push_back(XFSM_DONT_CARE);

    //first, remove the previous scheduled timer
    //this is done in rmTimer1 && rmTimer2
    
    // now, we can start setting flags and sending the new packet

    XfsmAction setGenTimer303_1 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 500000, XFSM_ARG2); // '0.5s' schedule time
    XfsmAction setGenTimer303_2 = XfsmAction(XFSM_SUM, XFSM_ZERO, XFSM_NEXTTXSEQ, XFSM_ARG0); // sequence
    XfsmAction setGenTimer303_3 = XfsmAction(XFSM_SETTIMER);


    std::vector<XfsmAction> actions303;
    actions303.push_back(rmTimer1);
    actions303.push_back(rmTimer2);
    actions303.push_back(clearRegister);
    actions303.push_back(setSynFlag);
    actions303.push_back(setAckFlag);
    actions303.push_back(sendEmptyPacket);
    actions303.push_back(setGenTimer303_1);
    actions303.push_back(setGenTimer303_2);
    actions303.push_back(setGenTimer303_3);

    TableEntry te303= TableEntry(results303, XFSM_SYNSENT, XFSM_SYNRCVD, actions303, 0, XFSM_SYN);
    table.push_back(te303);

    // 4. we are in state 0, the timer for the ack fires

    std::vector <Results_t> results304;
    results304.push_back(XFSM_DONT_CARE);
    results304.push_back(XFSM_DONT_CARE);
    results304.push_back(XFSM_FALSE);



    std::vector<XfsmAction> actions304;
    XfsmAction setGenTimer304_1 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 500000, XFSM_ARG2); // '0.5s' schedule time
    XfsmAction setGenTimer304_2 = XfsmAction(XFSM_SUM, XFSM_ZERO, XFSM_NEXTTXSEQ, XFSM_ARG0); // sequence
    XfsmAction setGenTimer304_3 = XfsmAction(XFSM_SETTIMER);
    actions304.push_back(clearRegister);
    actions304.push_back(setAckFlag);
    actions304.push_back(sendEmptyPacket);
    actions304.push_back(setGenTimer304_1);
    actions304.push_back(setGenTimer304_2);
    actions304.push_back(setGenTimer304_3);


    TableEntry te304= TableEntry(results304, 0, 0, actions304, 0, XFSM_TIMEOUT);
    table.push_back(te304);

    //5. we are in state XFSM_SYNRCVD, timer fires

    std::vector <Results_t> results305;
    results305.push_back(XFSM_DONT_CARE);
    results305.push_back(XFSM_DONT_CARE);
    results305.push_back(XFSM_FALSE);


    std::vector<XfsmAction> actions305;
    XfsmAction setGenTimer305_1 = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 500000, XFSM_ARG2); // '0.5s' schedule time
    XfsmAction setGenTimer305_2 = XfsmAction(XFSM_SUM, XFSM_ZERO, XFSM_NEXTTXSEQ, XFSM_ARG0); // sequence
    XfsmAction setGenTimer305_3 = XfsmAction(XFSM_SETTIMER);
    actions305.push_back(clearRegister);
    actions305.push_back(setSynFlag);
    actions305.push_back(setAckFlag);
    actions305.push_back(sendEmptyPacket);
    actions305.push_back(setGenTimer305_1);
    actions305.push_back(setGenTimer305_2);
    actions305.push_back(setGenTimer305_3);


    TableEntry te305= TableEntry(results305, XFSM_SYNRCVD, XFSM_SYNRCVD, actions305, 0, XFSM_TIMEOUT);
    table.push_back(te305);



    //6. we are in any of the connection established states and a FIN arrives, we send our FINACK
    // since we have here the same conditions and actions for all states (0, 1, 2, ...)
    // we use the same vectors for different table entries

    std::vector <Results_t> results306;

    results306.push_back(XFSM_DONT_CARE);
    results306.push_back(XFSM_DONT_CARE);
    results306.push_back(XFSM_DONT_CARE);
    results306.push_back(XFSM_DONT_CARE);
    results306.push_back(XFSM_DONT_CARE);
    results306.push_back(XFSM_DONT_CARE);
    results306.push_back(XFSM_DONT_CARE);
    results306.push_back(XFSM_DONT_CARE);
    results306.push_back(XFSM_DONT_CARE); 



    std::vector<XfsmAction> actions306;
    XfsmAction setFinFlag= XfsmAction(XFSM_OR, XFSM_TCPFLAGS, (uint32_t) XFSM_FLAG_FIN, XFSM_TCPFLAGS);
    XfsmAction setGenTimer306_2 = XfsmAction(XFSM_SUM, XFSM_ZERO, XFSM_NEXTTXSEQ, XFSM_ARG0); // sequence
    XfsmAction setGenTimer306_3 = XfsmAction(XFSM_SETTIMER);
    actions306.push_back(clearRegister);
    actions306.push_back(setFinFlag);
    actions306.push_back(setAckFlag);
    actions306.push_back(sendEmptyPacket);
    actions306.push_back(setRtt);
    actions306.push_back(setGenTimer306_2);
    actions306.push_back(setGenTimer306_3);

    /*
    //for state 0
    TableEntry te306= TableEntry(results306, 0, XFSM_CLOSE_WAIT, actions306, 0, XFSM_EMPTYPACKET);
    table.push_back(te306);

    //for state 1
    TableEntry te307= TableEntry(results306, 1, XFSM_CLOSE_WAIT, actions306, 0, XFSM_EMPTYPACKET);
    table.push_back(te307);

    //for state 2
    TableEntry te308= TableEntry(results306, 2, XFSM_CLOSE_WAIT, actions306, 0, XFSM_EMPTYPACKET);
    table.push_back(te308);

    //for state 3
    TableEntry te309= TableEntry(results306, 3, XFSM_CLOSE_WAIT, actions306, 0, XFSM_EMPTYPACKET);
    table.push_back(te309);
    
    */


    TableEntry te310= TableEntry(results306, 0, XFSM_CLOSE_WAIT, actions306, 0, XFSM_FINACK);
    table.push_back(te310);

    TableEntry te311= TableEntry(results306, 1, XFSM_CLOSE_WAIT, actions306, 0, XFSM_FINACK);
    table.push_back(te311);

    TableEntry te312= TableEntry(results306, 2, XFSM_CLOSE_WAIT, actions306, 0, XFSM_FINACK);
    table.push_back(te312);

    TableEntry te313= TableEntry(results306, 3, XFSM_CLOSE_WAIT, actions306, 0, XFSM_FINACK);
    table.push_back(te313);


    /* 7. We are in any of the connection_established states and we've ran out of data to send,
     *   the FIN was sent in the last packet
     *   and so the only thing we have to do is to move to the next state
    */

    //Nothing to do in these two vectors... just change state
    std::vector <Results_t> results307;
    results307.push_back(XFSM_DONT_CARE);
    std::vector<XfsmAction> actions307;

    TableEntry te314 = TableEntry(results307, 0, XFSM_FIN_WAIT1, actions307, 0, XFSM_MOVE_STATE);
    table.push_back(te314);

    TableEntry te315 = TableEntry(results307, 1, XFSM_FIN_WAIT1, actions307, 0, XFSM_MOVE_STATE);
    table.push_back(te315);

    TableEntry te316 = TableEntry(results307, 2, XFSM_FIN_WAIT1, actions307, 0, XFSM_MOVE_STATE);
    table.push_back(te316);

    TableEntry te317 = TableEntry(results307, 3, XFSM_FIN_WAIT1, actions307, 0, XFSM_MOVE_STATE);
    table.push_back(te317);



    /* 8. We are in XFSM_FIN_WAIT_1 and our FIN is ACKed with an empty packet, so we move to XFSM_FIN_WAIT_2
    */

    std::vector <Results_t> results308;


    results308.push_back(XFSM_DONT_CARE);
    results308.push_back(XFSM_DONT_CARE);
    results308.push_back(XFSM_DONT_CARE);
    results308.push_back(XFSM_DONT_CARE);
    results308.push_back(XFSM_DONT_CARE);
    results308.push_back(XFSM_FALSE);
    results308.push_back(XFSM_DONT_CARE);
    results308.push_back(XFSM_DONT_CARE); 
    results308.push_back(XFSM_DONT_CARE); 
    

    std::vector<XfsmAction> actions308;

    TableEntry te318 = TableEntry(results308, XFSM_FIN_WAIT1, XFSM_FIN_WAIT2, actions308, 0, XFSM_FINACK);
    table.push_back(te318);

    /* 9. We are in XFSM_FIN_WAIT_2 and a FIN arrives, so we reply with an ack and we set the timer for closing
          our socket
    */

    std::vector<Results_t> results309;
    results309.push_back(XFSM_DONT_CARE);
    results309.push_back(XFSM_DONT_CARE);
    results309.push_back(XFSM_DONT_CARE);
    results309.push_back(XFSM_DONT_CARE);
    results309.push_back(XFSM_DONT_CARE);
    results309.push_back(XFSM_DONT_CARE);
    results309.push_back(XFSM_DONT_CARE);
    results309.push_back(XFSM_DONT_CARE);
    results309.push_back(XFSM_DONT_CARE); // FIN condition

    std::vector<XfsmAction> actions309;

    XfsmAction setExpTime = XfsmAction(XFSM_SUM, XFSM_ZERO, 120000000, XFSM_ARG2); //setting the 2 minutes timer
    XfsmAction setSeqParam = XfsmAction(XFSM_SUM, XFSM_ZERO, XFSM_NEXTTXSEQ, XFSM_ARG0); //setting the sequence param as the next transmission byte (just to recognize the timer expired)
    XfsmAction setLastTimer = XfsmAction(XFSM_SETTIMER);


    actions309.push_back(clearRegister);
    actions309.push_back(setAckFlag);
    actions309.push_back(sendEmptyPacket);
    actions309.push_back(setExpTime);
    actions309.push_back(setSeqParam);
    actions309.push_back(setLastTimer);

    TableEntry te319 = TableEntry(results309, XFSM_FIN_WAIT2, XFSM_TIME_WAIT, actions309, 0, XFSM_FIN);
    table.push_back(te319);

    /* 10. We are in close wait and the closing socket timer expires, so we close our socket
    */

    std::vector<Results_t> results310;
    results310.push_back(XFSM_DONT_CARE);
    results310.push_back(XFSM_DONT_CARE);
    results310.push_back(XFSM_DONT_CARE);
    results310.push_back(XFSM_DONT_CARE);
    results310.push_back(XFSM_DONT_CARE);
    results310.push_back(XFSM_DONT_CARE);
    /*
    results310.push_back(XFSM_DONT_CARE);
    results310.push_back(XFSM_DONT_CARE);
    results310.push_back(XFSM_DONT_CARE);
    results310.push_back(XFSM_DONT_CARE);
    results310.push_back(XFSM_DONT_CARE);
    results310.push_back(XFSM_TRUE); //the generic timer expired is the one we need to close the socket
    */

    std::vector<XfsmAction> actions310;
    XfsmAction closeSocket = XfsmAction(XFSM_CLOSESOCKET);
    actions310.push_back(closeSocket);

    TableEntry te320 = TableEntry(results310, XFSM_TIME_WAIT, XFSM_CLOSED, actions310, 0, XFSM_TIMEOUT);
    table.push_back(te320);


    //11. In any state, if the socket tells me to close, I set the register XFSM_SOCKETISCLOSED to 1

    std::vector<Results_t> results311;
    results311.push_back(XFSM_DONT_CARE);
    results311.push_back(XFSM_DONT_CARE);
    results311.push_back(XFSM_DONT_CARE);
    results311.push_back(XFSM_DONT_CARE);
    results311.push_back(XFSM_DONT_CARE);
    results311.push_back(XFSM_DONT_CARE);
    results311.push_back(XFSM_DONT_CARE);
    results311.push_back(XFSM_DONT_CARE);
    results311.push_back(XFSM_DONT_CARE);
    results311.push_back(XFSM_DONT_CARE);
    results311.push_back(XFSM_DONT_CARE);
    results311.push_back(XFSM_DONT_CARE);
    results311.push_back(XFSM_TRUE);

    std::vector<XfsmAction> actions311;
    //set the XFSM_SOCKETISCLOSED register to 1
    XfsmAction setSocketClosed = XfsmAction(XFSM_SUM, XFSM_ZERO, (uint32_t) 1, XFSM_SOCKETISCLOSED);
    actions311.push_back(setSocketClosed);

    TableEntry te321 = TableEntry(results311, 0, 0, actions311, 0, XFSM_CLOSE);
    TableEntry te322 = TableEntry(results311, 1, 1, actions311, 0, XFSM_CLOSE);
    TableEntry te323 = TableEntry(results311, 2, 2, actions311, 0, XFSM_CLOSE);
    TableEntry te324 = TableEntry(results311, 3, 3, actions311, 0, XFSM_CLOSE);

    table.push_back(te321);
    table.push_back(te322);
    table.push_back(te323);
    table.push_back(te324);

    //12. In any state, an ack arrives. It acks everything i've send and the socket told me to close, so i send FIN

    std::vector<Results_t> results312;

    results312.push_back(XFSM_DONT_CARE);
    results312.push_back(XFSM_DONT_CARE);
    results312.push_back(XFSM_DONT_CARE);
    results312.push_back(XFSM_DONT_CARE);
    results312.push_back(XFSM_DONT_CARE);
    results312.push_back(XFSM_DONT_CARE);
    results312.push_back(XFSM_DONT_CARE);
    results312.push_back(XFSM_DONT_CARE);
    results312.push_back(XFSM_DONT_CARE);
    results312.push_back(XFSM_DONT_CARE);
    results312.push_back(XFSM_DONT_CARE);
    results312.push_back(XFSM_TRUE); // i have to close the socket
    results312.push_back(XFSM_FALSE); //No more data to send
    results312.push_back(XFSM_TRUE);

    std::vector<XfsmAction> actions312;
    actions312.push_back(clearRegister);
    actions312.push_back(setFinFlag);
    actions312.push_back(sendEmptyPacket);
    actions312.push_back(setRtt);
    actions312.push_back(setGenTimer306_2);
    actions312.push_back(setGenTimer306_3);


    TableEntry te325 = TableEntry(results312, 3, XFSM_FIN_WAIT1, actions312, 0, XFSM_ACK);
    TableEntry te326 = TableEntry(results312, 2, XFSM_FIN_WAIT1, actions312, 0, XFSM_ACK);
    TableEntry te327 = TableEntry(results312, 1, XFSM_FIN_WAIT1, actions312, 0, XFSM_ACK);
    TableEntry te328 = TableEntry(results312, 0, XFSM_FIN_WAIT1, actions312, 0, XFSM_ACK);

    TableEntry te334 = TableEntry(results312, 3, XFSM_FIN_WAIT1, actions312, 0, XFSM_TIMEOUT);
    TableEntry te335 = TableEntry(results312, 2, XFSM_FIN_WAIT1, actions312, 0, XFSM_TIMEOUT);
    TableEntry te336 = TableEntry(results312, 1, XFSM_FIN_WAIT1, actions312, 0, XFSM_TIMEOUT);
    TableEntry te337 = TableEntry(results312, 0, XFSM_FIN_WAIT1, actions312, 0, XFSM_TIMEOUT);

    TableEntry te338 = TableEntry(results312, 3, XFSM_FIN_WAIT1, actions312, 0, XFSM_GENERIC_TIMEOUT);
    TableEntry te339 = TableEntry(results312, 2, XFSM_FIN_WAIT1, actions312, 0, XFSM_GENERIC_TIMEOUT);
    TableEntry te340 = TableEntry(results312, 1, XFSM_FIN_WAIT1, actions312, 0, XFSM_GENERIC_TIMEOUT);
    TableEntry te341 = TableEntry(results312, 0, XFSM_FIN_WAIT1, actions312, 0, XFSM_GENERIC_TIMEOUT);

    table.push_back(te325);
    table.push_back(te326);
    table.push_back(te327);
    table.push_back(te328);

    table.push_back(te334);
    table.push_back(te335);
    table.push_back(te336);
    table.push_back(te337);

    table.push_back(te338);
    table.push_back(te339);
    table.push_back(te340);
    table.push_back(te341);


    //13. timer for FIN fires, so i resend it

    std::vector<Results_t> results313;
    results313.push_back(XFSM_DONT_CARE);
    results313.push_back(XFSM_DONT_CARE);
    results313.push_back(XFSM_FALSE); //timeout is a good one!

    TableEntry te329 = TableEntry(results313, XFSM_FIN_WAIT1, XFSM_FIN_WAIT1, actions312, 0, XFSM_TIMEOUT);
    table.push_back(te329);    


    //14. In any state, if the socket tells me to close and i've already send everything, i can send FIN

    std::vector<Results_t> results314;
    results314.push_back(XFSM_DONT_CARE);
    results314.push_back(XFSM_DONT_CARE);
    results314.push_back(XFSM_DONT_CARE);
    results314.push_back(XFSM_DONT_CARE);
    results314.push_back(XFSM_DONT_CARE);
    results314.push_back(XFSM_DONT_CARE);
    results314.push_back(XFSM_DONT_CARE);
    results314.push_back(XFSM_DONT_CARE);
    results314.push_back(XFSM_DONT_CARE);
    results314.push_back(XFSM_DONT_CARE);
    results314.push_back(XFSM_DONT_CARE);
    results314.push_back(XFSM_DONT_CARE);
    results314.push_back(XFSM_FALSE); //Nothing more to send

    std::vector<XfsmAction> actions314;
    //set the XFSM_SOCKETISCLOSED register to 1 & send FIN
    actions314.push_back(setSocketClosed);
    actions314.push_back(clearRegister);
    actions314.push_back(setFinFlag);
    actions314.push_back(sendEmptyPacket);
    actions314.push_back(setRtt);
    actions314.push_back(setGenTimer306_2);
    actions314.push_back(setGenTimer306_3);

    TableEntry te330 = TableEntry(results314, 0, 0, actions314, 0, XFSM_CLOSE);
    TableEntry te331 = TableEntry(results314, 1, 1, actions314, 0, XFSM_CLOSE);
    TableEntry te332 = TableEntry(results314, 2, 2, actions314, 0, XFSM_CLOSE);
    TableEntry te333 = TableEntry(results314, 3, 3, actions314, 0, XFSM_CLOSE);

    table.push_back(te330);
    table.push_back(te331);
    table.push_back(te332);
    table.push_back(te333);


    // ********** BORING TCP CONNECTION ESTABLISHMENT AND CLOSING PART ENDS HERE *************



    ns3TcpSocket->SetXfsm(new TcpXfsm(table, conditionList, 300));


    /****************************************************/
    /* End TCP XFSM configuration (sender side)         */
    /****************************************************/


    Ptr<MyApp> app = CreateObject<MyApp> ();
    // super greedy sender: this puts blocks of 20*MSS size into the send buffer.
    //    app->Setup (ns3TcpSocket, sinkAddress, TCPMSS*20, 1000000, DataRate ("1Gbps"));
    app->Setup (ns3TcpSocket, sinkAddress, TCPMSS*APPBLOCKSIZE, APPNUMBLOCKS, DataRate (APPRATE));
    d.GetLeft (0)->AddApplication (app);
    

    /****************************************************/
    // Initialize interference: errors, bg traffic
    /****************************************************/
    
    // the list of packets to drop, numbering them from 0
    std::list<uint32_t> lostPackets;
    // not dropping any right now
    int numDrops = sizeof(PACKETS_TO_DROP)/sizeof(int);
    if(numDrops>0) {
        for(int i=0; i<numDrops; i++)
        lostPackets.push_back(PACKETS_TO_DROP[i]);
    }
    //lostPackets.push_back(5);
    
    Ptr<ReceiveListErrorModel> pem = CreateObject<ReceiveListErrorModel> ();
    pem->SetList (lostPackets);
    d.GetRight (0)->GetDevice (0)->SetAttribute ("ReceiveErrorModel", PointerValue (pem));
    
    // ---------------------------------------------------------------------
    
    
    
    
    /*
     // Background traffic:
     // Install on/off app on all left side nodes except node 0
     OnOffHelper clientHelper ("ns3::UdpSocketFactory", Address ());
     clientHelper.SetAttribute ("OnTime", StringValue ("ns3::UniformRandomVariable"));
     clientHelper.SetAttribute ("OffTime", StringValue ("ns3::UniformRandomVariable"));
     ApplicationContainer clientApps;
     
     for (uint32_t i = 1; i < d.LeftCount (); ++i)
     {
     // Create an on/off app sending packets to the same leaf right side
     AddressValue remoteAddress (InetSocketAddress (d.GetRightIpv4Address (i), 1000));
     clientHelper.SetAttribute ("Remote", remoteAddress);
     clientApps.Add (clientHelper.Install (d.GetLeft (i)));
     }
     clientApps.Start (Seconds (0.0));
     clientApps.Stop (Seconds (10.0));
     */

    
    /****************************************************/
    // Tracing
    /****************************************************/
    
    // cwnd
    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> cwndStream = asciiTraceHelper.CreateFileStream ("cwnd.txt");
    ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", MakeBoundCallback (&CwndChange, cwndStream));

    // nextseq - don't use this for a time-seq plot, it's misleading
    //
    //    Ptr<OutputStreamWrapper> seqStream = asciiTraceHelper.CreateFileStream ("seq.txt");
    //    ns3TcpSocket->TraceConnectWithoutContext ("NextTxSequence", MakeBoundCallback (&NextSeqChange, seqStream));
    
    // qlen
    /*Ptr<OutputStreamWrapper> qlenStream = asciiTraceHelper.CreateFileStream ("qlen.txt");
    Ptr<TrafficControlLayer> tc = d.GetLeft()->GetObject<TrafficControlLayer> ();
    Ptr< QueueDisc > q = tc->GetRootQueueDiscOnDevice (d.GetLeft()->GetDevice(0));
    q->TraceConnectWithoutContext ("PacketsInQueue", MakeBoundCallback (&QlenChange, qlenStream));
    */
    // pcap
    pointToPointLeaf.EnablePcap ("tcpsender", d.GetLeft (0)->GetId (), 0);
    pointToPointLeaf.EnablePcap("tcpreceiver", d.GetRight(0)->GetId(), 0);
    
    // anim
/*    d.BoundingBox (1, 1, 100, 100);
    AnimationInterface anim ("anim.xml");
    anim.EnablePacketMetadata (); // Optional
    anim.EnableIpv4L3ProtocolCounters (Seconds (0), Seconds (20)); // Optional
*/
    
    /****************************************************/
    // Schedule events
    /****************************************************/
    
    // TCP
    app->SetStartTime (Seconds (0.));
    app->SetStopTime (Seconds (SIMDURATION-3));
     
    sinkAppA.Start (Seconds (0.));
    sinkAppA.Stop (Seconds (SIMDURATION));
    

    
    Simulator::Stop (Seconds (SIMDURATION+2));
    Simulator::Run ();
    
    Ptr<PacketSink> sink1 = DynamicCast<PacketSink> (sinkAppA.Get (0));
    std::cout << "Total Bytes Received from sender: " << sink1->GetTotalRx () << std::endl;
    //    std::cout << "Animation Trace file created:" << animFile.c_str ()<< std::endl;
    std::cout << "Simulation finished." << std::endl;
    Simulator::Destroy ();
    return 0;
}