#ifndef TrueBypass_PIC12F6XX_H
#define TrueBypass_PIC12F6XX_H

#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

#define _XTAL_FREQ 4000000  //!< クロック周波数 4MHz

#pragma config FOSC = INTRCIO  // 内部クロック          使用
#pragma config WDTE = OFF      // Watch-Dogタイマー     不使用
#pragma config PWRTE = ON      // パワーアップタイマー  使用
#pragma config MCLRE = OFF     // リセットピン          不使用
#pragma config BOREN = ON      // Brown-Out検出         使用
#pragma config CP = ON         // プログラム保護        使用
#pragma config CPD = ON        // データ保護            使用

// ピンアウト
#define Relay GP1         //!< Relayピン
#define PhotoCoupler GP2  //!< PhotoCouplerピン
#define FootSwitch GP3    //!< FootSwitchピン
#define LED GP5           //!< LEDピン

// 外部へ共有する変数
// EEPROMから読み込むグローバル変数
extern bool initState;  // ペダルの初期状態
extern bool momentary;  // モーメンタリ動作の是非
extern bool timing;     // オルタネイト動作時、いつ反転するか

// 関数のプロトタイプ宣言
uint8_t eepRead(uint8_t adress);
void eepWrite(uint8_t adress, uint8_t myData);
void init_scan(void);
void init_interrupt(void);
void bypass_scan(void);
void bypass_interrupt(void);

#endif /* PIC12F6XX_H */
