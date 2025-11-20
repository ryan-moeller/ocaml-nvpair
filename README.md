OCaml Bindings for OpenZFS libnvpair(3)
=======================================

[![14.3-RELEASE Build Status](https://api.cirrus-ci.com/github/ryan-moeller/ocaml-nvpair.svg?branch=main&task=releases/amd64/14.3-RELEASE)](https://cirrus-ci.com/github/ryan-moeller/ocaml-nvpair)
[![15.0-RC2 Build Status](https://api.cirrus-ci.com/github/ryan-moeller/ocaml-nvpair.svg?branch=main&task=releases/amd64/15.0-RC2)](https://cirrus-ci.com/github/ryan-moeller/ocaml-nvpair)

linbvpair provides functions for manipulating name-value pair lists.

Most nvlist and nvpair functions are implemented.

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
