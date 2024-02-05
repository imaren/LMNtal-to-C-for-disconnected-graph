# 非連結グラフマッチング（削除無し）

## ガードの情報を使う
- LMNtal プログラムから抽出する情報
-- 引数が増えないかどうか(またその最大値はなにか)
--- ガードに new が無い
--- 無限に増加しうる式が無い N < 100, A = N+1 等は A の最大値がわかるので OK  
--ガードの等式でデータ構造を決定
---例えば atom1(Args1), atom2(Args2) \:- a \in Args1 =:= b \in Args2, uniq... | Body の場合、atom1[Args1_aの最大値] と atom2[Args2_aの最大値] を c++ が持っておけば効率的にマッチング可能
---- 最大値がわからなければ Vector 等の可変長配列
--- ガードが、a \in Args1 =:= b \in Args2, c \in Args1 =:= d \in Args2 の場合は、2次元配列になる．（n 個の条件の and になっているときは n 次元配列…1次元で割り当てて計算する？）

## ルール左辺（右辺に出現するルールの左辺も含む）の情報を使う
- ルール左辺に出現しないアトムはテキストデータで入力のまま保持する
- ルール左辺に出現するアトムに対してはリストを作る
- list の要素の型はガード見る
-- 例えばint を扱うルールだったら string とかが引数に含まれているアトムは絶対にマッチング失敗するので上のようにテキストのまま保持
- HL は 整数にマッピング（プログラムのガードに new が含まれなければ静的にマッピング可能）
- 左辺の連結成分が 1つのルールを優先する
-- 今回はアトムが消えない場合を考えているけど、消えるときは消えるルールを優先する

## c++ プログラム
```C++:概要
    struct type_atom_1{
        type arg_1;
        ...
        type arg_n;
    }
    ...
    struct type_atom_n{
        type arg_1;
        ...
        type arg_n
    }

    int main(){
        // アトムリスト
        list<type_atom_1> atom_1;
        ...
        list<type_atom_n> atom_n;

        // 挿入前のアトムを保持するリスト
        list<type_atom_1> push_atom_1;
        ...
        list<type_atom_n> push_atom_n;

        //rule_1 で使うデータ(ガード条件から適切に抽出)
        arr<type,size> rule_1_atom_1_from_guard;
        ...
        arr<type,size> rule_1_atom_n_from_guard;

        ...

        //rule_m で使うデータ（ガード条件から適切に抽出）
        arr<type,size> rule_m_atom_1_from_guard;
        ...
        arr<type,size> rule_m_atom_n_from_guard;


        while(all push_atom are not empty){
            atom = push_atom_1.pop();
                // atom_1 を使うルールのマッチングを試す

            ...
            atom = push_atom_n.pop();
                // atom_n を使うルールのマッチングを試す
        }
        return 0;
    }

```

```C++:atom_n を使うルールのマッチング
    //uniq を含むなら（アトムを消さない場合、uniq を含まないとおかしい）

    set<args*> history;
    type_atom_n atom_pop;
    for(atom_pop の引数にマッチングするアトム){
        args[0] = /*history に対応する引数*/,...,args[m] = /*history に対応する引数*/
        if(history.insert(uniqの引数)){
            //マッチング成功
            type_atom_n newatom = {/*右辺*/};
            push_atom_n.push(newatom);
        }
    }
```
