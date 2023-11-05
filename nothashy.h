/*
 * NotHashy is a simple hash table implementation in C.
 * Made by Nyvyme in 2023.
 */

#ifndef NOTHASHY_H
#define NOTHASHY_H

typedef struct _hash_table_t hash_table_t;

hash_table_t *hash_table_create(void);
void hash_table_destory(hash_table_t *ht);
void hash_table_print(hash_table_t *ht);
char hash_table_insert(hash_table_t *ht, const char *key, void *value);
void *hash_table_get(hash_table_t *ht, const char *key);
void *hash_table_remove(hash_table_t *ht, const char *key);

#endif // NOTHASHY_H

/* IMPLEMENTATION */

#ifdef NOTHASHY_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static const unsigned int TABLE_SIZE = 100000;

typedef struct _entry_t {
  char *key;
  void *value;
  struct _entry_t *next;
} entry_t;

struct _hash_table_t {
  entry_t **entries;
};


unsigned int hash(const char *key) {
  unsigned long int value = 0;
  unsigned int i = 0;
  unsigned int key_len = strlen(key);

  for (; i < key_len; i++) {
    value = value * 37 + key[i];
  }

  return value % TABLE_SIZE;
}

static unsigned int _ht_index(hash_table_t *ht, const char *key) {
  return hash(key) % TABLE_SIZE;
}

hash_table_t *hash_table_create(void) {
  hash_table_t *ht = malloc(sizeof(*ht));
  ht->entries = calloc(sizeof(entry_t*), TABLE_SIZE);
  return ht;
}

void hash_table_destroy(hash_table_t *ht) {
  if (ht == NULL) return;
  for (unsigned int i = 0; i < TABLE_SIZE; i++) {
    while (ht->entries[i]) {
      entry_t *tmp = ht->entries[i];
      ht->entries[i] = ht->entries[i]->next;
      free(tmp->key);
      free(tmp->value);
      free(tmp);
    }
  }

  free(ht->entries);
  free(ht);
}

void hash_table_print(hash_table_t *ht) {
  printf("Start Table\n");
  for (unsigned int i = 0; i < TABLE_SIZE; i++) {
    if (ht->entries[i] == NULL) continue;

    printf("\t%i\t", i);
    entry_t *tmp = ht->entries[i];
    while (tmp != NULL) {
      printf("\"%s\"(%p) - ", tmp->key, tmp->value);
      tmp = tmp->next;
    }
    printf("\n");
  }
  printf("End Table\n");
}

char hash_table_insert(hash_table_t *ht, const char *key, void *value) {
  if (ht == NULL || key == NULL || value == NULL) return 0;
  unsigned int index = _ht_index(ht, key);

  if (hash_table_get(ht, key) != NULL) return 0;

  entry_t *e = malloc(sizeof(*e));
  e->value = value;
  e->key = malloc(strlen(key) + 1);
  strcpy(e->key, key);

  e->next = ht->entries[index];
  ht->entries[index] = e;
  return 1;
}

void *hash_table_get(hash_table_t *ht, const char *key) {
  if (ht == NULL || key == NULL) return NULL;
  unsigned int index = _ht_index(ht, key);

  entry_t *tmp = ht->entries[index];
  while (tmp != NULL && strcmp(tmp->key, key) != 0) {
    tmp = tmp->next;
  }
  if (tmp == NULL) return NULL;
  return tmp->value;
}

void *hash_table_remove(hash_table_t *ht, const char *key) {
  if (ht == NULL || key == NULL) return NULL;
  unsigned int index = _ht_index(ht, key);

  entry_t *tmp = ht->entries[index];
  entry_t *prev = NULL;
  while (tmp != NULL && strcmp(tmp->key, key) != 0) {
    prev = tmp;
    tmp = tmp->next;
  }

  if (tmp == NULL) return NULL;
  if (prev == NULL) {
    ht->entries[index] = tmp->next;
  }
  else {
    prev->next = tmp->next;
  }

  void *result = tmp->value;
  free(tmp);
  return result;
}

#endif // NOTHASHY_IMPLEMENTATION
