/*
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "mt_c_wrappers.h"

//============================================================================//
/* Straight from mttest */
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <limits.h>
#if HAVE_NUMA_H
#include <numa.h>
#endif
#if HAVE_SYS_EPOLL_H
#include <sys/epoll.h>
#endif
#if __linux__
#include <asm-generic/mman.h>
#endif
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <signal.h>
#include <errno.h>
#ifdef __linux__
#include <malloc.h>
#endif
#include "kvstats.hh"
#include "masstree_query.hh"
#include "json.hh"
#include "kvtest.hh"
#include "kvrandom.hh"
#include "kvrow.hh"
#include "clp.h"
#include <algorithm>

/* Globals */

static std::vector<int> cores;
volatile bool timeout[2] = {false, false};
double duration[2] = {10, 0};
// Do not start timer until asked
static bool lazy_timer = false;

uint64_t test_limit = ~uint64_t(0);
bool quiet = false;
bool print_table = false;
//static const char *gid = NULL;

// all default to the number of cores
static int udpthreads = 0;
static int tcpthreads = 0;

//static bool tree_stats = false;
static bool json_stats = false;
static bool pinthreads = false;
volatile uint64_t globalepoch = 1;     // global epoch, updated by main thread regularly
kvepoch_t global_log_epoch = 0;
//static int port = 2117;
//static int rscale_ncores = 0;

#if MEMSTATS && HAVE_NUMA_H && HAVE_LIBNUMA
static struct {
  long long free;
  long long size;
} numa[MaxNumaNode];
#endif

volatile bool recovering = false; // so don't add log entries, and free old value immediately
kvtimestamp_t initial_timestamp;

static const char *threadcounter_names[(int) tc_max];


/* running local tests */
void test_timeout(int) {
    size_t n;
    for (n = 0; n < arraysize(timeout) && timeout[n]; ++n)
	/* do nothing */;
    if (n < arraysize(timeout)) {
	timeout[n] = true;
	if (n + 1 < arraysize(timeout) && duration[n + 1])
	    xalarm(duration[n + 1]);
    }
}

template <typename T>
struct kvtest_client {
    kvtest_client()
	: limit_(test_limit), ncores_(udpthreads), kvo_() {
    }
    ~kvtest_client() {
	if (kvo_)
	    free_kvout(kvo_);
    }
    int nthreads() const {
	return udpthreads;
    }
    int id() const {
	return ti_->ti_index;
    }
    void set_table(T *table, threadinfo *ti) {
	table_ = table;
	ti_ = ti;
    }
    void reset(const String &test, int trial) {
	json_ = Json().set("table", T().name())
	    .set("test", test).set("trial", trial)
	    .set("thread", ti_->ti_index);
    }
    static void start_timer() {
        mandatory_assert(lazy_timer && "Cannot start timer without lazy_timer option");
        mandatory_assert(duration[0] && "Must specify timeout[0]");
        xalarm(duration[0]);
    }
    bool timeout(int which) const {
	return ::timeout[which];
    }
    uint64_t limit() const {
	return limit_;
    }
    int ncores() const {
        return ncores_;
    }
    double now() const {
	return ::now();
    }
    int ruscale_partsz() const {
        return (140 * 1000000) / 16;
    }
    int ruscale_init_part_no() const {
        return ti_->ti_index;
    }
    long nseqkeys() const {
        return 16 * ruscale_partsz();
    }

    void get(long ikey);
    bool get_sync(const str &key);
    bool get_sync(const str &key, str &value);
    bool get_sync(long ikey) {
	quick_istr key(ikey);
	return get_sync(key.string());
    }
    bool get_sync_key16(long ikey) {
	quick_istr key(ikey, 16);
	return get_sync(key.string());
    }
    void get_check(const str &key, const str &expected);
    void get_check(const char *key, const char *expected) {
	get_check(str(key), str(expected));
    }
    void get_check(long ikey, long iexpected) {
	quick_istr key(ikey), expected(iexpected);
	get_check(key.string(), expected.string());
    }
    void get_check(const str &key, long iexpected) {
	quick_istr expected(iexpected);
	get_check(key, expected.string());
    }
    void get_check_key8(long ikey, long iexpected) {
	quick_istr key(ikey, 8), expected(iexpected);
	get_check(key.string(), expected.string());
    }
    void get_col_check(const str &key, int col, const str &value);
    void get_col_check(long ikey, int col, long ivalue) {
	quick_istr key(ikey), value(ivalue);
	get_col_check(key.string(), col, value.string());
    }
    void get_col_check_key10(long ikey, int col, long ivalue) {
	quick_istr key(ikey, 10), value(ivalue);
	get_col_check(key.string(), col, value.string());
    }
    void many_get_check(int nk, long ikey[], long iexpected[]);

    void scan_sync(const str &firstkey, int n,
		   std::vector<str> &keys, std::vector<str> &values);
    void rscan_sync(const str &firstkey, int n,
		    std::vector<str> &keys, std::vector<str> &values);

    void put(const str &key, const str &value);
    void put(const char *key, const char *value) {
	put(str(key), str(value));
    }
    void put(long ikey, long ivalue) {
	quick_istr key(ikey), value(ivalue);
	put(key.string(), value.string());
    }
    void put(const str &key, long ivalue) {
	quick_istr value(ivalue);
	put(key, value.string());
    }
    void put_key8(long ikey, long ivalue) {
	quick_istr key(ikey, 8), value(ivalue);
	put(key.string(), value.string());
    }
    void put_key16(long ikey, long ivalue) {
	quick_istr key(ikey, 16), value(ivalue);
	put(key.string(), value.string());
    }
    void put_col(const str &key, int col, const str &value);
    void put_col(long ikey, int col, long ivalue) {
	quick_istr key(ikey), value(ivalue);
	put_col(key.string(), col, value.string());
    }
    void put_col_key10(long ikey, int col, long ivalue) {
	quick_istr key(ikey, 10), value(ivalue);
	put_col(key.string(), col, value.string());
    }

    void remove(const str &key);
    void remove(long ikey) {
	quick_istr key(ikey);
	remove(key.string());
    }
    void remove_key8(long ikey) {
	quick_istr key(ikey, 8);
	remove(key.string());
    }
    void remove_key16(long ikey) {
	quick_istr key(ikey, 16);
	remove(key.string());
    }
    bool remove_sync(const str &key);
    bool remove_sync(long ikey) {
	quick_istr key(ikey);
	return remove_sync(key.string());
    }

    void puts_done() {
    }
    void wait_all() {
    }
    void rcu_quiesce() {
	uint64_t e = timestamp() >> 16;
	if (e != globalepoch)
	    globalepoch = e;
	ti_->rcu_quiesce();
    }
    String make_message(StringAccum &sa) const;
    void notice(const char *fmt, ...);
    void fail(const char *fmt, ...);
    void report(const Json &result) {
	json_.merge(result);
	Json counters;
	for (int i = 0; i < tc_max; ++i)
	    if (uint64_t c = ti_->counter(threadcounter(i)))
		counters.set(threadcounter_names[i], c);
	if (counters)
	    json_.set("counters", counters);
#if MEMSTATS
	json_.set("treesize", ti_->pstat.tree_mem);
#endif
	if (!quiet)
	    fprintf(stderr, "%d: %s\n", ti_->ti_index, json_.unparse().c_str());
    }

    T *table_;
    threadinfo *ti_;
    query<row_type> q_[10];
    kvrandom_lcg_nr rand;
    uint64_t limit_;
    Json json_;
    int ncores_;
    kvout *kvo_;

  private:
    void output_scan(std::vector<str> &keys, std::vector<str> &values) const;
};
static volatile int kvtest_printing;
template <bool CP> struct kvtest_print_helper {};
template <> struct kvtest_print_helper<true> {
    template <typename T> static void print(const T &table, FILE *f, int indent, threadinfo *ti) {
	// only print out the tree from the first failure
	while (!bool_cmpxchg((int *) &kvtest_printing, 0, ti->ti_index + 1))
	    /* spin */;
	table.print(f, indent);
    }
};
template <> struct kvtest_print_helper<false> {
    template <typename T> static void print(const T &, FILE *, int, threadinfo *) {
    }
};
template <typename T> inline void kvtest_print(const T &table, FILE *f, int indent, threadinfo *ti) {
    kvtest_print_helper<table_has_print<T>::value>::print(table, f, indent, ti);
}

template <typename T> inline void kvtest_json_stats(T &table, Json &j, threadinfo *ti) {
    kvtable_helper<table_has_json_stats<T>::value>::json_stats(table, j, ti);
}

template <typename T>
void kvtest_client<T>::get(long ikey) {
    quick_istr key(ikey);
    q_[0].begin_get1(key.string());
    (void) table_->get(q_[0], ti_);
}

template <typename T>
bool kvtest_client<T>::get_sync(const str &key) {
    q_[0].begin_get1(key);
    return table_->get(q_[0], ti_);
}

template <typename T>
bool kvtest_client<T>::get_sync(const str &key, str &value) {
    q_[0].begin_get1(key);
    if (table_->get(q_[0], ti_)) {
	value = q_[0].get1_value();
	return true;
    } else
	return false;
}

template <typename T>
void kvtest_client<T>::get_check(const str &key, const str &expected) {
    q_[0].begin_get1(key);
    if (!table_->get(q_[0], ti_))
	fail("get(%.*s) failed (expected %.*s)\n", key.len, key.s,
	     expected.len, expected.s);
    else {
        str val = q_[0].get1_value();
        if (expected != val)
	    fail("get(%.*s) returned unexpected value %.*s (expected %.*s)\n",
		 key.len, key.s, std::min(val.len, 40), val.s,
		 expected.len, expected.s);
    }
}

template <typename T>
void kvtest_client<T>::get_col_check(const str &key, int col,
				     const str &expected) {
    q_[0].begin_get1(key, col);
    if (!table_->get(q_[0], ti_))
	fail("get.%d(%.*s) failed (expected %.*s)\n",
	     col, key.len, key.s, expected.len, expected.s);
    else {
        str val = q_[0].get1_value();
        if (expected != val)
	    fail("get.%d(%.*s) returned unexpected value %.*s (expected %.*s)\n",
		 col, key.len, key.s, std::min(val.len, 40), val.s,
		 expected.len, expected.s);
    }
}

template <typename T>
void kvtest_client<T>::many_get_check(int nk, long ikey[], long iexpected[]) {
    std::vector<quick_istr> ka(2*nk, quick_istr());
    for(int i = 0; i < nk; i++){
      ka[i].set(ikey[i]);
      ka[i+nk].set(iexpected[i]);
      q_[i].begin_get1(ka[i].string());
    }
    table_->many_get(q_, nk, ti_);
    for(int i = 0; i < nk; i++){
      str val = q_[i].get1_value();
      if (ka[i+nk] != val){
        printf("get(%ld) returned unexpected value %.*s (expected %ld)\n",
             ikey[i], std::min(val.len, 40), val.s, iexpected[i]);
        exit(1);
      }
    }
}

template <typename T>
void kvtest_client<T>::scan_sync(const str &firstkey, int n,
				 std::vector<str> &keys,
				 std::vector<str> &values) {
    if (!kvo_)
	kvo_ = new_kvout(-1, 2048);
    kvout_reset(kvo_);
    q_[0].begin_scan1(firstkey, n, kvo_);
    kvtable_helper<table_has_scan<T>::value>::scan(*table_, q_[0], ti_);
    output_scan(keys, values);
}

template <typename T>
void kvtest_client<T>::rscan_sync(const str &firstkey, int n,
				  std::vector<str> &keys,
				  std::vector<str> &values) {
    if (!kvo_)
	kvo_ = new_kvout(-1, 2048);
    kvout_reset(kvo_);
    q_[0].begin_scan1(firstkey, n, kvo_);
    kvtable_helper<table_has_rscan<T>::value>::rscan(*table_, q_[0], ti_);
    output_scan(keys, values);
}

template <typename T>
void kvtest_client<T>::output_scan(std::vector<str> &keys,
				   std::vector<str> &values) const {
    keys.clear();
    values.clear();
    str key, value;

    kvin kvi;
    kvin_init(&kvi, kvo_->buf, kvo_->n);
    short nfields;
    while (kvcheck(&kvi, 0)) {
	kvread_str_inplace(&kvi, key);
	KVR(&kvi, nfields);
	assert(nfields == 1);
	kvread_str_inplace(&kvi, value);
	keys.push_back(key);
	values.push_back(value);
    }
}

template <typename T>
void kvtest_client<T>::put(const str &key, const str &value) {
    q_[0].begin_put1(key, value);
    table_->put(q_[0], ti_);
}

template <typename T>
void kvtest_client<T>::put_col(const str &key, int col, const str &value) {
#if !KVDB_ROW_TYPE_STR
    if (!kvo_)
	kvo_ = new_kvout(-1, 2048);
    q_[0].begin_put(key, row_type::make_put_col_request(kvo_, col, value));
    table_->put(q_[0], ti_);
#else
    (void) key, (void) col, (void) value;
    assert(0);
#endif
}

template <bool CR> struct kvtest_remove_helper {};
template <> struct kvtest_remove_helper<true> {
    template <typename T> static bool remove(kvtest_client<T> &client, const str &key) {
	client.q_[0].begin_remove(key);
	return client.table_->remove(client.q_[0], client.ti_);
    }
};
template <> struct kvtest_remove_helper<false> {
    template <typename T> static bool remove(kvtest_client<T> &client, const str &key) {
	client.fail("table does not support remove(%.*s)", std::min(key.len, 40), key.s);
	return false;
    }
};
template <typename T> inline bool kvtest_remove(kvtest_client<T> &client, const str &key) {
    return kvtest_remove_helper<table_has_remove<T>::value>::remove(client, key);
}

template <typename T>
void kvtest_client<T>::remove(const str &key) {
    (void) kvtest_remove(*this, key);
}

template <typename T>
bool kvtest_client<T>::remove_sync(const str &key) {
    return kvtest_remove(*this, key);
}

template <typename T>
String kvtest_client<T>::make_message(StringAccum &sa) const {
    const char *begin = sa.begin();
    while (begin != sa.end() && isspace((unsigned char) *begin))
	++begin;
    String s = String(begin, sa.end());
    if (!s.empty() && s.back() != '\n')
	s += '\n';
    return s;
}

template <typename T>
void kvtest_client<T>::notice(const char *fmt, ...) {
    va_list val;
    va_start(val, fmt);
    String m = make_message(StringAccum().vsnprintf(500, fmt, val));
    va_end(val);
    if (m && !quiet)
	fprintf(stderr, "%d: %s", ti_->ti_index, m.c_str());
}

template <typename T>
void kvtest_client<T>::fail(const char *fmt, ...) {
    static spinlock failing_lock = {0};
    static spinlock fail_message_lock = {0};
    static String fail_message;

    va_list val;
    va_start(val, fmt);
    String m = make_message(StringAccum().vsnprintf(500, fmt, val));
    va_end(val);
    if (!m)
	m = "unknown failure";

    acquire(&fail_message_lock);
    if (fail_message != m) {
	fail_message = m;
	fprintf(stderr, "%d: %s", ti_->ti_index, m.c_str());
    }
    release(&fail_message_lock);

    if (table_has_print<T>::value) {
	acquire(&failing_lock);
	fprintf(stdout, "%d: %s", ti_->ti_index, m.c_str());
	kvtest_print(*table_, stdout, 0, ti_);
    }

    mandatory_assert(0);
}


static const char *current_test_name;
static int current_trial;
static FILE *test_output_file;
static pthread_mutex_t subtest_mutex;
static pthread_cond_t subtest_cond;

template <typename T>
struct test_thread {
    test_thread(void *arg, void* table) {
        client_.set_table(( T* )table, (threadinfo *) arg);
	client_.ti_->enter();
	client_.ti_->rcu_start();
    }
    ~test_thread() {
	client_.ti_->rcu_stop();
    }
    void *go(void *arg) {
	/*      //	if (!table_) {
	    table_ = new T;
	    table_->initialize((threadinfo *) arg);
	    //Masstree::default_table::test((threadinfo *) arg);
	    return 0;
	    //	}

        if (!arg) {
	    table_->stats(test_output_file);
            return 0;
        }
#if __linux__
        if (pinthreads) {
            cpu_set_t cs;
            CPU_ZERO(&cs);
            CPU_SET(cores[((threadinfo *)arg)->ti_index], &cs);
	    int r = sched_setaffinity(0, sizeof(cs), &cs);
	    mandatory_assert(r == 0);
        }
#else
        mandatory_assert(!pinthreads && "pinthreads not supported\n");
#endif

	test_thread<T> tt(arg);
	if (fetch_and_add(&active_threads_, 1) == 0)
	    tt.ready_timeouts();
	String test = ::current_test_name;
	int subtestno = 0;
	for (int pos = 0; pos < test.length(); ) {
	    int comma = test.find_left(',', pos);
	    comma = (comma < 0 ? test.length() : comma);
	    String subtest = test.substring(pos, comma - pos), tname;
	    tname = (subtest == test ? subtest : test + String("@") + String(subtestno));
	    tt.client_.reset(tname, ::current_trial);
	    tt.run(subtest);
	    if (comma == test.length())
		break;
	    pthread_mutex_lock(&subtest_mutex);
	    if (fetch_and_add(&active_threads_, -1) == 1) {
		pthread_cond_broadcast(&subtest_cond);
		tt.ready_timeouts();
	    } else
		pthread_cond_wait(&subtest_cond, &subtest_mutex);
	    fprintf(test_output_file, "%s\n", tt.client_.json_.unparse().c_str());
	    pthread_mutex_unlock(&subtest_mutex);
	    fetch_and_add(&active_threads_, 1);
	    pos = comma + 1;
	    ++subtestno;
	}
	int at = fetch_and_add(&active_threads_, -1);
		if (at == 1 && print_table)
	    kvtest_print(*table_, stdout, 0, tt.client_.ti_);
	if (at == 1 && json_stats) {
	    Json j;
	    kvtest_json_stats(*table_, j, tt.client_.ti_);
	    if (j) {
		fprintf(stderr, "%s\n", j.unparse(Json::indent_depth(4).tab_width(2), true).c_str());
		tt.client_.json_.merge(j);
	    }
	}
	fprintf(test_output_file, "%s\n", tt.client_.json_.unparse().c_str()); */
	return 0;
    }
    void ready_timeouts() {
	for (size_t i = 0; i < arraysize(timeout); ++i)
	    timeout[i] = false;
	if (!lazy_timer && duration[0])
	    xalarm(duration[0]);
    }
    void run(const String &test) {
	if (test == "rw1")
	    kvtest_rw1(client_);
	else
	    client_.fail("unknown test %s", test.c_str());
    }
    T *table_;
  //    static unsigned active_threads_;
    kvtest_client<T> client_;
};
//template <typename T> T *test_thread<T>::table_;
//template <typename T> unsigned test_thread<T>::active_threads_;

void runtest(int nthreads, void *(*func)(void *)) {
    std::vector<threadinfo *> tis;
    for (int i = 0; i < nthreads; ++i)
	tis.push_back(threadinfo::make(threadinfo::TI_PROCESS, i));
    signal(SIGALRM, test_timeout);
    for (int i = 0; i < nthreads; ++i) {
	int r = pthread_create(&tis[i]->ti_threadid, 0, func, tis[i]);
	mandatory_assert(r == 0);
    }
    for (int i = 0; i < nthreads; ++i)
	pthread_join(tis[i]->ti_threadid, 0);
}




//============================================================================//
extern "C"{
  /* Exported Structures */
  struct masstree_t{
    Masstree::default_table *table;
    threadinfo *main_ti;
    std::vector<threadinfo *> tis;
    int max_clients;
    int registered_clients;
  };

  struct masstree_client_t{
    kvtest_client<Masstree::default_table> client;
    masstree_t* parent;
    int client_no;
  };
    
  struct masstree_client_trans_t{
    int instruction_count;
  };

//============================================================================//  
/* Exported C Functions */
  /* Basic start/stop functions */
  int masstree_table_open(masstree_t **mt, int num_threads,
			  bool use_checkpoint, bool use_logging){
    // Gathered from main() in mttest.cc
    threadcounter_names[(int) tc_root_retry] = "root_retry";
    threadcounter_names[(int) tc_internode_retry] = "internode_retry";
    threadcounter_names[(int) tc_leaf_retry] = "leaf_retry";
    threadcounter_names[(int) tc_leaf_walk] = "leaf_walk";
    threadcounter_names[(int) tc_stable_internode_insert] = "stable_internode_insert";
    threadcounter_names[(int) tc_stable_internode_split] = "stable_internode_split";
    threadcounter_names[(int) tc_stable_leaf_insert] = "stable_leaf_insert";
    threadcounter_names[(int) tc_stable_leaf_split] = "stable_leaf_split";
    threadcounter_names[(int) tc_internode_lock] = "internode_lock_retry";
    threadcounter_names[(int) tc_leaf_lock] = "leaf_lock_retry";

    //Setup parameters
    //int normtype = normtype_pertest, 
    int ret, firstcore = -1, corestride = 1;
    std::vector<const char *> tests, treetypes;
    std::vector<String> comparisons;
    tcpthreads = udpthreads = sysconf(_SC_NPROCESSORS_ONLN);
    
    //OMIT: CLP parser

    //Setup cores and NUMA
    if (firstcore < 0)
        firstcore = cores.size() ? cores.back() + 1 : 0;
    for (; (int) cores.size() < udpthreads; firstcore += corestride)
        cores.push_back(firstcore);

#if PMC_ENABLED
    mandatory_assert(pinthreads && "Using performance counter requires pinning threads to cores!");
#endif
#if MEMSTATS && HAVE_NUMA_H && HAVE_LIBNUMA
    if (numa_available() != -1) {
	mandatory_assert(numa_max_node() <= MaxNumaNode);
	for (int i = 0; i <= numa_max_node(); i++)
	    numa[i].size = numa_node_size64(i, &numa[i].free);
    }
#endif
    
    //OMIT treetypes and test arrays

    // arrange for a per-thread threadinfo pointer
    ret = pthread_key_create(&threadinfo::key, 0);
    mandatory_assert(ret == 0);
    pthread_mutex_init(&subtest_mutex, 0);
    pthread_cond_init(&subtest_cond, 0);

    // pipe for them to write back to us
    int p[2];
    ret = pipe(p);
    mandatory_assert(ret == 0);
    test_output_file = fdopen(p[1], "w");

    //OMIT collector setup. Possibly okay to remove the pipe, too?
    initial_timestamp = timestamp();

    //Pulled from run_one_test_body() in mttest
    threadinfo *main_ti = threadinfo::make(threadinfo::TI_MAIN, -1);
    main_ti->enter();
    globalepoch = timestamp() >> 16;
    current_test_name = "test";
    current_trial = 0;
    Masstree::default_table *table = new Masstree::default_table;
    table->initialize(main_ti);
    test_thread<Masstree::default_table> tt(main_ti, table);

    //Pulled from runtest()
    // modified to not spawn threads, instead create the stuff here
    // and store the ids when they register
    std:: vector<threadinfo *> tis;
    for (int i = 0; i < num_threads; ++i)
      tis.push_back(threadinfo::make(threadinfo::TI_PROCESS, i));
    signal(SIGALRM, test_timeout);

    //Allocate and fill in the structure to allow multiple calls
    masstree_t *new_mt = (masstree_t *) malloc(sizeof(masstree_t));
    new_mt->table = table;
    new_mt->main_ti = main_ti;
    new_mt->tis = tis;
    new_mt->max_clients = num_threads;
    new_mt->registered_clients = 0;
    
    *mt = new_mt;
    return 0;
  }
  int masstree_client_start(masstree_t *mt, masstree_client_t **mtc){
    //assumes that pthread_create has already been called
    //register the thread in masstree_t and return a masstree_client_t
    if(mt->registered_clients == mt->max_clients)
      return -1;
    int i = mt->registered_clients;
    mt->registered_clients++;

    pthread_t r = pthread_self();
    mt->tis[i]->ti_threadid = r;

    //Do the relevant part of ::go
    void* arg = mt->tis[i];
#if __linux__
    if (pinthreads){
      cpu_set_t cs;
      CPU_ZERO(&cs);
      CPU_SET(cores[((threadinfo *)arg)->ti_index], &cs);
      int r = sched_setaffinity(0, sizeof(cs), &cs);
      mandatory_assert(r == 0);
    }
#else
    mandatory_assert(!pinthreads && "pinthreads not supported\n");
#endif
    
    test_thread<Masstree::default_table> tt(arg, mt->table);
    //OMIT find tests
    tt.client_.reset("test", 1);
    //This is where we'd run tests

    //Pulled from kvtest_sync()
    int seed = 31949+tt.client_.id()%48;
    tt.client_.rand.reset(seed);
    
    
    //Allocate and return structure
    masstree_client_t* new_mtc = (masstree_client_t*)malloc(sizeof(masstree_client_t));
    new_mtc->client = tt.client_;
    new_mtc->parent = mt;
    new_mtc->client_no = i;
    *mtc = new_mtc;

    return 0;
  }
  int masstree_client_stop(masstree_client_t *mtc){
    // assumes that someone else (caller) will join on the thread
    //necessary?
    //    pthread_mutex_lock(&subtest_mutex);
    //    if(fetch_and_add(&active_threads_, -1) == 1) {
    //      pthread_cond_broadcast(&subtest_cond);
    //      tt.ready_timeouts();
    //    } else
    //      pthread_cond_wait(&subtest_cond, &subtest_mutex);
    //    pthread_mutex_unlock(&subtest_mutex);
    //    fetch_and_add(&active_threads_, 1);
    //OMIT Labook and Json output

    // Structure crud
    masstree_t *mt = mtc->parent;
    mt->tis[mtc->client_no]->ti_threadid = -1;
    mtc->client.reset("test", 0);
    free(mtc);
    mt->registered_clients --;
    return 0;
  }
  int masstree_table_close(masstree_t *mt){
    //    assert(mt->registered_clients == 0);

    //Pulled from the end of main()
    //Is this strictly necessary??
    struct timeval delay;
    delay.tv_sec = 0;
    delay.tv_usec = 250000;
    (void) select(0, 0, 0, 0, &delay);
    
    if (quiet)
      fprintf(stderr, "\r%60s\r", "");
    fclose(test_output_file);
    //    pthread_join(collector, 0);

    //OMIT labnotebook and gnuplot
    delete(mt->table);
    free(mt);
    return 0;
  }
  
  /* Unimplemented: transactions */
  int masstree_client_start_trans(masstree_client_t *mtc, 
				  masstree_client_trans_t **trans){
    printf("*** unimplemented function ***\n");
    return 0;
  }
  int masstree_client_end_trans(masstree_client_t *mtc,
				masstree_client_trans_t **trans){
    printf("*** unimplemented function ***\n");
    return 0;
  }
  
  /* Operations on store */
  int masstree_client_put(masstree_client_t *mtc, 
			  const char *key, const char *value){
    mtc->client.put(key, value);
    return 0;
  }
  int masstree_client_get(masstree_client_t *mtc, 
			  const char *key, const char **value, size_t size){
    str ivalue;
    mtc->client.get_sync(key, ivalue);

    const char *ivalue_ptr =  ivalue.data();
    /*    if(ivalue_ptr != NULL){
      char *copied_value = (char*) malloc(size+1);
      strncpy(copied_value, ivalue_ptr, size);
      *value = copied_value;
    }
    else
    *value = NULL; */
    *value = ivalue_ptr;
    return 0;
  }
  int masstree_client_delete(masstree_client_t *mtc, const char *key){
    mtc->client.remove(key);
    return 0;
  }
  
  void *masstree_verify_thread(void* arg){
    masstree_t *mt = (masstree_t *) arg;
    masstree_client_t *mtc;
    int rc = 0;
    printf("\tyou created a thread! go team!\n");

    rc = masstree_client_start(mt, &mtc);
    assert(rc == 0);
    printf("\tand client registration worked\n");

    printf("Putting to the Masstree clients \n");
    masstree_client_put(mtc, "key0", "all");
    masstree_client_put(mtc, "key1", "piglets");
    masstree_client_put(mtc, "key2", "run");
    masstree_client_put(mtc, "key3", "speedily");
    printf("\t...done\n");

    printf("Getting and checking gets\n");
    const char* value = (char *)malloc(16);
    masstree_client_get(mtc, "key0", &value, (size_t) 4);
    printf("\t\t retrieved (key0, %s) expecting (key0, %s)\n",
	   value, "all");
    masstree_client_get(mtc, "key1", &value, (size_t) 8);
    printf("\t\t retrieved (key1, %s) expecting (key1, %s)\n",
	   value, "piglets");
    masstree_client_get(mtc, "key2", &value, (size_t) 4);
    printf("\t\t retrieved (key2, %s) expecting (key2, %s)\n",
	   value, "run");
    printf("\t...done\n");

    printf("Trying to delete a couple keys\n");
    masstree_client_delete(mtc, "key3");
    masstree_client_get(mtc, "key3", &value, (size_t) 10);
    if(value == NULL)
      printf("\t\t retrieved null results, as expected\n");
    else
      printf("\t\t retrieved (key3, %s) expecting deleted key\n",
	     value);

    rc = masstree_client_stop(mtc);
    assert(rc == 0);
    printf("\tso did client murder\n");

    return NULL;
  }
  /* Test the functionality */
  int masstree_verify_bindings(void){
    int num_threads = 2;
    int rc = 0;
    masstree_t *mt;
    pthread_t threadids[num_threads];

    printf("Creating Masstree main with no log, no checkpoint...\n");
    rc = masstree_table_open(&mt, num_threads, false, false);
    assert(rc == 0);
    printf("\t...done\n");

    printf("Creating %d Masstree client threads...\n", num_threads);
    for( int i = 0; i< num_threads; i++){
      rc = pthread_create(&threadids[i], 0, 
			  masstree_verify_thread, (void*) mt);
      assert(rc == 0);
    }
    printf("\t...done\n");

    
    printf("Destroying %d Masstree client threads...\n", num_threads);
    for(int i = 0; i < num_threads; i++){
      pthread_join(threadids[i],0);
    }
    printf("\t...done\n");

    printf("Destroying Masstree main...\n");
    rc = masstree_table_close(mt);
    assert(rc == 0);
    printf("\t...done\n");    

    printf("Verification Complete\n");
    printf("\n\n\n");

    return 0;
  }
}
