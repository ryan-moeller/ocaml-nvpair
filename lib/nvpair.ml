type hrtime = int64

module Nvpair = struct
  type t
  type nvlist

  type data_type =
    | Unknown
    | Boolean
    | Byte
    | Int16
    | Uint16
    | Int32
    | Uint32
    | Int64
    | Uint64
    | String
    | ByteArray
    | Int16Array
    | Uint16Array
    | Int32Array
    | UInt32Array
    | Int64Array
    | Uint64Array
    | StringArray
    | Hrtime
    | Nvlist
    | NvlistArray
    | BooleanValue
    | Int8
    | Uint8
    | BooleanArray
    | Int8Array
    | Uint8Array
    | Double

  external name : t -> string = "caml_nvpair_name"
  external data_type : t -> data_type = "caml_nvpair_type"
  external value_boolean_value : t -> bool = "caml_nvpair_value_boolean_value"
  external value_byte : t -> int = "caml_nvpair_value_byte"
  external value_int8 : t -> int = "caml_nvpair_value_int8"
  external value_uint8 : t -> int = "caml_nvpair_value_uint8"
  external value_int16 : t -> int = "caml_nvpair_value_int16"
  external value_uint16 : t -> int = "caml_nvpair_value_uint16"
  external value_int32 : t -> int32 = "caml_nvpair_value_int32"
  external value_uint32 : t -> int32 = "caml_nvpair_value_uint32"
  external value_int64 : t -> int64 = "caml_nvpair_value_int64"
  external value_uint64 : t -> int64 = "caml_nvpair_value_uint64"
  external value_string : t -> string = "caml_nvpair_value_string"
  external value_nvlist : t -> nvlist = "caml_nvpair_value_nvlist"
  external value_boolean_array : t -> bool array = "caml_nvpair_value_boolean_array"
  external value_byte_array : t -> bytes = "caml_nvpair_value_byte_array"
  external value_int8_array : t -> int array = "caml_nvpair_value_int8_array"
  external value_uint8_array : t -> int array = "caml_nvpair_value_uint8_array"
  external value_int16_array : t -> int array = "caml_nvpair_value_int16_array"
  external value_uint16_array : t -> int array = "caml_nvpair_value_uint16_array"
  external value_int32_array : t -> int32 array = "caml_nvpair_value_int32_array"
  external value_uint32_array : t -> int32 array = "caml_nvpair_value_uint32_array"
  external value_int64_array : t -> int64 array = "caml_nvpair_value_int64_array"
  external value_uint64_array : t -> int64 array = "caml_nvpair_value_uint64_array"
  external value_string_array : t -> string array = "caml_nvpair_value_string_array"
  external value_nvlist_array : t -> nvlist array = "caml_nvpair_value_nvlist_array"
  external value_hrtime : t -> hrtime = "caml_nvpair_value_hrtime"
  external value_double : t -> float = "caml_nvpair_value_double"
end

module Nvlist = struct
  type t = Nvpair.nvlist

  type encoding =
    | Native
    | XDR

  external alloc : unit -> t = "caml_nvlist_alloc"
  external pack : t -> encoding -> bytes = "caml_nvlist_pack"
  external unpack : bytes -> t = "caml_nvlist_unpack"
  external dup : t -> t = "caml_nvlist_dup"
  external merge : t -> t -> t = "caml_nvlist_merge"
  external add_nvpair : t -> Nvpair.t -> unit = "caml_nvlist_add_nvpair"
  external add_boolean : t -> string -> unit = "caml_nvlist_add_boolean"
  external add_boolean_value : t -> string -> bool -> unit = "caml_nvlist_add_boolean_value"
  external add_byte : t -> string -> int -> unit = "caml_nvlist_add_byte"
  external add_int8 : t -> string -> int -> unit = "caml_nvlist_add_int8"
  external add_uint8 : t -> string -> int -> unit = "caml_nvlist_add_uint8"
  external add_int16 : t -> string -> int -> unit = "caml_nvlist_add_int16"
  external add_uint16 : t -> string -> int -> unit = "caml_nvlist_add_uint16"
  external add_int32 : t -> string -> int32 -> unit = "caml_nvlist_add_int32"
  external add_uint32 : t -> string -> int32 -> unit = "caml_nvlist_add_uint32"
  external add_int64 : t -> string -> int64 -> unit = "caml_nvlist_add_int64"
  external add_uint64 : t -> string -> int64 -> unit = "caml_nvlist_add_uint64"
  external add_string : t -> string -> string -> unit = "caml_nvlist_add_string"
  external add_nvlist : t -> string -> t -> unit = "caml_nvlist_add_nvlist"
  external add_boolean_array : t -> string -> bool array -> unit = "caml_nvlist_add_boolean_array"
  external add_byte_array : t -> string -> bytes -> unit = "caml_nvlist_add_byte_array"
  external add_int8_array : t -> string -> int array -> unit = "caml_nvlist_add_int8_array"
  external add_uint8_array : t -> string -> int array -> unit = "caml_nvlist_add_uint8_array"
  external add_int16_array : t -> string -> int array -> unit = "caml_nvlist_add_int16_array"
  external add_uint16_array : t -> string -> int array -> unit = "caml_nvlist_add_uint16_array"
  external add_int32_array : t -> string -> int32 array -> unit = "caml_nvlist_add_int32_array"
  external add_uint32_array : t -> string -> int32 array -> unit = "caml_nvlist_add_uint32_array"
  external add_int64_array : t -> string -> int64 array -> unit = "caml_nvlist_add_int64_array"
  external add_uint64_array : t -> string -> int64 array -> unit = "caml_nvlist_add_uint64_array"
  external add_string_array : t -> string -> string array -> unit = "caml_nvlist_add_string_array"
  external add_nvlist_array : t -> string -> t array -> unit = "caml_nvlist_add_nvlist_array"
  external add_hrtime : t -> string -> hrtime -> unit = "caml_nvlist_add_hrtime"
  external add_double : t -> string -> float -> unit = "caml_nvlist_add_double"
  external remove : t -> string -> unit = "caml_nvlist_remove"
  external remove_nvpair : t -> Nvpair.t -> unit = "caml_nvlist_remove_nvpair"
  external lookup_boolean : t -> string -> bool option = "caml_nvlist_lookup_boolean"
  external lookup_boolean_value : t -> string -> bool option = "caml_nvlist_lookup_boolean_value"
  external lookup_byte : t -> string -> int option = "caml_nvlist_lookup_byte"
  external lookup_int8 : t -> string -> int option = "caml_nvlist_lookup_int8"
  external lookup_uint8 : t -> string -> int option = "caml_nvlist_lookup_uint8"
  external lookup_int16 : t -> string -> int option = "caml_nvlist_lookup_int16"
  external lookup_uint16 : t -> string -> int option = "caml_nvlist_lookup_uint16"
  external lookup_int32 : t -> string -> int32 option = "caml_nvlist_lookup_int32"
  external lookup_uint32 : t -> string -> int32 option = "caml_nvlist_lookup_uint32"
  external lookup_int64 : t -> string -> int64 option = "caml_nvlist_lookup_int64"
  external lookup_uint64 : t -> string -> int64 option = "caml_nvlist_lookup_uint64"
  external lookup_string : t -> string -> string option = "caml_nvlist_lookup_string"
  external lookup_nvlist : t -> string -> t option = "caml_nvlist_lookup_nvlist"
  external lookup_boolean_array : t -> string -> bool array option = "caml_nvlist_lookup_boolean_array"
  external lookup_byte_array : t -> string -> bytes option = "caml_nvlist_lookup_byte_array"
  external lookup_int8_array : t -> string -> int array option = "caml_nvlist_lookup_int8_array"
  external lookup_uint8_array : t -> string -> int array option = "caml_nvlist_lookup_uint8_array"
  external lookup_int16_array : t -> string -> int array option = "caml_nvlist_lookup_int16_array"
  external lookup_uint16_array : t -> string -> int array option = "caml_nvlist_lookup_uint16_array"
  external lookup_int32_array : t -> string -> int32 array option = "caml_nvlist_lookup_int32_array"
  external lookup_uint32_array : t -> string -> int32 array option = "caml_nvlist_lookup_uint32_array"
  external lookup_int64_array : t -> string -> int64 array option = "caml_nvlist_lookup_int64_array"
  external lookup_uint64_array : t -> string -> int64 array option = "caml_nvlist_lookup_uint64_array"
  external lookup_string_array : t -> string -> string array option = "caml_nvlist_lookup_string_array"
  external lookup_nvlist_array : t -> string -> t array option = "caml_nvlist_lookup_nvlist_array"
  external lookup_hrtime : t -> string -> hrtime option = "caml_nvlist_lookup_hrtime"
  external lookup_double : t -> string -> float option = "caml_nvlist_lookup_double"
  external lookup_nvpair : t -> string -> Nvpair.t option = "caml_nvlist_lookup_nvpair"
  external exists : t -> string -> bool = "caml_nvlist_exists"
  external empty : t -> bool = "caml_nvlist_empty"
  external next_nvpair : t -> Nvpair.t option -> Nvpair.t option = "caml_nvlist_next_nvpair"
  external prev_nvpair : t -> Nvpair.t option -> Nvpair.t option = "caml_nvlist_prev_nvpair"
end
