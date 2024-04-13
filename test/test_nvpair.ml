open Nvpair

let pools =
  let f = In_channel.open_bin "zpool.cache" in
  let len = Int64.to_int @@ In_channel.length f in
  let buf = Bytes.create len in
  Option.get @@ In_channel.really_input f buf 0 len;
  Nvlist.unpack buf

let () =
  let rec loop pair =
    match Nvlist.next_nvpair pools pair with
    | None -> ()
    | Some p ->
      Printf.printf "\t%s\n" @@ Nvpair.name p;
      loop @@ Some p
  in
  print_endline "Pools in cache file:";
  loop None

let storage = Option.get @@ Nvlist.lookup_nvlist pools "storage"

let () =
  let vdev_tree = Option.get @@ Nvlist.lookup_nvlist storage "vdev_tree" in
  let top_vdevs = Option.get @@ Nvlist.lookup_nvlist_array vdev_tree "children" in
  let raidz = Array.get top_vdevs 0 in
  let raidz_children = Option.get @@ Nvlist.lookup_nvlist_array raidz "children" in
  print_endline "storage pool vdevs:";
  Array.iter (fun child -> Printf.printf "\t%s\n" @@ Option.get @@ Nvlist.lookup_string child "path") raidz_children

let () =
  let version = Option.get @@ Nvlist.lookup_uint64 storage "version" in
  let txg = Option.get @@ Nvlist.lookup_uint64 storage "txg" in
  let pool_guid = Option.get @@ Nvlist.lookup_uint64 storage "pool_guid" in
  let hostid = Option.get @@ Nvlist.lookup_uint64 storage "hostid" in
  let hostname = Option.get @@ Nvlist.lookup_string storage "hostname" in
  print_endline "storage pool details:";
  Printf.printf "\tversion: %Lu\n" version;
  Printf.printf "\ttxg: %Lu\n" txg;
  Printf.printf "\tpool_guid: %Lu\n" pool_guid;
  Printf.printf "\thostid: %Lu\n" hostid;
  Printf.printf "\thostname: %s\n" hostname

let () =
  let features_for_read = Option.get @@ Nvlist.lookup_nvlist storage "features_for_read" in
  let rec loop pair =
    match Nvlist.next_nvpair features_for_read pair with
    | None -> ()
    | Some p ->
      Printf.printf "\t%s\n" @@ Nvpair.name p;
      loop @@ Some p
  in
  print_endline "storage pool features required for read:";
  loop None
