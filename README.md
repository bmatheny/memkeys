# memkeys

Show your memcache key usage in realtime. This was originally inspired by
[mctop](https://github.com/etsy/mctop) which is written in Ruby. I found that
under load mctop would drop between 50 and 75 percent of packets. Under the same
load memkeys will typically drop less than 3 percent of packets.

## Do on Monday

 1. Thread for main report interface - Done
 2. Implement text (CSV) report as variation on printStats logic - Done
 3. Implement very simple file based logging so we don't mess up the curses UI - Done
   * CLI specified -l /path/to/file - Done
   * If so, update the Logger::log method - Done
 4. Start on curses UI

This keeps packet captures fast since the mqueue implementation allows fast
buffering of data points. Stats can read from it as quickly as it can.

Report has a handle on Stats which calls getLeaders periodically to get data
needed for generating a report. This won't be super fast since we copy the
entire map into a vector. That's ok, optimizing for packet capture and data
accuracy over speed of display.

## Command line options

Docs here.

## Development

Install gperftools and gperftools-devel if you want to build with
`--enable-profiling`.

You will need libpcap-devel.

You will need libpcrecpp.

# License

Copyright 2013 Blake Matheny.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
