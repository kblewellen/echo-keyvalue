/* Masstree bindings to C code
 * Katelin Bailey 
 * March 2013
 *
 * Written for inclusion in the Snapshot/Echo evaluation code
 * Assumes that the calling code creates a store (table)
 *   with maximum number of threads as a parameter, along with
 *   logging and checkpointing. Single-threaded, not networked. 
 * Creating a client spawns and associates a logging thread with a
 *   calling thread, assuming the same caller uses the same client. 
 * Based off a mishmash of mttest and mtd, bound to C for compatibilty.
 *
 */

#ifndef MASSTREE_INCLUDE_C_H_
#define MASSTREE_INCLUDE_C_H_


#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
  
  /* Exported types */
  typedef struct masstree_t              masstree_t;
  typedef struct masstree_client_t       masstree_client_t;
  typedef struct masstree_client_trans_t masstree_client_trans_t;
  
  /* Basic start/stop functions */
  extern int masstree_table_open(masstree_t **mt, int num_threads,
				 bool use_checkpoint, bool use_logging);
  extern int masstree_client_start(masstree_t *mt, masstree_client_t **mtc);
  extern int masstree_client_stop(masstree_client_t *mtc);
  extern int masstree_table_close(masstree_t *mt);
  
  /* Unimplemented: transactions */
  extern int masstree_client_start_trans(masstree_client_t *mtc, 
					 masstree_client_trans_t **trans);
  extern int masstree_client_end_trans(masstree_client_t *mtc,
				       masstree_client_trans_t **trans);
  
  /* Operations on store */
  extern int masstree_client_put(masstree_client_t *mtc, 
				 const char *key, const char *value);
  extern int masstree_client_get(masstree_client_t *mtc, 
				 const char *key, const char **value, size_t size);
  extern int masstree_client_delete(masstree_client_t *mtc, const char *key);
  
  /* Test the functionality */
  extern int masstree_verify_bindings(void);

#ifdef __cplusplus
}
#endif

#endif    
