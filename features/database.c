#include "../headers/database.h"
#include "../headers/include.h"

int callback(void *NotUsed, int argc, char **argv, char **azColName)
{

    NotUsed = 0;

    for (int i = 0; i < argc; i++)
    {
        // printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        switch (i)
        {
        case 0:
            ranking[counter].id = argv[i] ? atoi(argv[i]) : -1;
            break;
        case 1:
            ranking[counter].score = argv[i] ? atoi(argv[i]) : -1;
            break;
        case 2:
            strcpy(ranking[counter].username, argv[i] ? argv[i] : "NULL");
            break;
        }
    }
    ++counter;

    return 0;
}

int check_callback(void *NotUsed, int argc, char **argv, char **azColName)
{

    NotUsed = 0;

    for (int i = 0; i < argc; i++)
        argv[i] ? exist = 1 : 0;

    return 0;
}

bool create()
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;

    int rc = sqlite3_open("database/slay-the-assembly.db", &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    char *sql = "CREATE TABLE IF NOT EXISTS ranking (id INTEGER PRIMARY KEY AUTOINCREMENT, score INTEGER, username CHAR(30));";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK)
    {
        sqlite3_step(res);
        sqlite3_finalize(res);
    }
    else
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_close(db);
    return true;
}

bool insert(int score, char *username)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;

    int rc = sqlite3_open("database/slay-the-assembly.db", &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    char *sql = "INSERT INTO ranking (score, username) VALUES (?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK)
    {
        sqlite3_bind_int(res, 1, score);
        sqlite3_bind_text(res, 2, username, strlen(username) / sizeof(char), SQLITE_TRANSIENT);
        sqlite3_step(res);
        sqlite3_finalize(res);
    }
    else
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_close(db);
    return true;
}

bool select_head()
{

    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("database/slay-the-assembly.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n",
                sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

    char *sql = "SELECT * FROM ranking ORDER BY score DESC LIMIT 5";

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    counter = 0;

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);

        return 1;
    }

    sqlite3_close(db);

    return 0;
}

bool select_by_username(char *username)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;

    int rc = sqlite3_open("database/slay-the-assembly.db", &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    char *sql = "SELECT id FROM ranking WHERE username=?";

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username) / sizeof(char), SQLITE_TRANSIENT);
        rc = sqlite3_step(res);
        printf("%s\n", sqlite3_column_text(res, 0));
        if (rc == SQLITE_ROW)
        {
            exist = 1;
            printf("%s\n", sqlite3_column_text(res, 0));
        }
        sqlite3_finalize(res);
    }
    else
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_close(db);
    return true;
}

bool check_username(char *username)
{
    select_by_username(username);
    if (exist)
        return true;
    return false;
}

bool update(int score, char *username)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;

    int rc = sqlite3_open("database/slay-the-assembly.db", &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    char *sql = "UPDATE ranking SET score=? WHERE username=?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK)
    {
        sqlite3_bind_int(res, 1, score);
        sqlite3_bind_text(res, 2, username, strlen(username) / sizeof(char), SQLITE_TRANSIENT);
        sqlite3_step(res);
        sqlite3_finalize(res);
    }
    else
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_close(db);
    return true;
}

bool delete(char *username)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;

    int rc = sqlite3_open("database/slay-the-assembly.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return false;
    }

    char *sql = "DELETE FROM ranking WHERE username=?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc == SQLITE_OK)
    {
        sqlite3_bind_text(res, 1, username, strlen(username) / sizeof(char), SQLITE_TRANSIENT);
        sqlite3_step(res);
        sqlite3_finalize(res);
    }
    else
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_close(db);
    return true;
}

bool delete_all()
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;

    int rc = sqlite3_open("database/slay-the-assembly.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return false;
    }

    char *sql = "DELETE FROM ranking;";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc == SQLITE_OK)
    {
        sqlite3_step(res);
        sqlite3_finalize(res);
    }
    else
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_close(db);
    return true;
}

void display_ranking_head()
{
    select_head();
    for (int i = 0; i < 5; ++i)
        printf("Id: %d | Username: %s | Score: %d\n", ranking[i].id, ranking[i].username, ranking[i].score);
}
