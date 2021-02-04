# qtcre

A QT embedded resource export helper.

```
          __
  _______/  |_  ___________   ____
 / ____/\   __\/ ___\_  __ \_/ __ \
< <_|  | |  | \  \___|  | \/\  ___/
 \__   | |__|  \___  >__|    \___  >
    |__|           \/            \/   qtcre v1.0.0

usage: ./qtcre <executable path> <image base>
```

## Example

```
> qtcre ./gfclient.exe 0x400000
          __
  _______/  |_  ___________   ____
 / ____/\   __\/ ___\_  __ \_/ __ \
< <_|  | |  | \  \___|  | \/\  ___/
 \__   | |__|  \___  >__|    \___  >
    |__|           \/            \/   qtcre v1.0.0

resource_struct_ptr: 0x2b49c0
resource_data_ptr: 0x25b728
resource_name_ptr: 0x2b4870

- certs [dir, children: 1, last_modified: 0]
 - client.p12 [file, country: 0, language: 1, data_offset: 0x0, last_modified: 1611943857035]
- Client [dir, children: 2, last_modified: 0]
 - images [dir, children: 8, last_modified: 0]
  - +goe [dir, children: 1, last_modified: 0]
   - game.ico [file, country: 0, language: 1, data_offset: 0x2a539, last_modified: 1611943857160]
  - +tera [dir, children: 1, last_modified: 0]
   - game.ico [file, country: 0, language: 1, data_offset: 0x1163f, last_modified: 1611943857207]
  - +runesofmagic [dir, children: 1, last_modified: 0]
   - game.ico [file, country: 0, language: 1, data_offset: 0xd0a2, last_modified: 1611943857191]
  - +metin2 [dir, children: 1, last_modified: 0]
   - game.ico [file, country: 0, language: 1, data_offset: 0x4cd61, last_modified: 1611943857175]
  - +soulworker [dir, children: 1, last_modified: 0]
   - game.ico [file, country: 0, language: 1, data_offset: 0x2e02b, last_modified: 1611943857207]
  - +nostale [dir, children: 1, last_modified: 0]
   - game.ico [file, country: 0, language: 1, data_offset: 0x4ea53, compressed, last_modified: 1611943857191]
  - +sample_game_client [dir, children: 1, last_modified: 0]
   - game.ico [file, country: 0, language: 1, data_offset: 0xdd64, compressed, last_modified: 1611943857191]
  - splashscreen.bmp [file, country: 0, language: 1, data_offset: 0x4d09, compressed, last_modified: 1611943857207]
 - appicon [file, country: 0, language: 1, data_offset: 0x12d1, compressed, last_modified: 1611943857207]

>
```
