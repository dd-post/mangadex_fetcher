# mangadex_fetcher

A simple tool for scrapping from mangadex.org using their [API](https://mangadex.org/api/). Supports writing to plain directories or cbz.

### Building
---
*mangadex_fetcher* requires standard `c++17` and 3 dependencies:

- [nlohmann::json](https://github.com/nlohmann/json)
- [libcurl](https://curl.haxx.se/libcurl/)
- [libarchive](https://www.libarchive.org/)

These libraries should be available in your distribution's package manager if they aren't already installed.

Once the dependencies are squared away, do the dance:

``` bash
$ git clone https://github.com/dd-post/mangadex_fetcher.git
$ cd mangadex_fetcher
$ ./configure
$ make
$ make install
```

At this point *mangadex_fetcher* should be ready to use.

### Usage
---
The program supplies some basic info to help get you started:

``` bash
Usage: mangadex_fetcher [OPTION...] URL
Simple tool for scrapping manga from mangadex.org using their API (https://mangadex.org/api/). Supports writing to plain directories or cbz.

  -d, --output-dir=PATH                 Change the output directory for the downloaded chapters to PATH. Defaults to name of manga
  -e, --end-chapter=END_CHAP            Specify the chapter to end fetch at. Use indices from --list-chapters output
  -f, --force                           Force overwrites of files. Be careful with this option as it will overwrite files if they share the same name
  -g, --group=GROUP                     When multiple groups have translated a chapter, prefer this group's translation. Can be specified multiple times to form a priority list
  -l, --language=LANG_CODE              Fetch chapters only matching this language
  -L, --list-chapters                   List all chapters and generate indices to reference them. Use --language and/or --group to narrow down output
  -q, --quiet                           Suppress all standard output
  -r, --most-recent                     Get only the most recent version of a chapter. Use --language to narrow down output
  -s, --start-chapter=START_CHAP        Specify the chapter to start fetch from. Use indices from --list-chapters output
  -S, --single-chapter=CHAP             Specify a single chapter to download. Use indices from --list-chapters output
  -t, --output-type=TYPE                Define how output will be saved. TYPE can be dir (default) or cbz
  -v, --volume                          Write out chapters in groups by volume
  -?, --help                            Give this help list
      --usage                           Give a short usage message
```

#### Example: Generating a Listing
We can get a listing of chapters to be downloaded by using the `--list-chapters` or `-L` option:

``` bash
$ mangadex_fetcher -L https://mangadex.org/title/22369/jahy-sama-wa-kujikenai
Using API url: https://mangadex.org/api/manga/22369
Title: Jahy-sama wa Kujikenai!

Idx   | Vol       | Chap       | Group                | Lang   | Title
========================================================================================================================
00000 |         0 |       0000 | ScarlettScans        | gb     | Oneshot
00001 |         0 |       0000 | Mangaryn             | ru     | Oneshot
00002 |         0 |       0000 | Black Stone          | sa     | Oneshot
00003 |         1 |       0001 | ScarlettScans        | gb     | Jahy-Sama And The Part-Time Job
00004 |         1 |       0001 | Mangaryn             | ru     | Джахи-сама и Подработка
00005 |         1 |       0001 | Genjitsu             | fr     | <no title>
00006 |         1 |       0002 | ScarlettScans        | gb     | Jahy-sama and the Subordinate
00007 |         1 |       0002 | Mangaryn             | ru     | Джахи-сама и Элитный Дом
00008 |         1 |       0002 | Genjitsu             | fr     | <no title>
(....)
```
The order is determined by chapter number, where any chapter missing a number is assumed to be chapter `0`.

Typically, we're only interested in one language. We can use the `--language` or `-l` (watch your case!) flag to denote that we want, for instance, English chapters only:

``` bash
$ mangadex_fetcher -L -l gb https://mangadex.org/title/22369/jahy-sama-wa-kujikenai
Using API url: https://mangadex.org/api/manga/22369
Title: Jahy-sama wa Kujikenai!

Idx   | Vol       | Chap       | Group                | Lang   | Title
========================================================================================================================
00000 |         0 |       0000 | ScarlettScans        | gb     | Oneshot
00001 |         1 |       0001 | ScarlettScans        | gb     | Jahy-Sama And The Part-Time Job
00002 |         1 |       0002 | ScarlettScans        | gb     | Jahy-sama and the Subordinate
00003 |         1 |       0003 | ScarlettScans        | gb     | Jahy-sama and the Precious Gem
00004 |         1 |       0004 | ScarlettScans        | gb     | Jahy-sama and the Landlady
00005 |         1 |       0005 | ScarlettScans        | gb     | Jahy-sama and the Mountain
00006 |         1 |     0005.5 | Orchesc/a/ns         | gb     | Jahy-sama and Her Daily Life
00007 |         2 |       0006 | ScarlettScans        | gb     | <no title>
00008 |         2 |       0007 | ScarlettScans        | gb     | <no title>
(....)
```

Sometimes there is a case where multiple scanlators have translated a chapter. In this instance, both `Orchesc/a/ns` and `Tempor/a/ry scans` have translated chapter `32`:

``` bash
(....)
00033 |         4 |       0031 | Tempor/a/ry scans    | gb     | Jahy-Sama and the Master Part Timer
00034 |         4 |       0032 | Tempor/a/ry scans    | gb     | Jahy-Sama and the Runaway Landlady
00035 |         4 |       0032 | Orchesc/a/ns         | gb     | Jahy-sama and the Landlady Who Ran from Home
00036 |         5 |       0033 | Orchesc/a/ns         | gb     | Jahy-sama and Tomatoes
(....)
```

We can select one of these groups by using the `--group` or `-g` flag. If we want to use `Orchesc/a/ns` version of this chapter:

``` bash
$ mangadex_fetcher -L -l gb -g Orchesc/a/ns https://mangadex.org/title/22369/jahy-sama-wa-kujikenai
Using API url: https://mangadex.org/api/manga/22369
Title: Jahy-sama wa Kujikenai!

Idx   | Vol       | Chap       | Group                | Lang   | Title
========================================================================================================================
(....)
00033 |         4 |       0031 | Tempor/a/ry scans    | gb     | Jahy-Sama and the Master Part Timer
00034 |         4 |       0032 | Orchesc/a/ns         | gb     | Jahy-sama and the Landlady Who Ran from Home
00035 |         5 |       0033 | Orchesc/a/ns         | gb     | Jahy-sama and Tomatoes
(....)
```
The conflict is resolved. Unresolved conflicts are not fatal however. If left as before, the chapter will simply be downloaded from both sources as `032 - Orchesc/a/ns` and `032 - Tempor/a/ry scans`.

`--group` can be specified multiple times to produce a priority list where priority is determined by specification order. This will attempt to select the groups with the highest priority first before selecting lower priorities. If no group is found for a chapter, the most recent version of said chapter will be selected.

If we only want one version of the chapter and don't particularly care which scanlator it comes from, we can use the `--most-recent` or `-r` flag. This will simply take the most recent version of any chapter.

Lastly, it is possible to only download a segment of the chapters using `--start-chapter` and `--end-chapter` (or `-s` and `-e` respectively). Using the indices from the left most column, we can specify where we should start and where we should end. This will not affect the listing, but will cause the downloader to skip any chapters that do not fall within this range. Be careful to use the indices and not the actual chapter number as these numbers are NOT equivalent. If only a single chapter is desired, use the `--single-chapter` or `-S` option to specify that chapter. This is equivalent to using `-s` and `-e` with the same index.

#### Example: Downloading
Once we have the listing of the chapters we want, it is time to download them. To perform the download, simply remove the `-L` flag from the command. This will begin the download process:

``` bash
$ mangadex_fetcher -l gb -g Orchesc/a/ns https://mangadex.org/title/22369/jahy-sama-wa-kujikenai
Using API url: https://mangadex.org/api/manga/22369
Title: Jahy-sama wa Kujikenai!

Fetching chapter: 0000 - ScarlettScans
Downloading: https://s2.mangadex.org/data/9461c2987335e34df739758230c32d3b/p1.jpg -> 000.jpg...   434 kB: OK.
Downloading: https://s2.mangadex.org/data/9461c2987335e34df739758230c32d3b/p2.jpg -> 001.jpg...   414 kB: OK.
Downloading: https://s2.mangadex.org/data/9461c2987335e34df739758230c32d3b/p3.jpg -> 002.jpg...   342 kB: OK.
Downloading: https://s2.mangadex.org/data/9461c2987335e34df739758230c32d3b/p4.jpg -> 003.jpg...   348 kB: OK.
Downloading: https://s2.mangadex.org/data/9461c2987335e34df739758230c32d3b/p5.jpg -> 004.jpg...   421 kB: OK.
(....)
```
By default, all chapters are downloaded to a sub-directory with the same name as the title. If you wish to change this, use the `--output-path` or `-d` option to specify another path.

One of the more useful options is `--volume` or `-v`. This will attempt to group chapters by their respective volumes. If a chapter does not have a volume, it is assumed to be volume `0`. In this case, the directory structure ends up as:
``` bash
$ tree "Jahy-sama wa Kujikenai!" -d
Jahy-sama wa Kujikenai!/
├── 00
│   └── 0000 - ScarlettScans
├── 01
│   ├── 0001 - ScarlettScans
│   ├── 0002 - ScarlettScans
│   ├── 0003 - ScarlettScans
(....)
```

It is possible to alter the type of output generated with the `--output-type` or `-t` option. Currently, the only available option other than the default `dir` is `cbz`, which will package the downloaded files in `.cbz` archives. When combined with `-v` this will generate a number of archives, one for each volume. This option is ideal for transferring to an e-reader or tablet for reading later.

Combining all these, the following will download all English chapters with a preference for translation group `Orchesc/a/ns`, organize them by volume, and create `.cbz` archives from the results:

``` bash
$ mangadex_fetcher -l gb -g Orchesc/a/ns -v -t cbz https://mangadex.org/title/22369/jahy-sama-wa-kujikenai
Using API url: https://mangadex.org/api/manga/22369
Title: Jahy-sama wa Kujikenai!

Fetching chapter: 0000 - ScarlettScans
Downloading: https://s2.mangadex.org/data/9461c2987335e34df739758230c32d3b/p1.jpg -> 000.jpg...   434 kB: OK.
Downloading: https://s2.mangadex.org/data/9461c2987335e34df739758230c32d3b/p2.jpg -> 001.jpg...   414 kB: OK.
Downloading: https://s2.mangadex.org/data/9461c2987335e34df739758230c32d3b/p3.jpg -> 002.jpg...   342 kB: OK.
Downloading: https://s2.mangadex.org/data/9461c2987335e34df739758230c32d3b/p4.jpg -> 003.jpg...   348 kB: OK.
Downloading: https://s2.mangadex.org/data/9461c2987335e34df739758230c32d3b/p5.jpg -> 004.jpg...   421 kB: OK.
(....)

Collecting for archives...

Packaging volume: 00
00/0000 - ScarlettScans/005.jpg
00/0000 - ScarlettScans/009.jpg
00/0000 - ScarlettScans/013.jpg
00/0000 - ScarlettScans/050.jpg
00/0000 - ScarlettScans/015.jpg
00/0000 - ScarlettScans/035.jpg
00/0000 - ScarlettScans/055.jpg
00/0000 - ScarlettScans/037.jpg
00/0000 - ScarlettScans/041.jpg
00/0000 - ScarlettScans/053.jpg
(....)

Finished download of title 'Jahy-sama wa Kujikenai!'.
```

Finally, it should be noted that none of the above operations will overwrite any existing data. To force a redownload or a rebuild of the `.cbz` archives, use the `--force` or `-f` flag to overwrite. Be careful with this option however as it will clobber any files with conflicting names. This is unlikely but please ensure that your data is not in the line of fire before using.

### Miscellaneous Topics
---
 - Thanks to all library developers for creating such useful tools.
 - Additional thanks to MangaDex for making their API public. If you've got an extra PC and the bandwidth, please support their [MangaDex@Home Project](https://mangadex.org/thread/262074).
 - If you find a bug or would like to request a feature, please create an issue.
 - For anyone who looks at the code, sorry for the mess. It's like a garbled mixture of C and C++.
 - It should be pointed out that this was developed for Linux machines. It is likely that you can get this working on Windows by running it in WSL or by replacing Linux-specific for Windows code, but I have nothing to check that on.


