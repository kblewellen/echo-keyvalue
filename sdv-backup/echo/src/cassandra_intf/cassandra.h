/**
 * Autogenerated by Thrift Compiler (0.7.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 */
#ifndef CASSANDRA_H
#define CASSANDRA_H

#include "cassandra_types.h"

/* Cassandra service interface */
typedef struct _CassandraIf CassandraIf;  /* dummy object */

struct _CassandraIfInterface
{
	GTypeInterface parent;

	gboolean (*login) (CassandraIf *iface, const AuthenticationRequest * auth_request, AuthenticationException ** authnx, AuthorizationException ** authzx, GError **error);
	gboolean (*set_keyspace) (CassandraIf *iface, const gchar * keyspace, InvalidRequestException ** ire, GError **error);
	gboolean (*get) (CassandraIf *iface, ColumnOrSuperColumn ** _return, const GByteArray * key, const ColumnPath * column_path, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, NotFoundException ** nfe, UnavailableException ** ue, TimedOutException ** te, GError **error);
	gboolean (*get_slice) (CassandraIf *iface, GPtrArray ** _return, const GByteArray * key, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
	gboolean (*get_count) (CassandraIf *iface, gint32* _return, const GByteArray * key, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
	gboolean (*multiget_slice) (CassandraIf *iface, GHashTable ** _return, const GPtrArray * keys, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
	gboolean (*multiget_count) (CassandraIf *iface, GHashTable ** _return, const GPtrArray * keys, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
	gboolean (*get_range_slices) (CassandraIf *iface, GPtrArray ** _return, const ColumnParent * column_parent, const SlicePredicate * predicate, const KeyRange * range, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
	gboolean (*get_indexed_slices) (CassandraIf *iface, GPtrArray ** _return, const ColumnParent * column_parent, const IndexClause * index_clause, const SlicePredicate * column_predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
	gboolean (*insert) (CassandraIf *iface, const GByteArray * key, const ColumnParent * column_parent, const Column * column, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
	gboolean (*add) (CassandraIf *iface, const GByteArray * key, const ColumnParent * column_parent, const CounterColumn * column, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
	gboolean (*remove) (CassandraIf *iface, const GByteArray * key, const ColumnPath * column_path, const gint64 timestamp, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
	gboolean (*remove_counter) (CassandraIf *iface, const GByteArray * key, const ColumnPath * path, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
	gboolean (*batch_mutate) (CassandraIf *iface, const GHashTable * mutation_map, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
	gboolean (*truncate) (CassandraIf *iface, const gchar * cfname, InvalidRequestException ** ire, UnavailableException ** ue, GError **error);
	gboolean (*describe_schema_versions) (CassandraIf *iface, GHashTable ** _return, InvalidRequestException ** ire, GError **error);
	gboolean (*describe_keyspaces) (CassandraIf *iface, GPtrArray ** _return, InvalidRequestException ** ire, GError **error);
	gboolean (*describe_cluster_name) (CassandraIf *iface, gchar ** _return, GError **error);
	gboolean (*describe_version) (CassandraIf *iface, gchar ** _return, GError **error);
	gboolean (*describe_ring) (CassandraIf *iface, GPtrArray ** _return, const gchar * keyspace, InvalidRequestException ** ire, GError **error);
	gboolean (*describe_partitioner) (CassandraIf *iface, gchar ** _return, GError **error);
	gboolean (*describe_snitch) (CassandraIf *iface, gchar ** _return, GError **error);
	gboolean (*describe_keyspace) (CassandraIf *iface, KsDef ** _return, const gchar * keyspace, NotFoundException ** nfe, InvalidRequestException ** ire, GError **error);
	gboolean (*describe_splits) (CassandraIf *iface, GPtrArray ** _return, const gchar * cfName, const gchar * start_token, const gchar * end_token, const gint32 keys_per_split, InvalidRequestException ** ire, GError **error);
	gboolean (*system_add_column_family) (CassandraIf *iface, gchar ** _return, const CfDef * cf_def, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError **error);
	gboolean (*system_drop_column_family) (CassandraIf *iface, gchar ** _return, const gchar * column_family, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError **error);
	gboolean (*system_add_keyspace) (CassandraIf *iface, gchar ** _return, const KsDef * ks_def, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError **error);
	gboolean (*system_drop_keyspace) (CassandraIf *iface, gchar ** _return, const gchar * keyspace, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError **error);
	gboolean (*system_update_keyspace) (CassandraIf *iface, gchar ** _return, const KsDef * ks_def, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError **error);
	gboolean (*system_update_column_family) (CassandraIf *iface, gchar ** _return, const CfDef * cf_def, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError **error);
	gboolean (*execute_cql_query) (CassandraIf *iface, CqlResult ** _return, const GByteArray * query, const Compression compression, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, SchemaDisagreementException ** sde, GError **error);
};
typedef struct _CassandraIfInterface CassandraIfInterface;

GType cassandra_if_get_type (void);
#define TYPE_CASSANDRA_IF (cassandra_if_get_type())
#define CASSANDRA_IF(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_CASSANDRA_IF, CassandraIf))
#define IS_CASSANDRA_IF(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj, TYPE_CASSANDRA_IF))
#define CASSANDRA_IF_GET_INTERFACE(inst) (G_TYPE_INSTANCE_GET_INTERFACE ((inst), TYPE_CASSANDRA_IF, CassandraIfInterface))

		gboolean cassandra_if_login (CassandraIf *iface, const AuthenticationRequest * auth_request, AuthenticationException ** authnx, AuthorizationException ** authzx, GError **error);
		gboolean cassandra_if_set_keyspace (CassandraIf *iface, const gchar * keyspace, InvalidRequestException ** ire, GError **error);
		gboolean cassandra_if_get (CassandraIf *iface, ColumnOrSuperColumn ** _return, const GByteArray * key, const ColumnPath * column_path, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, NotFoundException ** nfe, UnavailableException ** ue, TimedOutException ** te, GError **error);
		gboolean cassandra_if_get_slice (CassandraIf *iface, GPtrArray ** _return, const GByteArray * key, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
		gboolean cassandra_if_get_count (CassandraIf *iface, gint32* _return, const GByteArray * key, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
		gboolean cassandra_if_multiget_slice (CassandraIf *iface, GHashTable ** _return, const GPtrArray * keys, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
		gboolean cassandra_if_multiget_count (CassandraIf *iface, GHashTable ** _return, const GPtrArray * keys, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
		gboolean cassandra_if_get_range_slices (CassandraIf *iface, GPtrArray ** _return, const ColumnParent * column_parent, const SlicePredicate * predicate, const KeyRange * range, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
		gboolean cassandra_if_get_indexed_slices (CassandraIf *iface, GPtrArray ** _return, const ColumnParent * column_parent, const IndexClause * index_clause, const SlicePredicate * column_predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
		gboolean cassandra_if_insert (CassandraIf *iface, const GByteArray * key, const ColumnParent * column_parent, const Column * column, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
		gboolean cassandra_if_add (CassandraIf *iface, const GByteArray * key, const ColumnParent * column_parent, const CounterColumn * column, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
		gboolean cassandra_if_remove (CassandraIf *iface, const GByteArray * key, const ColumnPath * column_path, const gint64 timestamp, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
		gboolean cassandra_if_remove_counter (CassandraIf *iface, const GByteArray * key, const ColumnPath * path, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
		gboolean cassandra_if_batch_mutate (CassandraIf *iface, const GHashTable * mutation_map, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError **error);
		gboolean cassandra_if_truncate (CassandraIf *iface, const gchar * cfname, InvalidRequestException ** ire, UnavailableException ** ue, GError **error);
		gboolean cassandra_if_describe_schema_versions (CassandraIf *iface, GHashTable ** _return, InvalidRequestException ** ire, GError **error);
		gboolean cassandra_if_describe_keyspaces (CassandraIf *iface, GPtrArray ** _return, InvalidRequestException ** ire, GError **error);
		gboolean cassandra_if_describe_cluster_name (CassandraIf *iface, gchar ** _return, GError **error);
gboolean cassandra_if_describe_version (CassandraIf *iface, gchar ** _return, GError **error);
gboolean cassandra_if_describe_ring (CassandraIf *iface, GPtrArray ** _return, const gchar * keyspace, InvalidRequestException ** ire, GError **error);
gboolean cassandra_if_describe_partitioner (CassandraIf *iface, gchar ** _return, GError **error);
gboolean cassandra_if_describe_snitch (CassandraIf *iface, gchar ** _return, GError **error);
gboolean cassandra_if_describe_keyspace (CassandraIf *iface, KsDef ** _return, const gchar * keyspace, NotFoundException ** nfe, InvalidRequestException ** ire, GError **error);
gboolean cassandra_if_describe_splits (CassandraIf *iface, GPtrArray ** _return, const gchar * cfName, const gchar * start_token, const gchar * end_token, const gint32 keys_per_split, InvalidRequestException ** ire, GError **error);
gboolean cassandra_if_system_add_column_family (CassandraIf *iface, gchar ** _return, const CfDef * cf_def, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError **error);
gboolean cassandra_if_system_drop_column_family (CassandraIf *iface, gchar ** _return, const gchar * column_family, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError **error);
gboolean cassandra_if_system_add_keyspace (CassandraIf *iface, gchar ** _return, const KsDef * ks_def, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError **error);
gboolean cassandra_if_system_drop_keyspace (CassandraIf *iface, gchar ** _return, const gchar * keyspace, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError **error);
gboolean cassandra_if_system_update_keyspace (CassandraIf *iface, gchar ** _return, const KsDef * ks_def, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError **error);
gboolean cassandra_if_system_update_column_family (CassandraIf *iface, gchar ** _return, const CfDef * cf_def, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError **error);
gboolean cassandra_if_execute_cql_query (CassandraIf *iface, CqlResult ** _return, const GByteArray * query, const Compression compression, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, SchemaDisagreementException ** sde, GError **error);

/* Cassandra service client */
struct _CassandraClient
{
	GObject parent;

	ThriftProtocol *input_protocol;
	ThriftProtocol *output_protocol;
};
typedef struct _CassandraClient CassandraClient;

struct _CassandraClientClass
{
	GObjectClass parent;
};
typedef struct _CassandraClientClass CassandraClientClass;

GType cassandra_client_get_type (void);
#define TYPE_CASSANDRA_CLIENT (cassandra_client_get_type())
#define CASSANDRA_CLIENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_CASSANDRA_CLIENT, CassandraClient))
#define CASSANDRA_CLIENT_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), TYPE_CASSANDRA_CLIENT, CassandraClientClass))
#define CASSANDRA_IS_CLIENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_CASSANDRA_CLIENT))
#define CASSANDRA_IS_CLIENT_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), TYPE_CASSANDRA_CLIENT))
#define CASSANDRA_CLIENT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_CASSANDRA_CLIENT, CassandraClientClass))

gboolean cassandra_client_login (CassandraIf * iface, const AuthenticationRequest * auth_request, AuthenticationException ** authnx, AuthorizationException ** authzx, GError ** error);
gboolean cassandra_client_send_login (CassandraIf * iface, const AuthenticationRequest * auth_request, GError ** error);
gboolean cassandra_client_recv_login (CassandraIf * iface, AuthenticationException ** authnx, AuthorizationException ** authzx, GError ** error);
gboolean cassandra_client_set_keyspace (CassandraIf * iface, const gchar * keyspace, InvalidRequestException ** ire, GError ** error);
gboolean cassandra_client_send_set_keyspace (CassandraIf * iface, const gchar * keyspace, GError ** error);
gboolean cassandra_client_recv_set_keyspace (CassandraIf * iface, InvalidRequestException ** ire, GError ** error);
gboolean cassandra_client_get (CassandraIf * iface, ColumnOrSuperColumn ** _return, const GByteArray * key, const ColumnPath * column_path, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, NotFoundException ** nfe, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_send_get (CassandraIf * iface, const GByteArray * key, const ColumnPath * column_path, const ConsistencyLevel consistency_level, GError ** error);
gboolean cassandra_client_recv_get (CassandraIf * iface, ColumnOrSuperColumn ** _return, InvalidRequestException ** ire, NotFoundException ** nfe, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_get_slice (CassandraIf * iface, GPtrArray ** _return, const GByteArray * key, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_send_get_slice (CassandraIf * iface, const GByteArray * key, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, GError ** error);
gboolean cassandra_client_recv_get_slice (CassandraIf * iface, GPtrArray ** _return, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_get_count (CassandraIf * iface, gint32* _return, const GByteArray * key, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_send_get_count (CassandraIf * iface, const GByteArray * key, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, GError ** error);
gboolean cassandra_client_recv_get_count (CassandraIf * iface, gint32* _return, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_multiget_slice (CassandraIf * iface, GHashTable ** _return, const GPtrArray * keys, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_send_multiget_slice (CassandraIf * iface, const GPtrArray * keys, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, GError ** error);
gboolean cassandra_client_recv_multiget_slice (CassandraIf * iface, GHashTable ** _return, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_multiget_count (CassandraIf * iface, GHashTable ** _return, const GPtrArray * keys, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_send_multiget_count (CassandraIf * iface, const GPtrArray * keys, const ColumnParent * column_parent, const SlicePredicate * predicate, const ConsistencyLevel consistency_level, GError ** error);
gboolean cassandra_client_recv_multiget_count (CassandraIf * iface, GHashTable ** _return, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_get_range_slices (CassandraIf * iface, GPtrArray ** _return, const ColumnParent * column_parent, const SlicePredicate * predicate, const KeyRange * range, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_send_get_range_slices (CassandraIf * iface, const ColumnParent * column_parent, const SlicePredicate * predicate, const KeyRange * range, const ConsistencyLevel consistency_level, GError ** error);
gboolean cassandra_client_recv_get_range_slices (CassandraIf * iface, GPtrArray ** _return, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_get_indexed_slices (CassandraIf * iface, GPtrArray ** _return, const ColumnParent * column_parent, const IndexClause * index_clause, const SlicePredicate * column_predicate, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_send_get_indexed_slices (CassandraIf * iface, const ColumnParent * column_parent, const IndexClause * index_clause, const SlicePredicate * column_predicate, const ConsistencyLevel consistency_level, GError ** error);
gboolean cassandra_client_recv_get_indexed_slices (CassandraIf * iface, GPtrArray ** _return, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_insert (CassandraIf * iface, const GByteArray * key, const ColumnParent * column_parent, const Column * column, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_send_insert (CassandraIf * iface, const GByteArray * key, const ColumnParent * column_parent, const Column * column, const ConsistencyLevel consistency_level, GError ** error);
gboolean cassandra_client_recv_insert (CassandraIf * iface, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_add (CassandraIf * iface, const GByteArray * key, const ColumnParent * column_parent, const CounterColumn * column, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_send_add (CassandraIf * iface, const GByteArray * key, const ColumnParent * column_parent, const CounterColumn * column, const ConsistencyLevel consistency_level, GError ** error);
gboolean cassandra_client_recv_add (CassandraIf * iface, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_remove (CassandraIf * iface, const GByteArray * key, const ColumnPath * column_path, const gint64 timestamp, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_send_remove (CassandraIf * iface, const GByteArray * key, const ColumnPath * column_path, const gint64 timestamp, const ConsistencyLevel consistency_level, GError ** error);
gboolean cassandra_client_recv_remove (CassandraIf * iface, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_remove_counter (CassandraIf * iface, const GByteArray * key, const ColumnPath * path, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_send_remove_counter (CassandraIf * iface, const GByteArray * key, const ColumnPath * path, const ConsistencyLevel consistency_level, GError ** error);
gboolean cassandra_client_recv_remove_counter (CassandraIf * iface, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_batch_mutate (CassandraIf * iface, const GHashTable * mutation_map, const ConsistencyLevel consistency_level, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_send_batch_mutate (CassandraIf * iface, const GHashTable * mutation_map, const ConsistencyLevel consistency_level, GError ** error);
gboolean cassandra_client_recv_batch_mutate (CassandraIf * iface, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, GError ** error);
gboolean cassandra_client_truncate (CassandraIf * iface, const gchar * cfname, InvalidRequestException ** ire, UnavailableException ** ue, GError ** error);
gboolean cassandra_client_send_truncate (CassandraIf * iface, const gchar * cfname, GError ** error);
gboolean cassandra_client_recv_truncate (CassandraIf * iface, InvalidRequestException ** ire, UnavailableException ** ue, GError ** error);
gboolean cassandra_client_describe_schema_versions (CassandraIf * iface, GHashTable ** _return, InvalidRequestException ** ire, GError ** error);
gboolean cassandra_client_send_describe_schema_versions (CassandraIf * iface, GError ** error);
gboolean cassandra_client_recv_describe_schema_versions (CassandraIf * iface, GHashTable ** _return, InvalidRequestException ** ire, GError ** error);
gboolean cassandra_client_describe_keyspaces (CassandraIf * iface, GPtrArray ** _return, InvalidRequestException ** ire, GError ** error);
gboolean cassandra_client_send_describe_keyspaces (CassandraIf * iface, GError ** error);
gboolean cassandra_client_recv_describe_keyspaces (CassandraIf * iface, GPtrArray ** _return, InvalidRequestException ** ire, GError ** error);
gboolean cassandra_client_describe_cluster_name (CassandraIf * iface, gchar ** _return, GError ** error);
gboolean cassandra_client_send_describe_cluster_name (CassandraIf * iface, GError ** error);
gboolean cassandra_client_recv_describe_cluster_name (CassandraIf * iface, gchar ** _return, GError ** error);
gboolean cassandra_client_describe_version (CassandraIf * iface, gchar ** _return, GError ** error);
gboolean cassandra_client_send_describe_version (CassandraIf * iface, GError ** error);
gboolean cassandra_client_recv_describe_version (CassandraIf * iface, gchar ** _return, GError ** error);
gboolean cassandra_client_describe_ring (CassandraIf * iface, GPtrArray ** _return, const gchar * keyspace, InvalidRequestException ** ire, GError ** error);
gboolean cassandra_client_send_describe_ring (CassandraIf * iface, const gchar * keyspace, GError ** error);
gboolean cassandra_client_recv_describe_ring (CassandraIf * iface, GPtrArray ** _return, InvalidRequestException ** ire, GError ** error);
gboolean cassandra_client_describe_partitioner (CassandraIf * iface, gchar ** _return, GError ** error);
gboolean cassandra_client_send_describe_partitioner (CassandraIf * iface, GError ** error);
gboolean cassandra_client_recv_describe_partitioner (CassandraIf * iface, gchar ** _return, GError ** error);
gboolean cassandra_client_describe_snitch (CassandraIf * iface, gchar ** _return, GError ** error);
gboolean cassandra_client_send_describe_snitch (CassandraIf * iface, GError ** error);
gboolean cassandra_client_recv_describe_snitch (CassandraIf * iface, gchar ** _return, GError ** error);
gboolean cassandra_client_describe_keyspace (CassandraIf * iface, KsDef ** _return, const gchar * keyspace, NotFoundException ** nfe, InvalidRequestException ** ire, GError ** error);
gboolean cassandra_client_send_describe_keyspace (CassandraIf * iface, const gchar * keyspace, GError ** error);
gboolean cassandra_client_recv_describe_keyspace (CassandraIf * iface, KsDef ** _return, NotFoundException ** nfe, InvalidRequestException ** ire, GError ** error);
gboolean cassandra_client_describe_splits (CassandraIf * iface, GPtrArray ** _return, const gchar * cfName, const gchar * start_token, const gchar * end_token, const gint32 keys_per_split, InvalidRequestException ** ire, GError ** error);
gboolean cassandra_client_send_describe_splits (CassandraIf * iface, const gchar * cfName, const gchar * start_token, const gchar * end_token, const gint32 keys_per_split, GError ** error);
gboolean cassandra_client_recv_describe_splits (CassandraIf * iface, GPtrArray ** _return, InvalidRequestException ** ire, GError ** error);
gboolean cassandra_client_system_add_column_family (CassandraIf * iface, gchar ** _return, const CfDef * cf_def, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError ** error);
gboolean cassandra_client_send_system_add_column_family (CassandraIf * iface, const CfDef * cf_def, GError ** error);
gboolean cassandra_client_recv_system_add_column_family (CassandraIf * iface, gchar ** _return, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError ** error);
gboolean cassandra_client_system_drop_column_family (CassandraIf * iface, gchar ** _return, const gchar * column_family, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError ** error);
gboolean cassandra_client_send_system_drop_column_family (CassandraIf * iface, const gchar * column_family, GError ** error);
gboolean cassandra_client_recv_system_drop_column_family (CassandraIf * iface, gchar ** _return, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError ** error);
gboolean cassandra_client_system_add_keyspace (CassandraIf * iface, gchar ** _return, const KsDef * ks_def, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError ** error);
gboolean cassandra_client_send_system_add_keyspace (CassandraIf * iface, const KsDef * ks_def, GError ** error);
gboolean cassandra_client_recv_system_add_keyspace (CassandraIf * iface, gchar ** _return, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError ** error);
gboolean cassandra_client_system_drop_keyspace (CassandraIf * iface, gchar ** _return, const gchar * keyspace, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError ** error);
gboolean cassandra_client_send_system_drop_keyspace (CassandraIf * iface, const gchar * keyspace, GError ** error);
gboolean cassandra_client_recv_system_drop_keyspace (CassandraIf * iface, gchar ** _return, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError ** error);
gboolean cassandra_client_system_update_keyspace (CassandraIf * iface, gchar ** _return, const KsDef * ks_def, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError ** error);
gboolean cassandra_client_send_system_update_keyspace (CassandraIf * iface, const KsDef * ks_def, GError ** error);
gboolean cassandra_client_recv_system_update_keyspace (CassandraIf * iface, gchar ** _return, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError ** error);
gboolean cassandra_client_system_update_column_family (CassandraIf * iface, gchar ** _return, const CfDef * cf_def, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError ** error);
gboolean cassandra_client_send_system_update_column_family (CassandraIf * iface, const CfDef * cf_def, GError ** error);
gboolean cassandra_client_recv_system_update_column_family (CassandraIf * iface, gchar ** _return, InvalidRequestException ** ire, SchemaDisagreementException ** sde, GError ** error);
gboolean cassandra_client_execute_cql_query (CassandraIf * iface, CqlResult ** _return, const GByteArray * query, const Compression compression, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, SchemaDisagreementException ** sde, GError ** error);
gboolean cassandra_client_send_execute_cql_query (CassandraIf * iface, const GByteArray * query, const Compression compression, GError ** error);
gboolean cassandra_client_recv_execute_cql_query (CassandraIf * iface, CqlResult ** _return, InvalidRequestException ** ire, UnavailableException ** ue, TimedOutException ** te, SchemaDisagreementException ** sde, GError ** error);

#endif /* CASSANDRA_H */
