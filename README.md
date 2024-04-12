Nvpair
------

OCaml bindings for OpenZFS libnvpair.

linbvpair provides functions for manipulating name-value pair lists.

Most nvlist functions are implemented here. Operations on nvpairs are left for future work.


Usage
-----

Example manipulating the ZFS zpool cache file on a system with a pool named "storage":

```ocaml
open Nvpair;;
let pools =
  let f = In_channel.open_bin "/etc/zfs/zpool.cache" in
  let len = Int64.to_int @@ In_channel.length f in
  let buf = Bytes.create len in
  Option.get @@ In_channel.really_input f buf 0 len;
  Nvlist.unpack buf;;
let storage = Option.get @@ Nvlist.lookup_nvlist pools "storage";;
let vdev_tree = Option.get @@ Nvlist.lookup_nvlist storage "vdev_tree";;
let vdevs = Option.get @@ Nvlist.lookup_nvlist_array vdev_tree "children";;
let children = Option.get @@ Nvlist.lookup_nvlist_array (Array.get vdevs 0) "children";;
let path = Option.get @@ Nvlist.lookup_string (Array.get children 0) "path";;
```
