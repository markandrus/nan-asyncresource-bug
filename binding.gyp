{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "addon.cc",
        "bar.cc",
        "foo.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"]
    }
  ]
}
