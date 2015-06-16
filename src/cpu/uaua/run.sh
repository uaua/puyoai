#!/bin/bash
cd "$(dirname "$0")"
exec ./uaua "$@" 2> uaua.err

