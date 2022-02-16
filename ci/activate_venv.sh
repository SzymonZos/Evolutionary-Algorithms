#!/usr/bin/env bash
set -Eeuo pipefail

pip2 install --upgrade pip
pip2 install virtualenv
virtualenv -p "$(which python2)" venv
. ./venv/bin/activate
pip2 install -r requirements.txt
