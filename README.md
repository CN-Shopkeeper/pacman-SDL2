# pacman-SDL2

> :warning: ~~**本游戏的全部售卖收入将用于充值原神！**~~

基于 SDL2 的吃豆人游戏

[主页](https://github.com/CN-Shopkeeper/pacman-SDL2)，如果你喜欢这个项目，请给一个 star。

[点击跳转](https://cn-shopkeeper.github.io/Projects/Pacman-SDL2/Pacman.html)

如果上述链接无法访问或加载，可以尝试[这个链接](http://146.56.248.15/games/pacman/)。注意，这个链接可能会失效，排行榜后端服务器也会随之失效。

如果不通过 Emscripten 编译，只是在本地运行，则不会受到影响。本地文件是 Stand Alone 的。

## 一些功能

1. 墙体碰撞监测

2. 移动意图

3. 随机创造地图(R)

4. 四种 Ghost 的 AI。7 秒分散模式，20 秒追逐模式

5. 开发者模式(G): 显示 Ghost 路径，`H`更改 Ghost 模式

6. 暂停游戏(P)

7. 排行榜

## 更新日志

- 2023 年 8 月 20 日

  - v1.2。感谢诸多内测人员的反馈与建议，该项目才能有了更多改进之处！
  - 增加了排行榜。当获胜时会通过消息框引导录入成绩。输入的用户 ID 最多为 10 个 ASCII 字符(因为 shopkeeper 就是 10 个字符)。
  - 排行榜存储在服务器本地文件，不具备 ACID 特性。如有错误产生，请见谅！
  - Ghost 的难度调整：speed 3 -> 5

- 2023 年 8 月 18 日

  - 增加了彩蛋：
    > > 在神秘的迷宫中，有四处遥远的角落，正午的太阳啊，能消灭世间一切邪魅，却为何不能将你的光芒挥洒到这些阴暗的角落，让其成为了邪魅躲避阳光猛烈的避难之所。
    > >
    > > 勇士啊，若你被困于那狭小的角落，当你半边的视野被阻隔，不要放弃希望，拼尽全力挣脱牢笼，伟大的先贤将会显现，为你降下保护结界，助你冲出重围！

- 2023 年 8 月 18 日

  - v1.1，极有可能是最终版本
  - 增加了连杀奖励
  - Pacman 现在不可以去 Ghost house 刷分

- 2023 年 8 月 17 日

  - 地图中有能量豆，吃了之后 Ghost 会进入 Frightened 模式
  - Frightened 模式下 Ghost 会变成蓝色，移动速度减慢
  - Frightened 模式下 Ghost 可以被吃掉（一个两百分）
  - 能量豆持续时间为 15 秒，最后三秒时 Ghost 会闪烁
  - 增加了 Debug 模式下的文字输出
  - 现在可以在 Frightened 模式时进鬼屋刷分。。。。。

- 2023 年 8 月 16 日
  - 完成了 v1.0 版本，有了正常的胜利、失败判定
  - 增加了计时器，根据时间改变 ghost 的行为模式
  - 增加了 inky 和 clyde 的出动条件（默认开局是不出鬼屋的）
  - 为 pacman 添加了动画

## 算法依据

[地图生成](https://shaunlebron.github.io/pacman-mazegen/) (简化了的)

[游戏机制](https://gameinternals.com/understanding-pac-man-ghost-behavior)

## TODO

- 随着游戏进程出现的加分水果
- ~~Pacman 不应该可以进入鬼屋~~
- ~~Frightened 模式下对 Ghost 应该有连杀奖励~~
- 触发彩蛋后无敌三秒
- ~~排行榜~~
- ~~Ghost 速度调整~~
- 剩余时间奖励
- 三条命

## WASM

```shell
emcmake cmake -S . -B wasm-build
```

```shell
cmake --build wasm-build
```
