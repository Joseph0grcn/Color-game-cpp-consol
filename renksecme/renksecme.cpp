// renksecme.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream> // Dosya işlemleri için gerekli kütüphane
#include <conio.h> // _kbhit() için gerekli kütüphane
#include <iomanip>  // setw, left için gerekli kütüphane
#include <thread>
#include <chrono>
using namespace std;

// Oyuncu verisi için bir yapı tanımlıyoruz
struct Player {
	string name; // Oyuncunun adı
	int score;   // Oyuncunun puanı

	// Oyuncu bilgilerini kolayca yazdırmak için bir fonksiyon
	void print() const {
		// Oyuncu bilgilerini hizalayarak yazdırma
		cout << "| "
			<< left << setw(10) << name  // 10 karakter genişliğinde, soldan hizalı
			<< " - "
			<< setw(3) << score  // 3 karakter genişliğinde, sağdan hizalı
			<< " puan |" << endl;// "puan" kelimesiyle birlikte yazdırma
	}
};

// Sıralama fonksiyonu: Puanlara göre azalan sırada sıralama
bool comparePlayers(const Player& a, const Player& b) {
	return a.score > b.score;
}

// Virtual Terminal özelliğini etkinleştirme
void enableVirtualTerminal() {
	// Konsol çıktısını almak için standart çıkışa (console) erişim sağlanıyor
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// Mevcut konsol modunu almak için değişken tanımlanıyor
	DWORD dwMode = 0;

	// Konsol modunu alıyoruz (virtual terminal işlemleri için)
	GetConsoleMode(hOut, &dwMode);

	// Virtual Terminal özelliklerini etkinleştiriyoruz
   // ENABLE_VIRTUAL_TERMINAL_PROCESSING bayrağını mevcut moda ekliyoruz
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	// Yeni modu uyguluyoruz
	SetConsoleMode(hOut, dwMode);
}

void printNumberWithColor(int number, int r, int g, int b) {
	// RGB renk ayarı yapmak için ANSI kaçış dizisi kullanılıyor
	// \033[38;2;r;g;b şeklinde RGB renk kodunu belirtiyoruz
	// Burada 'r', 'g' ve 'b' parametreleri ile gelen renk değerleri kullanılıyor
	cout << "\033[38;2;" << r << ";" << g << ";" << b << "m"
		<< number << number << number << " \033[0m"; // Renk sıfırlanıyor (sonraki yazılar etkilenmesin diye)
	
}

int playGame(int zorluk) {
	int tahmin;
	int targetIndex; // Rastgele bir sayı seç (0-8 arası)
	int puan = -1; // Puanı -1 ile başlatıyoruz, her doğru tahminde artırılacak
	int maxValue = zorluk; // Zorluk seviyesine bağlı olarak renk farkı büyüklüğü
	
	do
	{
		system("CLS");// Ekranı temizliyoruz

		// 0-8 arasında rastgele bir sayı seçiyoruz (hedef)
		targetIndex = rand() % 9;

		// Puanı bir artırıyoruz her döngüde
		puan++;
		cout << "puaniniz: " << puan <<endl;
		// Zorluk seviyesine göre renk farkını azaltıyoruz
		maxValue--;

		// RGB renklerini rastgele oluşturuyoruz
		int r = rand() % 255;
		int g = rand() % 255;
		int b = rand() % 255;

		int baseColor[3] = { r, g, b }; // Diğer 8 sayı için temel RGB rengi
		

		int randchange = maxValue;/*minValue + (rand() % (maxValue - minValue + 1));*/

		int rchange = r - randchange;
		int gchange = g - randchange;
		int bchange = b - randchange;

		// Eğer renk değeri negatifse, renk farkını düzeltiyoruz
		if (rchange < 0)
		{
			rchange += maxValue;
		}
		if (gchange < 0)
		{
			gchange += maxValue;
		}
		if (bchange < 0)
		{
			bchange += maxValue;
		}
		int diffColor[3] = { rchange, gchange, bchange };  // Farklı renk (temelden hafif farklı)

		// Kullanıcıya 1-9 arasındaki sayılardan birini tahmin etmesi için soruyoruz
		cout << "123456789 icinde bir sayinin rengi farklidir. Hangisi?" << endl;

		for (int i = 0; i < 9; ++i) {

			if (i % 3 == 0) {
				cout << "\n";// 3 sayıda bir satır atla
			}
			if (i == targetIndex) {
				// Rastgele seçilen sayı için farklı renk
				printNumberWithColor(i + 1, diffColor[0], diffColor[1], diffColor[2]);
			}
			else {
				// Diğer sayılar için temel renk
				printNumberWithColor(i + 1, baseColor[0], baseColor[1], baseColor[2]);
			}

		}
		// Kullanıcıdan tahmin alıyoruz
		cout << "Sayinizi giriniz :";
		cin >> tahmin;
	} while (tahmin-1 == targetIndex);// Kullanıcı yanlis sayıyı soyleyene kadar devam et
	cout << "tahmininiz yanlis dogru cevap :" << targetIndex+1 << "  Olacakti" << endl;
	cout << "aldiginiz puan : " << puan << endl << "ana menuye donmek icin enter tusuna basiniz.";
	while (!_kbhit()) {
		// Döngü, tuşa basılana kadar devam eder
	}
	return puan;
}
void liderTablosu(vector<Player> leaderboard) {
	// Dosyayı okuma işlemi
	ifstream inputFile("lider_tablosu.txt"); // Dosya adı: lider_tablosu.txt
	if (inputFile.is_open()) {
		string line;
		while (getline(inputFile, line)) {
			// Dosyadaki her satırı işleyerek oyuncuları ekle
			if (line != "Lider Tablosu:") { // Başlık satırını atla
				size_t pos = line.find(" - ");
				if (pos != string::npos) {
					string name = line.substr(0, pos); // Adı al
					int score = stoi(line.substr(pos + 3)); // Puanı al
					leaderboard.push_back({ name, score }); // Listeye ekle
				}
			}
		}
		inputFile.close();// Dosya işlemi tamamlandığında dosyayı kapatıyoruz
	}
	else {
		cerr << "Dosya açılamadı!" << endl;// Dosya açılamazsa hata mesajı veriyoruz
	}
	// Lider tablosunu yazdırıyoruz
	cout << "Lider Tablosu:" << endl;
	for (const auto& player : leaderboard) {
		player.print();// Her oyuncunun bilgilerini yazdırıyoruz
	}

	// Devam etmek için enter tuşuna basılmasını bekliyoruz
	cout << "devam etmek icin enter tusuna basiniz." << endl;
	while (!_kbhit()) {
		// Döngü, tuşa basılana kadar devam eder
	}
	system("CLS");// Ekranı temizliyoruz
}
void liderTablosu(vector<Player> leaderboard,string isim, int puan) {
	// Dosyayı okuma işlemi
	ifstream inputFile("lider_tablosu.txt"); // Dosya adı: lider_tablosu.txt
	if (inputFile.is_open()) {
		string line;
		while (getline(inputFile, line)) {
			// Dosyadaki her satırı işleyerek oyuncuları ekle
			if (line != "Lider Tablosu:") { // Başlık satırını atla
				size_t pos = line.find(" - ");
				if (pos != string::npos) {
					string name = line.substr(0, pos); // Adı al
					int score = stoi(line.substr(pos + 3)); // Puanı al
					leaderboard.push_back({ name, score }); // Listeye ekle
				}
			}
		}
		inputFile.close();
	}
	else {
		cerr << "Dosya açılamadı!" << endl;
	}

	// Yeni oyuncu (isim ve puan) lider tablosuna ekleniyor
	leaderboard.push_back({ isim, puan });
	// Lider tablosunu puanlarına göre sıralıyoruz
	sort(leaderboard.begin(), leaderboard.end(), comparePlayers);
	// Lider tablosunu yazdırıyoruz
	cout << "Lider Tablosu:" << endl;
	for (const auto& player : leaderboard) {
		player.print();
	}

	// Lider tablosunu bir dosyaya kaydediyoruz
	ofstream file("lider_tablosu.txt"); // Dosya adı: lider_tablosu.txt
	if (file.is_open()) {
		file << "Lider Tablosu:\n";
		for (const auto& player : leaderboard) {
			file << player.name << " - " << player.score << " puan\n";
		}
		file.close();
		cout << "\nLider tablosu 'lider_tablosu.txt' dosyasına kaydedildi.\ndevam etmek icin enter tusuna basiniz." << endl;
	}
	else {
		cerr << "Dosya açılamadı!" << endl;
	}
	// Tuşa basılana kadar bekle
	while (!_kbhit()) {
		// Döngü, tuşa basılana kadar devam eder
	}
}
int zorlukAyari() {
	int zorluk;
	cout << "Zorluk Ayarlari" << endl;
	cout << "kolay -> 1" << endl;
	cout << "normal-> 2" << endl;
	cout << "zor   -> 3" << endl;
	cout << "Giriniz." << endl;
	cout << "seciminiz: ";
	int secim;
	cin >> secim;// Kullanıcıdan zorluk seviyesini alıyoruz

	// Kullanıcının seçimine göre zorluk seviyesini ayarlıyoruz
	switch (secim) {
	case 1:
		zorluk = 150;// Kolay zorlukta fark 150 olacak
		cout << "zorluk seviyesi KOLAY olarak duzenlendi." << endl;
		break;
	case 2:
		zorluk = 100;// Normal zorlukta fark 100 olacak
		cout << "zorluk seviyesi NORMAL olarak duzenlendi." << endl;
		break;
	case 3:
		zorluk = 50;// Zor zorlukta fark 50 olacak
		cout << "zorluk seviyesi ZOR olarak duzenlendi." << endl;
		break;
	
	default:
		cout << "Gecersiz secim!" << endl; // Geçersiz bir seçim yapıldıysa hata mesajı
	}
	cout << "menuye devam etmek icin enter tusuna basiniz." << endl;
	while (!_kbhit()) {
		// Döngü, tuşa basılana kadar devam eder
	}
	return zorluk;// Seçilen zorluk seviyesini döndürüyoruz
}
void loading() {
	// Yükleniyor animasyonu için kullanılan semboller
	const char* animation[] = { "|", "/", "-", "\\" };
	int i = 0;// Animasyon sembolü için indeks

	// Yükleniyor mesajı ve ASCII sanatı
	string loadingMessage = "Yukleniyor...";
	string asciiArt = R"(
       __
      /  \
     /    \
    /______\
    )";// ASCII sanatını tanımlıyoruz

	auto start = chrono::steady_clock::now();// Zaman ölçmeye başlıyoruz

	// 5 saniye boyunca yükleniyor animasyonunu gösteriyoruz
	while (chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start).count() < 5) {
		system("CLS");// Ekranı temizliyoruz
		// Ekrana yükleniyor mesajını ve animasyon sembolünü yazdırıyoruz
		cout << "\r" << loadingMessage << " " << animation[i] << endl;
		cout << asciiArt << endl;  // ASCII sanatını ekliyoruz

		// Animasyon sembolünü döndürüp değiştiriyoruz
		i = (i + 1) % 4;

		// Animasyon arasındaki gecikme süresi: 100ms
		this_thread::sleep_for(chrono::milliseconds(100));
	}
	system("CLS");// Ekranı temizliyoruz
	// Yükleme tamamlandığında mesaj gösteriliyor
	cout << "\rYukleme Tamamlandi!         " << endl;
}


int main() {
	enableVirtualTerminal(); // Virtual Terminal'ı etkinleştiriyoruz, renkli çıktılar için
	vector<Player> leaderboard;// Lider tablosu için oyuncu listesi
	srand(static_cast<unsigned int>(time(0))); // Rastgele sayı üreticisini başlatıyoruz
	
	int puan;
	int choice;
	string isim;
	int zorluk=100;// Başlangıç zorluk seviyesi (normal)
	do {// Menü ekranını yazdırıyoruz
		cout << "=================================" << endl;
		cout << "=========== MENU ================" << endl;
		cout << "RENK OYUNUNA HOSGELDINIZ!" << endl;
		cout << "1. Oyunu Baslat" << endl;
		cout << "2. Cikis" << endl;
		cout << "3. lider tahtasi" << endl;
		cout << "4. ayarlar" << endl;
		cout << "Seciminiz: ";
		cin >> choice;// Kullanıcıdan seçim alıyoruz


		switch (choice) {
		case 1:
			// Oyunu başlatmak için isim alıyoruz ve oyuna başlıyoruz
			cout << "isminizi giriniz: ";
			cin >> isim;
			loading();// Yükleniyor animasyonunu başlatıyoruz
			puan = playGame(zorluk);// Oyunu oynatıyoruz ve puanı alıyoruz
			liderTablosu(leaderboard, isim, puan);// Lider tablosunu güncelliyoruz
			system("CLS");// Ekranı temizliyoruz
			break;
		case 2:
			system("CLS");// Ekranı temizliyoruz
			cout << "Cikiliyor..." << endl;// Çıkış işlemi
			break;
		case 3:
			system("CLS");// Ekranı temizliyoruz
			liderTablosu(leaderboard);// Lider tablosunu görüntülüyoruz
			break;
		case 4:
			system("CLS");// Ekranı temizliyoruz
			zorluk = zorlukAyari();// Zorluk seviyesini ayarlıyoruz
			system("CLS");// Ekranı temizliyoruz
			break;
		default:
			system("CLS");// Ekranı temizliyoruz
			cout << "Gecersiz secim!" << endl; // Geçersiz seçim yapılırsa hata mesajı
		}
	} while (choice != 2);// Kullanıcı çıkana kadar menüyü göster

	return 0;// Program sonlanıyor
}


// Programı çalıştır: Ctrl + F5 veya Hata Ayıkla > Hata Ayıklamadan Başlat menüsü
// Programda hata ayıkla: F5 veya Hata Ayıkla > Hata Ayıklamayı Başlat menüsü

// Kullanmaya Başlama İpuçları: 
//   1. Dosyaları eklemek/yönetmek için Çözüm Gezgini penceresini kullanın
//   2. Kaynak denetimine bağlanmak için Takım Gezgini penceresini kullanın
//   3. Derleme çıktısını ve diğer iletileri görmek için Çıktı penceresini kullanın
//   4. Hataları görüntülemek için Hata Listesi penceresini kullanın
//   5. Yeni kod dosyaları oluşturmak için Projeye Git > Yeni Öğe ekle veya varolan kod dosyalarını projeye eklemek için Proje > Var Olan Öğeyi Ekle adımlarını izleyin
//   6. Bu projeyi daha sonra yeniden açmak için Dosya > Aç > Proje'ye gidip .sln uzantılı dosyayı seçin
