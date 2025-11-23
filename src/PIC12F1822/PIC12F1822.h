#ifndef TrueBypass_PIC12F1822_H
#define TrueBypass_PIC12F1822_H

#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

#define _XTAL_FREQ 16000000  //!< システム周波数 (16 MHz)

// PIC12F1822の設定ビット
#pragma config FOSC = INTOSC   // 内部オシレータ、CLKINはI/O
#pragma config WDTE = OFF      // ウォッチドッグタイマ無効
#pragma config PWRTE = ON      // 電源オンタイマ有効
#pragma config MCLRE = OFF     // MCLRピン無効（入力として使用）
#pragma config CP = ON         // プログラムメモリ保護有効
#pragma config CPD = ON        // データメモリ保護有効
#pragma config BOREN = ON      // 電圧低下リセット有効
#pragma config CLKOUTEN = OFF  // CLKOUT無効
#pragma config IESO = OFF      // 内部/外部切替無効
#pragma config FCMEN = OFF     // フェイルセーフクロック無効
#pragma config WRT = OFF       // フラッシュ書き込み保護無効
#pragma config PLLEN = OFF     // 4x PLL無効（INTOSC速度使用）
#pragma config STVREN = ON  // スタックオーバーフロー / アンダーフローでリセット
#pragma config BORV = HI  // 電圧低下リセット電圧高設定
#pragma config LVP = OFF  // 低電圧プログラミング無効

// ピン割り当て (PIC12F675と互換)
#define Relay LATAbits.LATA1         //!< Relayピン
#define PhotoCoupler LATAbits.LATA2  //!< PhotoCouplerピン
#define FootSwitch PORTAbits.RA3     //!< FootSwitchピン
#define LED LATAbits.LATA5           //!< LEDピン

// EEPROMから読み取るグローバル変数
extern bool initState;  // 初期状態 (電源オン時のON/OFF)
extern bool momentary;  // モーメンタリーモードフラグ
extern bool timing;     // タイミングモードフラグ

// 関数プロトタイプ
uint8_t eepRead(uint8_t adress);
void eepWrite(uint8_t adress, uint8_t myData);
void init_scan(void);
void init_interrupt(void);
void bypass_scan(void);
void bypass_interrupt(void);

#endif /* TrueBypass_PIC12F1822_H */
