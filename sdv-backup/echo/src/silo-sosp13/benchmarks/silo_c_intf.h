#ifndef STORAGE_SILO_INCLUDE_C_H
#define STORAGE_SILO_INCLUDE_C_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

  /* Exported types */
  struct silo_struct{ 
    void *db;
    void *index;
  };
  typedef struct silo_struct silo_t;

  struct silo_worker_struct{
    silo_t  *parent; 
    void *arena;
    void *txn_buf;
  };
  typedef struct silo_worker_struct silo_worker_t;

  /* Exported functions */
  silo_t *silo_db_start(int num_threads, size_t size_hint);
  int silo_db_stop(silo_t *db);
    
  silo_worker_t *silo_client_start(silo_t *master);
  int silo_client_stop(silo_worker_t *db);

  void *silo_client_start_trans(silo_worker_t *db);
  int silo_client_stop_trans(silo_worker_t *db, void *wb);

  int silo_client_batch_put(silo_worker_t *db, void *batch, 
			    const char* key, const char* value);

  const char* silo_client_batch_get(silo_worker_t *db, void *batch, char* key);
  //  int silo_client_delete(silo_worker_t *db, const char *key);

#ifdef __cplusplus
}  /* end extern "C" */
#endif

#endif
