#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <string>
#include <set>

#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>

#include "../macros.h"
#include "../varkey.h"
#include "../thread.h"
#include "../util.h"
#include "../spinbarrier.h"
#include "../core.h"
#include "../allocator.h"
#include "../stats_server.h"
#include "bench.h"
#include "bdb_wrapper.h"
#include "ndb_wrapper.h"
#include "ndb_wrapper_impl.h"
#include "kvdb_wrapper.h"
#include "kvdb_wrapper_impl.h"
#include "silo_c_intf.h"

using namespace std;
using namespace util;

extern "C" {
  silo_t *silo_db_start(int nthreads, size_t size){
    abstract_db *db = NULL;

    /* Steal from dbtest */
    string db_type = "ndb-proto2";
    char *curdir = get_current_dir_name();
    string basedir = curdir;
    free(curdir);

    string bench_opts;
    int nofsync = 0;
    int do_compress = 0;
    int fake_writes = 0;

    vector<string> logfiles;
    vector<vector<unsigned>> assignments;
    string stats_server_sockfile;

    db = new ndb_wrapper<transaction_proto2>(
	logfiles, assignments, !nofsync, do_compress, fake_writes);
    ALWAYS_ASSERT(!transaction_proto2_static::get_hack_status());
    transaction_proto2_static::InitGC();
    if (!stats_server_sockfile.empty()) {
      stats_server *srvr = new stats_server(stats_server_sockfile);
      thread(&stats_server::serve_forever, srvr).detach();
    }


    /* Steal from YCSB benchmark */
    abstract_ordered_index *new_index = db->open_index("USERTABLE", size);

    /* Fill in struct to return to eval */
    silo_t* new_db = (silo_t*) malloc(sizeof(silo_t));
    new_db->db = db;
    new_db->index = new_index;
    return new_db;
  }

  int silo_db_stop(silo_t *master){
    abstract_db *db = (abstract_db*) master->db;
    delete(db);
    free(db);
    return 0;
  }
    
  silo_worker_t *silo_client_start(silo_t *master){
    abstract_db *db = (abstract_db*) master->db;
    db->thread_init(false);

    silo_worker_t* new_wkr = (silo_worker_t*) malloc(sizeof(silo_worker_t));
    new_wkr->parent = master;
    return new_wkr;
  }
  int silo_client_stop(silo_worker_t *worker){
    abstract_db *db = (abstract_db*) worker->parent->db;
    db->thread_end();

    free(worker);
    return 0;
  }

  void *silo_client_start_trans(silo_worker_t *worker){
    abstract_db *db = (abstract_db*) worker->parent->db;
    worker->arena = new str_arena;
    worker->txn_buf = malloc(100*sizeof(char));
    void *txn = db->new_txn(txn_flags,*((str_arena*)worker->arena), worker->txn_buf);

    return txn;
  }
  int silo_client_stop_trans(silo_worker_t *worker, void *txn){
    abstract_db *db = (abstract_db*) worker->parent->db;
    db->commit_txn(txn);
    return 0;
  }

  int silo_client_batch_put(silo_worker_t *worker, void *txn,
			    const char* key, const char* value){
    abstract_ordered_index *index = (abstract_ordered_index*) worker->parent->index;
    index->put(txn, string(key), string(value));

    return 0;
  }

  const char* silo_client_batch_get(silo_worker_t *worker, void *txn, 
				    char* key){
    abstract_ordered_index *index = (abstract_ordered_index*) worker->parent->index;
    std::string val;
    index->get(txn, string(key), val);

    return val.c_str();
  }

#ifdef __cplusplus
}  /* end extern "C" */
#endif
