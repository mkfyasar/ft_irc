#!/bin/bash

# Varsayılan değerler
HOST="localhost"
PORT=4242
CONNECTIONS=10
MESSAGE="PING"

# Kullanım bilgisi
usage() {
    echo "Kullanım: $0 [-h host] [-p port] [-c connections] [-m message]"
    echo "  -h: Sunucu host adresi (varsayılan: localhost)"
    echo "  -p: Sunucu port numarası (varsayılan: 4242)"
    echo "  -c: Oluşturulacak bağlantı sayısı (varsayılan: 10)"
    echo "  -m: Gönderilecek mesaj (varsayılan: PING)"
    exit 1
}

# Parametre parsing
while getopts "h:p:c:m:" opt; do
    case ${opt} in
        h ) HOST=$OPTARG ;;
        p ) PORT=$OPTARG ;;
        c ) CONNECTIONS=$OPTARG ;;
        m ) MESSAGE=$OPTARG ;;
        ? ) usage ;;
    esac
done

# Bağlantıları oluştur ve mesaj gönder
create_connections() {
    for i in $(seq 1 $CONNECTIONS); do
        (echo "$MESSAGE"; sleep 1) | nc $HOST $PORT &
        echo "Bağlantı $i oluşturuldu ve '$MESSAGE' mesajı gönderildi."
    done
}

# Bağlantıları kapat
close_connections() {
    killall nc
    echo "Tüm nc bağlantıları kapatıldı."
}

# Ana menü
while true; do
    echo "
1. Bağlantıları oluştur ve mesaj gönder
2. Tüm bağlantıları kapat
3. Çıkış
"
    read -p "Seçiminiz (1-3): " choice
    case $choice in
        1 ) create_connections ;;
        2 ) close_connections ;;
        3 ) exit 0 ;;
        * ) echo "Geçersiz seçim. Lütfen tekrar deneyin." ;;
    esac
done