#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

// #include "./PIC12F6XX/PIC12F6XX.h"
#include "./PIC12F1822/PIC12F1822.h"

/**
 * @fn      void waitChattering(void)
 * @brief   チャタリングを待つ関数
 * @details 長くとも数十 [ms] らしい
 */
static inline void waitChattering() { __delay_ms(5); }

/**
 * @fn      void waitPhotoCoupler(void)
 * @brief   フォトカプラの応答を待つ関数
 * @details TLP225Aなら上昇時間2 [ms] / 下降時間2 [ms]
 */
static inline void waitPhotoCoupler() { __delay_ms(3); }

/**
 * @fn      void waitRelay(void)
 * @brief   リレーの応答を待つ関数
 * @details AZ850なら動作時間2 [ms] / 復帰時間1 [ms]
 */
static inline void waitRelay() { __delay_ms(3); }

// その他EEPROM関連
#define eepAdd_initState 0x00  //!< EEPROM内のアドレスその1
#define eepAdd_momentary 0x02  //!< EEPROM内のアドレスその2
#define eepAdd_timing 0x04     //!< EEPROM内のアドレスその3

#define interval_short 50  //!< LED用のインターバル [ms]
#define interval_long 150  //!< LED用のインターバル [ms]

// プロトタイプ宣言
void disAnalog(void);
void setInitState(void);
void flashLED(uint8_t interval, uint8_t times);
void insideISR(void);
void bypass_scan(void);
void bypass_interrupt(void);
