#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

#define pinTrigger 35
#define pinEcho 34 

float leituraSimples();
float calculaDistance();
void sonarBegin(byte trig, byte echo);

float distancia;

//Codificação da imagem Logo ("TESTE") que aparecerá após iniciar a placa.

const unsigned char LOGO [] PROGMEN= {              
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x00, 0x01, 0xe0, 0x03, 0xf8, 0x1f, 0x00, 0x01, 0xe0, 0x03, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfe, 0x00, 0x01, 0xc0, 0x03, 0xe0, 0x07, 0x00, 0x00, 0xc0, 0x01, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfe, 0x00, 0x01, 0xc0, 0x03, 0xc0, 0x07, 0x00, 0x00, 0xc0, 0x01, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfe, 0x00, 0x01, 0xc3, 0xff, 0x83, 0xe7, 0x00, 0x01, 0xc1, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc3, 0xff, 0x87, 0xff, 0xf8, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc3, 0xff, 0x87, 0xff, 0xf8, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc3, 0xff, 0x83, 0xff, 0xf8, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc3, 0xff, 0x81, 0xff, 0xf8, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc0, 0x07, 0xc0, 0x3f, 0xf8, 0x3f, 0xc0, 0x07, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc0, 0x07, 0xe0, 0x1f, 0xf8, 0x3f, 0xc0, 0x07, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc0, 0x07, 0xf0, 0x07, 0xf8, 0x3f, 0xc0, 0x07, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc3, 0xff, 0xfc, 0x03, 0xf8, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc3, 0xff, 0xff, 0x03, 0xf8, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc3, 0xff, 0xff, 0x83, 0xf8, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc3, 0xff, 0xff, 0xc3, 0xf8, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc3, 0xff, 0xff, 0xc3, 0xf8, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc3, 0xff, 0x9f, 0x83, 0xf8, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc0, 0x03, 0x80, 0x07, 0xf8, 0x3f, 0xc0, 0x01, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc0, 0x03, 0x80, 0x07, 0xf8, 0x3f, 0xc0, 0x01, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x7f, 0xc0, 0x03, 0xc0, 0x1f, 0xfc, 0x3f, 0xc0, 0x01, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff  
};

void display_logo()
{
  display.clearDisplay();
  display.display();
  delay(1000);
  display.drawbitmap(0, 0, LOGO, 128, 64, 1);
  display.clearDisplay();
  display.display();
}

void setup()
{
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)
  {
    Serial.println(F("Display Oled Não Detectado."));
    for(;;)
  }

  display_logo();
  delay(500);
  display.clearDisplay();
  display.display();
  Serial.begin(9600);
  sonarBegin(pinTrigger, pinEcho);
}


void loop()
{
  distancia = calcularDistancia();

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setcursor(20,40);
  display.print(distancia);
  display.display();

  Serial.println("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
}

void sonarBegin (byte trig, byte echo)
{
  #define divisor 58.0
  #define intervaloMedida 35
  #define qtdMedida 20

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  digitalWrite(trig, LOW);
  delayMicroseconds(500);
}

float calculaDistancia()
{
  float leituraSoma = 0;
  float resultado = 0;

  for (int index = 0; index < qtdMedidas; index++)
  {
    delay(intervaloMedida);
   leituraSoma += leituraSimples();
  }

  resultado = (float) leituraSoma / qtdMedidas;
  resultados = resultado + 2.2;

  return resultado;

}

float leituraSimples()
{
  long duracao = 0;
  float resultado = 0;

  digitalWrite(pinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrigger, LOW);

  duracao = pulseIn(pinEcho, HIGH);
  resultado = ((float) duracao/divisor);

  return resultado;  
}

















