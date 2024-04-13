#ifndef CAML_NVPAIR_H
#define CAML_NVPAIR_H

#include <libnvpair.h>
#include <caml/mlvalues.h>
#include <caml/custom.h>

/* Accessing the nvpair_t * part of an OCaml custom block */
#define Nvpair_val(v) (*((nvpair_t **) Data_custom_val(v)))

extern value nvpair_alloc_custom(nvpair_t *);

/* Accessing the nvlist_t * part of an OCaml custom block */
#define Nvlist_val(v) (*((nvlist_t **) Data_custom_val(v)))

extern value nvlist_alloc_custom(nvlist_t *);

#endif /* CAML_NVPAIR_H */
