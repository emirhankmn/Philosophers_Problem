# Çoklu İş Parçacığı (Thread), Mutex ve Senkronizasyon – Kapsamlı Dokümantasyon

## Giriş
Bu doküman, çoklu iş parçacığı (thread), mutex (karşılıklı dışlama kilidi) ve senkronizasyon kavramlarını hem teorik hem de pratik olarak detaylıca açıklar. Ayrıca, bu kavramların farklı yazılım dillerinde nasıl kullanıldığını örneklerle gösterir. Amacımız, bu konularda hem temel hem de ileri düzeyde bilgi sunmak ve gerçek hayatta karşılaşabileceğiniz sorunlara çözüm yolları önermektir.

---

## 1. Thread (İş Parçacığı) Nedir?
Bir programın aynı anda birden fazla işi yapabilmesini sağlayan, işletim sistemi tarafından yönetilen en küçük çalışma birimidir. Thread'ler, aynı süreç (process) içinde çalışır ve belleği paylaşır.

### Thread Yapısı
```
┌─────────────────────────────────────┐
│              Process                │
│  ┌─────────────┐  ┌─────────────┐   │
│  │   Thread 1  │  │   Thread 2  │   │
│  │             │  │             │   │
│  │  - Stack    │  │  - Stack    │   │
│  │  - Registers│  │  - Registers│   │
│  │  - PC       │  │  - PC       │   │
│  └─────────────┘  └─────────────┘   │
│                                     │
│  ┌─────────────────────────────────┐ │
│  │        Shared Memory            │ │
│  │  - Global Variables             │ │
│  │  - Heap                         │ │
│  │  - Code                         │ │
│  └─────────────────────────────────┘ │
└─────────────────────────────────────┘
```

### Neden Thread Kullanılır?
- Paralel işlem yapmak (ör. bir yandan dosya indirirken bir yandan kullanıcı arayüzünü güncel tutmak)
- Performansı artırmak (çok çekirdekli işlemcilerde)
- I/O işlemlerini (ağ, disk) beklerken ana programı kilitlememek

### Gerçek Hayattan Analojiler
- Bir restoranda birden fazla garsonun aynı anda farklı masalara hizmet etmesi
- Bir fabrikanın farklı bantlarında aynı anda üretim yapılması

### Thread Yaşam Döngüsü
```
    ┌─────────┐
    │  NEW    │ ← Thread oluşturuldu
    └────┬────┘
         │
         ▼
    ┌─────────┐
    │ RUNNABLE│ ← Çalışmaya hazır
    └────┬────┘
         │
         ▼
    ┌─────────┐
    │ RUNNING │ ← Aktif olarak çalışıyor
    └────┬────┘
         │
         ▼
    ┌─────────┐
    │ BLOCKED │ ← I/O veya kilit bekliyor
    └────┬────┘
         │
         ▼
    ┌─────────┐
    │  DEAD   │ ← Thread sonlandı
    └─────────┘
```

### Thread Kullanımına Örnekler
#### C (POSIX pthread)
```c
#include <pthread.h>
#include <stdio.h>

void* print_hello(void* arg) {
    printf("Merhaba, thread!\n");
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, print_hello, NULL);
    pthread_join(tid, NULL);
    return 0;
}
```

#### Python
```python
import threading

def merhaba():
    print("Merhaba, thread!")

thread = threading.Thread(target=merhaba)
thread.start()
thread.join()
```

#### Java
```java
public class Main {
    public static void main(String[] args) {
        Thread t = new Thread(() -> System.out.println("Merhaba, thread!"));
        t.start();
        try { t.join(); } catch (InterruptedException e) {}
    }
}
```

---

## 2. Mutex (Karşılıklı Dışlama Kilidi) Nedir?
Birden fazla thread'in aynı anda paylaşılan bir kaynağa (ör. değişken, dosya) erişmesini engelleyen kilit mekanizmasıdır. "Mutual Exclusion" kelimelerinin kısaltmasıdır.

### Mutex Çalışma Prensibi
```
Thread A                    Mutex                    Thread B
    │                         │                         │
    │─── Lock Request ───────▶│                         │
    │                         │                         │
    │◀─── Lock Granted ───────│                         │
    │                         │                         │
    │─── Lock Request ───────▶│                         │
    │                         │                         │
    │◀─── Lock Denied ────────│                         │
    │                         │                         │
    │                         │                         │
    │─── Unlock ─────────────▶│                         │
    │                         │                         │
    │                         │◀─── Lock Granted ───────│
    │                         │                         │
    │                         │◀─── Unlock ─────────────│
```

### Mutex Durumları
```
┌─────────────────────────────────────────────────────────┐
│                    MUTEX STATES                         │
│                                                         │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐  │
│  │   UNLOCKED  │───▶│   LOCKED    │───▶│   WAITING   │  │
│  │             │    │             │    │             │  │
│  │ Owner: None │    │ Owner: T1   │    │ Owner: T1   │  │
│  │ Queue: Empty│    │ Queue: Empty│    │ Queue: T2,T3│  │
│  └─────────────┘    └─────────────┘    └─────────────┘  │
│         ▲                   │                   │       │
│         │                   │                   │       │
│         └───────────────────┴───────────────────┘       │
└─────────────────────────────────────────────────────────┘
```

### Neden Mutex Kullanılır?
- Veri tutarlılığını sağlamak
- Yarış durumu (race condition) ve veri bozulmasını önlemek

### Gerçek Hayattan Analojiler
- Tuvaletin kapısındaki kilit: Bir kişi içerideyken başkası giremez.
- ATM: Aynı anda iki kişi aynı hesaptan para çekemez.

### Mutex Kullanımına Örnekler
#### C (pthread_mutex)
```c
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock;
int sayac = 0;

void* arttir(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock);
        sayac++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&t1, NULL, arttir, NULL);
    pthread_create(&t2, NULL, arttir, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Sonuç: %d\n", sayac);
    pthread_mutex_destroy(&lock);
    return 0;
}
```

#### Python (threading.Lock)
```python
import threading
sayac = 0
lock = threading.Lock()

def arttir():
    global sayac
    for _ in range(100000):
        with lock:
            sayac += 1

thread1 = threading.Thread(target=arttir)
thread2 = threading.Thread(target=arttir)
thread1.start()
thread2.start()
thread1.join()
thread2.join()
print("Sonuç:", sayac)
```

#### Java (synchronized)
```java
public class Main {
    static int sayac = 0;
    static Object lock = new Object();
    public static void main(String[] args) throws InterruptedException {
        Runnable r = () -> {
            for (int i = 0; i < 100000; i++) {
                synchronized(lock) {
                    sayac++;
                }
            }
        };
        Thread t1 = new Thread(r);
        Thread t2 = new Thread(r);
        t1.start(); t2.start();
        t1.join(); t2.join();
        System.out.println("Sonuç: " + sayac);
    }
}
```

---

## 3. Semaphore (Semafor) Nedir?
Semaphore, thread'ler arasında sinyal mekanizması sağlayan, negatif olmayan bir tamsayı değişkenidir. Mutex'ten farklı olarak, semaphore'da katı bir sahiplik yoktur ve herhangi bir thread sinyal gönderebilir veya bekleyebilir.

### Semaphore Çalışma Prensibi
```
Initial Value: 3

Thread A                    Semaphore                 Thread B
    │                         │                         │
    │─── Wait() ─────────────▶│  Value: 2               │
    │                         │                         │
    │◀─── Granted ────────────│                         │
    │                         │                         │
    │                         │◀─── Wait() ─────────────│
    │                         │  Value: 1               │
    │                         │                         │
    │                         │◀─── Granted ────────────│
    │                         │                         │
    │                         │◀─── Wait() ─────────────│
    │                         │  Value: 0               │
    │                         │                         │
    │                         │◀─── Blocked ────────────│
    │                         │                         │
    │─── Signal() ───────────▶│  Value: 1               │
    │                         │                         │
    │                         │◀─── Granted ────────────│
```

### Semaphore Türleri
```
┌─────────────────────────────────────────────────────────┐
│                  SEMAPHORE TYPES                        │
│                                                         │
│  ┌─────────────────┐        ┌─────────────────┐        │
│  │ Binary Semaphore│        │Counting Semaphore│        │
│  │                 │        │                 │        │
│  │ Values: 0, 1    │        │ Values: 0,1,2,3 │        │
│  │                 │        │                 │        │
│  │ Use: Mutex-like │        │ Use: Resource   │        │
│  │                 │        │   Management    │        │
│  └─────────────────┘        └─────────────────┘        │
└─────────────────────────────────────────────────────────┘
```

### Semaphore Kullanımına Örnekler
#### C (POSIX semaphore)
```c
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

sem_t semaphore;
int sayac = 0;

void* worker(void* arg) {
    sem_wait(&semaphore);  // Sinyal bekle
    sayac++;
    printf("Thread %ld: sayac = %d\n", (long)arg, sayac);
    sem_post(&semaphore);  // Sinyal gönder
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;
    sem_init(&semaphore, 0, 2);  // 2 thread aynı anda çalışabilir
    
    pthread_create(&t1, NULL, worker, (void*)1);
    pthread_create(&t2, NULL, worker, (void*)2);
    pthread_create(&t3, NULL, worker, (void*)3);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    
    sem_destroy(&semaphore);
    return 0;
}
```

#### Python (threading.Semaphore)
```python
import threading
import time

semaphore = threading.Semaphore(2)  # 2 thread aynı anda çalışabilir

def worker(worker_id):
    semaphore.acquire()
    print(f"Worker {worker_id} çalışıyor...")
    time.sleep(1)
    print(f"Worker {worker_id} bitti.")
    semaphore.release()

threads = []
for i in range(5):
    t = threading.Thread(target=worker, args=(i,))
    threads.append(t)
    t.start()

for t in threads:
    t.join()
```

---

## 4. Mutex vs Semaphore Karşılaştırması

| Özellik | Mutex | Semaphore |
|---------|-------|-----------|
| **Tür** | Bir nesne (object) | Bir tamsayı (integer) |
| **Mekanizma** | Kilitleme mekanizması | Sinyal mekanizması |
| **İşlemler** | Lock/Unlock | Wait/Signal |
| **Alt Türler** | Yok | Binary ve Counting |
| **Sahiplik** | Katı sahiplik (sadece kilitleyen açabilir) | Sahiplik yok (herhangi biri sinyal gönderebilir) |
| **Kullanım Amacı** | Karşılıklı dışlama | Thread koordinasyonu |

### Görsel Karşılaştırma
```
MUTEX:                    SEMAPHORE:
┌─────────────┐           ┌─────────────┐
│   LOCKED    │           │   Value: 3  │
│  Owner: T1  │           │             │
│             │           │ T1, T2, T3  │
│   WAITING   │           │  can enter  │
│    T2,T3    │           │             │
└─────────────┘           └─────────────┘
```

### Mutex'in Avantajları
- Yarış durumu oluşmaz, çünkü aynı anda sadece bir thread kritik bölümdedir
- Veri tutarlılığı sağlanır
- Basit kilitleme mekanizması

### Mutex'in Dezavantajları
- Thread uykuya geçerse veya yüksek öncelikli bir süreç tarafından kesilirse, diğer thread'ler beklemek zorunda kalır
- Aç kalma (starvation) sorunu olabilir
- Meşgul bekleme (busy waiting) CPU döngülerini boşa harcayabilir

### Semaphore'un Avantajları
- Birden fazla thread aynı anda kritik bölüme erişebilir
- Makine bağımsızdır
- Esnek kaynak yönetimi

### Semaphore'un Dezavantajları
- Öncelik tersine çevirme (priority inversion) sorunu
- Deadlock'a yol açabilir
- Büyük ölçekli sistemler için uygun değildir

### Gerçek Hayattan Örnekler

#### Mutex Örneği: Üretici-Tüketici Problemi
```c
// Tek bir buffer için mutex kullanımı
pthread_mutex_t buffer_mutex;
char buffer[4096];

void producer() {
    pthread_mutex_lock(&buffer_mutex);
    // Buffer'a veri yaz
    pthread_mutex_unlock(&buffer_mutex);
}

void consumer() {
    pthread_mutex_lock(&buffer_mutex);
    // Buffer'dan veri oku
    pthread_mutex_unlock(&buffer_mutex);
}
```

#### Semaphore Örneği: Üretici-Tüketici Problemi
```c
// Birden fazla buffer için semaphore kullanımı
sem_t empty_slots;  // Boş slot sayısı
sem_t filled_slots; // Dolu slot sayısı
char buffers[4][1024];  // 4 adet 1KB buffer

void producer() {
    sem_wait(&empty_slots);  // Boş slot bekle
    // Buffer'a veri yaz
    sem_post(&filled_slots); // Dolu slot sinyali gönder
}

void consumer() {
    sem_wait(&filled_slots); // Dolu slot bekle
    // Buffer'dan veri oku
    sem_post(&empty_slots);  // Boş slot sinyali gönder
}
```

---

## 5. Senkronizasyon Nedir?
Çoklu thread'lerin veya süreçlerin belirli bir düzen ve sırayla çalışmasını sağlama işlemidir. Amaç, veri tutarlılığını ve programın doğru çalışmasını garanti etmektir.

### Senkronizasyon Araçları
- Mutex (kilit)
- Semaphore (semafor)
- Condition Variable (koşul değişkeni)
- Barrier (bariyer)

### Kullanım Alanları
- Banka uygulamaları (aynı hesaba birden fazla işlem)
- Oyun motorları (fizik ve grafik işlemleri)
- Web sunucuları (aynı anda birden fazla istek)

### Yarış Durumu (Race Condition) Nedir?
İki veya daha fazla thread'in aynı anda bir veriyi değiştirmeye çalışması sonucu beklenmeyen sonuçların ortaya çıkmasıdır.

#### Race Condition Görselleştirmesi
```
Thread A:                    Thread B:
    │                           │
    │─── Read: count=5 ────────▶│
    │                           │
    │                           │◀─── Read: count=5 ─────
    │                           │
    │─── Add: 5+1=6 ───────────▶│
    │                           │
    │─── Write: count=6 ───────▶│
    │                           │
    │                           │◀─── Add: 5+1=6 ───────
    │                           │
    │                           │◀─── Write: count=6 ───
    │                           │
    │                           │
    │                    Result: count=6 (should be 7!)
```

#### Örnek (C)
```c
// Mutex olmadan
int sayac = 0;
void* arttir(void* arg) {
    for (int i = 0; i < 100000; i++) {
        sayac++;
    }
    return NULL;
}
```
Bu kodda, beklenen sonuç elde edilemeyebilir. Çünkü iki thread aynı anda sayacı değiştirebilir ve bazı artışlar kaybolur.

---

## 6. Farklı Dillerde Thread ve Mutex Kullanımı
### C/C++
- POSIX pthread (Unix/Linux)
- std::thread ve std::mutex (C++11 ve sonrası)

### Python
- `threading` modülü (gerçek paralellik için GIL kısıtlaması var)
- `multiprocessing` modülü (gerçek paralellik için)

### Java
- Thread sınıfı ve Runnable arayüzü
- synchronized anahtar kelimesi ve Lock sınıfları

### Go
- Goroutine (hafif thread)
- Channel ve Mutex

#### Go Örneği
```go
package main
import (
    "fmt"
    "sync"
)

func main() {
    var sayac int
    var wg sync.WaitGroup
    var mtx sync.Mutex
    for i := 0; i < 2; i++ {
        wg.Add(1)
        go func() {
            for j := 0; j < 100000; j++ {
                mtx.Lock()
                sayac++
                mtx.Unlock()
            }
            wg.Done()
        }()
    }
    wg.Wait()
    fmt.Println("Sonuç:", sayac)
}
```

---

## 7. Gerçek Hayatta Karşılaşılan Problemler ve Çözüm Yolları
### Deadlock (Kilitlenme)
İki veya daha fazla thread'in birbirini beklemesi sonucu programın durmasıdır.

#### Deadlock Görselleştirmesi
```
Thread A:                    Thread B:
    │                           │
    │─── Lock Resource 1 ──────▶│
    │                           │
    │                           │◀─── Lock Resource 2 ───
    │                           │
    │─── Wait for Resource 2 ──▶│
    │                           │
    │                           │◀─── Wait for Resource 1
    │                           │
    │                    DEADLOCK!
    │                           │
    │                    Both threads waiting forever
    │                           │
```

#### Deadlock Örneği (C)
```c
pthread_mutex_lock(&lock1);
pthread_mutex_lock(&lock2);
// ...
pthread_mutex_unlock(&lock2);
pthread_mutex_unlock(&lock1);
```
Eğer başka bir thread önce lock2'yi, sonra lock1'i alırsa, iki thread sonsuza kadar birbirini bekler.

**Çözüm:**
- Her zaman aynı sırayla kilitleri almak
- Zaman aşımı (timeout) kullanmak
- Deadlock algılama algoritmaları

### Starvation (Aç Kalma)
Bir thread'in sürekli kaynaklara erişememesi ve işini yapamaması.

#### Starvation Görselleştirmesi
```
High Priority Threads:        Low Priority Thread:
    │                           │
    │─── Resource Request ─────▶│
    │                           │
    │◀─── Resource Granted ─────│
    │                           │
    │─── Resource Request ─────▶│
    │                           │
    │◀─── Resource Granted ─────│
    │                           │
    │─── Resource Request ─────▶│
    │                           │
    │◀─── Resource Granted ─────│
    │                           │
    │                           │◀─── Resource Request ─
    │                           │
    │                           │◀─── Resource Denied ──
    │                           │
    │                           │◀─── Resource Request ─
    │                           │
    │                           │◀─── Resource Denied ──
    │                           │
    │                    STARVATION!
```

**Çözüm:**
- Adil kilitler (fair lock)
- Önceliklendirme

### Livelock
Thread'ler çalışıyor gibi görünür ama ilerleme kaydedemezler.

#### Livelock Görselleştirmesi
```
Thread A:                    Thread B:
    │                           │
    │─── Try to lock ──────────▶│
    │                           │
    │◀─── Lock failed ──────────│
    │                           │
    │─── Release lock ─────────▶│
    │                           │
    │                           │◀─── Try to lock ─────
    │                           │
    │                           │◀─── Lock failed ─────
    │                           │
    │                           │◀─── Release lock ────
    │                           │
    │                    LIVELOCK!
    │                    Both threads keep trying
    │                    but never succeed
```

**Çözüm:**
- Daha iyi algoritma tasarımı

### Priority Inversion (Öncelik Tersine Çevirme)
Düşük öncelikli bir thread'in yüksek öncelikli thread'i bekletmesi.

#### Priority Inversion Görselleştirmesi
```
High Priority Thread:         Medium Priority Thread:
    │                           │
    │─── Resource Request ─────▶│
    │                           │
    │◀─── Resource Denied ──────│
    │                           │
    │                    BLOCKED│
    │                           │
    │                           │◀─── Resource Request ─
    │                           │
    │                           │◀─── Resource Granted ─
    │                           │
    │                           │◀─── Working ──────────
    │                           │
    │                    High priority thread
    │                    blocked by medium priority!
```

**Çözüm:**
- Priority inheritance (öncelik mirası)
- Priority ceiling (öncelik tavanı)

---

## 8. Kaynaklar ve Daha Fazla Okuma
- [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)
- [The Little Book of Semaphores](https://greenteapress.com/wp/semaphores/)
- [GeeksforGeeks - Mutex vs Semaphore](https://www.geeksforgeeks.org/operating-systems/mutex-vs-semaphore/)
- [Python threading documentation](https://docs.python.org/3/library/threading.html)
- [Java Concurrency Tutorial](https://docs.oracle.com/javase/tutorial/essential/concurrency/)

---

## Son Söz
Çoklu iş parçacığı ve senkronizasyon, modern yazılım geliştirmede hem çok güçlü hem de dikkatli kullanılmazsa çok tehlikeli araçlardır. Mutex ve Semaphore arasındaki farkları anlamak, doğru senkronizasyon aracını seçmek için kritik öneme sahiptir. Bol bol pratik yap, örnekleri dene ve kodunu test etmeyi unutma!

Soruların olursa ya da daha fazla örnek istersen, çekinmeden ulaşabilirsin. Kolay gelsin! :) 