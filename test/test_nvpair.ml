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
      print_endline @@ Nvpair.name p;
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
  Array.iter (fun child -> print_endline @@ Option.get @@ Nvlist.lookup_string child "path") raidz_children
