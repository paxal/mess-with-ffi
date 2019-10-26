typedef unsigned char zend_bool;
typedef unsigned char zend_uchar;
typedef struct _zend_class_entry     zend_class_entry;

typedef unsigned int uint32;
typedef uint32 uint32_t;

typedef unsigned long uint64;
typedef uint64 uint64_t;

typedef struct _zend_function zend_function;
typedef struct _zend_class_iterator_funcs zend_class_iterator_funcs;
typedef struct _zend_object_iterator zend_object_iterator;
typedef struct _zend_serialize_data zend_serialize_data;
typedef struct _zend_unserialize_data zend_unserialize_data;
typedef struct _zend_class_name zend_class_name;
typedef struct _zend_executor_globals zend_executor_globals;
typedef struct _zend_trait_alias zend_trait_alias;
typedef struct _zend_trait_precedence zend_trait_precedence;
typedef struct _zval_struct     zval;
typedef struct _zend_string     zend_string;
typedef struct _zend_execute_data    zend_execute_data;

typedef struct _zend_vm_stack *zend_vm_stack;
struct _zend_vm_stack {
	zval *top;
	zval *end;
	zend_vm_stack prev;
};

typedef struct _zend_object     zend_object;
typedef uint64_t zend_long;
typedef uint64_t zend_ulong;
typedef void (*dtor_func_t)(zval *pDest);

typedef struct _zend_refcounted_h {
	uint32_t         refcount;			/* reference counter 32-bit */
	union {
		uint32_t type_info;
	} u;
} zend_refcounted_h;

typedef struct _Bucket Bucket;

struct _zend_array {
	zend_refcounted_h gc;
#	union {
#		struct {
#			ZEND_ENDIAN_LOHI_4(
#				zend_uchar    flags,
#				zend_uchar    _unused,
#				zend_uchar    nIteratorsCount,
#				zend_uchar    _unused2)
#		} v;
#		uint32_t flags;
#	} u;
	uint32_t flags;
	uint32_t          nTableMask;
	Bucket           *arData;
	uint32_t          nNumUsed;
	uint32_t          nNumOfElements;
	uint32_t          nTableSize;
	uint32_t          nInternalPointer;
	zend_long         nNextFreeElement;
	dtor_func_t       pDestructor;
};

struct _zend_string {
	zend_refcounted_h gc;
	zend_ulong        h;                /* hash value */
	size_t            len;
	char              val[1];
};

typedef struct _zend_array      zend_array;
typedef struct _zend_array HashTable;

typedef union _zend_value {
	zend_long         lval;				/* long value */
#	double            dval;				/* double value */
#	zend_refcounted  *counted;
#	zend_string      *str;
#	zend_array       *arr;
	zend_object      *obj;
#	zend_resource    *res;
#	zend_reference   *ref;
#	zend_ast_ref     *ast;
	zval             *zv;
	void             *ptr;
	zend_class_entry *ce;
	void    *func;
	struct {
		uint32_t w1;
		uint32_t w2;
	} ww;
} zend_value;

struct _zend_class_entry {
	char type;
	zend_string *name;
	/* class_entry or string depending on ZEND_ACC_LINKED */
	union {
		zend_class_entry *parent;
		zend_string *parent_name;
	};
	int refcount;
	uint32_t ce_flags;

	int default_properties_count;
	int default_static_members_count;
	zval *default_properties_table;
	zval *default_static_members_table;
	zval ** static_members_table;
	HashTable function_table;
	HashTable properties_info;
	HashTable constants_table;

	struct _zend_property_info **properties_info_table;

	zend_function *constructor;
	zend_function *destructor;
	zend_function *clone;
	zend_function *__get;
	zend_function *__set;
	zend_function *__unset;
	zend_function *__isset;
	zend_function *__call;
	zend_function *__callstatic;
	zend_function *__tostring;
	zend_function *__debugInfo;
	zend_function *serialize_func;
	zend_function *unserialize_func;

	/* allocated only if class implements Iterator or IteratorAggregate interface */
	zend_class_iterator_funcs *iterator_funcs_ptr;

	/* handlers */
	union {
		zend_object* (*create_object)(zend_class_entry *class_type);
		int (*interface_gets_implemented)(zend_class_entry *iface, zend_class_entry *class_type); /* a class implements this interface */
	};
	zend_object_iterator *(*get_iterator)(zend_class_entry *ce, zval *object, int by_ref);
	zend_function *(*get_static_method)(zend_class_entry *ce, zend_string* method);

	/* serializer callbacks */
	int (*serialize)(zval *object, unsigned char **buffer, size_t *buf_len, zend_serialize_data *data);
	int (*unserialize)(zval *object, zend_class_entry *ce, const unsigned char *buf, size_t buf_len, zend_unserialize_data *data);

	uint32_t num_interfaces;
	uint32_t num_traits;

	/* class_entry or string(s) depending on ZEND_ACC_LINKED */
	union {
		zend_class_entry **interfaces;
		zend_class_name *interface_names;
	};

	zend_class_name *trait_names;
	zend_trait_alias **trait_aliases;
	zend_trait_precedence **trait_precedences;

	union {
		struct {
			zend_string *filename;
			uint32_t line_start;
			uint32_t line_end;
			zend_string *doc_comment;
		} user;
		struct {
			const struct _zend_function_entry *builtin_functions;
			struct _zend_module_entry *module;
		} internal;
	} info;
};



struct _zval_struct {
	zend_value        value;			/* value */
#	union {
#		struct {
#				zend_uchar    type,			/* active type */
#				zend_uchar    type_flags,
#				union {
#					uint16_t  extra;        /* not further specified */
#				} u;
#		} v;
#		uint32_t type_info;
#	} u1;
	uint32_t type_info;
	union {
		uint32_t     next;                 /* hash collision chain */
		uint32_t     cache_slot;           /* cache slot (for RECV_INIT) */
		uint32_t     opline_num;           /* opline number (for FAST_CALL) */
		uint32_t     lineno;               /* line number (for ast nodes) */
		uint32_t     num_args;             /* arguments number for EX(This) */
		uint32_t     fe_pos;               /* foreach position */
		uint32_t     fe_iter_idx;          /* foreach iterator index */
		uint32_t     access_flags;         /* class constant access flags */
		uint32_t     property_guard;       /* single property guard */
		uint32_t     constant_flags;       /* constant flags */
		uint32_t     extra;                /* not further specified */
	} u2;
};
struct _zend_execute_data {
	/*const */ void       *opline;           /* executed opline                */
	zend_execute_data   *call;             /* current call                   */
	zval                *return_value;
	void       *func;             /* executed function              */
	zval                 This;             /* this + call_info + num_args    */
	zend_execute_data   *prev_execute_data;
	zend_array          *symbol_table;
	void               **run_time_cache;   /* cache op_array->run_time_cache */
};

struct _zend_object {
	zend_refcounted_h gc;
	uint32_t          handle; // TODO: may be removed ???
	zend_class_entry *ce;
	/*const zend_object_handlers*/ void *handlers;
	HashTable        *properties;
	zval              properties_table[1];
};

typedef struct _Bucket {
	zval              val;
	zend_ulong        h;                /* hash value (or numeric index)   */
	zend_string      *key;              /* string key or NULL for numerics */
} Bucket;

struct _zend_executor_globals {
	zval uninitialized_zval;
	zval error_zval;

	/* symbol table cache */
	zend_array *symtable_cache[32];
	/* Pointer to one past the end of the symtable_cache */
	zend_array **symtable_cache_limit;
	/* Pointer to first unused symtable_cache slot */
	zend_array **symtable_cache_ptr;

	zend_array symbol_table;		/* main symbol table */
	HashTable included_files;	/* files already included */

	int *bailout;

	int error_reporting;
	int exit_status;

	HashTable *function_table;	/* function symbol table */
	HashTable *class_table;		/* class table */
	HashTable *zend_constants;	/* constants table */

	zval          *vm_stack_top;
	zval          *vm_stack_end;
	zend_vm_stack  vm_stack;
	size_t         vm_stack_page_size;

	struct _zend_execute_data *current_execute_data;
	zend_class_entry *fake_scope; /* used to avoid checks accessing properties */

	zend_long precision;

	int ticks_count;

	uint32_t persistent_constants_count;
	uint32_t persistent_functions_count;
	uint32_t persistent_classes_count;

	HashTable *in_autoload;
#	zend_function *autoload_func;
#	zend_bool full_tables_cleanup;
#
#	/* for extended information support */
#	zend_bool no_extensions;
#
#	zend_bool vm_interrupt;
#	zend_bool timed_out;
#	zend_long hard_timeout;
};


extern zend_executor_globals executor_globals;
extern zval* zend_hash_find(const HashTable *ht, zend_string *key);
extern zval* zend_hash_str_find(const HashTable *ht, const char *key, size_t len);
extern void zend_hash_rehash(HashTable *ht);
extern zval* _zend_hash_find_known_hash(const HashTable *ht, zend_string *key);

typedef zend_string *(*zend_string_init_interned_func_t)(const char *str, size_t size, int permanent);
extern zend_string_init_interned_func_t zend_string_init_interned;
extern zval* zend_hash_update(HashTable *ht, zend_string *key,zval *pData);
extern zval* zend_hash_str_update(HashTable *ht, const char *key, size_t len, zval *pData);
extern int zend_hash_str_del(HashTable *ht, const char *str, size_t len);
extern zval* zend_hash_str_add_new(HashTable *ht, const char *key, size_t len, zval *pData);
void function_add_ref(void *function);
