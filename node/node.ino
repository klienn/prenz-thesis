#include "Arduino.h"
#include "LoRa_E22.h"
#include "eMon_structs.h"

#include <ModbusRTU.h>
#define SLAVE_ID 1
#define FIRST_REG 0x2006  //  starting address of Holding register to read
#define REG_COUNT 36      // number of registers to read
int DE_RE = 4;            //D2  For MAX485 chip

ModbusRTU mb;

LoRa_E22 e22ttl(&Serial2, 18, 21, 19);  //  RX AUX M0 M1

union {  // variables in the union shares the same memory location
  uint16_t i[2];
  float f;
} datamod;
bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) {  // Callback to monitor errors
  Serial.println(event);
  if (event != Modbus::EX_SUCCESS) {
    Serial.print("Request result: 0x");
    Serial.print(event, HEX);
  }
  return true;
}

unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 5000;        // interval at which to blink (milliseconds)
int tx_command = 0;
int addr;  //0 of base_station,
int rx_addr;
boolean send_flag = false;
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
float qc, pft, pfa, pfb, pfc;
String bldg = "cafet";  //CHANGE BLDG
struct Message rxmessage;

void setup() {
  Serial.begin(115200);
  delay(500);

  // Startup all pins and UART
  e22ttl.begin();

  Serial.print("Hi, I'm: ");
  Serial.println(MY_ADDR);

  Serial1.begin(9600, SERIAL_8N1, 22, 23);
  mb.begin(&Serial1, DE_RE);  //Assing Software serial port to Modbus instance for MAX485 chip having DI,DE,RE,RO Pin at TTL side
  mb.master();                //Assing Modbus function as master
}

void loop() {
  uint16_t res[REG_COUNT];
  unsigned long currentMillis = millis();
  // If something available
  if (e22ttl.available() > 1) {
    Serial.println("start e220");
    // read the String message
    ResponseStructContainer rsc = e22ttl.receiveMessageRSSI(sizeof(Message));  //#ifdef ENABLE_RSSIrsc
    // Is something goes wrong print error
    if (rsc.status.code != 1) {
      Serial.println("receive error");
      Serial.print(rsc.status.getResponseDescription());
      Serial.print(": ");
    } else {
      // Print the data received
      Serial.print(rsc.status.getResponseDescription());
      Serial.print(": ");
      rxmessage = *(Message*)rsc.data;

      Serial.print(rxmessage.txAddr);
      Serial.print(", ");
      Serial.print(rxmessage.rxAddr);
      Serial.print(", ");
      Serial.print(rxmessage.command);
      Serial.print(", RSSI: ");
      Serial.println(rsc.rssi, DEC);

      if ((rxmessage.rxAddr == tx_addr) && (rxmessage.command == REQUEST)) {
        send_flag = true;
        if (rxmessage.txAddr == base_addr) {  // from base to this node
          tx_command = RECEIVE;
          Serial.print("Sending to base:");
        } else {
          tx_command = FORWARD;
          Serial.print("Sending to node:");
        }
      }
    }
  } else {
    if (send_flag) {  // Reply to base the sensor readings
      struct Message txmessage = { loopCount, tx_addr, base_addr, tx_command, ua, ub, uc, ia, ib, ic, pt, pa, pb, pc, qt, qa, qb, qc, pft, pfa, pfb, pfc, bldg };
      ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rxmessage.txAddr], CHAN, &txmessage, sizeof(Message));  // return to the sender which is rxmessage.txAddr
      Serial.println(rs.getResponseDescription());
      delay(1000);
      Serial.print(txmessage.counter);
      Serial.print(", ");
      Serial.print(txmessage.txAddr);
      Serial.print(", ");
      Serial.print(txmessage.rxAddr);
      Serial.print(", ");
      Serial.print(txmessage.command);
      Serial.print(", ");
      Serial.print(txmessage.ua);
      Serial.print(", ");
      Serial.print(txmessage.ub);
      Serial.print(", ");
      Serial.print(txmessage.uc);
      Serial.print(", ");
      Serial.print(txmessage.ia);
      Serial.print(", ");
      Serial.print(txmessage.ib);
      Serial.print(", ");
      Serial.print(txmessage.ic);
      Serial.print(", ");
      Serial.print(txmessage.pt);
      Serial.print(", ");
      Serial.print(txmessage.pa);
      Serial.print(", ");
      Serial.print(txmessage.pb);
      Serial.print(", ");
      Serial.print(txmessage.pc);
      Serial.print(", ");
      Serial.print(txmessage.qt);
      Serial.print(", ");
      Serial.print(txmessage.qa);
      Serial.print(", ");
      Serial.print(txmessage.qb);
      Serial.print(", ");
      Serial.print(txmessage.qc);
      Serial.print(", ");
      Serial.print(txmessage.pft);
      Serial.print(", ");
      Serial.print(txmessage.pfa);
      Serial.print(", ");
      Serial.print(txmessage.pfb);
      Serial.print(", ");
      Serial.print(txmessage.pfc);
      Serial.print(", ");
      Serial.print(txmessage.bldg);
      Serial.print(": ");
      Serial.println(rs.getResponseDescription());
      send_flag = false;
    }
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      Serial.println("start data collect;");
      if (!mb.slave()) {                                       // Check if no transaction in progress
        mb.readHreg(SLAVE_ID, FIRST_REG, res, REG_COUNT, cb);  // Send Read Hreg from Modbus Server
        while (mb.slave()) {                             // Check if transaction is active
          mb.task();
          delay(10);
        }
        //VOLT
        datamod.i[0] = res[1];
        datamod.i[1] = res[0];
        ua = datamod.f / 10;
        datamod.i[0] = res[3];
        datamod.i[1] = res[2];
        ub = datamod.f / 10;
        datamod.i[0] = res[5];
        datamod.i[1] = res[4];
        uc = datamod.f / 10;
        Serial.println("Voltage:");
        Serial.println(ua);
        Serial.println(ub);
        Serial.println(uc);

        //CURRENT
        datamod.i[0] = res[7];
        datamod.i[1] = res[6];
        ia = datamod.f / 10;
        datamod.i[0] = res[9];
        datamod.i[1] = res[8];
        ib = datamod.f / 10;
        datamod.i[0] = res[11];
        datamod.i[1] = res[10];
        ic = datamod.f / 10;
        Serial.println("Current:");
        Serial.println(ia);
        Serial.println(ib);
        Serial.println(ic);

        Serial.println("Active Power:");
        //ACTIVE POWER
        datamod.i[0] = res[13];
        datamod.i[1] = res[12];
        pt = datamod.f;
        datamod.i[0] = res[15];
        datamod.i[1] = res[14];
        pa = datamod.f;
        datamod.i[0] = res[17];
        datamod.i[1] = res[16];
        pb = datamod.f;
        datamod.i[0] = res[19];
        datamod.i[1] = res[18];
        pc = datamod.f;

        Serial.println(pt);
        Serial.println(pa);
        Serial.println(pb);
        Serial.println(pc);

        //REACTIVE POWER
        datamod.i[0] = res[21];
        datamod.i[1] = res[20];
        qt = datamod.f / 10;
        datamod.i[0] = res[23];
        datamod.i[1] = res[22];
        qa = datamod.f / 10;
        datamod.i[0] = res[25];
        datamod.i[1] = res[24];
        qb = datamod.f / 10;
        datamod.i[0] = res[27];
        datamod.i[1] = res[26];
        qc = datamod.f / 10;
        Serial.println("Reactive Power:");
        Serial.println(qt);
        Serial.println(qa);
        Serial.println(qb);
        Serial.println(qc);

        //powerFactor
        datamod.i[0] = res[29];
        datamod.i[1] = res[28];
        pft = datamod.f / 10;
        datamod.i[0] = res[31];
        datamod.i[1] = res[30];
        pfa = datamod.f / 10;
        datamod.i[0] = res[33];
        datamod.i[1] = res[32];
        pfb = datamod.f / 10;
        datamod.i[0] = res[35];
        datamod.i[1] = res[34];
        pfc = datamod.f / 10;
        Serial.println("pf:");
        Serial.println(pft);
        Serial.println(pfa);
        Serial.println(pfb);
        Serial.println(pfc);

        Serial.println("");
        loopCount++;
      }
    }
  }
}
