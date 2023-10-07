#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

struct dbcolumn{
    int id;
    std::string name;
    std::string category;
    int cost;
    int size;
    int weight;

    void dump(){
        std::cout << id << "|" << name << "|" << category << "|" << cost << "|" << size << "|" << weight ;
    };

    void dumpln(){
         std::cout << id << "|" << name << "|" << category << "|" << cost << "|" << size << "|" << weight << std::endl;
    };
};

int main(void){
    sqlite3 *db = NULL;
    int ret = sqlite3_open(":memory:", &db);
    // int ret = sqlite3_open("hoge.db", &db);
    char* err = NULL;
    int count = 0;
    sqlite3_stmt *stmt = NULL;
    std::vector<dbcolumn> m_dbList;
    
    if(ret != SQLITE_OK){
        printf("FILE Open Error \n");
        return -1;
    }

    ret = sqlite3_exec(db, "create table huga(id INTEGER, name TEXT, category TEXT, cost INTEGER, size INTEGER, weight INTEGER);", NULL, NULL, &err);
    if(ret != SQLITE_OK){
        printf("Execution Err \n");
        sqlite3_close(db);
        sqlite3_free(err);
        return -1;
    }

    char insertMsg[100] ="insert into huga values(1, 'AAA', 'aaa', 10, 100, 111);" ;
    ret = sqlite3_exec(db, insertMsg, NULL, NULL, &err);

    char select[30] = "select * from huga;";
    sqlite3_prepare_v2(db, select, strlen(select), &stmt, NULL);
    sqlite3_reset(stmt);

    int r = 0;
    dbcolumn m_dbcolumn;
    while (SQLITE_ROW == (r=sqlite3_step(stmt))){

        // 数値は列数に対応し、各行に対するどの列を読み出すか指定が可能
        m_dbcolumn.id = sqlite3_column_int(stmt, 0);                    
        // sqlite3_column_intを使用すればint型で定義したデータが読み込める.
        const unsigned char* name = sqlite3_column_text(stmt, 1);       
        // sqlite3_column_textを使用すればtext型で定義したデータが読み込める
        const unsigned char* category = sqlite3_column_text(stmt, 2);
        m_dbcolumn.name = std::string((char*)name);
        m_dbcolumn.category = std::string((char*)category);
        m_dbcolumn.cost = sqlite3_column_int(stmt, 3);
        m_dbcolumn.size = sqlite3_column_int(stmt, 4);
        m_dbcolumn.weight = sqlite3_column_int(stmt, 5);

        m_dbList.push_back(m_dbcolumn);
    }
    if (SQLITE_DONE!=r){
        printf("Read Error \n");
        return -1;
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);

    for(auto i : m_dbList){
        i.dumpln();
    }

    printf("Finish \n");
    return 0;
}