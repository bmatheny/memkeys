# mCtop

A top like tool for inspecting memcache key values in realtime.

This is a clone of https://github.com/etsy/mctop written in C.

## Do on Monday

 1. Thread for main report interface
 2. Implement text (CSV) report as variation on printStats logic
 3. Implement very simple file based logging so we don't mess up the curses UI
   * CLI specified -l /path/to/file
   * If so, update the Logger::log method
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
