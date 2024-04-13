#include <sys/param.h>

/*
 * Solaris types not defined in system headers.
 */
typedef unsigned int uint_t;
typedef unsigned char uchar_t;
typedef enum { B_FALSE, B_TRUE } boolean_t;
typedef uint64_t hrtime_t;

#include <libnvpair.h>
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/custom.h>

static const struct custom_operations nvpair_ops = {
	"org.openzfs.nvpair",
	custom_finalize_default,
	custom_compare_default,
	custom_hash_default,
	custom_serialize_default,
	custom_deserialize_default,
	custom_compare_ext_default,
	custom_fixed_length_default
};

/* Accessing the nvpair_t * part of an OCaml custom block */
#define Nvpair_val(v) (*((nvpair_t **) Data_custom_val(v)))

/* Allocating an OCaml custom block to hold the given nvpair_t * */
static value
nvpair_alloc_custom(nvpair_t *nvp)
{
	value v = caml_alloc_custom(&nvpair_ops, sizeof (nvpair_t *), 0, 1);
	Nvpair_val(v) = nvp;
	return v;
}

static void custom_finalize_nvlist(value v);

static const struct custom_operations nvlist_ops = {
	"org.openzfs.nvlist",
	custom_finalize_nvlist,
	custom_compare_default,
	custom_hash_default,
	custom_serialize_default,
	custom_deserialize_default,
	custom_compare_ext_default,
	custom_fixed_length_default
};

/* Accessing the nvlist_t * part of an OCaml custom block */
#define Nvlist_val(v) (*((nvlist_t **) Data_custom_val(v)))

/* Allocating an OCaml custom block to hold the given nvlist_t * */
static value
nvlist_alloc_custom(nvlist_t *nvl)
{
	value v = caml_alloc_custom(&nvlist_ops, sizeof (nvlist_t *), 0, 1);
	Nvlist_val(v) = nvl;
	return v;
}

/*
 * Nvlist module
 */

CAMLprim value
caml_nvlist_alloc(value unit)
{
	CAMLparam1 (unit);
	nvlist_t *nvl;
	
	if (nvlist_alloc(&nvl, NV_UNIQUE_NAME, 0)) {
		caml_failwith("nvlist_alloc");
	}
	CAMLreturn (nvlist_alloc_custom(nvl));
}

static void
custom_finalize_nvlist(value nvl_custom)
{
	nvlist_free(Nvlist_val(nvl_custom));
}

CAMLprim value
caml_nvlist_pack(value nvl_custom, value encoding)
{
	CAMLparam2 (nvl_custom, encoding);
	CAMLlocal1 (packed);
	nvlist_t *nvl = Nvlist_val(nvl_custom);
	int enc = Int_val(encoding);
	char *buf;
	size_t size;

	if (nvlist_size(nvl, &size, enc)) {
		caml_failwith("nvlist_size");
	}
	packed = caml_alloc_string(size);
	buf = (char *)Bytes_val(packed);
	if (nvlist_pack(nvl, &buf, &size, enc, 0)) {
		caml_failwith("nvlist_pack");
	}
	CAMLreturn (packed);
}

CAMLprim value
caml_nvlist_unpack(value bytes)
{
	CAMLparam1 (bytes);
	nvlist_t *nvl = NULL;
	char *buf;
	size_t size;

	buf = (char *)Bytes_val(bytes);
	size = caml_string_length(bytes);
	if (nvlist_unpack(buf, size, &nvl, 0)) {
		caml_failwith("nvlist_unpack");
	}
	CAMLreturn (nvlist_alloc_custom(nvl));
}

CAMLprim value
caml_nvlist_dup(value nvl_custom)
{
	CAMLparam1 (nvl_custom);
	nvlist_t *dup;

	if (nvlist_dup(Nvlist_val(nvl_custom), &dup, 0)) {
		caml_failwith("nvlist_dup");
	}
	CAMLreturn (nvlist_alloc_custom(dup));
}

CAMLprim value
caml_nvlist_merge(value dst_custom, value nvl_custom)
{
	CAMLparam2 (dst_custom, nvl_custom);
	nvlist_t *dst;

	if (nvlist_dup(Nvlist_val(dst_custom), &dst, 0)) {
		caml_failwith("nvlist_dup");
	}
	if (nvlist_merge(dst, Nvlist_val(nvl_custom), 0)) {
		caml_failwith("nvlist_merge");
	}
	CAMLreturn (nvlist_alloc_custom(dst));
}

CAMLprim value
caml_nvlist_add_nvpair(value nvl_custom, value nvp_custom)
{
	CAMLparam2 (nvl_custom, nvp_custom);

	if (nvlist_add_nvpair(Nvlist_val(nvl_custom), Nvpair_val(nvp_custom))) {
		caml_failwith("nvlist_add_nvpair");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_boolean(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);

	if (nvlist_add_boolean(Nvlist_val(nvl_custom), String_val(name))) {
		caml_failwith("nvlist_add_boolean");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_boolean_value(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_boolean_value(Nvlist_val(nvl_custom), String_val(name), Bool_val(v))) {
		caml_failwith("nvlist_add_boolean_value");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_byte(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_byte(Nvlist_val(nvl_custom), String_val(name), Int_val(v))) {
		caml_failwith("nvlist_add_byte");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_int8(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_int8(Nvlist_val(nvl_custom), String_val(name), Int_val(v))) {
		caml_failwith("nvlist_add_int8");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_uint8(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_uint8(Nvlist_val(nvl_custom), String_val(name), Int_val(v))) {
		caml_failwith("nvlist_add_uint8");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_int16(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_int16(Nvlist_val(nvl_custom), String_val(name), Int_val(v))) {
		caml_failwith("nvlist_add_int16");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_uint16(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_uint16(Nvlist_val(nvl_custom), String_val(name), Int_val(v))) {
		caml_failwith("nvlist_add_uint16");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_int32(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_int32(Nvlist_val(nvl_custom), String_val(name), Int32_val(v))) {
		caml_failwith("nvlist_add_int32");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_uint32(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_uint32(Nvlist_val(nvl_custom), String_val(name), Int32_val(v))) {
		caml_failwith("nvlist_add_uint32");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_int64(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_int64(Nvlist_val(nvl_custom), String_val(name), Int64_val(v))) {
		caml_failwith("nvlist_add_int64");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_uint64(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_uint64(Nvlist_val(nvl_custom), String_val(name), Int64_val(v))) {
		caml_failwith("nvlist_add_uint64");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_string(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_string(Nvlist_val(nvl_custom), String_val(name), String_val(v))) {
		caml_failwith("nvlist_add_string");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_nvlist(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_nvlist(Nvlist_val(nvl_custom), String_val(name), Nvlist_val(v))) {
		caml_failwith("nvlist_add_nvlist");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_boolean_array(value nvl_custom, value name, value vs)
{
	CAMLparam3 (nvl_custom, name, vs);
	boolean_t *bools;
	uint_t nbools;

	nbools = Wosize_val(vs);
	bools = malloc(nbools * sizeof (boolean_t));
	if (bools == NULL) {
		caml_failwith("malloc");
	}
	for (uint_t i = 0; i < nbools; i++) {
		bools[i] = Bool_val(Field(vs, i)) ? B_TRUE : B_FALSE;
      	}
	if (nvlist_add_boolean_array(Nvlist_val(nvl_custom), String_val(name), bools, nbools)) {
		free(bools);
		caml_failwith("nvlist_add_boolean_array");
	}
	free(bools);
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_byte_array(value nvl_custom, value name, value vs)
{
	CAMLparam3 (nvl_custom, name, vs);
	uchar_t *bytes;
	uint_t nbytes;

	nbytes = caml_string_length(vs);
	bytes = Bytes_val(vs);
	if (nvlist_add_byte_array(Nvlist_val(nvl_custom), String_val(name), bytes, nbytes)) {
		caml_failwith("nvlist_add_byte_array");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_int8_array(value nvl_custom, value name, value vs)
{
	CAMLparam3 (nvl_custom, name, vs);
	int8_t *ints;
	uint_t nints;

	nints = Wosize_val(vs);
	ints = malloc(nints * sizeof (int8_t));
	if (ints == NULL) {
		caml_failwith("malloc");
	}
	for (uint_t i = 0; i < nints; i++) {
		ints[i] = Int_val(Field(vs, i));
	}
	if (nvlist_add_int8_array(Nvlist_val(nvl_custom), String_val(name), ints, nints)) {
		free(ints);
		caml_failwith("nvlist_add_int8_array");
	}
	free(ints);
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_uint8_array(value nvl_custom, value name, value vs)
{
	CAMLparam3 (nvl_custom, name, vs);
	uint8_t *ints;
	uint_t nints;

	nints = Wosize_val(vs);
	ints = malloc(nints * sizeof (uint8_t));
	if (ints == NULL) {
		caml_failwith("malloc");
	}
	for (uint_t i = 0; i < nints; i++) {
		ints[i] = Int_val(Field(vs, i));
	}
	if (nvlist_add_uint8_array(Nvlist_val(nvl_custom), String_val(name), ints, nints)) {
		free(ints);
		caml_failwith("nvlist_add_uint8_array");
	}
	free(ints);
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_int16_array(value nvl_custom, value name, value vs)
{
	CAMLparam3 (nvl_custom, name, vs);
	int16_t *ints;
	uint_t nints;

	nints = Wosize_val(vs);
	ints = malloc(nints * sizeof (int16_t));
	if (ints == NULL) {
		caml_failwith("malloc");
	}
	for (uint_t i = 0; i < nints; i++) {
		ints[i] = Int_val(Field(vs, i));
	}
	if (nvlist_add_int16_array(Nvlist_val(nvl_custom), String_val(name), ints, nints)) {
		free(ints);
		caml_failwith("nvlist_add_int16_array");
	}
	free(ints);
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_uint16_array(value nvl_custom, value name, value vs)
{
	CAMLparam3 (nvl_custom, name, vs);
	uint16_t *ints;
	uint_t nints;

	nints = Wosize_val(vs);
	ints = malloc(nints * sizeof (uint16_t));
	if (ints == NULL) {
		caml_failwith("malloc");
	}
	for (uint_t i = 0; i < nints; i++) {
		ints[i] = Int_val(Field(vs, i));
	}
	if (nvlist_add_uint16_array(Nvlist_val(nvl_custom), String_val(name), ints, nints)) {
		free(ints);
		caml_failwith("caml_add_uint16_array");
	}
	free(ints);
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_int32_array(value nvl_custom, value name, value vs)
{
	CAMLparam3 (nvl_custom, name, vs);
	int32_t *ints;
	uint_t nints;

	nints = Wosize_val(vs);
	ints = malloc(nints * sizeof (int32_t));
	if (ints == NULL) {
		caml_failwith("malloc");
	}
	for (uint_t i = 0; i < nints; i++) {
		ints[i] = Int32_val(Field(vs, i));
	}
	if (nvlist_add_int32_array(Nvlist_val(nvl_custom), String_val(name), ints, nints)) {
		free(ints);
		caml_failwith("nvlist_add_int32_array");
	}
	free(ints);
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_uint32_array(value nvl_custom, value name, value vs)
{
	CAMLparam3 (nvl_custom, name, vs);
	uint32_t *ints;
	uint_t nints;

	nints = Wosize_val(vs);
	ints = malloc(nints * sizeof (uint32_t));
	if (ints == NULL) {
		caml_failwith("malloc");
	}
	for (uint_t i = 0; i < nints; i++) {
		ints[i] = Int32_val(Field(vs, i));
	}
	if (nvlist_add_uint32_array(Nvlist_val(nvl_custom), String_val(name), ints, nints)) {
		free(ints);
		caml_failwith("nvlist_add_uint32_array");
	}
	free(ints);
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_int64_array(value nvl_custom, value name, value vs)
{
	CAMLparam3 (nvl_custom, name, vs);
	int64_t *ints;
	uint_t nints;

	nints = Wosize_val(vs);
	ints = malloc(nints * sizeof (int64_t));
	if (ints == NULL) {
		caml_failwith("malloc");
	}
	for (uint_t i = 0; i < nints; i++) {
		ints[i] = Int64_val(Field(vs, i));
	}
	if (nvlist_add_int64_array(Nvlist_val(nvl_custom), String_val(name), ints, nints)) {
		free(ints);
		caml_failwith("nvlist_add_int64_array");
	}
	free(ints);
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_uint64_array(value nvl_custom, value name, value vs)
{
	CAMLparam3 (nvl_custom, name, vs);
	uint64_t *ints;
	uint_t nints;

	nints = Wosize_val(vs);
	ints = malloc(nints * sizeof (uint64_t));
	if (ints == NULL) {
		caml_failwith("malloc");
	}
	for (uint_t i = 0; i < nints; i++) {
		ints[i] = Int64_val(Field(vs, i));
	}
	if (nvlist_add_uint64_array(Nvlist_val(nvl_custom), String_val(name), ints, nints)) {
		free(ints);
		caml_failwith("nvlist_add_uint64_array");
	}
	free(ints);
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_string_array(value nvl_custom, value name, value vs)
{
	CAMLparam3 (nvl_custom, name, vs);
	const char **strs;
	uint_t nstrs;

	nstrs = Wosize_val(vs);
	strs = malloc(nstrs * sizeof (const char *));
	if (strs == NULL) {
		caml_failwith("malloc");
	}
	for (uint_t i = 0; i < nstrs; i++) {
		strs[i] = String_val(Field(vs, i));
	}
	if (nvlist_add_string_array(Nvlist_val(nvl_custom), String_val(name), strs, nstrs)) {
		free(strs);
		caml_failwith("nvlist_add_string_array");
	}
	free(strs);
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_nvlist_array(value nvl_custom, value name, value vs)
{
	CAMLparam3 (nvl_custom, name, vs);
	const nvlist_t **lists;
	uint_t nlists;

	nlists = Wosize_val(vs);
	lists = malloc(nlists * sizeof (const nvlist_t *));
	if (lists == NULL) {
		caml_failwith("malloc");
	}
	for (uint_t i = 0; i < nlists; i++) {
		lists[i] = Nvlist_val(Field(vs, i));
	}
	if (nvlist_add_nvlist_array(Nvlist_val(nvl_custom), String_val(name), lists, nlists)) {
		free(lists);
		caml_failwith("nvlist_add_nvlist_array");
	}
	free(lists);
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_hrtime(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_hrtime(Nvlist_val(nvl_custom), String_val(name), Int64_val(v))) {
		caml_failwith("nvlist_add_hrtime");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_add_double(value nvl_custom, value name, value v)
{
	CAMLparam3 (nvl_custom, name, v);

	if (nvlist_add_double(Nvlist_val(nvl_custom), String_val(name), Double_val(v))) {
		caml_failwith("nvlist_add_double");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_remove(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	int err;

	err = nvlist_remove_all(Nvlist_val(nvl_custom), String_val(name));
	if (err && err != ENOENT) {
		caml_failwith("nvlist_remove_all");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_remove_nvpair(value nvl_custom, value nvp_custom)
{
	CAMLparam2 (nvl_custom, nvp_custom);
	int err;

	err = nvlist_remove_nvpair(Nvlist_val(nvl_custom), Nvpair_val(nvp_custom));
	if (err && err != ENOENT) {
		caml_failwith("nvlist_remove_nvpair");
	}
	CAMLreturn (Val_unit);
}

CAMLprim value
caml_nvlist_lookup_boolean(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	int err;

	err = nvlist_lookup_boolean(Nvlist_val(nvl_custom), String_val(name));
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_boolean");
	}
	CAMLreturn (caml_alloc_some(Val_true));
}

CAMLprim value
caml_nvlist_lookup_boolean_value(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	boolean_t v;
	int err;

	err = nvlist_lookup_boolean_value(Nvlist_val(nvl_custom), String_val(name), &v);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_boolean_value");
	}
	CAMLreturn (caml_alloc_some(v == B_TRUE ? Val_true : Val_false));
}

CAMLprim value
caml_nvlist_lookup_byte(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	uchar_t v;
	int err;

	err = nvlist_lookup_byte(Nvlist_val(nvl_custom), String_val(name), &v);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_byte");
	}
	CAMLreturn (caml_alloc_some(Val_int(v)));
}

CAMLprim value
caml_nvlist_lookup_int8(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	int8_t v;
	int err;

	err = nvlist_lookup_int8(Nvlist_val(nvl_custom), String_val(name), &v);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_int8");
	}
	CAMLreturn (caml_alloc_some(Val_int(v)));
}

CAMLprim value
caml_nvlist_lookup_uint8(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	uint8_t v;
	int err;

	err = nvlist_lookup_uint8(Nvlist_val(nvl_custom), String_val(name), &v);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_uint8");
	}
	CAMLreturn (caml_alloc_some(Val_int(v)));
}

CAMLprim value
caml_nvlist_lookup_int16(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	int16_t v;
	int err;

	err = nvlist_lookup_int16(Nvlist_val(nvl_custom), String_val(name), &v);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_int16");
	}
	CAMLreturn (caml_alloc_some(Val_int(v)));
}

CAMLprim value
caml_nvlist_lookup_uint16(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	uint16_t v;
	int err;

	err = nvlist_lookup_uint16(Nvlist_val(nvl_custom), String_val(name), &v);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_uint16");
	}
	CAMLreturn (caml_alloc_some(Val_int(v)));
}

CAMLprim value
caml_nvlist_lookup_int32(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	int32_t v;
	int err;

	err = nvlist_lookup_int32(Nvlist_val(nvl_custom), String_val(name), &v);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_int32");
	}
	CAMLreturn (caml_alloc_some(caml_copy_int32(v)));
}

CAMLprim value
caml_nvlist_lookup_uint32(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	uint32_t v;
	int err;

	err = nvlist_lookup_uint32(Nvlist_val(nvl_custom), String_val(name), &v);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_uint32");
	}
	CAMLreturn (caml_alloc_some(caml_copy_int32(v)));
}

CAMLprim value
caml_nvlist_lookup_int64(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	int64_t v;
	int err;

	err = nvlist_lookup_int64(Nvlist_val(nvl_custom), String_val(name), &v);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_int64");
	}
	CAMLreturn (caml_alloc_some(caml_copy_int64(v)));
}

CAMLprim value
caml_nvlist_lookup_uint64(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	uint64_t v;
	int err;

	err = nvlist_lookup_uint64(Nvlist_val(nvl_custom), String_val(name), &v);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_uint64");
	}
	CAMLreturn (caml_alloc_some(caml_copy_int64(v)));
}

CAMLprim value
caml_nvlist_lookup_string(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	const char *s;
	int err;

	err = nvlist_lookup_string(Nvlist_val(nvl_custom), String_val(name), &s);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_string");
	}
	CAMLreturn (caml_alloc_some(caml_copy_string(s)));
}

CAMLprim value
caml_nvlist_lookup_nvlist(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	nvlist_t *nvl;
	int err;

	err = nvlist_lookup_nvlist(Nvlist_val(nvl_custom), String_val(name), &nvl);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_nvlist");
	}
	CAMLreturn (caml_alloc_some(nvlist_alloc_custom(nvl)));
}

CAMLprim value
caml_nvlist_lookup_boolean_array(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	CAMLlocal1 (vs);
	boolean_t *bools;
	uint_t nbools;
	int err;

	err = nvlist_lookup_boolean_array(Nvlist_val(nvl_custom), String_val(name), &bools, &nbools);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_boolean_array");
	}
	vs = caml_alloc(nbools, 0);
	for (uint_t i = 0; i < nbools; i++) {
		Store_field(vs, i, bools[i] == B_TRUE ? Val_true : Val_false);
	}
	CAMLreturn (caml_alloc_some(vs));
}

CAMLprim value
caml_nvlist_lookup_byte_array(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	CAMLlocal1 (vs);
	uchar_t *bytes;
	uint_t nbytes;
	int err;

	err = nvlist_lookup_byte_array(Nvlist_val(nvl_custom), String_val(name), &bytes, &nbytes);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_byte_array");
	}
	vs = caml_alloc_initialized_string(nbytes, (char *)bytes);
	CAMLreturn (caml_alloc_some(vs));
}

CAMLprim value
caml_nvlist_lookup_int8_array(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	CAMLlocal1 (vs);
	int8_t *ints;
	uint_t nints;
	int err;

	err = nvlist_lookup_int8_array(Nvlist_val(nvl_custom), String_val(name), &ints, &nints);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_int8_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, Val_int(ints[i]));
	}
	CAMLreturn (caml_alloc_some(vs));
}

CAMLprim value
caml_nvlist_lookup_uint8_array(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	CAMLlocal1 (vs);
	uint8_t *ints;
	uint_t nints;
	int err;

	err = nvlist_lookup_uint8_array(Nvlist_val(nvl_custom), String_val(name), &ints, &nints);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_uint8_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, Val_int(ints[i]));
	}
	CAMLreturn (caml_alloc_some(vs));
}

CAMLprim value
caml_nvlist_lookup_int16_array(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	CAMLlocal1 (vs);
	int16_t *ints;
	uint_t nints;
	int err;

	err = nvlist_lookup_int16_array(Nvlist_val(nvl_custom), String_val(name), &ints, &nints);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_int16_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, Val_int(ints[i]));
	}
	CAMLreturn (caml_alloc_some(vs));
}

CAMLprim value
caml_nvlist_lookup_uint16_array(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	CAMLlocal1 (vs);
	uint16_t *ints;
	uint_t nints;
	int err;

	err = nvlist_lookup_uint16_array(Nvlist_val(nvl_custom), String_val(name), &ints, &nints);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_uint16_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, Val_int(ints[i]));
	}
	CAMLreturn (caml_alloc_some(vs));
}

CAMLprim value
caml_nvlist_lookup_int32_array(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	CAMLlocal1 (vs);
	int32_t *ints;
	uint_t nints;
	int err;

	err = nvlist_lookup_int32_array(Nvlist_val(nvl_custom), String_val(name), &ints, &nints);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_int32_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, caml_copy_int32(ints[i]));
	}
	CAMLreturn (caml_alloc_some(vs));
}

CAMLprim value
caml_nvlist_lookup_uint32_array(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	CAMLlocal1 (vs);
	uint32_t *ints;
	uint_t nints;
	int err;

	err = nvlist_lookup_uint32_array(Nvlist_val(nvl_custom), String_val(name), &ints, &nints);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_uint32_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, caml_copy_int32(ints[i]));
	}
	CAMLreturn (caml_alloc_some(vs));
}

CAMLprim value
caml_nvlist_lookup_int64_array(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	CAMLlocal1 (vs);
	int64_t *ints;
	uint_t nints;
	int err;

	err = nvlist_lookup_int64_array(Nvlist_val(nvl_custom), String_val(name), &ints, &nints);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_int64_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, caml_copy_int64(ints[i]));
	}
	CAMLreturn (caml_alloc_some(vs));
}

CAMLprim value
caml_nvlist_lookup_uint64_array(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	CAMLlocal1 (vs);
	uint64_t *ints;
	uint_t nints;
	int err;

	err = nvlist_lookup_uint64_array(Nvlist_val(nvl_custom), String_val(name), &ints, &nints);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_uint64_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, caml_copy_int64(ints[i]));
	}
	CAMLreturn (caml_alloc_some(vs));
}

CAMLprim value
caml_nvlist_lookup_string_array(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	CAMLlocal1 (vs);
	char **strs;
	uint_t nstrs;
	int err;

	err = nvlist_lookup_string_array(Nvlist_val(nvl_custom), String_val(name), &strs, &nstrs);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_string_array");
	}
	vs = caml_alloc(nstrs, 0);
	for (uint_t i = 0; i < nstrs; i++) {
		Store_field(vs, i, caml_copy_string(strs[i]));
	}
	CAMLreturn (caml_alloc_some(vs));
}

CAMLprim value
caml_nvlist_lookup_nvlist_array(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	CAMLlocal1 (vs);
	nvlist_t **lists;
	uint_t nlists;
	int err;

	err = nvlist_lookup_nvlist_array(Nvlist_val(nvl_custom), String_val(name), &lists, &nlists);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_nvlist_array");
	}
	vs = caml_alloc(nlists, 0);
	for (uint_t i = 0; i < nlists; i++) {
		nvlist_t *nvl;
		if (nvlist_dup(lists[i], &nvl, 0)) {
			caml_failwith("nvlist_dup");
		}
		Store_field(vs, i, nvlist_alloc_custom(nvl));
	}
	CAMLreturn (caml_alloc_some(vs));
}

CAMLprim value
caml_nvlist_lookup_hrtime(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	hrtime_t v;
	int err;

	err = nvlist_lookup_hrtime(Nvlist_val(nvl_custom), String_val(name), &v);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_hrtime");
	}
	CAMLreturn (caml_alloc_some(caml_copy_int64(v)));
}

CAMLprim value
caml_nvlist_lookup_double(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	double v;
	int err;

	err = nvlist_lookup_double(Nvlist_val(nvl_custom), String_val(name), &v);
	if (err == ENOENT) {
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_double");
	}
	CAMLreturn (caml_alloc_some(caml_copy_double(v)));
}

CAMLprim value
caml_nvlist_lookup_nvpair(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	nvpair_t *nvp;
	int err;

	err = nvlist_lookup_nvpair(Nvlist_val(nvl_custom), String_val(name), &nvp);
	if (err == EINVAL) { /* XXX: nvlist_lookup_nvpair(3) doesn't return ENOENT */
		CAMLreturn (Val_none);
	} else if (err) {
		caml_failwith("nvlist_lookup_nvpair");
	}
	CAMLreturn (caml_alloc_some(nvpair_alloc_custom(nvp)));
}

CAMLprim value
caml_nvlist_exists(value nvl_custom, value name)
{
	CAMLparam2 (nvl_custom, name);
	boolean_t exists;

	exists = nvlist_exists(Nvlist_val(nvl_custom), String_val(name));
	CAMLreturn (exists == B_TRUE ? Val_true : Val_false);
}

CAMLprim value
caml_nvlist_empty(value nvl_custom)
{
	CAMLparam1 (nvl_custom);
	boolean_t empty;

	empty = nvlist_empty(Nvlist_val(nvl_custom));
	CAMLreturn (empty == B_TRUE ? Val_true : Val_false);
}

CAMLprim value
caml_nvlist_next_nvpair(value nvl_custom, value nvp_custom_opt)
{
	CAMLparam2 (nvl_custom, nvp_custom_opt);
	nvpair_t *nvp, *next;
	int err;

	nvp = Is_none(nvp_custom_opt) ? NULL : Nvpair_val(Some_val(nvp_custom_opt));
	next = nvlist_next_nvpair(Nvlist_val(nvl_custom), nvp);
	if (next == NULL) {
		CAMLreturn (Val_none);
	}
	CAMLreturn (caml_alloc_some(nvpair_alloc_custom(next)));
}

CAMLprim value
caml_nvlist_prev_nvpair(value nvl_custom, value nvp_custom_opt)
{
	CAMLparam2 (nvl_custom, nvp_custom_opt);
	nvpair_t *nvp, *prev;
	int err;

	nvp = Is_none(nvp_custom_opt) ? NULL : Nvpair_val(Some_val(nvp_custom_opt));
	prev = nvlist_prev_nvpair(Nvlist_val(nvl_custom), nvp);
	if (prev == NULL) {
		CAMLreturn (Val_none);
	}
	CAMLreturn (caml_alloc_some(nvpair_alloc_custom(prev)));
}

/*
 * Nvpair module
 */

CAMLprim value
caml_nvpair_name(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	const char *name;

	name = nvpair_name(Nvpair_val(nvp_custom));
	CAMLreturn (caml_copy_string(name));
}

CAMLprim value
caml_nvpair_type(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	data_type_t t;

	t = nvpair_type(Nvpair_val(nvp_custom));
	CAMLreturn (Val_int(t));
}

CAMLprim value
caml_nvpair_value_boolean_value(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	boolean_t v;

	if (nvpair_value_boolean_value(Nvpair_val(nvp_custom), &v)) {
		caml_failwith("nvpair_value_boolean_value");
	}
	CAMLreturn (v == B_TRUE ? Val_true : Val_false);
}

CAMLprim value
caml_nvpair_value_byte(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	uchar_t v;

	if (nvpair_value_byte(Nvpair_val(nvp_custom), &v)) {
		caml_failwith("nvpair_value_byte");
	}
	CAMLreturn (Val_int(v));
}

CAMLprim value
caml_nvpair_value_int8(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	int8_t v;

	if (nvpair_value_int8(Nvpair_val(nvp_custom), &v)) {
		caml_failwith("nvpair_value_int8");
	}
	CAMLreturn (Val_int(v));
}

CAMLprim value
caml_nvpair_value_uint8(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	uint8_t v;

	if (nvpair_value_uint8(Nvpair_val(nvp_custom), &v)) {
		caml_failwith("nvpair_value_uint8");
	}
	CAMLreturn (Val_int(v));
}

CAMLprim value
caml_nvpair_value_int16(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	int16_t v;

	if (nvpair_value_int16(Nvpair_val(nvp_custom), &v)) {
		caml_failwith("nvpair_value_int16");
	}
	CAMLreturn (Val_int(v));
}

CAMLprim value
caml_nvpair_value_uint16(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	uint16_t v;

	if (nvpair_value_uint16(Nvpair_val(nvp_custom), &v)) {
		caml_failwith("nvpair_value_uint16");
	}
	CAMLreturn (Val_int(v));
}

CAMLprim value
caml_nvpair_value_int32(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	int32_t v;

	if (nvpair_value_int32(Nvpair_val(nvp_custom), &v)) {
		caml_failwith("nvpair_value_int32");
	}
	CAMLreturn (caml_copy_int32(v));
}

CAMLprim value
caml_nvpair_value_uint32(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	uint32_t v;

	if (nvpair_value_uint32(Nvpair_val(nvp_custom), &v)) {
		caml_failwith("nvpair_value_uint32");
	}
	CAMLreturn (caml_copy_int32(v));
}

CAMLprim value
caml_nvpair_value_int64(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	int64_t v;

	if (nvpair_value_int64(Nvpair_val(nvp_custom), &v)) {
		caml_failwith("nvpair_value_int64");
	}
	CAMLreturn (caml_copy_int64(v));
}

CAMLprim value
caml_nvpair_value_uint64(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	uint64_t v;

	if (nvpair_value_uint64(Nvpair_val(nvp_custom), &v)) {
		caml_failwith("nvpair_value_uint64");
	}
	CAMLreturn (caml_copy_int64(v));
}

CAMLprim value
caml_nvpair_value_string(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	const char *s;

	if (nvpair_value_string(Nvpair_val(nvp_custom), &s)) {
		caml_failwith("nvpair_value_string");
	}
	CAMLreturn (caml_copy_string(s));
}

CAMLprim value
caml_nvpair_value_nvlist(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	nvlist_t *nvl, *dup;

	if (nvpair_value_nvlist(Nvpair_val(nvp_custom), &nvl)) {
		caml_failwith("nvpair_value_nvlist");
	}
	if (nvlist_dup(nvl, &dup, 0)) {
		caml_failwith("nvlist_dup");
	}
	CAMLreturn (nvlist_alloc_custom(dup));
}

CAMLprim value
caml_nvpair_value_boolean_array(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	CAMLlocal1 (vs);
	boolean_t *bools;
	uint_t nbools;

	if (nvpair_value_boolean_array(Nvpair_val(nvp_custom), &bools, &nbools)) {
		caml_failwith("nvpair_value_boolean_array");
	}
	vs = caml_alloc(nbools, 0);
	for (uint_t i = 0; i < nbools; i++) {
		Store_field(vs, i, bools[i] == B_TRUE ? Val_true : Val_false);
	}
	CAMLreturn (vs);
}

CAMLprim value
caml_nvpair_value_byte_array(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	uchar_t *bytes;
	uint_t nbytes;

	if (nvpair_value_byte_array(Nvpair_val(nvp_custom), &bytes, &nbytes)) {
		caml_failwith("nvpair_value_byte_array");
	}
	CAMLreturn (caml_alloc_initialized_string(nbytes, (const char *)bytes));
}

CAMLprim value
caml_nvpair_value_int8_array(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	CAMLlocal1 (vs);
	int8_t *ints;
	uint_t nints;

	if (nvpair_value_int8_array(Nvpair_val(nvp_custom), &ints, &nints)) {
		caml_failwith("nvpair_value_int8_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, Val_int(ints[i]));
	}
	CAMLreturn (vs);
}

CAMLprim value
caml_nvpair_value_uint8_array(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	CAMLlocal1 (vs);
	uint8_t *ints;
	uint_t nints;

	if (nvpair_value_uint8_array(Nvpair_val(nvp_custom), &ints, &nints)) {
		caml_failwith("nvpair_value_uint8_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, Val_int(ints[i]));
	}
	CAMLreturn (vs);
}

CAMLprim value
caml_nvpair_value_int16_array(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	CAMLlocal1 (vs);
	int16_t *ints;
	uint_t nints;

	if (nvpair_value_int16_array(Nvpair_val(nvp_custom), &ints, &nints)) {
		caml_failwith("nvpair_value_int16_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, Val_int(ints[i]));
	}
	CAMLreturn (vs);
}

CAMLprim value
caml_nvpair_value_uint16_array(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	CAMLlocal1 (vs);
	uint16_t *ints;
	uint_t nints;

	if (nvpair_value_uint16_array(Nvpair_val(nvp_custom), &ints, &nints)) {
		caml_failwith("nvpair_value_uint16_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, Val_int(ints[i]));
	}
	CAMLreturn (vs);
}

CAMLprim value
caml_nvpair_value_int32_array(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	CAMLlocal1 (vs);
	int32_t *ints;
	uint_t nints;

	if (nvpair_value_int32_array(Nvpair_val(nvp_custom), &ints, &nints)) {
		caml_failwith("nvpair_value_int32_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, caml_copy_int32(ints[i]));
	}
	CAMLreturn (vs);
}

CAMLprim value
caml_nvpair_value_uint32_array(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	CAMLlocal1 (vs);
	uint32_t *ints;
	uint_t nints;

	if (nvpair_value_uint32_array(Nvpair_val(nvp_custom), &ints, &nints)) {
		caml_failwith("nvpair_value_uint32_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, caml_copy_int32(ints[i]));
	}
	CAMLreturn (vs);
}

CAMLprim value
caml_nvpair_value_int64_array(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	CAMLlocal1 (vs);
	int64_t *ints;
	uint_t nints;

	if (nvpair_value_int64_array(Nvpair_val(nvp_custom), &ints, &nints)) {
		caml_failwith("nvpair_value_int64_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, caml_copy_int64(ints[i]));
	}
	CAMLreturn (vs);
}

CAMLprim value
caml_nvpair_value_uint64_array(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	CAMLlocal1 (vs);
	uint64_t *ints;
	uint_t nints;

	if (nvpair_value_uint64_array(Nvpair_val(nvp_custom), &ints, &nints)) {
		caml_failwith("nvpair_value_uint64_array");
	}
	vs = caml_alloc(nints, 0);
	for (uint_t i = 0; i < nints; i++) {
		Store_field(vs, i, caml_copy_int64(ints[i]));
	}
	CAMLreturn (vs);
}

CAMLprim value
caml_nvpair_value_string_array(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	CAMLlocal1 (vs);
	const char **strs;
	uint_t nstrs;

	if (nvpair_value_string_array(Nvpair_val(nvp_custom), &strs, &nstrs)) {
		caml_failwith("nvpair_value_string_array");
	}
	vs = caml_alloc(nstrs, 0);
	for (uint_t i = 0; i < nstrs; i++) {
		Store_field(vs, i, caml_copy_string(strs[i]));
	}
	CAMLreturn (vs);
}

CAMLprim value
caml_nvpair_value_nvlist_array(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	CAMLlocal1 (vs);
	nvlist_t **lists;
	uint_t nlists;

	if (nvpair_value_nvlist_array(Nvpair_val(nvp_custom), &lists, &nlists)) {
		caml_failwith("nvpair_value_nvlist_array");
	}
	vs = caml_alloc(nlists, 0);
	for (uint_t i = 0; i < nlists; i++) {
		nvlist_t *nvl;
		if (nvlist_dup(lists[i], &nvl, 0)) {
			caml_failwith("nvlist_dup");
		}
		Store_field(vs, i, nvlist_alloc_custom(nvl));
	}
	CAMLreturn (vs);
}

CAMLprim value
caml_nvpair_value_hrtime(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	hrtime_t v;

	if (nvpair_value_hrtime(Nvpair_val(nvp_custom), &v)) {
		caml_failwith("nvpair_value_hrtime");
	}
	CAMLreturn (caml_copy_int64(v));
}

CAMLprim value
caml_nvpair_value_double(value nvp_custom)
{
	CAMLparam1 (nvp_custom);
	double v;

	if (nvpair_value_double(Nvpair_val(nvp_custom), &v)) {
		caml_failwith("nvpair_value_double");
	}
	CAMLreturn (caml_copy_double(v));
}
