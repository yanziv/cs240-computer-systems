#!/usr/bin/env python3

import json
import os
import shutil
import subprocess

BIN = './adventure.bin'
BINDIR = '.executables'
BIN_FORMAT = '{}.bin'

with open('.team.json') as f:
    teamblob = json.load(f)
    pass

firstid = list(sorted(teamblob[-1]['team']))[0]

shutil.copy2(os.path.join(BINDIR, BIN_FORMAT.format(firstid)), BIN)
# shutil.rmtree(BINDIR)

try:
    subprocess.run(['git', 'add', BIN], check = True) #, BINDIR])
    subprocess.run(['git', 'commit', '-m', 'selected executable by id {}'.format(firstid), BIN], check = True)
    subprocess.run(['git', 'push', '--quiet'], check = True)
except subprocess.CalledProcessError as e:
    print('An error occurred. Please send this output to your instructor.')
    raise e

try:
    subprocess.run(['git', 'rm', __file__], check = True)
    subprocess.run(['git', 'commit', '-m', 'removed select script', __file__], check = True)
    subprocess.run(['git', 'push', '--quiet'], check = True)
except subprocess.CalledProcessError as e:
    print('An error occurred. Please send this output to your instructor.')
    raise e

print("Created your executable as: {}".format(BIN))

