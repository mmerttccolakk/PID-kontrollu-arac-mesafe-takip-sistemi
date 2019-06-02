//kutuphane_tanimlamalari
#include <Ultrasonic.h>
#include <PID_v1.h>

//pin tanimlamalari
#define motor_ileri 11
#define motor_geri 10
#define buzzer 9
#define trig 8
#define echo 7

//sabit deger tanimlama
#define max_timeout 3480 //max 60cm TimeOut = Max.Distance(cm) * 58[µs]
#define mesafe 30 //korunacak mesafe [cm]

//degisken tanimlamalari
int anlik_uzaklik;

//pid icin referans_giris_cikis_degisken
double Setpoint, Input, Output;

//ozel pid parametreleri
double Kp=120, Ki=60, Kd=20;

//uzaklik_sensoru_tanimlama
Ultrasonic ultrasonic(trig,echo,max_timeout);

//pid_tanimlama
PID motor_pwm(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

//ilk_baslangic
void setup() {
  //debug_icin_serial
  Serial.begin(9600); 
  
  //giris_cikis_tanimlama
  pinMode(motor_ileri, OUTPUT);
  pinMode(motor_geri, OUTPUT);
  pinMode(buzzer, OUTPUT);

  //istenen mesafe 30cm = 255/2 [60 üzerinden 30]
  Setpoint = 127;

  //pid sinir ve mod ayarlari
  motor_pwm.SetOutputLimits(-80,80);//cikis_limitleri belirlendi
  motor_pwm.SetSampleTime(1); //calisma_periyodu 5ms
  motor_pwm.SetMode(AUTOMATIC); //pid calistirildi
}

//ana_program_dongusu
void loop() {

    //anlik_uzaklik_olculuyor
    anlik_uzaklik=ultrasonic.Ranging(CM);
    Input = map(anlik_uzaklik, 0, 60, 0, 255); //dikkat max mesafe dogru girilmeli [timeout] 2900µs=100cm
    motor_pwm.Compute();

Serial.println(Output);

if(Output<0){
    analogWrite(motor_geri, 0);
    analogWrite(motor_ileri, -Output);
  }else{
    analogWrite(motor_ileri, 0);
    analogWrite(motor_geri, Output);
    }

}
