/*
 * testlibpq.c
 *
 
 *              C言語PostgreSQLフロントエンドライブラリlibpqの試験
 */
#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

static void
exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

int
main(int argc, char **argv)
{
    const char *conninfo;
    PGconn     *conn;
    PGresult   *res;
    int         nFields;
    int         i,
                j;

    /*
     * ユーザがコマンドラインでパラメータを提供した場合、conninfo文字列として使用する。
     * 提供されない場合はデフォルトでdbname=postgresを使用する。
     * その他の接続パラメータについては環境変数やデフォルトを使用する。
     * 
     */
    if (argc > 1)
        conninfo = argv[1];
    else
        conninfo = "dbname = postgres";

    /* データベースとの接続を確立する */
    conn = PQconnectdb(conninfo);

    /* バックエンドとの接続確立に成功したかを確認する */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s",
                PQerrorMessage(conn));
        exit_nicely(conn);
    }

    /*
     * この試験ケースではカーソルを使用する。
     * そのため、トランザクションブロック内で実行する必要がある。
     * 全てを単一の"select * from pg_database"というPQexec()で行うこと
     * も可能だが、例としては簡単過ぎる。
     */

    /* トランザクションブロックを開始する。 */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }

    /*
    
     * 不要になったら、メモリリークを防ぐためにPGresultをPQclearすべき。
     */
    PQclear(res);

    /*
     * データベースのシステムカタログpg_databaseから行を取り出す。
     */
    res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from pg_database");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "DECLARE CURSOR failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }
    PQclear(res);

    res = PQexec(conn, "FETCH ALL in myportal");
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "FETCH ALL failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }

    /* まず属性名を表示する。 */
    nFields = PQnfields(res);
    for (i = 0; i < nFields; i++)
        printf("%-15s", PQfname(res, i));
    printf("\n\n");

    /* そして行を表示する。 */
    for (i = 0; i < PQntuples(res); i++)
    {
        for (j = 0; j < nFields; j++)
            printf("%-15s", PQgetvalue(res, i, j));
        printf("\n");
    }

    PQclear(res);

    /* ポータルを閉ざす。ここではエラーチェックは省略した… */
    res = PQexec(conn, "CLOSE myportal");
    PQclear(res);

    /* トランザクションを終了する */
    res = PQexec(conn, "END");
    PQclear(res);

    /* データベースとの接続を閉じ、後始末を行う。 */
    PQfinish(conn);

    return 0;
}