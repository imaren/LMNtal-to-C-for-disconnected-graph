# LMNtal-to-C-for-disconnected-graph

## LMNtal を C に変換する
- トランスレータ未完成

## 注意
- 試行錯誤の経過がそのまま残っているので、間違っているものや未完成のものも含まれています
- 暫定的に入力はファイルから、出力はアトム毎に手作業で関数を作って行っていますが、トランスレータ作成時にはLMNtal の構造を持つ構造体（クラス）と処理を行ってその構造体を返す関数を作って、main から呼び出す形のプログラムが出力されるようにすると良いのかなと思います
    - 入力は、LMNtal プログラムに記述される初期状態なので、C++ プログラムに含まれるようにするべきだと考えています
    - (繰り返しになりますが)トランスレータまで手が回らなかったので、暫定的にファイル入力のままです

## Examples
- example/2023cpp/
- 以下の3題について計測
- 各例題 delfirst_fromfront.cpp という名前の cpp プログラムが計測に用いたもの
    - thesis.cpp というプログラムが論文に載せたもので、変数名等を変えただけですが一応計測に使ったままのプログラム（delfirst_fromfront.cpp）を残してあります
- テストケースは、C と L の後ろに書かれた文字列が同じものは同じ初期状態の C++ プログラム用記述と LMNtal プログラム用記述
### time-point
- イベントスケジューリング例題
- 元 LMNtal プログラム名
    - time-point.lmn
- testcase/C2_{number}-1 と testcase/C2n_{number}-1, testcase/L2-{number}-1 について計測
- 入力
    - start アトムの数（今回の問題は start が 1つの場合を想定）
    - distance アトムの数
    - 初期状態のハイパーリンクの種類数
    - 初期状態 start アトム
    - 初期状態 distance アトム
### maximum_flow
- 整数最大流問題
- 元 LMNtal プログラム名
    - maxflow_fixed.lmn
- testcase/ 下の各ケースについて計測
- 入力
    - 初期状態のハイパーリンクの種類数
    - cap アトムの数
    - 初期状態の cap アトム

### closure
- 単一の閉包計算からなる例題
- 元 LMNtal プログラム名
    - path.lmn
- testcase/ 下の各ケースについて計測
- 入力
    - 初期状態のハイパーリンクの種類数
    - edge アトムの数
    - 初期状態の edge アトム