# ft_irc Projesi

Bu proje, C++ 98 standardında geliştirilmiş basit bir IRC (Internet Relay Chat) sunucusudur.

## Proje Yapısı

Projemiz aşağıdaki ana bileşenlerden oluşmaktadır:

1. **Server**: Ana sunucu sınıfı, bağlantıları yönetir.
2. **Client**: Bağlanan kullanıcıları temsil eder.
3. **Channel**: Sohbet kanallarını yönetir.
4. **Message**: IRC mesajlarını işler.
5. **Command**: Kullanıcı komutlarını yönetir.

## Socket Nedir ?

- Socket, iki bilgisayar arasinda veri alisverisi yapmak icin kullanilan bir uc noktadir. 
- Socketler ag uzerinden veri gondermek ve almak icin kullanilir.
- TCP veya UDP protokolleriyle kullanilir.
    
### Socket Fonksiyonu
```cpp
int socket(int domain, int type, int protocol)
```
### Parametreler
1. `domain`: İletişim alanını belirtir. Örneğin:
   - `AF_INET`: IPv4 için
   - `AF_INET6`: IPv6 için
   - `AF_UNIX`: Unix domain soketleri için

2. `type`: Socket türünü belirtir. Örneğin:
   - `SOCK_STREAM`: TCP için
   - `SOCK_DGRAM`: UDP için
   - `SOCK_RAW`: Ham ağ protokolleri için

3. `protocol`: Kullanılacak protokolü belirtir. Genellikle 0 olarak ayarlanır, bu durumda sistem `domain` ve `type` parametrelerine göre uygun protokolü seçer.

### Socket Olusturma

```cpp
#include <sys/socket.h> // socket programlama icin temel yapilar -> socket() - bind() -listen()...
#include <netinet/in.h> // Internet protokelleri icin ozel veri yapilari ve sabitler -> htons()...

int sockFd = socket(AF_INET, SOCK_STREAM, 0);
// Socket basarili olursa olusturlan soketin fdsi doner
// Basarisizlikda -1 doner

```

- `Server(int port, std::string password)`: Sunucuyu belirtilen port ve şifre ile başlatır.
- `run()`: Sunucuyu çalıştırır, yeni bağlantıları kabul eder ve mevcut istemcileri yönetir.
- `acceptNewConnections()`: Yeni istemci bağlantılarını kabul eder.
- `handleClientMessages()`: İstemcilerden gelen mesajları işler.


### Bind Fonksiyonu
- Bir soket belirli bir ag adresi(ip) ve port numarasina baglar.
- Sunucu soketleri icin hangi adresten ve porttan gelen baglantilari dinleyecegini belirtir.
- Istemciler bu adres ve port uzerinden sunucuya baglanabilir.
- Bind cagrisi genellikle sunucu tarafindan kullanilir.
- Ayni port numarasini birden fazla soket kullanmaya calisirsaniz hata alirsiniz.
  
```cpp
int bind(int sockFd, const struct sockaddr *addr, socklen_t addrlen);
```
### Parametreler
1. `sockFd`: Baglanacak soketin dosya numarasi
2. `addr`: Baglanilacak adres bilgisini iceren yapi
3. `addrlen`:addr yapisinin boyutu

### Bind Fonskiyonu Ornek Kullanimi
```cpp
struct sockaddr_in server_addr;
memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
server_addr.sin_port = htons(6667);

if(bind(sockFd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
    perror("bind failed!");
    exit(EXIT_FAILURE);
}
```

### Listen Fonksiyonu
- Soketi dinleme moduna gecirir.
- Gelen baglanti isteklerini kabul etmeye hazir konuma getirir.
- Baglanti isteklerini bir kuyrukta toplar.
- Basarili olursa 0 doner basarisizda -1 doner.
  
```cpp
    int listen(int socketFd, int backLog);
```
### Parametreler
1. `socketFd`: Dinlemeye gecirelecek olan socketin fdsi.
2. `backLog`: Bekleyen baglantilar kuyrugunun maksimum uzunlugu.

### Listen Fonksiyonu Ornek Kullanimi
```cpp
    if(listen(socketFd, 10) < 0){
        perror("Listen Failed!");
        exit(EXIT_FAILURE);
    }
```
### Irc icin Listen Fonksiyonun Onemi
- Listen cagrisi sunucunuzun istemilerden gelen baglanti isteklerini kabul etmeye hazir oldugunu belirtir.
- 10 backlog degeri ile sunucunuzun ayni anda 10 istemcinin baglanti istegini kuyruga alabilir.
- Bu yogun trafikli irc sunucunuzun birden fazla istemcinin eszamanli olarak baglanabilmesini saglar

















### Accept Fonksiyonu
-   Bir sunucu soketinde bekleyen baglanti istegini kabul etmek icin kullanilir.
-   Bu fonksiyon irc sunucumuzun istemcilerle baglanti kurmasinin temel tasidir. Her basarli accept() cagrisi, yeni bir irc istemcisinin sunucumuza baglandigini anlamina gelir.
  
```cpp
    int accept(int socketFd, struct sockaddr *addr, socklen_t *addrlen);
```
### Parametreler
1. `socketFd`: Dinleyen sunucu socketin fdsi.
2. `addr`: Baglanan istemcinin adres bilgisini alacak yapi.
3. `addrlen`: addr yapisinin boyutunu gosteren pointer.

    #### -  Accept Fonksiyonu Ornek Kullanimi
    ```cpp
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int clientFd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_len);
    ```
