Nvpair
------

OCaml bindings for OpenZFS libnvpair.

linbvpair provides functions for manipulating name-value pair lists.

Most nvlist functions are implemented here. Operations on nvpairs are left for future work.


Usage
-----

```ocaml
open Nvpair;;
let f = In_channel.open_bin "/etc/zfs/zpool.cache";;
let len = Int64.to_int @@ In_channel.length f;;
let buf = Bytes.create len;;
let () = Option.get @@ In_channel.really_input f buf 0 len;;
let pools = Nvlist.unpack buf;;
let storage = Option.get @@ Nvlist.lookup_nvlist pools "storage";;
let vdev_tree = Option.get @@ Nvlist.lookup_nvlist storage "vdev_tree";;
let vdevs = Option.get @@ Nvlist.lookup_nvlist_array vdev_tree "children";;
let children = Option.get @@ Nvlist.lookup_nvlist_array (Array.get vdevs 0) "children";;
let path = Option.get @@ Nvlist.lookup_string (Array.get children 0) "path";;
```
