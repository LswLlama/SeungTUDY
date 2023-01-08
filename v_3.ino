#include <PID_v1.h>
#include <Servo.h> //PID, 서보모터 라이브러리

const int servoPin = 9;
const int echoPin = 6;
const int trigPin = 7;
const int sensor = A0; //핀 설정

float Kp = 2.0;
float Ki = 1.0;
float Kd = 2.0; //PID 상수 설정, 실험에 따라 달라짐, 중요!
double Setpoint, Input, Output, ServoOutput; //PID 제어 변수

float duration, distance1, distance2; //거리 측정 변수

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT); //PID 객체 생성
Servo myServo; //서보 객체 생성


void setup() {
  Serial.begin(9600); //시리얼 통신 사용
  
   myServo.attach(servoPin);
   myServo.write(); //슬로프가 평형을 이루는 각도 : 115도

   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);
   pinMode(sensor, INPUT); //입력, 출력 핀

  myPID.SetMode(AUTOMATIC); //PID모드를 AUTOMATIC으로 설정
  myPID.SetOutputLimits(-50,55); //PID의 값을 최소 -50부터 최대 55까지 설정
}


void loop() {
   Setpoint = map(analogRead(sensor), 0, 1023, 5, 40); //가변저항 범위를 슬로프의 길이 범위로 변경
   Input = errorCollection(); //오차값 보정한 초음파 센서의 거리 수치를 저장
   Serial.print(Input);
   Serial.print(",");
   Serial.println(Setpoint); //시리얼 출력

   myPID.Compute(); //PID계산
   ServoOutput=115-Output; //서보모터의 각도 설정(115도는 서보모터가 수평을 이루었을 때 각도)
   myServo.write(ServoOutput); //서보모터에게 값 전달
  }


float readPosition() { //초음파 센서 거리 측정
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(2);
   digitalWrite(trigPin, LOW);

   duration = pulseIn(echoPin, HIGH);
   float a = ((float)(340 * duration) / 10000) / 2;

   return a;
}


float errorCollection() { //초음파 센서로 측정한 값 오차 보정
   delay(40);
   float distance;
   distance2=readPosition(); //distance2에 거리값 저장
  
   if (distance2-distance1>45) { //차가 45보다 크다면, 즉 값이 갑자기 튄다면
    distance=distance1; //바로 전 측정한 거리값 사용
   }
   else { //일반적인 경우이면
     distance=distance2; //지금 측정한 값을 사용
     distance1=distance2; //다음 측정값과 비교하기 위해 저장
    }

  if (distance>45) { //슬로프 길이가 45cm이므로 45cm보다 더 큰 값이 반환되면 오류
   distance=45;
  }

 return distance; //distance 리턴
}
