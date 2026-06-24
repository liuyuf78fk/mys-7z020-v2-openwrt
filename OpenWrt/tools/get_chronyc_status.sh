#!/bin/sh

case "$1" in
    1)
        chronyc tracking
        ;;
    2)
        chronyc sources -v
        ;;
    3)
        chronyc sourcestats -v
        ;;
    *)
        echo "Usage: $0 {1|2|3}"
        echo "  1 - chronyc tracking"
        echo "  2 - chronyc sources -v"
        echo "  3 - chronyc sourcestats -v"
        exit 1
        ;;
esac
