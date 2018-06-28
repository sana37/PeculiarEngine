# PeculiarEngine

Qtでopenglを使って自作の物理エンジンを開発しています。

### version

Qt 5.3


### 使い方

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

続けて、

```
qmake
```

を実行

自動生成された`Makefile`について

```
$(TARGET):  $(OBJECTS)
```

を

```
$(TARGET):  $(OBJECTS_DIR) $(OBJECTS)
```

に修正し、さらに、適当な位置に

```
$(OBJECTS_DIR):
       @test -d $@ || mkdir -p $@
```

を挿入します。

あとは、

```
make
```

を実行してコンパイルすれば、`target/PeculiarEngine`が生成されるので、これを実行すると動きます。
