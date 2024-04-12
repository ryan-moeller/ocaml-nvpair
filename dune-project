(lang dune 3.14)

(name nvpair)

(generate_opam_files true)

(source
 (github ryan-moeller/ocaml-nvpair))

(authors "Ryan Moeller")

(maintainers "Ryan Moeller")

(license LICENSE)

(documentation https://url/to/documentation)

(package
 (name nvpair)
 (synopsis "Bindings for OpenZFS libnvpair(3) name-value pair library")
 (description "Functions for managing name-value pairs")
 (depends ocaml dune)
 (tags
  (topics OpenZFS ZFS nvpair nvlist)))

; See the complete stanza docs at https://dune.readthedocs.io/en/stable/dune-files.html#dune-project
