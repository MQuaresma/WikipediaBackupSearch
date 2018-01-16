#!/bin/bash

rm -rf *.txt target/

mvn clean package

mycommand="java -jar target/project2-1.0-SNAPSHOT-jar-with-dependencies.jar snapshot_dec16 snapshot_jan17 snapshot_fev17"$@
echo $mycommand
$mycommand
