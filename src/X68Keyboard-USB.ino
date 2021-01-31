#include <TimerTC3.h>
#include <U8g2lib.h>
#include "Keyboard_jp.h"

// READY信号出力ピン
const int READY_PIN = 8;

// LED 制御信号
const byte LED_CONTROL = B10000000;
const byte LED_KANA = B00000001;
const byte LED_ROMA = B00000010;
const byte LED_CODE = B00000100;
const byte LED_CAPS = B00001000;
const byte LED_INS  = B00011000;
const byte LED_HIRA = B00100000;
const byte LED_ZEN  = B01000000;

// LED 点灯情報
#define KEY_LED_NUMLOCK    1
#define KEY_LED_CAPSLOCK  2
#define KEY_LED_SCRLOCK   4
#define KEY_LED_COMPOSE   8
#define KEY_LED_KANA    0x10

const byte KEY_CONVERT_TABLE[] PROGMEM = {
  0x00,   // 00h
  0x29,   // 01h  [ESC]
  //0x35,   // 01h  [ESC] → [漢字]
  0x1E,   // 02h  [1]
  0x1F,   // 03h  [2]
  0x20,   // 04h  [3]
  0x21,   // 05h  [4]
  0x22,   // 06h  [5]
  0x23,   // 07h  [6]
  0x24,   // 08h  [7]
  0x25,   // 09h  [8]
  0x26,   // 0Ah  [9]
  0x27,   // 0Bh  [0]
  0x2D,   // 0Ch  [-]
  0x2E,   // 0Dh  [^]
  0x89,   // 0Eh  [\]
  0x2A,   // 0Fh  [BS]
  0x2B,   // 10h  [TAB]
  0x14,   // 11h  [Q]
  0x1A,   // 12h  [W]
  0x08,   // 13h  [E]
  0x15,   // 14h  [R]
  0x17,   // 15h  [T]
  0x1C,   // 16h  [Y]
  0x18,   // 17h  [U]
  0x0C,   // 18h  [I]
  0x12,   // 19h  [O]
  0x13,   // 1Ah  [P]
  0x2F,   // 1Bh  [@]
  0x30,   // 1Ch  [[]
  0x28,   // 1Dh  [RETURN]
  0x04,   // 1Eh  [A]
  0x16,   // 1Fh  [S]
  0x07,   // 20h  [D]
  0x09,   // 21h  [F]
  0x0A,   // 22h  [G]
  0x0B,   // 23h  [H]
  0x0D,   // 24h  [J]
  0x0E,   // 25h  [K]
  0x0F,   // 26h  [L]
  0x33,   // 27h  [;]
  0x34,   // 28h  [:]
  0x32,   // 29h  []]
  0x1D,   // 2Ah  [Z]
  0x1B,   // 2Bh  [X]
  0x06,   // 2Ch  [C]
  0x19,   // 2Dh  [V]
  0x05,   // 2Eh  [B]
  0x11,   // 2Fh  [N]
  0x10,   // 30h  [M]
  0x36,   // 31h  [,]
  0x37,   // 32h  [.]
  0x38,   // 33h  [/]
  0x87,   // 34h  [ろ]
  0x2C,   // 35h  [SPACE]
  0x4A,   // 36h  [HOME]
  0x4C,   // 37h  [DEL]
  0x4E,   // 38h  [ROLL UP]        -  [Page Doun]
  0x4B,   // 39h  [ROLL DOWN]      -  [Page Up]
  0x4D,   // 3Ah  [UNDO]           -  [END]
  0x50,   // 3Bh  [←]
  0x52,   // 3Ch  [↑]
  0x4F,   // 3Dh  [→]
  0x51,   // 3Eh  [↓]
  0x53,   // 3Fh  [CLR]            -  [NUM LOCK]
  0x54,   // 40h  [/] (10key)
  0x55,   // 41h  [*] (10key)
  0x56,   // 42h  [-] (10key)
  0x5F,   // 43h  [7] (10key)
  0x60,   // 44h  [8] (10key)
  0x61,   // 45h  [9] (10key)
  0x57,   // 46h  [+] (10key)
  0x5C,   // 47h  [4] (10key)
  0x5D,   // 48h  [5] (10key)
  0x5E,   // 49h  [6] (10key)
  0x67,   // 4Ah  [=] (10key)
  0x59,   // 4Bh  [1] (10key)
  0x5A,   // 4Ch  [2] (10key)
  0x5B,   // 4Dh  [3] (10key)
  0x58,   // 4Eh  [ENTER] (10key)
  0x62,   // 4Fh  [0] (10key)
  0x85,   // 50h  [,] (10key)
  //0x36,   // 50h  [,] (10key)
  0x63,   // 51h  [.] (10key)
  0x8B,   // 52h  [記号入力]       -  [無変換]
  0x47,   // 53h  [登録]           -  [ScroolLocK]
  0x00,   // 54h  [HELP]
  0xE3,   // 55h  [XF1]            -  [ミ田]
  0xE2,   // 56h  [XF2]            -  [Alt]
  0x8A,   // 57h  [XF3]            -  [変換]
  0xE6,   // 58h  [XF4]            -  [Alt]
  0x65,   // 59h  [XF5]            -  [(メニュー)]
  0x44,   // 5Ah  [かな]           -  [F11]
  0x45,   // 5Bh  [ローマ字]       -  [F12]
  0x00,   // 5Ch  [コード入力]
  0x39,   // 5Dh  [CAPS]
  0x49,   // 5Eh  [INS]
  0x88,   // 5Fh  [ひらがな]
  //0xE0,   // 5Fh  [ひらがな] → [CTRL]
  0x35,   // 60h  [全角]
  //0xE4,   // 60h  [全角] → Right Ctrl
  0x48,   // 61h  [BREAK]          -  [Pause]
  //0x29,   // 61h  [BREAK]          -  [Pause]→[ESC]
  0x46,   // 62h  [COPY]           -  [PrintScreen]
  0x3A,   // 63h  [F1]
  0x3B,   // 64h  [F2]
  0x3C,   // 65h  [F3]
  0x3D,   // 66h  [F4]
  0x3E,   // 67h  [F5]
  0x3F,   // 68h  [F6]
  0x40,   // 69h  [F7]
  0x41,   // 6Ah  [F8]
  0x42,   // 6Bh  [F9]
  0x43,   // 6Ch  [F10]
  0x00,   // 6Dh
  0x00,   // 6Eh
  0x00,   // 6Fh
  0xE1,   // 70h  [SHIFT]
  0xE0,   // 71h  [CTRL]
  //0x39,   // 71h  [CTRL] → [CAPS]
  0xE5,   // 72h  [OPT.1] - Right Shift
  0xE4,   // 73h  [OPT.2] - Right Ctrl
  //0x48,   // 73h  [OPT.2] - Right Ctrl → [Pause]
  0x00,   // 74h
  0x00,   // 75h
  0x00,   // 76h
  0x00,   // 77h
  0x00,   // 78h
  0x00,   // 79h
  0x00,   // 7Ah
  0x00,   // 7Bh
  0x00,   // 7Ch
  0x00,   // 7Dh
  0x00,   // 7Eh
  0x00,   // 7Fh
  0
};

// キー入力値
byte inputData = 0;
// キー入力ありフラグ
boolean keyAvailable = false;

// LED状態
boolean ledKana = false;
boolean ledRoma = false;
boolean ledCode = false;
boolean ledCaps = false;
boolean ledIns  = false;
boolean ledHira = false;
boolean ledZen  = false;

// OLED
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
int dispCnt = 0;

// USB RecvControl callback function
uint8_t led_status = 0;
uint8_t led_status_old = 0;
void led_func(uint8_t data) {
  led_status = data;
  Serial.println(String(led_status, HEX));
}

// LED制御値を生成
byte genLedValue() {
  return !(
           ledKana ? LED_KANA : 0 &
           ledRoma ? LED_ROMA : 0 &
           ledCode ? LED_KANA : 0 &
           ledCaps ? LED_CAPS : 0 &
           ledIns ? LED_INS : 0 &
           ledHira ? LED_HIRA : 0 &
           ledZen ? LED_ZEN : 0) &
         LED_CONTROL;
}

// タイマー割込み処理(キー入力値取得)
void TimerProc() {
  // キー入力処理済み
  if (!keyAvailable) {
    // キー入力あり
    if (Serial1.available() > 0)
    {
      // 入力値取得
      inputData = (byte)Serial1.read();
      // 未処理のキー入力あり
      keyAvailable = true;
      // 表示カウンタON
      dispCnt = 1;
    }
  }
  if (dispCnt > 0) dispCnt ++;
}

// 初期化
void setup() {

  // デバッグ用シリアル出力初期化
  Serial.begin(9600);

  // READY信号初期化
  pinMode( READY_PIN, OUTPUT );
  digitalWrite( READY_PIN, LOW );

  // キーボード通信ポート初期化
  Serial1.begin(2400, SERIAL_8N1);

  // USBキーボード初期化
  //HID().set_led_callback(led_func);
  Keyboard.begin();

  // キー入力監視タイマー初期化
  TimerTc3.initialize(2000); //2ms
  TimerTc3.attachInterrupt(TimerProc); //タイマ経過後に発生する割り込み関数を指定

  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_8x13_tf);
  u8g2.drawStr(0, 10, "X68000 Keyboard");
  u8g2.sendBuffer();

  led_status=KEY_LED_CAPSLOCK;
}

// メインループ
void loop() {
  char charBuf[17];
  if ( keyAvailable ) {
    if ((inputData & B10000000) == 0) {
      Keyboard.pressRaw(KEY_CONVERT_TABLE[inputData & B01111111]);
    } else {
      //Keyboard.writeRaw(0xF0);
      Keyboard.releaseRaw(KEY_CONVERT_TABLE[inputData & B01111111]);
    }
    keyAvailable = false;

    u8g2.setDrawColor(0);
    u8g2.drawBox(0, 12, 128, 10);
    u8g2.setDrawColor(1);
    u8g2.drawStr(0, 10, "X68000 Keyboard");
    String(inputData, HEX).toCharArray(charBuf, 16);
    u8g2.drawStr(0, 21, charBuf);
    String(KEY_CONVERT_TABLE[inputData & B01111111], HEX).toCharArray(charBuf, 16);
    u8g2.drawStr(24, 21, charBuf);
    u8g2.sendBuffer();
  } else {
    // 焼き付き防止のために30秒で表示を消す
    if (dispCnt > 30000) {
      dispCnt = 0;
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 128, 32);
      u8g2.sendBuffer();
    }
  }
    if (led_status != led_status_old) {
      // LEDマップ変換
      ledCode = !!(led_status & KEY_LED_NUMLOCK);
      ledCaps = !!(led_status & KEY_LED_CAPSLOCK);
      ledRoma = !!(led_status & KEY_LED_SCRLOCK);
      ledKana = !!(led_status & KEY_LED_COMPOSE);
      ledHira = !!(led_status & KEY_LED_KANA);
      // LED点灯/消灯指示
      Serial1.write(genLedValue());
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 22, 128, 10);
      u8g2.setDrawColor(1);
      String(led_status, HEX).toCharArray(charBuf, 16);
      u8g2.drawStr(0, 31, charBuf);
      String(genLedValue(), HEX).toCharArray(charBuf, 16);
      u8g2.drawStr(24, 31, charBuf);
      u8g2.sendBuffer();
      led_status_old = led_status;
    }
}
