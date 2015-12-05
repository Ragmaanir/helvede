
```
Dir.list.column["size"]

Dir.list # internal format
{
  header: [
    [
      {name: "filename", format: "string"},
      {name: "type", format: "enum", values: ["file", "dir"]},
      {name: "size", format: "integer", unit: "byte"},
      {name: "permissions", format: "flags"}
    ]
  ],
  body: [
    ["data", "dir", 1130],
    ["xyz.dat", "file", 4532314]
  ]
}
```
