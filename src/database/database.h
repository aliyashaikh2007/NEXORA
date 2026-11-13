#ifndef DATABASE_H
#define DATABASE_H

#include "../core/directory.h"

int database_init(const char *dbPath);
int database_save(Node *root);
Node *database_load(void);
void database_close(void);

#endif
