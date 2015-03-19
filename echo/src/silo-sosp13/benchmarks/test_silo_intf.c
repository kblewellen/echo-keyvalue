#include <stdlib.h>
#include <stdio.h>

#include "silo_c_intf.h"

int main(int argc, char **argv){
  silo_t *siloDB = silo_db_start(1, 100);
  if(siloDB != NULL)
    printf("master start success\n");

  silo_worker_t* workers[16];
  void* txn[16];
  int i;
  for( i = 0 ; i < 16; i++){
    workers[i] = silo_client_start(siloDB);
    txn[i] = silo_client_start_trans(workers[i]);
  }

  char* key = "key1";
  silo_client_batch_put(workers[1], txn[1], key, "val1");
  silo_client_stop_trans(workers[1], txn[1]);

  const char* buf;
  txn[1] = silo_client_start_trans(workers[1]);
  //silo_client_batch_put(workers[1], txn[1], key, "val2");
  buf = silo_client_batch_get(workers[1],txn[1], key);

  printf("got back %s\n", buf);
  for( i = 0 ; i < 16; i++){
    silo_client_stop_trans(workers[i], txn[i]);
    silo_client_stop(workers[i]);
  }

  silo_db_stop(siloDB);

  return 0;
}
