# Yemek Yiyen Filozoflar Problemi – Çözüm 2 (`Philosophers_main`)

## Proje Hakkında
Selam! Bu klasörde, **Yemek Yiyen Filozoflar Problemi**'nin biraz daha gelişmiş ve temiz bir çözümünü bulacaksın. C dili ve pthread (POSIX thread) kullanılarak, hem deadlock (kilitlenme) hem de aç kalan filozoflar gibi dertlere karşı sağlam bir senkronizasyon sağlanıyor. Kodun okunabilirliği ve modülerliği ön planda tutuldu.

## Gereksinimler
- GCC (C derleyicisi)
- Make
- Linux ortamı (ben WSL2 üzerinde test ettim)
- pthread kütüphanesi

## Nasıl Derlenir?
1. Terminali açıp bu klasöre gel:
   ```sh
   cd Philosophers_main
   ```
2. Sonra tek komutla derleyebilirsin:
   ```sh
   make
   ```
   Sonunda `philo` adında bir çalıştırılabilir dosya oluşacak.

## Nasıl Çalıştırılır?
Programı şu şekilde başlatabilirsin:
```sh
./philo filozof_sayısı ölme_süresi yeme_süresi uyuma_süresi [her_filozofun_yemesi_gereken_sayı]
```
- `filozof_sayısı`: Masadaki filozof (ve çatal) sayısı
- `ölme_süresi`: Bir filozofun yemeden dayanabileceği maksimum süre (ms)
- `yeme_süresi`: Bir filozofun yemek yemesi için geçen süre (ms)
- `uyuma_süresi`: Bir filozofun uyuması için geçen süre (ms)
- `her_filozofun_yemesi_gereken_sayı` (opsiyonel): Eğer girilirse, her filozof bu kadar kez yedikten sonra simülasyon biter

**Örnek:**
```sh
./philo 5 800 200 200 7
```

## Kodun Yapısı
- `source/main.c`: Programın başlangıcı, argüman kontrolü ve ana döngü
- `include/philo.h`: Yapılar, makrolar ve fonksiyon prototipleri
- `source/init.c`: Başlangıçta yapılan tüm ayarlamalar (filozoflar, çatallar vs.)
- `source/actions.c`: Filozofun temel hareketleri (yemek, uyumak, düşünmek), thread rutinleri ve izleme
- `source/utils.c`: Yardımcı fonksiyonlar (zaman, yazdırma, temizlik, dönüşümler)
- `Makefile`: Derleme talimatları

## Algoritma & Senkronizasyon
- Her filozof bir thread olarak çalışıyor.
- Çatallar mutex ile temsil ediliyor, böylece aynı anda iki filozof aynı çatalı alamıyor.
- Deadlock (kilitlenme) olmaması için:
  - Filozofların başlama zamanları hafifçe kaydırılıyor
  - Çatalların alınma sırası dikkatli seçiliyor
  - Bir izleyici (monitor) thread'i, ölüm ve bitiş durumlarını kontrol ediyor
- Mutex'ler şunlar için kullanılıyor:
  - Çatal erişimi (aynı anda iki kişi aynı çatalı almasın diye)
  - Terminale yazdırma (mesajlar karışmasın diye)
  - Ölüm ve bitiş kontrolü
  - Her filozofun son yeme zamanı ve yeme sayısı takibi
- Tek filozof varsa, o zavallı yine ölüyor :)

## Hata Yönetimi
- Yanlış argüman girilirse veya bir şeyler ters giderse, ekrana açıklayıcı hata mesajları yazılıyor.
- Programdan çıkarken tüm kaynaklar (thread, mutex, bellek) düzgünce temizleniyor.

## Notlar
- Kodun içinde bolca açıklama var, "bu niye böyle?" diye düşünmene gerek kalmaz.
- Modüler ve okunabilir bir yapı var, istersen kolayca geliştirebilirsin.

## Yazar
- eakman (arcemirhanakman@gmail.com)

---

Daha fazla detay için kaynak dosyalardaki yorumlara göz atabilirsin. Kolay gelsin! :) 