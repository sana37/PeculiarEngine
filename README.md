# PeculiarEngine

Qtでopenglを使って自作の物理エンジンを開発しています。

### version

Qt 5.11


### 使い方

```
qmake
```

を実行します。続けて、

```
make
```

を実行すれば、`target/PeculiarEngine`が生成されるので、これを実行すると動きます。




#### ソースコードが追加・削除されたとき

```
qmake -project
```

を実行

自動生成された`PeculiarEngine.pro`に次の内容を挿入

```
DESTDIR = target
OBJECTS_DIR = generated
MOC_DIR = generated
QT += widgets opengl
LIBS += -lGLU
```

また、`INCLUDEPATH`に`src src/util src/object src/event src/force`を追加
