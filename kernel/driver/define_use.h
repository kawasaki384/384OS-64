/*
このヘッダファイルはドライバの条件コンパイルの設定を行います。
*/

#include "../define_true_false.h"
/*↑definetrue = 0x01
   definefalse = 0x00*/

/*シリアルポートドライバの設定。*/
#define USE_SERIAL_FUNC definetrue //シリアルポートの関数を全て使う。

#if USE_SERIAL_FUNC == definetrue
#define SERIAL_PORT_CODE_RS232_C definetrue         //シリアルポート関連の関数を使用するか。
#define SERIALPORT_SOURCE_USE_GRAPHIC_FN definefalse //シリアルポートの関数をグラフィックの関数で使用するか
#endif                                              /*USE_SERIAL_FUNC*/
/*end シリアルポートドライバの設定。*/
