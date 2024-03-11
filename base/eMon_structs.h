int tx_addr = 0;

// With FIXED SENDER configuration
// int ADDR_LIST[] = { 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72 };
int ADDR_LIST[] = { 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114 };
int MY_ADDR = ADDR_LIST[tx_addr] ;

#define REQUEST 0
#define FORWARD 1
#define RECEIVE 2

#define CHAN    23
#define H_ADDR   20

#define ENABLE_RSSI true

unsigned int loopCount = 0;

struct Message {
  unsigned int counter;
  byte txAddr;
  byte rxAddr;
  int command;
  float ua;
  float ub;
  float uc;
  float ia;
  float ib;
  float ic;
  float pt;
  float pa;
  float pb;
  float pc;
  float qt;
  float qa;
  float qb;
  float qc;
  float pft;
  float pfa;
  float pfb;
  float pfc;
  String bldg;
};
