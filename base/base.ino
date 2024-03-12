#include "Arduino.h"
#include "LoRa_E22.h"
#include "eMon_structs.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

LoRa_E22 e22ttl(&Serial2, 18, 21, 19);  //  RX AUX M0 M1

const long intervallrc = 5000, intervalbldg24 = 5000, intervalfic = 15000, intervalscicom = 20000, intervaloldscicom = 25000, intervalcafet = 30000, intervalecom = 35000, intervalit = 40000, intervalgym = 45000, intervalict = 50000, intervaladmin = 55000, intervalcot = 60000, intervalfablab = 65000;  // interval at which to blink (milliseconds)
int rx_addrlrc = 1, rx_addrbldg24 = 3, rx_addrfic = 4, rx_addrscicom = 5, rx_addroldscicom = 6, rx_addrcafet = 7, rx_addrecom = 8, rx_addrit = 9, rx_addrgym = 10, rx_addrict = 11, rx_addradmin = 12, rx_addrcot = 13, rx_addrfablab = 14;
int command = 0;

float ualrc, ublrc, uclrc, ialrc, iblrc, iclrc, ptlrc, palrc, pblrc, pclrc, qtlrc, qalrc, qblrc, qclrc, salrc, sblrc, sclrc, pftlrc, pfalrc, pfblrc, pfclrc;
float uabldg24, ubbldg24, ucbldg24, iabldg24, ibbldg24, icbldg24, ptbldg24, pabldg24, pbbldg24, pcbldg24, qtbldg24, qabldg24, qbbldg24, qcbldg24, sabldg24, sbbldg24, scbldg24, pftbldg24, pfabldg24, pfbbldg24, pfcbldg24;
float uafic, ubfic, ucfic, iafic, ibfic, icfic, ptfic, pafic, pbfic, pcfic, qtfic, qafic, qbfic, qcfic, safic, sbfic, scfic, pftfic, pfafic, pfbfic, pfcfic;
float uascicom, ubscicom, ucscicom, iascicom, ibscicom, icscicom, ptscicom, pascicom, pbscicom, pcscicom, qtscicom, qascicom, qbscicom, qcscicom, sascicom, sbscicom, scscicom, pftscicom, pfascicom, pfbscicom, pfcscicom;
float uaoldscicom, uboldscicom, ucoldscicom, iaoldscicom, iboldscicom, icoldscicom, ptoldscicom, paoldscicom, pboldscicom, pcoldscicom, qtoldscicom, qaoldscicom, qboldscicom, qcoldscicom, saoldscicom, sboldscicom, scoldscicom, pftoldscicom, pfaoldscicom, pfboldscicom, pfcoldscicom;
float uacafet, ubcafet, uccafet, iacafet, ibcafet, iccafet, ptcafet, pacafet, pbcafet, pccafet, qtcafet, qacafet, qbcafet, qccafet, sacafet, sbcafet, sccafet, pftcafet, pfacafet, pfbcafet, pfccafet;
float uaecom, ubecom, ucecom, iaecom, ibecom, icecom, ptecom, paecom, pbecom, pcecom, qtecom, qaecom, qbecom, qcecom, saecom, sbecom, scecom, pftecom, pfaecom, pfbecom, pfcecom;
float uait, ubit, ucit, iait, ibit, icit, ptit, pait, pbit, pcit, qtit, qait, qbit, qcit, sait, sbit, scit, pftit, pfait, pfbit, pfcit;
float uagym, ubgym, ucgym, iagym, ibgym, icgym, ptgym, pagym, pbgym, pcgym, qtgym, qagym, qbgym, qcgym, sagym, sbgym, scgym, pftgym, pfagym, pfbgym, pfcgym;
float uaict, ubict, ucict, iaict, ibict, icict, ptict, paict, pbict, pcict, qtict, qaict, qbict, qcict, saict, sbict, scict, pftict, pfaict, pfbict, pfcict;
float uaadmin, ubadmin, ucadmin, iaadmin, ibadmin, icadmin, ptadmin, paadmin, pbadmin, pcadmin, qtadmin, qaadmin, qbadmin, qcadmin, saadmin, sbadmin, scadmin, pftadmin, pfaadmin, pfbadmin, pfcadmin;
float uacot, ubcot, uccot, iacot, ibcot, iccot, ptcot, pacot, pbcot, pccot, qtcot, qacot, qbcot, qccot, sacot, sbcot, sccot, pftcot, pfacot, pfbcot, pfccot;
float uafablab, ubfablab, ucfablab, iafablab, ibfablab, icfablab, ptfablab, pafablab, pbfablab, pcfablab, qtfablab, qafablab, qbfablab, qcfablab, safablab, sbfablab, scfablab, pftfablab, pfafablab, pfbfablab, pfcfablab;

int ping[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

unsigned long currentMillis = 0;
const char* ssid = "EEDEPT_Thesis";
const char* password = "thesisee";

// IPAddress local_IP(192, 168, 123, 201);
// // Set your Gateway IP address
// IPAddress gateway(192, 168, 123, 253);

// IPAddress subnet(255, 255, 255, 0);
// IPAddress primaryDNS(8, 8, 8, 8);    //optional
// IPAddress secondaryDNS(8, 8, 4, 4);  //optional
// //
// AsyncWebServer server(80);


String GenerateMetricslrc() {
  String message = "";

  message += "# Smart meter voltage \n";
  message += "esp_voltage_va ";
  message += ualrc;
  message += "\n";
  message += "esp_voltage_vb ";
  message += ublrc;
  message += "\n";
  message += "esp_voltage_vc ";
  message += uclrc;
  message += "\n";

  message += "# Smart meter current \n";
  message += "esp_current_ia ";
  message += ialrc;
  message += "\n";
  message += "esp_current_ib ";
  message += iblrc;
  message += "\n";
  message += "esp_current_ic ";
  message += iclrc;
  message += "\n";

  message += "# Smart meter active power\n";
  message += "esp_activePower_pt ";
  message += ptlrc;
  message += "\n";
  message += "esp_activePower_pa ";
  message += palrc;
  message += "\n";
  message += "esp_activePower_pb ";
  message += pblrc;
  message += "\n";
  message += "esp_activePower_pc ";
  message += pclrc;
  message += "\n";

  message += "# Smart meter reactive power\n";
  message += "esp_activePower_qt ";
  message += qtlrc;
  message += "\n";
  message += "esp_reactivePower_qa ";
  message += qalrc;
  message += "\n";
  message += "esp_reactivePower_qb ";
  message += qblrc;
  message += "\n";
  message += "esp_reactivePower_qc ";
  message += qclrc;
  message += "\n";

  message += "# apparent power\n";
  message += "esp_apparentPower_sa ";
  message += salrc;
  message += "\n";
  message += "esp_apparentPower_sb ";
  message += sblrc;
  message += "\n";
  message += "esp_apparentPower_sc ";
  message += sclrc;
  message += "\n";

  message += "# power factor\n";
  message += "esp_powerfactor_pft ";
  message += pftlrc;
  message += "\n";
  message += "esp_powerfactor_pfa ";
  message += pfalrc;
  message += "\n";
  message += "esp_powerfactor_pfb ";
  message += pfblrc;
  message += "\n";
  message += "esp_powerfactor_pfc ";
  message += pfclrc;
  message += "\n";
  return message;
}
String GenerateMetricsbldg24() {
  String message = "";

  message += "# Smart meter voltage \n";
  message += "esp_voltage_va ";
  message += uabldg24;
  message += "\n";
  message += "esp_voltage_vb ";
  message += ubbldg24;
  message += "\n";
  message += "esp_voltage_vc ";
  message += ucbldg24;
  message += "\n";

  message += "# Smart meter current \n";
  message += "esp_current_ia ";
  message += iabldg24;
  message += "\n";
  message += "esp_current_ib ";
  message += ibbldg24;
  message += "\n";
  message += "esp_current_ic ";
  message += icbldg24;
  message += "\n";

  message += "# Smart meter active power\n";
  message += "esp_activePower_pt ";
  message += ptbldg24;
  message += "\n";
  message += "esp_activePower_pa ";
  message += pabldg24;
  message += "\n";
  message += "esp_activePower_pb ";
  message += pbbldg24;
  message += "\n";
  message += "esp_activePower_pc ";
  message += pcbldg24;
  message += "\n";

  message += "# Smart meter reactive power\n";
  message += "esp_activePower_qt ";
  message += qtbldg24;
  message += "\n";
  message += "esp_reactivePower_qa ";
  message += qabldg24;
  message += "\n";
  message += "esp_reactivePower_qb ";
  message += qbbldg24;
  message += "\n";
  message += "esp_reactivePower_qc ";
  message += qcbldg24;
  message += "\n";

  message += "# apparent power\n";
  message += "esp_apparentPower_sa ";
  message += sabldg24;
  message += "\n";
  message += "esp_apparentPower_sb ";
  message += sbbldg24;
  message += "\n";
  message += "esp_apparentPower_sc ";
  message += scbldg24;
  message += "\n";

  message += "# power factor\n";
  message += "esp_powerfactor_pft ";
  message += pftbldg24;
  message += "\n";
  message += "esp_powerfactor_pfa ";
  message += pfabldg24;
  message += "\n";
  message += "esp_powerfactor_pfb ";
  message += pfbbldg24;
  message += "\n";
  message += "esp_powerfactor_pfc ";
  message += pfcbldg24;
  message += "\n";
  return message;
}
String GenerateMetricsfic() {
  String message = "";

  message += "# Smart meter voltage \n";
  message += "esp_voltage_va ";
  message += uafic;
  message += "\n";
  message += "esp_voltage_vb ";
  message += ubfic;
  message += "\n";
  message += "esp_voltage_vc ";
  message += ucfic;
  message += "\n";

  message += "# Smart meter current \n";
  message += "esp_current_ia ";
  message += iafic;
  message += "\n";
  message += "esp_current_ib ";
  message += ibfic;
  message += "\n";
  message += "esp_current_ic ";
  message += icfic;
  message += "\n";

  message += "# Smart meter active power\n";
  message += "esp_activePower_pt ";
  message += ptfic;
  message += "\n";
  message += "esp_activePower_pa ";
  message += pafic;
  message += "\n";
  message += "esp_activePower_pb ";
  message += pbfic;
  message += "\n";
  message += "esp_activePower_pc ";
  message += pcfic;
  message += "\n";

  message += "# Smart meter reactive power\n";
  message += "esp_activePower_qt ";
  message += qtfic;
  message += "\n";
  message += "esp_reactivePower_qa ";
  message += qafic;
  message += "\n";
  message += "esp_reactivePower_qb ";
  message += qbfic;
  message += "\n";
  message += "esp_reactivePower_qc ";
  message += qcfic;
  message += "\n";

  message += "# apparent power\n";
  message += "esp_apparentPower_sa ";
  message += safic;
  message += "\n";
  message += "esp_apparentPower_sb ";
  message += sbfic;
  message += "\n";
  message += "esp_apparentPower_sc ";
  message += scfic;
  message += "\n";

  message += "# power factor\n";
  message += "esp_powerfactor_pft ";
  message += pftfic;
  message += "\n";
  message += "esp_powerfactor_pfa ";
  message += pfafic;
  message += "\n";
  message += "esp_powerfactor_pfb ";
  message += pfbfic;
  message += "\n";
  message += "esp_powerfactor_pfc ";
  message += pfcfic;
  message += "\n";
  return message;
}
String GenerateMetricsscicom() {
  String message = "";

  message += "# Smart meter voltage \n";
  message += "esp_voltage_va ";
  message += uascicom;
  message += "\n";
  message += "esp_voltage_vb ";
  message += ubscicom;
  message += "\n";
  message += "esp_voltage_vc ";
  message += ucscicom;
  message += "\n";

  message += "# Smart meter current \n";
  message += "esp_current_ia ";
  message += iascicom;
  message += "\n";
  message += "esp_current_ib ";
  message += ibscicom;
  message += "\n";
  message += "esp_current_ic ";
  message += icscicom;
  message += "\n";

  message += "# Smart meter active power\n";
  message += "esp_activePower_pt ";
  message += ptscicom;
  message += "\n";
  message += "esp_activePower_pa ";
  message += pascicom;
  message += "\n";
  message += "esp_activePower_pb ";
  message += pbscicom;
  message += "\n";
  message += "esp_activePower_pc ";
  message += pcscicom;
  message += "\n";

  message += "# Smart meter reactive power\n";
  message += "esp_activePower_qt ";
  message += qtscicom;
  message += "\n";
  message += "esp_reactivePower_qa ";
  message += qascicom;
  message += "\n";
  message += "esp_reactivePower_qb ";
  message += qbscicom;
  message += "\n";
  message += "esp_reactivePower_qc ";
  message += qcscicom;
  message += "\n";

  message += "# apparent power\n";
  message += "esp_apparentPower_sa ";
  message += sascicom;
  message += "\n";
  message += "esp_apparentPower_sb ";
  message += sbscicom;
  message += "\n";
  message += "esp_apparentPower_sc ";
  message += scscicom;
  message += "\n";

  message += "# power factor\n";
  message += "esp_powerfactor_pft ";
  message += pftscicom;
  message += "\n";
  message += "esp_powerfactor_pfa ";
  message += pfascicom;
  message += "\n";
  message += "esp_powerfactor_pfb ";
  message += pfbscicom;
  message += "\n";
  message += "esp_powerfactor_pfc ";
  message += pfcscicom;
  message += "\n";
  return message;
}
String GenerateMetricsoldscicom() {
  String message = "";

  message += "# Smart meter voltage \n";
  message += "esp_voltage_va ";
  message += uaoldscicom;
  message += "\n";
  message += "esp_voltage_vb ";
  message += uboldscicom;
  message += "\n";
  message += "esp_voltage_vc ";
  message += ucoldscicom;
  message += "\n";

  message += "# Smart meter current \n";
  message += "esp_current_ia ";
  message += iaoldscicom;
  message += "\n";
  message += "esp_current_ib ";
  message += iboldscicom;
  message += "\n";
  message += "esp_current_ic ";
  message += icoldscicom;
  message += "\n";

  message += "# Smart meter active power\n";
  message += "esp_activePower_pt ";
  message += ptoldscicom;
  message += "\n";
  message += "esp_activePower_pa ";
  message += paoldscicom;
  message += "\n";
  message += "esp_activePower_pb ";
  message += pboldscicom;
  message += "\n";
  message += "esp_activePower_pc ";
  message += pcoldscicom;
  message += "\n";

  message += "# Smart meter reactive power\n";
  message += "esp_activePower_qt ";
  message += qtoldscicom;
  message += "\n";
  message += "esp_reactivePower_qa ";
  message += qaoldscicom;
  message += "\n";
  message += "esp_reactivePower_qb ";
  message += qboldscicom;
  message += "\n";
  message += "esp_reactivePower_qc ";
  message += qcoldscicom;
  message += "\n";

  message += "# apparent power\n";
  message += "esp_apparentPower_sa ";
  message += saoldscicom;
  message += "\n";
  message += "esp_apparentPower_sb ";
  message += sboldscicom;
  message += "\n";
  message += "esp_apparentPower_sc ";
  message += scoldscicom;
  message += "\n";

  message += "# power factor\n";
  message += "esp_powerfactor_pft ";
  message += pftoldscicom;
  message += "\n";
  message += "esp_powerfactor_pfa ";
  message += pfaoldscicom;
  message += "\n";
  message += "esp_powerfactor_pfb ";
  message += pfboldscicom;
  message += "\n";
  message += "esp_powerfactor_pfc ";
  message += pfcoldscicom;
  message += "\n";
  return message;
}
String GenerateMetricscafet() {
  String message = "";

  message += "# Smart meter voltage \n";
  message += "esp_voltage_va ";
  message += uacafet;
  message += "\n";
  message += "esp_voltage_vb ";
  message += ubcafet;
  message += "\n";
  message += "esp_voltage_vc ";
  message += uccafet;
  message += "\n";

  message += "# Smart meter current \n";
  message += "esp_current_ia ";
  message += iacafet;
  message += "\n";
  message += "esp_current_ib ";
  message += ibcafet;
  message += "\n";
  message += "esp_current_ic ";
  message += iccafet;
  message += "\n";

  message += "# Smart meter active power\n";
  message += "esp_activePower_pt ";
  message += ptcafet;
  message += "\n";
  message += "esp_activePower_pa ";
  message += pacafet;
  message += "\n";
  message += "esp_activePower_pb ";
  message += pbcafet;
  message += "\n";
  message += "esp_activePower_pc ";
  message += pccafet;
  message += "\n";

  message += "# Smart meter reactive power\n";
  message += "esp_activePower_qt ";
  message += qtcafet;
  message += "\n";
  message += "esp_reactivePower_qa ";
  message += qacafet;
  message += "\n";
  message += "esp_reactivePower_qb ";
  message += qbcafet;
  message += "\n";
  message += "esp_reactivePower_qc ";
  message += qccafet;
  message += "\n";

  message += "# apparent power\n";
  message += "esp_apparentPower_sa ";
  message += sacafet;
  message += "\n";
  message += "esp_apparentPower_sb ";
  message += sbcafet;
  message += "\n";
  message += "esp_apparentPower_sc ";
  message += sccafet;
  message += "\n";

  message += "# power factor\n";
  message += "esp_powerfactor_pft ";
  message += pftcafet;
  message += "\n";
  message += "esp_powerfactor_pfa ";
  message += pfacafet;
  message += "\n";
  message += "esp_powerfactor_pfb ";
  message += pfbcafet;
  message += "\n";
  message += "esp_powerfactor_pfc ";
  message += pfccafet;
  message += "\n";
  return message;
}
String GenerateMetricsecom() {
  String message = "";

  message += "# Smart meter voltage \n";
  message += "esp_voltage_va ";
  message += uaecom;
  message += "\n";
  message += "esp_voltage_vb ";
  message += ubecom;
  message += "\n";
  message += "esp_voltage_vc ";
  message += ucecom;
  message += "\n";

  message += "# Smart meter current \n";
  message += "esp_current_ia ";
  message += iaecom;
  message += "\n";
  message += "esp_current_ib ";
  message += ibecom;
  message += "\n";
  message += "esp_current_ic ";
  message += icecom;
  message += "\n";

  message += "# Smart meter active power\n";
  message += "esp_activePower_pt ";
  message += ptecom;
  message += "\n";
  message += "esp_activePower_pa ";
  message += paecom;
  message += "\n";
  message += "esp_activePower_pb ";
  message += pbecom;
  message += "\n";
  message += "esp_activePower_pc ";
  message += pcecom;
  message += "\n";

  message += "# Smart meter reactive power\n";
  message += "esp_activePower_qt ";
  message += qtecom;
  message += "\n";
  message += "esp_reactivePower_qa ";
  message += qaecom;
  message += "\n";
  message += "esp_reactivePower_qb ";
  message += qbecom;
  message += "\n";
  message += "esp_reactivePower_qc ";
  message += qcecom;
  message += "\n";

  message += "# apparent power\n";
  message += "esp_apparentPower_sa ";
  message += saecom;
  message += "\n";
  message += "esp_apparentPower_sb ";
  message += sbecom;
  message += "\n";
  message += "esp_apparentPower_sc ";
  message += scecom;
  message += "\n";

  message += "# power factor\n";
  message += "esp_powerfactor_pft ";
  message += pftecom;
  message += "\n";
  message += "esp_powerfactor_pfa ";
  message += pfaecom;
  message += "\n";
  message += "esp_powerfactor_pfb ";
  message += pfbecom;
  message += "\n";
  message += "esp_powerfactor_pfc ";
  message += pfcecom;
  message += "\n";
  return message;
}
String GenerateMetricsit() {
  String message = "";

  message += "# Smart meter voltage \n";
  message += "esp_voltage_va ";
  message += uait;
  message += "\n";
  message += "esp_voltage_vb ";
  message += ubit;
  message += "\n";
  message += "esp_voltage_vc ";
  message += ucit;
  message += "\n";

  message += "# Smart meter current \n";
  message += "esp_current_ia ";
  message += iait;
  message += "\n";
  message += "esp_current_ib ";
  message += ibit;
  message += "\n";
  message += "esp_current_ic ";
  message += icit;
  message += "\n";

  message += "# Smart meter active power\n";
  message += "esp_activePower_pt ";
  message += ptit;
  message += "\n";
  message += "esp_activePower_pa ";
  message += pait;
  message += "\n";
  message += "esp_activePower_pb ";
  message += pbit;
  message += "\n";
  message += "esp_activePower_pc ";
  message += pcit;
  message += "\n";

  message += "# Smart meter reactive power\n";
  message += "esp_activePower_qt ";
  message += qtit;
  message += "\n";
  message += "esp_reactivePower_qa ";
  message += qait;
  message += "\n";
  message += "esp_reactivePower_qb ";
  message += qbit;
  message += "\n";
  message += "esp_reactivePower_qc ";
  message += qcit;
  message += "\n";

  message += "# apparent power\n";
  message += "esp_apparentPower_sa ";
  message += sait;
  message += "\n";
  message += "esp_apparentPower_sb ";
  message += sbit;
  message += "\n";
  message += "esp_apparentPower_sc ";
  message += scit;
  message += "\n";

  message += "# power factor\n";
  message += "esp_powerfactor_pft ";
  message += pftit;
  message += "\n";
  message += "esp_powerfactor_pfa ";
  message += pfait;
  message += "\n";
  message += "esp_powerfactor_pfb ";
  message += pfbit;
  message += "\n";
  message += "esp_powerfactor_pfc ";
  message += pfcit;
  message += "\n";
  return message;
}
String GenerateMetricsgym() {
  String message = "";

  message += "# Smart meter voltage \n";
  message += "esp_voltage_va ";
  message += uagym;
  message += "\n";
  message += "esp_voltage_vb ";
  message += ubgym;
  message += "\n";
  message += "esp_voltage_vc ";
  message += ucgym;
  message += "\n";

  message += "# Smart meter current \n";
  message += "esp_current_ia ";
  message += iagym;
  message += "\n";
  message += "esp_current_ib ";
  message += ibgym;
  message += "\n";
  message += "esp_current_ic ";
  message += icgym;
  message += "\n";

  message += "# Smart meter active power\n";
  message += "esp_activePower_pt ";
  message += ptgym;
  message += "\n";
  message += "esp_activePower_pa ";
  message += pagym;
  message += "\n";
  message += "esp_activePower_pb ";
  message += pbgym;
  message += "\n";
  message += "esp_activePower_pc ";
  message += pcgym;
  message += "\n";

  message += "# Smart meter reactive power\n";
  message += "esp_activePower_qt ";
  message += qtgym;
  message += "\n";
  message += "esp_reactivePower_qa ";
  message += qagym;
  message += "\n";
  message += "esp_reactivePower_qb ";
  message += qbgym;
  message += "\n";
  message += "esp_reactivePower_qc ";
  message += qcgym;
  message += "\n";

  message += "# apparent power\n";
  message += "esp_apparentPower_sa ";
  message += sagym;
  message += "\n";
  message += "esp_apparentPower_sb ";
  message += sbgym;
  message += "\n";
  message += "esp_apparentPower_sc ";
  message += scgym;
  message += "\n";

  message += "# power factor\n";
  message += "esp_powerfactor_pft ";
  message += pftgym;
  message += "\n";
  message += "esp_powerfactor_pfa ";
  message += pfagym;
  message += "\n";
  message += "esp_powerfactor_pfb ";
  message += pfbgym;
  message += "\n";
  message += "esp_powerfactor_pfc ";
  message += pfcgym;
  message += "\n";
  return message;
}
String GenerateMetricsict() {
  String message = "";

  message += "# Smart meter voltage \n";
  message += "esp_voltage_va ";
  message += uaict;
  message += "\n";
  message += "esp_voltage_vb ";
  message += ubict;
  message += "\n";
  message += "esp_voltage_vc ";
  message += ucict;
  message += "\n";

  message += "# Smart meter current \n";
  message += "esp_current_ia ";
  message += iaict;
  message += "\n";
  message += "esp_current_ib ";
  message += ibict;
  message += "\n";
  message += "esp_current_ic ";
  message += icict;
  message += "\n";

  message += "# Smart meter active power\n";
  message += "esp_activePower_pt ";
  message += ptict;
  message += "\n";
  message += "esp_activePower_pa ";
  message += paict;
  message += "\n";
  message += "esp_activePower_pb ";
  message += pbict;
  message += "\n";
  message += "esp_activePower_pc ";
  message += pcict;
  message += "\n";

  message += "# Smart meter reactive power\n";
  message += "esp_activePower_qt ";
  message += qtict;
  message += "\n";
  message += "esp_reactivePower_qa ";
  message += qaict;
  message += "\n";
  message += "esp_reactivePower_qb ";
  message += qbict;
  message += "\n";
  message += "esp_reactivePower_qc ";
  message += qcict;
  message += "\n";

  message += "# apparent power\n";
  message += "esp_apparentPower_sa ";
  message += saict;
  message += "\n";
  message += "esp_apparentPower_sb ";
  message += sbict;
  message += "\n";
  message += "esp_apparentPower_sc ";
  message += scict;
  message += "\n";

  message += "# power factor\n";
  message += "esp_powerfactor_pft ";
  message += pftict;
  message += "\n";
  message += "esp_powerfactor_pfa ";
  message += pfaict;
  message += "\n";
  message += "esp_powerfactor_pfb ";
  message += pfbict;
  message += "\n";
  message += "esp_powerfactor_pfc ";
  message += pfcict;
  message += "\n";
  return message;
}
String GenerateMetricsadmin() {
  String message = "";

  message += "# Smart meter voltage \n";
  message += "esp_voltage_va ";
  message += uaadmin;
  message += "\n";
  message += "esp_voltage_vb ";
  message += ubadmin;
  message += "\n";
  message += "esp_voltage_vc ";
  message += ucadmin;
  message += "\n";

  message += "# Smart meter current \n";
  message += "esp_current_ia ";
  message += iaadmin;
  message += "\n";
  message += "esp_current_ib ";
  message += ibadmin;
  message += "\n";
  message += "esp_current_ic ";
  message += icadmin;
  message += "\n";

  message += "# Smart meter active power\n";
  message += "esp_activePower_pt ";
  message += ptadmin;
  message += "\n";
  message += "esp_activePower_pa ";
  message += paadmin;
  message += "\n";
  message += "esp_activePower_pb ";
  message += pbadmin;
  message += "\n";
  message += "esp_activePower_pc ";
  message += pcadmin;
  message += "\n";

  message += "# Smart meter reactive power\n";
  message += "esp_activePower_qt ";
  message += qtadmin;
  message += "\n";
  message += "esp_reactivePower_qa ";
  message += qaadmin;
  message += "\n";
  message += "esp_reactivePower_qb ";
  message += qbadmin;
  message += "\n";
  message += "esp_reactivePower_qc ";
  message += qcadmin;
  message += "\n";

  message += "# apparent power\n";
  message += "esp_apparentPower_sa ";
  message += saadmin;
  message += "\n";
  message += "esp_apparentPower_sb ";
  message += sbadmin;
  message += "\n";
  message += "esp_apparentPower_sc ";
  message += scadmin;
  message += "\n";

  message += "# power factor\n";
  message += "esp_powerfactor_pft ";
  message += pftadmin;
  message += "\n";
  message += "esp_powerfactor_pfa ";
  message += pfaadmin;
  message += "\n";
  message += "esp_powerfactor_pfb ";
  message += pfbadmin;
  message += "\n";
  message += "esp_powerfactor_pfc ";
  message += pfcadmin;
  message += "\n";
  return message;
}
String GenerateMetricscot() {
  String message = "";

  message += "# Smart meter voltage \n";
  message += "esp_voltage_va ";
  message += uacot;
  message += "\n";
  message += "esp_voltage_vb ";
  message += ubcot;
  message += "\n";
  message += "esp_voltage_vc ";
  message += uccot;
  message += "\n";

  message += "# Smart meter current \n";
  message += "esp_current_ia ";
  message += iacot;
  message += "\n";
  message += "esp_current_ib ";
  message += ibcot;
  message += "\n";
  message += "esp_current_ic ";
  message += iccot;
  message += "\n";

  message += "# Smart meter active power\n";
  message += "esp_activePower_pt ";
  message += ptcot;
  message += "\n";
  message += "esp_activePower_pa ";
  message += pacot;
  message += "\n";
  message += "esp_activePower_pb ";
  message += pbcot;
  message += "\n";
  message += "esp_activePower_pc ";
  message += pccot;
  message += "\n";

  message += "# Smart meter reactive power\n";
  message += "esp_activePower_qt ";
  message += qtcot;
  message += "\n";
  message += "esp_reactivePower_qa ";
  message += qacot;
  message += "\n";
  message += "esp_reactivePower_qb ";
  message += qbcot;
  message += "\n";
  message += "esp_reactivePower_qc ";
  message += qccot;
  message += "\n";

  message += "# apparent power\n";
  message += "esp_apparentPower_sa ";
  message += sacot;
  message += "\n";
  message += "esp_apparentPower_sb ";
  message += sbcot;
  message += "\n";
  message += "esp_apparentPower_sc ";
  message += sccot;
  message += "\n";

  message += "# power factor\n";
  message += "esp_powerfactor_pft ";
  message += pftcot;
  message += "\n";
  message += "esp_powerfactor_pfa ";
  message += pfacot;
  message += "\n";
  message += "esp_powerfactor_pfb ";
  message += pfbcot;
  message += "\n";
  message += "esp_powerfactor_pfc ";
  message += pfccot;
  message += "\n";
  return message;
}
String GenerateMetricsfablab() {
  String message = "";

  message += "# Smart meter voltage \n";
  message += "esp_voltage_va ";
  message += uafablab;
  message += "\n";
  message += "esp_voltage_vb ";
  message += ubfablab;
  message += "\n";
  message += "esp_voltage_vc ";
  message += ucfablab;
  message += "\n";

  message += "# Smart meter current \n";
  message += "esp_current_ia ";
  message += iafablab;
  message += "\n";
  message += "esp_current_ib ";
  message += ibfablab;
  message += "\n";
  message += "esp_current_ic ";
  message += icfablab;
  message += "\n";

  message += "# Smart meter active power\n";
  message += "esp_activePower_pt ";
  message += ptfablab;
  message += "\n";
  message += "esp_activePower_pa ";
  message += pafablab;
  message += "\n";
  message += "esp_activePower_pb ";
  message += pbfablab;
  message += "\n";
  message += "esp_activePower_pc ";
  message += pcfablab;
  message += "\n";

  message += "# Smart meter reactive power\n";
  message += "esp_activePower_qt ";
  message += qtfablab;
  message += "\n";
  message += "esp_reactivePower_qa ";
  message += qafablab;
  message += "\n";
  message += "esp_reactivePower_qb ";
  message += qbfablab;
  message += "\n";
  message += "esp_reactivePower_qc ";
  message += qcfablab;
  message += "\n";

  message += "# apparent power\n";
  message += "esp_apparentPower_sa ";
  message += safablab;
  message += "\n";
  message += "esp_apparentPower_sb ";
  message += sbfablab;
  message += "\n";
  message += "esp_apparentPower_sc ";
  message += scfablab;
  message += "\n";

  message += "# power factor\n";
  message += "esp_powerfactor_pft ";
  message += pftfablab;
  message += "\n";
  message += "esp_powerfactor_pfa ";
  message += pfafablab;
  message += "\n";
  message += "esp_powerfactor_pfb ";
  message += pfbfablab;
  message += "\n";
  message += "esp_powerfactor_pfc ";
  message += pfcfablab;
  message += "\n";
  return message;
}
void setup() {
  Serial.begin(115200);
  delay(500);
  while (!Serial)
    ;
  // // Configures static IP address
  // if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  //   Serial.println("STA Failed to configure");
  // }
  // Serial.print("Connecting to ");
  // Serial.println(ssid);
  // //Setup WIFI
  // WiFi.begin(ssid, password);
  // Serial.println("");

  // //Wait for WIFI connection
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.print("Connected to ");
  // Serial.println(ssid);
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());
  // Serial.print("MAC address: ");
  // Serial.println(WiFi.macAddress());
  // Serial.println("Gateway IP address: ");
  // Serial.println(WiFi.gatewayIP());

  // server.on("/lrc", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", GenerateMetricslrc());
  // });
  // server.on("/bldg24", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", GenerateMetricsbldg24());
  // });
  // server.on("/fic", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", GenerateMetricsfic());
  // });
  // server.on("/scicom", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", GenerateMetricsscicom());
  // });
  // server.on("/oldscicom", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", GenerateMetricsoldscicom());
  // });
  // server.on("/cafet", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", GenerateMetricscafet());
  // });
  // server.on("/ecom", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", GenerateMetricsecom());
  // });
  // server.on("/it", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", GenerateMetricsit());
  // });
  // server.on("/gym", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", GenerateMetricsgym());
  // });
  // server.on("/ict", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", GenerateMetricsict());
  // });
  // server.on("/admin", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", GenerateMetricsadmin());
  // });
  // server.on("/cot", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", GenerateMetricscot());
  // });
  // server.on("/fablab", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", GenerateMetricsfablab());
  // });
  // //  server.onNotFound( HandleNotFound);
  // //  server.enableCORS(true);
  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  // server.begin();

  // Serial.println("HTTP server started at ip " + WiFi.localIP().toString());


  Serial.println();
  // Startup all pins and UART
  e22ttl.begin();

  Serial.print("Hi, I'm: ");
  Serial.println(MY_ADDR);
}

void loop() {
  lrc();
  delay(10000);
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
      Serial.print(", ");
      Serial.print(rxmessage.bldg);
      Serial.print(", RSSI: ");
      Serial.println(rsc.rssi, DEC);
      Serial.println(" ");
      if (rxmessage.bldg == "lrc") {
        ualrc = rxmessage.ua * sqrt(3);
        ublrc = rxmessage.ub * sqrt(3);
        uclrc = rxmessage.uc * sqrt(3);

        ialrc = rxmessage.ia / sqrt(3);
        iblrc = rxmessage.ib / sqrt(3);
        iclrc = rxmessage.ic / sqrt(3);

        ptlrc = rxmessage.pt;
        palrc = rxmessage.pa;
        pblrc = rxmessage.pb;
        pclrc = rxmessage.pc;

        qtlrc = rxmessage.qt;
        qalrc = rxmessage.qa;
        qblrc = rxmessage.qb;
        qclrc = rxmessage.qc;

        pftlrc = rxmessage.pft;
        pfalrc = rxmessage.pfa;
        pfblrc = rxmessage.pfb;
        pfclrc = rxmessage.pfc;

        salrc = ualrc * ialrc;
        sblrc = ublrc * iblrc;
        sclrc = uclrc * iclrc;
        ping[0] = 0;
      } else if (rxmessage.bldg == "bldg24") {
        uabldg24 = rxmessage.ua;
        ubbldg24 = rxmessage.ub;
        ucbldg24 = rxmessage.uc;

        iabldg24 = rxmessage.ia;
        ibbldg24 = rxmessage.ib;
        icbldg24 = rxmessage.ic;

        ptbldg24 = rxmessage.pt;
        pabldg24 = rxmessage.pa;
        pbbldg24 = rxmessage.pb;
        pcbldg24 = rxmessage.pc;

        qtbldg24 = rxmessage.qt;
        qabldg24 = rxmessage.qa;
        qbbldg24 = rxmessage.qb;
        qcbldg24 = rxmessage.qc;

        pftbldg24 = rxmessage.pft;
        pfabldg24 = rxmessage.pfa;
        pfbbldg24 = rxmessage.pfb;
        pfcbldg24 = rxmessage.pfc;

        sabldg24 = uabldg24 * iabldg24;
        sbbldg24 = ubbldg24 * ibbldg24;
        scbldg24 = ucbldg24 * icbldg24;
        ping[1] = 0;
      } else if (rxmessage.bldg == "fic") {
        uafic = rxmessage.ua;
        ubfic = rxmessage.ub;
        ucfic = rxmessage.uc;

        iafic = rxmessage.ia;
        ibfic = rxmessage.ib;
        icfic = rxmessage.ic;

        ptfic = rxmessage.pt;
        pafic = rxmessage.pa;
        pbfic = rxmessage.pb;
        pcfic = rxmessage.pc;

        qtfic = rxmessage.qt;
        qafic = rxmessage.qa;
        qbfic = rxmessage.qb;
        qcfic = rxmessage.qc;

        pftfic = rxmessage.pft;
        pfafic = rxmessage.pfa;
        pfbfic = rxmessage.pfb;
        pfcfic = rxmessage.pfc;

        safic = uafic * iafic;
        sbfic = ubfic * ibfic;
        scfic = ucfic * icfic;
        ping[2] = 0;
      } else if (rxmessage.bldg == "scicom") {
        uascicom = rxmessage.ua;
        ubscicom = rxmessage.ub;
        ucscicom = rxmessage.uc;

        iascicom = rxmessage.ia;
        ibscicom = rxmessage.ib;
        icscicom = rxmessage.ic;

        ptscicom = rxmessage.pt;
        pascicom = rxmessage.pa;
        pbscicom = rxmessage.pb;
        pcscicom = rxmessage.pc;

        qtscicom = rxmessage.qt;
        qascicom = rxmessage.qa;
        qbscicom = rxmessage.qb;
        qcscicom = rxmessage.qc;

        pftscicom = rxmessage.pft;
        pfascicom = rxmessage.pfa;
        pfbscicom = rxmessage.pfb;
        pfcscicom = rxmessage.pfc;

        sascicom = uascicom * iascicom;
        sbscicom = ubscicom * ibscicom;
        scscicom = ucscicom * icscicom;
        ping[3] = 0;
      } else if (rxmessage.bldg == "oldscicom") {
        uaoldscicom = rxmessage.ua;
        uboldscicom = rxmessage.ub;
        ucoldscicom = rxmessage.uc;

        iaoldscicom = rxmessage.ia;
        iboldscicom = rxmessage.ib;
        icoldscicom = rxmessage.ic;

        ptoldscicom = rxmessage.pt;
        paoldscicom = rxmessage.pa;
        pboldscicom = rxmessage.pb;
        pcoldscicom = rxmessage.pc;

        qtoldscicom = rxmessage.qt;
        qaoldscicom = rxmessage.qa;
        qboldscicom = rxmessage.qb;
        qcoldscicom = rxmessage.qc;

        pftoldscicom = rxmessage.pft;
        pfaoldscicom = rxmessage.pfa;
        pfboldscicom = rxmessage.pfb;
        pfcoldscicom = rxmessage.pfc;

        saoldscicom = uaoldscicom * iaoldscicom;
        sboldscicom = uboldscicom * iboldscicom;
        scoldscicom = ucoldscicom * icoldscicom;
        ping[4] = 0;
      } else if (rxmessage.bldg == "cafet") {
        uacafet = rxmessage.ua;
        ubcafet = rxmessage.ub;
        uccafet = rxmessage.uc;

        iacafet = rxmessage.ia;
        ibcafet = rxmessage.ib;
        iccafet = rxmessage.ic;

        ptcafet = rxmessage.pt;
        pacafet = rxmessage.pa;
        pbcafet = rxmessage.pb;
        pccafet = rxmessage.pc;

        qtcafet = rxmessage.qt;
        qacafet = rxmessage.qa;
        qbcafet = rxmessage.qb;
        qccafet = rxmessage.qc;

        pftcafet = rxmessage.pft;
        pfacafet = rxmessage.pfa;
        pfbcafet = rxmessage.pfb;
        pfccafet = rxmessage.pfc;

        sacafet = uacafet * iacafet;
        sbcafet = ubcafet * ibcafet;
        sccafet = uccafet * iccafet;
        ping[5] = 0;
      } else if (rxmessage.bldg == "ecom") {
        uaecom = rxmessage.ua;
        ubecom = rxmessage.ub;
        ucecom = rxmessage.uc;

        iaecom = rxmessage.ia;
        ibecom = rxmessage.ib;
        icecom = rxmessage.ic;

        ptecom = rxmessage.pt;
        paecom = rxmessage.pa;
        pbecom = rxmessage.pb;
        pcecom = rxmessage.pc;

        qtecom = rxmessage.qt;
        qaecom = rxmessage.qa;
        qbecom = rxmessage.qb;
        qcecom = rxmessage.qc;

        pftecom = rxmessage.pft;
        pfaecom = rxmessage.pfa;
        pfbecom = rxmessage.pfb;
        pfcecom = rxmessage.pfc;

        saecom = uaecom * iaecom;
        sbecom = ubecom * ibecom;
        scecom = ucecom * icecom;
        ping[6] = 0;
      } else if (rxmessage.bldg == "it") {
        uait = rxmessage.ua;
        ubit = rxmessage.ub;
        ucit = rxmessage.uc;

        iait = rxmessage.ia;
        ibit = rxmessage.ib;
        icit = rxmessage.ic;

        ptit = rxmessage.pt;
        pait = rxmessage.pa;
        pbit = rxmessage.pb;
        pcit = rxmessage.pc;

        qtit = rxmessage.qt;
        qait = rxmessage.qa;
        qbit = rxmessage.qb;
        qcit = rxmessage.qc;

        pftit = rxmessage.pft;
        pfait = rxmessage.pfa;
        pfbit = rxmessage.pfb;
        pfcit = rxmessage.pfc;

        sait = uait * iait;
        sbit = ubit * ibit;
        scit = ucit * icit;
        ping[7] = 0;
      } else if (rxmessage.bldg == "gym") {
        uagym = rxmessage.ua;
        ubgym = rxmessage.ub;
        ucgym = rxmessage.uc;

        iagym = rxmessage.ia;
        ibgym = rxmessage.ib;
        icgym = rxmessage.ic;

        ptgym = rxmessage.pt;
        pagym = rxmessage.pa;
        pbgym = rxmessage.pb;
        pcgym = rxmessage.pc;

        qtgym = rxmessage.qt;
        qagym = rxmessage.qa;
        qbgym = rxmessage.qb;
        qcgym = rxmessage.qc;

        pftgym = rxmessage.pft;
        pfagym = rxmessage.pfa;
        pfbgym = rxmessage.pfb;
        pfcgym = rxmessage.pfc;

        sagym = uagym * iagym;
        sbgym = ubgym * ibgym;
        scgym = ucgym * icgym;
        ping[8] = 0;
      } else if (rxmessage.bldg == "ict") {
        uaict = rxmessage.ua;
        ubict = rxmessage.ub;
        ucict = rxmessage.uc;

        iaict = rxmessage.ia;
        ibict = rxmessage.ib;
        icict = rxmessage.ic;

        ptict = rxmessage.pt;
        paict = rxmessage.pa;
        pbict = rxmessage.pb;
        pcict = rxmessage.pc;

        qtict = rxmessage.qt;
        qaict = rxmessage.qa;
        qbict = rxmessage.qb;
        qcict = rxmessage.qc;

        pftict = rxmessage.pft;
        pfaict = rxmessage.pfa;
        pfbict = rxmessage.pfb;
        pfcict = rxmessage.pfc;

        saict = uaict * iaict;
        sbict = ubict * ibict;
        scict = ucict * icict;
        ping[9] = 0;
      } else if (rxmessage.bldg == "admin") {
        uaadmin = rxmessage.ua;
        ubadmin = rxmessage.ub;
        ucadmin = rxmessage.uc;

        iaadmin = rxmessage.ia;
        ibadmin = rxmessage.ib;
        icadmin = rxmessage.ic;

        ptadmin = rxmessage.pt;
        paadmin = rxmessage.pa;
        pbadmin = rxmessage.pb;
        pcadmin = rxmessage.pc;

        qtadmin = rxmessage.qt;
        qaadmin = rxmessage.qa;
        qbadmin = rxmessage.qb;
        qcadmin = rxmessage.qc;

        pftadmin = rxmessage.pft;
        pfaadmin = rxmessage.pfa;
        pfbadmin = rxmessage.pfb;
        pfcadmin = rxmessage.pfc;

        saadmin = uaadmin * iaadmin;
        sbadmin = ubadmin * ibadmin;
        scadmin = ucadmin * icadmin;
        ping[10] = 0;
      } else if (rxmessage.bldg == "cot") {
        uacot = rxmessage.ua;
        ubcot = rxmessage.ub;
        uccot = rxmessage.uc;

        iacot = rxmessage.ia;
        ibcot = rxmessage.ib;
        iccot = rxmessage.ic;

        ptcot = rxmessage.pt;
        pacot = rxmessage.pa;
        pbcot = rxmessage.pb;
        pccot = rxmessage.pc;

        qtcot = rxmessage.qt;
        qacot = rxmessage.qa;
        qbcot = rxmessage.qb;
        qccot = rxmessage.qc;

        pftcot = rxmessage.pft;
        pfacot = rxmessage.pfa;
        pfbcot = rxmessage.pfb;
        pfccot = rxmessage.pfc;

        sacot = uacot * iacot;
        sbcot = ubcot * ibcot;
        sccot = uccot * iccot;
        ping[11] = 0;
      } else if (rxmessage.bldg == "fablab") {
        uafablab = rxmessage.ua;
        ubfablab = rxmessage.ub;
        ucfablab = rxmessage.uc;

        iafablab = rxmessage.ia;
        ibfablab = rxmessage.ib;
        icfablab = rxmessage.ic;

        ptfablab = rxmessage.pt;
        pafablab = rxmessage.pa;
        pbfablab = rxmessage.pb;
        pcfablab = rxmessage.pc;

        qtfablab = rxmessage.qt;
        qafablab = rxmessage.qa;
        qbfablab = rxmessage.qb;
        qcfablab = rxmessage.qc;

        pftfablab = rxmessage.pft;
        pfafablab = rxmessage.pfa;
        pfbfablab = rxmessage.pfb;
        pfcfablab = rxmessage.pfc;

        safablab = uafablab * iafablab;
        sbfablab = ubfablab * ibfablab;
        scfablab = ucfablab * icfablab;
        ping[12] = 0;
      }
    }
  }
}

void lrc() {

  if (rx_addrlrc > 2) rx_addrlrc = 1;

  if (rx_addrlrc == 1) {
    Serial.print("Requesting to: ");
    Serial.println(rx_addrlrc);

    struct Message txmessage = { loopCount, tx_addr, rx_addrlrc, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addrlrc], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());

  } else {
    Serial.println("Requesting to 1 via 1");
    struct Message txmessage = { loopCount, tx_addr, 2, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());
    if (ping[0] == 2) {
      ualrc = 0;
      ublrc = 0;
      uclrc = 0;

      ialrc = 0;
      iblrc = 0;
      iclrc = 0;

      ptlrc = 0;
      palrc = 0;
      pblrc = 0;
      pclrc = 0;

      qtlrc = 0;
      qalrc = 0;
      qblrc = 0;
      qclrc = 0;

      pftlrc = 0;
      pfalrc = 0;
      pfblrc = 0;
      pfclrc = 0;

      salrc = 0;
      sblrc = 0;
      sclrc = 0;
      ping[0] = 0;
    } else {
      ping[0] += 1;
    }
  }
  rx_addrlrc++;
  loopCount++;
}

void bldg24() {

  if (rx_addrbldg24 > 4) rx_addrbldg24 = 3;

  if (rx_addrbldg24 == 3) {
    Serial.print("Requesting to bldg24: ");
    Serial.println(rx_addrbldg24);

    struct Message txmessage = { loopCount, tx_addr, rx_addrbldg24, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addrbldg24], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());


  } else {
    Serial.println("Requesting to 3 via 1");
    struct Message txmessage = { loopCount, tx_addr, 3, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());
    if (ping[1] == 2) {
      uabldg24 = 0;
      ubbldg24 = 0;
      ucbldg24 = 0;

      iabldg24 = 0;
      ibbldg24 = 0;
      icbldg24 = 0;

      ptbldg24 = 0;
      pabldg24 = 0;
      pbbldg24 = 0;
      pcbldg24 = 0;

      qtbldg24 = 0;
      qabldg24 = 0;
      qbbldg24 = 0;
      qcbldg24 = 0;

      pftbldg24 = 0;
      pfabldg24 = 0;
      pfbbldg24 = 0;
      pfcbldg24 = 0;

      sabldg24 = 0;
      sbbldg24 = 0;
      scbldg24 = 0;
      ping[1] = 0;
    } else {
      ping[1] += 1;
    }
  }
  rx_addrbldg24++;
  loopCount++;
}

void fic() {


  if (rx_addrfic > 5) rx_addrfic = 4;

  if (rx_addrfic == 4) {
    Serial.print("Requesting to fic: ");
    Serial.println(rx_addrfic);

    struct Message txmessage = { loopCount, tx_addr, rx_addrfic, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addrfic], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());

  } else {
    Serial.println("Requesting to 4 via 1");
    struct Message txmessage = { loopCount, tx_addr, 4, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());
    if (ping[2] == 2) {
      uafic = 0;
      ubfic = 0;
      ucfic = 0;

      iafic = 0;
      ibfic = 0;
      icfic = 0;

      ptfic = 0;
      pafic = 0;
      pbfic = 0;
      pcfic = 0;

      qtfic = 0;
      qafic = 0;
      qbfic = 0;
      qcfic = 0;

      pftfic = 0;
      pfafic = 0;
      pfbfic = 0;
      pfcfic = 0;

      safic = 0;
      sbfic = 0;
      scfic = 0;
      ping[2] = 0;
    } else {
      ping[2] += 1;
    }
  }
  rx_addrfic++;
  loopCount++;
}

void scicom() {
  if (rx_addrscicom > 6) rx_addrscicom = 5;
  if (rx_addrscicom == 5) {
    Serial.print("Requesting to scicom: ");
    Serial.println(rx_addrscicom);
    struct Message txmessage = { loopCount, tx_addr, rx_addrscicom, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addrscicom], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());

  } else {
    Serial.println("Requesting to 5 via 1");
    struct Message txmessage = { loopCount, tx_addr, 5, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());
    if (ping[3] == 2) {
      uascicom = 0;
      ubscicom = 0;
      ucscicom = 0;

      iascicom = 0;
      ibscicom = 0;
      icscicom = 0;

      ptscicom = 0;
      pascicom = 0;
      pbscicom = 0;
      pcscicom = 0;

      qtscicom = 0;
      qascicom = 0;
      qbscicom = 0;
      qcscicom = 0;

      pftscicom = 0;
      pfascicom = 0;
      pfbscicom = 0;
      pfcscicom = 0;

      sascicom = 0;
      sbscicom = 0;
      scscicom = 0;
      ping[3] = 0;
    } else {
      ping[3] += 1;
    }
  }
  rx_addrscicom++;
  loopCount++;
}

void oldscicom() {
  if (rx_addroldscicom > 7) rx_addroldscicom = 6;
  if (rx_addroldscicom == 6) {
    Serial.print("Requesting to oldscicom: ");
    Serial.println(rx_addroldscicom);
    struct Message txmessage = { loopCount, tx_addr, rx_addroldscicom, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addroldscicom], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());

  } else {
    Serial.println("Requesting to 6 via 1");
    struct Message txmessage = { loopCount, tx_addr, 6, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());
    if (ping[4] == 2) {
      uaoldscicom = 0;
      uboldscicom = 0;
      ucoldscicom = 0;

      iaoldscicom = 0;
      iboldscicom = 0;
      icoldscicom = 0;

      ptoldscicom = 0;
      paoldscicom = 0;
      pboldscicom = 0;
      pcoldscicom = 0;

      qtoldscicom = 0;
      qaoldscicom = 0;
      qboldscicom = 0;
      qcoldscicom = 0;

      pftoldscicom = 0;
      pfaoldscicom = 0;
      pfboldscicom = 0;
      pfcoldscicom = 0;

      saoldscicom = 0;
      sboldscicom = 0;
      scoldscicom = 0;
      ping[4] = 0;
    } else {
      ping[4] += 1;
    }
  }
  rx_addroldscicom++;
  loopCount++;
}

void cafet() {
  if (rx_addrcafet > 8) rx_addrcafet = 7;
  if (rx_addrcafet == 7) {
    Serial.print("Requesting to cafet: ");
    Serial.println(rx_addrcafet);
    struct Message txmessage = { loopCount, tx_addr, rx_addrcafet, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addrcafet], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());

  } else {
    Serial.println("Requesting to 7 via 1");
    struct Message txmessage = { loopCount, tx_addr, 7, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());
    if (ping[5] == 2) {
      uacafet = 0;
      ubcafet = 0;
      uccafet = 0;

      iacafet = 0;
      ibcafet = 0;
      iccafet = 0;

      ptcafet = 0;
      pacafet = 0;
      pbcafet = 0;
      pccafet = 0;

      qtcafet = 0;
      qacafet = 0;
      qbcafet = 0;
      qccafet = 0;

      pftcafet = 0;
      pfacafet = 0;
      pfbcafet = 0;
      pfccafet = 0;

      sacafet = 0;
      sbcafet = 0;
      sccafet = 0;
      ping[5] = 0;
    } else {
      ping[5] += 1;
    }
  }
  rx_addrcafet++;
  loopCount++;
}

void ecom() {
  if (rx_addrecom > 9) rx_addrecom = 8;
  if (rx_addrecom == 8) {
    Serial.print("Requesting to ecom: ");
    Serial.println(rx_addrecom);
    struct Message txmessage = { loopCount, tx_addr, rx_addrecom, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addrecom], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());

  } else {
    Serial.println("Requesting to 8 via 1");
    struct Message txmessage = { loopCount, tx_addr, 8, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());
    if (ping[6] == 2) {
      uaecom = 0;
      ubecom = 0;
      ucecom = 0;

      iaecom = 0;
      ibecom = 0;
      icecom = 0;

      ptecom = 0;
      paecom = 0;
      pbecom = 0;
      pcecom = 0;

      qtecom = 0;
      qaecom = 0;
      qbecom = 0;
      qcecom = 0;

      pftecom = 0;
      pfaecom = 0;
      pfbecom = 0;
      pfcecom = 0;

      saecom = 0;
      sbecom = 0;
      scecom = 0;
      ping[6] = 0;
    } else {
      ping[6] += 1;
    }
  }
  rx_addrecom++;
  loopCount++;
}

void it() {
  if (rx_addrit > 10) rx_addrit = 9;
  if (rx_addrit == 9) {
    Serial.print("Requesting to it: ");
    Serial.println(rx_addrit);
    struct Message txmessage = { loopCount, tx_addr, rx_addrit, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addrit], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());

  } else {
    Serial.println("Requesting to 9 via 1");
    struct Message txmessage = { loopCount, tx_addr, 9, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());
    if (ping[7] == 2) {
      uait = 0;
      ubit = 0;
      ucit = 0;

      iait = 0;
      ibit = 0;
      icit = 0;

      ptit = 0;
      pait = 0;
      pbit = 0;
      pcit = 0;

      qtit = 0;
      qait = 0;
      qbit = 0;
      qcit = 0;

      pftit = 0;
      pfait = 0;
      pfbit = 0;
      pfcit = 0;

      sait = 0;
      sbit = 0;
      scit = 0;
      ping[7] = 0;
    } else {
      ping[7] += 1;
    }
  }
  rx_addrit++;
  loopCount++;
}

void gym() {
  if (rx_addrgym > 11) rx_addrgym = 10;
  if (rx_addrgym == 10) {
    Serial.print("Requesting to gym: ");
    Serial.println(rx_addrgym);
    struct Message txmessage = { loopCount, tx_addr, rx_addrgym, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addrgym], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());

  } else {
    Serial.println("Requesting to 10 via 1");
    struct Message txmessage = { loopCount, tx_addr, 10, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());
    if (ping[8] == 2) {
      uagym = 0;
      ubgym = 0;
      ucgym = 0;

      iagym = 0;
      ibgym = 0;
      icgym = 0;

      ptgym = 0;
      pagym = 0;
      pbgym = 0;
      pcgym = 0;

      qtgym = 0;
      qagym = 0;
      qbgym = 0;
      qcgym = 0;

      pftgym = 0;
      pfagym = 0;
      pfbgym = 0;
      pfcgym = 0;

      sagym = 0;
      sbgym = 0;
      scgym = 0;
      ping[8] = 0;
    } else {
      ping[8] += 1;
    }
  }
  rx_addrgym++;
  loopCount++;
}

void ict() {
  if (rx_addrict > 12) rx_addrict = 11;
  if (rx_addrict == 11) {
    Serial.print("Requesting to ict: ");
    Serial.println(rx_addrict);
    struct Message txmessage = { loopCount, tx_addr, rx_addrict, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addrict], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());

  } else {
    Serial.println("Requesting to 11 via 1");
    struct Message txmessage = { loopCount, tx_addr, 11, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());
    if (ping[9] == 2) {
      uaict = 0;
      ubict = 0;
      ucict = 0;

      iaict = 0;
      ibict = 0;
      icict = 0;

      ptict = 0;
      paict = 0;
      pbict = 0;
      pcict = 0;

      qtict = 0;
      qaict = 0;
      qbict = 0;
      qcict = 0;

      pftict = 0;
      pfaict = 0;
      pfbict = 0;
      pfcict = 0;

      saict = 0;
      sbict = 0;
      scict = 0;
      ping[9] = 0;
    } else {
      ping[9] += 1;
    }
  }
  rx_addrict++;
  loopCount++;
}

void admin() {
  if (rx_addradmin > 13) rx_addradmin = 12;
  if (rx_addradmin == 12) {
    Serial.print("Requesting to admin: ");
    Serial.println(rx_addradmin);
    struct Message txmessage = { loopCount, tx_addr, rx_addradmin, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addradmin], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());

  } else {
    Serial.println("Requesting to 12 via 1");
    struct Message txmessage = { loopCount, tx_addr, 12, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());
    if (ping[10] == 2) {
      uaadmin = 0;
      ubadmin = 0;
      ucadmin = 0;

      iaadmin = 0;
      ibadmin = 0;
      icadmin = 0;

      ptadmin = 0;
      paadmin = 0;
      pbadmin = 0;
      pcadmin = 0;

      qtadmin = 0;
      qaadmin = 0;
      qbadmin = 0;
      qcadmin = 0;

      pftadmin = 0;
      pfaadmin = 0;
      pfbadmin = 0;
      pfcadmin = 0;

      saadmin = 0;
      sbadmin = 0;
      scadmin = 0;
      ping[10] = 0;
    } else {
      ping[10] += 1;
    }
  }
  rx_addradmin++;
  loopCount++;
}

void cot() {
  if (rx_addrcot > 14) rx_addrcot = 13;
  if (rx_addrcot == 13) {
    Serial.print("Requesting to cot: ");
    Serial.println(rx_addrcot);
    struct Message txmessage = { loopCount, tx_addr, rx_addrcot, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addrcot], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());

  } else {
    Serial.println("Requesting to 13 via 1");
    struct Message txmessage = { loopCount, tx_addr, 13, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());
    if (ping[11] == 2) {
      uacot = 0;
      ubcot = 0;
      uccot = 0;

      iacot = 0;
      ibcot = 0;
      iccot = 0;

      ptcot = 0;
      pacot = 0;
      pbcot = 0;
      pccot = 0;

      qtcot = 0;
      qacot = 0;
      qbcot = 0;
      qccot = 0;

      pftcot = 0;
      pfacot = 0;
      pfbcot = 0;
      pfccot = 0;

      sacot = 0;
      sbcot = 0;
      sccot = 0;
      ping[11] = 0;
    } else {
      ping[11] += 1;
    }
  }
  rx_addrcot++;
  loopCount++;
}

void fablab() {
  if (rx_addrfablab > 15) rx_addrfablab = 14;
  if (rx_addrfablab == 14) {
    Serial.print("Requesting to fablab: ");
    Serial.println(rx_addrfablab);
    struct Message txmessage = { loopCount, tx_addr, rx_addrfablab, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[rx_addrfablab], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());

  } else {
    Serial.println("Requesting to 14 via 1");
    struct Message txmessage = { loopCount, tx_addr, 14, REQUEST };
    ResponseStatus rs = e22ttl.sendFixedMessage(H_ADDR, ADDR_LIST[1], CHAN, &txmessage, sizeof(Message));
    Serial.println(rs.getResponseDescription());
    if (ping[12] == 2) {
      uafablab = 0;
      ubfablab = 0;
      ucfablab = 0;

      iafablab = 0;
      ibfablab = 0;
      icfablab = 0;

      ptfablab = 0;
      pafablab = 0;
      pbfablab = 0;
      pcfablab = 0;

      qtfablab = 0;
      qafablab = 0;
      qbfablab = 0;
      qcfablab = 0;

      pftfablab = 0;
      pfafablab = 0;
      pfbfablab = 0;
      pfcfablab = 0;

      safablab = 0;
      sbfablab = 0;
      scfablab = 0;
      ping[12] = 0;
    } else {
      ping[12] += 1;
    }
  }
  rx_addrfablab++;
  loopCount++;
}
