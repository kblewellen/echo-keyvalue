PLATFORM_CFLAGS=-pthread -DOS_LINUX
PLATFORM_LDFLAGS=-L../malloc/lib -lpthread -ltcmalloc_minimal
PLATFORM=OS_LINUX
SNAPPY=0
PORT_CFLAGS=-fno-builtin-memcmp -DLEVELDB_PLATFORM_POSIX
