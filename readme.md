# ICFPC2018 Girigiri

## Problem
[https://icfpcontest2018.github.io/full/task-description.html](https://icfpcontest2018.github.io/full/task-description.html)

# 理想
## Assemble Problem
担当: @xuzijian629

入力例は`FA122_tgt.mdl`を除いて連結。
以下のアルゴリズムでLowを保ったまま構築する。

1. 最初にノードを最上階まで最短で動かし、"柱"を作る。柱を作る過程で、各階にノードを１つずつ分裂させておく。親ノードは(0, R - 1, 0)に置いておく。

2. 各階のノードは、階内のブロックのみを塗る。階内のブロックを（非連結なものも含めて）すべて塗るような、最短路を計算し、
塗っていく。

3. 各階の処理を終えると、ロボットは(0, \*, 0)まで戻り、隅を辿ってまだブロックが配置されていない階の柱へ向かう。

4. すべての階が塗られるまでブロックが上昇していく。終了したブロックは(0, R - 1, 0)にマージしていく。

5. ノードが全部集まったらノードを(0, 0, 0)に戻す。

ここから余分なブロックを消していく。なおこの作業はReassemble Problemでも使えるのであえて分離して実装する。

6. もう一度下の階から順にノードを配置していく。その階の本来不要だったマスを消していく。

7. 塗ったときと同様に、各階の作業が終わると(0, \*, 0)まで戻って上の階に移動していく。

8. 作業が終了したロボットを(0, R - 1, 0)にマージしていく。

9. 集まったロボットを(0, 0, 0)に戻して作業終了。

# 現実
時間がなくて書き直しました。

## Assemble Problem
低い段から順に塗っていく。必要なときだけレベルをHighにする。必要かどうかの判定は、そのブロックを塗ったときに、全ブロックが床と結ばれているかをチェックすればよく、Union Findによって実質定数時間で可能。

各段では、塗るべきブロックを含むような最小の長方形を考え、その中を順に塗っていく。

## Disassemble Problem
考え方はAssemble Problemと同じで、今度は上から消していく。必要なときだけレベルをHighにする。必要かどうかの判定は動的に行なうと時間がかかってしまうため、どのような判定があるかを事前に記録し、その後前処理をしてすべての判定について回答を得ておく。

判定は逆順に見ると、Assemble Problemのときの判定と同様なので、逆順に解いていくと各クエリを定数時間で回答できる。

判定の回答が得られた状態で、再び上から消していく。

## Reassemble Problem
Disassemble後Assemble

# For International Readers
## Assemble Problem
We used only one robot. Robot swept each floor from bottom to fill voxels.
It enabled harmonics only when it was necessary. We used union find to check this necessity.

## Disassemble Problem
The robot swept from top to bottom. As we did in Assemble Problem, it used high harmonics only when necessary. We also used union find but this time it involved some preprocess.

## Reassemble Problem
We just concatenated our two binaries (of course we removed `halt` command in the middle).
