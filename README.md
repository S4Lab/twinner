# Twinner

Twinner is a deobfuscation and unpacking framework.
It inspects executable binaries, instruments them using the PIN Intel framework for binary instrumentation, analyzes executed assembly instructions through a concolic execution, models the behavior of the program as a set of symbolic expressions and constraints, and recodes it as twincode, a compilable C program with simplified logic.
The framework allows automatic deobfuscation of the virtualization obfuscated binaries on 64bit Linux and 32bit Windows.
The latest version is 0.24.0 and is a work in progress, so if you are not familiar with the context, you should wait for the v1.0.0 release. Otherwise, have happy hacking :) There is no regular release schedule and every version is released when it is ready.

## Installation

Use make files.

## Usage

Run Twinner binary with --help for details.

## License
    Copyright © 2013-2016 Behnam Momeni

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see {http://www.gnu.org/licenses/}.
