#!/bin/bash

mkdir -p Db || exit 1
cd build || exit 1

sqlite3 chat.db <schema.sql
