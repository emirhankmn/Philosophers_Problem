# Yemek Yiyen Filozoflar Problemi – Çözüm 1 (`problemcozumu_2`)

## Proje Hakkında
Merhaba! Bu proje, klasik **Yemek Yiyen Filozoflar Problemi**'nin C ve POSIX thread'leri (pthread) ile çözümünü içeriyor. Amacımız, bir masada oturan filozofların hem aç kalmadan hem de birbirlerini engellemeden yemek yiyip düşünmesini sağlamak. Tabii ki, işin içinde "çatallar" ve "senkronizasyon" olunca işler biraz karışıyor :)

## Gereksinimler
- GCC (C derleyicisi)
- Make
- Linux ortamı (ben WSL2 üzerinde test ettim)
- pthread kütüphanesi

## Nasıl Derlenir?
1. Terminali açıp bu klasöre gel:
   ```sh
   cd problemcozumu_2
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
- `main.c`: Programın başlangıcı, argüman kontrolü ve ana döngü
- `philo.h`: Yapılar, makrolar ve fonksiyon prototipleri
- `init.c`: Başlangıçta yapılan tüm ayarlamalar (filozoflar, çatallar vs.)
- `threads.c`: Thread oluşturma, izleme ve filozofun yaşam döngüsü
- `actions.c`: Filozofun temel hareketleri (yemek, uyumak, düşünmek) ve çatal yönetimi
- `utils.c`: Yardımcı fonksiyonlar (zaman, string karşılaştırma, temizlik, hata yönetimi)
- `Makefile`: Derleme talimatları

## Algoritma & Senkronizasyon
- Her filozof bir thread olarak çalışıyor.
- Çatallar mutex ile temsil ediliyor, böylece aynı anda iki filozof aynı çatalı alamıyor.
- Her filozof önce sol, sonra sağ çatalı almaya çalışıyor. Deadlock (kilitlenme) olmaması için dikkatli bir sıralama ve izleme var.
- Bir "monitor" thread'i, filozoflardan biri ölürse veya herkes yeterince yediyse simülasyonu bitiriyor.
- Yazdırma işlemleri de mutex ile korunuyor, böylece terminalde mesajlar karışmıyor.
- Tek filozof varsa, o zavallı maalesef ölüyor :)

## Hata Yönetimi
- Yanlış argüman girilirse veya bir şeyler ters giderse, ekrana açıklayıcı hata mesajları yazılıyor.
- Programdan çıkarken tüm kaynaklar (thread, mutex, bellek) düzgünce temizleniyor.

## Notlar
- Kodun içinde bolca açıklama var, okurken "neden böyle yapmış?" diye düşünmene gerek kalmaz.
- Modüler ve okunabilir bir yapı var, istersen kolayca geliştirebilirsin.

## Yazar
- eakman (arcemirhanakman@gmail.com)

---

Daha fazla detay için kaynak dosyalardaki yorumlara göz atabilirsin. Kolay gelsin! :) 