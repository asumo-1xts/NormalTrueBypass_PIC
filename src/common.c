/**
 * @file        common.c
 * @author      aSumo
 * @version     1.0
 * @date        2025-05-12
 * @copyright   (c) 2025 aSumo
 * @brief       PICマイコンによらず共通の関数を定義するファイル
 */

#include "header.h"

//! エフェクトON/OFFのためのマクロ（割り込み中に使用するため関数化しない）
#define TURN()                                            \
  do {                                                    \
    PhotoCoupler = true;  /* energize optocoupler */      \
    waitPhotoCoupler();   /* short delay */               \
    LED = !LED;           /* toggle LED state */          \
    Relay = !Relay;       /* toggle Relay state */        \
    waitRelay();          /* wait for relay to actuate */ \
    PhotoCoupler = false; /* de-energize optocoupler */   \
  } while (false)

/**
 * @fn      void flashLED(uint8_t interval, uint8_t times)
 * @param   types   点滅の間隔
 * @param   times   点滅の回数
 * @brief   LEDを点滅させる関数
 */
void flashLED(uint8_t interval, uint8_t times) {
  for (uint8_t i = 0; i < times; i++) {
    switch (interval) {
      case 1:
        LED = true;
        __delay_ms(interval_short);
        LED = false;
        __delay_ms(interval_short);
        break;

      case 2:
        LED = true;
        __delay_ms(interval_long);
        LED = false;
        __delay_ms(interval_long);
        break;

      default:
        break;
    }
  }
}

/**
 * @fn      void disAnalog(void)
 * @brief   アナログ機能を無効化する関数
 */
void disAnalog(void) {
#if defined(ANSEL)
  ANSEL = 0x00;
#endif
#if defined(ANSELA)
  ANSELA = 0x00;
#endif
#if defined(ADCON0)
  ADCON0 = 0x00;
#endif
}

/**
 * @fn     void setInitState(void)
 * @brief  EEPROMの値を読み書きして初期状態を設定する関数
 */
void setInitState(void) {
  int flag_i = 0;  // 踏まれている時間をカウントする変数

  // EEPROMからuint8_t型をbool型に変換して読み込む（0:false / 1:true）
  initState = (bool)(eepRead(eepAdd_initState));
  momentary = (bool)(eepRead(eepAdd_momentary));
  timing = (bool)(eepRead(eepAdd_timing));

  // 踏みながら電源を入れてモード切り替え
  if (!FootSwitch) {
    waitChattering();

    if (!FootSwitch) {  // surely pressed
      flashLED(1, 3);

      // Alternate or Momentary
      for (flag_i = 0; flag_i < 300; flag_i++) {  // flag_i*10秒間待ってやる
        if (!FootSwitch) {
          __delay_ms(10);          // 踏まれている間は待ち続ける
        } else {                   // 離されたら
          momentary = !momentary;  // 論理反転
          eepWrite(eepAdd_momentary, (uint8_t)momentary);  // EEPROMに書き込む
          break;  // forループごと抜ける
        }
      }

      // Effect ON/OFF when Power-ON
      if (flag_i > 275) {
        // initStateの設定に入る
        initState = !initState;  // 論理反転
        eepWrite(eepAdd_initState,
                 (uint8_t)initState);  // EEPROMに書き込む
        flashLED(2, 3);                // initState設定完了！
        while (!FootSwitch) {
          ;  // 離されるまでキープ
        };
      }
    }

    waitChattering();
  }
}

/**
 * @fn      void bypass_scan(void)
 * @brief   バイパスの挙動を定義する関数（スキャン式）
 * @details
 * 駆動中にmomentaryなどのモード変数を変更したいときは、適宜while文を抜ける
 */
void bypass_scan(void) {
  bool flag_momentary = true;  // 反転フラグ（モーメンタリ用）
  bool flag_alternate = true;  // 反転フラグ（オルタネイト用）

  if (momentary) {               // モーメンタリ動作
    flag_momentary = initState;  // まず常識が更新される
    while (true) {
      if (FootSwitch == flag_momentary) {    // 踏まれているか？
        waitChattering();                    // チャタリングを待つ
        if (FootSwitch == flag_momentary) {  // 本当に踏まれているか？
          TURN();  // ひっくり返す（初期状態がONの指定なら自動でひっくり返る）
          flag_momentary = !flag_momentary;  // 反転フラグを反転
        }
      }
    }
  } else {            // オルタネイト動作
    if (initState) {  // 初期状態がONの指定なら
      TURN();         // 手動でひっくり返しておく
    }
    while (true) {
      if (!FootSwitch) {         // 踏まれているか？
        waitChattering();        // チャタリングを待つ
        if (!FootSwitch) {       // 本当に踏まれているか？
          if (flag_alternate) {  // 反転フラグが立っているか？
            if (timing) {  // タイミングの指定によっては離されるまで待ってから
              while (!FootSwitch) {
                ;  // 離されるまでキープ
              };
            }
            TURN();                  // ひっくり返す
            flag_alternate = false;  // 反転フラグをクリア
          }
        }
      } else {                    // 離されているか？
        waitChattering();         // チャタリングを待つ
        if (FootSwitch) {         // 本当に離されているか？
          flag_alternate = true;  // 反転フラグを立てる
        }
      }
    }
  }
}

/**
 * @fn      void bypass_interrupt(void)
 * @brief   バイパスの挙動を定義する関数（割り込み式）
 * @details 基本的には割り込み待ち
 */
void bypass_interrupt(void) {
  if (initState) {  // 初期状態がONの指定なら
    TURN();         // 手動でひっくり返しておく
  }

  ei();  // ここにきて割り込みを許可
  while (true) {
    ;  // ひたすら割り込み待ち…
  }
}

/**
 * @fn      void insideISR(void)
 * @brief   割り込み中に実行される関数
 */
void insideISR(void) {
  waitChattering();

  if (!FootSwitch) {  // 踏まれているか？
    if (momentary) {  // モーメンタリ動作ならば
      TURN();         // ひっくり返す
      while (!FootSwitch) {
        ;  // 離されるまでキープ
      }
      TURN();        // 離されたらもう一度ひっくり返す
    } else {         // オルタネイト動作ならば
      if (timing) {  // タイミングの指定によっては離されるまで待ってから
        while (!FootSwitch) {
          ;  // 離されるまでキープ
        }
      }
      TURN();  // ひっくり返す
    }
  }
}