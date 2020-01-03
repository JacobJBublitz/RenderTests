# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

import re
from argparse import ArgumentParser
from os import environ

CONFIG_REGEX = re.compile(r"@(\w+)@")

parser = ArgumentParser()

parser.add_argument('input_file', type=str)
parser.add_argument('output_file', type=str)

args = parser.parse_args()

with open(args.input_file, encoding='utf8') as in_file, open(args.output_file, 'w', encoding='utf8') as out_file:
    for line in in_file.readlines():
        start_pos = 0
        while start_pos < len(line):
            match = CONFIG_REGEX.search(line, start_pos)
            if not match:
                break
            value = environ[match.group(1)]
            line = line[:match.start()] + value + line[match.end():]
            start_pos = match.end()
        out_file.write(line)
