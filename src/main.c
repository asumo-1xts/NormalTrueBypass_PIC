/**
 * @file        main.c
 * @author      aSumo
 * @version     2.0
 * @date        2025-05-04
 * @copyright   (c) 2024 aSumo
 * @brief       Program for Normal True Bypass with PIC microcontroller.
 * @details
 * header.hにてincludeするファイルを変更するだけで対象のデバイスを切り替えられます。
 */

#include "header.h"

/**
 * @fn      void main(void)
 * @brief   サボり上司
 * @details
 * スキャン式か割り込み式を選択できます。この選択はmain関数内で統一しなければなりません。
 */
int main(void) {
  // init_scan();
  // bypass_scan();
  // or
  init_interrupt();
  bypass_interrupt();

  return 0;
}
