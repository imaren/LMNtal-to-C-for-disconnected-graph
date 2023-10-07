#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <chrono>

struct distance{
    int min;
    std::string from;
    std::string to;
    int max;

    void dump(){
        std::cout << min << "|" << from << "|" << to << "|" << max ;
    };

    void dumpln(){
         std::cout << min << "|" << from << "|" << to << "|" << max << std::endl;
    };

    void lmn(){
        std::cout << " distance(" << min << "," << from << "," << to << "," << max << ").";
    }
};

struct betweem{
    int min;
    std::string node;
    int max;

    void dump(){
        std::cout << min << "|" << node <<  "|" << max ;
    };

    void dumpln(){
         std::cout << min << "|" << node << "|" << max << std::endl;
    };

    void lmn(){
        std::cout << " between(" << min << "," << node << "," << max << ").";
    }
};

std::string make_insert_b(betweem in){
    std::stringstream ss;
    ss << "insert into betweem(min,node,max) values(" << std::to_string(in.min) << "," << in.node << "," << std::to_string(in.max) <<
        ") on conflict(node) do update set min = case when " << std::to_string(in.min) << " > min then " << std::to_string(in.min) <<
        " else min end, max = case when " << std::to_string(in.max) << " < max then " << std::to_string(in.max) <<
        " else max end;" ;

    std::string tmp = ss.str();
    return tmp;
}

std::string make_insert_d(distance in){
    std::stringstream ss;
    ss << "insert into distance values(" << std::to_string(in.min) << "," << in.from << "," << in.to << "," 
        << std::to_string(in.max) << ");" ;
    std::string tmp = ss.str();
    return tmp;
}

int main(int argc, char *argv[]){
    sqlite3 *db = NULL;
    int ret = sqlite3_open(":memory:", &db);
    // int ret = sqlite3_open("hoge.db", &db);
    char* err = NULL;
    int count = 0;
    sqlite3_stmt *stmt = NULL;
    std::vector<betweem> m_betweem;
    std::vector<distance> m_distance;
   
    if(ret != SQLITE_OK){
        printf("FILE Open Error \n");
        return -1;
    }

    ret = sqlite3_exec(db, "create table distance(mind integer, frm text, t text, maxd integer);", NULL, NULL, &err);
    if(ret != SQLITE_OK){
        printf("Execution Err \n");
        sqlite3_close(db);
        sqlite3_free(err);
        return -1;
    }
    ret = sqlite3_exec(db, "create table betweem(min INTEGER, node TEXT primary key, max INTEGER);", NULL, NULL, &err);
    if(ret != SQLITE_OK){
        printf("Execution Err \n");
        sqlite3_close(db);
        sqlite3_free(err);
        return -1;
    }

    ret = sqlite3_exec(db, "PRAGMA recursive_triggers = ON;", 0, 0, 0);


     /* 
    * 入力形式
    *   <start_1 の数>
    *   <distance_4 の数>
    *   <hl id の数>
    *   <start_1> ...
    *   <distance_4> ...
    */
    std::string filename = "input.txt";
    if(argc >= 2){
        filename = argv[1];
    }
    std::ifstream i_file(filename);
    if(!i_file.is_open()){
        std::cerr << "Could not open input file - " << filename << std::endl;
        return 1;
    }
    std::string filename2 = "result_timepoint_norec.csv";
    // 入力補助情報. test case では betweem, max, min は入力 0個 とする
    int n_start, n_distance, n_betweem, n_max, n_min;
    // 解析情報．今回はガードに比較演算がないので HL の id の数だけ
    int n_hl;
    i_file >> n_start >> n_distance; 
    i_file >> n_hl;

    // まずは、insert しながら update するバージョン
    std::list<distance> push_distance; 
    std::list<betweem> push_betweem;


    int link1, link2, link3, link4;
    std::string tmp1,tmp2;
    for(size_t i = 0; i < n_start; i++){
        i_file >> link1;
        betweem t;
        t.min = 0; t.max = 0; t.node = std::to_string(link1);
        push_betweem.push_back(t);
    }
    for (size_t i = 0; i < n_distance; i++)
    {
        i_file >> link1 >> link2 >> link3 >> link4;
        distance t;
        t.min = link1; t.from= std::to_string(link2); t.to = std::to_string(link3); t.max = link4;
        push_distance.push_back(t);
    }

    // propagation の trigger を設定
    auto trig = "create trigger ins_rec_trigger after insert on betweem for each row\
                begin \
                    insert into betweem \
                    select mind + NEW.min, t,  maxd + NEW.max \
                    from distance \
                    where distance.frm = NEW.node \
                    on conflict(node) do update set \
                    min = case when excluded.min > min then excluded.min else min end, \
                    max = case when excluded.max < max then excluded.max else max end;\
                    insert into betweem \
                    select NEW.min - maxd, frm,  NEW.max - mind\
                    from distance \
                    where distance.t = NEW.node \
                    on conflict(node) do update set \
                    min = case when excluded.min > min then excluded.min else min end, \
                    max = case when excluded.max < max then excluded.max else max end\
                    where excluded.min > min or excluded.max < max;\
                end\
                ";

    auto trig2 = "create trigger ins_rec_trigger2 after update on betweem for each row\
                begin \
                    insert into betweem \
                    select mind + NEW.min, t,  maxd + NEW.max \
                    from distance \
                    where distance.frm = NEW.node \
                    on conflict(node) do update set \
                    min = case when excluded.min > min then excluded.min else min end, \
                    max = case when excluded.max < max then excluded.max else max end;\
                    insert into betweem \
                    select NEW.min - maxd, frm,  NEW.max - mind\
                    from distance \
                    where distance.t = NEW.node \
                    on conflict(node) do update set \
                    min = case when excluded.min > min then excluded.min else min end, \
                    max = case when excluded.max < max then excluded.max else max end\
                    where excluded.min > min or excluded.max < max;\
                end\
                ";

                    //                 on conflict(node) do update set \
                    // min = case when NEW.min > min then NEW.min else min end, \
                    // max = case when NEW.max < max then NEW.max else max end; \

    // ret = sqlite3_exec(db, trig, NULL, NULL, &err);
    // ret = sqlite3_exec(db, trig2, NULL, NULL, &err);
    // if(ret != 0){
    //     std::string e(err);
    //     std::cout << "err: " << e << std::endl;
    // }

    //開始 
    auto start = std::chrono::steady_clock::now();
    for(auto i = push_distance.begin(); i != push_distance.end();){
        auto tmp = *i;
        auto msg = make_insert_d(tmp);
        ret = sqlite3_exec(db, msg.c_str(), NULL, NULL, &err);
        i = push_distance.erase(i);
    }

    auto start2 = std::chrono::steady_clock::now();
    
    for(auto itr = push_betweem.begin(); itr != push_betweem.end(); ){
        // between b を push したいとき

        // 1. between から node が b.node と一致しているレコードを select
        //      更新がある場合はそれを push_between に push
        //      　　　ない場合はそのまま
        std::stringstream s2;
        s2 << "select * from betweem where node=" << itr->node << ";";
        std::string m2 = s2.str();
        sqlite3_prepare_v2(db, m2.c_str(), strlen(m2.c_str()), &stmt, NULL);
        sqlite3_reset(stmt);
        bool flag = true; // 同じ node に対する between が なかったら true
        if (SQLITE_ROW == sqlite3_step(stmt)){
            flag = false;
            betweem newb;
            newb.min = sqlite3_column_int(stmt, 0);   
            newb.max = sqlite3_column_int(stmt, 2);
            if(newb.min < itr->min || newb.max > itr->max){
                // std::cout << "update: " ;
                // itr->dump();
                // std::cout << " -> ";
                // newb.dumpln();
                // std::cout << "upd b: " << itr->node <<  " -> " << newb.min << " <? " << itr->min << " : " << newb.max << " >? " << itr->max << std::endl;
                newb.min = newb.min < itr->min ? itr->min : newb.min;
                newb.max = newb.max > itr->max ? itr->max : newb.max;
                // std::cout << "end upd b: " << itr->node <<  " -> " << newb.min << " <? " << itr->min << " : " << newb.max << " >? " << itr->max << std::endl;
                newb.node = itr->node;
                std::stringstream upd;
                upd << "update betweem set min=" << newb.min << ", max=" << newb.max << " where node=" << newb.node << ";";
                std::string updmsg = upd.str();
                sqlite3_exec(db,updmsg.c_str(),NULL,NULL,&err);
                if(err != 0){
                std::string e(err);
                std::cout << e << ": " << upd.str() << std::endl;
                }
                
                push_betweem.push_back(newb);
                // update が起こったら propagate
                std::stringstream s11;
                s11 << "select * from distance where frm=" << newb.node << ";";
                std::string m11 = s11.str();
                sqlite3_prepare_v2(db, m11.c_str(), strlen(m11.c_str()), &stmt, NULL);
                sqlite3_reset(stmt);
                while (SQLITE_ROW == sqlite3_step(stmt)){
                    betweem newb2;
                    newb2.min = sqlite3_column_int(stmt, 0) + newb.min;                       
                    const unsigned char* to = sqlite3_column_text(stmt, 2);
                    newb2.node = std::string((char*)to);
                    newb2.max = sqlite3_column_int(stmt, 3) + newb.max;
                    push_betweem.push_back(newb2);
                }

                // 2-2. distance から to, が b.node と一致しているレコードを select
                //      ルールを発火させて push_between に push
                std::stringstream s12;
                s12 << "select * from distance where t=" << newb.node << ";";
                std::string m12 = s12.str();
                sqlite3_prepare_v2(db, m12.c_str(), strlen(m12.c_str()), &stmt, NULL);
                sqlite3_reset(stmt);
                while (SQLITE_ROW == sqlite3_step(stmt)){
                    betweem newb2;
                    newb2.max = newb.max - sqlite3_column_int(stmt, 0);                       
                    const unsigned char* from = sqlite3_column_text(stmt, 1);
                    newb2.node = std::string((char*)from);
                    newb2.min = newb.min - sqlite3_column_int(stmt, 3);
                    push_betweem.push_back(newb2);
                }
            }
        }
        // 3. select で何も持ってこなかった場合はレコードを挿入
        if(flag){
                    // 2-1. distance から from,  が b.node と一致しているレコードを select
            //      ルールを発火させて push_between に push
            std::stringstream s11;
            s11 << "select * from distance where frm=" << itr->node << ";";
            std::string m11 = s11.str();
            sqlite3_prepare_v2(db, m11.c_str(), strlen(m11.c_str()), &stmt, NULL);
            sqlite3_reset(stmt);
            while (SQLITE_ROW == sqlite3_step(stmt)){
                betweem newb;
                newb.min = sqlite3_column_int(stmt, 0) + itr->min;                       
                const unsigned char* to = sqlite3_column_text(stmt, 2);
                newb.node = std::string((char*)to);
                newb.max = sqlite3_column_int(stmt, 3) + itr->max;
                push_betweem.push_back(newb);
            }

            // 2-2. distance から to, が b.node と一致しているレコードを select
            //      ルールを発火させて push_between に push
            std::stringstream s12;
            s12 << "select * from distance where t=" << itr->node << ";";
            std::string m12 = s12.str();
            sqlite3_prepare_v2(db, m12.c_str(), strlen(m12.c_str()), &stmt, NULL);
            sqlite3_reset(stmt);
            while (SQLITE_ROW == sqlite3_step(stmt)){
                betweem newb;
                newb.max = itr->max - sqlite3_column_int(stmt, 0);                       
                const unsigned char* from = sqlite3_column_text(stmt, 1);
                newb.node = std::string((char*)from);
                newb.min = itr->min - sqlite3_column_int(stmt, 3);
                push_betweem.push_back(newb);
            }
            std::stringstream s3;
            s3 << "insert into betweem(min,node,max) values(" << itr->min << "," << itr->node << "," << itr->max << ");";
            std::string msg = s3.str();
            ret = sqlite3_exec(db, msg.c_str(), NULL, NULL, &err);
            if(err != 0){
                std::string e(err);
                std::cout << e << ": " << s3.str() << std::endl;
            }
        }
        itr = push_betweem.erase(itr);
    }

    auto end = std::chrono::steady_clock::now();
    long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    long duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start2).count();
    std::ofstream o_file;
    o_file.open(filename2, std::ios::app);

    o_file << duration << "," << duration2 << ",";
    //終了

    // output
    char select2[30] = "select * from distance;";
    sqlite3_prepare_v2(db, select2, strlen(select2), &stmt, NULL);
    sqlite3_reset(stmt);
    int r = 0;
    distance m_dbcolumn;
    while (SQLITE_ROW == (r=sqlite3_step(stmt))){

        m_dbcolumn.min = sqlite3_column_int(stmt, 0);                    
        const unsigned char* from = sqlite3_column_text(stmt, 1);       
        const unsigned char* to = sqlite3_column_text(stmt, 2);
        m_dbcolumn.from = std::string((char*)from);
        m_dbcolumn.to = std::string((char*)to);
        m_dbcolumn.max = sqlite3_column_int(stmt, 3);

        m_distance.push_back(m_dbcolumn);
    }

    char select3[30] = "select * from betweem;";
    sqlite3_prepare_v2(db, select3, strlen(select3), &stmt, NULL);
    sqlite3_reset(stmt);
    r = 0;
    betweem m_dbcolumnb;
    while (SQLITE_ROW == (r=sqlite3_step(stmt))){

        m_dbcolumnb.min = sqlite3_column_int(stmt, 0);                    
        const unsigned char* node = sqlite3_column_text(stmt, 1);    
        m_dbcolumnb.node = std::string((char*)node);
        m_dbcolumnb.max = sqlite3_column_int(stmt, 2);

        m_betweem.push_back(m_dbcolumnb);
    }
    if (SQLITE_DONE!=r){
        printf("Read Error \n");
        return -1;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    

    for(auto i : m_distance){
        i.lmn();
        // i.dumpln();
    }
    // std::cout << "between" << std::endl;
    for(auto i : m_betweem){
        i.lmn();
        // i.dumpln();
    }
    std::cout << std::endl;

    // printf("Finish \n");
    return 0;
}