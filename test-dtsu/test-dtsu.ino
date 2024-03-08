#include "Arduino.h"
#include <ModbusRTU.h>
#define SLAVE_ID 1
#define FIRST_REG 0x2006  //  starting address of Holding register to read
#define REG_COUNT 36      // number of registers to read
int DE_RE = 4;            //D2  For MAX485 chip

ModbusRTU mb;

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
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial1.begin(9600, SERIAL_8N1, 22, 23);
  mb.begin(&Serial1, DE_RE);  //Assing Software serial port to Modbus instance for MAX485 chip having DI,DE,RE,RO Pin at TTL side
  mb.master();                //Assing Modbus function as master
  // put your setup code here, to run once:
}

void loop() {
  uint16_t res[REG_COUNT];
  if (!mb.slave()) {                                       // Check if no transaction in progress
    mb.readHreg(SLAVE_ID, FIRST_REG, res, REG_COUNT, cb);  // Send Read Hreg from Modbus Server
    while (mb.slave()) {                                   // Check if transaction is active
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
  }
  delay(1000);
  // put your main code here, to run repeatedly:
}
