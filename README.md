# Renk Seçme Oyunu

**Renk Seçme Oyunu**, Bu proje, kullanıcıların görsel algılarını test eden bir renk tahmin oyunudur. Oyunda, kullanıcı verilen sayı dizisinden farklı renge sahip olan sayıyı tahmin etmeye çalışır. Kullanıcılar doğru tahmin yaptıkça puan kazanır, yanlış tahmin ettiğinde oyun sona erer. Oyunun lider tablosu ve zorluk seviyesi gibi ek özellikleri bulunmaktadır.

## Özellikler

- **Farklı Zorluk Seviyeleri:** Kolay, normal ve zor seviyelerle oyun zorluğunu ayarlayabilirsiniz.
- **Renkli Arayüz:** Virtual Terminal desteğiyle zengin renkli bir çıktı sunar.
- **Liderlik Tablosu:** Oyuncuların skorlarını kaydeder ve sıralar.
- **Dinamik Animasyonlar:** Yükleniyor ekranı ve etkileyici ASCII sanatı içerir.

## Kullanılan Teknolojiler ve Araçlar

- **C++**: Projenin geliştirilmesi için kullanılan ana programlama dili.
- **Windows API** (Virtual Terminal kullanımı için)
- **cstdlib, <ctime>:** Rastgele sayı üretimi için
- **fstream:** Dosya okuma/yazma işlemleri
- **conio.h:** Klavye giriş kontrolü (_kbhit())
- **iomanip:** Formatlı yazdırma için
- **thread ve chrono:** Zamanlama ve animasyonlar


## Nasıl Çalışır?

1. **Ana Menü:**  
   - "Oyunu Başlat": Oyuna başlar, isim girilir ve puan hesaplanır.  
   - "Çıkış": Uygulamadan çıkış yapılır.  
   - "Liderlik Tablosu": Tüm oyuncuların sıralamasını gösterir.  
   - "Ayarlar": Zorluk seviyesi belirlenir.  

2. **Oyun Akışı:**  
   - Renk farkı gittikçe azalır ve oyuncu doğru sayıyı tahmin etmeye çalışır.  
   - Her yanlış tahminde oyun sona erer ve puan kaydedilir.  

3. **Lider Tablosu:**  
   - Oyuncu skorları bir dosyada saklanır (`lider_tablosu.txt`).  
   - Oyun sonunda sıralama güncellenir ve dosya üzerine yazılır.

## Nasıl Çalıştırılır?

1. Kaynak kodu bilgisayarınıza indirin.  
2. Bir C++ derleyicisi kullanarak (örneğin Visual Studio veya GCC) projeyi derleyin.  
3. Terminalden çalıştırın ve oyunun keyfini çıkarın!

## Örnek Kod Parçası

```cpp
void printNumberWithColor(int number, int r, int g, int b) {
    cout << "\033[38;2;" << r << ";" << g << ";" << b << "m"
         << number << number << number << " \033[0m";
}
