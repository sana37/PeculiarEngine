# PeculiarEngine

Qtでopenglを使って自作の物理演算シミュレータを開発しています。

（※現在開発途中です。不完全な部分が多々見られることと思います）

## 概要

三次元空間上で、物体が力を受けたときにどういった挙動を示すのかシミュレートします。

物体同士の衝突（現在実装されているのは非弾性衝突）や、重力による等加速度運動を実験できます。

（接触している物体同士に摩擦力が働いているように見えるかもしれませんが、
これはシミュレーションを行いやすくするために一時的な実装を行った結果であり、
古典力学に基づく正しい計算式によるものではありません。）

## 詳細

このシミュレータでは、次の処理をループで行っています。

 * 各物体にかかる静的な力を調べ、物体を加速・減速させる
 * 物体がそれぞれ衝突していないか、より厳密には次のループで衝突しないか判定し、衝突する場合は撃力を計算する
 * 各物体を、それぞれの速度のぶんだけ移動させる
 * 描画処理

また、各物体の頂点座標や色といったパラメータは、`res/`ディレクトリにあるjsonファイルから読みこんでいます。

### version

Qt 5.11


## 使い方

```
qmake
```

を実行します。続けて、

```
make
```

を実行すれば、`target/PeculiarEngine`が生成されるので、これを実行すると動きます。




#### ソースコードを追加・削除したとき

```
qmake -project
```

を実行

自動生成された`PeculiarEngine.pro`に次の内容を挿入

```
DESTDIR = target
OBJECTS_DIR = generated
MOC_DIR = generated
QT += widgets
LIBS += -lGLU
```

また、`INCLUDEPATH`に`src src/util src/object src/event src/force`を追加
