#include <sqlite3.h>
#include <stdio.h>
#include <stdbool.h>

int callback(void *NotUsed, int argc, char **argv, char **azColName)
{

    NotUsed = 0;

    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");

    return 0;
}

bool create()
{
    sqlite3 *db;
    char *err_msg = 0;
    char *sql = "CREATE TABLE IF NOT EXISTS score (id INTEGER PRIMARY KEY AUTOINCREMENT, score INTEGER, username CHAR(30))";
    int rc = sqlite3_open("test.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return false;
    }

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);

        return false;
    }

    sqlite3_close(db);

    return true;
}

int main(void)
{
    if (create())
        return 0;
}