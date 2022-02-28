# 384OS

uchan-nosさんのMikanOSを主に参考にしたRustとCでできたx86_64のシンプルなOSです。

UEFIに対応しています。

## 私について

プログラミングに興味があります。

中学2年生です。

私は自分で考えたコードを実装するのがプログラムを書く上での夢です。

主に使っている言語はC言語です。(初心者なので教えてください!)

また、Rust,Go,VHDLなどに興味があります。

### ディレクトリの構成

    384Boot    ← MikanOSを主に参考にしたBootLoader 

    384OS_test ← QEMUで384OSをテストできます。

    capsella_lib ← Rustでできたライブラリです。

    kernel       ← 384OSのカーネルです。

    Library      ← NewLibというライブラリが入っています。

    objs         ← コンパイルされたオブジェクトファイルが入ります。

    scripts      ← スクリプトが入っています。

### OSのビルド、デバッグ方法

#### ブートローダー

ブートローダーはMikanOSのブートローダーと同じようにビルドできます。

ビルドにはEDK2が必要です。

#### カーネル

カーネルは

    make

でビルドでき

    make run

でビルドしてQEMUで実行することができます。

ビルドにはクロスコンパイラが必要です。

#### クロスコンパイラ (OSのビルドとは違いますがここで説明します。)

クロスコンパイラのビルドは

    make CCInstall

ですることができます。

また、クロスコンパイラをビルドする際のConfigは __scripts/CCDownload.sh__ で設定することができます。

Configの設定の仕方を以下に示します。

__PREFIX__ ← ビルドしたクロスコンパイラが置かれる位置

__TARGET__ ← ビルドするクロスコンパイラのターゲット

__THREADOFNUMBER__ ← ビルドするときに使うコアの数

### QEMUでOSを実行する際の注意点

__384OS_test/EFI/BOOT/BOOTX64.EFI__ を削除してしまうとブートローダーを再度ビルドする必要があります。

__384OS_test/EFI/BOOT/font.psf__ を削除してしまうとブートローダーから

    fontfile not found

というエラーが出ます。フォントについて詳しくは __フォントについて__ をご覧ください。

### ライセンス

384OSのライセンスはMITLICENSEです。

また、MikanOSはApache License 2.0なのでLICNSE-2.0.txtにライセンスが書かれています。

MikanOSのソースコードを完全に写経したソースのファイルには著作権が書かれています。

### 参考

#### ブートローダー

[ブートローダーの主な機能](https://github.com/uchan-nos/mikanos)

[psfファイルの読み取り](https://github.com/Absurdponcho/PonchoOS)

[UEFIの仕様など1](https://uefi.org/specifications)

[UEFIの仕様など2](http://yuma.ohgami.jp/UEFI-Bare-Metal-Programming/)

#### カーネル

[カーネルの主な機能](https://github.com/uchan-nos/mikanos)

[カーネルのグラフィック](https://github.com/Absurdponcho/PonchoOS)

#### クロスコンパイラ

[クロスコンパイラのビルド](https://qiita.com/saru_cololado/items/69e5da4c8d030f44a3a3)

### 参考の"ブートローダーの主な機能"について

ブートローダーの中のELFファイルの読み取り、セグメントマッピングなど一部MikanOSの関数を写経させていただきました。

### 参考の"カーネルの主な機能"について

カーネルの中のアセンブラの関数はMikanOSを写経させていただいています。

IDTやGDT,メモリ管理などもMikanOSを大いに参考にしています。

ゼロから始めるOS入門が非常に参考になりました。

著者の内田公太さん、本当にありがとうございました！

### 質問やアドバイスなど

質問やアドバイスなどはこのリポジトリの __Issues__ でお願いします。

たくさんの質問やアドバイスを頂きたいです!

### 今後の展望

PCIドライバ、AHCIドライバ等を書いていきたいと思っています。

また、ファイルシステムやシステムコールの実装を進めていきたいと思います。

### OSを実装した感想

まだまだOSと言える機能は実装しきれていませんが、プログラムを始める時から夢だった自作OS

を少しでも実装することができ、非常に面白かったです。

今後はコンパイラやOSの理解をさらに進めたいと思います。

### その他

inb,outbという関数は完全に写経しましたが、写経した元のサイトが見れなくなっていたためその他に書かせていただきました。

## LICENSE

The source code is licensed MIT.

NewLibのライセンスについては __Library/LICENSE.newlib__ をご参照ください。