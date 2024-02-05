# LMNtal-to-C-for-disconnected-graph

## LMNtal を C に変換する
- トランスレータ未完成

## 注意
- 試行錯誤の経過がそのまま残っているので、間違っているものや未完成のものも含まれています

## Examples
- example/2023cpp/
- 以下の3題について計測
- 各例題 delfirst_fromfront という名前の cpp プログラムが計測に用いたもの
- テストケースは、C と L の後ろに書かれた文字列が同じものは同じ初期状態の C++ プログラム用記述と LMNtal プログラム用記述
### time-point
- イベントスケジューリング例題
- testcase/C2_{number}-1 と testcase/C2n_{number}-1, testcase/L2-{number}-1 について計測
### maximum_flow
- 整数最大流問題
- testcase/ 下の各ケースについて計測

### closure
- 単一の閉包計算からなる例題
- testcase/ 下の各ケースについて計測