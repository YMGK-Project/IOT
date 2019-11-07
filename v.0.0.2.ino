#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(52, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int getFingerprintIDez();
String Degisken;
uint8_t id;
int Kontrol = 0;
void setup()  
{
                                                                Serial.begin(9600);
                                                                while (!Serial);  // For Yun/Leo/Micro/Zero/...
                                                                delay(100);
                                                                Serial.println("Lütfen Bir İşlem Seçiniz ! Kayıt İçin : 1, ");
                                                              
                                                                // set the data rate for the sensor serial port
                                                                finger.begin(57600);
  

  }

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()                     // run over and over again
{ 
  
  char Kontrol = Serial.read();
  // 1 = 2 = Yoklama Kontrol        
  if (Kontrol == '2'){
  ParmakiziSensorunuDogrula();
  temizle();
  parmakAL();
  hexAL();
  Serial.println(Degisken);
  
  }
  else if (Kontrol == '3'){  
    //hexAL();
    }
  }

/* 
OLANLAR:
- HER BAŞLADIĞINDA 1 TUŞU İLE YOKLAMA AÇILDIĞINDA DİREK 1 NUMARALI ID ÜZERİNE KAYIT YAPILIYOR.
- PARMAK İZİ SADECE 1 DEFA ALINIYOR
- İŞLEM BAŞLAMADAN ÖNCE 1 NUMARALI ID TEMİZLENİYOR.

EKSİKLER:


- WİFİ İLE API YE BAĞLANMAK. APIDEN LİSTE VE İŞLEMİN ÇEKİLMESİ. YOKLAMAYI BAŞLATACAK ( YÜKLEME İŞLEMİ BİTTİĞİNDE YOKLAMA BAŞLAR.) VE BİTİRECEK ( YOKLAMAYI KAPATTIĞINDA BİTECEK VE API YE LİSTEYİ GÖNDERECEK) FONKSİYONLAR.
- KAYDEDİLEN PARMAĞIN ID Sİ LİSTE İLE KARŞILAŞTIRILMALI ( GEÇİÇİ ÇÖZÜM OLARAK HEX DOSYASINI KAYDETMEK VE ÖNCEDNE KAYDETTİĞİMİZ BİR PARMAK İZİ İLE KARŞILAŞTIRMAK )



*/

void ParmakiziSensorunuDogrula(){

                                                                          if (finger.verifyPassword()) {
                                                                      Serial.println("Parmak İzi Sensörü Bulundu !");
                                                                    } else {
                                                                      Serial.println("Parmak İzi Sensörü Bulunamadı !");
                                                                      while (1) { delay(1); }
  
}}

void parmakAL(){                                                
                                                                      Serial.println("Parmak İzi Kaydı İçin Hazır!");
                                                                      Serial.println("Lütfen (1 ile 127) Arasında Kaydetmek İstediğiniz ID NO Giriniz.");
                                                                      id = 1;
                                                                      if (id == 0) {// ID #0 not allowed, try again!
                                                                         return;
                                                                      }
                                                                      Serial.print("ID Kayıt Ediliyor. ID #");
                                                                      Serial.println(id);

 parmakKontrol();
}


uint8_t parmakKontrol() {

                                                                    int p = -1;
                                                                    Serial.print("Yoklama girişi için parmak izi bekleniyor. as #"); Serial.println(id);
                                                                    while (p != FINGERPRINT_OK) {
                                                                      p = finger.getImage();
                                                                      switch (p) {
                                                                      case FINGERPRINT_OK:
                                                                        Serial.println("Parmak izi Alındı");
                                                                        break;
                                                                      case FINGERPRINT_NOFINGER:
                                                                        Serial.println(".");
                                                                        break;
                                                                      case FINGERPRINT_PACKETRECIEVEERR:
                                                                        Serial.println("İletişim Hatası");
                                                                        break;
                                                                      case FINGERPRINT_IMAGEFAIL:
                                                                        Serial.println("Parmak izi hatası Hatası");
                                                                        break;
                                                                      default:
                                                                        Serial.println("Bilinmeyen Hata");
                                                                        break;
                                                                      }
                                                                    }
                                                                  
                                                                    // OK success!
                                                                  
                                                                    p = finger.image2Tz(1);
                                                                    switch (p) {
                                                                      case FINGERPRINT_OK:
                                                                        Serial.println("Resim Dönüştürüldü");
                                                                        break;
                                                                      case FINGERPRINT_IMAGEMESS:
                                                                        Serial.println("Parmak izi Çok Dağınık");
                                                                        return p;
                                                                      case FINGERPRINT_PACKETRECIEVEERR:
                                                                        Serial.println("İletişim Hatası");
                                                                        return p;
                                                                      case FINGERPRINT_FEATUREFAIL:
                                                                        Serial.println("Parmak izi Girdisi Bulunamadı");
                                                                        return p;
                                                                      case FINGERPRINT_INVALIDIMAGE:
                                                                        Serial.println("Parmak izi Girdisi Bulunamadı");
                                                                        return p;
                                                                      default:
                                                                        Serial.println("Bilinmeyen Hata");
                                                                        return p;
                                                                    }
                                                                    Serial.print("ID "); Serial.println(id);
                                                                    p = finger.storeModel(id);
                                                                    if (p == FINGERPRINT_OK) {
                                                                      Serial.println("Kaydedildi!");
                                                                    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
                                                                      Serial.println("İletişim Hatası");
                                                                      return p;
                                                                    } else if (p == FINGERPRINT_BADLOCATION) {
                                                                      Serial.println("Bu Alana Kayıt Edilemez");
                                                                      return p;
                                                                    } else if (p == FINGERPRINT_FLASHERR) {
                                                                      Serial.println("Flasha Yazarken Hata Oluştu");
                                                                      return p;
                                                                    } else {
                                                                      Serial.println("Bilinmeyen Hata");
                                                                      return p;
                                                                    }   
                                                                  


}



void temizle(){
uint8_t id = 1;
                                                                    if (id == 0) {// ID #0 not allowed, try again!
                                                                       return;
                                                                    }
                                                                  
                                                                     Serial.print("Temizleniyor. ID #");
                                                                    Serial.println(id);
                                                                    
                                                                    deleteFingerprint(id);
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
                                                                    p = finger.deleteModel(id);
                                                                  
                                                                    if (p == FINGERPRINT_OK) {
                                                                      Serial.println("Temizlendi!");
                                                                    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
                                                                      Serial.println("İletişim Hatası");
                                                                      return p;
                                                                    } else if (p == FINGERPRINT_BADLOCATION) {
                                                                      Serial.println("Bu konum silinemez");
                                                                      return p;
                                                                    } else if (p == FINGERPRINT_FLASHERR) {
                                                                      Serial.println("Flasha Yazma Hatası");
                                                                      return p;
                                                                    } else {
                                                                      Serial.print("Bilinmeyen Hata: 0x"); Serial.println(p, HEX);
                                                                      return p;
                                                                    }   
}


void hexAL(){

downloadFingerprintTemplate(1);

}

uint8_t downloadFingerprintTemplate(uint16_t id)
{
  Serial.println("------------------------------------");
  Serial.print("Attempting to load #"); Serial.println(id);
  uint8_t p = finger.loadModel(id);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.print("Kalıp "); Serial.print(id); Serial.println(" Yüklendi");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("İletişim Hatası");
      return p;
    default:
      Serial.print("Bilinmeyen Hata"); Serial.println(p);
      return p;
  }

  // OK success!

  Serial.print("Şablonu Tanımlanıyor #"); Serial.println(id);
  p = finger.getModel();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.print("Şablon "); Serial.print(id); Serial.println(" Transfer Ediliyor.:");
      break;
   default:
      Serial.print("Bilinmeyen Hata "); Serial.println(p);
      return p;
  }
  
  // one data packet is 267 bytes. in one data packet, 11 bytes are 'usesless' :D
  uint8_t bytesReceived[534]; // 2 data packets
  memset(bytesReceived, 0xff, 534);

  uint32_t starttime = millis();
  int i = 0;
  while (i < 534 && (millis() - starttime) < 20000) {
      if (mySerial.available()) {
          bytesReceived[i++] = mySerial.read();
      }
  }
  Serial.print(i); Serial.println(" Byte Okunuyor.");
  Serial.println("Paket Çözümleniyor...");

  uint8_t fingerTemplate[512]; // the real template
  memset(fingerTemplate, 0xff, 512);

  // filtering only the data packets
  int uindx = 9, index = 0;
  while (index < 534) {
      while (index < uindx) ++index;
      uindx += 256;
      while (index < uindx) {
          fingerTemplate[index++] = bytesReceived[index];
      }
      uindx += 2;
      while (index < uindx) ++index;
      uindx = index + 9;
  }
  Degisken = "";
  for (int i = 0; i < 256; ++i) {
      //Serial.print("0x");
      Degisken = Degisken + printHex(fingerTemplate[i], 2);
      //Serial.print(", ");
  }
  
  
  Serial.println("\nBitti.");
}



String printHex(int num, int precision) {
    char tmp[16];
    char format[128];
 
    sprintf(format, "%%.%dX", precision);
 
    sprintf(tmp, format, num);
    //Serial.print(tmp);
    return tmp;
    
}

 
