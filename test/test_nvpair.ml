open Nvpair

let alist = Nvlist.alloc ()

let () =
  Nvlist.add_boolean alist "boolean";
  Nvlist.add_boolean_value alist "boolean_value" false;
  Nvlist.add_byte alist "byte" 0xff;
  Nvlist.add_int8 alist "int8" (-12);
  Nvlist.add_uint8 alist "uint8" 255;
  Nvlist.add_int16 alist "int16" (-1234);
  Nvlist.add_uint16 alist "uint16" 0xffff;
  Nvlist.add_int32 alist "int32" Int32.min_int;
  Nvlist.add_uint32 alist "uint32" 0xffff_ffffl;
  Nvlist.add_int64 alist "int64" Int64.min_int;
  Nvlist.add_uint64 alist "uint64" 0xffff_ffff_ffff_ffffL;
  Nvlist.add_string alist "string" "this is a string"

let blist = Nvlist.alloc ()

let () =
  Nvlist.add_nvlist blist "nvlist" alist;
  Nvlist.add_boolean_array blist "boolean_array" [| true; false; true; false |];
  Nvlist.add_byte_array blist "byte_array" @@ Bytes.of_string "these are bytes";
  Nvlist.add_int8_array blist "int8_array" [| 0x7f; 0x00; -1; 123 |];
  Nvlist.add_uint8_array blist "uint8_array" [| 0xff; 0x00 |];
  Nvlist.add_int16_array blist "int16_array" [| 1234; -1234; 0x1234; -0x1234 |];
  Nvlist.add_uint16_array blist "uint16_array" [| 0xffff; 0x0000 |];
  Nvlist.add_int32_array blist "int32_array" [| Int32.max_int; Int32.min_int |];
  Nvlist.add_uint32_array blist "uint32_array" [| 0xffff_ffffl; 0x0000_0000l |];
  Nvlist.add_int64_array blist "int64_array" [| Int64.max_int; Int64.min_int |];
  Nvlist.add_uint64_array blist "uint64_array"
    [| 0xffff_ffff_ffff_ffffL; 0x0000_0000_0000_0000L |];
  Nvlist.add_string_array blist "string_array" [| "these"; "are"; "strings" |]

let clist = Nvlist.alloc ()
let dlist = Nvlist.alloc ()
let elist = Nvlist.dup alist

let () =
  Nvlist.add_hrtime clist "hrtime" 12345L;
  Nvlist.add_double dlist "double" 123.45;
  Nvlist.add_nvlist_array blist "nvlist_array" [| clist; dlist; elist |]

let packed_native = Nvlist.pack blist Nvlist.Native
let packed_xdr = Nvlist.pack blist Nvlist.XDR

let () =
  Printf.printf "packed size (native): %u\n" @@ Bytes.length packed_native;
  Printf.printf "packed size (XDR): %u\n" @@ Bytes.length packed_xdr

let unpacked_native = Nvlist.unpack packed_native
let unpacked_xdr = Nvlist.unpack packed_xdr
let flist = Nvlist.dup blist
let glist = Nvlist.alloc ()

let () =
  Nvlist.add_string glist "some" "text";
  Nvlist.add_string glist "more" "data";
  let p = Option.get @@ Nvlist.next_nvpair unpacked_xdr None in
  Nvlist.add_nvpair glist p

let merged = Nvlist.merge flist glist

let () =
  Nvlist.remove merged "some";
  let more = Option.get @@ Nvlist.lookup_nvpair merged "more" in
  Nvlist.remove_nvpair merged more

let () =
  assert (None = Nvlist.lookup_boolean merged "not there");
  assert (None = Nvlist.lookup_boolean_value merged "not there");
  assert (None = Nvlist.lookup_byte merged "not there");
  assert (None = Nvlist.lookup_int8 merged "not there");
  assert (None = Nvlist.lookup_uint8 merged "not there");
  assert (None = Nvlist.lookup_int16 merged "not there");
  assert (None = Nvlist.lookup_uint16 merged "not there");
  assert (None = Nvlist.lookup_int32 merged "not there");
  assert (None = Nvlist.lookup_uint32 merged "not there");
  assert (None = Nvlist.lookup_int64 merged "not there");
  assert (None = Nvlist.lookup_uint64 merged "not there");
  assert (None = Nvlist.lookup_string merged "not there");
  assert (None = Nvlist.lookup_nvlist merged "not there");
  assert (None = Nvlist.lookup_boolean_array merged "not there");
  assert (None = Nvlist.lookup_byte_array merged "not there");
  assert (None = Nvlist.lookup_int8_array merged "not there");
  assert (None = Nvlist.lookup_uint8_array merged "not there");
  assert (None = Nvlist.lookup_int16_array merged "not there");
  assert (None = Nvlist.lookup_uint16_array merged "not there");
  assert (None = Nvlist.lookup_int32_array merged "not there");
  assert (None = Nvlist.lookup_uint32_array merged "not there");
  assert (None = Nvlist.lookup_int64_array merged "not there");
  assert (None = Nvlist.lookup_uint64_array merged "not there");
  assert (None = Nvlist.lookup_string_array merged "not there");
  assert (None = Nvlist.lookup_nvlist_array merged "not there");
  assert (None = Nvlist.lookup_hrtime merged "not there");
  assert (None = Nvlist.lookup_double merged "not there");
  assert (None = Nvlist.lookup_nvpair merged "not there")

let () =
  assert (Nvlist.exists clist "hrtime");
  Nvlist.remove clist "hrtime";
  assert (Nvlist.empty clist)

let () =
  let a = Option.get @@ Nvlist.lookup_nvlist unpacked_native "nvlist" in
  assert (Some true = Nvlist.lookup_boolean a "boolean");
  assert (Some false = Nvlist.lookup_boolean_value a "boolean_value");
  assert (Some 0xff = Nvlist.lookup_byte a "byte");
  assert (Some (-12) = Nvlist.lookup_int8 a "int8");
  assert (Some 255 = Nvlist.lookup_uint8 a "uint8");
  assert (Some (-1234) = Nvlist.lookup_int16 a "int16");
  assert (Some 0xffff = Nvlist.lookup_uint16 a "uint16");
  assert (Some Int32.min_int = Nvlist.lookup_int32 a "int32");
  assert (Some 0xffff_ffffl = Nvlist.lookup_uint32 a "uint32");
  assert (Some Int64.min_int = Nvlist.lookup_int64 a "int64");
  assert (Some 0xffff_ffff_ffff_ffffL = Nvlist.lookup_uint64 a "uint64");
  assert (Some "this is a string" = Nvlist.lookup_string a "string");
  let b = unpacked_native in
  assert (
    Some [| true; false; true; false |]
    = Nvlist.lookup_boolean_array b "boolean_array");
  assert (
    Some (Bytes.of_string "these are bytes")
    = Nvlist.lookup_byte_array b "byte_array");
  assert (
    Some [| 0x7f; 0x00; -1; 123 |] = Nvlist.lookup_int8_array b "int8_array");
  assert (Some [| 0xff; 0x00 |] = Nvlist.lookup_uint8_array b "uint8_array");
  assert (
    Some [| 1234; -1234; 0x1234; -0x1234 |]
    = Nvlist.lookup_int16_array b "int16_array");
  assert (
    Some [| 0xffff; 0x0000 |] = Nvlist.lookup_uint16_array b "uint16_array");
  assert (
    Some [| Int32.max_int; Int32.min_int |]
    = Nvlist.lookup_int32_array b "int32_array");
  assert (
    Some [| 0xffff_ffffl; 0x0000_0000l |]
    = Nvlist.lookup_uint32_array b "uint32_array");
  assert (
    Some [| Int64.max_int; Int64.min_int |]
    = Nvlist.lookup_int64_array b "int64_array");
  assert (
    Some [| 0xffff_ffff_ffff_ffffL; 0x0000_0000_0000_0000L |]
    = Nvlist.lookup_uint64_array b "uint64_array");
  assert (
    Some [| "these"; "are"; "strings" |]
    = Nvlist.lookup_string_array b "string_array");
  let c, d, e =
    match Nvlist.lookup_nvlist_array b "nvlist_array" with
    | Some [| c; d; e |] -> (c, d, e)
    | _ -> failwith "match failed"
  in
  assert (Some 12345L = Nvlist.lookup_hrtime c "hrtime");
  assert (Some 123.45 = Nvlist.lookup_double d "double");
  assert (Some true = Nvlist.lookup_boolean e "boolean");
  assert (Some false = Nvlist.lookup_boolean_value e "boolean_value");
  assert (Some 0xff = Nvlist.lookup_byte e "byte");
  assert (Some (-12) = Nvlist.lookup_int8 e "int8");
  assert (Some 255 = Nvlist.lookup_uint8 e "uint8");
  assert (Some (-1234) = Nvlist.lookup_int16 e "int16");
  assert (Some 0xffff = Nvlist.lookup_uint16 e "uint16");
  assert (Some Int32.min_int = Nvlist.lookup_int32 e "int32");
  assert (Some 0xffff_ffffl = Nvlist.lookup_uint32 e "uint32");
  assert (Some Int64.min_int = Nvlist.lookup_int64 e "int64");
  assert (Some 0xffff_ffff_ffff_ffffL = Nvlist.lookup_uint64 e "uint64");
  assert (Some "this is a string" = Nvlist.lookup_string e "string")

let () =
  let pair = Option.get @@ Nvlist.lookup_nvpair unpacked_xdr "nvlist" in
  let a = Nvpair.value_nvlist pair in
  let _ = Option.get @@ Nvlist.lookup_nvpair a "boolean" in
  let pair = Option.get @@ Nvlist.lookup_nvpair a "boolean_value" in
  assert (false = Nvpair.value_boolean_value pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair a "byte" in
  assert (0xff = Nvpair.value_byte pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair a "int8" in
  assert (-12 = Nvpair.value_int8 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair a "uint8" in
  assert (255 = Nvpair.value_uint8 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair a "int16" in
  assert (-1234 = Nvpair.value_int16 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair a "uint16" in
  assert (0xffff = Nvpair.value_uint16 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair a "int32" in
  assert (Int32.min_int = Nvpair.value_int32 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair a "uint32" in
  assert (0xffff_ffffl = Nvpair.value_uint32 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair a "int64" in
  assert (Int64.min_int = Nvpair.value_int64 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair a "uint64" in
  assert (0xffff_ffff_ffff_ffffL = Nvpair.value_uint64 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair a "string" in
  assert ("this is a string" = Nvpair.value_string pair);
  let b = unpacked_xdr in
  let pair = Option.get @@ Nvlist.lookup_nvpair b "boolean_array" in
  assert ([| true; false; true; false |] = Nvpair.value_boolean_array pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair b "byte_array" in
  assert (Bytes.of_string "these are bytes" = Nvpair.value_byte_array pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair b "int8_array" in
  assert ([| 0x7f; 0x00; -1; 123 |] = Nvpair.value_int8_array pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair b "uint8_array" in
  assert ([| 0xff; 0x00 |] = Nvpair.value_uint8_array pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair b "int16_array" in
  assert ([| 1234; -1234; 0x1234; -0x1234 |] = Nvpair.value_int16_array pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair b "uint16_array" in
  assert ([| 0xffff; 0x0000 |] = Nvpair.value_uint16_array pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair b "int32_array" in
  assert ([| Int32.max_int; Int32.min_int |] = Nvpair.value_int32_array pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair b "uint32_array" in
  assert ([| 0xffff_ffffl; 0x0000_0000l |] = Nvpair.value_uint32_array pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair b "int64_array" in
  assert ([| Int64.max_int; Int64.min_int |] = Nvpair.value_int64_array pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair b "uint64_array" in
  assert (
    [| 0xffff_ffff_ffff_ffffL; 0x0000_0000_0000_0000L |]
    = Nvpair.value_uint64_array pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair b "string_array" in
  assert ([| "these"; "are"; "strings" |] = Nvpair.value_string_array pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair b "nvlist_array" in
  let c, d, e =
    match Nvpair.value_nvlist_array pair with
    | [| c; d; e |] -> (c, d, e)
    | _ -> failwith "match failed"
  in
  let pair = Option.get @@ Nvlist.lookup_nvpair c "hrtime" in
  assert (12345L = Nvpair.value_hrtime pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair d "double" in
  assert (123.45 = Nvpair.value_double pair);
  let _ = Option.get @@ Nvlist.lookup_nvpair e "boolean" in
  let pair = Option.get @@ Nvlist.lookup_nvpair e "boolean_value" in
  assert (false = Nvpair.value_boolean_value pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair e "byte" in
  assert (0xff = Nvpair.value_byte pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair e "int8" in
  assert (-12 = Nvpair.value_int8 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair e "uint8" in
  assert (255 = Nvpair.value_uint8 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair e "int16" in
  assert (-1234 = Nvpair.value_int16 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair e "uint16" in
  assert (0xffff = Nvpair.value_uint16 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair e "int32" in
  assert (Int32.min_int = Nvpair.value_int32 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair e "uint32" in
  assert (0xffff_ffffl = Nvpair.value_uint32 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair e "int64" in
  assert (Int64.min_int = Nvpair.value_int64 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair e "uint64" in
  assert (0xffff_ffff_ffff_ffffL = Nvpair.value_uint64 pair);
  let pair = Option.get @@ Nvlist.lookup_nvpair e "string" in
  assert ("this is a string" = Nvpair.value_string pair)

let () =
  let pair = Option.get @@ Nvlist.lookup_nvpair dlist "double" in
  assert ("double" = Nvpair.name pair);
  assert (Nvpair.Double = Nvpair.data_type pair)

let () =
  let prev = Nvlist.prev_nvpair dlist None in
  assert (Option.is_some prev);
  assert (None = Nvlist.prev_nvpair dlist prev)

let () =
  Nvlist.free alist;
  Nvlist.free blist;
  Nvlist.free clist;
  Nvlist.free dlist;
  Nvlist.free elist;
  Nvlist.free flist;
  Nvlist.free glist;
  Nvlist.free unpacked_native;
  Nvlist.free unpacked_xdr
