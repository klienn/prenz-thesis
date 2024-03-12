int tx_addr = 0;

// With FIXED SENDER configuration
int ADDR_LIST[] = { 100, 101, 102 , 103, 104, 105, 106 , 107 , 108, 109, 110, 111, 112, 113, 114, 115 };
int MY_ADDR = ADDR_LIST[tx_addr] ;


//char SLAVE_TOPIC[12][12] = { "BaseStation", "LRC_", "SciCom_", "EnggCom_42", "EnggCom_43", "Gym_", "Cafet_", "Admin_", "ICT_", "Finance_", "COT_", "FabLab_" };
//char LOCATION = SLAVE_TOPIC[][2];

#define REQUEST 0
#define FORWARD 1
#define RECEIVE 2

#define CHAN    23
#define H_ADDR   0

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

