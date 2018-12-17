
#include <Wire.h>
#include "BH1750.h"
#include "DHT.h"
#include <SoftwareSerial.h>

SoftwareSerial bt(5,6); // RX 5 - TX 6

String msg;
char arrData[30];
int lenMsg;

/* Cam bien anh sang */ 
BH1750 lightMeter;
/* ==================*/
/* Cam bien nhiet do */
const int DHTPIN = 2; // doc du lieu tu DHT11 tai pin D2
const int DHTTYPE = DHT11; // khai bao ten cam bien
DHT dht(DHTPIN, DHTTYPE);
/* ================== */

uint16_t lux, h_number, h_phantram;// gia tri lux va do am dat
float t; // gia tri nhiet do 

const int pump = 3; // Output Actuator D3

/* Algorithm */
const int a[]={0,1,2};
const double water[] = {0,5,20};
const double vth = 3;
const int L = -1;
const int OK = 0;
const int H = 1;
const int countAction = 3;
const int countState = 3;
const double alpha = (water[2]-water[1])/(vth*(a[2]-a[1]));
const int penalty = 100;
const double th1 = 15;
const double th2 = 60;
double v;
int boolean_v;
int humid2State(double humid)
{
  if (humid < th1)  return L;
  if (humid > th2)  return H;
  return OK;
}
double reward(int curState, int action, int nextState)
{
  int p = 0;
  if (nextState != OK) p = penalty;
  double r = - alpha * curState * v * a[action] - water[action] - p;
  return r;
}

int makeDecision(double curHumid, double preHumid)
{
  int curState = humid2State(curHumid);
  v = preHumid - curHumid;
  double tmpReward = 0, maxReward = -2000, nextHumid;
  int nextState;
  int action;
  for(int i = 0;i<countAction;i++)
  {
    nextHumid = curHumid - v + water[i];
    nextState = humid2State(nextHumid);
    tmpReward = reward(curState,i,nextState);
    if(tmpReward > maxReward)
    {
      maxReward = tmpReward;
      action = i;
    }
  }
  return a[action];
}

int curHumid, preHumid, decision;

void setup()
{
  Serial.begin(9600);
  bt.begin(9600);
  Wire.begin();

  pinMode(A0, INPUT); // do am dat
  pinMode(pump, OUTPUT); // dau ra may bom
  dht.begin(); // active module dht
  lightMeter.begin(); // active module light
  preHumid = analogRead(A0);
  preHumid = map(preHumid, 0, 1023, 100, 0);
  decision = 0;
  boolean_v = 0;
}

void loop()
{
  lux = lightMeter.readLightLevel(); //doc gia tri lux
  h_number = analogRead(A0); // do am tu 0-1023
  h_phantram = map(h_number,0,1023,100,0);
  t = dht.readTemperature();

  curHumid = h_phantram;
  decision = makeDecision(curHumid, preHumid);

  if (v < vth) boolean_v = 0;
    else boolean_v = 1;
  msg = "#"+String(lux)+" "+String(t)+" "+String(h_phantram)+" "+String(boolean_v)+" "+String(decision)+"~";
  lenMsg = msg.length();
  msg.toCharArray(arrData, lenMsg+1);
  bt.write(arrData);
  Serial.println(arrData);

  /* Action */
  if (decision == 2)
  {
    digitalWrite(pump,HIGH);
    delay(7000);
    digitalWrite(pump, LOW);
    delay(3000);
  }
    if (decision == 1)
  {
    digitalWrite(pump,HIGH);
    delay(4000);
    digitalWrite(pump, LOW);
    delay(6000);
  }
  if(decision == 0)
  {
    digitalWrite(pump,LOW);
    delay(10000);
  }
  preHumid = curHumid;
}
