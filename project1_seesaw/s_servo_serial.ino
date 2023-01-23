#include <Servo.h>
Servo servomotor;
int a = 0;

void setup() {
  servomotor.attach(9);
  Serial.begin(9600);                               // Serial 통신을 설정합니다.
  while(!Serial);                                           
  Serial.println("Servor Mortor");
}

void loop() {
  if(Serial.available()){                             //Serial 모니터창에 어떤 값이 들어오면 실행을 합니다.
    a = Serial.parseInt();                           //Serial 모니터창에서 받은 값을 a라는 변수로 넘겨줍니다.
    if(a>=0 && a<=180)
    {
      Serial.print("angle : ");
      Serial.println(a);
      servomotor.write(a);
      delay(15);
    }
  }
}

// 42도 정상값  0-140