#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(52, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int getFingerprintIDez();
uint8_t id;
char Kontrol = '9'; // IF ELSE kontrolü için

void setup()  
{
                                                                Serial.begin(9600);
                                                                while (!Serial);
                                                                delay(100);
                                                                
                                                              
                                                                // set the data rate for the sensor serial port
                                                                finger.begin(57600);
                                                                delay(5);
                                                                if (finger.verifyPassword()) {
                                                                  Serial.println(" Parmak İzi Sensörü Bulundu!");
                                                                  Aciklama();
                                                                } else {
                                                                  Serial.println("Parmak İzi Sensörü Bulunamadı :(");
                                                                  while (1) { delay(1); }
                                                                }
  

}


void Aciklama(){
Serial.println(" Bekleme Modundadır Bir işlem Seçene Kadar Bekler. \n İşlem Seçmek için Lütfen Bir Sayı Giriniz. ");
Serial.println(" 0 - Yoklama Başlatma \n 1 - Öğrenci Kaydı \n 2 - Öğrenci Silme \n 3 - Tüm Kayıtları Temizlemek ");
}

void loop()

{
char Kontrol = Serial.read();
if (Kontrol == '0'){
  Serial.println("Yoklama Başladı ");
  while(1)
    {getFingerprintIDez();}
}
if (Kontrol == '1'){
  Serial.println("\n Parmak İzi Kaydına Hazır!");
  Serial.println("\n Lütfen (1 ile 127) Arasında Kaydetmek İstediğiniz ID NO Giriniz.");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
 
  getFingerprintEnroll();
  Serial.println("\n \n \n \n \n");
  Aciklama();
  char Kontrol = '9';
  }

if ( Kontrol == '3'){
  finger.emptyDatabase();

  Serial.println("\n Veri Tabanı Temizlendi :) \n");
  Aciklama();
  char Kontrol = '9';
}

if ( Kontrol == '2'){
  Serial.println("Lütfen (1 ile 127) Arasında Silmek İstediğiniz ID NO Giriniz. ");
  uint8_t id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }

  Serial.print("Siliniyor ID #");
  Serial.println(id);
  
  deleteFingerprint(id);
  
}

}
  


uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Parmak Resim Alındı");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Parmak Testip Edilemedi!");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("İletişim Hatası");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Parmak Resimleştirme Hatası");
      return p;
    default:
      Serial.println("Bilinmeyen Hata");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Resim Dönüştürüldü");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Resim Çok Dağınık! Hata!");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("İletişim Hatası");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Parmak İzi Girdileri Çıkartılamadı");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Parmak İzi Girdileri Çıkartılamadı");
      return p;
    default:
      Serial.println("Bilinmeyen Hata");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Parmak İzi Eşleşmesi Bulundu!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("İletişim Hatası");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Eşleşme Bulunamadı");
    return p;
  } else {
    Serial.println("Bilinmeyen Hata");
    return p;
  }   
  
  // found a match!
  Serial.print("Bulunan ID #"); Serial.print(finger.fingerID); 
  Serial.print(" Güven Oranı "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Bulunan ID #"); Serial.print(finger.fingerID); 
  /* #############################################################################################################################################################################
   * Buraya fonksiyonun adını yazacaksın örnek veriyorum
   * 
   * api_id_gönder(finger.fingerID); <---- finger.fingerID eşleşen parmağın ID sini barındırıyor. Bunu fonksiyona göndereceksin ve o fonksiyon o ID değerini API ye gönderecek.
   * ##############################################################################################################################################################################
   */
  Serial.print(" Güven Oranı "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Kayıt Edilmek İçin Parmak Bekleniyor. #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Parmak izi Fotoğraf Alındı");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("İletişim Hatası");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Parmak izi resimi Oluşturma Hatası");
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
      Serial.println("Parmak İzi Resmi Dönüştürüldü.");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Parmak İzi Resmi Çok Karışık");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("İletişim Hatası");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Parmak İzi Girdileri Çıkartılamadı");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Parmak İzi Girdileri Çıkartılamadı");
      return p;
    default:
      Serial.println("Bilinmeyen Hata");
      return p;
  }
  
  Serial.println("Parmağınızı Kaldırın");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Kayıt Ettiğiniz Aynı Parmağı Tekrar Okutun");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Parmak izi Resmi Alındı");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("İletişim Hatası");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Parmak İzi Resim çıkartma Hatası");
      break;
    default:
      Serial.println("Bilinmeyen Hata");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Parmak İzi Resmi Dönüştürüldü.");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Parmak İzi Resmi Çok Karışık");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("İletişim Hatası");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Parmak İzi Resim çıkartma Hatası");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Parmak İzi Resim çıkartma Hatası");
      return p;
    default:
      Serial.println("Bilinmeyen Hata");
      return p;
  }
  
  // OK converted!
  Serial.print("Model Yaratılıyor #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Parmak İzleri Eşleşti!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("İletişim Hatası");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Parmak İzleri Uyuşmadı");
    return p;
  } else {
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
    Serial.println("Parmak İzi Resim çıkartma Hatası");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Flash üzerine Yazarken Hata Oluştu");
    return p;
  } else {
    Serial.println("Bilinmeyen Hata");
    return p;
  }   
  
}


uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Silindi !");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("İletişim Hatası");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Bu Konumdaki Veri Silinemiyor");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Flash üzerine Yazarken Hata Oluştu");
    return p;
  } else {
    Serial.print("Bilinmeyen Hata : 0x"); Serial.println(p, HEX);
    return p;
  }   
}
