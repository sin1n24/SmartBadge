#include <Arduino.h>
#include <bluefruit.h>
#include <Wire.h>

// メーカーID（0x004CはApple, 0x0059はNordic）
#define MANUFACTURER_ID   0x004C

// ビーコンのUUID（識別子）
uint8_t beaconUuid[] = {
  0xE2, 0xC5, 0x6D, 0xB5, 0xDF, 0xFB, 0x48, 0xD2,
  0xB0, 0x60, 0xD0, 0xF5, 0xA7, 0x10, 0x96, 0xE0,
};

// ビーコンオブジェクトの作成 (UUID, Major, Minor, RSSI@1m)
BLEBeacon beacon(beaconUuid, 0x0001, 0x0000, -54);

void setup() {
  Serial.begin(115200);
  
  // Bluefruitの初期化
  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // 送信出力
  Bluefruit.setName("XIAO Beacon"); // デバイス名

  // ビーコンにメーカーIDをセット
  beacon.setManufacturer(MANUFACTURER_ID);

  // --- アドバタイズ（ブロードキャスト）の設定 ---
  
  // 1. ビーコンデータをアドバタイズパケットにセット
  // これにより、メインのパケット(31バイト)の大部分がビーコンデータで埋まります
  Bluefruit.Advertising.setBeacon(beacon);

  // 2. デバイス名は「スキャンレスポンス」に逃がす
  // ★ここが重要です。メインパケットがいっぱいなので、名前は2つ目のパケットに入れます。
  // 出典: [6][7] "Since there is no room for 'Name' in Advertising packet..."
  Bluefruit.ScanResponse.addName();

  // 3. アドバタイズの開始設定
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(160, 160);    // 100ms間隔
  Bluefruit.Advertising.setFastTimeout(30);       // ファストモードの時間
  
  // 0を指定すると、タイムアウトせず永遠にブロードキャストし続けます
  Bluefruit.Advertising.start(0); 
  
  Serial.println("Broadcasting beacon...");
}

void loop() {
  // ブロードキャストは裏で自動的に行われるため、loopは空で構いません
  // 省電力のためにサスペンドなどを入れることも可能です [8]
  delay(1000);
}