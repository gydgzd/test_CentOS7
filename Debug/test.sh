#!/bin/bash
serverName="$1"
PID=$(ps -C $serverName|grep $serverName |grep -v grep |awk ' { print $1 }')
echo "$line"

echo "$serverName"
echo "$PID"
echo "hi"
echo $(pidof java)
