
#include <Servo.h>  
#include <PID_v1.h>

const int servoPin = 9;                          //서보의 핀번호

float Kp = 10;                //P게인 값
float Ki = 0;                  //I게인 값 
float Kd = 0;                  //D게인 값
double Setpoint, Input, Output, ServoOutput;                                       

float duration, distance;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);   //PID객체 생성

Servo myServo;                                     //서보 객체 생성, 초기화


int echoPin = 6;                 //초음파 핀번호 설정
int trigPin = 7;

void setup() {
  Serial.begin(9600);                  //시리얼 통신 초기화
  myServo.attach(servoPin);            //서보모터 핀번호 설정

  Input = readPosition();             //막대 위의 공의 위치를 측정값 함수 호출
  
  //초음파 센서 설정
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);                                                                  

  myPID.SetMode(AUTOMATIC);               //PID모드를 AUTOMATIC으로 설정
  myPID.SetOutputLimits(100,-40);          //PID의 값을 최소 -80부터 최대 80까지 설정
}

void loop() {
  Setpoint = 20;                         //막대 중앙 위치(Set Point를 15cm로 설정)
  Input = readPosition();                //공의 위치 측정                            

  myPID.Compute();                       //PID계산 

  ServoOutput = 42+Output;               //서보모터의 각도 설정(100도는 서보모터가 수평을 이루었을 때 각도) 
  myServo.write(ServoOutput);            //서보모터에게 값 전달
}


float readPosition() {
  delay(10);                             //딜레이 설정 

  //초음파 센서 거리 측정 부분
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); 
  distance = ((float)(340 * duration) / 10000) / 2;  


  if(distance > 35) {                   //공의 측정거리가 30cm 이상일 경우 최대 30으로 설정
    distance=35;
  }

  Serial.println(distance);             //시리얼 모니터로 공의 거리 출력

  return distance;                      //측정값 반환
}

// 수정함
