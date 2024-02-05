#include <iostream>
#include <vector>
#include<list>

int main() {
    std::list<int> myVector = {1, 2, 3, 4, 5};
    std::list<int>::iterator it = myVector.begin();
    std::list<int>::iterator it2 = myVector.begin();

    // イテレーターが無効化されているかどうかの確認
    if (it != myVector.end()) {
        // イテレーターが有効な場合の処理
        std::cout << "Iterator is valid. Value: " << *it << std::endl;
    } else {
        // イテレーターが無効な場合の処理
        std::cout << "Iterator is invalid." << std::endl;
    }

    // 例として、要素を削除したり、コレクションを変更する操作を行う
    myVector.erase(it);
    for(auto a : myVector){
        std::cout << a << ", " ;
    }
    std::cout << std::endl;

    // イテレーターが無効化されているかどうかの再確認
    if (it != myVector.end()) {
        std::cout << "Iterator is valid. Value: " << *it << std::endl;
    } else {
        std::cout << "Iterator is invalid." << std::endl;
    }
    if (it2 != myVector.end()) {
        std::cout << "Iterator is valid. Value: " << *it2 << std::endl;
    } else {
        std::cout << "Iterator is invalid." << std::endl;
    }

    return 0;
}