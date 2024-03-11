#include "Arduino.h"
#include "LoRa_E22.h"
#include "eMon_structs.h"

LoRa_E22 e22ttl(&Serial2, 18, 21, 19);  //  RX AUX M0 M1

unsigned long previousMillislrc = 0, previousMillisbldg24 = 0, previousMillisfic = 0, previousMillisscicom = 0, previousMillisoldscicom = 0, previousMilliscafet = 0, previousMillisecom = 0, previousMillisit = 0, previousMillisgym = 0, previousMillisict = 0, previousMillisadmin = 0, previousMilliscot = 0, previousMillisfablab = 0;  // will store last time LED was updated
const long intervallrc = 5000, intervalbldg24 = 5050, intervalfic = 5100, intervalscicom = 5150, intervaloldscicom = 5200, intervalcafet = 5250, intervalecom = 5300, intervalit = 5350, intervalgym = 5400, intervalict = 5450, intervaladmin = 5500, intervalcot = 5550, intervalfablab = 5600;                                            // interval at which to blink (milliseconds)
int rx_addrlrc = 1, rx_addrbldg24 = 2, rx_addrfic = 4, rx_addrscicom = 5, rx_addroldscicom = 6, rx_addrcafet = 7, rx_addrecom = 8, rx_addrit = 9, rx_addrgym = 10, rx_addrict = 11, rx_addradmin = 12, rx_addrcot = 13, rx_addrfablab = 14;
int command = 0;

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
float sa;
float sb;
float sc, pft, pfa, pfb, pfc;

unsigned long currentMillis = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  while (!Serial)
    ;

  // Startup all pins and UART
  e22ttl.begin();

  Serial.print("Hi, I'm: ");
  Serial.println(MY_ADDR);
}

void loop() {
  lrc();
  delay(5000);
  check();
}

void check() {
  if (e22ttl.available() > 1) {
    // read the String message
    Serial.println("received, decrypting...");
    ResponseStructContainer rsc = e22ttl.receiveMessageRSSI(sizeof(Message));  //#ifdef ENABLE_RSSI
    // Is something goes wrong print error
    if (rsc.status.code != 1) {
      Serial.println("error:");
      Serial.println(rsc.status.getResponseDescription());
    } else {
      // Print the data received
      Serial.print(rsc.status.getResponseDescription());
      Serial.print(": ");
      struct Message rxmessage = *(Message*)rsc.data;
      Serial.print(rxmessage.counter);
      Serial.print(", ");
      Serial.print(rxmessage.txAddr);
      Serial.print(", ");
      Serial.print(rxmessage.rxAddr);
      Serial.print(", ");
      Serial.print(rxmessage.command);
      Serial.print(", ");
      Serial.print(rxmessage.ua);
      Serial.print(", ");
      Serial.print(rxmessage.ub);
      Serial.print(", ");
      Serial.print(rxmessage.uc);
      Serial.print(", ");
      Serial.print(rxmessage.ia);
      Serial.print(", ");
      Serial.print(rxmessage.ib);
      Serial.print(", ");
      Serial.print(rxmessage.ic);
      Serial.print(", ");
      Serial.print(rxmessage.pt);
      Serial.print(", ");
      Serial.print(rxmessage.pa);
      Serial.print(", ");
      Serial.print(rxmessage.pb);
      Serial.print(", ");
      Serial.print(rxmessage.pc);
      Serial.print(", ");
      Serial.print(rxmessage.qt);
      Serial.print(", ");
      Serial.print(rxmessage.qa);
      Serial.print(", ");
      Serial.print(rxmessage.qb);
      Serial.print(", ");
      Serial.print(rxmessage.qc);

      Serial.print(", RSSI: ");
      Serial.println(rsc.rssi, DEC);
      Serial.println(" ");
      if (rxmessage.command == 1) {
        ua = rxmessage.ua;
        ub = rxmessage.ub;
        uc = rxmessage.uc;

        ia = rxmessage.ia;
        ib = rxmessage.ib;
        ic = rxmessage.ic;

        pt = rxmessage.pt;
        pa = rxmessage.pa;
        pb = rxmessage.pb;
        pc = rxmessage.pc;

        qt = rxmessage.qt;
        qa = rxmessage.qa;
        qb = rxmessage.qb;
        qc = rxmessage.qc;

        pft = rxmessage.pft;
        pfa = rxmessage.pfa;
        pfb = rxmessage.pfb;
        pfc = rxmessage.pfc;

        sa = ua * ia;
        sb = ub * ib;
        sc = uc * ic;
      }
    }
  }
}

void lrc() {
  if (rx_addrlrc > 2) rx_addrlrc =1;

  if (rx_addrlrc == 1) {
    Serial.print("Requesting to: ");
    Serial.println(rx_addrlrc);
    struct Message txmessage = { loopCount, tx_addr, rx_addrlrc, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addrlrc], CHAN, &txmessage, sizeof(Message));
    //Serial.println(rs.getResponseDescription());
    // Serial.print(rxmessage.counter);
    //Serial.print(", ");
    // Serial.print(ADDR_LIST[tx_addr]);
    // Serial.print(", ");
    // Serial.print(ADDR_LIST[rx_addr]);
    // Serial.print(", ");
    // Serial.print(rxmessage.Addr);
    // Serial.print(", ");
    // Serial.println(rxmessage.command);
  } else {
    Serial.println("Requesting to 1 via 1");
    struct Message txmessage = { loopCount, tx_addr, 2, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    //      Serial.println(rs.getResponseDescription());
    //      Serial.print(txmessage.counter);
    //      Serial.print(", ");
    //      Serial.print(txmessage.txAddr);
    //      Serial.print(", ");
    //      Serial.print(txmessage.rxAddr);
    //      Serial.print(", ");
    //      Serial.print(txmessage.command);
    //      Serial.print(", ");
    //      Serial.print(txmessage.ua);
    //      Serial.print(", ");
    //      Serial.print(txmessage.ub);
    //      Serial.print(", ");
    //      Serial.print(txmessage.uc);
    //      Serial.print(", ");
    //      Serial.print(txmessage.ia);
    //      Serial.print(", ");
    //      Serial.print(txmessage.ib);
    //      Serial.print(", ");
    //      Serial.print(txmessage.ic);
    //      Serial.print(", ");
    //      Serial.print(txmessage.pt);
    //      Serial.print(", ");
    //      Serial.print(txmessage.pa);
    //      Serial.print(", ");
    //      Serial.print(txmessage.pb);
    //      Serial.print(", ");
    //      Serial.print(txmessage.pc);
    //      Serial.print(", ");
    //      Serial.print(txmessage.qt);
    //      Serial.print(", ");
    //      Serial.print(txmessage.qa);
    //      Serial.print(", ");
    //      Serial.print(txmessage.qb);
    //      Serial.print(", ");
    //      Serial.print(txmessage.qc);
  }
  rx_addrlrc++;
  loopCount++;
}

void bldg24() {

  if (rx_addrbldg24 > 3) rx_addrbldg24 = 2;

  if (rx_addrbldg24 == 2) {
    Serial.print("Requesting to ");
    Serial.println(rx_addrbldg24);
    struct Message txmessage = { loopCount, tx_addr, rx_addrbldg24, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addrbldg24], CHAN, &txmessage, sizeof(Message));
    //Serial.println(rs.getResponseDescription());
    // Serial.print(rxmessage.counter);
    //Serial.print(", ");
    // Serial.print(ADDR_LIST[tx_addr]);
    // Serial.print(", ");
    // Serial.print(ADDR_LIST[rx_addr]);
    // Serial.print(", ");
    // Serial.print(rxmessage.Addr);
    // Serial.print(", ");
    // Serial.println(rxmessage.command);
  } else {
    Serial.println("Requesting to 2 via 1");
    struct Message txmessage = { loopCount, tx_addr, 3, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    //      Serial.println(rs.getResponseDescription());
    //      Serial.print(txmessage.counter);
    //      Serial.print(", ");
    //      Serial.print(txmessage.txAddr);
    //      Serial.print(", ");
    //      Serial.print(txmessage.rxAddr);
    //      Serial.print(", ");
    //      Serial.print(txmessage.command);
    //      Serial.print(", ");
    //      Serial.print(txmessage.ua);
    //      Serial.print(", ");
    //      Serial.print(txmessage.ub);
    //      Serial.print(", ");
    //      Serial.print(txmessage.uc);
    //      Serial.print(", ");
    //      Serial.print(txmessage.ia);
    //      Serial.print(", ");
    //      Serial.print(txmessage.ib);
    //      Serial.print(", ");
    //      Serial.print(txmessage.ic);
    //      Serial.print(", ");
    //      Serial.print(txmessage.pt);
    //      Serial.print(", ");
    //      Serial.print(txmessage.pa);
    //      Serial.print(", ");
    //      Serial.print(txmessage.pb);
    //      Serial.print(", ");
    //      Serial.print(txmessage.pc);
    //      Serial.print(", ");
    //      Serial.print(txmessage.qt);
    //      Serial.print(", ");
    //      Serial.print(txmessage.qa);
    //      Serial.print(", ");
    //      Serial.print(txmessage.qb);
    //      Serial.print(", ");
    //      Serial.print(txmessage.qc);
  }
  rx_addrbldg24++;
  loopCount++;
}
